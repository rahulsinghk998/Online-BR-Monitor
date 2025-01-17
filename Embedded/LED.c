int LED()
{
GPIO_InitTypeDef GPIO_LED1;
	GPIO_InitTypeDef GPIO_LED2;
	GPIO_InitTypeDef GPIO_LED3;
	GPIO_InitTypeDef GPIO_LED4;

	  //unsigned int i=100000;
	int i=1000000;

	//now we have to start the clock of the GPIO portD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	// Setting up the GPIO
	GPIO_LED1.GPIO_Pin=GPIO_Pin_12; //there are 5 ports ABCDE each haveing 16 pins
	GPIO_LED2.GPIO_Pin=GPIO_Pin_13;
	GPIO_LED3.GPIO_Pin=GPIO_Pin_14;
	GPIO_LED4.GPIO_Pin=GPIO_Pin_15;

	GPIO_LED1.GPIO_Mode=GPIO_Mode_OUT; // we want Uc in output mode(many other modes are ther) we want Uc to give high to pin12
	GPIO_LED2.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_LED3.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_LED4.GPIO_Mode=GPIO_Mode_OUT;
	
	GPIO_LED1.GPIO_OType=GPIO_OType_PP; // we want the pin to be in push pull mode
	GPIO_LED2.GPIO_OType=GPIO_OType_PP;
	GPIO_LED3.GPIO_OType=GPIO_OType_PP;
	GPIO_LED4.GPIO_OType=GPIO_OType_PP;
	
	GPIO_LED1.GPIO_Speed=GPIO_Speed_50MHz; // we are telling the GPIO speed 2,5,15MHz
	GPIO_LED2.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_LED3.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_LED4.GPIO_Speed=GPIO_Speed_50MHz;
	
	//Calling the GPIO_Init
	GPIO_Init(GPIOD,&GPIO_LED1);// passing all info to portD,info=GPIO_LED
	GPIO_Init(GPIOD,&GPIO_LED2);
	GPIO_Init(GPIOD,&GPIO_LED3);
	GPIO_Init(GPIOD,&GPIO_LED4);
	
	//to write logic 1 to PD12,BIT_SET means logic1 and viceversa logic0
	//GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_SET);
	
	
	while(1)
	{
	
		while(i)
		{	
			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_SET);
		i--;
		}	
		i=1000000;
		GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_RESET);
		/*
		while(i)
		{	
			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_RESET);
		i--;
		}	
    */ 
	i=1000000;
		while(i)
		{	
			GPIO_WriteBit(GPIOD,GPIO_Pin_13,Bit_SET);
		i--;
		}	
		GPIO_WriteBit(GPIOD,GPIO_Pin_13,Bit_RESET);
		i=1000000;
		while(i)
		{	
			GPIO_WriteBit(GPIOD,GPIO_Pin_14,Bit_SET);
		i--;
		}	
		GPIO_WriteBit(GPIOD,GPIO_Pin_14,Bit_RESET);
   i=1000000;
		while(i)
		{	
			GPIO_WriteBit(GPIOD,GPIO_Pin_15,Bit_SET);
		i--;
		}	
		GPIO_WriteBit(GPIOD,GPIO_Pin_15,Bit_RESET);
		i=1000000;
 }
 }