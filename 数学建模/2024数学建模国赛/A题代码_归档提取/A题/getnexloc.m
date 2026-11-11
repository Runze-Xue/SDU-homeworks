function [nexnumber, nextheta] = getnexloc(nownumber, nowtheta, distance)
%UNTITLED12 此处显示有关此函数的摘要
%   此处显示详细说明
l = 0; r = pi;
eps = 0.0000001;
while r - l > eps
    mid = (l + r) / 2;
    [tmpnumber ,tmptheta] = getnext(nownumber, nowtheta, mid);
    %fprintf("P%f %f %f %f %f %f %f\n",nownumber, nowtheta, tmpnumber, tmptheta,mid,getdis(nownumber, nowtheta, tmpnumber, tmptheta),distance);
    if getdis(nownumber, nowtheta, tmpnumber, tmptheta) >= distance
        r = mid;
    else 
        l = mid;
    end
end
[nexnumber, nextheta] = getnext(nownumber, nowtheta, l);

end