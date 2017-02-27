function [y]=sumSquare(a,b,data)

sum=0;
for i=a:b
   sum=sum+(data(i))^2;
end
y=sum;
end