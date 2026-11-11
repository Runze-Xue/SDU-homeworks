function re=cut(tr)
   arguments
      tr DTreeNode;
   end
    if (isempty(tr.class))
    
        c1=cut(tr.childno);
        c2=cut(tr.childyes);
        if(c1==c2&&c1~=-1)  
           
            tr.childno=[];
            tr.childyes=[];
            tr.featurenum=[];
            tr.edge=[];
            tr.class=c1;    
            re=c1;
            return;
        end
        re=-1;
        return ;
    else
        re=tr.class;
        return;
    end
end