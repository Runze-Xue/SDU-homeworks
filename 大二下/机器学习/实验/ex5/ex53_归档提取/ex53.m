t = readmatrix("training_3.text");

t1 = [t(:, 1) , t(:, 2)];
y = t(: ,3);
L = length(t1(:,1));
figure;
hold on;
for i = 1 : L
    if y(i) == 1 
        plot(t1(i, 1), t1(i, 2), 'or');
    elseif y(i) == -1
        plot(t1(i, 1), t1(i, 2), 'ob');
    end
end


lamuda = 100;
H = zeros(L, L);

for  i = 1 : L
    for j = 1 : L
        H(i, j) = y(i) * y(j) * Kernel(t1(i, :)' , t1(j, :)', lamuda);
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

% alpha = quadprog(H, -f,[],[],Aeq', Beq,[],[],[],options);

for i = 1 : L
    if alpha(i) < 0.00001 
        alpha(i) = 0;
    end
end 

% Make classificationpredictionsoveragridofvalues
xplot = linspace (min(t1(:, 1)) , max(t1(:, 1)), 100)' ;
yplot = linspace (min(t1(:, 2)), max(t1(:, 2)), 100)' ;
[X, Y] = meshgrid (xplot, yplot) ;
vals= zeros(size(X)) ;

num = 0;
b = 0;
for i = 1 : L
    if(alpha(i)> 0.0001)
        num = num + 1;
        b = b + y(i);
        for j = 1 : L
            if(alpha(j) > 0.001)
                b = b - alpha(j) * y(j) * Kernel(t1(j, :)' , t1(i, :)', lamuda); 
            end
        end
    end
end
b = b / num;

for i = 1 : size(xplot)
    for j = 1 : size(yplot)
        for k = 1 : length(alpha)
            if alpha(k) < 0.0001 
                continue;
            else 
                vals(i, j) = vals(i, j) + alpha(k) * y(k) * Kernel([X(i, j) ; Y(i, j)], t1(k, :)', lamuda) + b; 
            end
        end
    end
end

% 
% % For each pointinthisgrid , you need to compute its decision
% % value . Store the decision values invals .
% % Plot the SVM boundary
colormap bone
contour (X, Y, vals, [0 0], 'LineWidth'  , 4) 
