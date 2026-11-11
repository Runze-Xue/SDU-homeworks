function [centerx1, centery1, centerx2, centery2,centerx3, centery3, centerx4, centery4] = getcenter(number1, theta1, number2, theta2,ansx,ansy) %使用极坐标传入两个点
%UNTITLED7 此处显示有关此函数的摘要
%   此处显示详细说明
pitch = 170; %螺距
a1 = number1 * pitch;
a2 = number2 * pitch;
b = pitch / (2* pi);
rho1 = a1 + b * theta1;
rho2 = a2 + b * theta2;
[x1, y1] = arc_vertical(rho1, theta1);
[x2, y2] = arc_vertical(rho2, theta2);
normal1 = gett4normal(number1, theta1);
normal2 = gett4normal(number2, theta2);

syms k11 k12;
eq1 = k11-2*k12 ==0;
eq2 = ((x1 - cos(normal1)*k11) - (x2 - sin(normal2)*k12))^2 +((y1 - cos(normal1)*k11) - (y2 - sin(normal2)*k12))^2-(k11 + k12)^2 == 0;
solutions = solve([eq1, eq2], [k11, k12]);
sol11 = solutions.k11;
sol12 = solutions.k12;
centerx1 = sol11;
centery1 = sol12;


syms k21 k22;
eq1 = k21-2*k22 ==0;
eq2 = ((x1 + cos(normal1)*k21) - (x2 - sin(normal2)*k22))^2 +((y1 + cos(normal1)*k21) - (y2 - sin(normal2)*k22))^2-(k21 + k22)^2 == 0;
solutions = solve([eq1, eq2], [k21, k22]);
sol21 = solutions.k21;
sol22 = solutions.k22;
centerx2 = sol21;
centery2 = sol22;

syms k31 k32;
eq1 = k31-2*k32 ==0;
eq2 = ((x1 - cos(normal1)*k31) - (x2 + sin(normal2)*k32))^2 +((y1 - cos(normal1)*k31) - (y2 + sin(normal2)*k32))^2-(k31 + k32)^2 == 0;
solutions = solve([eq1, eq2], [k31, k32]);
sol31 = solutions.k31;
sol32 = solutions.k32;
centerx3 = sol31;
centery3 = sol32;

syms k41 k42;
eq1 = k41-2*k42 ==0;
eq2 = ((x1 + cos(normal1)*k41) - (x2 + sin(normal2)*k42))^2 +((y1 + cos(normal1)*k41) - (y2 + sin(normal2)*k42))^2-(k41 + k42)^2 == 0;
solutions = solve([eq1, eq2], [k41, k42]);
sol41 = solutions.k41;
sol42 = solutions.k42;
centerx4 = sol41;
centery4 = sol42;

% 设置螺旋线参数
b = 170 / (2 * pi);  % 每圈的间距为 55
num_turns = 16;     % 圈数
theta_max = num_turns * 2 * pi;  % 最大角度（16圈）
theta = linspace(0, theta_max, 1000);  % 生成角度范围

% 计算螺旋线的半径 r = b * theta
r = b * theta;

% 将极坐标转换为笛卡尔坐标
x = r .* cos(theta);  % x = r * cos(theta)
y = r .* sin(theta);  % y = r * sin(theta)

% 绘制等距螺旋线
figure;
plot(x, y, 'b', 'LineWidth', 2);  % 绘制蓝色螺旋线
hold on;  % 保持图形窗口，继续绘制点

% 假设数组 points 中包含要绘制的点 (r, theta)，例如：
points_r = [100, 200, 300];  % 半径
points_theta = [pi/6, pi/3, pi/2];  % 对应的角度

% 将数组中的极坐标转换为笛卡尔坐标
%points_x = points_r .* cos(points_theta);
%points_y = points_r .* sin(points_theta);

points_x = ansx;
points_y = ansy;

% 在图中绘制这些点，红色圆点标记
plot(points_x, points_y, 'ro', 'MarkerSize', 8, 'MarkerFaceColor', 'r');

quiver(ansx(1), ansy(1), cos(normal1), sin(normal1), 1, 'black', 'LineWidth', 2); 
quiver(ansx(2), ansy(2), cos(normal2), sin(normal2), 1, 'black', 'LineWidth', 2); 


% 设置图形参数
xlabel('X');
ylabel('Y');
title('等距螺旋线与数组中的点');
axis equal;  % 保持轴的比例相等
grid on;     % 打开网格

hold off;  % 结束绘制


end
