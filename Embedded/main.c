//============================================================================================================================================================================//
//																																	       REFERENCES 																																										    //
//============================================================================================================================================================================//
/* 	1. Controlling STM32 Hardware Timers with Interrupts::Link:: http://visualgdb.com/tutorials/arm/stm32/timers/
		2. Dynamic Memory Allocation in STM32F4Discovery in Keil :: Changes made in <startup_stm32f407xx.s> :: LINK:: http://stm32f4-discovery.com/2015/05/library-58-dynamic-strings-on-stm32f4xx/
		3.
*/

//============================================================================================================================================================================//
//																																	      GLOBAL DEFINES  																																										//
//============================================================================================================================================================================//
#define 	_FPU_USED
#define 	ARM_MATH_CM4
#define 	LCD_CLS_SCREEN				12
#define 	LCD_CUR_POS_00				128
#define 	LCD_NEXT_LINE					13
#define 	FFT_N 								1024
#define 	TEST_LENGTH_SAMPLES 	FFT_N*2
#define		SAMPLE_RATE						200

//============================================================================================================================================================================//
//																																	     LIBRARIES INCLUDES																																										//
//============================================================================================================================================================================//
#include "stm32f4xx.h"
#include "arm_math.h"
#include "arm_const_structs.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#include "USART_config.h"
#include "stdio.h"
#include "stdlib.h"
#include "BR.h"
//#include "DAC.h"
#include "stm32f4xx_dac.h"


#include "data.c"
extern uint16_t ecga[6000];
char newLine='\n';

//============================================================================================================================================================================//
//																																	  FUNCTION PROTOTYPES																																											//
//============================================================================================================================================================================//
extern float32_t ecgD[TEST_LENGTH_SAMPLES];
static float32_t fftOutput[TEST_LENGTH_SAMPLES/2];

uint32_t adcCount=0;																						//Counter for the ADC convertion
uint32_t fftSize = FFT_N;																				//Variable for storing FFT Size
uint32_t ifftFlag = 0;																					//Set the flag for INVERSE FAST FOURIER TRASFORM to zero
uint32_t doBitReverse = 1;																			//Set the BIT REVERSAL flag to 1 for FFT Computation
uint32_t refIndex = 213, testIndex = 0;

int debugMode=1;																								//Flag for DebugMode where PAIR of ELECTRODE is selected based on the amplitude ratio of fundamental amp./50Hz amp.
int debugCount=0;																								//ADC Conversion Counter when the MCU is in DEBUG MODE

float snrRatio[4]={-1,-1,-1,-1};																//Stores the SNR ration i.e. (mag. of fundamental frequency)/(mag. 50Hz frequency)
float sigMean[4]={0,0,0,0};
float sigAvg=0;

int	sigLowLimit=600;																						//Signal Lower Limit(Threshold) i.e. Base Line (above zero)
int sigHighLimit=3800;																					//Signal Higher Limit(Threshold) i.e. Saturation Value
int sigLowDev=0;																								//Stores the standard deviation of the base line value 
int sigHighDev=0;																								//Stores the standard deviation of the threshold value

int testFlag=0;

struct FFTMagVal{																								//Datatype for storing the bin number as well as its magnitude
		float32_t mag;
		int bin;
};

//typedef struct hrList hrQue;  
typedef struct FFTMagVal	hrBin;
hrQue *head,*node,*tempPtr,*tempPtrPtr,*prevtest;
int avgCounter=0;
char espData;

//============================================================================================================================================================================//
//																																	    GLOBAL VARIABLES																																											//
//============================================================================================================================================================================//
void Delay_count(uint32_t count);
void adcConfig(void);
void ledConfig(void);
void InitializeTimer(int sampleRate);
hrBin SearchFundamentalBin(float *inArray, int minRange, int maxRange);
void EnableTimerInterrupt(void);
int ElectrodeSelection(void);

//============================================================================================================================================================================//
//																																	     MAIN FUNCTION/LOOP		   																																							//
//============================================================================================================================================================================//
int main(void)
{
	char newLine='\n';
	char breakLine='\r';
	int i=0;	
	int cntBR=-1;
	arm_status status;
  status = ARM_MATH_SUCCESS;
	
	node =(hrQue *)malloc(sizeof(hrQue));
	node->input=0;
	node->next=NULL;
	head=tempPtr=node;
	
	node=head;
  ledConfig();
  adcConfig();
	init_USART1(9600);																													//Baudrate needs to be set for LCD as LCD Baudrate is 9600
																																							//Need to use another USART for the LCD
  TIM_SetCounter(TIM2, 0);
	EnableTimerInterrupt();
	//ElectrodeSelection(); 
	InitializeTimer(SAMPLE_RATE);   // sampling rate
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	//DAC_Clock_Config();
  //DAC_Config(DAC_Channel_1);
  //DAC_SetChannel1Data(DAC_Align_12b_R,2047);
	
	while(1)
    {			
			if((adcCount == 2048) || (avgCounter == (SAMPLE_RATE+1))){
				USART_puts(USART1, "WHILE LOOP : ");
				TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
					tempPtrPtr=head;
					for(i=0; i<FFT_N*2; i=i+2){
						ecgD[i]=tempPtrPtr->input;
						ecgD[i+1]=0;
						tempPtrPtr=tempPtrPtr->next->next;
					}
					TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
				
					arm_cfft_f32(&arm_cfft_sR_f32_len1024, ecgD, ifftFlag, doBitReverse);  /* Process the data through the CFFT/CIFFT module */
					arm_cmplx_mag_f32(ecgD, fftOutput, fftSize);
					
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//					USART_puts(USART1, "FFT OUTPUT");
//					USART_SendData(USART1, newLine);
//					for(i=0; i<1024; i++){
//						USART_Send_Num(USART1,fftOutput[i]);
//						USART_SendData(USART1, newLine);
//					}
//					
					USART_SendData(USART1,LCD_CLS_SCREEN);
					Delay_count(10000);
					USART_puts(USART1, "HR: ");
					USART_Send_Num(USART1,((SearchFundamentalBin(fftOutput, 7 , 22).bin)*6000)/1024);
					USART_SendData(USART1,newLine);
					Delay_count(10000);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					
					avgCounter=1;
					cntBR++;
					if(cntBR==0){
						cntBR=0;
						Breathing_Rate(head, 2048/(SearchFundamentalBin(fftOutput, 7 , 22).bin) , SAMPLE_RATE);	
					}
			}
    }
}

//============================================================================================================================================================================//
//																																	      INTERRUPT HANDLER																																										//
//============================================================================================================================================================================//
void ADC_IRQHandler()
{
	if(debugMode==1){
		ecgD[debugCount*2]=ADC_GetConversionValue(ADC3);
		ecgD[debugCount*2+1]=0;
		
		sigAvg=ecgD[debugCount*2]+sigAvg;
		debugCount++;
	}
	else{
		node=(hrQue *)malloc(sizeof(hrQue));
		node->input=ADC_GetConversionValue(ADC3);
		//espData=node->input/16;
		//USART_puts(USART1, &espData);
		if(node==NULL) testFlag=1;
		
		tempPtr->next=node;
		tempPtr=node;
		
		adcCount++;
		if(adcCount>FFT_N){
			tempPtrPtr=head;
			head=head->next;
			free(tempPtrPtr);
			avgCounter++;
		}
	}
  ADC_ClearFlag(ADC3, ADC_FLAG_EOC);
	ADC_ITConfig(ADC3, ADC_IT_EOC, DISABLE);
}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			if(debugMode==0){
        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
			}
			//	ADC_SoftwareStartConv(ADC3);
			
			//TEST  CODE
					if(adcCount<=6000)
					{
					node=(hrQue*)malloc(sizeof(hrQue));
						if(node==NULL) adcCount=9000;
						
					node->input= ecga[adcCount];
					node->next=NULL;
					if(adcCount==0)
					head=node;
					else
					prevtest->next=node;

					prevtest=node;
          adcCount++;
					/*
					  USART_SendData(USART1,newLine);
						USART_SendData(USART1,'\r');
					  USART_Send_Num(USART1,adcCount);
					  USART_puts(USART1, "::");
						USART_Send_Num(USART1,node->input);
						USART_SendData(USART1,newLine);
						USART_SendData(USART1,'\r');
					*/
				}
					
					
				//ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);
					  
    }
}
//============================================================================================================================================================================//
//																																	          FUNCTIONS 																																											//
//============================================================================================================================================================================//
int ElectrodeSelection()
{
	int i;
	int x,y,z;
	debugCount=0;
	InitializeTimer(200);
	for(i=0; i<4; i++){
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
		if(i==0){
			GPIO_ResetBits(GPIOD,GPIO_Pin_13|GPIO_Pin_12);
		}
		if(i==1){
			GPIO_SetBits(GPIOD, GPIO_Pin_12);
		}
		if(i==2){
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			GPIO_SetBits(GPIOD, GPIO_Pin_13);
		}
		if(i==3){
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
		}
		
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		while(debugCount!=1024);
		arm_cfft_f32(&arm_cfft_sR_f32_len1024, ecgD, ifftFlag, doBitReverse);    /*Process the data through the CFFT/CIFFT module */
		arm_cmplx_mag_f32(ecgD, fftOutput, fftSize);
		snrRatio[i]=((float)(SearchFundamentalBin(fftOutput, 4 , 10).mag))/fftOutput[256];
		
		sigMean[i]=sigAvg/1024;																													//SEND THE OUTPUT VALUES TO CHECK THE BIN NUMBERS AND SNRRATIO
		if(sigMean[i]<sigLowLimit || sigMean[i]>sigHighLimit){
			snrRatio[i]=-1;
		}
		sigAvg=0;
		
		debugCount=0;
		Delay_count(100000);
	}
	//compare the stored values
	x=(snrRatio[0]>snrRatio[1])?0:1;
	y=(snrRatio[2]>snrRatio[3])?2:3;
	z=(snrRatio[x]>snrRatio[y])?x:y;
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	debugMode=0;
	if(z==0){
		GPIO_ResetBits(GPIOD,GPIO_Pin_13|GPIO_Pin_12);
	}
	if(z==1){
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	}
	if(z==2){
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
	}
	if(z==3){
		GPIO_SetBits(GPIOD,GPIO_Pin_12);
	}
	
///////////////////////////////////////////////////////////////////////////////////////////
//					USART_puts(USART1, "SNR RATIO");
//					USART_SendData(USART1, '\n');
//					for(i=0; i<4; i++){
//						USART_Send_Num(USART1,snrRatio[i]);
//						USART_SendData(USART1, '\n');
//					}
//					
//					USART_puts(USART1, "SIGNAL MEAN");
//					USART_SendData(USART1, '\n');
//					for(i=0; i<4; i++){
//						USART_Send_Num(USART1,sigMean[i]);
//						USART_SendData(USART1, '\n');
//					}
///////////////////////////////////////////////////////////////////////////////////////////
					
	return z;
}

void InitializeTimer(int sampleRate)
{
	  TIM_TimeBaseInitTypeDef timerInitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 
    timerInitStructure.TIM_Prescaler = 1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = (16000000/sampleRate);
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
}

void EnableTimerInterrupt()
{
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

hrBin SearchFundamentalBin(float32_t *inArray, int minBin, int maxBin){
	int find;
	hrBin tempBin;
	tempBin.mag=inArray[minBin];
	tempBin.bin=minBin;
	for(find=minBin; find<=maxBin; find++){
		if(inArray[find]> tempBin.mag){
			tempBin.mag=inArray[find];
			tempBin.bin=find;
		}
	}
	return tempBin;
}

void ledConfig()
{
  GPIO_InitTypeDef led_gpio;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  led_gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  led_gpio.GPIO_Mode = GPIO_Mode_OUT;
  led_gpio.GPIO_OType = GPIO_OType_PP;
  led_gpio.GPIO_Speed = GPIO_Speed_50MHz;
  led_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &led_gpio);
}


void adcConfig()
{
  ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
  NVIC_InitTypeDef    NVIC_adc;


  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);


  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);


  ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

  ADC_Cmd(ADC3, ENABLE);

  NVIC_adc.NVIC_IRQChannel = ADC_IRQn;
  NVIC_adc.NVIC_IRQChannelCmd = ENABLE;
  NVIC_adc.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_adc.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_Init(&NVIC_adc);

}


//============================================================================================================================================================================//
//============================================================================================================================================================================//
//while( !(USARTx->SR & 0x00000040) );
//USART1->DR=128;
//GPIO_SetBits(GPIOD,GPIO_Pin_13|GPIO_Pin_15);
//GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_14);
//					TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//					TIM_SetCounter(TIM2,0);


//					USART_puts(USART1, "INPUT SIGNAL");
//					USART_SendData(USART1, newLine);
//					for(i=0; i<1024; i++){
//						USART_Send_Num(USART1,ecgD[i]);
//						USART_SendData(USART1, newLine);
//					}