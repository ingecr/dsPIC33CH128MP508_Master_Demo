/*
 * File:   phase_control.c
 * Author: hectorcruz
 *
 * Created on August 13, 2019, 1:29 PM
 */


#include "xc.h"
#include "phase_control.h"

#define COUNTS_PER_HALF_CYCLE  4096

typedef struct _PHASE_OBJ_STRUCT
{ 
    volatile uint8_t     negativeDutyCycle;
    volatile uint16_t    phase_reference;  
    volatile uint16_t    accumulate_counts;  
    
} PHASE_OBJ;

static PHASE_OBJ phase_obj;



void phaseControl_handler(void){
    
    phase_obj.accumulate_counts = TMR1_SoftwareCounterGet();
    if(phase_obj.accumulate_counts == phase_obj.phase_reference ){
        
        TRIAC_TRIGGER_SetLow();
    }
    if(phase_obj.accumulate_counts >= 4090 ){
    
        TRIAC_TRIGGER_SetHigh();
        TMR1_Stop();
        TMR1_SoftwareCounterClear(); 
    }

}

void TMR1_Handler(void){
    
    PR1 = 0x07;
    phaseControl_handler();
}
void phaseControl_SetReference(uint16_t phaseReference){

    phase_obj.phase_reference = phaseReference;

}

void phaseControl_Initialize(void){

    phase_obj.accumulate_counts = 0;
    phase_obj.negativeDutyCycle = 0;
    phase_obj.phase_reference   = 0;
    
    TMR1_Stop();
    TMR1_SetInterruptHandler(&TMR1_Handler);
}




