function [x, y] = getvertical(number,theta)
%UNTITLED10 此处显示有关此函数的摘要
%   此处显示详细说明
rho=rtrho(number,theta);
x = rho * cos(theta);
y = rho * sin(theta);
end