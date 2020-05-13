/**
  * @author  Xiaotong XIE INSA2019/2020 4AE-SE TP1
	*/

#ifndef MY_ADC_H
#define MY_ADC_H
/**
  Driver pour ADC et DMA
  */

#include "stm32f103xb.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_usart.h"

#define PinAccX LL_GPIO_PIN_0
#define PinAccY LL_GPIO_PIN_1
#define PinBat LL_GPIO_PIN_2


void MyADC_Conf(void (*IT_function) (void));




#endif
