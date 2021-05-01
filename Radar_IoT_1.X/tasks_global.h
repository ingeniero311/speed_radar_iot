/* 
 * File:   taskGlobla.h
 * Author: jguar
 *
 * Created on December 20, 2020, 7:22 PM
 */

#ifndef TASKGLOBLA_H
#define	TASKGLOBLA_H



#include "pt.h"
#include "mcc_generated_files/mcc.h"
#include "time_system.h"
#include <stdio.h>
#include <string.h>
#include "GV300.h"


//Tiempo de comprobacion del brillo y ajuste
#define TIM_SET_PWM 600000UL

//Tiempo de comprobacion para el encendido de los ventiladores
#define TIM_ACT_FAN 600000UL

//Tiempo de comprobacion para el encendido de los ventiladores
#define TIM_SEN_MINBAT 60000UL









//Activacion general de la visualizacion por bateria baja




void executeGlobalTask(void);

void startGlobalTask(void);

#endif	/* TASKGLOBLA_H */

