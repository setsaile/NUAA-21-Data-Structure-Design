#include<iostream>
using namespace std;
//地铁站修建
//算法思想：找出全部简单路径,统计出所有简单路径的耗时,将他们进行比较,值最小的为最短修建时间。
struct Arcnode  //储存修建时间的结构体
{
    int endstation;
    int daytime;
    Arcnode *next;
};
struct Vexnode
{
    int startstation;
    Arcnode *first;
};
struct Graph
{
    Vexnode * vex;
    int vexnum,arcnum;  //记录顶点数和边数
};
void Desory(Graph *G,int x);  //释放空间
void FindMintime(Graph *G,int sta,int over,int d,int *path,int &time,int *vist); //找到这个的最短修建时间

int main()
{
    int n,m;
    cin>>n>>m;
    Graph *G=new Graph;
    G->vexnum=n;    //枢纽数量等于顶点个数
    G->arcnum=m;    //隧道数量等于边数
    G->vex=new Vexnode[n+1];
    for(int i=1;i<=n;i++)   //对这条边进行相应的初始化
    {
        G->vex[i].first=NULL;
    }
    int start,end,daytime;
    Arcnode *p;
    for(int i=0;i<m;i++)    //创建图的模样
    {
        cin>>start>>end>>daytime;
        
        p=new Arcnode;
        p->endstation=end;
        p->daytime=daytime;

        p->next=G->vex[start].first;    //将p存入到这个邻接表中去
        G->vex[start].first=p;
    }
    int Time=1000;
    int Path[128];
    int visit[n+1];
    for(int i=1;i<=n;i++)   //visit是辅助数组表示还没被访问过
    {
        visit[i]=0;
    }
    FindMintime(G,1,n,0,Path,Time,visit);
    cout<<Time;
    Desory(G,n);    //释放相应的空间
    return 0;
}

void Desory(Graph *G,int x)  //释放空间
{
    Arcnode*p;
    Arcnode*q;
    for(int i=1;i<=G->vexnum;i++)   //从最下层删除到最上层才能保证空间被释放完成
    {
        p=G->vex[i].first;   //p赋值为邻接表的第一个节点
        //可能第一个节点就是空指针,就不能进行下一步,否则会异常数据,终止终端!!!
        if(p==NULL) //如果是孤立点则不用删除,直接跳过这一个循环!!!
        {
            continue;
        }
        while(p->next)  //将邻接表的节点空间一一释放
        {
            q=p->next;   //q为p的下一个节点空间
            delete p;   //将p这个空间删除
            p=q;    //p又等于下一个空间
        }
        delete p;   //单独提出来删除最后一个空间
    } 
    delete G->vex;
    delete G;   //最后再删除这个大空间
}

//利用寻找全部简单路径来求最小施工天数
void FindMintime(Graph *G,int sta,int over,int d,int *path,int &time,int *vist) //找到这个的最短修建时间
{
    Arcnode *p;
    path[d]=sta;    //将当前位置放进去
    vist[sta]=1;
    if(sta==over)
    {
        int linshitime=0;
        Arcnode *p2;
        for(int i=0;i<=d;i++)
        {
            p2=G->vex[path[i]].first;
            while(p2)
            {
                if(p2->endstation==path[i+1])   //当这个点与下一个点相同时则找到正确的时间
                {
                    if(linshitime<p2->daytime)
                    {
                        linshitime=p2->daytime; //找到最长时间,即为这条路的工期
                        break;
                    }
                }
                p2=p2->next;
            }           
        }
        if(time>linshitime) //将最小工期存在time中
        {
            time=linshitime;
        }
    }
    else
    {
        p=G->vex[sta].first;
        while(p)
        {
            if(vist[p->endstation]==0)  //这个点还没被访问
            {
                FindMintime(G,p->endstation,over,d+1,path,time,vist);
                vist[p->endstation]=0;  //恢复访问标记,因为已经移动到下一个起点了
            }
            p=p->next;
        }
    }
}
