function [finalans] = hx(t,ex1x, ex1y)
 finalans = 0;   
for i = 1 : 50
     finalans = finalans + ((ex1x(i,:)*t-ex1y(i))^2);
end