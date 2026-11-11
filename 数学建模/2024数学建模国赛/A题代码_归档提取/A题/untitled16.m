% 假设 x 和 y 坐标分别存储在以下矩阵的两行中
coordinates = [ansx ;  % x 坐标
               ansy];  % y 坐标

% 提取 x 和 y 坐标
x = coordinates(1,:);
y = coordinates(2,:);

% 绘制点，'o' 是点的标记
figure;
plot(x, y, 'o');
xlabel('X坐标');
ylabel('Y坐标');
title('点的分布图');
%grid on; % 打开网格以便更好地观察
%axis equal; % 保持轴的比例相等

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
plot(x, y, 'r', 'LineWidth', 2);  % 绘制蓝色的螺旋线
xlabel('X');
ylabel('Y');
title('等距螺旋线');
axis equal;  % 保持比例尺相等
grid on;     % 打开网格
