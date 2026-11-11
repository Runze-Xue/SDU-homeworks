function [normalk] = gett4normal(number, theta)
%UNTITLED6 此处显示有关此函数的摘要
%   此处显示详细说明
%此时使用极坐标表示
r = number * 55 + (55/(2*pi)) * theta;
b = (55/(2*pi));

normalk = -1 * atan((b*cos(theta) - r * sin(theta)) / (b*sin(theta) + r* cos(theta)));
%normalk 注意，0 情况为考虑

end