/* 
 * File:   eeprom_utils.h
 * Author: jguar
 * #include <pic18.h>

 *
 * Created on 13 de diciembre de 2020, 01:45 PM
 */

#ifndef EEPROM_UTILS_H
#define	EEPROM_UTILS_H

#include "mcc_generated_files/mcc.h"
#include "crystal_frequency.h"
#include <pic18.h>
#include <math.h>
#include "GV300.h"

//-----------Direciones de eprom--------------------- 
#define EE_ADD_TEMPERATURE_LIMIT 2
#define EE_ADD_HUMIDITY_LIMIT 1
#define EE_ADD_FAN_CONDITION 0

#define EE_ADD_BRIGHT_SLOPE 3
#define EE_ADD_BRIGHT_OFFSET 7

#define EE_ADD_PWM_SLOPE 11
#define EE_ADD_PWM_OFFSET 15

#define EE_ADD_SPEED_BUFFER_LEN 19
#define EE_ADD_SPEED_TIMEOUT 20

#define EE_ADD_HMT_T 21

#define EE_ADD_MINBAT 25

#define EE_ADD_BATTERY_SLOPE 30
#define EE_ADD_BATTERY_OFFSET 34
#define EE_ADD_MINIMUM_SPEED 38 //1 byte




void iniValueEeprom();

void writeEepromDouble(unsigned int address, double data);
double readEepromDoble(unsigned int address);

void writeEepromLong(unsigned int address, long data);
long readEepromLong(unsigned int address);
void iniValueEeprom(void);

#endif	/* EEPROM_UTILS_H */

