function [H] = Hessian(m, lamuda, x,theta)
    one_zeroMatrix = zeros(28, 28);
    H = zeros(28,28);
    for i = 2 : 28
        one_zeroMatrix(i, i) = 1;
    end
    for i = 1 : m
        H = H + hx(theta, x(i,:)')*(1 - hx(theta, x(i,:)'))*x(i,:)'*x(i,:);
    end
    H = H + lamuda*one_zeroMatrix;
    H = H/m;
end