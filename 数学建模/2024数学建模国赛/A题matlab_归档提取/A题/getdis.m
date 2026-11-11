function [length] = getdis(number1 , theta1, number2, theta2)
%UNTITLED7 此处显示有关此函数的摘要
%   此处显示详细说明
rho1=rtrho(number1,theta1);
rho2=rtrho(number2,theta2);
length = sqrt(rho1*rho1 + rho2*rho2 - 2 * rho1 * rho2 *cos(theta1 - theta2));

end