
function draw(tr,line,p)
    arguments
    tr DTreeNode
    line
    p
    end
    h=5;
    %[~,name]=xlsread('ex6Data.csv');
    global name;
    if(isempty(tr.class))
        str=char(char(name(tr.featurenum)));
        str=[str,newline,num2str(tr.edge)];
        text(p(1),p(2),str);
        p1(1)=p(1)-line;
        p1(2)=p(2)-h;
        plot([p(1) p1(1)],[p(2) p1(2)],'k');
        draw(tr.childno,line/2,p1);
         p1(1)=p(1)+line;
         plot([p(1) p1(1)],[p(2) p1(2)],'k');
        draw(tr.childyes,line/2,p1);
    else
        if(tr.class==1)
            str='P';
        else
             str='N';
        end
        text(p(1),p(2),str);
     end
   

end