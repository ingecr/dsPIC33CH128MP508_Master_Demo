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

#include "Thermo5.h"
#include "../drivers/i2c_simple_master.h"

#define ADDRESS 0b1001100
#define CONVERSION_FACTOR 8
#define EXTENDED_OFFSET_FACTOR 64

#define MAX_TEMP 191
#define MIN_TEMP -64

#define INTERNAL_DIODE_DATA_HIGH_BYTE 0x00
#define EXTERNAL_DIODE_1_DATA_HIGH_BYTE 0x01
#define STATUS_REGISTER 0x02
#define CONFIGURATION 0x03
#define CONVERSION_RATE 0x04
#define INTERNAL_DIODE_HIGH_LIMIT 0x05
#define INTERNAL_DIODE_LOW_LIMIT 0x06
#define EXTERNAL_DIODE_1_HIGH_LIMIT_HIGH_BYTE 0x07
#define EXTERNAL_DIODE_1_LOW_LIMIT_HIGH_BYTE 0x08
#define ONE_SHOT 0x0F
#define EXTERNAL_DIODE_1_DATA_LOW_BYTE 0x10
#define SCRATCHPAD_1 0x11
#define SCRATCHPAD_2 0x12
#define EXTERNAL_DIODE_1_HIGH_LIMIT_LOW_BYTE 0x13
#define EXTERNAL_DIODE_1_LOW_LIMIT_LOW_BYTE 0x14
#define EXTERNAL_DIODE_2_HIGH_LIMIT_HIGH_BYTE 0x15
#define EXTERNAL_DIODE_2_LOW_LIMIT_HIGH_BYTE 0x16
#define EXTERNAL_DIODE_2_HIGH_LIMIT_LOW_BYTE 0x17
#define EXTERNAL_DIODE_2_LOW_LIMIT_LOW_BYTE 0x18
#define EXTERNAL_DIODE_1_THERM_LIMIT 0x19
#define EXTERNAL_DIODE_2_THERM_LIMIT 0x1A
#define EXTERNAL_DIODE_FAULT 0x1B
#define CHANNEL_MASK_REGISTER 0x1F
#define INTERNAL_DIODE_THERM_LIMIT 0x20
#define THERM_HYSTERESIS 0x21
#define CONSECUTIVE_ALERT 0x22
#define EXTERNAL_DIODE_2_DATA_HIGH_BYTE 0x23
#define EXTERNAL_DIODE_2_DATA_LOW_BYTE 0x24
#define EXTERNAL_DIODE_1_BETA_CONFIGURATION 0x25
#define EXTERNAL_DIODE_2_BETA_CONFIGURATION 0x26
#define EXTERNAL_DIODE_1_IDEALITY_FACTOR 0x27
#define EXTERNAL_DIODE_2_IDEALITY_FACTOR 0x28
#define INTERNAL_DIODE_DATA_LOW_BYTE 0x29
#define EXTERNAL_DIODE_3_DATA_HIGH_BYTE 0x2A
#define EXTERNAL_DIODE_3_DATA_LOW_BYTE 0x2B
#define EXTERNAL_DIODE_3_HIGH_LIMIT_HIGH_BYTE 0x2C
#define EXTERNAL_DIODE_3_LOW_LIMIT_HIGH_BYTE 0x2D
#define EXTERNAL_DIODE_3_HIGH_LIMIT_LOW_BYTE 0x2E
#define EXTERNAL_DIODE_3_LOW_LIMIT_LOW_BYTE 0x2F
#define EXTERNAL_DIODE_3_THERM_LIMIT 0x30
#define EXTERNAL_DIODE_3_IDEALITY_FACTOR 0x31
#define HIGH_LIMIT_STATUS 0x35
#define LOW_LIMIT_STATUS 0x36
#define THERM_LIMIT_STATUS 0x37
#define FILTER_CONTROL 0x40
#define PRODUCT_ID 0xFD
#define MANUFACTURER_ID 0xFE
#define REVISION 0xFF

static void (*Thermo5_InterruptHandler)(struct INTERRUPTS);

static uint8_t Read_Thermo_Register(uint8_t addr) 
{
    uint8_t data = i2c_read1ByteRegister(ADDRESS, addr);
    return data;
}

static void Write_Thermo_Register(uint8_t addr, uint8_t val) 
{
    i2c_write1ByteRegister(ADDRESS, addr, val);
}

float Thermo5_ReadTemperature(uint8_t sensor) 
{
    uint16_t dataH, dataL;
    switch (sensor) 
    {
        case INTERNAL_DIODE:
            dataH = Read_Thermo_Register(INTERNAL_DIODE_DATA_HIGH_BYTE);
            dataL = Read_Thermo_Register(INTERNAL_DIODE_DATA_LOW_BYTE);
            break;
        case DIODE_1:
            dataH = Read_Thermo_Register(EXTERNAL_DIODE_1_DATA_HIGH_BYTE);
            dataL = Read_Thermo_Register(EXTERNAL_DIODE_1_DATA_LOW_BYTE);
            break;
        case DIODE_2:
            dataH = Read_Thermo_Register(EXTERNAL_DIODE_2_DATA_HIGH_BYTE);
            dataL = Read_Thermo_Register(EXTERNAL_DIODE_2_DATA_LOW_BYTE);
            break;
        case DIODE_3:
            dataH = Read_Thermo_Register(EXTERNAL_DIODE_3_DATA_HIGH_BYTE);
            dataL = Read_Thermo_Register(EXTERNAL_DIODE_3_DATA_LOW_BYTE);
            break;
        default:
            dataH = 0;
            dataL = 0;
            break;
    }
    float temp = (float) ((dataH << 3) + (dataL >> 5));
    temp /= CONVERSION_FACTOR;
    temp -= EXTENDED_OFFSET_FACTOR;
    return temp;
}

void Thermo5_SetHighLimit(uint8_t sensor, int limit) 
{
    if (limit > MAX_TEMP) 
    {
        limit = MAX_TEMP;
    } 
    else if (limit < MIN_TEMP) 
    {
        limit = MIN_TEMP;
    }
    
    limit += EXTENDED_OFFSET_FACTOR;

    uint8_t dat = (uint8_t) limit;

    switch (sensor) 
    {
        case INTERNAL_DIODE:
            Write_Thermo_Register(INTERNAL_DIODE_HIGH_LIMIT, dat);
            break;
        case DIODE_1:
            Write_Thermo_Register(EXTERNAL_DIODE_1_HIGH_LIMIT_HIGH_BYTE, dat);
            break;
        case DIODE_2:
            Write_Thermo_Register(EXTERNAL_DIODE_2_HIGH_LIMIT_HIGH_BYTE, dat);
            break;
        case DIODE_3:
            Write_Thermo_Register(EXTERNAL_DIODE_3_HIGH_LIMIT_HIGH_BYTE, dat);
            break;
        default:
            break;
    }

}

void Thermo5_SetLowLimit(uint8_t sensor, int limit) 
{
    if (limit > MAX_TEMP) 
    {
        limit = MAX_TEMP;
    } 
    else if (limit < MIN_TEMP) 
    {
        limit = MIN_TEMP;
    }

    limit += EXTENDED_OFFSET_FACTOR;

    uint8_t dat = (uint8_t) limit;

    switch (sensor) 
    {
        case INTERNAL_DIODE:
            Write_Thermo_Register(INTERNAL_DIODE_LOW_LIMIT, dat);
            break;
        case DIODE_1:
            Write_Thermo_Register(EXTERNAL_DIODE_1_LOW_LIMIT_HIGH_BYTE, dat);
            break;
        case DIODE_2:
            Write_Thermo_Register(EXTERNAL_DIODE_2_LOW_LIMIT_HIGH_BYTE, dat);
            break;
        case DIODE_3:
            Write_Thermo_Register(EXTERNAL_DIODE_3_LOW_LIMIT_HIGH_BYTE, dat);
            break;
        default:
            break;
    }
}

void Thermo5_SetIdeality(uint8_t sensor, uint8_t ideality) 
{
    switch (sensor) 
    {
        case DIODE_1:
            Write_Thermo_Register(EXTERNAL_DIODE_1_IDEALITY_FACTOR, ideality);
            break;
        case DIODE_2:
            Write_Thermo_Register(EXTERNAL_DIODE_2_IDEALITY_FACTOR, ideality);
            break;
        case DIODE_3:
            Write_Thermo_Register(EXTERNAL_DIODE_3_IDEALITY_FACTOR, ideality);
            break;
        default:
            break;
    }
}

void Thermo5_SetBeta(uint8_t sensor, uint8_t beta) 
{
    switch (sensor) 
    {
        case DIODE_1:
            Write_Thermo_Register(EXTERNAL_DIODE_1_BETA_CONFIGURATION, beta);
            break;
        case DIODE_2:
            Write_Thermo_Register(EXTERNAL_DIODE_2_BETA_CONFIGURATION, beta);
            break;
        default:
            break;
    }
}

void Thermo5_SetHysteresis(uint8_t hysteresis) 
{
    Write_Thermo_Register(THERM_HYSTERESIS, hysteresis);
}

void Thermo5_SetThermLimit(uint8_t sensor, int limit) 
{
    if (limit > MAX_TEMP) 
    {
        limit = MAX_TEMP;
    } 
    else if (limit < MIN_TEMP) 
    {
        limit = MIN_TEMP;
    }

    limit += EXTENDED_OFFSET_FACTOR;

    uint8_t dat = (uint8_t) limit;

    switch (sensor) 
    {
        case INTERNAL_DIODE:
            Write_Thermo_Register(INTERNAL_DIODE_THERM_LIMIT, dat);
            break;
        case DIODE_1:
            Write_Thermo_Register(EXTERNAL_DIODE_1_THERM_LIMIT, dat);
            break;
        case DIODE_2:
            Write_Thermo_Register(EXTERNAL_DIODE_2_THERM_LIMIT, dat);
            break;
        case DIODE_3:
            Write_Thermo_Register(EXTERNAL_DIODE_3_THERM_LIMIT, dat);
            break;
        default:
            break;
    }
}

void Thermo5_Initialize(void) 
{
    Thermo5_SetHighLimit(INTERNAL_DIODE, MAX_TEMP);
    Thermo5_SetLowLimit(INTERNAL_DIODE,MIN_TEMP);
    Thermo5_SetHighLimit(DIODE_1, MAX_TEMP);
    Thermo5_SetLowLimit(DIODE_1,MIN_TEMP);
    Thermo5_SetHighLimit(DIODE_2, MAX_TEMP);
    Thermo5_SetLowLimit(DIODE_2,MIN_TEMP);
    Thermo5_SetHighLimit(DIODE_3, MAX_TEMP);
    Thermo5_SetLowLimit(DIODE_3,MIN_TEMP);
    
    Write_Thermo_Register(CONFIGURATION, 0b00000100);
    Write_Thermo_Register(CHANNEL_MASK_REGISTER, 0b0000);
    Thermo5_SetHysteresis(0);
}

void Thermo5_SetInterruptHandler(void* vector) 
{
    Thermo5_InterruptHandler = vector;
}

void Thermo5_SetCallback(void (*callback)(struct INTERRUPTS))
{
    Thermo5_InterruptHandler = callback;
}

struct ALERT_REGISTER getAlertRegister(uint8_t addr) 
{
    struct ALERT_REGISTER WorkingRegister;
    
    uint8_t dat = Read_Thermo_Register(addr);
    
    WorkingRegister.triggered = true;
    
    if (dat & 0b1) 
    {
        WorkingRegister.internal = true;
    } 
    else 
    {
        WorkingRegister.internal = false;
    }
    
    if (dat & 0b10) 
    {
        WorkingRegister.diode_1 = true;
    } 
    else 
    {
        WorkingRegister.diode_1 = false;
    }

    if (dat & 0b100) 
    {
        WorkingRegister.diode_2 = true;
    } 
    else 
    {
        WorkingRegister.diode_2 = false;
    }

    if (dat & 0b1000) 
    {
        WorkingRegister.diode_3 = true;
    } 
    else 
    {
        WorkingRegister.diode_3 = false;
    }
    
    return WorkingRegister;
}

void Thermo5_ISR(void) 
{
    struct INTERRUPTS status;

    uint8_t stat = Read_Thermo_Register(STATUS_REGISTER);
    
    if (stat & 1 << 2) 
    {
        uint8_t dat = Read_Thermo_Register(EXTERNAL_DIODE_FAULT);
        status.fault = getAlertRegister(dat);
    } 
    else 
    {
        status.fault.triggered = false;
        status.fault.internal = false;
        status.fault.diode_1 = false;
        status.fault.diode_2 = false;
        status.fault.diode_3 = false;
    }
    
    if (stat & 1 << 4) 
    {
        status.high_limit = getAlertRegister(HIGH_LIMIT_STATUS);
    } 
    else 
    {
        status.high_limit.triggered = false;
        status.high_limit.internal = false;
        status.high_limit.diode_1 = false;
        status.high_limit.diode_2 = false;
        status.high_limit.diode_3 = false;
    }

    if (stat & 1 << 3) 
    {
        status.low_limit = getAlertRegister(LOW_LIMIT_STATUS);
    } 
    else 
    {
        status.low_limit.triggered = false;
        status.low_limit.internal = false;
        status.low_limit.diode_1 = false;
        status.low_limit.diode_2 = false;
        status.low_limit.diode_3 = false;
    }

    if (stat & 1 << 1) 
    {
        status.therm_limit = getAlertRegister(THERM_LIMIT_STATUS);
    } 
    else 
    {
        status.therm_limit.triggered = false;
        status.therm_limit.internal = false;
        status.therm_limit.diode_1 = false;
        status.therm_limit.diode_2 = false;
        status.therm_limit.diode_3 = false;
    }

    if (stat & 1 << 7) 
    {
        status.busy = true;
    } 
    else 
    {
        status.busy = false;
    }

    Thermo5_InterruptHandler(status);
}
