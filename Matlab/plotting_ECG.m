close all;
clear 
clc
newplotATM;
set(0,'DefaultFigureWindowStyle','docked');

for i=1:9
figure
plot(val(1+(i-1)*10^5:10^5+(i-1)*10^5,1));
end