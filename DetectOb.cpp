#include<iostream>
#include<math.h>
using namespace std;
//障碍物探测

struct Obstacle //障碍物信息
{
    int bianhao;     //障碍物的编号     
    //int time;   //更新障碍物的时间(以10s为一个周期)
    int count;  //记录10s内障碍物出现的次数(是否是10s内有三个障碍物)
    int firsttime;
    int secondtime;
    int thirdtime;
};

struct Sqlist
{
    Obstacle Obs[8];    //这个障碍物数量设置的很小,根据测试需要进行修改
    int sumnum;
};

void Initial(Sqlist *L);    //初始化这个障碍物列表
void InsertData(Sqlist *L,int t,int dx,int dy,int sy,int Time);  //向这个顺序表中增添数据
int Isexist(Sqlist*L,int t);   //判断这个障碍物在之前是不是已经在这个里面
void Update(Sqlist *L,int Time); //空位更新这个数据
bool FindObs(Sqlist * L);
int main()
{
    int N;  //一共返回了多少的信息
    cin>>N;
    int arcs[N+1];  //储存输出结果的数组
    Sqlist *L=new Sqlist;
    Initial(L);
    int tempnum;
    for(int i=1;i<=N;i++)
    {
        int l,t,dx,dy,sx,sy;
        cin>>tempnum;
        if(tempnum==0)
        {
            Update(L,i);    //直接对所有的值进行更新
        }
        else
        {
            for(int j=1;j<=tempnum;j++)
            {
                cin>>l>>t>>dx>>dy>>sx>>sy;
                InsertData(L,t,dx,dy,sy,i);
            }
        }
        if(FindObs(L))
        {
            arcs[i]=1;
        }
        else
        {
            arcs[i]=0;
        }
    }
    for(int i=1;i<=N;i++)
    {
        cout<<arcs[i]<<" ";
    }
    delete L;
    return 0;
}

void Initial(Sqlist *L)    //初始化这个障碍物列表
{
    L->sumnum=0;
    for(int i=0;i<8;i++)    //将障碍物数量进行初始化为0
    {
        L->Obs[i].count=0;
        L->Obs[i].firsttime=-1;
        L->Obs[i].secondtime=-1;
        L->Obs[i].secondtime=-1;
    }
}

void InsertData(Sqlist *L,int t,int dx,int dy,int sy,int Time)  //向这个顺序表中增添数据
{
    long long distance=abs(dx*dx)+abs(dy*dy);   
    long long ndis=5000*5000;
    if(distance>=ndis||abs(sy)<=300)    //当不满足距离小于50或者相对速度大于3m/s则不算障碍物
    {
        Update(L,Time);
        return ;
    }

    int js=Isexist(L,t);
    if(js==-1)  //就之前没有这个值
    {
        L->Obs[L->sumnum].bianhao=t;
        L->Obs[L->sumnum].firsttime=Time;
        L->sumnum++;        
    }
    else    //之前存在这个编号的
    {
        if(L->Obs[js].count==1) //对其值进行更新
        {   
            L->Obs[js].firsttime=L->Obs[js].secondtime;
            L->Obs[js].secondtime=L->Obs[js].thirdtime;
            L->Obs[js].thirdtime=Time;
        }
        else if(L->Obs[js].count==0&&L->Obs[js].secondtime!=-1) //将其存在第三个位置
        {
            L->Obs[js].count=1;
            L->Obs[js].thirdtime=Time;
        }
        else if(L->Obs[js].firsttime!=-1&&L->Obs[js].secondtime==-1) //将其存在第二个位置
        {
            L->Obs[js].secondtime=Time;
        }
    }
}

int Isexist(Sqlist*L,int t)   //判断这个障碍物在之前是不是已经在这个里面
{
    for(int i=0;i<L->sumnum;i++)    //如果有就返回相应的编号
    {
        if(L->Obs[i].bianhao==t)
        {
            return i;
        }
    }
    return (-1);    //如果没得就要重新创建
}

void Update(Sqlist *L,int Time) //空位更新这个数据
{
    for(int i=0;i<L->sumnum;i++)
    {
        if(L->Obs[i].count==1)
        {
            if(Time-L->Obs[i].firsttime>9)  //超过了9就要更新为无障碍物
            {
                L->Obs[i].count=0;
                L->Obs[i].firsttime=L->Obs[i].secondtime;
                L->Obs[i].secondtime=L->Obs[i].thirdtime;
                L->Obs[i].thirdtime=-1;
            }
        }
        else if(L->Obs[i].secondtime!=-1&&L->Obs[i].count==0)   //只有前2个有记录
        {
            if(Time-L->Obs[i].firsttime>9)  //超过了9就要进行相应更新
            {
                L->Obs[i].firsttime=L->Obs[i].secondtime;
                L->Obs[i].secondtime=-1;
            }
        }
        else if(L->Obs[i].firsttime!=-1&&L->Obs[i].secondtime==-1)  //只有第一个位置有记录
        {
            if(Time-L->Obs[i].firsttime>9)  //超过了9就要进行相应更新
            {
                L->Obs[i].firsttime=-1;
            }
        }
    }
}
bool FindObs(Sqlist * L)
{
    for(int i=0;i<L->sumnum;i++)
    {
        if(L->Obs[i].count==1)
        {
            return true;
        }
    }
    return false;
}
