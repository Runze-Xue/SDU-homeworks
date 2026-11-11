for i=1:223
    x1=ansx(i);
    x2=ansx(i+1);
    y1=ansy(i);
    y2=ansy(i+1);
    p=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    disp(p);
end

