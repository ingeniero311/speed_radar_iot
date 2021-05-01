

#include "mcc_generated_files/mcc.h"
#include "mux.h"
#include "radar_uart.h"
#include "read_adc.h"
#include "GV300.h"
#include "eeprom_utils.h"
#include "tasks_global.h"
#include "fan_control.h"
#include "maintenance.h"

void main(void) {
    // Initialize the device
    SYSTEM_Initialize();

    iniValueEeprom();

    startMux();
    startRadarUart();
    startGV300Uart();
    startReadAdc();

    startFanControl();
    startMaintenance();
    startGlobalTask();


    // Enable high priority global interrupts
    INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    INTERRUPT_GlobalInterruptLowEnable();


    while (1) {
        executeMux();
        executeRadarUart();
        executeReadAdc();
        executeGV300Uart();
        executeFanControl();
        executeMaintenance();
        executeGlobalTask();

    }
}
