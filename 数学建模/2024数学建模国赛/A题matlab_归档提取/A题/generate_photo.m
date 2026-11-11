% 假设 x 和 y 坐标分别存储在以下矩阵的两行中
coordinates = [ansx ;  % x 坐标
               ansy];  % y 坐标

% 提取 x 和 y 坐标
x = coordinates(1,:);
y = coordinates(2,:);

% 绘制点，'o' 是点的标记
figure;
plot(x, y, 'ob');
xlabel('X坐标');
ylabel('Y坐标');
title('点的分布图');
grid on; % 打开网格以便更好地观察
axis equal; % 保持轴的比例相等

hold on;

% 参数设置
a = 0;    % 控制初始半径
b = 55 / (2*pi);    % 控制螺旋增长速度
theta_max = 32 * pi;  % 设置角度范围的最大值
theta = linspace(0, theta_max, 1000);  % 生成角度的范围

% 极坐标方程：r = a + b*theta
r = a + b * theta;

% 将极坐标转换为笛卡尔坐标
x = r .* cos(theta);  % x = r * cos(theta)
y = r .* sin(theta);  % y = r * sin(theta)

% 绘制等距螺旋线
plot(x, y, 'r', 'LineWidth', 1);  % 绘制蓝色的螺旋线

hold on;

data = csvread('2.csv');
for i = 1 : 223
    plot([data(i, 1), data(i, 3)], [data(i, 2), data(i, 4)], 'b-', 'LineWidth', 1);  % 蓝色线条，宽度为2
    plot([data(i, 3), data(i, 5)], [data(i, 4), data(i, 6)], 'b-', 'LineWidth', 1);
    plot([data(i, 5), data(i, 7)], [data(i, 6), data(i, 8)], 'b-', 'LineWidth', 1);
    plot([data(i, 7), data(i, 1)], [data(i, 8), data(i, 2)], 'b-', 'LineWidth', 1);
    hold on;
end

% 绘制两个端点，红色圆圈标记

