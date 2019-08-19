/*
 * File:   phase_control.c
 * Author: hectorcruz
 *
 * Created on August 13, 2019, 1:29 PM
 */


#include "xc.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "phase_control.h"
#include "LCD/lcd.h"

#define COUNTS_PER_HALF_CYCLE  4096

typedef struct _PHASE_OBJ_STRUCT
{ 
    volatile uint8_t     negativeDutyCycle;
    volatile uint16_t    phase_reference;  
    volatile uint16_t    accumulate_counts;  
    
} PHASE_OBJ;

static PHASE_OBJ phase_obj;
int count = 0;


void phaseControl_handler(void){
    
    phase_obj.accumulate_counts = TMR1_SoftwareCounterGet();
    if(phase_obj.accumulate_counts == phase_obj.phase_reference ){
        
        //TRIAC_TRIGGER_SetLow();
        //TRIAC_TRIGGER_SetHigh();
        //LED_D9_SetHigh();
        
    }
    if(phase_obj.accumulate_counts >= 4090 ){
        
        //TRIAC_TRIGGER_SetHigh();
        //TRIAC_TRIGGER_SetLow();
        //LED_D9_SetLow();
        TMR1_Stop();
        TMR1_SoftwareCounterClear();
        
        //printf("\fTimerStop %d\n\r",count);
        count ++;
        
    }
 
}

void TMR1_Handler(void){
    
    //LED_D9_Toggle();
    TRIAC_TRIGGER_SetHigh();
    TRIAC_TRIGGER_SetLow();

    phaseControl_handler();
}
void phaseControl_SetReference(uint16_t phaseReference){

    phase_obj.phase_reference = phaseReference;

}

void phaseControl_Initialize(void){

    phase_obj.accumulate_counts = 0;
    phase_obj.negativeDutyCycle = 0;
    phase_obj.phase_reference   = 512;
   
    TMR1_SetInterruptHandler(&TMR1_Handler);
}




