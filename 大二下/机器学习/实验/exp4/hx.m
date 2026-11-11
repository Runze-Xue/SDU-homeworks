function [g] = hx(Theta, ex4x)
g = 1.0/(1.0 + exp(-Theta'*ex4x));
end