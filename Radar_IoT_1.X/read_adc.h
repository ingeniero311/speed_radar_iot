

#ifndef READ_ADC_H
#define	READ_ADC_H

#include "pt.h"
#include "mcc_generated_files/mcc.h"
#include "pt-sem.h"
#include "time_system.h"
#include "GV300.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


#define BUFFER_ADC_SIZE 5

#define Vo (0.4)
#define Tc (0.01953)




//Tiempo de captura de luminosidad Fotocelda 
#define INTERVAL_BETWEEN_SAMPLES 1000UL




void startReadAdc(void);
void executeReadAdc(void);

#endif	/* READ_ADC_H */

