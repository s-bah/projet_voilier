#include "CodeurIncremental.h"
#include "MyTimer.h"
#include "stm32f103xb.h" 
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "RFtelecommande.h"

void Encoder2_Config(void){
	
RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // pour enable de GPIO pour la connection avec la girouette (PA : 5,6,7)
RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // pour enable timer 3	

LL_TIM_SetAutoReload(TIM3,0x05A0); //autoreload : on a 360 x 4 (on compte 4 fois par cycle et il y a 360 cycles par tour)

//Programmation selon la datasheet 

TIM3->CCMR1 &=~ (TIM_CCMR1_CC1S_1);
TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 ;

TIM3->CCMR1 &=~ (TIM_CCMR1_CC2S_1);
TIM3->CCMR1 |= TIM_CCMR1_CC2S_0 ;

TIM3->CCER &=~ (TIM_CCER_CC1P | TIM_CCER_CC1NP);
TIM3->CCMR1 &=~(TIM_CCMR1_IC1F);

TIM3->CCER &=~ (TIM_CCER_CC2P |TIM_CCER_CC2NP);
TIM3->CCMR1 &=~ (TIM_CCMR1_IC2F);

TIM3->SMCR &=~ (TIM_SMCR_SMS_2);
TIM3->SMCR |= (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0) ;

TIM3->CR1 |= TIM_CR1_CEN;

LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH1);

LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH2);

LL_TIM_EnableCounter(TIM3);
}
void GPIO_config (void) {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_5,LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_6, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_7,LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(GPIOA,LL_GPIO_PIN_5,LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinPull(GPIOA,LL_GPIO_PIN_6,LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinPull(GPIOA,LL_GPIO_PIN_7,LL_GPIO_PULL_DOWN);
	
}

void Index_configuration (void) {
	GPIO_config();
	GPIOB_pb6_pb7_Config();
	MyTimer_Start(TIM3);
	MyTimer_Conf_PWM_In(TIM4,LL_TIM_CHANNEL_CH1,LL_TIM_CHANNEL_CH2);
	while(!LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_5)){

	}
	TIM3->CNT = 0;	
}
void if_Index (void) { 
	if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_5)) {
		TIM3->CNT = 0 ;
	}
}



	
