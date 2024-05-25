/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stddef.h>
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

// define
// define external_memory
#define BASE_ADD 0x1100 

// chip-select (CS) definition (offset)
#define CS0 0
#define CS1 1
#define CS2 2
#define CS3 3
#define CS4 4
#define CS5 5
#define CS6 6 
#define CS7 7
#define CS8 8
#define CS9 9
#define CS10 10
#define CS11 11
#define CS12 12
#define CS13 13
#define CS14 14
#define CS15 15
#define CS16 16
#define CS17 17
#define CS18 18
#define CS19 19

// define external devices
#define LED0 *(unsigned char*) (BASE_ADD + CS0)
#define LED1 *(unsigned char*) (BASE_ADD + CS1)
#define LED2 *(unsigned char*) (BASE_ADD + CS2)
#define LED3 *(unsigned char*) (BASE_ADD + CS3)

#define LED7D1 *(unsigned char*) (BASE_ADD + CS4)
#define LED7D2 *(unsigned char*) (BASE_ADD + CS5)
#define LED7D3 *(unsigned char*) (BASE_ADD + CS6)
#define LED7D4 *(unsigned char*) (BASE_ADD + CS7)

#define LED7CTRL *(unsigned char*) (BASE_ADD + CS8)
#define LED7DATA *(unsigned char*) (BASE_ADD + CS9)

#define RELAYMAP *(unsigned char*) (BASE_ADD + CS10)
#define RELAY_ACTIVATE (*(volatile unsigned char*) (BASE_ADD+CS10) = *(unsigned char*)(&relays ))
struct bits{
    unsigned char RL0 :1;
    unsigned char RL1 :1;
    unsigned char RL2 :1;
    unsigned char RL3 :1;
    unsigned char RL4 :1;
    unsigned char RL5 :1;
    unsigned char RL6 :1;
    unsigned char RL7 :1;
};

union RL{
    unsigned char relayAll;
    struct bits relay;
};

union RL relays;

struct Motor
{
    unsigned char DC1 : 2;
    unsigned char DC2 : 2;
    unsigned char DCfree : 4;
};

#define STOP 0
#define FORWARD 1
#define BACKWARD 2

struct Motor Motors;
#define MOTOR_ACTIVATE (*(volatile unsigned char*) (BASE_ADD + CS13) = *(unsigned char*) (&Motors))

struct Button{
    unsigned char LEFT :1;
    unsigned char RIGHT :1;
    unsigned char UP :1;
    unsigned char DOWN :1;
    unsigned char GO :1;
    unsigned char BTN1 :1;
    unsigned char BTN2 :1;
    unsigned char BTN3 :1;
};

struct Button Buttons;
#define READKEY (*(unsigned char*) (&Buttons) = *(volatile unsigned char*)(BASE_ADD + CS14))

#define LCD_DATA *(unsigned char*) (BASE_ADD + CS11)
#define LCD_INS *(unsigned char*) (BASE_ADD + CS12)
#define LCDE_H (PORTG |= (1<<3))
#define LCDE_L (PORTG &= ~(1<<3))

//---------------------------------------//
#define ON 1
#define OFF 0
#define Device_Relay 0
#define Device_Motor1 1
#define Device_Motor2 2
#define Device_LED 3

// Define function
void ActuatorsActivation(char Devices, char Status)
{
    switch(Devices)
    {
        case Device_Relay:
            if(Status) relays.relayAll = 0xff;
            else relays.relayAll = 0x00;
            RELAY_ACTIVATE;
            break;
        case Device_Motor1:
            Motors.DC1 = Status;
            MOTOR_ACTIVATE;
            break;
        case Device_Motor2:
            Motors.DC2 = Status;
            MOTOR_ACTIVATE;
            break;
        case Device_LED:
            if(Status == ON) LED0 = 0xff;
            else LED0 = 0x00;
            break;
    }
}

// ------------------------------------- //
typedef struct Linker{
    char MenuID;
    struct Linker *pre;
    char Title[20];
    char List1[20];
    struct Linker *MenuList1; void (*ActivationON)(char, char);
    char List2[20];
    struct Linker *MenuList2; void (*ActivationOFF)(char, char);
    char List3[20];
    struct Linker *MenuList3;
} Menu;

Menu MainMenu, SensorMenu, ActuratorMenu, SettingMenu, 
        TempMenu, HumidityMenu, MotorMenu, Motor1Menu,
        Motor2Menu, RelaysMenu, LedMenu;

Menu MainMenu = {
    NULL,
    NULL,
    "      MAIN MENU    ",
    "    SENSORS       ",
    &SensorMenu, NULL,
    "    ACTUATOR      ",
    &ActuratorMenu, NULL,
    "    SETTING       ",
    &SettingMenu,
};

Menu SensorMenu = {
    NULL,
    &MainMenu,
    "    SENSOR         ",
    "    TEMPERATURE    ",
    &TempMenu, NULL,
    "    HUMIDITY       ",
    &HumidityMenu, NULL,
    "    DATE & TIME    ",
    NULL,
};
Menu TempMenu = {
    NULL,
    &SensorMenu,
    "    TEMPERTURE     ",
    "    ON             ",
    NULL, NULL,
    "    OFF            ",
    NULL, NULL,
    "                   ",
    NULL,
};
Menu HumidityMenu = {
    NULL,
    &SensorMenu,
    "    HUMIDITY       ",
    "    ON             ",
    NULL, NULL,
    "    OFF            ",
    NULL, NULL,
    "                   ",
    NULL,
};

/*----------------------------*/
Menu ActuratorMenu = {
    NULL,
    &MainMenu,
    "    ACTUATOR MENU  ",
    "    RELAYS         ",
    &RelaysMenu, NULL,
    "    MOTORS         ",
    &MotorMenu, NULL,
    "    LEDS           ",
    &LedMenu,
};
Menu MotorMenu = {
    NULL,
    &ActuratorMenu,
    "    MOTORS         ",
    "    MOTOR 01       ",
    &Motor1Menu, NULL,
    "    MOTOR 02       ",
    &Motor2Menu, NULL,
    "                   ",
    NULL,
};
Menu Motor1Menu = {
    Device_Motor1,
    &MotorMenu,
    "    MOTOR 01       ",
    "    ON             ",
    NULL, ActuatorsActivation,
    "    OF             ",
    NULL, ActuatorsActivation,
    "                   ",
    NULL,
};
Menu Motor2Menu = {
    Device_Motor2,
    &MotorMenu,
    "    MOTOR 02       ",
    "    ON             ",
    NULL, ActuatorsActivation,
    "    OF             ",
    NULL, ActuatorsActivation,
    "                   ",
    NULL,
};
Menu RelaysMenu = {
    Device_Relay,
    &ActuratorMenu,
    "    RELAY          ",
    "    ON             ",
    NULL, &ActuatorsActivation,
    "    OF             ",
    NULL, &ActuatorsActivation,
    "                   ",
    NULL,
};
Menu LedMenu = {
    Device_LED,
    &ActuratorMenu,
    "    LED            ",
    "    ON             ",
    NULL, ActuatorsActivation,
    "    OF             ",
    NULL, ActuatorsActivation,
    "                   ",
    NULL,
};
/*--------------------------------*/
Menu SettingMenu = {
    NULL,
    &MainMenu,
    "     SETTING MENU  ",
    "    TIME           ",
    NULL, NULL,
    "    SPEED          ",
    NULL, NULL,
    "    CHANNELS       ",
    NULL,
};


//-------------- ONE WIRE -----------------------//
// Definition of DS18B20
#define DS18B20 0
#define OneWire_High (PORTB|=(1<<DS18B20))
#define OneWire_LOW (PORTB&=~(1<<DS18B20))
#define OneWire_Master (DDRB |=(1<<DS18B20))
#define OneWire_Free (DDRB &=~(1<<DS18B20))

#define OneWire_State (PINB &(1<<DS18B20))