function [res] = average_entropy(A) % A is a attibute × 2 that not sort , A是n行两列矩阵 
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
res = 0;
check = zeros(length(A(1, :)), 1);
for i = 1: length(A)
    if check(i) == 0 
    countnum = 0;
    [row, ~] = find(A(:, 1) == A(i, 1)); %：这将返回矩阵A中所有等于val的元素的行索引和列索引。
    tmp = zeros(length(row), 1);
    for j = 1 : length(row)
        check(row(j)) = 1;
        tmp(j) = A(row(j), 2);
    end
    countnum = length(row);
    res = res + engtropy(tmp) / countnum;
    end
end
end