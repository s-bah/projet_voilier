
#include "MyTimer.h"
#include "CodeurIncremental.h"
#include "stm32f103xb.h" 
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"
#include "RFtelecommande.h"
#include "stm32f1xx_ll_bus.h"

void GPIOB_pb6_pb7_Config (void) {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_DOWN); 
}
	
void MyTimer_Conf_PWM_In(TIM_TypeDef * Timer, int Channel, int Channel2){

	LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;
	LL_TIM_IC_InitTypeDef My_LL_Tim_IC_Init_Struct;

	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

	

	// Initialiser le CLK

	My_LL_Tim_Init_Struct.Autoreload=0xFFFF;
	My_LL_Tim_Init_Struct.Prescaler=28;
	My_LL_Tim_Init_Struct.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	My_LL_Tim_Init_Struct.CounterMode=LL_TIM_COUNTERMODE_UP;
	My_LL_Tim_Init_Struct.RepetitionCounter=0;

	LL_TIM_Init(Timer,&My_LL_Tim_Init_Struct);
	// Initialize the Input Capture Channel 1

	My_LL_Tim_IC_Init_Struct.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	My_LL_Tim_IC_Init_Struct.ICFilter = LL_TIM_IC_FILTER_FDIV1;
	My_LL_Tim_IC_Init_Struct.ICPolarity = LL_TIM_IC_POLARITY_RISING;
	My_LL_Tim_IC_Init_Struct.ICPrescaler = LL_TIM_ICPSC_DIV1;

	

	LL_TIM_IC_Init(Timer, Channel, &My_LL_Tim_IC_Init_Struct);

	

	// Initialize the Input Capture Channel 2

	My_LL_Tim_IC_Init_Struct.ICActiveInput = LL_TIM_ACTIVEINPUT_INDIRECTTI;
	My_LL_Tim_IC_Init_Struct.ICFilter = LL_TIM_IC_FILTER_FDIV1;
	My_LL_Tim_IC_Init_Struct.ICPolarity = LL_TIM_IC_POLARITY_FALLING;
	My_LL_Tim_IC_Init_Struct.ICPrescaler = LL_TIM_ICPSC_DIV1;

	LL_TIM_IC_Init(Timer, Channel2, &My_LL_Tim_IC_Init_Struct);

	LL_TIM_SetSlaveMode(Timer, LL_TIM_SLAVEMODE_RESET );
	LL_TIM_SetTriggerInput(Timer, LL_TIM_TS_TI1FP1);
	LL_TIM_EnableCounter(Timer);

}

float MyTimer_PWM_In_Duty_Cycle(TIM_TypeDef * Timer){
	float a = LL_TIM_ReadReg(Timer,CCR2);
	float b = LL_TIM_ReadReg(Timer,CCR1);
	float c = a/b;
	return c ;
	
}



