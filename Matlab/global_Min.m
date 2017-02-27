function [z,flag]= global_Min(x,y,ecg)

   if x<=0
       x=1;
   end
   min=ecg(x);
   z=x;
   flag=0;
   if y>=length(ecg)
       flag=1;
   end
   
   for j=x:y-1
       if(j+1<=length(ecg))
       if ecg(j+1)<=min
           z=j+1;
           min=ecg(j+1);
       end
       end
   end
end