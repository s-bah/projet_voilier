
#include "Chrono.h"
#include "MyTimer.h"
#include "Servo.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

void Servo_Conf(void){
	
	MyTimer_Conf(TIM1,2879, 499);
	MyTimer_IT_Enable(TIM1);
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_8,LL_GPIO_OUTPUT_PUSHPULL);
	MyTimer_Start(TIM1);
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
	TIM1-> CCER |= TIM_CCER_CC1E;
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->EGR |= TIM_EGR_UG;
}

float conversion_angle(int angle_girouette){
	float theta = 0;//initialisation de variables
	if(angle_girouette>180)angle_girouette=360-angle_girouette;
	if(angle_girouette > 0 && angle_girouette<45){//A l'intervalle angle_girouette [0° , 45°] on ferme totalement l'angle des voiles theta  = 0°.
		theta=0;
	}
	if(angle_girouette >= 45  && angle_girouette <= 180){//A l'intervalle angle_girouette[45° , 180°] correspond, suivant une loi affine, l'intervalle theta[0° , 90°]
		theta=(0.67*angle_girouette) - 30;
	}
	return theta;
}


void Servo_Start(float angle){

	TIM1->CCR1 = (1.6*angle)+144;
}


