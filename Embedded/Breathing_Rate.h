#ifndef BREATHING_RATE_H
#define BREATHING_RATE_H

#include "stm32f4xx.h"
#include "arm_math.h"
#include "arm_const_structs.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

struct hrList{																					
		float32_t input;
	  struct hrList	*next;
};

typedef struct hrList hrQue; 
float Breathing_Rate(/*hrQue *head,*/ int win);


#endif