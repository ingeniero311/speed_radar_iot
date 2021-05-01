
#ifndef MUX_H
#define	MUX_H

#include <string.h>
#include "pt.h"
#include "mcc_generated_files/mcc.h"
#include "time_system.h"
#include "GV300.h"


/**
 * 
 */
void startMux(void);
void executeMux(void);
void receiveRadarSpeed(char *radarSpeed);
int updateDigits(struct pt *pt, char *vel);


#endif	/* MUX_H */

