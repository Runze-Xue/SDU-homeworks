load("ex1x.dat");
load("ex1y.dat");
figure;
plot(ex1x,ex1y,'o');
alpha = 0.07;
ylabel('Height in meters');
xlabel('Age in years');
m =  length(ex1y);
ex1x = [ones(m,1),ex1x];
Theta = [0, 0];

for i = 1:1500
    sum1 = 0;
    sum2 = 0;
    for j = 1:50
        sum1 = sum1 +(Theta*ex1x(j,:)'-ex1y(j,:))*ex1x(j,1);
        sum2 = sum2 +(Theta*ex1x(j,:)'-ex1y(j,:))*ex1x(j,2);
    end
    sum1 = sum1*alpha/m;
    sum2 = sum2*alpha/m;
    Theta(1,1) = Theta(1,1)-sum1;
    Theta(1,2) = Theta(1,2)-sum2;
end

hold on
plot(ex1x(:,2),ex1x*Theta','-');
legend('Training data', 'Linear regression')

%预测
boys = [1,3.5;1,7];
for i = 1:2
boysans(i) = Theta*boys(i,:)';
end
boysans
J_vals = zeros(100,100);
theta0_vals = logspace(-3,3,100);
theta1_vals = logspace(-1,1,100);

for i = 1 : length(theta1_vals)
    for j = 1:length(theta1_vals)
        t = [theta0_vals(i); theta1_vals(j)];
        J_vals(i,j) = hx(t, ex1x ,ex1y)/(m*2);
    end
end

J_vals = J_vals';
figure;
surf(theta0_vals, theta1_vals,J_vals);
hold on
contour(theta0_vals, theta1_vals,J_vals);
xlabel('\theta_0'); ylabel('\theta_1');


