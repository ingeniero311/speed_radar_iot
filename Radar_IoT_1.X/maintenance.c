#include "maintenance.h"

static struct pt ptResetWdt;
static struct pt ptResetMcu;

static struct pt ptLedOk;
static struct pt ptBootEvent;
extern struct pt_sem mutexUart2Tx;


//Tarea de mantenimiento
static int taskLed(struct pt *pt) {
    static unsigned long delay;

    PT_BEGIN(pt);
    Nop();
    while (1) {
        LED_OK_Toggle();      
        delay = millis() + 500;
        PT_WAIT_UNTIL(pt, millis() >= delay);
    }
    PT_END(pt);
}

static int taskWdtReset(struct pt *pt) {
    static unsigned long delay;

    PT_BEGIN(pt);
    Nop();
    while (1) {
        delay = millis() + 1500;
        PT_WAIT_UNTIL(pt, millis() >= delay);
        CLRWDT();
    }
    PT_END(pt);
}

//Reiniciamos el micro cada 6 horas

static int taskRstMcu(struct pt *pt) {
    static unsigned long delay;
    static unsigned long resetCounter = 0;

    PT_BEGIN(pt);
    //--------------------------------------------------------------------------
    //Se espera 5 segundos para enviar el evento de inicio de la tarjeta
    delay = millis() + 5000;
    PT_WAIT_UNTIL(pt, millis() >= delay);
    PT_SEM_WAIT(pt, &mutexUart2Tx);
    sendUart2Str(STR_EVENT_BOOT);
    delay = millis() + UART_MUTEX_DELAY;
    PT_WAIT_UNTIL(pt, millis() >= delay);
    PT_SEM_SIGNAL(pt, &mutexUart2Tx);
    //--------------------------------------------------------------------------
    while (1) {
        resetCounter++;
        delay = millis() + 1000;
        PT_WAIT_UNTIL(pt, millis() >= delay);
        if (resetCounter >= 21600UL) {
            sendUart2Str(STR_EVENT_AUTO_RESET);
            delay = millis() + 1000;
            PT_WAIT_UNTIL(pt, millis() >= delay);
            Reset();
        }
    }
    PT_END(pt);
}

void startMaintenance(void) {
    PT_INIT(&ptResetMcu);
    PT_INIT(&ptResetWdt);
    PT_INIT(&ptLedOk);
}

void executeMaintenance(void) {
    taskWdtReset(&ptResetWdt);
    taskRstMcu(&ptResetMcu);
    taskLed(&ptLedOk);
}
