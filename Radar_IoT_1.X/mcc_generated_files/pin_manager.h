/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.3
        Device            :  PIC18F27K42
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB 	          :  MPLAB X 5.40	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set ADC_BAT aliases
#define ADC_BAT_TRIS                 TRISAbits.TRISA0
#define ADC_BAT_LAT                  LATAbits.LATA0
#define ADC_BAT_PORT                 PORTAbits.RA0
#define ADC_BAT_WPU                  WPUAbits.WPUA0
#define ADC_BAT_OD                   ODCONAbits.ODCA0
#define ADC_BAT_ANS                  ANSELAbits.ANSELA0
#define ADC_BAT_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define ADC_BAT_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define ADC_BAT_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define ADC_BAT_GetValue()           PORTAbits.RA0
#define ADC_BAT_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define ADC_BAT_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define ADC_BAT_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define ADC_BAT_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define ADC_BAT_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define ADC_BAT_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define ADC_BAT_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define ADC_BAT_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set RA1 procedures
#define RA1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define RA1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define RA1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define RA1_GetValue()              PORTAbits.RA1
#define RA1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define RA1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define RA1_SetPullup()             do { WPUAbits.WPUA1 = 1; } while(0)
#define RA1_ResetPullup()           do { WPUAbits.WPUA1 = 0; } while(0)
#define RA1_SetAnalogMode()         do { ANSELAbits.ANSELA1 = 1; } while(0)
#define RA1_SetDigitalMode()        do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set ADC_FOT aliases
#define ADC_FOT_TRIS                 TRISAbits.TRISA2
#define ADC_FOT_LAT                  LATAbits.LATA2
#define ADC_FOT_PORT                 PORTAbits.RA2
#define ADC_FOT_WPU                  WPUAbits.WPUA2
#define ADC_FOT_OD                   ODCONAbits.ODCA2
#define ADC_FOT_ANS                  ANSELAbits.ANSELA2
#define ADC_FOT_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define ADC_FOT_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define ADC_FOT_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define ADC_FOT_GetValue()           PORTAbits.RA2
#define ADC_FOT_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define ADC_FOT_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define ADC_FOT_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define ADC_FOT_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define ADC_FOT_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define ADC_FOT_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define ADC_FOT_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define ADC_FOT_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set ADC_HUM aliases
#define ADC_HUM_TRIS                 TRISAbits.TRISA3
#define ADC_HUM_LAT                  LATAbits.LATA3
#define ADC_HUM_PORT                 PORTAbits.RA3
#define ADC_HUM_WPU                  WPUAbits.WPUA3
#define ADC_HUM_OD                   ODCONAbits.ODCA3
#define ADC_HUM_ANS                  ANSELAbits.ANSELA3
#define ADC_HUM_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define ADC_HUM_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define ADC_HUM_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define ADC_HUM_GetValue()           PORTAbits.RA3
#define ADC_HUM_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define ADC_HUM_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define ADC_HUM_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define ADC_HUM_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define ADC_HUM_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define ADC_HUM_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define ADC_HUM_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define ADC_HUM_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set FAN aliases
#define FAN_TRIS                 TRISAbits.TRISA4
#define FAN_LAT                  LATAbits.LATA4
#define FAN_PORT                 PORTAbits.RA4
#define FAN_WPU                  WPUAbits.WPUA4
#define FAN_OD                   ODCONAbits.ODCA4
#define FAN_ANS                  ANSELAbits.ANSELA4
#define FAN_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define FAN_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define FAN_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define FAN_GetValue()           PORTAbits.RA4
#define FAN_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define FAN_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define FAN_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define FAN_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define FAN_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define FAN_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define FAN_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define FAN_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set ADC_MCP aliases
#define ADC_MCP_TRIS                 TRISAbits.TRISA5
#define ADC_MCP_LAT                  LATAbits.LATA5
#define ADC_MCP_PORT                 PORTAbits.RA5
#define ADC_MCP_WPU                  WPUAbits.WPUA5
#define ADC_MCP_OD                   ODCONAbits.ODCA5
#define ADC_MCP_ANS                  ANSELAbits.ANSELA5
#define ADC_MCP_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define ADC_MCP_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define ADC_MCP_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define ADC_MCP_GetValue()           PORTAbits.RA5
#define ADC_MCP_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define ADC_MCP_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define ADC_MCP_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define ADC_MCP_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define ADC_MCP_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define ADC_MCP_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define ADC_MCP_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define ADC_MCP_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set RA6 procedures
#define RA6_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define RA6_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define RA6_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define RA6_GetValue()              PORTAbits.RA6
#define RA6_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define RA6_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define RA6_SetPullup()             do { WPUAbits.WPUA6 = 1; } while(0)
#define RA6_ResetPullup()           do { WPUAbits.WPUA6 = 0; } while(0)
#define RA6_SetAnalogMode()         do { ANSELAbits.ANSELA6 = 1; } while(0)
#define RA6_SetDigitalMode()        do { ANSELAbits.ANSELA6 = 0; } while(0)

// get/set LED_OK aliases
#define LED_OK_TRIS                 TRISAbits.TRISA7
#define LED_OK_LAT                  LATAbits.LATA7
#define LED_OK_PORT                 PORTAbits.RA7
#define LED_OK_WPU                  WPUAbits.WPUA7
#define LED_OK_OD                   ODCONAbits.ODCA7
#define LED_OK_ANS                  ANSELAbits.ANSELA7
#define LED_OK_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define LED_OK_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define LED_OK_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define LED_OK_GetValue()           PORTAbits.RA7
#define LED_OK_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define LED_OK_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define LED_OK_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define LED_OK_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define LED_OK_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define LED_OK_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define LED_OK_SetAnalogMode()      do { ANSELAbits.ANSELA7 = 1; } while(0)
#define LED_OK_SetDigitalMode()     do { ANSELAbits.ANSELA7 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()              PORTBbits.RB0
#define RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()             do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()           do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode()         do { ANSELBbits.ANSELB0 = 1; } while(0)
#define RB0_SetDigitalMode()        do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()              PORTBbits.RB1
#define RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()             do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()           do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode()         do { ANSELBbits.ANSELB1 = 1; } while(0)
#define RB1_SetDigitalMode()        do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set RB2 procedures
#define RB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define RB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define RB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RB2_GetValue()              PORTBbits.RB2
#define RB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define RB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define RB2_SetPullup()             do { WPUBbits.WPUB2 = 1; } while(0)
#define RB2_ResetPullup()           do { WPUBbits.WPUB2 = 0; } while(0)
#define RB2_SetAnalogMode()         do { ANSELBbits.ANSELB2 = 1; } while(0)
#define RB2_SetDigitalMode()        do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RB3 procedures
#define RB3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define RB3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define RB3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define RB3_GetValue()              PORTBbits.RB3
#define RB3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define RB3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define RB3_SetPullup()             do { WPUBbits.WPUB3 = 1; } while(0)
#define RB3_ResetPullup()           do { WPUBbits.WPUB3 = 0; } while(0)
#define RB3_SetAnalogMode()         do { ANSELBbits.ANSELB3 = 1; } while(0)
#define RB3_SetDigitalMode()        do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set AN1 aliases
#define AN1_TRIS                 TRISBbits.TRISB4
#define AN1_LAT                  LATBbits.LATB4
#define AN1_PORT                 PORTBbits.RB4
#define AN1_WPU                  WPUBbits.WPUB4
#define AN1_OD                   ODCONBbits.ODCB4
#define AN1_ANS                  ANSELBbits.ANSELB4
#define AN1_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define AN1_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define AN1_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define AN1_GetValue()           PORTBbits.RB4
#define AN1_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define AN1_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define AN1_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define AN1_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define AN1_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define AN1_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define AN1_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define AN1_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set AN2 aliases
#define AN2_TRIS                 TRISBbits.TRISB5
#define AN2_LAT                  LATBbits.LATB5
#define AN2_PORT                 PORTBbits.RB5
#define AN2_WPU                  WPUBbits.WPUB5
#define AN2_OD                   ODCONBbits.ODCB5
#define AN2_ANS                  ANSELBbits.ANSELB5
#define AN2_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define AN2_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define AN2_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define AN2_GetValue()           PORTBbits.RB5
#define AN2_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define AN2_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define AN2_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define AN2_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define AN2_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define AN2_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define AN2_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define AN2_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set SEG_A aliases
#define SEG_A_TRIS                 TRISCbits.TRISC0
#define SEG_A_LAT                  LATCbits.LATC0
#define SEG_A_PORT                 PORTCbits.RC0
#define SEG_A_WPU                  WPUCbits.WPUC0
#define SEG_A_OD                   ODCONCbits.ODCC0
#define SEG_A_ANS                  ANSELCbits.ANSELC0
#define SEG_A_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define SEG_A_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define SEG_A_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define SEG_A_GetValue()           PORTCbits.RC0
#define SEG_A_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define SEG_A_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define SEG_A_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define SEG_A_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define SEG_A_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define SEG_A_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define SEG_A_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define SEG_A_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set SEG_B aliases
#define SEG_B_TRIS                 TRISCbits.TRISC1
#define SEG_B_LAT                  LATCbits.LATC1
#define SEG_B_PORT                 PORTCbits.RC1
#define SEG_B_WPU                  WPUCbits.WPUC1
#define SEG_B_OD                   ODCONCbits.ODCC1
#define SEG_B_ANS                  ANSELCbits.ANSELC1
#define SEG_B_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define SEG_B_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define SEG_B_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define SEG_B_GetValue()           PORTCbits.RC1
#define SEG_B_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define SEG_B_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define SEG_B_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define SEG_B_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define SEG_B_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define SEG_B_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define SEG_B_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define SEG_B_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set SEG_C aliases
#define SEG_C_TRIS                 TRISCbits.TRISC2
#define SEG_C_LAT                  LATCbits.LATC2
#define SEG_C_PORT                 PORTCbits.RC2
#define SEG_C_WPU                  WPUCbits.WPUC2
#define SEG_C_OD                   ODCONCbits.ODCC2
#define SEG_C_ANS                  ANSELCbits.ANSELC2
#define SEG_C_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define SEG_C_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define SEG_C_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define SEG_C_GetValue()           PORTCbits.RC2
#define SEG_C_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define SEG_C_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define SEG_C_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define SEG_C_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define SEG_C_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define SEG_C_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define SEG_C_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define SEG_C_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set SEG_D aliases
#define SEG_D_TRIS                 TRISCbits.TRISC3
#define SEG_D_LAT                  LATCbits.LATC3
#define SEG_D_PORT                 PORTCbits.RC3
#define SEG_D_WPU                  WPUCbits.WPUC3
#define SEG_D_OD                   ODCONCbits.ODCC3
#define SEG_D_ANS                  ANSELCbits.ANSELC3
#define SEG_D_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define SEG_D_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define SEG_D_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define SEG_D_GetValue()           PORTCbits.RC3
#define SEG_D_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define SEG_D_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define SEG_D_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define SEG_D_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define SEG_D_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define SEG_D_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define SEG_D_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define SEG_D_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set SEG_E aliases
#define SEG_E_TRIS                 TRISCbits.TRISC4
#define SEG_E_LAT                  LATCbits.LATC4
#define SEG_E_PORT                 PORTCbits.RC4
#define SEG_E_WPU                  WPUCbits.WPUC4
#define SEG_E_OD                   ODCONCbits.ODCC4
#define SEG_E_ANS                  ANSELCbits.ANSELC4
#define SEG_E_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SEG_E_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SEG_E_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SEG_E_GetValue()           PORTCbits.RC4
#define SEG_E_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SEG_E_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define SEG_E_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define SEG_E_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define SEG_E_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define SEG_E_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define SEG_E_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define SEG_E_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set SEG_F aliases
#define SEG_F_TRIS                 TRISCbits.TRISC5
#define SEG_F_LAT                  LATCbits.LATC5
#define SEG_F_PORT                 PORTCbits.RC5
#define SEG_F_WPU                  WPUCbits.WPUC5
#define SEG_F_OD                   ODCONCbits.ODCC5
#define SEG_F_ANS                  ANSELCbits.ANSELC5
#define SEG_F_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define SEG_F_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define SEG_F_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define SEG_F_GetValue()           PORTCbits.RC5
#define SEG_F_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define SEG_F_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define SEG_F_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define SEG_F_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define SEG_F_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define SEG_F_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define SEG_F_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define SEG_F_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set SEG_G aliases
#define SEG_G_TRIS                 TRISCbits.TRISC6
#define SEG_G_LAT                  LATCbits.LATC6
#define SEG_G_PORT                 PORTCbits.RC6
#define SEG_G_WPU                  WPUCbits.WPUC6
#define SEG_G_OD                   ODCONCbits.ODCC6
#define SEG_G_ANS                  ANSELCbits.ANSELC6
#define SEG_G_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define SEG_G_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define SEG_G_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define SEG_G_GetValue()           PORTCbits.RC6
#define SEG_G_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define SEG_G_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define SEG_G_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define SEG_G_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define SEG_G_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define SEG_G_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define SEG_G_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define SEG_G_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set AN3 aliases
#define AN3_TRIS                 TRISCbits.TRISC7
#define AN3_LAT                  LATCbits.LATC7
#define AN3_PORT                 PORTCbits.RC7
#define AN3_WPU                  WPUCbits.WPUC7
#define AN3_OD                   ODCONCbits.ODCC7
#define AN3_ANS                  ANSELCbits.ANSELC7
#define AN3_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define AN3_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define AN3_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define AN3_GetValue()           PORTCbits.RC7
#define AN3_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define AN3_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define AN3_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define AN3_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define AN3_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define AN3_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define AN3_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define AN3_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/