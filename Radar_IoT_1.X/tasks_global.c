#include "tasks_global.h"



extern double pwmSlope;
extern double pwmOffset;

extern double adcVdcFoto;
extern double adcBattery;
extern double adcTemp;
extern double adcHum;

extern unsigned char flagBatLow;
//Nivel de luiz actual en el rango de 0~100
double lightLevel;

double batterySlope;
double batteryOffset;

double batteryVoltage;

extern struct pt_sem mutexUart2Tx;

extern double currentHumidity;
extern double currentTemperature;

static struct pt ptResetWdt;
static struct pt ptResetMcu;
static struct pt ptEvenHgm;
static struct pt ptSetPwm;
static struct pt ptActFan;
static struct pt ptSenBat;
double minimumBatVoltage;


//Pendiente de la funcion para cualcular el brillo, basado en el ADC de la fotocelda
double brightSlope;

//Corte de la funcion para cualcular el brillo, basado en el ADC de la fotocelda
double brightOffset;

//Tarea de activacion o desactivacion de display por bateria baja

static int taskCheckBattery(struct pt *pt) {
    static unsigned long delay;
    double voltBat;

    PT_BEGIN(pt);
    Nop();
    while (1) {

        delay = millis() + 10000UL;
        PT_WAIT_UNTIL(pt, millis() >= delay);

        Nop();
        batteryVoltage = (batterySlope * adcBattery) + batteryOffset;

        if (batteryVoltage < (minimumBatVoltage - 0.5)) {
            flagBatLow = 1;
        } else if (batteryVoltage > (minimumBatVoltage + 0.5)) {
            flagBatLow = 0;
        }

        Nop();

    }

    PT_END(pt);

}




//Se verifica el brillo y se ajusta el PWM cada 10 min
//PWM5_LoadDutyValue(46); //46 888  -  5 95 %

static int taskSetPwm(struct pt *pt) {
    static unsigned long delay;
    double pwmPercentage;
    double pwmDutyVal;

    PT_BEGIN(pt);
    PWM5_LoadDutyValue(31);
    while (1) {
        delay = millis() + 10000UL;
        PT_WAIT_UNTIL(pt, millis() >= delay);
        //--------------------------------------------------------------------- 
        //Se calcula el valor de 0~100 de la luz leida por la fotocelda
        lightLevel = (brightSlope * adcVdcFoto) + brightOffset;
        if (lightLevel > 100.0) {
            lightLevel = 100.0;
        } else if (lightLevel < 0.0) {
            lightLevel = 0.0;
        }
        //---------------------------------------------------------------------
        //Con el valor del nivel de luz (0~100), se calcula el porcentaje de PWM
        pwmPercentage = (pwmSlope * lightLevel) + pwmOffset;
        pwmDutyVal = (pwmPercentage * 31) / 100.0;
        PWM5_LoadDutyValue((unsigned int) pwmDutyVal);
        //---------------------------------------------------------------------
    }
    PT_END(pt);
}

//Envio de valores Higrometro

static int taskEvnHgm(struct pt *pt) {
    static unsigned long delay;
    char numericChar[15];

    PT_BEGIN(pt);
    Nop();
    while (1) {
        delay = millis() + 600000UL; //Cada 10 minutos
        PT_WAIT_UNTIL(pt, millis() >= delay);



        PT_SEM_WAIT(pt, &mutexUart2Tx);
        sendUart2Str(EVN_HGM_INI);

        sprintf(numericChar, "%.3f", currentHumidity);
        sendUart2Str(numericChar);
        sendUart2Str("|");
        sprintf(numericChar, "%.3f", currentTemperature);
        sendUart2Str(numericChar);
        sendUart2Str("|");
        if (FAN_LAT) {
            sendUart2Str("1");
        } else {
            sendUart2Str("0");
        }
        sendUart2Str(EVN_HGM_FIN);

        delay = millis() + UART_MUTEX_DELAY;
        PT_WAIT_UNTIL(pt, millis() >= delay);
        PT_SEM_SIGNAL(pt, &mutexUart2Tx);
    }

    PT_END(pt);

}

//reiniciamos el watch dog cada 1.5 segundos

void executeGlobalTask(void) {


    taskEvnHgm(&ptEvenHgm);
    taskSetPwm(&ptSetPwm);
    taskCheckBattery(&ptSenBat);
}

void startGlobalTask(void) {
    PT_INIT(&ptResetWdt);
    PT_INIT(&ptResetMcu);
    PT_INIT(&ptEvenHgm);
    PT_INIT(&ptSetPwm);
    PT_INIT(&ptActFan);
    PT_INIT(&ptSenBat);

}