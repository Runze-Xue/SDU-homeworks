x = load ("ex5Logx.dat") ;
y = load ("ex5Logy.dat") ;
figure
% Find t h e i n d i c e s f o r t h e 2 c l a s s e s
pos = find (y) ; neg = find (y == 0) ;
plot ( x(pos ,1) , x(pos,2),'+')
hold on;
plot ( x(neg, 1) , x(neg,2),'o') 
xlabel("u"); ylabel("v");
legend(" y = 1"," y = 0")
hold on;
x2 = map_feature(x(:,1), x(:,2));
theta  = zeros(28,1);
m = 117; lamuda = 10;
n  = 28;

for i = 1 : 10
    theta = theta - inv(Hessian(m, lamuda, x2,theta)) * J(lamuda, n, m ,theta, x2, y);
end


u = linspace(-1, 1.5, 200);
v = linspace(-1, 1.5, 200);
z = zeros(length(u), length(v));
for i = 1 : length(u)
    for j = 1 : length(v)
        z(j,i) = map_feature(u(i), v(j))*theta;
    end
end
z  = z';
contour(u, v, z, [0,0],'LineWidth',2);
legend('y=1','y=0','Decision Boundary');