#include<iostream>
#include<vector>
#include<fstream>
#include<string.h>
#include <cstring>
#include<string>
using namespace std;
//南京公交线路图
//输入任意两站点，给出转车次数最少的乘车路线
//输入任意两站点，给出经过站点最少的乘车路线

struct Lnodestation
{
    int Busnumber;  //到达这个站点的公交线路
    int index;  //此时这个站点的编号
    Lnodestation*Nextstation;
};

struct Graph
{
    int Totalstation;   //公交站台的总数量
    vector<Lnodestation> Busroute;  //一条线上所有的站点
    vector<Lnodestation> Vexnode;   //相邻站点的邻接表
    vector<Lnodestation> ZdVexnode; //能够直达的邻接表
    vector<string> StationName; //站台的名称
    vector<vector<int> > Busnums;   //可以经过这个站的公交线路
};

struct BFSCount //BFS遍历时得到的用来记录的值
{
    int index;
    int pre_index;
};

int returnBusnum(string name);  //将字符串转换成数字
int Findappear(Graph G,string name);    //查找以前是否出现过,没有返回-1
bool judgepreN(Graph G,int str1,int str2);    //判断之前是否相邻
bool judgezdpreN(Graph G,int str1,int str2);    //判断之前是否直达
void Shortesrway(Graph G,int str1,int str2);    //最短路径
void Shortesthuancheng(Graph G,int str1,int str2);  //最短换乘车

int main()
{
    Graph G;    //创建这个图     
    G.Totalstation=0;
    fstream file;
    file.open("C:\\Codefield\\CODE_C\\CDesign\\NjBusPath\\Njbus.txt",ios::in);
    if(!file)
    {
        cout<<"文件打开失败";
        exit(0);
    }
    vector<int> tempvec;    //临时容器
    while(1)
    {      
        int Busnum; //这一路车的公交线路
        int flag=0; //当第一个站点时不需要与其他站点相连接
        string num;
        file>>num;
        if(file.eof())
        {
            break;  //读到结尾就跳出循环
        }
        char Kongge=' ';
        Busnum=returnBusnum(num);
        Lnodestation *L=new Lnodestation;
        L->Busnumber=Busnum;    //L是记录这条公交线路的信息
        L->Nextstation=0;
        G.Busroute.push_back(*L);  
        while(Kongge==' ')  //把中间的空格给全部读完
        {
            file.get(Kongge);
        }
        while(1)    //开始读入后面的文字数据
        {
            string tempname;    //记录这个站的名字
            string pretempname; //记录这个站点的前一个站的名字
            while(Kongge!=','&&Kongge!='\n')
            {
                tempname=tempname+Kongge;   //将这个站点名字一个一个读入
                file.get(Kongge);
            }
            int location=Findappear(G,tempname);
            if(location==-1)
            {
                
                G.StationName.push_back(tempname);
                Lnodestation *l1=new Lnodestation;
                l1->Nextstation=NULL;
                l1->index=G.Totalstation;           
                G.Vexnode.push_back(*l1);
                G.ZdVexnode.push_back(*l1);
                G.Busnums.push_back(tempvec);
                G.Busnums[G.Totalstation].push_back(Busnum);
                G.Totalstation++;
                delete l1;
            }
            else
            {
                G.Busnums[location].push_back(location);
            }
            Lnodestation*l2=new Lnodestation;
            l2->index=location;
            l2->Busnumber=Busnum;
            l2->Nextstation=L->Nextstation;
            L->Nextstation=l2;
            L=L->Nextstation;
            delete l2;
            if(flag!=0)
            {
                int bianhao1=Findappear(G,pretempname);
                int bianhao2=Findappear(G,tempname);
                if(judgepreN(G,bianhao1,bianhao2)==0)
                {
                    Lnodestation*l3=new Lnodestation;
                    l3->Busnumber=Busnum;
                    l3->index=bianhao2;
                    l3->Nextstation=G.Vexnode[bianhao1].Nextstation;
                    G.Vexnode[bianhao1].Nextstation=l3;

                    Lnodestation*l4=new Lnodestation;
                    l4->Busnumber=Busnum;
                    l4->index=bianhao1;
                    l4->Nextstation=G.Vexnode[bianhao2].Nextstation;
                    G.Vexnode[bianhao2].Nextstation=l4;
                    delete l3;
                    delete l4;
                }
            }
            flag++;
            pretempname=tempname;
            
            if(Kongge=='\n')
            {
                break;
            }
            else
            {
                file.get(Kongge);
            }            
        }
    }
    file.close();
    for(int i=0;i<G.Busroute.size();i++)
    {
        Lnodestation *l5=G.Busroute[i].Nextstation;
        Lnodestation *l6;
        while(l5->Nextstation)
        {
            l6=l5->Nextstation;
            while(l6)
            {
                if(judgezdpreN(G,l5->index,l6->index)==0)
                {
                    Lnodestation * temp1=new Lnodestation;
                    temp1->index=l6->index;
                    temp1->Nextstation=G.ZdVexnode[l5->index].Nextstation;
                    G.ZdVexnode[l5->index].Nextstation=temp1;

                    Lnodestation * temp2=new Lnodestation;
                    temp2->index=l5->index;
                    temp2->Nextstation=G.ZdVexnode[l6->index].Nextstation;
                    G.ZdVexnode[l6->index].Nextstation=temp2;
                    
                    delete temp1;
                    delete temp2;
                    
                }
                l6=l6->Nextstation;
            }
            l5=l5->Nextstation;
        }
        delete l5;
        delete l6;
    }
    char ch;
    cout<<"           欢迎使用南京公交线路查询系统"<<endl;
    while(1)
    {
        cout<<"请输入起点：";
        string Name1;
        cin>>Name1;
        int judge1;
        judge1=Findappear(G,Name1);
        while(judge1==-1)
        {
        cout<<"输入错误站点,请重输：";
        cin>>Name1;
        judge1=Findappear(G,Name1);
        }
        cout<<"请输入终点：";
        cin>>Name1;
        int judge2;
        judge2=Findappear(G,Name1);
        while(judge2==-1)
        {
            cout<<"输入错误站点,请重输：";
            cin>>Name1;
            judge2=Findappear(G,Name1);
        }
        cout<<"最短换乘路线(中间记录换乘站):";
        Shortesthuancheng(G,judge1,judge2);
        cout<<endl;
        cout<<"最短公交站路线:";
        Shortesrway(G,judge1,judge2);
        cout<<endl;
        cout<<endl;
        cout<<"是否还要继续查找:(y or n)";
        cin>>ch;
        if(ch=='n')
        {
            break;
        }
    }
}

int returnBusnum(string name)  //将字符串转换成数字
{
    int sumnum=0;
    for(int i=0;name[i]>=48&&name[i]<=57;i++)
    {
        sumnum=sumnum+int(name[i])-48;
        sumnum=sumnum*10;
    }
    sumnum=sumnum/10;
    return sumnum;
}


int Findappear(Graph G,string name)    //查找以前是否出现过,没有返回-1
{
    for(int i=0;i<G.Totalstation;i++)
    {
        if(G.StationName[i]==name)
        {
            return i;   //找到返回相应的编号
        }
    }
    return (-1);    //没有找到返回-1
}

bool judgepreN(Graph G,int str1,int str2)    //判断之前是否相邻
{
    Lnodestation *p=G.Vexnode[str1].Nextstation;
    while(p)
    {
        if(p->index==str2)
        {
            return 1;
        }
        p=p->Nextstation;
    }
    return 0;
}
bool judgezdpreN(Graph G,int str1,int str2)    //判断之前是否直达
{
    Lnodestation* p;
    p = G.Vexnode[str1].Nextstation;
    while (p)
    {
        if (p->index == str2)
            return 1;
        p = p->Nextstation;
    }
    return 0;
}

void Shortesrway(Graph G,int str1,int str2)
{
    if(str1==str2)
    {
        cout<<"你此时的起点和终点一致";
        return;
    }
    vector<BFSCount> lnode;
    BFSCount l1,l2;
    Lnodestation *p;
    int base=0,flag=0;
    int vex[10007]={0};
    BFSCount head;
    head.pre_index=-1;
    head.index=str1;
    lnode.push_back(head);
    while(base!=lnode.size()&&!flag)
    {
        l1=lnode[base];
        base++;
        p=G.Vexnode[l1.index].Nextstation;
        flag=0;
        while(p&&!flag)
        {
            if(!vex[p->index])
            {
                l2.pre_index=base-1;
                l2.index=p->index;
                if(p->index==str2)
                {
                    flag=1;
                }
                lnode.push_back(l2);
            }
            p=p->Nextstation;
        }
    }
    if(base==lnode.size())
    {
        cout<<"未找到合适路径"<<endl;
        return;
    }
    else
    {
        vector<int>pr;
        vector<int>::iterator i;
        while(l2.pre_index!=-1)
        {
            pr.push_back(l2.index);
            l2=lnode[l2.pre_index];
        }
        pr.push_back(l2.index);
        i=pr.end()-1;
        cout<<"起点："<<G.StationName[pr.back()];
        for(int j=1;j<pr.size();j++)
        {
            cout<<"->"<<G.StationName[*(i-j)];
            if((j-1)%8==0&&j!=1)
            {
                cout<<endl;
            }
        }
        cout<<endl;
    }
}

void Shortesthuancheng(Graph G,int str1,int str2)   //最短换乘
{
    if(str1==str2)
    {
        cout<<"你此时的起点和终点一致";
        return;
    }
    vector<BFSCount> lnode;
    BFSCount l1,l2;
    Lnodestation *p;
    int base=0,flag=0;
    int vex[10007]={0};
    BFSCount head;
    head.pre_index=-1;
    head.index=str1;
    lnode.push_back(head);
    while(base!=lnode.size()&&!flag)
    {
        l1=lnode[base];
        base++;
        p=G.ZdVexnode[l1.index].Nextstation;
        flag=0;
        while(p&&!flag)
        {
            if(!vex[p->index])
            {
                l2.pre_index=base-1;
                l2.index=p->index;
                if(p->index==str2)
                {
                    flag=1;
                }
                lnode.push_back(l2);
            }
            p=p->Nextstation;
        }
    }
    if(base==lnode.size())
    {
        cout<<"未找到合适路径"<<endl;
        return;
    }
    else
    {
        vector<int>pr;
        vector<int>::iterator i;
        while(l2.pre_index!=-1)
        {
            pr.push_back(l2.index);
            l2=lnode[l2.pre_index];
        }
        pr.push_back(l2.index);
        i=pr.end()-1;
        cout<<"起点："<<G.StationName[pr.back()];
        for(int j=1;j<pr.size();j++)
        {
            cout<<"->"<<G.StationName[*(i-j)];
            if((j-1)%8==0&&j!=1)
            {
                cout<<endl;
            }
        }
        cout<<endl;
    }
}
