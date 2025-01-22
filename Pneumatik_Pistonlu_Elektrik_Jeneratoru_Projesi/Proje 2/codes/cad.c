#include "stm32f10x.h"                  // Device header
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "delay.h"

void gpioConfig(void);
void adcConfig(void);
void dmaConfig(void);

uint16_t degerF1=0;
uint16_t degerF2=0;

uint16_t degerS1=0;
uint16_t degerS2=0;
uint16_t adcValue1=0;
uint16_t adcValue2=0;

float mapValue = 0;
float mapValue1 = 0;

uint16_t adcValue[2];

uint16_t test=0;
uint16_t test2=0;


	uint16_t onceki_deger = 0;
	
	uint16_t yeni_deger; 
	uint16_t deger;
	
	uint16_t oku1=0;
		uint16_t oku2=0;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//ADC PIN0 CONFIG
  GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//ADC PIN1 CONFIG
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//tr1 ve tr2 (pb0 ve pb1) röle (pb2 ve pb3)
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_5;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	

}
void delay (uint32_t time) {
	while(time--);
		
}


void adcConfig(){
	
	ADC_InitTypeDef ADCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	ADCInitStructure.ADC_ContinuousConvMode=ENABLE;
	ADCInitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADCInitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADCInitStructure.ADC_Mode=ADC_Mode_Independent;
	ADCInitStructure.ADC_NbrOfChannel=2;
	ADCInitStructure.ADC_ScanConvMode=ENABLE;
	
	ADC_Init(ADC1,&ADCInitStructure);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}

void dmaConfig(){
	
	DMA_InitTypeDef DMAInitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_DeInit(DMA1_Channel1);
	
	DMAInitStructure.DMA_BufferSize=2;
	DMAInitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMAInitStructure.DMA_M2M=DISABLE;
	DMAInitStructure.DMA_MemoryBaseAddr=(uint32_t)adcValue;
	DMAInitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMAInitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMAInitStructure.DMA_Mode=DMA_Mode_Circular;
	DMAInitStructure.DMA_PeripheralBaseAddr=(uint32_t) & ADC1->DR;
	DMAInitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMAInitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMAInitStructure.DMA_Priority=DMA_Priority_High;
	
	DMA_Init(DMA1_Channel1,&DMAInitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
}



uint16_t map(float adcVal, float max, float min, float conMax, float conMin){

return adcVal*((conMax-conMin)/(max-min));

}

uint16_t map1(float adcVal, float max, float min, float conMax, float conMin){

return  adcVal*((conMax-conMin)/(max-min));

}




uint16_t potF1 () {

		mapValue=map(adcValue[0],4030,0,269,0);	
		
	return mapValue;
}
uint16_t potF2 () {

		mapValue1=map(adcValue[1],4030,0,269,0);	
		
	return mapValue1;
}

uint16_t potS1 () {

		mapValue=map(adcValue[0],4030,0,269,0);	
		
	return mapValue;
}
uint16_t potS2 () {

		mapValue1=map(adcValue[1],4030,0,269,0);	
		
	return mapValue1;
}

int main(){
	gpioConfig();
	adcConfig();
	dmaConfig();
	

	
	while(1){
		
		if (potF1()!=0) {
			
					onceki_deger = potF1();
		delay(7200000);
		potF1();
		
		if (potF1() > onceki_deger) {
			GPIO_SetBits(GPIOB,GPIO_Pin_0); //tr1 
			GPIO_SetBits(GPIOB,GPIO_Pin_5); // röle1
			
			test=100;
			delay(7200000);
			
		}

		
		if (onceki_deger > potF1()) {
			
			GPIO_SetBits(GPIOB,GPIO_Pin_0); //tr1 
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); // röle1
		
			
			test=150;
			delay(7200000);
		}
				else {
		
					
			GPIO_ResetBits(GPIOB,GPIO_Pin_0); //tr1 
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); // röle1
		
					
					test=200;
		}
		
		

		}
		
				//		else {
			//test=0;
		//}


		
		
//		
//		if (potF2()!=0) {
//			
//		uint16_t onceki_deger1 = potF2();
//		delay(3600000);
//		potF2();
//		
//		if (potF2() > onceki_deger1) {
//			
//			
//				
//			GPIO_SetBits(GPIOB,GPIO_Pin_1); //tr2 
//			GPIO_SetBits(GPIOB,GPIO_Pin_3); // röle2
//		
//			
//			//test2=100;
//			delay(3600000);
//		}
//		
//		if (onceki_deger1 > potF2()) {
//			
//			
//			
//			GPIO_SetBits(GPIOB,GPIO_Pin_1); //tr2 
//			GPIO_ResetBits(GPIOB,GPIO_Pin_3); // röle2
//		
//			
//			//test2=150;
//			delay(3600000);
//		}
//		
//				
//		else {
//			
//			
//			GPIO_ResetBits(GPIOB,GPIO_Pin_1); //tr2 
//			GPIO_ResetBits(GPIOB,GPIO_Pin_3); // röle2
//		
//			
//			
//			//test2=200;
//		}
//		
//	

//		}
				//else {
		//test2=0;
		//}
		
		
	
}


	return 0;
}
