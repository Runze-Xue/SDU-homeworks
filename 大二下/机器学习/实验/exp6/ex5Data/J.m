function [result] = J(lamuda, n, m ,theta, x, y)
    result = zeros(28,1);
    for j = 1 : n
        for i = 1 : m
            result(j) = result(j) + ((hx(theta, x(i, :)') - y(i))*x(i, j) / m);
        end
        if(j ~= 1)
            result(j) = result(j) + lamuda*theta(j)/m;
        end
    end
end