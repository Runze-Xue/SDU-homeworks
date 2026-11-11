function  maxi=dgcreat( tr,att,fea,feaset)
    %建树
    %att为特征，fea为标记，feaset为对应分量的特征名。
    arguments
    tr  DTreeNode
    att
    fea
    feaset
    end
    if(sum(fea==1)==length(fea))
       tr.class=1;
       return;
    elseif(sum(fea==1)==0)
       tr.class=0;
       return;
    end
     
    if(isempty(att))
        if(sum(fea==1)>length(fea)/2)
            tr.class=1;
            return;
        else
            tr.class=0;
            return;
        end
    end
    [i,edge,gain]=select(att,fea);
    %前剪枝
    if(gain<0.01*Entropy(fea))
         if(sum(fea==1)>length(fea)/2)
            tr.class=1;
            return;
        else
            tr.class=0;
            return;
        end
    end
    tr.featurenum=feaset(i);
    tr.edge=edge;
    feaset(i)=[];
    fea1=zeros(size(fea));
    fea2=zeros(size(fea));
    att1=zeros(size(att));
    att2=zeros(size(att));
    c1=0;
    c2=0;
    for j=1:length(fea)
        if(att(j,i)<edge)
        c1=c1+1;
        att1(c1,:)=att(j,:);
        fea1(c1)=fea(j);
        else
            c2=c2+1;
            att2(c2,:)=att(j,:);
            fea2(c2)=fea(j);
        end
    end
    att1=att1(1:c1,:);
    fea1=fea1(1:c1);
    att2=att2(1:c2,:);
    fea2=fea2(1:c2);
    att1(:,i)=[];
    att2(:,i)=[];
    tr.childno=DTreeNode;
    tr.childyes=DTreeNode;
    dgcreat( tr.childno,att1,fea1,feaset);
    dgcreat( tr.childyes,att2,fea2,feaset);
end