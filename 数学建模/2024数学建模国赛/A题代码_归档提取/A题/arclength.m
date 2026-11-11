function [length] = arclength(number, theta)
%UNTITLED6 此处显示有关此函数的摘要
%   此处显示详细说明
% ax + 1/2 b* x * x 
%a = 55 * number;
%b = 55 / (2 * pi)
%length = 55 * number * theta + (1 / 2) * (55 / (2 * pi)) * theta * theta;
a=55/(2*pi);
theta1=number*2*pi+theta;
theta2=number*2*pi;
s1=(a/2)*(theta1*sqrt(theta1*theta1+1)+log(theta1+sqrt(theta1*theta1+1)));
s2=(a/2)*(theta2*sqrt(theta2*theta2+1)+log(theta2+sqrt(theta2*theta2+1)));
length=s1-s2;
end