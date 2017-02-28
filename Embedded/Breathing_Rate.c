#define 	_FPU_USED
#define 	ARM_MATH_CM4
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

#include "stdio.h"
#include "USART_config.h"
#include "Breathing_Rate.h"
//#include "filter_data.c"

#include "stdio.h"
#include "stdlib.h"

#define Lecg 1024

float ecga[Lecg];

typedef struct hrList hrQue;  

float Breathing_Rate(/*hrQue *head,*/ int win)
{
			hrQue *p,*prev,*ecg,*head;//,*r_positive,*r_negative;//,*ecgD,*prev,*Dhead;*prevP,*prevN,
			float32_t ecgD[Lecg];
			int r_positive[100],r_negative[100],i,Rwave[100];
	    
	for(int i=0;i<Lecg;i++)
	{  
		 ecg=(hrQue*)malloc(sizeof(hrQue));
	   ecg->input= ecga[i];
		 ecg->next=NULL;
		 if(i==0)
			 head=ecg;
		 else
			 prev->next=ecg;
		
		 prev=ecg;
		
	}

	
	/*
	//Finding Difference ECG Signal
while(p!=NULL)
{
	ecgD=(hrQue*)malloc(sizeof(hrQue));
	ecgD->input=(p->next)->input - p->input ;
	ecgD->next=NULL;
	if (p==head)
	Dhead=ecgD;
	else
	prev->next=ecgD;
	
	prev=ecgD;
	p=p->next;
	
}
*/
p=head;
for (i=0;i<Lecg;i++)
{
			ecgD[i]=((p->next)->input) - (p->input);   //ecgD x(n)=x(n)-x(n-1) is difference signal
			p=p->next;
}

int sum_positive=0;
int pos=0;
int sum_negative=0;
int neg=0;

for (i=1;i<Lecg;i++)
{
		 if(ecgD[i]>=ecgD[i-1] && ecgD[i]>=ecgD[i+1])
		 {	 
			 sum_positive=sum_positive+ecgD[i];
			 pos=pos+1;
		 }
		 
		 if(ecgD[i]<=ecgD[i-1] && ecgD[i]<=ecgD[i+1])
		 {
				sum_negative=sum_negative+ecgD[i];
				neg=neg+1;
		 }

}
//T1=2MVp T2=2MVn
float T1=2*(sum_positive/pos); //sum_positive/pos represents the mean of all amplitude s.t ecg(i)>0,T1 is the threshold for the positive,this is done to remove all disturbances between R-R peaks 
float T2=2*(sum_negative/neg); //sum_negative/neg represents the mean of all amplitude s.t ecg(i)<0,T2 is the threshold for the negative

for (i=0;i<Lecg;i++)
{
     if(ecgD[i]<=T1 && T2<=ecgD[i])
		 {
       //ecgD_cap[i]=0;           %
		     ecgD[i]=0;
		 }
    
    if(ecgD[i]>T1 || ecgD[i]<T2)
		{
				//ecgD_cap[i]=ecgD[i];
          ecgD[i]=ecgD[i];
		}

 }

int indexpos;
int indexneg;	
int j=0; //index for position of Rwave;

 
 //for i=1:14988-win
//for i=1:length(ecgDf_cap_pos)-win
  //  for (i=1;i<=min;i=i+1) i=1:min(length(ecgD_cap_neg),length(ecgD_cap_pos))-win
	for (i=0;i<Lecg-win;i++)
	{
    indexpos=i;  //variable that store the index of peak in ecgDf_cap_pos
    indexneg=i;
						for (j=i+1;i<=i+win;i++)//:i+win
						{
								if(ecgD[i]>=0)
								{
										if(ecgD[j]>ecgD[indexpos]) //check for equality condition and also check for saving R index here itself
										{
												ecgD[indexpos]=0;
												indexpos=j;
										}
										else
										{ 
												ecgD[j]=0;
										}
								}
								if(ecgD[i]<=0)
								{
										if(ecgD[j]<ecgD[indexneg])
										{
												ecgD[indexneg]=0;
												indexneg=j;
										}
										else
										{
										   ecgD[j]=0;
								    }
						    }
						}
  }

int rPcount=1;
int rNcount=1;
		
// check for equality condition when two points are equal
			for (i=1;i<Lecg-1;i++)
			{
							if(ecgD[i]>=0)
							{
									if(ecgD[i]>ecgD[i-1]&&ecgD[i]>ecgD[i+1])
									{
											/*
											r_positive=(hrQue*)malloc(sizeof(hrQue));
											r_positive->input=i;
											r_positive->next=NULL;
											prevP=r_positive;
											if(rPcount!=1)
											prevP->next=r_positive;			
											*/ 
											r_positive[rPcount]=i;
											rPcount=rPcount+1;
									}
							}

							if(ecgD[i]<=0)
							{
									if(ecgD[i]<ecgD[i-1]&&ecgD[i]<ecgD[i+1])
									{
											/*	
											r_negative=(hrQue*)malloc(sizeof(hrQue));
											r_negative->input=i;
											r_negative->next=NULL;
											prevN=r_negative;
											if(rNcount!=1)
											prevN->next=r_negative;			
											*/  
											r_negative[rNcount]=i;
											rNcount=rNcount+1;
									}
							}
			}
	

int k=1;
int n=1;
int m=1;
			
			if ((r_positive[1]-r_negative[1])>win)
			{  
					n=n+1;
					//if length(r_negative)==length(r_positive)
					if(rNcount==rPcount)
					{	
							//r_positive[rNcount]=[];
						rPcount--;
					}
			}

			
 if ((r_positive[1]-r_negative[1])>0)
 {
     n=n+1;
     m=m+1;
 }
 
 p=head;
 int l,min; 
 
 if(rPcount<rNcount)
	 min=rPcount;
 else
	 min=rNcount;
 
for (i=m;i<min;i++)
 {
  //  for j=r_positive(i)-2:_negative(i) 
    for ( j=r_positive[i];j<=r_negative[n];j++)
		{
       // if(ecg[j]>=ecg[j-1]&& ecg[j]>ecg[j+1])    
           for(l=1;l<j;l++)
           {
				     prev=p;
				     p=p->next;
			     }			
          if( (p->input>=prev->input)&&((p->input > p->next->input) ) )   
					{
      			Rwave[k]=j;
            k=k+1;
					}
		}
	 n=n+1; 
 }
  
}		

/*

for i=1:length(Rwave)
    
    Q1(i)=global_Min(Rwave(i)-floor(0.04*sf),Rwave(i)-1,ecg);
    S1(i)=global_Min(Rwave(i),Rwave(i)+floor(0.04*sf),ecg);
    Q2(i)=global_Min(Rwave(i)-floor(0.11*sf),Rwave(i)-1,ecg);
    S2(i)=global_Min(Rwave(i),Rwave(i)+floor(0.11*sf)-1,ecg);
    
        [MVq,index]=Maximum(Q1(i),Q2(i)-1,ecg);
        Vq1=ecg(Q1(i));
        Vq2=ecg(Q2(i));
        Tv=0.18;
        
   
    if Q1(i)==Q2(i)
        Q(i)=Q1(i);
        
        else if MVq>Vq1+Tv
        Q(i)=Q2(i);
         
            else if Vq2>Vq1
                    Q(i)=Q1(i);
                else
                   Q(i)=Q2(i);
                end
            end
    end
        Vs1=ecg(S1(i));
        Vs2=ecg(S2(i));
      
        if S1(i)==S2(i)
         S(i)=S1(i);
        
             else if Vs2>Vs1
                S(i)=S1(i);
                 else
                    S(i)=S2(i);             
                 end
        end
        
end

figure(9)
plot(ecg)
hold on
plot(Rwave,ecg(Rwave),'rv','MarkerFaceColor','r');
hold on;
plot(Q,ecg(Q),'rv','MarkerFaceColor','g');
hold on;
plot(S,ecg(S),'rv','MarkerFaceColor','y');
title('ECG Signal Red->R,Green->Q,Yellow->S component of ECG Signal');

 for i=1:length(Rwave)
   RSA(i)=ecg(Rwave(i))-ecg(S(i));
   if (i<length(Rwave))
   RPA(i)=Rwave(i+1)-Rwave(i);
   end
 end
 
 figure(10)
 plot(RSA);
 title('Plot for RSA');
 
 figure(11)
 plot(RPA);
 title('Plot for RPA');
 figure(12)
 plot(BR_theoritical);
 title('BR Refrence');
 % these values are for 15000 samples HARD CODED
 [y, BR_rpa]=Maximum(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),abs(fft(RPA)));
 [y,BR_rsa]=Maximum(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),abs(fft(RSA)));
 [y,BR_theoritical]=Maximum(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),abs(fft(BR_theoritical)));
 BR_rpa=(BR_rpa-1)*((sf*60)/(length(ecg)))
 BR_rsa=(BR_rsa-1)*((sf*60)/(length(ecg)))
 BR_theoritical=(BR_theoritical-1)*((sf*60)/(length(ecg)))

*/








