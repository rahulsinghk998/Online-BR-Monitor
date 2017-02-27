function [y,index]= Maximum(x,y,data)
    if x<=0
        x=1;
    end
index=x;
max=data(x);
    for i=x:y-1
        if data(i+1)>=max
            max=data(i+1);
            index=i+1;
        end
    end
y=max;
%index=index1;
end