% L1 = Link([0, 0.2, 1, pi/4, 0],'modified');
% L2 = Link([0, 0.2, 1, pi/4, 0],'modified');
% L3 = Link([0, 0, 0.5, pi/4, 0],'modified');
% robot = SerialLink([L1,L2,L3]);
% % robot = SerialLink([L1,L2]);
% axis([-10 10 -10 10 -10 10]);
% robot.plot([0,0,0]);

L1 = Link([0, 0.2, 1, pi/4, 0],'standard');
L2 = Link([0, 0.2, 1, pi/4, 0],'standard');
L3 = Link([0, 0, 0.5, pi/4, 0],'standard');
robot = SerialLink([L1,L2,L3]);
% robot = SerialLink([L1,L2]);
axis([-15 15 -15 15 -15 15]);
robot.plot([0,0,0]);

% L1 = Link([pi/4, 0, 0, 0, 0],'modified');
% L2 = Link([pi/2, 0.2, 0.1, 0, 0],'modified');
% L3 = Link([0, 0.1, 0.2, 0, 0],'modified');
% robot = SerialLink([L1,L2,L3]);
% robot.display();
% theta = [0 0 0];
% robot.plot(theta);

