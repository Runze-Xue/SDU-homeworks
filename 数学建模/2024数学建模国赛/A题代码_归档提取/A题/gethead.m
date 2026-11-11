function [nownumber, nowtheta] = gethead(cnttime)
%UNTITLED9 此处显示有关此函数的摘要
%   此处显示详细说明
nownumber = 16;
nowtheta = 0;
lastarclength = cnttime * 100;
while lastarclength > 0
    nownumber = nownumber - 1;
    lastarclength = lastarclength - getwholearclength(nownumber);
end

l = 0; r = 2 * pi;
%fprintf("????%.10f %.10f\n",lastarclength,0 );
%disp(lastarclength);
eps = 0.0000000001;
while  r - l > eps
    mid = (l + r) / 2; 
    if arclength(nownumber, mid) + lastarclength >= 0
        r = mid;
    else 
        l = mid;
    end
end
%fprintf("Q%.10f %.10f\n",lastarclength,arclength(nownumber, (l+r)/2) );
nowtheta = (l+r)/2;
end