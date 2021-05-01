/* 
 * File:   fan_control.h
 * Author: Fabio
 *
 * Created on 31 de diciembre de 2020, 02:09 PM
 */

#ifndef FAN_CONTROL_H
#define	FAN_CONTROL_H

#include "pt-sem.h"
#include "pt.h"
#include "time_system.h"
#include "eeprom_utils.h"
#include "GV300.h"

void startFanControl(void);

void executeFanControl(void);

#endif	/* FAN_CONTROL_H */

