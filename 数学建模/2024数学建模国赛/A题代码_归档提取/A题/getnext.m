function [nexnumber, nextheta] = getnext(number, theta, changetheta)
%UNTITLED5 此处显示有关此函数的摘要
%   此处显示详细说明
nextheta = theta + changetheta;
nexnumber = number;
if nextheta >= 2 * pi
    nextheta = nextheta - 2 * pi;
    nexnumber = nexnumber + 1;
end
end