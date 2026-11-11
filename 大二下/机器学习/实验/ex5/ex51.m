t1 = readmatrix("training_2.txt");
L = length(t1(:,1));
figure;
H = zeros(L, L);
for  i = 1 : L
    for j = 1 : L
        H(i, j) = H(i, j) + t1(i, 3)*t1(j,3)*(t1(i,1)* t1(j,1) +t1(i,2)*t1(j,2));
    end
end
f = ones(L,1);
Aeq = zeros(L, 1);
Beq = 0;
for i = 1 : L
    Aeq(i) = t1(i,3);
end
C = 5;
lb = 0; ub = C;
options = optimoptions('quadprog','MaxIterations',1000); % 设置最大迭代次数为
alpha = quadprog(H, -f,[],[],Aeq', Beq,lb,ub,[],options);
% alpha = quadprog(H, -f,[],[],Aeq', Beq,[],[],[],options);
w = [0 ; 0];
for i = 1 : L
    w = w + alpha(i) * t1(i,3) * [t1(i,1); t1(i,2)];
end
typepos = 10000000000;
typeneg = -10000000000;
for i = 1 : L
    if t1(i,3) == 1
        typepos = min(typepos, w(1)*t1(i,1) + w(2)*t1(i,2));
    else
        typeneg = max(typeneg, w(1)*t1(i,1) + w(2)*t1(i,2));
    end
end
hold on;
b = -(typepos + typeneg) / 2;
k = -w(1)/w(2); b2 = -b/w(2);
displayx = 0 : 0.1 :200;
displayy = displayx * k + b2;
plot(displayx, displayy,'g');
title('C = 5')
hold on;
test1 =  readmatrix("test_2.txt");
for i = 1 : length(test1)
    if test1(i,3) == 1
        plot(test1(i,1),test1(i,2),'ro');
    else
        plot(test1(i,1),test1(i,2),'bo');
    end
    hold on;
end
hold off;
num = 0;
for i = 1 : length(test1)
    if(w(1)*test1(i,1) + w(2)* test1(i,2) + b > 0 && test1(i,3) == -1) num = num + 1;
    elseif(w(1)*test1(i,1) + w(2)* test1(i,2) + b < 0 && test1(i,3) == 1) num = num + 1;
    end
end




