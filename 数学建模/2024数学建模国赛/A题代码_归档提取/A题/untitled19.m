% 设置螺旋线参数
b = 55 / (2 * pi);  % 每圈的间距为 55
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

% 设置图形参数
xlabel('X');
ylabel('Y');
title('等距螺旋线与数组中的点');
axis equal;  % 保持轴的比例相等
grid on;     % 打开网格

hold off;  % 结束绘制
