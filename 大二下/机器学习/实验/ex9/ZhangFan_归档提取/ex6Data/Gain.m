function re = Gain(B1,B2)
%计算B分成B1B2的信息熵收益
        B1=B1(:);
        B2=B2(:);
        re= Entropy([B1;B2])- length(B1)/(length(B1)+length(B2))*Entropy(B1)- length(B2)/(length(B1)+length(B2))*Entropy(B1);
end

