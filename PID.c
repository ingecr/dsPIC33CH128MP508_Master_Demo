/*
 * File:   PID.c
 * Author: hectorcruz
 *
 * Created on August 20, 2019, 5:42 PM
 */


#include "xc.h"
#include "PID.h"

typedef struct PID_OBJECT{

    float Ka;
    float Kb;
    float Kc;
    
    volatile int8_t referencia;
    volatile int8_t error_actual;
    volatile int8_t error_pasado;
    volatile int8_t error_antepasado;
    volatile int16_t entrada_acutal;
    volatile int16_t entrada_pasada;
 
}PID_OBJ;

static PID_OBJ pid_obj;


void PID_Initialize(){

    pid_obj.Ka = 170.95; //80.95
    pid_obj.Kb = -30.3136;// 10.3136
    pid_obj.Kc = .9;
    
    
    pid_obj.error_actual       = 0;
    pid_obj.error_pasado       = 0;
    pid_obj.error_antepasado   = 0;
    pid_obj.entrada_acutal     = 0;
    pid_obj.entrada_pasada     = 0;
      
}

int16_t PID(int8_t* temp_actual, int8_t* referencia ){
    
    pid_obj.error_actual =  *referencia - *temp_actual;
    
    pid_obj.entrada_acutal = (uint16_t)(pid_obj.entrada_pasada + (pid_obj.Ka * pid_obj.error_actual)
            + (pid_obj.Kb * pid_obj.error_pasado) + (pid_obj.Kc * pid_obj.error_antepasado));
    
    /* El limte de la entrada dependera de cuantas cuentas se necesitan para completar medio ciclo
     * Ver tmr1.C y phase_control.C -> 
     
     */
    if(pid_obj.entrada_acutal > 4096){
        
        pid_obj.entrada_acutal = 4096;
        
    }else if(pid_obj.entrada_acutal < 0){
        
        pid_obj.entrada_acutal = 0;
    }
    
    pid_obj.error_antepasado   = pid_obj.error_pasado;
    pid_obj.error_pasado       = pid_obj.error_actual;
    pid_obj.entrada_pasada     = pid_obj.entrada_acutal;

    return (pid_obj.entrada_acutal);      
}