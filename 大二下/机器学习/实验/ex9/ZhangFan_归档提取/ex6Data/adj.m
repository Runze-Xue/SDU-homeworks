function re = adj(tr,x)
    %使用树判断属性
    arguments
        tr DTreeNode
        x 
    end
   
    while(isempty(tr.class))
        
        if(x(tr.featurenum)<tr.edge)
            tr=tr.childno;
        else
            tr=tr.childyes;
        end
        
        
    end
    re=tr.class;
end