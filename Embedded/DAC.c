#include "DAC.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_tim.h"

void DAC_Clock_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
}

void DAC_Config(uint32_t DAC_Channel)
{
	DAC_InitTypeDef DACInitStruct;
	DACInitStruct.DAC_LFSRUnmask_TriangleAmplitude=DAC_TriangleAmplitude_2047;
	DACInitStruct.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
	DACInitStruct.DAC_Trigger=DAC_Trigger_Software;
	DACInitStruct.DAC_WaveGeneration=DAC_WaveGeneration_Triangle;

	DAC_Init(DAC_Channel,&DACInitStruct);

	DAC_Cmd(DAC_Channel,ENABLE);

}