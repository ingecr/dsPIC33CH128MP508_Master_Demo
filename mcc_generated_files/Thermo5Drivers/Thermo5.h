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

#ifndef THERMO5_H
#define	THERMO5_H

#include <stdint.h>
#include <stdbool.h>

struct ALERT_REGISTER 
{
    bool triggered;
    bool internal;
    bool diode_1;
    bool diode_2;
    bool diode_3;
};

struct INTERRUPTS 
{
    bool busy;
    struct ALERT_REGISTER high_limit;
    struct ALERT_REGISTER low_limit;
    struct ALERT_REGISTER therm_limit;
    struct ALERT_REGISTER fault;
};

enum DIODES 
{
    INTERNAL_DIODE, 
    DIODE_1, 
    DIODE_2, 
    DIODE_3
};

float Thermo5_ReadTemperature(uint8_t sensor);

void Thermo5_SetHighLimit(uint8_t sensor, int limit);
void Thermo5_SetLowLimit(uint8_t sensor, int limit);
void Thermo5_SetThermLimit(uint8_t sensor, int limit);

void Thermo5_SetIdeality(uint8_t sensor, uint8_t ideality);
void Thermo5_SetBeta(uint8_t sensor, uint8_t beta);
void Thermo5_SetHysteresis(uint8_t hysteresis);

void Thermo5_Initialize(void);

void Thermo5_SetCallback(void (*callback)(struct INTERRUPTS));
void Thermo5_ISR(void);

/* For backwards compatibility */
void Thermo5_SetInterruptHandler(void* vector);

#endif
