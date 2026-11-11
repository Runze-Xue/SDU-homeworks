mdl_puma560;
p560=SerialLink(p560);
% q = [0 -pi/4 -pi/4 0 pi/8 0];
q = [0 pi/5 -pi/3 pi/2 pi/9 -pi/2]
T = fkine(p560, q);
qi = ikine(p560, T)

