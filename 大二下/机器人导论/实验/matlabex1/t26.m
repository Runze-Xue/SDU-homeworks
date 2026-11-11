F = [0.527, -0.574, 0.628, 5; 0.369, 0.819, 0.439, 3; -0.766, 0, 0.643, 8; 0, 0, 0, 1];
T = transl(0, 10, 5);
T2 = T*F;
trplot(F, 'frame','A','color','b');
axis([0,15,0,15,0,15]);
hold on;
tranimate(F, T2,'frame','B','color','r');