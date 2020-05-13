#include "ADConv.h"

void Average(void);
float ADCValue[3];
float AccX,AccY,Batt;
extern uint16_t ADCData[10][3];


void ADConvert(void){
	MyADC_Conf(&Average);
}

void Average(void){
	int sum;
	uint8_t i,j=0;

	for(i=0;i<3;i++){
		sum=0;
		for(j=0;j<10;j++)
		{
			sum+=ADCData[j][i];
		}
		if(i<2){	
		ADCValue[i]=(float)sum/(10*4096)*3.3;//Average ADC Value for X and Y (sum/10)*(3.3/4096)
		}
		else{
		ADCValue[i]=(float)sum/(10*4096)*12*3.6;//Average ADC Value for B  (sum/10)*(12/4096)
		}	
	}   
	AccX=ADCValue[0];
	AccY=ADCValue[1]; //>45°--------y<1.7
	Batt=ADCValue[2];	
}
