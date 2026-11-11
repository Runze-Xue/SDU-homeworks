function [length] = get2pdistance(theta1 , rho1, theta2, rho2)
%UNTITLED7 此处显示有关此函数的摘要
%   此处显示详细说明
length = sqrt(rho1*rho1 + rho2*rho2 - 2 * rho1 * rho2 *cos(theta1 - theta2));

end