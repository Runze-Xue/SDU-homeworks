function re =EdgeOfAttribute (A,B)
L=length(A);
for i=1:length(A)
    for j=1:length(A)-i
        if(A(j)>A(j+1))
            mid=A(j);
            A(j)=A(j+1);
            A(j+1)=mid;
            mid=B(j);
            B(j)=B(j+1);
            B(j+1)=mid;
        end
    end
end
edge=A(1)-(A(2)-A(1))/2;
minE=Entropy(B(1:end));
% showdata(1,1)=edge;
% showdata(1,2)=minE;
for i=1:L-1
%     showdata(i+1,1)=(A(i)+A(i+1))/2;
%     showdata(i+1,2)=i/L*Entropy(B(1:i))+(L-i)/L*Entropy(B(i+1:end));
    if B(i)~=B(i+1)
        E=i/L*Entropy(B(1:i))+(L-i)/L*Entropy(B(i+1:end));
        if(E<minE )
            minE=E;
            edge=(A(i)+A(i+1))/2;
        end
        
    end
end
% showdata(L+1,1)=A(L)+(A(L)-A(L-1))/2;
% showdata(L+1,2)=Entropy(B(1:end));
if(Entropy(B(1:end))<minE )
    minE=Entropy(B(1:end));
    edge=A(L)+(A(L)-A(L-1))/2;
end

%showdata
re=edge;
end