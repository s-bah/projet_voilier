#ifndef RFtelecommande_H
#define RFtelecommande_H

#include "stm32f103xb.h"

void MyTimer_Conf_PWM_In(TIM_TypeDef * Timer, int Channel, int Channel2) ;
float MyTimer_PWM_In_Duty_Cycle(TIM_TypeDef * Timer);
void GPIOB_pb6_pb7_Config (void);
float affine_PWM (float Valeur);

#endif
