figure
ECG=val(15000*13+1:15000*14,1);
xxx=(1:15000)/250;
plot(xxx,ECG);
Rwave=(Rwave-(15000*13));
Q=(Q-(15000*13));
S=(S-(15000*13));
Rwaven=Rwave/250;
Qn=Q/250;
Sn=S/250;

hold on
plot(Rwaven,ECG15000(Rwave),'rv','MarkerFaceColor','r');
hold on;
plot(Qn,ECG15000(Q),'rv','MarkerFaceColor','g');
hold on;
plot(Sn,ECG15000(S),'rv','MarkerFaceColor','y');
title('ECG Signal Red->R,Green->Q,Yellow->S component of ECG Signal');

hold on
plot(Rwave,ECG15000(Rwave),'rv','MarkerFaceColor','r');
hold on;
plot(Q,ECG15000(Q),'rv','MarkerFaceColor','g');
hold on;
plot(S,ECG15000(S),'rv','MarkerFaceColor','y');
title('ECG Signal Red->R,Green->Q,Yellow->S component of ECG Signal');

subplot(3,1,1);
rs=rsa_global/max(rsa_global);
x=(1:length(rsa_global))/(length(rsa_global)/60);
plot(x,rs,'b')
subplot(3,1,2);
x=(1:length(rpa_global))/(length(rpa_global)/60);
plot(x,rp,'r')
subplot(3,1,3);
figure
plot(BR_th)
x=(1:length(BR_th))/(length(Br_th)/60);
Undefined function or variable 'Br_th'.

for i=1:60
br(i)=BR_th(i*250);
end
figure
plot(br)
figure
plot(abs(fft(br)));




Did you mean:
x=(1:length(BR_th))/(length(BR_th)/60);
plot(x,BR_th)
subplot(3,1,1);
legend('RSA')
subplot(3,1,2);
legend('RPA')
subplot(3,1,3);







legend('BR_a_c_t_u_a_l')
figure
FFTrsa=abs(fft(rsa_global));
subplot(3,1,1);
x=(1:length(FFTrsa))/(length(FFTrsa)/60);
FFTrsa[1]=[];
 FFTrsa[1]=[];
      |
Error: Unbalanced or unexpected parenthesis or bracket.
 
FFTrsa(1)=[];
x=(1:length(FFTrsa))/(length(FFTrsa)/60);
FFTrsa=FFTrsa/max(FFTrsa);
plot(x,FFTrsa);
subplot(3,1,2);
FFTrpa(1)=[];
x=(1:length(FFTrpa))/(length(FFTrpa)/60);
plot(x,FFTrpa)
FFTrpa=FFTrpa/max(FFTrpa);
plot(x,FFTrpa,'r')
subplot(3,1,3);
FFTBR=abs(fft(BR_th));
FFTBR=FFTBR/max(FFTBR);
figure
plot(FFTBR)
FFTBR(1)=[];
plot(FFTBR)
figure
plot(FFTBR)
x=(1:length(FFTBR))/(length(FFTBR)/60);
plot(x,FFTBR);
plot(x,FFTBR);
figure
F=abs(fft(rsa_global));
plot(F)
x=(1:7499)/7499/60);
 x=(1:7499)/7499/60);
                  |
Error: Unbalanced or unexpected parenthesis or bracket.
 
x=(1:7499)/7499/60;
plot(x,FFTBR(1:7499))
x=1:7499;
x=(x*60)/7499;
plot(x,FFTBR(1:7499))
subplot(3,1,3);
plot(x,FFTBR(1:7499))
x=1:50;
x=(x*60)/50;
plot(x,FFTBR(1:7499))
Error using plot
Vectors must be the same lengths.
 
plot(x,FFTBR(1:50))
subplot(3,1,1);
legend('fft_r_s_a')
subplot(3,1,2);
legend('fft_r_p_a')
subplot(3,1,3);
legend('fft_B_R_a_c_t_u_a_l')
legend('fft_B_R__a_c_t_u_a_l')
legend('fft_B_R_a_c_t_u_a_l')
legend('BR_a_c_t_u_a_l')





























% clc;     % https://www.physionet.org/physiobank/database/slpdb/ , https://physionet.org/cgi-bin/atm/ATM
% clear all;
% close all;

function [abcde,abcd,abc,j_correct,f1,f2,RSA,RPA,win,win1,Q,Rwave,S,ecgD,ecgDf_cap]= Breathing_rate(ecg,BR_theoritical,sf,ii,jj)
disp('Breathing_rate');
ii
jj

% newplotATM;
% kuberData;
% person_3;
% sf=250;
% start=1;
% END=14770;
%ecg=VarName3(1800:15000,1);                    %IMPORTING ECG DATA SET
%ecg=val(start:END,1);
%BR_theoritical=VarName4(1800:15000,1);

%BR_theoritical=val(start:END,4);
r1=floor((40*length(ecg))/(sf*60));
r2=floor((120*length(ecg))/(sf*60));

[Maxval,Maxbin]=Maximum(r1,r2,abs(fft(ecg)));
Maxbin
RR=(length(ecg))/Maxbin;
 win=floor(0.8*(length(ecg))/Maxbin);
 win1=floor(0.60*(length(ecg))/Maxbin);
 win2=floor(0.05*(length(ecg))/Maxbin);
 
 HR=(Maxbin*sf*60)/(length(ecg));
%Finding R wave using difference operation method
for i=2:length(ecg)
ecgD(i-1,1)=ecg(i,1)-ecg(i-1,1); %ecgD is difference signal
end

%{
figure(1);
plot(ecg);
title('ECG');
%{
figure(2);
plot(ecgD);
%}
title('Difference of ECG Signal x(n)=x(n)-x(n-1)');
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
    
  if(ecgDf(i)<=T1 && T2<=ecgDf(i))
        ecgDf_cap(i)=0;   
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

% figure(4);
% plot(ecgDf_cap);
% title('Difference of ECG Signal after applying Low Pass Filter and Threshold T1 and T2 ECG_D_fcap');
% hold on;
% plot(x,T1,'r');
% hold on;
% plot(x,T2,'g');

% figure(5);
% plot(ecgDf_cap_pos);
% title('Positive part of filtered ECG'); 
% figure(6);
% plot(ecgDf_cap_neg);
% title('Negative part of filtered ECG');
% 

j=0; %index for position of Rwave;
abc=ecgDf_cap_neg;
    for i=1:length(ecgDf_cap_neg)-win %min(length(ecgDf_cap_neg),length(ecgDf_cap_pos))-win
    indexpos=i;  %variable that store the index of peak in ecgDf_cap_pos
    indexneg=i;
    i+1;
    win;
    for j=i+1:i+win
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

rPcount=1;
rNcount=1;
% check for equality condition when two points are equal
for i=2:length(ecgDf_cap_neg)-1
    if(ecgDf_cap_neg(i)<ecgDf_cap_neg(i-1)&&ecgDf_cap_neg(i)<ecgDf_cap_neg(i+1))
        r_negative(rNcount)=i;  
          rNcount=rNcount+1;
    end
end

sum=0;
for i=1:length(r_negative)
     sum=sum+ecgDf_cap_neg(r_negative(i));
end
mean_rnegative=sum/rNcount;

l=1;
i=1;
abcd=ecgDf_cap_neg;
while i<=l
    if ecgDf_cap_neg(r_negative(i))>( (mean_rnegative)/2 );
    r_negative(i)=[];
    end
    l=length(r_negative);
    i=i+1;
end


% if FLAG==0
% j_correct=0;
% else
% j_correct=1;
% end
j_correct=0;
f1=0;
f2=0;
win2
r_negative

if (r_negative(1)-win2) <=0
    j_correct=1;
    r_negative(1)
    RSA=0;
    RPA=0;
    Rwave=0;
%     BR_rpa=0;
%     BR_rsa=0;
%     BR_theoritical=0;
%     BR=0;
    return;   
end
abcde=ecgDf_cap_pos;
for i=1:length(r_negative)

    [mmm,temp]=max(ecgDf_cap_pos( (r_negative(i)-win2:r_negative(i)) ));
    r_positive(i)=temp+r_negative(i)-win2;
end

r_positive

k=1;
n=1;
m=1;

%  r_positive
%  r_negative
 
 for i=1:length(r_negative) 
 
    for j=r_positive(i):r_negative(i)
        
        if(ecg(j)>=ecg(j-1)&& ecg(j)>ecg(j+1))     
            %disp('Rwave');
            Rwave(k)=j;
            k=k+1;
        end
    end    
end
%Rwave
% figure(7)
% plot(ecgDf_cap_pos);
% title('ECG_D_fcap+');
% figure(8)
% plot(ecgDf_cap_neg);
% title('ECG_D_fcap-');

for i=1:length(Rwave)
    
    [Q1(i),flag]=global_Min(Rwave(i)-floor(0.04*sf),Rwave(i),ecg);
    [S1(i),f1]=global_Min(Rwave(i),Rwave(i)+floor(0.04*sf),ecg);
    [Q2(i),f2]=global_Min(Rwave(i)-floor(0.11*sf),Rwave(i),ecg);
    [S2(i),flag]=global_Min(Rwave(i),Rwave(i)+floor(0.11*sf),ecg);
    
%     if (f1==1 || f2==1)
%         continue
%     end
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

% 
%                                 figure
%                                 plot(ecg)
%                                 hold on
%                                 plot(Rwave,ecg(Rwave),'rv','MarkerFaceColor','r');
%                                 hold on;
%                                 plot(Q,ecg(Q),'rv','MarkerFaceColor','g');
%                                 hold on;
%                                 plot(S,ecg(S),'rv','MarkerFaceColor','y');
%                                 title('ECG Signal Red->R,Green->Q,Yellow->S component of ECG Signal');
%                                  
%                                
 for i=1:length(Rwave)
   RSA(i)=ecg(Rwave(i))-ecg(S(i));
   if (i<length(Rwave))
   RPA(i)=Rwave(i+1)-Rwave(i);
   end
 end
 
%  figure(10)
%  plot(RSA);
%  title('Plot for RSA');
%  
%  figure(11)
%  plot(RPA);
%  title('Plot for RPA');
%  figure(12)
%  plot(BR_theoritical);
%  title('BR Refrence');
 
end 
