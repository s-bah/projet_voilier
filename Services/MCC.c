#include "MCC.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

void MCC_Conf(void){
MyTimer_Conf(TIM2,2879,499);
MyTimer_IT_Enable(TIM2);
RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_1,LL_GPIO_MODE_ALTERNATE);
LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_1,LL_GPIO_OUTPUT_PUSHPULL);
LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_2,LL_GPIO_OUTPUT_PUSHPULL);
TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
TIM2-> CCER |= TIM_CCER_CC2E;
TIM1->BDTR |= TIM_BDTR_MOE;
LL_TIM_EnableAllOutputs(TIM2);
TIM2->EGR |= TIM_EGR_UG;
}

void MCC_Start(float vitesse){
	MyTimer_Start(TIM2);
	int sens;
	int autoreload = 2879;
	vitesse = vitesse*100; //Normalisation de vitesse
	if(vitesse <11)sens=0;
	else sens=1;
	if(vitesse<=11.5 && vitesse >=11)TIM2->CCR2 = 0; //Etat neutre
	else if(vitesse < 11 && vitesse >8) TIM2->CCR2 = (int)(((11.3-vitesse)*0.2)*autoreload); //mouvement gauche
	else if(vitesse >11.5 && vitesse<15) TIM2->CCR2 = (int)(((vitesse-11.3)*0.2)*autoreload); //mouvement droite
	else TIM2->CCR2 = 0;
	


	
	
	if (sens==0){
		LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_2);
	} else LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_2);
}