function [ansx, ansy,ansnum,anstheta] = work(cnttime)
%UNTITLED13 此处显示有关此函数的摘要
%   此处显示详细说明
ansx = zeros(1, 223); ansy = zeros(1, 223);
ansnum = zeros(1, 223); anstheta = zeros(1, 223);

[nownumber, nowtheta] = gethead(cnttime);
fprintf("%f %.10f\n",nownumber,nowtheta);
loc = 1;

ansnum(loc) = nownumber;
anstheta(loc) = nowtheta;

[ansx(loc), ansy(loc)] = getvertical(nownumber, nowtheta);
[nownumber, nowtheta] = getnexloc(nownumber, nowtheta, 341 - 2 * 27.5);

loc=loc+1;
[ansx(loc), ansy(loc)] = getvertical(nownumber, nowtheta);
ansnum(loc) = nownumber;
anstheta(loc) = nowtheta;
loc = loc + 1;
for i = 1 : 222
    [nownumber, nowtheta] = getnexloc(nownumber, nowtheta, 220 - 2 * 27.5);
    [ansx(loc), ansy(loc)] = getvertical(nownumber, nowtheta);

    ansnum(loc) = nownumber;
    anstheta(loc) = nowtheta;
    loc = loc + 1;
end
end
