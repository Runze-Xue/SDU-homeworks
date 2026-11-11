function [res] = Kernel(x1, x2, lamuda)
% gauss Kernel
res = exp(-lamuda * (x1 - x2)' *(x1 - x2));
end