#include <string.h>

#include "GV300.h"
#include "time_system.h"
#include "eeprom_utils.h"
#include "read_adc.h"
#include <stdio.h>


#define BUFFER_RX_SIZE 160



extern unsigned char minimumSpeed;

extern double pwmSlope;
extern double pwmOffset;

extern double adcVdcFoto;

extern double batteryVoltage;

extern double minimumBatVoltage;

extern double batterySlope;
extern double batteryOffset;

extern char speedBufferLen;
extern char speedBufferTimeout;

extern double lightLevel;

extern double brightSlope;
extern double brightOffset;

extern char fanCondition;
extern char temperatureLimit;
extern char humidityLimit;

extern double batterySlope;
extern double batteryOffset;



static struct pt ptGv300Rx;
static struct pt ptGV300Timeout;


static char bufferRx[BUFFER_RX_SIZE];
static unsigned char indexRx;


static unsigned int bufferIndex = 0;
static char bufferData[BUFFER_RX_SIZE];

struct pt_sem mutexUart2Tx;

//Envia datos a la unidad

void sendUart2Str(char *str) {
    unsigned char i;
    unsigned char strSize;

    strSize = strlen(str);
    for (i = 0; i < strSize; i++) {
        UART2_Write(str[i]);
    }

}

//Quitamos caracteres de un arreglo

void removeChars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

//Enviamos el Ack de un comando

static int sendAck(struct pt *pt, char *command, char *params) {

    static char miscStr[100];
    static unsigned long delay;

    PT_BEGIN(pt);


    strcpy(miscStr, "");

    strcat(miscStr, command);
    strcat(miscStr, ";A");


    if (params != 0) {
        strcat(miscStr, ";");
        strcat(miscStr, params);
    }
    strcat(miscStr, "#");

    Nop();

    PT_SEM_WAIT(pt, &mutexUart2Tx);
    sendUart2Str("AT+GTDAT=gv300,0,,");
    sendUart2Str(miscStr);
    sendUart2Str(",0,,,,FFFF$\r\n");
    delay = millis() + UART_MUTEX_DELAY;
    PT_WAIT_UNTIL(pt, millis() >= delay);
    PT_SEM_SIGNAL(pt, &mutexUart2Tx);

    PT_EXIT(pt);
    PT_END(pt);
}

//Limpiamos el buffer

static void clearBuffer(void) {
    bufferIndex = 0;
    memset(bufferData, 0, sizeof (bufferData));
}

//------------------------------------------------------------------------------
// Ventilador Funcion de ajuste

static int cmdSetFan(struct pt *pt, char * strCommand, char *strCondition, char *strHumidity, char *strTemperature) {
    unsigned char condition;
    unsigned char humidity;
    unsigned char temperature;
    static struct pt ptSubTask;

    PT_BEGIN(pt);
    condition = (unsigned char) atoi(strCondition);
    humidity = (unsigned char) atoi(strHumidity);
    temperature = (unsigned char) atoi(strTemperature);

    if (condition > 1 || humidity > 100 || temperature > 250) {
        PT_EXIT(pt);
    }

    fanCondition = condition;
    temperatureLimit = temperature;
    humidityLimit = humidity;

    DATAEE_WriteByte(EE_ADD_FAN_CONDITION, condition);
    DATAEE_WriteByte(EE_ADD_HUMIDITY_LIMIT, humidity);
    DATAEE_WriteByte(EE_ADD_TEMPERATURE_LIMIT, temperature);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

// Ventilador Funcion de lectura

static int cmdGetFan(struct pt *pt, char * strCommand) {
    unsigned char condition;
    unsigned char humidity;
    unsigned char temperature;
    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[20];

    PT_BEGIN(pt);
    condition = DATAEE_ReadByte(EE_ADD_FAN_CONDITION);
    humidity = DATAEE_ReadByte(EE_ADD_HUMIDITY_LIMIT);
    temperature = DATAEE_ReadByte(EE_ADD_TEMPERATURE_LIMIT);

    strcpy(ackStr, "");
    sprintf(numericChar, "%u", condition);
    strcat(ackStr, numericChar);
    strcat(ackStr, ";");
    sprintf(numericChar, "%u", humidity);
    strcat(ackStr, numericChar);
    strcat(ackStr, ";");
    sprintf(numericChar, "%u", temperature);
    strcat(ackStr, numericChar);


    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);
}


//------------------------------------------------------------------------------
// Brillo Funcion de ajuste

static int cmdSetBrightFunction(struct pt *pt, char * strCommand, char *strSlope, char *strOffset) {
    static struct pt ptSubTask;

    PT_BEGIN(pt);
    brightSlope = atof(strSlope);
    brightOffset = atof(strOffset);


    writeEepromDouble(EE_ADD_BRIGHT_SLOPE, brightSlope);
    writeEepromDouble(EE_ADD_BRIGHT_OFFSET, brightOffset);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

// Brillo Funcion de lectura

static int cmdGetBrightFunction(struct pt *pt, char * strCommand) {
    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);

    strcpy(ackStr, "");
    sprintf(numericChar, "%.3f", brightSlope);
    strcat(ackStr, numericChar);
    strcat(ackStr, ";");
    sprintf(numericChar, "%.3f", brightOffset);
    strcat(ackStr, numericChar);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}


//------------------------------------------------------------------------------
// ADC fotocelda  Funcion de lectura

static int cmdGetAdcFoto(struct pt *pt, char * strCommand) {
    static struct pt ptSubTask;
    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);

    strcpy(ackStr, "");
    sprintf(numericChar, "%u", (unsigned int) adcVdcFoto);
    strcat(ackStr, numericChar);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}



//------------------------------------------------------------------------------
// Luz  Funcion de lectura

static int cmdGetLightLevel(struct pt *pt, char * strCommand) {


    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);

    strcpy(ackStr, "");
    sprintf(numericChar, "%u", (unsigned int) lightLevel);
    strcat(ackStr, numericChar);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}


//------------------------------------------------------------------------------
// PWM Funcion de ajuste

static int cmdSetPwmFunction(struct pt *pt, char * strCommand, char *strSlope, char *strOffset) {
    static struct pt ptSubTask;


    PT_BEGIN(pt);
    pwmSlope = atof(strSlope);
    pwmOffset = atof(strOffset);

    writeEepromDouble(EE_ADD_PWM_SLOPE, pwmSlope);
    writeEepromDouble(EE_ADD_PWM_OFFSET, pwmOffset);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

// Brillo Funcion de lectura

static int cmdGetPwmFunction(struct pt *pt, char * strCommand) {
    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);
    strcpy(ackStr, "");
    sprintf(numericChar, "%.3f", pwmSlope);
    strcat(ackStr, numericChar);
    strcat(ackStr, ";");
    sprintf(numericChar, "%.3f", pwmOffset);
    strcat(ackStr, numericChar);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}


//------------------------------------------------------------------------------
// Velocidades acumuladas  Funcion de ajuste

static int cmdSetVelcont(struct pt *pt, char * strCommand, char *strLen) {


    static struct pt ptSubTask;

    PT_BEGIN(pt);

    speedBufferLen = (unsigned char) atoi(strLen);




    DATAEE_WriteByte(EE_ADD_SPEED_BUFFER_LEN, speedBufferLen);
    __delay_ms(20);
    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

// Velocidades acumuladas Funcion de lectura

static int cmdGetVelcont(struct pt *pt, char * strCommand) {


    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[20];

    PT_BEGIN(pt);



    strcpy(ackStr, "");
    sprintf(numericChar, "%u", speedBufferLen);
    strcat(ackStr, numericChar);




    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);
}


//------------------------------------------------------------------------------
// Reporte de Humedad y temperatura  Funcion de ajuste

static int cmdSetHump(struct pt *pt, char * strCommand, char *strSeg) {

    long segundos;

    static struct pt ptSubTask;

    PT_BEGIN(pt);

    segundos = atol(strSeg);

    if (segundos > 3600 || segundos < 60) {
        //TODO error
    }

    writeEepromLong(EE_ADD_HMT_T, segundos);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

// Reporte de Humedad y temperatura  Funcion de lectura

static int cmdGetHump(struct pt *pt, char * strCommand) {

    long segundos;

    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[20];

    PT_BEGIN(pt);

    segundos = readEepromLong(EE_ADD_HMT_T);


    strcpy(ackStr, "");
    sprintf(numericChar, "%u", segundos);
    strcat(ackStr, numericChar);



    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);
}



//------------------------------------------------------------------------------
// Minimo de Bateria Funcion de ajuste

static int cmdSetMinbat(struct pt *pt, char * strCommand, char *strMinb) {

    double minBat;
    static struct pt ptSubTask;


    PT_BEGIN(pt);
    minBat = atof(strMinb);


    if (minBat > 14.5 || minBat < 10.5) {
        PT_EXIT(pt);
    }

    minimumBatVoltage=minBat;

    writeEepromDouble(EE_ADD_MINBAT, minBat);


    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

// Minimo de Bateria Funcion de lectura

static int cmdGetMinbat(struct pt *pt, char * strCommand) {


    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);



    strcpy(ackStr, "");
    sprintf(numericChar, "%.3f", minimumBatVoltage);
    strcat(ackStr, numericChar);


    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}

static int cmdSetMinimumSpeed(struct pt *pt, char * strCommand, char *strSpeed) {

    static struct pt ptSubTask;


    PT_BEGIN(pt);
    minimumSpeed = (unsigned char) atoi(strSpeed);

    DATAEE_WriteByte(EE_ADD_MINIMUM_SPEED, minimumSpeed);
    __delay_ms(20);


    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

static int cmdGetMinimumSpeed(struct pt *pt, char * strCommand) {

    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);

    strcpy(ackStr, "");
    sprintf(numericChar, "%u", minimumSpeed);
    strcat(ackStr, numericChar);


    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}


//------------------------------------------------------------------------------
// Factor ADC bateria Funcion de ajuste

static int cmdSetBatteryFunction(struct pt *pt, char * strCommand, char *strSlope, char *strOffset) {

    static struct pt ptSubTask;


    PT_BEGIN(pt);
    batterySlope = atof(strSlope);
    batteryOffset = atof(strOffset);

    writeEepromDouble(EE_ADD_BATTERY_OFFSET, batterySlope);
    writeEepromDouble(EE_ADD_BATTERY_SLOPE, batteryOffset);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));

    PT_EXIT(pt);
    PT_END(pt);
}

// Factor ADC bateria Funcion de lectura

static int cmdGetBatteryFunction(struct pt *pt, char * strCommand) {

    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);

    strcpy(ackStr, "");
    sprintf(numericChar, "%.3f", batterySlope);
    strcat(ackStr, numericChar);
    strcat(ackStr, ";");
    sprintf(numericChar, "%.3f", batteryOffset);
    strcat(ackStr, numericChar);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}


// Voltaje bateria Funcion de lectura

static int cmdGetBatteryVoltage(struct pt *pt, char * strCommand) {
    double voltBat;
    static struct pt ptSubTask;

    char numericChar[15];
    char ackStr[30];

    PT_BEGIN(pt);



    strcpy(ackStr, "");
    sprintf(numericChar, "%.3f", batteryVoltage);
    strcat(ackStr, numericChar);

    PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, ackStr));

    PT_EXIT(pt);
    PT_END(pt);

}

static int taskGV300RxTimeout(struct pt *pt) {
    static unsigned long delay;
    PT_BEGIN(pt);

    while (1) {
        PT_WAIT_UNTIL(pt, bufferIndex > 0);
        delay = millis() + 2000;
        PT_WAIT_UNTIL(pt, millis() >= delay || bufferIndex == 0);
        clearBuffer();
    }

    PT_END(pt);
}


//Recibe los comandos de la unidad

static int taskReadGV300Com(struct pt *pt) {
    static unsigned long delayRx;
    static char numericStr[10];
    static struct pt ptSubTask;
    static unsigned char returnValueChar;

#define PARAM_SIZE 25

    static char rxChar;
    static char strCommand[PARAM_SIZE];
    static char strModifier[PARAM_SIZE];
    static char strParam1[PARAM_SIZE];
    static char strParam2[PARAM_SIZE];
    static char strParam3[PARAM_SIZE];
    static char strParam4[PARAM_SIZE];
    static char strParam5[PARAM_SIZE];
    static char strParam6[PARAM_SIZE];
    static char strParam7[PARAM_SIZE];
    static char strParam8[PARAM_SIZE];
    static char strParam9[PARAM_SIZE];
    static char strParam10[PARAM_SIZE];
    static char strParam11[PARAM_SIZE];



    PT_BEGIN(pt);

    while (1) {
        PT_WAIT_UNTIL(pt, UART2_is_rx_ready());
        while (UART2_is_rx_ready()) {
            rxChar = UART2_Read();
            NOP();

            if (bufferIndex < (BUFFER_RX_SIZE - 1)) {
                bufferData[bufferIndex] = rxChar;
                bufferIndex++;
            } else {
                clearBuffer();
            }

            if (strrchr(bufferData, '#') == 0) {
                continue;
            }

            memset(strCommand, 0, PARAM_SIZE);
            memset(strModifier, 0, PARAM_SIZE);
            memset(strParam1, 0, PARAM_SIZE);
            memset(strParam2, 0, PARAM_SIZE);
            memset(strParam3, 0, PARAM_SIZE);
            memset(strParam4, 0, PARAM_SIZE);
            memset(strParam5, 0, PARAM_SIZE);
            memset(strParam6, 0, PARAM_SIZE);
            memset(strParam7, 0, PARAM_SIZE);
            memset(strParam8, 0, PARAM_SIZE);
            memset(strParam9, 0, PARAM_SIZE);
            memset(strParam10, 0, PARAM_SIZE);
            memset(strParam11, 0, PARAM_SIZE);

            //Split process
            strcpy(strCommand, strtok(bufferData, ";"));
            strcpy(strModifier, strtok(0, ";"));
            strcpy(strParam1, strtok(0, ";"));
            strcpy(strParam2, strtok(0, ";"));
            strcpy(strParam3, strtok(0, ";"));
            strcpy(strParam4, strtok(0, ";"));
            strcpy(strParam5, strtok(0, ";"));
            strcpy(strParam6, strtok(0, ";"));
            strcpy(strParam7, strtok(0, ";"));
            strcpy(strParam8, strtok(0, ";"));
            strcpy(strParam9, strtok(0, ";"));
            strcpy(strParam10, strtok(0, ";"));
            strcpy(strParam11, strtok(0, ";"));


            removeChars(strCommand, '#');
            removeChars(strModifier, '#');
            removeChars(strParam1, '#');
            removeChars(strParam2, '#');
            removeChars(strParam3, '#');
            removeChars(strParam4, '#');
            removeChars(strParam5, '#');
            removeChars(strParam6, '#');
            removeChars(strParam7, '#');
            removeChars(strParam8, '#');
            removeChars(strParam9, '#');
            removeChars(strParam10, '#');
            removeChars(strParam11, '#');


            removeChars(strCommand, 13);
            removeChars(strModifier, 13);
            removeChars(strParam1, 13);
            removeChars(strParam2, 13);
            removeChars(strParam3, 13);
            removeChars(strParam4, 13);
            removeChars(strParam5, 13);
            removeChars(strParam6, 13);
            removeChars(strParam7, 13);
            removeChars(strParam8, 13);
            removeChars(strParam9, 13);
            removeChars(strParam10, 13);
            removeChars(strParam11, 13);

            removeChars(strCommand, 10);
            removeChars(strModifier, 10);
            removeChars(strParam1, 10);
            removeChars(strParam2, 10);
            removeChars(strParam3, 10);
            removeChars(strParam4, 10);
            removeChars(strParam5, 10);
            removeChars(strParam6, 10);
            removeChars(strParam7, 10);
            removeChars(strParam8, 10);
            removeChars(strParam9, 10);
            removeChars(strParam10, 10);
            removeChars(strParam11, 10);

            if (strcmp(strCommand, "RESET") == 0 && strcmp(strModifier, "W") == 0) {
                PT_SPAWN(pt, &ptSubTask, sendAck(&ptSubTask, strCommand, 0));
                sendUart2Str("RESET;A#");
                delayRx = millis() + 1000;
                PT_WAIT_UNTIL(pt, millis() > delayRx);
                asm("reset");
            } else if (strcmp(strCommand, "FAN") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetFan(&ptSubTask, strCommand, strParam1, strParam2, strParam3));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetFan(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "BRIGHT") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetBrightFunction(&ptSubTask, strCommand, strParam1, strParam2));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetBrightFunction(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "LIGHT") == 0) {
                if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetLightLevel(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "PWM") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetPwmFunction(&ptSubTask, strCommand, strParam1, strParam2));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetPwmFunction(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "VELCOUNT") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetVelcont(&ptSubTask, strCommand, strParam1));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetVelcont(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "HGM") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetHump(&ptSubTask, strCommand, strParam1));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetHump(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "MINBAT") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetMinbat(&ptSubTask, strCommand, strParam1));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetMinbat(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "FUNBAT") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetBatteryFunction(&ptSubTask, strCommand, strParam1, strParam2));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetBatteryFunction(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "BATVDC") == 0) {
                if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetBatteryVoltage(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "ADCFOTO") == 0) {
                if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetAdcFoto(&ptSubTask, strCommand));
                }
            } else if (strcmp(strCommand, "MINSPEED") == 0) {
                if (strcmp(strModifier, "W") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdSetMinimumSpeed(&ptSubTask, strCommand, strParam1));
                } else if (strcmp(strModifier, "R") == 0) {
                    PT_SPAWN(pt, &ptSubTask, cmdGetMinimumSpeed(&ptSubTask, strCommand));
                }
            }



            clearBuffer();

        }

    }

    PT_END(pt);
}

void executeGV300Uart(void) {
    taskReadGV300Com(&ptGv300Rx);
    taskGV300RxTimeout(&ptGV300Timeout);
}

void startGV300Uart(void) {
    PT_INIT(&ptGv300Rx);
    PT_INIT(&ptGV300Timeout);

    PT_SEM_INIT(&mutexUart2Tx, 1);
    memset(bufferRx, 0, sizeof (bufferRx));
    indexRx = 0;
}
