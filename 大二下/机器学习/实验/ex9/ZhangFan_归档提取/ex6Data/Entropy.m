function re = Entropy(A)
%计算信息熵
    C1=sum(A==0);
    C2=sum(A==1);
    re=0;
    if(C1~=0)
    re=re-C1/(C1+C2)*log(C1/(C1+C2));
    end
    if(C2~=0)
    re=re-C2/(C1+C2)*log(C2/(C1+C2));
    end
    re=re/log(2);
end

