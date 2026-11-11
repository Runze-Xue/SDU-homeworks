function [res] = Jd1(m, Theta, ex4x, ex4y)
    res = 0;
    for i = 1:m
        res  = res + (hx(Theta, ex4x(i,:)') - ex4y(i))*ex4x(i,:)';
    end
    res = res/m;
end