clear;
clc;
% theta   d   a   alpha  offset
L1 = Link([0, 0,0,-pi/2, pi/2]);
L2 = Link([0, 6, 15, 0, 0]);
L3 = Link([0, 0, 1, -pi/2, 0]);
L4 = Link([0, 18, 0, pi/2, 0]);
L5 = Link([0, 0, 0, -pi/2, 0]);
L6 = Link([0, 0, 0, 0]);
robot = SerialLink([L1 L2 L3 L4 L5 L6],'name', 'man');
robot.plot([0, pi/4, 0, 0, -pi/4, 0]);