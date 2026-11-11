function [res] = entropy(A)% A is a length × 1 Vector that point out class
res = 0;
totalnum =  length(A);
P_num = 0; N_num = 0;
for i = 1 : totalnum
    if A(i) == 0 
        N_num = N_num + 1;
    elseif A(i) == 1
        P_num = P_num + 1;
    end
end

if totalnum ~= 0
    P_res = 0;
    N_res = 0;
    if P_num ~= 0
        P_res = P_num / totalnum * log2(P_num / totalnum);
    end
    if N_num ~= 0
        N_res = N_num / totalnum *log2(N_num / totalnum);
    end
    res = -P_res -N_res;
end
end