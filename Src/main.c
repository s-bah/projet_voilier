/**
  ******************************************************************************
  * @file    Templates_LL/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_gpio.h"
#include "Chrono.h"
#include "stm32f1xx_ll_tim.h"
#include "CodeurIncremental.h"
#include "RFtelecommande.h"
#include "stm32f1xx_ll_bus.h"

#include "ADConv.h"
#include "MyADC.h"
#include "servo.h"
#include "MCC.h"
#include "MyTimer.h"

extern float AccX,AccY,Batt;
char AR[12];//RollDANGER
char AB[9];// BattLOW
char OK[4];//OK
extern int f;

void  SystemClock_Config(void);
void UART(int f);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	

int main(void)
{
  /* Configure the system clock to 72 MHz */
	SystemClock_Config();
	
	volatile float impulsion; // Cette variable va contenir le duty_cycle
	volatile int Deci;
	
	Encoder2_Config();
	Index_configuration();
	Servo_Conf();
	MCC_Conf();
	ADConvert();

  /* Infinite loop */
  while (1)
  {	
		if_Index();// Detection de l'index pour reinitialiser le compteur, commencer à compter et mesurer les angles
		UART(f);
		Deci=LL_TIM_ReadReg(TIM3,CNT)/4; // Ayant configurer l'ARR à 1440, pour retrouver l'angle en degré, on normalise la valeur de sorte à obtenir 360
		Servo_Start(conversion_angle(Deci));
		impulsion=MyTimer_PWM_In_Duty_Cycle(TIM4);
		MCC_Start(impulsion);
		
	}
}



void UART(int f){
		if(f==1)
		{
			if(AccY<1.75)
			{
				AR[0]='R';
				AR[1]='o';
				AR[2]='l';
				AR[3]='l';
				AR[4]='D';
				AR[5]='A';
				AR[6]='N';
				AR[7]='G';
				AR[8]='E';
				AR[9]='R';
				AR[10]=0x0D;
				AR[11]=0x0A;
				for(int k=0;k<12;k++)
				{
					while (!LL_USART_IsActiveFlag_TXE(USART1)){};
					LL_USART_TransmitData8(USART1,AR[k]);
				}
			}
			if(Batt<8)
			{
				AB[0]='B';
				AB[1]='a';
				AB[2]='t';
				AB[3]='t';
				AB[4]='L';
				AB[5]='O';
				AB[6]='W';
				AB[7]=0x0D;
				AB[8]=0x0A;
				for(int m=0;m<9;m++)
				{
					while (!LL_USART_IsActiveFlag_TXE(USART1)){};
					LL_USART_TransmitData8(USART1,AB[m]);
				}
			}
			if(Batt>8&&AccY>1.75)
			{
				OK[0]='O';
				OK[1]='K';
				OK[2]=0x0D;
				OK[3]=0x0A;
				for(int n=0;n<4;n++)
				{
					while (!LL_USART_IsActiveFlag_TXE(USART1)){};
					LL_USART_TransmitData8(USART1,OK[n]);
				}			
			}
			f=0;	
			}



}





/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucléo*********************************
  // LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000); // utile lorsqu'on utilise la fonction LL_mDelay

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}



/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
