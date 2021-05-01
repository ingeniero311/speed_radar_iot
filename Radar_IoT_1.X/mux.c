#include "mux.h"

/*
 * Se definen las constantes de encendido de los anodos
 * AN3 primero de la derecha a Izquierda
 * AN2 segundo de la derecha a Izquierda
 * AN1 tercero de la derecha a Izquierda
 */

#define onDigit 0
#define offDigit 1

extern unsigned char flagBatLow;

double pwmSlope;
double pwmOffset;

unsigned char actdisplay = 1;

struct TEACHDIGIT {
    unsigned char segA;
    unsigned char segB;
    unsigned char segC;
    unsigned char segD;
    unsigned char segE;
    unsigned char segF;
    unsigned char segG;
};

static struct pt ptLedOk;
static struct pt ptShowdigt;

static struct TEACHDIGIT digits[3];

char digtS = 0;
int fin = 0;

//Validacion del estado si esta visualizando una velocidad
unsigned char isMuxing = 0;

extern unsigned char actdisplay;


int updateDigits(struct pt *pt, char *vel);

// Permite cargar cada segmento con la forma del numero a representar 

void updateEachdigit(struct TEACHDIGIT *digit, signed char value) {

    //al enviar -1 significa que el digito estará apagado
    if (value == -1) { // 1
        digit->segA = 0;
        digit->segB = 0;
        digit->segC = 0;
        digit->segD = 0;
        digit->segE = 0;
        digit->segF = 0;
        digit->segG = 0;
    } else if (value == 0) { // 0
        digit->segA = 1;
        digit->segB = 1;
        digit->segC = 1;
        digit->segD = 1;
        digit->segE = 1;
        digit->segF = 1;
        digit->segG = 0;
    } else if (value == 1) { // 1
        digit->segA = 0;
        digit->segB = 1;
        digit->segC = 1;
        digit->segD = 0;
        digit->segE = 0;
        digit->segF = 0;
        digit->segG = 0;
    } else if (value == 2) { //2
        digit->segA = 1;
        digit->segB = 1;
        digit->segC = 0;
        digit->segD = 1;
        digit->segE = 1;
        digit->segF = 0;
        digit->segG = 1;
    } else if (value == 3) {//3
        digit->segA = 1;
        digit->segB = 1;
        digit->segC = 1;
        digit->segD = 1;
        digit->segE = 0;
        digit->segF = 0;
        digit->segG = 1;
    } else if (value == 4) {// 4
        digit->segA = 0;
        digit->segB = 1;
        digit->segC = 1;
        digit->segD = 0;
        digit->segE = 0;
        digit->segF = 1;
        digit->segG = 1;
    } else if (value == 5) {// 5
        digit->segA = 1;
        digit->segB = 0;
        digit->segC = 1;
        digit->segD = 1;
        digit->segE = 0;
        digit->segF = 1;
        digit->segG = 1;
    } else if (value == 6) {// 6
        digit->segA = 1;
        digit->segB = 0;
        digit->segC = 1;
        digit->segD = 1;
        digit->segE = 1;
        digit->segF = 1;
        digit->segG = 1;
    } else if (value == 7) {// 7
        digit->segA = 1;
        digit->segB = 1;
        digit->segC = 1;
        digit->segD = 0;
        digit->segE = 0;
        digit->segF = 0;
        digit->segG = 0;
    } else if (value == 8) {// 8
        digit->segA = 1;
        digit->segB = 1;
        digit->segC = 1;
        digit->segD = 1;
        digit->segE = 1;
        digit->segF = 1;
        digit->segG = 1;
    } else if (value == 9) {// 9
        digit->segA = 1;
        digit->segB = 1;
        digit->segC = 1;
        digit->segD = 0;
        digit->segE = 0;
        digit->segF = 1;
        digit->segG = 1;
    }
}

//Muestra los dos o tres digitos de la velocidad

static int taskShowDigt(struct pt *pt) {
    static unsigned long delay;
    PT_BEGIN(pt);
    Nop();



    while (1) {
        PT_WAIT_UNTIL(pt, actdisplay && !flagBatLow);

        //Digito #1 (centenas)
        SEG_A_LAT = digits[0].segA;
        SEG_B_LAT = digits[0].segB;
        SEG_C_LAT = digits[0].segC;
        SEG_D_LAT = digits[0].segD;
        SEG_E_LAT = digits[0].segE;
        SEG_F_LAT = digits[0].segF;
        SEG_G_LAT = digits[0].segG;
        __delay_us(15);
        AN1_SetHigh();
        delay = millis() + 2;
        PT_WAIT_UNTIL(pt, millis() > delay);
        AN1_SetLow();
        __delay_us(5);


        //Digito #1 (decenas)
        SEG_A_LAT = digits[1].segA;
        SEG_B_LAT = digits[1].segB;
        SEG_C_LAT = digits[1].segC;
        SEG_D_LAT = digits[1].segD;
        SEG_E_LAT = digits[1].segE;
        SEG_F_LAT = digits[1].segF;
        SEG_G_LAT = digits[1].segG;
        __delay_us(15);
        AN2_SetHigh();
        delay = millis() + 2;
        PT_WAIT_UNTIL(pt, millis() > delay);
        AN2_SetLow();
        __delay_us(5);


        //Digito #1 (unidades)
        SEG_A_LAT = digits[2].segA;
        SEG_B_LAT = digits[2].segB;
        SEG_C_LAT = digits[2].segC;
        SEG_D_LAT = digits[2].segD;
        SEG_E_LAT = digits[2].segE;
        SEG_F_LAT = digits[2].segF;
        SEG_G_LAT = digits[2].segG;
        __delay_us(15);
        AN3_SetHigh();
        delay = millis() + 2;
        PT_WAIT_UNTIL(pt, millis() > delay);
        AN3_SetLow();
        __delay_us(5);

        digtS = 0;
        fin = 0;

        PT_YIELD(pt);
    }


    PT_END(pt);

}



//Se actualiza el nuevo digito para mostrar en el display

int updateDigits(struct pt *pt, char *vel) {

    static signed char digit1;
    static signed char digit2;
    static signed char digit3;
    static unsigned int strSize;


    static char lastStr[20];

    PT_BEGIN(pt);

    //si son iguales    
    if (strcmp(lastStr, vel) == 0) {
        PT_EXIT(pt);
    }

    strSize = strlen(vel);

    if (strSize == 1) {
        digit1 = 0;
        digit2 = 0;
        digit3 = (signed char) (vel[2] - '0');
        digtS = 1;
        PT_YIELD(pt);
    } else if (strSize == 2) {
        digit1 = 0;
        digit2 = (signed char) (vel[1] - '0');
        digit3 = (signed char) (vel[2] - '0');
        digtS = 2;
        PT_YIELD(pt);
    } else if (strSize == 3) {
        digit1 = (signed char) (vel[0] - '0');
        digit2 = (signed char) (vel[1] - '0');
        digit3 = (signed char) (vel[2] - '0');

        if (digit1 == 0 && digit2 == 0 && digit3 == 0) {
            digtS = 0;
            PT_YIELD(pt);
            updateEachdigit(&digits[0], -1);
            updateEachdigit(&digits[1], -1);
            updateEachdigit(&digits[2], -1);
        } else if (digit1 == 0 && digit2 == 0 && digit3 > 0) {
            digtS = 1;
            PT_YIELD(pt);
            updateEachdigit(&digits[0], -1);
            updateEachdigit(&digits[1], -1);
            updateEachdigit(&digits[2], digit3);
        } else if (digit1 == 0 && digit2 > 0) {
            digtS = 2;
            PT_YIELD(pt);
            updateEachdigit(&digits[0], -1);
            updateEachdigit(&digits[1], digit2);
            updateEachdigit(&digits[2], digit3);
        } else if (digit1 > 0) {
            PT_YIELD(pt);
            fin = (digit1 * 100) + (digit2 * 10) + digit3;
            PT_YIELD(pt);
            digtS = 3;
            updateEachdigit(&digits[0], digit1);
            updateEachdigit(&digits[1], digit2);
            updateEachdigit(&digits[2], digit3);
        }
    }

    strcpy(lastStr, vel);


    PT_EXIT(pt);
    PT_END(pt);
    //NOP();
}

void executeMux(void) {
    taskShowDigt(&ptShowdigt);
}

void startMux(void) {

    //PT_SPAWN(pt, &ptSubTask, updateDigits(&ptSubTask, "000"));
    PT_INIT(&ptLedOk);
    PT_INIT(&ptShowdigt);
}