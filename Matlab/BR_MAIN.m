% Code for computing the Breathing Rate(BR) of a subject from ECG signal
% Using DOM method.
% https://www.physionet.org/physiobank/database/slpdb/ (Link from which I took ECG dataset)
% In the comments the word between $ represents a function call and word
% between $ represents the name of the function.
% Our Hardware (i.e in embedded platform) the BR  of the person is
% displayed after 1 minutes(if process starts at t=0sec)and then BR is updated every 10 seconds
%%
clc;        %Clearing Command Window  
clear all;  %Clearing all the previous data from work space
close all;  %Closing all the previous figures
set(0,'DefaultFigureWindowStyle','docked'); % docking all the figures
%%
% This is the main function of my algorithm to find the BR
% From here the given data set of ECG is divided into chunks of 2500 points and then they 
% are sent to $Breathing_rate$ function and then the output of Breathing
% rate function is the RSA and RPA waveform of the subject of 2500 points
% and then we save these RSA and RPA waveform in global variables rsa_global
% and rpa_global. Initially when rsa and rpa for 6 chunks of these 2500 dataset is evaluated
% BR is upadated and again the BR is updated after 10sec for this second
% iteration the points for which BR is calculated between 2500 points after
% t=0 and 2500 points after 6*2500
%% SOME INITIALIZATIONS AND LOADING DATA
newplotATM;     % $newplotATM$ is a matlab code that loads the ECG dataset into workspace corresponding to the record name specified in $newplotATm$
sf=250;         % Sampling Frequency for MIT BIH data set it was 250
a=27;           % Total number of 15000 chunks of ECG dataset that we have
%b=7;
c=1;            %  
cntrsa=1;       % Counter for counting the total number of RSA points
cntrpa=1;       % Counter for counting the total number of RPA points
COUNT_BR=1;     % Counter for counting the number of points in the reference BR waveform  
j_correct=0;    % j_correct=1 if when a chunk of 2500 points is sent to Breathing_rate and it misses a R peak due to half cut at the starting of the 2500 data point chunk
R_correct=0;    % R_correct=1 if when a chunk of 2500 points is sent to Breathing_rate and it misses a R peak due to half cut at the end of the 2500 data point chunk
cnt_Dfcap=1;
x=11;           % Our BR algo can measure the BR between 11bpm to 25bpm  bpm->breaths per minutes
y=25;            
alpha=0.5;       
CT=1;
ct=1;
pp=1;
cnt_br_th=1;     
cnt_R=1;
cnt_ecgD=1;
%%   MAIN ALGORITHM 


for i=c:a           %
   j_correct=0; 
   j=0;
   FLAG=0;
       while (END ~=15000*(i-1)+2500+(6-1)*2500)||j_correct==1

       j=j+1;
       start=15000*(i-1)+1+(j-1)*2500;          % staring index for the 2500 chunk
       END=15000*(i-1)+2500+(j-1)*2500;         % Ending index for the 2500 chunk
       
       if j_correct==1                          % Checking for a half cut at the start of the previous chunk if YES than for the next iteration the chunk is taken a little bit back so as to consider that half cut point at the ending of the last chunk can be consider at the starting of the new chunk
       FLAG=1;                                  % Flag is jus to know at the ith iteration whether j_correct was 2 for the previous iteration
       start=15000*(i-1)+1+(j-1)*2500-win1;     % New staring index shifted by win1 so as to consider the left away point
       END=15000*(i-1)+2500+(j-1)*2500;         % Ending Index of the new 2500 chunk
       end
        
        if R_correct==1                           % Checking for a half cut at the end of the previous chunk if YES than for the next iteration the chunk is taken a little bit back so as to consider that half cut point at the ending of the last chunk can be consider at the starting of the new chunk
         start=15000*(i-1)+1+(j-1)*2500-win1;     % Modified starting Index
         END=15000*(i-1)+2500+(j-1)*2500;         % Modified end of the new Index 
         IN=1;                                    % IN is for checking whether the half cut happended at the last iteration    
        end      
        R_correct=0;

       f1=0;
       f2=0;
        ecg=val(start:END,1);                    % Loadinf 2500 ECG data point in the variable ecg
        BR_theoritical=val(start:END,4);         % Loading the reference general  
        [abcde,abcd,abc,j_correct,f1,f2,RSA,RPA,win,win1,q,Rw,s,ecD,ecdf]=Breathing_rate(ecg,BR_theoritical,sf,i,j);  % Function to the Breathing_rate function

        
           if(j_correct==1)                       % If j_correct=1 then send the previous data set one more time but now with different index, 
               j=j-1;
               continue;
           end
        
           Rw=Rw+start-1;                        %Rw contians temporary R peaks of the 2500 points
           q=q+start-1;
           s=s+start-1;
           RR=Rw(2)-Rw(1);                       % Calculating R-R peaks distance
        if cnt_R ~=1                              % Will go inside this if atleast more than 2500 data points have been analysed
           
            (Rw(1)-Rwave(cnt_R-1))
            if (Rw(1)-Rwave(cnt_R-1))>1.7*RR && IN==0
            j=j-1;
            R_correct=1
            END=END-5;
            continue;
            end
        end
   IN=0;
           for cc=1:length(Rw)
           Rwave(cnt_R)=Rw(cc);
           Q(cnt_R)=q(cc);
           S(cnt_R)=s(cc);
           cnt_R=cnt_R+1;
           end
           
           for cc=1:length(ecD)
           ecgD(cnt_ecgD)=ecD(cc);
           cnt_ecgD=cnt_ecgD+1;
           end
           for cc=1:length(abc)
           ecgNEG(CT)=abc(cc);
           CT=CT+1;
           end
           for cc=1:length(abcd)
           ecgNNEG(ct)=abcd(cc);
           ct=ct+1;
           end
           for cc=1:length(abcde)
           ecgpos(pp)=abcde(cc);
           pp=pp+1;
           end
           for cc=1:length(ecdf)
           ecgDf_cap(cnt_Dfcap)=ecdf(cc);
           cnt_Dfcap=cnt_Dfcap+1;
           end
         
        for g=1:length(RSA)
        rsa_global(cntrsa)=RSA(g);
        cntrsa=cntrsa+1;
        end
        rsa_pos(i,j)=cntrsa;                  % rsa_pos(i,j) remembers the index of the previous 2500 data chunk end
        
        for g=1:length(RPA)
        rpa_global(cntrpa)=RPA(g);
        cntrpa=cntrpa+1;
        end
        rpa_pos(i,j)=cntrpa; 
        
        for g=1:length(BR_theoritical)
        BR_th(cnt_br_th)=BR_theoritical(g);
        cnt_br_th=cnt_br_th+1;
        end
        br_pos(i,j)=cnt_br_th;
        


        if i~=c
            i
            disp('hello')
        yRPA=abs(fft(rpa_global(rpa_pos(i-1,j):length(rpa_global))));
        yRSA=abs(fft(rsa_global(rsa_pos(i-1,j):length(rsa_global))));
        yBR=abs(fft(BR_th(br_pos(i-1,j):length(BR_th))));
        
        [BRrpa11,BRrsa11,BRvalue11,nt]=CalcBR(sf,yRPA,yRSA,yBR,x,y,x,y,x,y,i,c);
        [BRrpa3,BRrsa3,BRvalue3,nt]=CalcBR(sf,yRPA,yRSA,yBR,BR_rpa(COUNT_BR-1)-2,BR_rpa(COUNT_BR-1)+4,BR_rsa(COUNT_BR-1)-2,BR_rsa(COUNT_BR-1)+4,BR_value(COUNT_BR-1)-2,BR_value(COUNT_BR-1)+4,i,c);
       
        BRrpa=BRrpa3+(BRrpa11-BRrpa3)*alpha;
        BRrsa=BRrsa3+(BRrsa11-BRrsa3)*alpha;
        BRvalue=BRvalue3+(BRvalue11-BRvalue3)*alpha;
        
        
        y1=yRPA(floor(BRrpa));
        y2=yRPA(floor(BRrsa));
        BRrpa=(BRrpa-1)*((sf*60)/15000);
        BRrsa=(BRrsa-1)*((sf*60)/15000);
        BRvalue=(BRvalue-1)*((sf*60)/15000);
        len=15000;
        
         a=(y1*y1)/sumSquare(floor((11*len)/(sf*60)),floor((25*len)/(sf*60)),yRPA);
         b=(y2*y2)/sumSquare(floor((11*len)/(sf*60)),floor((25*len)/(sf*60)),yRSA);

         BR1=(a/(a+b))*BRrpa+(b/(a+b))*BRrsa;
        
         BR_rpa(COUNT_BR)=BRrpa;
         BR_rsa(COUNT_BR)=BRrsa;
         BR_value(COUNT_BR)=BRvalue;
         BR(COUNT_BR)=BR1;
         COUNT_BR=COUNT_BR+1;
        end
        
       end
         if i==c
        yRPA=abs(fft(rpa_global));                  % Calculating the FFT of the RSA and RPA and BR referencewaveform and they are sent ot $CalcBr$ function for calculating the index 
        yRSA=abs(fft(rsa_global));
        yBR=abs(fft(BR_th));      
        
        [BR_rpa(COUNT_BR),BR_rsa(COUNT_BR),BR_value(COUNT_BR),BR(COUNT_BR)]=CalcBR(sf,yRPA,yRSA,yBR,x,y,x,y,x,y,i,c);
        COUNT_BR=COUNT_BR+1;
        end
    
       
end

       
       
BR=BR';
BR_value=BR_value';
rsa_global=rsa_global';
rpa_global=rpa_global';
BR_rpa=BR_rpa';
BR_rsa=BR_rsa';

error=abs(BR-BR_value);
figure
plot(abs(error))
MEAN_ERROR=mean(error)
std_BR=std(error)
MEAN_SQUARE=meansqr(error)
MEAN_BRrpa=mean(abs(BR_rpa-BR_value))
MEAN_BRrsa=mean(abs(BR_rsa-BR_value))
figure
plot(BR_value,'r')
hold on
plot(BR)
Total(:,1)=BR;
Total(:,2)=BR_value;
Total(:,3)=BR_rsa;
Total(:,4)=BR_rpa;
RSA=RSA';
RPA=RPA';

for i=1:60
    br(i)=BR_th(i*250);
end