/*
 * File:   newmain.c
 * Author: Acer
 *
 * Created on May 13, 2024, 11:27 PM
 */


#include <xc.h>
#include "userDef.h"
#define F_CPU 1000000
#include <util/delay.h>

unsigned long led = 0x55555555;
int i;
unsigned char SegCode[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
        0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
// 0 1 2 3 4 5 6 7 8 9 A B C D E F

void seven_Segment(unsigned int value)
{
    LED7D1 = SegCode[value%10];
    LED7D2 = SegCode[(value/10)%10];
    LED7D3 = SegCode[(value/100)%10];
    LED7D4 = SegCode[(value/1000)%10];
    
}

void seven_display_scan(unsigned int value)
{
    LED7DATA = SegCode[value%10];
    LED7CTRL = 0b11111110;
    _delay_ms(500);
    LED7CTRL = 0xff;
    
    LED7DATA = SegCode[(value/10)%10];
    LED7CTRL = 0b11111101;
    _delay_ms(500);
    LED7CTRL = 0xff;
    
    LED7DATA = SegCode[(value/100)%10];
    LED7CTRL = 0b11111011;
    _delay_ms(500);
    LED7CTRL = 0xff;
    
    LED7DATA = SegCode[(value/1000)%10];
    LED7CTRL = 0b11110111;
    _delay_ms(500);
    LED7CTRL = 0xff;
    
}

void LcdInit()
{
    LCD_INS = 0x38; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1);
    _delay_us(200);
    LCD_INS = 0x0C; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1);
    _delay_us(200);
    LCD_INS = 0x06; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1);
    _delay_us(200);
    LCD_INS = 0x01; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1);
    _delay_ms(20);
}

void led_display(unsigned long led){
    LED0 = led&0x000000ff;
    LED1 = (led>>8)&0x000000ff;
    LED2 = (led>>16)&0x000000ff;
    LED3 = (led>>24)&0x000000ff;
}

void PrintLcd(const char *str, unsigned line, unsigned col)
{
    unsigned char add;
    switch (line)
    {
        case 0: add = 0x80; break;
        case 1: add = 0xC0; break;
        case 2: add = 0x94; break;
        case 3: add = 0xD4; break;
    }
    LCD_INS = add + col;
    LCDE_H; _delay_us(1); LCDE_L; _delay_us(50);
    while(*(str)!='\0')
    {
        LCD_DATA = *str++;
        LCDE_H; _delay_us(1); LCDE_L; _delay_us(50);
    }
}
void MenuDisplay(Menu *menu, unsigned char select)
{
    PrintLcd(menu->Title, 0, 0);
    PrintLcd(menu->List1, 1, 0);
    PrintLcd(menu->List2, 2, 0);
    PrintLcd(menu->List3, 3, 0);
    
    PrintLcd(">", select, 0);
}

char OneWireReset(void)
{
    char Presence;
    OneWire_Master;
    OneWire_LOW;
    _delay_us(480);
    OneWire_Free;
    _delay_us(80);
    Presence = OneWire_State;
    _delay_us(60);
}

OneWireWriteByte(unsigned char Byte)
{
    unsigned char i;
    for(i=0; i<8; i++)
    {
        OneWire_Master;
        if(Byte&0x01)
        {
            OneWire_LOW;
            _delay_us(15);
            OneWire_Free;
            _delay_us(45);
            _delay_us(50);
        }
        else
        {
            OneWire_LOW;
            _delay_us(15);
            _delay_us(45);
            OneWire_Free;
            _delay_us(30);
        }
        Byte = Byte >> 1;
    }
}

unsigned char OneWireRead(void)
{
    unsigned char i;
    unsigned Byte=0;
    
    for(i=0; i<8; i++)
    {
        OneWire_Master;
        OneWire_LOW;
        _delay_us(15);
        OneWire_Free;
        _delay_us(15);
        Byte = (Byte>>1) |(OneWire_State<<7);
        _delay_us(30);
    }
    return Byte;
}

unsigned int ReadTemp(void)
{
    unsigned char data[2];
    if(!OneWireReset())
    {
        OneWireWriteByte(0xCC);
        OneWireWriteByte(0x44);
        OneWire_Free;
        _delay_ms(750);
        data[0] = OneWireReset();
        OneWireWriteByte(0xCC);
        OneWireWriteByte(0xBE);
        data[0] = OneWireRead();
        data[1] = OneWireRead();
        return (data[0]+data[1]*256)*0.0625;
    }
    else
        return 0;
}

void main(void) {
    unsigned char data;
    unsigned char temp;
    MCUCR |= 0x80; //external memory wait state ON, 1 wait state
    XMCRA = 0;
    DDRG = 0x0f;
    
    Menu *menu;
    menu = &MainMenu;
    unsigned char select = 1;
    LcdInit();
    PrintLcd("Test one-wire", 0, 0);
    
    while(1){
        temp = ReadTemp();
        seven_Segment(temp);
    }
    //MenuDisplay(menu, select);
    
//    while(1)
//    {
//        READKEY;
//        if(!Buttons.DOWN)
//        {
//            while(!Buttons.DOWN) READKEY;
//            select = (select==3)?1:select+1;
//        }
//        if(!Buttons.UP)
//        {
//            while(!Buttons.UP) READKEY;
//            select = (select==1)?3:select-1;
//        }
//        if(!Buttons.RIGHT)
//        {
//            while(!Buttons.RIGHT) READKEY;
//            switch(select)
//            {
//                case 1: menu = (menu->MenuList1 == NULL) ? menu:menu->MenuList1; break;
//                case 2: menu = (menu->MenuList2 == NULL) ? menu:menu->MenuList2; break;
//                case 3: menu = (menu->MenuList3 == NULL) ? menu:menu->MenuList3; break;
//            }
//            select = 1;
//        }
//        if(!Buttons.LEFT)
//        {
//            while(!Buttons.LEFT) READKEY;
//            menu = (menu->pre == NULL) ? menu:menu->pre;
//            select = 1;
//        }
//        if(!Buttons.GO)
//        {
//            while(!Buttons.GO) READKEY;
//            switch(select)
//            {
//                case 1:
//                    if (menu->ActivationON != NULL)
//                        menu->ActivationON(menu->MenuID, ON);
//                    break;
//                case 2:
//                    if (menu->ActivationOFF != NULL)
//                        menu->ActivationOFF(menu->MenuID, OFF);
//                    break;
//            }
//        }
//        MenuDisplay(menu, select);
//        _delay_ms(100);
//    }
    
    return;
}
