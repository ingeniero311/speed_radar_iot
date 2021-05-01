#include <string.h>

#include "radar_uart.h"
#include "eeprom_utils.h"


#define BUFFER_RX_SIZE 50
#define BUFFER_VEL_SIZE 250

unsigned char minimumSpeed;

unsigned char flagBatLow = 0;

char speedBufferLen = 0;
char speedBufferTimeout = 0;

extern struct pt_sem mutexUart2Tx;

static char txVelStr[250] = "";
static unsigned char velCount = 0;

static struct pt ptRadarRx;
static struct pt ptTxVel;
static struct pt ptUpdateVel;
static struct pt ptTestSpeed;


static char bufferVel[BUFFER_VEL_SIZE];
static unsigned char indexVel;

static char bufferRx[BUFFER_RX_SIZE];
static unsigned char indexRx;

extern unsigned char isMuxing;

static unsigned long lastRadarTimeStamp;

//Se espera 1 segundo si la velocidad no cambia de valor se apaga el display

static int taskSpeedTimeout(struct pt *pt) {
    static struct pt ptSubTask;
    static unsigned long delay;

    PT_BEGIN(pt);
    while (1) {
        PT_WAIT_UNTIL(pt, isMuxing);
        delay = millis() + 1000;
        PT_WAIT_UNTIL(pt, millis() >= delay);
        isMuxing = 0;
        PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, "0"));
        //        if (millis() - lastRadarTimeStamp > 3000) {
        //            PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, "000"));
        //        }
        //        PT_YIELD(pt);
    }
    PT_END(pt);
}

// Tarea para envio de velocidades al display

static int taskReadRadarSpeed(struct pt *pt) {
    static unsigned char charRx;
    static struct pt ptSubTask;

    PT_BEGIN(pt);
    Nop();
    while (1) {
        PT_WAIT_UNTIL(pt, UART1_is_rx_ready());
        while (UART1_is_rx_ready()) {
            charRx = UART1_Read();

            bufferRx[indexRx++] = charRx;
            PT_YIELD(pt);
            if (charRx == 0x0A) {


                lastRadarTimeStamp = millis();
                removeChars(bufferRx, 0x0D);
                removeChars(bufferRx, 0x0A);
                PT_YIELD(pt);


                Nop();

                //Si la velocidad leida por el radar es menor que la velocidad minima detectable
                //se pone a cero
                if (atoi(bufferRx) < minimumSpeed) {
                    strcpy(bufferRx, "000");
                }


                if (strcmp(bufferRx, "0") == 0) {
                    PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, bufferRx));
                    memset(bufferRx, 0, sizeof (bufferRx));
                    indexRx = 0;
                    PT_YIELD(pt);
                    continue;
                }

                if (strcmp(bufferRx, "00") == 0) {
                    PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, bufferRx));
                    memset(bufferRx, 0, sizeof (bufferRx));
                    indexRx = 0;
                    PT_YIELD(pt);
                    continue;
                }

                if (strcmp(bufferRx, "000") == 0) {
                    PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, bufferRx));
                    memset(bufferRx, 0, sizeof (bufferRx));
                    indexRx = 0;
                    PT_YIELD(pt);
                    continue;
                }


                isMuxing = 1;
                PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, bufferRx));

                if (strlen(txVelStr) != 0) {
                    PT_YIELD(pt);
                    strcat(txVelStr, "|");
                }
                strcat(txVelStr, bufferRx);
                velCount++;
                PT_YIELD(pt);
                // sendVel(bufferRx);
                memset(bufferRx, 0, sizeof (bufferRx));
                indexRx = 0;
                PT_YIELD(pt);
            } else if (indexRx > 5) {
                memset(bufferRx, 0, sizeof (bufferRx));
                indexRx = 0;
                PT_YIELD(pt);
            }
        }
    }
    PT_END(pt);
}

static int taskTestRadar(struct pt *pt) {
    static unsigned long delay;
    static unsigned char speedVal;
    static char speedStr[10];
    static unsigned char i;
    static unsigned char size;
    static char finalSpeed[10];
    static struct pt ptSubTask;

    PT_BEGIN(pt);

    while (1) {
        for (i = 0; i <= 120; i += 11) {
            delay = millis() + 200;
            PT_WAIT_UNTIL(pt, millis() > delay);
            CLRWDT();

            sprintf(speedStr, "%u", i);
            if (strlen(speedStr) == 1) {
                strcpy(finalSpeed, "00");
                strcat(finalSpeed, speedStr);
            } else if (strlen(speedStr) == 2) {
                strcpy(finalSpeed, "0");
                strcat(finalSpeed, speedStr);
            } else if (strlen(speedStr) == 3) {
                strcpy(finalSpeed, "");
                strcat(finalSpeed, speedStr);
            }
            lastRadarTimeStamp = millis();
            Nop();
            isMuxing = 1;
            PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, finalSpeed));
        }
    }

    PT_END(pt);
}

//Se valida si ha pasado un tiempo sin sensar velocidades envia las que tenga almacenadas, 
//pero si no tiene ninguna no envia nada, o si tiene mas de cirta cantidad de velocidades las envia al servidor 

static int taskTimeout(struct pt *pt) {
    static unsigned long delay;
    char numericStr[15];



    PT_BEGIN(pt);


    while (1) {

        delay = millis() + 60000UL;
        PT_WAIT_UNTIL(pt, ((millis() > delay) || (strlen(txVelStr) >= speedBufferLen)));


        if (strlen(txVelStr) == 0) {
            velCount = 0;
            continue;
        } else {

            sprintf(numericStr, "%u", velCount);
            PT_SEM_WAIT(pt, &mutexUart2Tx);

            sendUart2Str(TRANSPARENT_MSG_HEADER);
            sendUart2Str("|");
            sendUart2Str(numericStr);
            sendUart2Str("|");
            sendUart2Str(txVelStr);
            sendUart2Str(TRANSPARENT_MSG_FOOTER);

            velCount = 0;
            strcpy(txVelStr, "");

            delay = millis() + UART_MUTEX_DELAY;
            PT_WAIT_UNTIL(pt, millis() >= delay);
            PT_SEM_SIGNAL(pt, &mutexUart2Tx);
        }
    }
    PT_END(pt);
}

void executeRadarUart(void) {
    taskReadRadarSpeed(&ptRadarRx);
    taskTimeout(&ptTxVel);
    taskSpeedTimeout(&ptUpdateVel);
   // taskTestRadar(&ptTestSpeed);
}

void startRadarUart(void) {
    PT_INIT(&ptRadarRx);
    PT_INIT(&ptTxVel);
    PT_INIT(&ptUpdateVel);
    PT_INIT(&ptTestSpeed);



    strcpy(txVelStr, "");

    memset(bufferRx, 0, sizeof (bufferRx));
    indexRx = 0;

    memset(bufferVel, 0, sizeof (bufferVel));
    indexVel = 0;
}

