#include "read_adc.h"

double currentHumidity = 0;
double currentTemperature = 0;

static unsigned int bufferAdcPanel[BUFFER_ADC_SIZE];
static unsigned int bufferAdcBateria[BUFFER_ADC_SIZE];
static unsigned int bufferAdcFoto[BUFFER_ADC_SIZE];
static unsigned int bufferAdcHumedad[BUFFER_ADC_SIZE];

static struct pt_sem mutexAdc;

static struct pt ptAdcHum;
static struct pt ptAdcBat;
static struct pt ptAdcFoto;
static struct pt ptAdcTemp;


double adcVdcFoto = 0;
double adcBattery = 0;
double adcTemp = 0;
double adcHum = 0;


extern unsigned char actdisplay;

/*
 * Funcion de lectura de sensor de humedad
 */
static int taskAdcHum(struct pt *pt) {
    static unsigned long delay;
    static unsigned char i;
    static unsigned long summatory;
    static unsigned int currentAdc;



    PT_BEGIN(pt);

    PT_SEM_WAIT(pt, &mutexAdc);
    ADCC_StartConversion(ADC_HUM);
    PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
    currentAdc = ADCC_GetConversionResult();
    PT_SEM_SIGNAL(pt, &mutexAdc);

    Nop();
    for (i = 0; i < BUFFER_ADC_SIZE; i++) {
        bufferAdcHumedad[i] = currentAdc;
    }

    while (1) {
        delay = millis() + INTERVAL_BETWEEN_SAMPLES;
        PT_WAIT_UNTIL(pt, millis() >= delay);

        PT_SEM_WAIT(pt, &mutexAdc);
        ADCC_StartConversion(ADC_HUM);
        PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
        currentAdc = ADCC_GetConversionResult();
        PT_SEM_SIGNAL(pt, &mutexAdc);

        for (i = 0; i < (BUFFER_ADC_SIZE - 1); i++) {
            bufferAdcHumedad[i] = bufferAdcHumedad[i + 1];
        }
        bufferAdcHumedad[BUFFER_ADC_SIZE - 1] = currentAdc;

        summatory = 0;
        for (i = 0; i < BUFFER_ADC_SIZE; i++) {
            summatory += bufferAdcHumedad[i];
        }

        adcHum = (summatory / BUFFER_ADC_SIZE)*(4.096 / 4096);


        currentHumidity = (adcHum * 33.333);

    }

    PT_END(pt);

}

/*
 * Funcion de lectura de sensor de temperatura
 */
static int taskAdcMcp(struct pt *pt) {
    static unsigned long delay;
    static unsigned char i;
    static unsigned long summatory;

    static unsigned int currentAdc;



    PT_BEGIN(pt);

    PT_SEM_WAIT(pt, &mutexAdc);
    ADCC_StartConversion(ADC_MCP);
    PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
    PT_SEM_SIGNAL(pt, &mutexAdc);
    currentAdc = ADCC_GetConversionResult();

    for (i = 0; i < BUFFER_ADC_SIZE; i++) {
        bufferAdcPanel[i] = currentAdc;
    }

    while (1) {
        delay = millis() + INTERVAL_BETWEEN_SAMPLES;
        PT_WAIT_UNTIL(pt, millis() >= delay);

        PT_SEM_WAIT(pt, &mutexAdc);
        ADCC_StartConversion(ADC_MCP);
        PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
        currentAdc = ADCC_GetConversionResult();
        PT_SEM_SIGNAL(pt, &mutexAdc);

        for (i = 0; i < (BUFFER_ADC_SIZE - 1); i++) {
            bufferAdcPanel[i] = bufferAdcPanel[i + 1];
        }
        bufferAdcPanel[BUFFER_ADC_SIZE - 1] = currentAdc;

        summatory = 0;
        for (i = 0; i < BUFFER_ADC_SIZE; i++) {
            summatory += bufferAdcPanel[i];
        }


        adcTemp = (summatory / BUFFER_ADC_SIZE)*(4.096 / 4096);

        currentTemperature = (adcTemp - Vo) / Tc;
        Nop();


    }

    PT_END(pt);

}

/*
 * Funcion de lectura de fotocelda
 */
static int taskAdcBat(struct pt *pt) {
    static unsigned long delay;
    static unsigned char i;
    static unsigned long summatory;
    static unsigned int currentAdc;


    PT_BEGIN(pt);

    PT_SEM_WAIT(pt, &mutexAdc);
    ADCC_StartConversion(ADC_BAT);
    PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
    currentAdc = ADCC_GetConversionResult();
    PT_SEM_SIGNAL(pt, &mutexAdc);


    for (i = 0; i < BUFFER_ADC_SIZE; i++) {
        bufferAdcBateria[i] = currentAdc;
    }

    summatory = 0;
    for (i = 0; i < BUFFER_ADC_SIZE; i++) {
        summatory += bufferAdcBateria[i];
    }
    adcBattery = (summatory / BUFFER_ADC_SIZE);
    //--------------------------------------------------------------------------
    while (1) {
        //Tiempo de espera entre muestreo
        delay = millis() + INTERVAL_BETWEEN_SAMPLES;
        PT_WAIT_UNTIL(pt, millis() >= delay);

        //Se captura la muestra del ADC
        PT_SEM_WAIT(pt, &mutexAdc);
        ADCC_StartConversion(ADC_BAT);
        PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
        currentAdc = ADCC_GetConversionResult();
        PT_SEM_SIGNAL(pt, &mutexAdc);


        for (i = 0; i < (BUFFER_ADC_SIZE - 1); i++) {
            bufferAdcBateria[i] = bufferAdcBateria[i + 1];
        }
        bufferAdcBateria[BUFFER_ADC_SIZE - 1] = currentAdc;


        summatory = 0;
        for (i = 0; i < BUFFER_ADC_SIZE; i++) {
            summatory += bufferAdcBateria[i];
        }
        adcBattery = (summatory / BUFFER_ADC_SIZE); //*(4.096 / 4096);

    }

    PT_END(pt);

}

/*
 * Funcion de lectura de fotocelda
 */
static int taskAdcFoto(struct pt *pt) {
    static unsigned long delay;
    static unsigned char i;
    static unsigned long summatory;
    static unsigned int currentAdc;


    PT_BEGIN(pt);

    //--------------------------------------------------------------------------
    //Se captura una muestra inicial y con ella se llena el buffer
    actdisplay = 0; //Se inhabilita el PWM
    delay = millis() + 5;
    PT_WAIT_UNTIL(pt, millis() >= delay);

    PT_SEM_WAIT(pt, &mutexAdc);
    ADCC_StartConversion(ADC_FOT);
    PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
    currentAdc = ADCC_GetConversionResult();
    PT_SEM_SIGNAL(pt, &mutexAdc);

    actdisplay = 1;

    for (i = 0; i < BUFFER_ADC_SIZE; i++) {
        bufferAdcFoto[i] = currentAdc;
    }

    summatory = 0;
    for (i = 0; i < BUFFER_ADC_SIZE; i++) {
        summatory += bufferAdcFoto[i];
    }
    adcVdcFoto = (summatory / BUFFER_ADC_SIZE);
    //--------------------------------------------------------------------------
    while (1) {
        //Tiempo de espera entre muestreo
        delay = millis() + 10000UL; //para el caso particular de la fotocelda, se lee cada 10seg
        PT_WAIT_UNTIL(pt, millis() >= delay);

        actdisplay = 0; //Se inhabilita el PWM
        delay = millis() + 5;
        PT_WAIT_UNTIL(pt, millis() >= delay);

        //Se captura la muestra del ADC
        PT_SEM_WAIT(pt, &mutexAdc);
        ADCC_StartConversion(ADC_FOT);
        PT_WAIT_UNTIL(pt, ADCC_IsConversionDone());
        currentAdc = ADCC_GetConversionResult();
        PT_SEM_SIGNAL(pt, &mutexAdc);

        actdisplay = 1;

        //se establece un punto de libreación del procesador, para que la 
        //multiplexacion retome lo mas rapido posible
        PT_YIELD(pt);

        for (i = 0; i < (BUFFER_ADC_SIZE - 1); i++) {
            bufferAdcFoto[i] = bufferAdcFoto[i + 1];
        }
        bufferAdcFoto[BUFFER_ADC_SIZE - 1] = currentAdc;


        summatory = 0;
        for (i = 0; i < BUFFER_ADC_SIZE; i++) {
            summatory += bufferAdcFoto[i];
        }
        adcVdcFoto = (summatory / BUFFER_ADC_SIZE); //*(4.096 / 4096);

    }

    PT_END(pt);

}

void startReadAdc(void) {
    PT_INIT(&ptAdcTemp);
    PT_INIT(&ptAdcBat);
    PT_INIT(&ptAdcFoto);
    PT_INIT(&ptAdcHum);
    PT_SEM_INIT(&mutexAdc, 1);
}

void executeReadAdc(void) {
    taskAdcMcp(&ptAdcTemp);
    taskAdcFoto(&ptAdcFoto);
    taskAdcBat(&ptAdcBat);
    taskAdcHum(&ptAdcHum);

}