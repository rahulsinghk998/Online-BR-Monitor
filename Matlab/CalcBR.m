function [BR_rpa,BR_rsa,BR_value,BR]=CalcBR(sf,yRPA,yRSA,yBR,x,y,w,z,u,v,i,c) 
% yRPA=abs(fft(rpa_global));
% yRSA=abs(fft(rsa_global));
length=15000;

if x<11
x=11;
end
if w<11
w=11;
end
if u<11
u=11;
end
 [y1, BR_rpa]=Maximum(floor((x*length)/(sf*60)),floor((y*length)/(sf*60)),yRPA);
 [y2,BR_rsa]=Maximum(floor((w*length)/(sf*60)),floor((z*length)/(sf*60)),yRSA);
 [y3,BR_value]=Maximum(floor((u*length)/(sf*60)),floor((v*length)/(sf*60)),yBR);

 BR=0;
 if i==c
 
 BR_rpa=(BR_rpa-1)*((sf*60)/(length));
 BR_rsa=(BR_rsa-1)*((sf*60)/(length));
 BR_value=(BR_value-1)*((sf*60)/(length));
 
 a=(y1*y1)/sumSquare(floor((x*length)/(sf*60)),floor((y*length)/(sf*60)),yRPA);
 b=(y2*y2)/sumSquare(floor((x*length)/(sf*60)),floor((y*length)/(sf*60)),yRSA);
 
 BR=(a/(a+b))*BR_rpa+(b/(a+b))*BR_rsa;
 end
end