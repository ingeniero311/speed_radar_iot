/* 
 * File:   GV300.h
 * Author: Vialtec Colombia SAS
 *
 * Created on December 12, 2020, 2:08 PM
 */

#ifndef GV300_H
#define	GV300_H




#include "pt.h"
#include "mcc_generated_files/mcc.h"
#include "time_system.h"
#include "eeprom_utils.h"
#include "read_adc.h"

//Tiempo (en milisegundos) para liberar la UART del modem, y evitar que se "peguen" las transmisiones
#define UART_MUTEX_DELAY 2000

//Evento de reset automático
#define STR_EVENT_AUTO_RESET "AT+GTDAT=gv300,0,,[ASR100|DS|1.0|EVT|ART],0,,,,FFFF$\r\n"

//Evento de arranque de la tarjeta
#define STR_EVENT_BOOT "AT+GTDAT=gv300,0,,[ASR100|DS|1.0|EVT|DBT],0,,,,FFFF$\r\n"

//Ventilador encendido
#define EVN_FAN_ON "AT+GTDAT=gv300,0,,[ASR100|DS|1.0|EVT|FON],0,,,,FFFF$\r\n"

//Ventilador apagado
#define EVN_FAN_OFF "AT+GTDAT=gv300,0,,[ASR100|DS|1.0|EVT|FOF],0,,,,FFFF$\r\n"

#define TRANSPARENT_MSG_HEADER "AT+GTDAT=gv300,0,,[ASR100|DS|1.0|EVT|SRP"

#define TRANSPARENT_MSG_FOOTER "],0,,,,FFFF$\r\n"


//Evento de Higrometro
#define EVN_HGM_INI "AT+GTDAT=gv300,0,,[ASR100|DS|1.0|EVT|HGR|"

#define EVN_HGM_FIN "],0,,,,FFFF$\r\n"


void executeGV300Uart(void);

void startGV300Uart(void);

void sendUart2Str(char *str);

void removeChars( char* str, char c);

#endif	/* RADAR_UART_H */