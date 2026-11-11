function [res,sym] = opensvm(filename)
  fidin = fopen(filename); % 打开filename文件 
  i = 1;
  apres = [];

while ~feof(fidin)
  tline = fgetl(fidin); % 从文件读行 
  apres{i} = tline;
  i = i+1;
end
  Length = length(apres);
  res = zeros(Length, 59);
  sym = zeros(Length,1);

  for j = 1 : length(apres)
  a = char(apres(j));
  if a(1) == '-' 
      sym(j) = -1;
  else 
      sym(j) = 1;
  end
  lena = size(a);
  lena = lena(2);
  xy = sscanf(a(4:lena), '%d:%d');
    
  lenxy = size(xy);
  lenxy = lenxy(1);
      grid = [];
      grid(784) = 0;
      for i=2:2:lenxy  %% 隔一个数
          if(xy(i)<=0)
              break
          end
        grid(xy(i-1)) = xy(i) * 100/255;
      end
      grid1 = reshape(grid,28,28);
      grid1 = fliplr(diag(ones(28,1)))*grid1;
      grid1 = rot90(grid1,3);
      x = extractLBPFeatures(grid1);
      for k = 1 : length(x)
            res(j, k) = x(k);
      end
  end
end