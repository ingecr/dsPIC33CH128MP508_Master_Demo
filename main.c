/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.125.1
        Device            :  dsPIC33CH128MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB 	          :  MPLAB X v5.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/Thermo5Drivers/Thermo5.h"
#include "mcc_generated_files/delay.h"
#include "mcc_generated_files/pin_manager.h"
#include "LCD/lcd.h"

#define FCY 8000000UL
#define LED_D10_TRIS    TRISEbits.TRISE7
#define LED_D10_LAT     LATEbits.LATE7
#define LED_ON  1
#define LED_OFF 0

#define INPUT  1
#define OUTPUT 0
 

uint16_t internal_temp  =   0;
uint16_t temp_1         =   0;
uint16_t temp_2         =   0;
uint16_t temp_3         =   0;

int main(void)
{   //LED_D10_TRIS = INPUT;
    // initialize the device
    SYSTEM_Initialize();
    LCD_Initialize ();
    printf("\fThermo5 Example \r\n Temperatures");
    while (1)
    {
      internal_temp =   (uint16_t)Thermo5_ReadTemperature(INTERNAL_DIODE);
      temp_1        =   (uint16_t)Thermo5_ReadTemperature(DIODE_1); 
      temp_2        =   (uint16_t)Thermo5_ReadTemperature(DIODE_2);
      temp_3        =   (uint16_t)Thermo5_ReadTemperature(DIODE_3);
      printf("\f0:%d(c) 1:%d(c)\r\n2:%d(c) 3:%d(c)",internal_temp,temp_1,temp_2,temp_3);
      DELAY_milliseconds(100);
      //LED_D10_LAT ^= 1 ;
      LED_D4_Toggle();
    }
    return 1; 
}
/**
 End of File
*/
