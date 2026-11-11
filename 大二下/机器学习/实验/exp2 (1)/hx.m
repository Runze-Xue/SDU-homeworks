function [finalans] = hx(t,ex1x, ex1y,m)
 finalans = 0;   
for i = 1 : m
     finalans = finalans + ((ex1x(i,:)*t-ex1y(i))^2);
end
finalans = finalans/(2*m);