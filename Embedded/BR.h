#ifndef _BR_H
#define _BR_H

#define 	_FPU_USED
#define 	ARM_MATH_CM4
#include "stm32f4xx.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "stdio.h"
#include "USART_config.h"

#define Lecg 2000

struct hrList{																									//A Linked List Struct for dyanmic storage of 1024 values of ADC Conversion Results
	uint16_t input;
	struct hrList	*next;
};
typedef struct hrList hrQue; 

float Breathing_Rate(hrQue *head, int win,int sf);
int global_Min(int x,int y, hrQue* h);
float traverse(hrQue* h,int bound);
float  Maximum(int x,int y,hrQue* h);
int Length_LL(hrQue* h);
void Delay_count(uint32_t count);


#endif
