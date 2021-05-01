#include "fan_control.h"

//Limite de temperatura minima, por debajo de la cual de evalua si se deben prender los ventiladores
char temperatureLimit;

//Limite de humedad máxima, por encima de la cual de evalua si se deben prender los ventiladores
char humidityLimit;

//PT de la tarea que verifica si se deben encender los ventiladores
static struct pt ptFanControl;

//Define si se deben cumplir ambas condiciones de humedad y temperatura (1) 
//o solo alguna de ellas (0))
char fanCondition;

extern double currentHumidity;
extern double currentTemperature;


extern struct pt_sem mutexUart2Tx;

static int taskCheckFan(struct pt *pt) {
    static unsigned long delay;
    static double lowTemperatureLimit;
    static double lowHumidityLimit;
    static double highTemperatureLimit;
    static double highHumidityLimit;

    PT_BEGIN(pt);

    while (1) {
        //----------------------------------------------------------------------
        //Tarea ciclica cada 30 segundos
        delay = millis() + 30000;
        PT_WAIT_UNTIL(pt, millis() >= delay);
        //----------------------------------------------------------------------
        //Se deben volver a calcular los límites, ya que los valores pueden cambiar
        //debido a un comando remoto
        lowTemperatureLimit = ((double) temperatureLimit) - 2.0;
        highTemperatureLimit = ((double) temperatureLimit) + 2.0;
        lowHumidityLimit = ((double) humidityLimit) - 2.0;
        highHumidityLimit = ((double) humidityLimit) + 2.0;
        //----------------------------------------------------------------------

        if (fanCondition == 1) { //Condicion de cumplimientos de AMBAS condiciones
            if (FAN_LAT == 0 && (currentTemperature < lowTemperatureLimit && currentHumidity > highHumidityLimit)) {
                PT_SEM_WAIT(pt, &mutexUart2Tx);
                sendUart2Str(EVN_FAN_ON);
                delay = millis() + UART_MUTEX_DELAY;
                PT_WAIT_UNTIL(pt, millis() >= delay);
                PT_SEM_SIGNAL(pt, &mutexUart2Tx);
                FAN_SetHigh();
            } else if (FAN_LAT == 1 && (currentTemperature > highTemperatureLimit || currentHumidity < lowHumidityLimit)) {
                PT_SEM_WAIT(pt, &mutexUart2Tx);
                sendUart2Str(EVN_FAN_OFF);
                delay = millis() + UART_MUTEX_DELAY;
                PT_WAIT_UNTIL(pt, millis() >= delay);
                PT_SEM_SIGNAL(pt, &mutexUart2Tx);
                FAN_SetLow();
            }
        } else if (fanCondition == 0) { //Condicion de cumplimientos de ALGUNA de las condiciones
            if (FAN_LAT == 0 && (currentTemperature < lowTemperatureLimit || currentHumidity > highHumidityLimit)) {
                PT_SEM_WAIT(pt, &mutexUart2Tx);
                sendUart2Str(EVN_FAN_ON);
                delay = millis() + UART_MUTEX_DELAY;
                PT_WAIT_UNTIL(pt, millis() >= delay);
                PT_SEM_SIGNAL(pt, &mutexUart2Tx);
                FAN_SetHigh();
            } else if (FAN_LAT == 1 && (currentTemperature > highTemperatureLimit && currentHumidity < lowHumidityLimit)) {
                PT_SEM_WAIT(pt, &mutexUart2Tx);
                sendUart2Str(EVN_FAN_OFF);
                delay = millis() + UART_MUTEX_DELAY;
                PT_WAIT_UNTIL(pt, millis() >= delay);
                PT_SEM_SIGNAL(pt, &mutexUart2Tx);
                FAN_SetLow();
            }
        }
    }
    PT_END(pt);
}

void startFanControl(void) {
    PT_INIT(&ptFanControl);
}

void executeFanControl(void) {
    taskCheckFan(&ptFanControl);
}
