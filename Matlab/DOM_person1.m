clc;     % https://www.physionet.org/physiobank/database/slpdb/
clear all;
close all;
%kuberData;
%person_3;
sf=250;

newplotATM;
start=10000+4.5*10^4;
Total_data=12500+4.5*10^4;
ecg=val(start:Total_data,1);
BR_theoritical=val(start:Total_data,4);
r1=floor((40*length(ecg))/(sf*60));
r2=floor((120*length(ecg))/(sf*60));

[Maxval,Maxbin]=Maximum(r1,r2,abs(fft(ecg)));
 win=floor(0.7*(length(ecg))/Maxbin);
 win1=floor(0.40*(length(ecg))/Maxbin);
 win2=floor(0.08*(length(ecg))/Maxbin);

 HR=(Maxbin*sf*60)/(length(ecg));
%Finding R wave using difference operation method
for i=2:length(ecg)
ecgD(i-1,1)=ecg(i,1)-ecg(i-1,1); %ecgD is difference signal
end

%{
%subplot(y,1,1);
figure(1);
plot(ecg);
title('ECG');
%{
subplot(y,1,2);
figure(2);
plot(ecgD);
%}
title('Difference of ECG Signal x(n)=x(n)-x(n-1)');
%coef=fir1(50,0.5);
%ecgDf=filter(coef,1,ecgD);
ecgDf=ecgD;
%ecgDf=filter(Hd,ecgD); %Hd represents the low pass filter, ecgDf represent the ecgD after applying LPF 
%subplot(y,1,3);

figure(3);
plot(ecgDf);
title('Difference of ECG Signal after applying Low Pass Filter');
%}

sum_positive=0;
pos=0;
sum_negative=0;
neg=0;
ecgDf=ecgD;
for i=2:length(ecgDf)-1
 if(ecgDf(i)>=ecgDf(i-1) && ecgDf(i)>=ecgDf(i+1)) 
     sum_positive=sum_positive+ecgDf(i);
     pos=pos+1;
 end
 if(ecgDf(i)<=ecgDf(i-1) && ecgDf(i)<=ecgDf(i+1))
     sum_negative=sum_negative+ecgDf(i);
     neg=neg+1;
 end
end

%T1=2MVp T2=2MVn
T1=2*(sum_positive/pos); %sum_positive/pos represents the mean of all amplitude s.t ecg(i)>0,T1 is the threshold for the positive,this is done to remove all disturbances between R-R peaks 
T2=2*(sum_negative/neg); %sum_negative/neg represents the mean of all amplitude s.t ecg(i)<0,T2 is the threshold for the negative

for i=1:length(ecgDf)
    
   % if( (0<=ecgDf(i)&&ecgDf(i)<=T1)||(T2<=ecgDf(i)&&ecgDf(i)<=0) )
  % if( (0<=ecgDf(i)&&ecgDf(i)<=T1)||(T2<=ecgDf(i)&&ecgDf(i)<=0) )
     if(ecgDf(i)<=T1 && T2<=ecgDf(i))
        ecgDf_cap(i)=0;           %
    end
    
    if(ecgDf(i)>T1 || ecgDf(i)<T2)
        ecgDf_cap(i)=ecgDf(i);
    end
    
    if(ecgDf_cap(i)>=0)
        ecgDf_cap_pos(i)=ecgDf_cap(i);  %storing positive part of ecgDf_cap
        ecgDf_cap_neg(i)=0;    
    end
    
    if(ecgDf_cap(i)<=0)
        ecgDf_cap_neg(i)=ecgDf_cap(i);  %storing negative part of ecgDf_cap
        ecgDf_cap_pos(i)=0;   
    end
end
x=1:length(ecgDf_cap);
%subplot(y,1,4);

% figure;
% plot(ecgDf_cap);
% title('Difference of ECG Signal after applying Low Pass Filter and Threshold T1 and T2 ECG_D_fcap');
% hold on;
% plot(x,T1,'r');
% hold on;
% plot(x,T2,'g');


%subplot(y,1,6);
figure;
plot(ecgDf_cap_pos);
title('Positive part of filtered ECG');

figure;
plot(ecgDf_cap_neg);
title('Negative part of filtered ECG');


j=0; %index for position of Rwave;
%for i=1:14988-win
%for i=1:length(ecgDf_cap_pos)-win
    for i=1:min(length(ecgDf_cap_neg),length(ecgDf_cap_pos))-win
    indexpos=i;  %variable that store the index of peak in ecgDf_cap_pos
    indexneg=i;
    i+1;
    win;
    for j=i+1:i+win
%         if(ecgDf_cap_pos(j)>ecgDf_cap_pos(indexpos)) %check for equality condition and also check for saving R index here itself
%         ecgDf_cap_pos(indexpos)=0;
%         indexpos=j;
%         
%         else
%             ecgDf_cap_pos(j)=0;
%         end
        
        if(ecgDf_cap_neg(j)<ecgDf_cap_neg(indexneg))
        ecgDf_cap_neg(indexneg)=0;
        indexneg=j;
        else
            ecgDf_cap_neg(j)=0;
        end
    end
    wn(i,1)=i+1;
    wn(i,2)=i+win;
    end
figure;
plot(ecgDf_cap_neg);
title('Negative part of filtered ECG');

rPcount=1;
rNcount=1;

for i=2:length(ecgDf_cap_pos)-1
%     if(ecgDf_cap_pos(i)>ecgDf_cap_pos(i-1)&&ecgDf_cap_pos(i)>ecgDf_cap_pos(i+1))
%         r_positive(rPcount)=i;  
%           rPcount=rPcount+1;
%     end
    
    if(ecgDf_cap_neg(i)<ecgDf_cap_neg(i-1)&&ecgDf_cap_neg(i)<ecgDf_cap_neg(i+1))
        r_negative(rNcount)=i;  
          rNcount=rNcount+1;
    end
end
sum=0;
length(r_negative)
for i=1:length(r_negative)
     sum=sum+ecgDf_cap_neg(r_negative(i));
end
mean_rnegative=sum/rNcount;

l=1;
i=1;
while i<=l
    if ecgDf_cap_neg(r_negative(i))>( (mean_rnegative)/2 );
    r_negative(i)=[];
    end
    l=length(r_negative);
    i=i+1;
end

for i=1:length(r_negative)

    [mmm,temp]=max(ecgDf_cap_pos( (r_negative(i)-win2:r_negative(i)) ));
    r_positive(i)=temp+r_negative(i)-win2;
end
k=1;
n=1;
m=1;


%for i=m:min(length(r_negative),length(r_positive));
 for i=1:length(r_negative) 
 
    for j=r_positive(i):r_negative(i)
        
        if(ecg(j)>=ecg(j-1)&& ecg(j)>ecg(j+1))     
            %disp('Rwave');
            Rwave(k)=j;
            k=k+1;
        end
    end    
end
figure
plot(ecgDf_cap_pos);
title('ECG_D_fcap+');
figure
plot(ecgDf_cap_neg);
title('ECG_D_fcap-');
flags1=0;
flags2=0;
for i=1:length(Rwave)
    [Q1(i),flag]=global_Min(Rwave(i)-floor(0.04*sf),Rwave(i),ecg);
    [S1(i),flags1]=global_Min(Rwave(i),Rwave(i)+floor(0.04*sf),ecg);
    [Q2(i),flag]=global_Min(Rwave(i)-floor(0.11*sf),Rwave(i),ecg);
    [S2(i),flags2]=global_Min(Rwave(i),Rwave(i)+floor(0.11*sf),ecg);
    
    if (flags1==1 || flags2==1)
        
        continue
    end
        [MVq,index]=Maximum(Q1(i),Q2(i),ecg);
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

figure(20)
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
 
 figure
 plot(RSA);
 title('Plot for RSA');
 
 figure
 plot(RPA);
 title('Plot for RPA');
 figure
 plot(BR_theoritical);
 title('BR Refrence');
 % these values are for 15000 samples HARD CODED
 yRPA=abs(fft(RPA));
 yRSA=abs(fft(RSA));
 
 [y1, BR_rpa]=Maximum(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)), yRPA);
 [y2,BR_rsa]=Maximum(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),yRSA);
 %[y3,BR_theoritical]=Maximum(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),abs(fft(BR_theoritical)));
 [y3,BR_value]=Maximum(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),abs(fft(BR_theoritical)));
 BR_rpa=(BR_rpa-1)*((sf*60)/(length(ecg)))
 BR_rsa=(BR_rsa-1)*((sf*60)/(length(ecg)))
 
 %BR_theoritical=(BR_theoritical-1)*((sf*60)/(length(ecg)))
 BR_value=(BR_value-1)*((sf*60)/(length(ecg)))
 

 %a=sumSquare(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),yRPA)/sumSquare(1,length(yRPA),yRPA);
 %b=sumSquare(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),yRSA)/sumSquare(1,length(yRSA),yRSA);
 
 a=(y1*y1)/sumSquare(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),yRPA)
 b=(y2*y2)/sumSquare(floor((11*length(ecg))/(sf*60)),floor((25*length(ecg))/(sf*60)),yRSA)
 
 BR=(a/(a+b))*BR_rpa+(b/(a+b))*BR_rsa
 
 