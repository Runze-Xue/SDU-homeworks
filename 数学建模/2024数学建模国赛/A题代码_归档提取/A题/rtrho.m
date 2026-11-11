function [length] = rtrho(number, theta) % 0 - 2pai, 返回的是螺旋线半径
%UNTITLED3 此处显示有关此函数的摘要
%   此处显示详细说明
length = 55 * number + (55 / (2 * pi)) * theta;
end