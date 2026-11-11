load("ex4x.dat");
load("ex4y.dat");
[m,~] = size(ex4x);
ex4x = [ones(m,1),ex4x];
pos = find(ex4y == 1);
neg = find(ex4y == 0);
plot(ex4x(pos,2), ex4x(pos,3),'+'); hold on;
plot(ex4x(neg,2),ex4x(neg,3),'o')
legend("Admitted","Not admitted");
hold on;
% 
Theta = [0; 0; 0];

for i = 1 : 5
Theta =  Theta - inv(hessian(m , Theta, ex4x))*Jd1(m,Theta,ex4x, ex4y);
end

x2 =0: 0.1:100;
y2 = -(Theta(1) + Theta(2)*x2)/Theta(3);
plot(x2, y2);
legend("Decision boundary");

testx = [1,20,80];
predict = hx(Theta, testx')
