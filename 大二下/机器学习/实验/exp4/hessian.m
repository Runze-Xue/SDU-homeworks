function [res] = hessian(m, Theta, ex4x)

    res = [0 0 0; 0 0 0;0 0 0];
    for i = 1:m
    res = res + ex4x(i,:)'*hx(Theta, ex4x(i,:)') * hx(-Theta, ex4x(i,:)')*ex4x(i,:);
    end
    res = res/m;
end