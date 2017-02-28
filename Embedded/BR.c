//============================================================================================================================================================================//
//																																	     LIBRARIES INCLUDES																																										//
//============================================================================================================================================================================//
#define 	_FPU_USED
#define 	ARM_MATH_CM4

#include "BR.h"
#include "stm32f4xx.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "stdio.h"
#include "stdlib.h"
#include "USART_config.h"
//#include "data.c"



//Global Variables Declaration
//char newLine='\n';
//extern float32_t ecga[4096];  
float32_t ecgD[2048]={0};
hrQue *p,*prev,*ecg,*qHead;
int sf=200;



float Breathing_Rate(hrQue *qHead, int win,int sf,int win2)
{
								USART_puts(USART1, "    BREATHING RATE::    ");
								USART_SendData(USART1,'\n');
								USART_SendData(USART1,'\r');
	static int counter=0;
	counter++;
	int r_positive[100],r_negative[100],i,Rwave[100];
	/*
	for(int i=0;i<Lecg;i++){  
	ecg=(hrQue*)malloc(sizeof(hrQue));
	ecg->input= ecga[i];
	ecg->next=NULL;
	if(i==0)
		qHead=ecg;
	else
		prev->next=ecg;

	prev=ecg;
	}
	*/

	//Finding The Difference from the ecg signal
	p=qHead;
	for (i=0;i<Lecg-1;i++){
		ecgD[i]=((p->next)->input) - (p->input);   //ecgD x(n)=x(n)-x(n-1) is difference signal
		p=p->next;
	}

	//Finding  the positive and negative for R calculation algorithm
	float sum_positive=0;
	int pos=0;
	float sum_negative=0;
	int neg=0;
	for (i=1;i<Lecg-1;i++){
		if(ecgD[i]>=ecgD[i-1] && ecgD[i]>=ecgD[i+1]){	 
			sum_positive=sum_positive+ecgD[i];
			pos=pos+1;
		}
		if(ecgD[i]<=ecgD[i-1] && ecgD[i]<=ecgD[i+1]){
			sum_negative=sum_negative+ecgD[i];
			neg=neg+1;
		}
	}
	
	
	//T1=2MVp T2=2MVn
	float T1=2*(sum_positive/pos); //sum_positive/pos represents the mean of all amplitude s.t ecg(i)>0,T1 is the threshold for the positive,this is done to remove all disturbances between R-R peaks 
	float T2=2*(sum_negative/neg); //sum_negative/neg represents the mean of all amplitude s.t ecg(i)<0,T2 is the threshold for the negative
	for (i=0;i<Lecg-1;i++){
		if(ecgD[i]<=T1 && T2<=ecgD[i])      
			ecgD[i]=0;
		if(ecgD[i]>T1 || ecgD[i]<T2)
			ecgD[i]=ecgD[i];
	 }

	 
	int indexpos;
	int indexneg;	
	int j=0; 											//index for position of Rwave; 
	for (i=0;i<Lecg-win-1;i++){					
    indexpos=i;  								//variable that store the index of peak in ecgDf_cap_pos
		indexneg=i;
		for (j=i+1;j<=i+win;j++){//:i+win
			if(ecgD[j]>=0&&ecgD[indexpos]>=0){
					if(ecgD[j]>ecgD[indexpos]){ //check for equality condition and also check for saving R index here itself
						ecgD[indexpos]=0;
						indexpos=j;
					}
					else{ 
							ecgD[j]=0;
					}
			}	
			if(ecgD[j]<=0&&ecgD[indexneg]<=0){
				if(ecgD[j]<ecgD[indexneg]){
					ecgD[indexneg]=0;
					indexneg=j;
				}
				else{
				   ecgD[j]=0;
				}
			}
		}
	}

	//Counting rPosive peaks and rNegative Peaks
	int rPcount=0;
	int rNcount=0;	
	//check for equality condition when two points are equal
	for (i=1;i<Lecg-1-1;i++)		{
//		if(ecgD[i]>0){
//			if(ecgD[i]>ecgD[i-1]&&ecgD[i]>ecgD[i+1]){
//					r_positive[rPcount]=i;
//					rPcount=rPcount+1;
//			}
//		}
		if(ecgD[i]<0){
			if(ecgD[i]<ecgD[i-1]&&ecgD[i]<ecgD[i+1]){
					r_negative[rNcount]=i;
					rNcount=rNcount+1;
			}
		}
	}
	


float sum=0;
for (i=1;i<rNcount;i++)
{
	if (ecgD[i]<0)
     sum=sum+ecgD[r_negative[i]];
}
float mean_rnegative=sum/rNcount;

int l=1;
 i=1;
int COUNT=rNcount-1; //COUNT is the number of r_negative till now
while( i<=COUNT)
{
		if (ecgD[i]<0)
		{
			if (ecgD[r_negative[i]]>( (mean_rnegative)/2 );
			{
				COUNT--;
				int k=i;
				while (k<=COUNT)
				{
					r_negative[k]=r_negative[k+1];
					k++;
				}
			// r_negative(i)=[];
			//  rNcount=rNcount-1;
			}
	//		l=length(r_negative);
			i=i+1;
		}
}

for (i=1;i<=COUNT;i++)
{
	for (j=r_negative[i]-win2;j<=r_negative[i];j++)
	{
		if(ecgD[j]>ecgD[j-1]&&ecgD[j]>ecgD[j+1])
		{
			r_positive[i]=j;
		}
//    [mmm,temp]=max(ecgD[ [r_negative[i]-win2:r_negative[i]] ]);
//    r_positive(i)=temp+r_negative(i)-win2;
	}
}



	p=qHead;
	for (i=1;i<=COUNT;i++){
		for ( j=r_positive[i] ; j<=r_negative[i] ;j++){
				for(l=0;l<j;l++){
					prev=p;
					p=p->next;
				}			
				if( (p->input>=prev->input)&&((p->input > p->next->input) ) )   
				{
					Rwave[i]=j;
//					k=k+1;
				}
				p=qHead;
		}
//		n=n+1; 
	}
 	
	
	//Q,S Detection Algorithm from here// Time windows for Q is taken as ___ and for S is taken as __ and their values taken from from sampling rate
	int Q1[100],Q2[100],S1[100],S2[100],Q[100],S[100];
	for( i=0;i<=COUNT;i++){	
		Q1[i]=global_Min(Rwave[i]-(int)(0.04*sf),Rwave[i],qHead);
		S1[i]=global_Min(Rwave[i],Rwave[i]+((int)(0.04*sf)),qHead);
		Q2[i]=global_Min(Rwave[i]-(int)(0.11*sf),Rwave[i],qHead);
		S2[i]=global_Min(Rwave[i],Rwave[i]+((int)(0.11*sf)),qHead);
	 
		float   MVq=Maximum(Q1[i],Q2[i],qHead);
		float	Vq1=traverse(qHead,Q1[i]);//ecg(Q1(i));
		float Vq2=traverse(qHead,Q2[i]);//Q2(i));
		float  Tv=0.18;    
			 
		if (Q1[i]==Q2[i])
			Q[i]=Q1[i];
		else  if( MVq>(Vq1+Tv))
			Q[i]=Q2[i];
		else if (Vq2>Vq1)
			Q[i]=Q1[i];
		else
			Q[i]=Q2[i];

		float Vs1=traverse(qHead,S1[i]);//ecg(S1(i));
		float Vs2=traverse(qHead,S2[i]);//ecg(S2(i));

		if (S1[i]==S2[i])
			S[i]=S1[i];
		else if( Vs2>Vs1)
			S[i]=S1[i];
		else
			S[i]=S2[i];             
		} 			
	
	//RSA and RPA Calculation which will be given for the Respiratiory Rate estimation
	float RSA[100],RPA[100];
	for(i=0;i<=COUNT;i++){ //i=1:length(Rwave)
		
						USART_puts(USART1, "i::");
						USART_Send_Num(USART1,i);
						USART_SendData(USART1,'\n');
						USART_SendData(USART1,'\r');
		RSA[i]=traverse(qHead,Rwave[i])-traverse(qHead,S[i]);    //RSA[i]=ecg(Rwave[i])-ecg(S[i]);
		if (i<=COUNT){

		 RPA[i]=Rwave[i+1]-Rwave[i];
					  USART_puts(USART1, "RPA::");
						USART_Send_Num(USART1,RPA[i]);
		}
		

					  USART_puts(USART1, "   RSA::");
						USART_Send_Num(USART1,RSA[i]);
						USART_SendData(USART1,'\n');
						USART_SendData(USART1,'\r');
		
	}


	//if counter >12 //thne send to fft and find Breating rate	
		
	return 0;   //Should return breathing rate
}


float traverse(hrQue* h,int bound)
{
	int l;
	hrQue* p;
	p=h;
	for(l=0;l<bound;l++)
		p=p->next;
	
	return p->input;	
}


int global_Min(int x,int y, hrQue* h)
{
	hrQue *p;
	int l,j,z;
	float min;
	
	if (x<0)
	x=0;
	if (y>4095)
		y=4095;
	p=h;
	for(l=0;l<x;l++) 
	p=p->next;
		    
	min=p->input;
	for( j=x;j<=y-1;j++ ){  // check for n0. of times loops runs
		if (p->next->input<=min){
			z=j+1;
			min=p->next->input;						//min=ecg(j+1);  
		}
		p=p->next;
	}
	
	return z;
	
}
	

float  Maximum(int x,int y,hrQue* h)
{
	hrQue *p;
	int l,index,i;
	float max;
	if (x<0)
	  x=0;
	
	p=h;
	
	for(l=0;l<x;l++)
		p=p->next;
  
	max=p->input;         //max=ecg(x+1);

	for(i=x;i<=y-1;i++){		
		if (p->next->input>=max){
			max=p->next->input;            //max=ecg(i+1);
			index=i+1;	
		}
		p=p->next;
	}
	
	return max;
}


int Length_LL(hrQue* h)
{
	hrQue* p;
	int length=1;
	p=h;

	while(p!=NULL){
		p=p->next;
		length++;	  
	}
	return length;
}
	
void Delay_count(uint32_t count) {
	while(count--) {
  }
}
