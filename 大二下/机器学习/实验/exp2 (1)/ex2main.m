load("ex2x.dat");
load("ex2y.dat");
[m,~] = size(ex2x); 
ex2x = [ones(m,1),ex2x];

test = [1, 1650 , 3];


sigma = std(ex2x);
mu = mean(ex2x);

ex2x(:,2) = (ex2x(:,2) - mu(2))./ sigma(2);
ex2x(:,3) = (ex2x(:,3) - mu(3))./sigma(3);

test(:,2) = (test(:,2) - mu(2))./ sigma(2);
test(:,3) = (test(:,3) - mu(3))./sigma(3);
% ylabel('Height in meters');
% xlabel('Age in years');


Theta = zeros(size(ex2x(1,:)))';
J = zeros(50,1);

%0.1
Theta = [0;0;0];
alpha = 0.1;
for i = 1:50
 J(i) = hx(Theta,ex2x,ex2y,m);
    sum1 = 0;
    sum2 = 0;
    sum3 = 0;
    for j = 1:m
        sum1 = sum1 +(ex2x(j,:)*Theta-ex2y(j,:))*ex2x(j,1);
        sum2 = sum2 +(ex2x(j,:)*Theta-ex2y(j,:))*ex2x(j,2);
        sum3 = sum3 +(ex2x(j,:)*Theta-ex2y(j,:))*ex2x(j,3);
    end
    sum1 = sum1*alpha/m;
    sum2 = sum2*alpha/m;
    sum3 = sum3*alpha/m;
    Theta(1,1) = Theta(1,1)-sum1;
    Theta(2,1) = Theta(2,1)-sum2;
    Theta(3,1) = Theta(3,1)-sum3;
end
result = test*Theta