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

#include <stdio.h>
#include <stdbool.h>
#include "pin_manager.h"
#include "delay.h"
#include "Thermo5Drivers/Thermo5.h"

static void ExampleHandler(struct INTERRUPTS status)
{
    if(status.high_limit.internal)
    {
        printf("High limit flag triggered\r\n");
    }
}

void Thermo5_Example(void) 
{
    Thermo5_SetCallback(&ExampleHandler);
    Thermo5_SetHighLimit(INTERNAL_DIODE, 190);

    while (1) 
    {
        float data;
        bool nAlert;
        
        DELAY_milliseconds(1000);
        data = Thermo5_ReadTemperature(INTERNAL_DIODE);
        
        //printf("Value is: %3.2f\r\n", (double)data);
        
        nAlert = PORTAbits.RA3 /* get Thermo5_Interrupt value */;
        if(!nAlert)
        {
            Thermo5_ISR();
        }
    }
}
