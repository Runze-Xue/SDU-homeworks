#include<bits/stdc++.h>
using namespace std;
#define N 300
#define ld long double
ld pi=acosl(-1.0L);
ld eps=1e-15L;
ld interval = 55L;
struct xyPoint{
    ld x,y;
    xyPoint operator-(const xyPoint& rhs){
        return {x - rhs.x, y - rhs.y};
    }
};
struct cirPoint{
    ld rho,theta;
};
struct numPoint{
    int number;ld theta;
};
inline ld rtrho(const numPoint&s){
    return interval*s.number+(interval/(2.0L*pi))*s.theta;
}
inline xyPoint arc_vertical(const cirPoint&s){
//坐标转换
    xyPoint res;
    res.x=s.rho*cosl(s.theta);
    res.y=s.rho*sinl(s.theta);
    return res;
}
inline xyPoint getvertical(const numPoint&s){
//坐标转换
    xyPoint res;
    ld rho=rtrho(s);
    res.x=rho*cosl(s.theta);
    res.y=rho*sinl(s.theta);
    return res;
}
inline ld arclength(const int&number,const ld&theta){
//计算螺旋线弧长
    ld a=interval/(2.0L*pi);
    ld theta1=2.0L*pi*number+theta;
    ld theta2=2.0L*pi*number;
    ld s1=(a/2.0L)*(theta1*sqrtl(theta1*theta1+1)+logl(theta1+sqrtl(theta1*theta1+1)));
    ld s2=(a/2.0L)*(theta2*sqrtl(theta2*theta2+1)+logl(theta2+sqrtl(theta2*theta2+1)));
    return s1-s2;
}
inline ld getwholearclength(const int&number){
//一圈的弧长
    return arclength(number,2.0L*pi);
}
inline ld get2pdistance(const cirPoint&t1,const cirPoint&t2){
//计算直线距离
    return sqrtl(t1.rho*t1.rho+t2.rho*t2.rho-2.0L*t1.rho*t2.rho*cosl(t1.theta-t2.theta));
}
inline ld getdis(const numPoint&t1,const numPoint&t2){
//计算直线距离
    ld rho1=rtrho(t1);
    ld rho2=rtrho(t2);
    return sqrtl(rho1*rho1+rho2*rho2-2.0L*rho1*rho2*cosl(t1.theta-t2.theta));
}
inline numPoint gethead(const ld&cnttime){
//求头的位置
    int nownumber=16;
    ld nowtheta=0.0L;
    ld lstarclength=cnttime*100.0L;
    while(lstarclength>0){
        nownumber--;
        lstarclength-=getwholearclength(nownumber);
    }
    ld l=0.0L,r=2.0L*pi;
    while(r-l>eps){
        ld mid=(l+r)/2.0L;
        if(arclength(nownumber,mid)+lstarclength>=0)r=mid;
        else l=mid;
    }
    return (numPoint){nownumber,(l+r)/2.0L};
}
inline numPoint getnext(const numPoint&nowx,const ld&changetheta){
//沿螺旋线转多少度
    numPoint res;
    res.number=nowx.number;
    res.theta=nowx.theta+changetheta;
    if(res.theta>=2.0L*pi){
        res.theta-=2.0L*pi;
        res.number++;
    }
    return res;
}

inline numPoint getnexloc(const numPoint&nows,const ld&distance){
//求下一个点位置
    ld l=0.0L,r=pi;
    while(r-l>eps){
        ld mid=(l+r)/2.0L;
        numPoint tmp=getnext(nows,mid);
        if(getdis(nows,tmp)>=distance)r=mid;
        else l=mid;
    }
    return getnext(nows,(l+r)/2.0L);
}

numPoint finalPoint[N];
inline void work(const ld&cnttime){
    numPoint nows=gethead(cnttime);
    finalPoint[0]=nows;
    ld d1=341.0L - 2.0L * 27.5L,d2=220.0L - 2.0L * 27.5L;
    nows=getnexloc(nows,d1);
    finalPoint[1]=nows;
    for(int i=2;i<=223;i++){
        nows=getnexloc(nows,d2);
        finalPoint[i]=nows;
    }
    return;
}
struct Rectangle{
    xyPoint a,b,c,d;
};
Rectangle finalRectangle[N];
inline Rectangle getRectangle(const xyPoint&p1,const xyPoint&p2,const ld&dlen){
    Rectangle res;
    ld deltax=(p2.x-p1.x)/dlen;
    ld deltay=(p2.y-p1.y)/dlen;
    res.a.x=p1.x-deltax*27.5L-deltay*15.0L;
    res.a.y=p1.y-deltay*27.5L+deltax*15.0L;
    res.b.x=p1.x-deltax*27.5L+deltay*15.0L;
    res.b.y=p1.y-deltay*27.5L-deltax*15.0L;
    res.c.x=p2.x+deltax*27.5L+deltay*15.0L;
    res.c.y=p2.y+deltay*27.5L-deltax*15.0L;
    res.d.x=p2.x+deltax*27.5L-deltay*15.0L;
    res.d.y=p2.y+deltay*27.5L+deltax*15.0L;
    return res;
}   
inline Rectangle getRectangleBody(const xyPoint&p1,const xyPoint&p2){
    return getRectangle(p1,p2,165.0L);
}
inline Rectangle getRectangleHead(const xyPoint&p1,const xyPoint&p2){
    return getRectangle(p1,p2,286.0L);
}

// step 2

ld cross(xyPoint a, xyPoint b){
    return a.x * b.y - a.y * b.x;
}
bool intersection(xyPoint a, xyPoint b, xyPoint c, xyPoint d)
{
    if(max(c.x,d.x)<min(a.x,b.x)||max(a.x,b.x)<min(c.x,d.x)||max(c.y,d.y)<min(a.y,b.y)||max(a.y,b.y)<min(c.y,d.y)){
        return false;
    }
    if(cross(a-d,c-d)*cross(b-d,c-d)>0||cross(d-b,a-b)*cross(c-b,a-b)>0){
        return false;
    }
    return true;
}

int main(){
    work(412.473837);
    finalRectangle[1]=getRectangleHead(getvertical(finalPoint[0]),getvertical(finalPoint[1]));
    for(int i=2;i<=223;i++)
        finalRectangle[i]=getRectangleBody(getvertical(finalPoint[i-1]),getvertical(finalPoint[i]));
    FILE*f=fopen("1.csv","w");
    for(int i=1;i<=223;i++)
        std::fprintf(f, "%.20Lf, %.20Lf, %.20Lf, %.20Lf, %.20Lf, %.20Lf, %.20Lf, %.20Lf,\n", 
            finalRectangle[i].a.x, finalRectangle[i].a.y, 
            finalRectangle[i].b.x, finalRectangle[i].b.y,
            finalRectangle[i].c.x, finalRectangle[i].c.y,
            finalRectangle[i].d.x, finalRectangle[i].d.y);
    
}




