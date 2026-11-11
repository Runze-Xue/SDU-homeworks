function  [maxi,maxedge,maxgain]=select(att,fea)   
    %选择最好的特征
    %att为特征，fea为标志
    maxi=1;
    [maxgain ,maxedge]=Gain2(att(:,1),fea);
    for i=2:size(att,2)
        [gain,edge] = Gain2(att(:,i),fea);
        if(gain>maxgain)
            maxi=i;
            maxgain=gain;
            maxedge=edge;
        end
    end
 
end

