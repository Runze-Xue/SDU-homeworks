data=xlsread('ex6Data.csv');
t=DTreeNode;
dgcreat(t,data(:,1:11),data(:,12),[1:11]);
sum=0;
%%
for i =1:length(data)
    
    sum=sum+(adj(t,data(i,1:11))==data(i,12));
end
%%
hold on
global name;
[~,name]=xlsread('ex6Data.csv');
draw(t,100,[0 0])
