
#include "eeprom_utils.h"

extern unsigned char minimumSpeed;

extern double pwmSlope;
extern double pwmOffset;

extern double minimumBatVoltage;
extern double batterySlope;
extern double batteryOffset;

extern char fanCondition;
extern char temperatureLimit;
extern char humidityLimit;

extern double brightSlope;
extern double brightOffset;

extern char speedBufferLen;
extern char speedBufferTimeout;

union TUnionDouble {
    double value;
    unsigned char chars[4];
};

union TUnionLong {
    long value;
    unsigned char chars[4];
};


//Escribimos un valor double de la eeprom

void writeEepromDouble(unsigned int address, double data) {
    union TUnionDouble unionDouble;
    unsigned char i;

    unionDouble.value = data;
    for (i = 0; i < 4; i++) {
        DATAEE_WriteByte(address + i, unionDouble.chars[i]);
        __delay_ms(20);
    }

}

//Leemos un valor double de la eeprom

double readEepromDoble(unsigned int address) {
    union TUnionDouble unionDouble;
    unsigned char i;

    for (i = 0; i < 4; i++) {
        unionDouble.chars[i] = DATAEE_ReadByte(address + i);
    }
    return unionDouble.value;
}

//Escribimos un valor long de la eeprom

void writeEepromLong(unsigned int address, long data) {
    union TUnionLong unionLong;
    unsigned char i;

    unionLong.value = data;
    for (i = 0; i < 4; i++) {
        DATAEE_WriteByte(address + i, unionLong.chars[i]);
        __delay_ms(20);
    }

}

//Leemos un valor long de la eeprom

long readEepromLong(unsigned int address) {
    union TUnionLong unionLong;
    unsigned char i;

    for (i = 0; i < 4; i++) {
        unionLong.chars[i] = DATAEE_ReadByte(address + i);
    }
    return unionLong.value;
}

//Inicializamos los valores de la eeprom

void iniValueEeprom(void) {

    temperatureLimit = DATAEE_ReadByte(EE_ADD_TEMPERATURE_LIMIT);
    if (temperatureLimit > 100) {
        temperatureLimit = 10;
        DATAEE_WriteByte(EE_ADD_TEMPERATURE_LIMIT, temperatureLimit);
        __delay_ms(20);
    }

    humidityLimit = DATAEE_ReadByte(EE_ADD_HUMIDITY_LIMIT);
    if (humidityLimit > 100) {
        humidityLimit = 70;
        DATAEE_WriteByte(EE_ADD_HUMIDITY_LIMIT, humidityLimit);
        __delay_ms(20);
    }

    fanCondition = DATAEE_ReadByte(EE_ADD_FAN_CONDITION);
    if (fanCondition > 1) {
        fanCondition = 0;
        DATAEE_WriteByte(EE_ADD_FAN_CONDITION, fanCondition);
        __delay_ms(20);
    }



    brightSlope = readEepromDoble(EE_ADD_BRIGHT_SLOPE);
    if (isnan(brightSlope)) {
        writeEepromDouble(EE_ADD_BRIGHT_SLOPE, 0.0254);
        brightSlope = readEepromDoble(EE_ADD_BRIGHT_SLOPE);
    }

    brightOffset = readEepromDoble(EE_ADD_BRIGHT_OFFSET);
    if (isnan(brightOffset)) {
        writeEepromDouble(EE_ADD_BRIGHT_OFFSET, -4.0821);
        brightOffset = readEepromDoble(EE_ADD_BRIGHT_OFFSET);
    }


    pwmSlope = readEepromDoble(EE_ADD_PWM_SLOPE);
    if (pwmSlope > 100.0 || pwmSlope < (-100.0)) {
        pwmSlope = 0.9;
        writeEepromDouble(EE_ADD_PWM_SLOPE, pwmSlope);

    }

    pwmOffset = readEepromDoble(EE_ADD_PWM_OFFSET);
    if (pwmOffset > 100.0 || pwmOffset < (-100.0)) {
        pwmOffset = 5;
        writeEepromDouble(EE_ADD_PWM_OFFSET, pwmOffset);
    }


    speedBufferLen = DATAEE_ReadByte(EE_ADD_SPEED_BUFFER_LEN);
    if (speedBufferLen > 250) {
        speedBufferLen = 150;
        DATAEE_WriteByte(EE_ADD_SPEED_BUFFER_LEN, speedBufferLen);
        __delay_ms(20);
    }






    minimumBatVoltage = readEepromDoble(EE_ADD_MINBAT);
    if (minimumBatVoltage < 9.0 || minimumBatVoltage > 16.0) {
        minimumBatVoltage = 10.5;
        writeEepromDouble(EE_ADD_MINBAT, minimumBatVoltage);
    }
   

    batterySlope = readEepromDoble(EE_ADD_BATTERY_SLOPE);
    if (batterySlope > 100.0 || batterySlope < 100.0) {
        batterySlope = 0.01;
        writeEepromDouble(EE_ADD_BATTERY_SLOPE, batterySlope);
    }


    batteryOffset = readEepromDoble(EE_ADD_BATTERY_OFFSET);
    if (batteryOffset > 100.0 || batteryOffset < 100.0) {
        batteryOffset = 0.0;
        writeEepromDouble(EE_ADD_BATTERY_OFFSET, batteryOffset);
    }

    minimumSpeed = DATAEE_ReadByte(EE_ADD_MINIMUM_SPEED);
    if (minimumSpeed > 20) {
        minimumSpeed=7;
        DATAEE_WriteByte(EE_ADD_MINIMUM_SPEED, minimumSpeed);
    }
    Nop();

}