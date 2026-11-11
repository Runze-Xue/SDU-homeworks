function [re re2] = Gain2(att,fea)
edge=EdgeOfAttribute(att,fea);
B1=[];
B2=[];
for i=1:length(att)
    if(att(i)<edge)
    B1=[B1 fea(i)];
    else
    B2=[B2 fea(i)];    
    end
end
re2=edge;
re=Gain(B1,B2);
end

