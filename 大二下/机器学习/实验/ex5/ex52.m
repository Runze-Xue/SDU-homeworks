[t1, y] = opensvm('train-01-images.svm');
L = length(t1(:,1));
% figure;
H = zeros(L, L);
for  i = 1 : L
    for j = 1 : L
        H(i, j) = y(i) * y(j) * t1(i, :) * t1(j, :)';
    end
end

f = ones(L,1);
Aeq = zeros(L, 1);
Beq = 0;
for i = 1 : L
    Aeq(i) = y(i);
end

C = 5;
lb = zeros(L,1); ub = C * ones(L, 1);
options = optimoptions('quadprog','MaxIterations',500); % 设置最大迭代次数为
alpha = quadprog(H, -f,[], [], Aeq', Beq, lb, ub, [], options);

% alpha = quadprog(H, -f,[],[], Aeq', Beq);

for i = 1 : L
    if alpha(i) < 0.00001 
        alpha(i) = 0;
    end
end

w = zeros(size(t1, 2),1);
for i = 1 : L
    w = w + alpha(i) * y(i) * t1(i,:)';
end

num = 0;
b = 0;
tag = 0;

for i = 1 : L
    if(alpha(i)> 0.001)
        num = num + 1;
        b = b + y(i);
        for j = 1 : L
            if(alpha(j) > 0.001)
                b = b - alpha(j)*y(j) * t1(j,:) * t1(i,:)'; 
            end
        end
    end
end
b = b / num;

[t2, y2] = opensvm('test-01-images.svm');
% [t2, y2] = opensvm('train-01-images.svm');
L2 = length(t2);
cnt = 1;

keep = zeros(L, 1);
% training repredict wrong about 50%
for i = 1 : L2
    if sign(w' * t2(i,:)' + b) * y2(i) < 0 
        keep(cnt) = i;
        cnt = cnt + 1;
        figure;
        strimage(i) ; 
    end
end
cnt = cnt - 1;

