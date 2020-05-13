#include "MyADC.h"
volatile uint16_t ADCData[10][3];
void (*Ptr_ItFct)(void);
int f;

void MyADC_Conf(void (*IT_function)(void)){
	
	/*==Configuration GPIOC==*/
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);//Enable clock GPIOC
	LL_GPIO_SetPinMode(GPIOC,PinAccX,LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC,PinAccY,LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC,PinBat,LL_GPIO_MODE_ANALOG);
	
	/*==Configuration USART1==*/
	LL_USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.BaudRate=9600;
	USART_InitStruct.DataWidth= LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity= LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection= LL_USART_DIRECTION_TX;
	USART_InitStruct.HardwareFlowControl= LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);//Enable clock USART2	
	LL_USART_Init(USART1, &USART_InitStruct);
	LL_USART_Enable(USART1);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);//TX->PA2	
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);


	
	/*== Configuration ADC: Mode Scan Not Continuous PLUS DMA ==*/
	/**Initialisation DMA**/
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);//Enable clock DMA1
	LL_DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.PeriphOrM2MSrcAddress=LL_ADC_DMA_GetRegAddr(ADC1,LL_ADC_DMA_REG_REGULAR_DATA);//ADC register address
	DMA_InitStruct.MemoryOrM2MDstAddress=(uint32_t)&ADCData;//Destination address
	DMA_InitStruct.Direction=LL_DMA_DIRECTION_PERIPH_TO_MEMORY;//Transfer direction
	DMA_InitStruct.Mode=LL_DMA_MODE_CIRCULAR;//Circular operation mode
	DMA_InitStruct.PeriphOrM2MSrcIncMode=LL_DMA_PERIPH_NOINCREMENT;//Peripheral address not incremented
	DMA_InitStruct.MemoryOrM2MDstIncMode=LL_DMA_MEMORY_INCREMENT;//Destination address incremented
	DMA_InitStruct.PeriphOrM2MSrcDataSize=LL_DMA_PDATAALIGN_HALFWORD;//16 bits
	DMA_InitStruct.MemoryOrM2MDstDataSize=LL_DMA_MDATAALIGN_HALFWORD;//16 bits
	DMA_InitStruct.NbData=30;//3*10
	DMA_InitStruct.Priority=LL_DMA_PRIORITY_HIGH;//Priority
	LL_DMA_Init(DMA1,LL_DMA_CHANNEL_1,&DMA_InitStruct);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_1);//Enable DMA1 channel1(ADC)
	/*Interruption DMA*/
	NVIC_SetPriority(DMA1_Channel1_IRQn,3);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	Ptr_ItFct=IT_function;
	LL_DMA_EnableIT_TC(DMA1,LL_DMA_CHANNEL_1);//Enable Interruption
	/**Initialisation ADC**/
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);//Enable Clock ADC1
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);//72Mhz to less 14Mhz
	LL_ADC_InitTypeDef ADC_InitStruct;//Features of ADC instance
	ADC_InitStruct.DataAlignment=LL_ADC_DATA_ALIGN_RIGHT;//ADC conversion data alignment
	ADC_InitStruct.SequencersScanMode=LL_ADC_SEQ_SCAN_ENABLE;//Multi-channel -> SCAN ENABLE
	LL_ADC_Init(ADC1,&ADC_InitStruct);
	LL_ADC_REG_InitTypeDef ADC_RegInitStruct;//Features of ADC group regular
	ADC_RegInitStruct.TriggerSource=LL_ADC_REG_TRIG_SOFTWARE;
	ADC_RegInitStruct.SequencerLength=LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS;
	ADC_RegInitStruct.SequencerDiscont=LL_ADC_REG_SEQ_DISCONT_DISABLE;//Discontinuous mode DISABLE
	ADC_RegInitStruct.ContinuousMode=LL_ADC_REG_CONV_CONTINUOUS;//Continous mode
	ADC_RegInitStruct.DMATransfer=LL_ADC_REG_DMA_TRANSFER_UNLIMITED;//Permit DMA transfer
	LL_ADC_REG_Init(ADC1,&ADC_RegInitStruct);
	/*Configuration des channels ADC*/
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_10,LL_ADC_SAMPLINGTIME_239CYCLES_5);
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_11,LL_ADC_SAMPLINGTIME_239CYCLES_5);
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_12,LL_ADC_SAMPLINGTIME_239CYCLES_5);
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_10);
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_2,LL_ADC_CHANNEL_11);
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_3,LL_ADC_CHANNEL_12);
	LL_ADC_Enable(ADC1);//Enable ADC1
	/*Calibration*/
	LL_ADC_StartCalibration(ADC1);
	while(LL_ADC_IsCalibrationOnGoing(ADC1));
	/*Start*/
	LL_ADC_REG_StartConversionSWStart(ADC1);//Start ADC group regular conversion for Software triger
}

void DMA1_Channel1_IRQHandler(void){

	LL_DMA_ClearFlag_GI1(DMA1);
	(*Ptr_ItFct)();
	f=1;
}
