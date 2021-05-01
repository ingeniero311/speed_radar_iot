#include "time_system.h"


volatile unsigned long globalMillis=0;

unsigned long millis(void){
    
    unsigned long localMillis=0;
    TMR0IE=0;
    
    localMillis=globalMillis;
    
    TMR0IE=1;
    
    return localMillis;    
    
}
