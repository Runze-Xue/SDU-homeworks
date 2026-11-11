tem = zeros(1, 224);
cuttheta = zeros(1, 224); 
ver = zeros(1, 224);
ver(1) = 100;
for i = 1 : 224
    r = ansnum(i) * 55 + (55/(2*pi)) * anstheta(i);
    b = (55/(2*pi));
    cuttheta(i) = atan((b*sin(anstheta(i)) + r* cos(anstheta(i)))/ (b*cos(anstheta(i)) - r * sin(anstheta(i))));
end

for i = 1 : 223
    dx = ansx(i+1) - ansx(i);
    dy = ansy(i+1) - ansy(i);

    % 计算该向量与x轴的夹角 (弧度)
    alpha = atan2(dy, dx);

    delta = ver(i) * cos(cuttheta(i) - alpha);

    ver(i+1) = delta / cos(cuttheta(i+1) - alpha);
end


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

for i = 1 : 224
    dx = ver(i) * cos(cuttheta(i));
    dy = ver(i) * sin(cuttheta(i));
    quiver(ansx(i), ansy(i), dx,  dy, 1, 'black', 'LineWidth', 2); 
    hold on;
end

% 设置图形参数
xlabel('X');
ylabel('Y');
title('等距螺旋线与数组中的点');
axis equal;  % 保持轴的比例相等
grid on;     % 打开网格

hold off;  % 结束绘制
