#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;
//利用递归和回溯进行迷宫路线的寻找
//二维数组中0为可以走的路,1为墙挡住的路
//通过输出线路坐标来表示路线
struct stack
{
    int number[1280];  
    int total;  //初始值为-1,总的数量为total+1
};
void Initstack(stack*L);    //初始化这个栈
int top(stack*L);   //显示栈顶元素
void pop(stack*L);  //弹栈
void push(stack*L,int number);  //压栈
void Findpath(char **p,stack *hangL,stack* lieL,int s1,int s2,int egderow,int egdecol,int &js);  //递归回溯找出全部路线
void Printsum(stack*hangL,stack*lieL);  //将线路打印出来

int main()
{
    char **arc;
    arc=new char*[64];   //将这个迷宫从文件之中移出来
    for(int i=0;i<64;i++)
    {
        arc[i]=new char[64];
    }
    fstream file;
    file.open("C:\\Codefield\\CODE_C\\CDesign\\Maze\\Maze.txt",ios::in);    //读文件
    if(!file)
    {
        cout<<"文件读取失败";
        exit(0);
    }
    int js=0;    
    while(!file.eof())
    {
        file.getline(arc[js],12);   //将二维迷宫一行一行的读入到整个二维数组中
        js++;
    }
    file.close();
    int col;    //迷宫列
    int row;    //迷宫行
    row=js; //行就是有几个指针读取了数据,最下面的一行是row-1
    col=strlen(arc[0]); //列就是一个指针中字符串长度,所以最后是最右边的一列是col-1
    stack * rowstack=new stack;
    stack * colstack=new stack;
    Initstack(rowstack);    //将这两个栈进行初始化
    Initstack(colstack);
    cout<<"迷宫图形："<<endl;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            cout<<arc[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<endl;
    int start1,start2,end1,end2;
    cout<<"请输入起点的行和列：";
    cin>>start1>>start2;
    cout<<"请输入终点的行和列：";
    cin>>end1>>end2;
    push(rowstack,end1);  //将终点元素进行压栈处理
    push(colstack,end2);
    int count=0;
    if(arc[end1][end2]=='1'||arc[start1][start2]=='1')
    {
        cout<<"没有路线能到达。";
    }
    else
    {
        Findpath(arc,rowstack,colstack,start1,start2,row-1,col-1,count);
        cout<<endl;
        cout<<"一共有："<<count<<"条路线。";
    }  
    for(int i=0;i<64;i++)   //释放内存所占的空间
    {
        delete []arc[i];
    }
    delete arc;
    delete rowstack;
    delete colstack;
    
    return 0;
}

void Initstack(stack*L)    //初始化这个栈
{
    L->total=-1; 
}
int top(stack*L)   //显示栈顶元素
{
    return L->number[L->total]; //直接返回栈顶元素
}
void pop(stack*L)  //弹栈
{
    L->total--; //直接减1即为弹栈
}
void push(stack*L,int number)  //压栈   
{
    L->total++;
    L->number[L->total]=number;
}

void Findpath(char **p,stack *hangL,stack* lieL,int s1,int s2,int egderow,int egdecol,int &js)  //递归回溯找出全部路线
{
    int heng,shu;
    if(hangL->number[hangL->total]==s1&&lieL->number[lieL->total]==s2)
    {   //找到一个值就需要进行输出并且弹栈回溯
        Printsum(hangL,lieL);   //输出其中的一条线路即可,每条路线都输出使得有些线路显得很冗余       
        pop(hangL);
        pop(lieL);
        js++;
        return;
    }
    //这个点进行向上递归
    if(hangL->number[hangL->total]!=0)  //检查是否是在上边界
    {   int judge=0;     
        heng=hangL->number[hangL->total]-1; //此时这两个点具体的位置
        shu=lieL->number[lieL->total];
        if(p[heng][shu]=='0')   //检查是否有路可以行走
        {
            for(int i=0;i<=hangL->total;i++)    //确保这个路线是简单路线,并不是重复一些点的路线
            {
                if(hangL->number[i]==heng&&lieL->number[i]==shu)
                {
                    judge=1;
                    break;
                }
            }
            if(judge==0)
            {
                push(hangL,heng);  //将最新坐标进行压栈处理
                push(lieL,shu);
                Findpath(p,hangL,lieL,s1,s2,egderow,egdecol,js); 
            }
        }        
    }
    //这个点进行向下递归
    if(hangL->number[hangL->total]!=egderow)  //检查是否是在下边界
    { 
        int judge=0;       
        heng=hangL->number[hangL->total]+1; //此时这两个点具体的位置
        shu=lieL->number[lieL->total];
        if(p[heng][shu]=='0')   //检查是否有路可以行走
        {
            for(int i=0;i<=hangL->total;i++)    //确保这个路线是简单路线,并不是重复一些点的路线
            {
                if(hangL->number[i]==heng&&lieL->number[i]==shu)
                {
                    judge=1;
                    break;
                }
            }
            if(judge==0)
            {
                push(hangL,heng);  //将最新坐标进行压栈处理
                push(lieL,shu);
                Findpath(p,hangL,lieL,s1,s2,egderow,egdecol,js); 
            }
        }        
    }
    //这个点进行向右递归
    if(lieL->number[lieL->total]!=egdecol)  //检查是否是在右边界
    {    
        int judge=0;    
        heng=hangL->number[hangL->total]; //此时这两个点具体的位置
        shu=lieL->number[lieL->total]+1;
        if(p[heng][shu]=='0')   //检查是否有路可以行走
        {
            for(int i=0;i<=hangL->total;i++)    //确保这个路线是简单路线,并不是重复一些点的路线
            {
                if(hangL->number[i]==heng&&lieL->number[i]==shu)
                {
                    judge=1;
                    break;
                }
            }
            if(judge==0)
            {
                push(hangL,heng);  //将最新坐标进行压栈处理
                push(lieL,shu);
                Findpath(p,hangL,lieL,s1,s2,egderow,egdecol,js); 
            }
        }        
    }
    //这个点进行向左递归
    if(lieL->number[lieL->total]!=0)  //检查是否是在左边界
    {       
        int judge=0; 
        heng=hangL->number[hangL->total]; //此时这两个点具体的位置
        shu=lieL->number[lieL->total]-1;
        if(p[heng][shu]=='0')   //检查是否有路可以行走
        {
            for(int i=0;i<=hangL->total;i++)    //确保这个路线是简单路线,并不是重复一些点的路线
            {
                if(hangL->number[i]==heng&&lieL->number[i]==shu)
                {
                    judge=1;
                    break;
                }
            }
            if(judge==0)
            {
                push(hangL,heng);  //将最新坐标进行压栈处理
                push(lieL,shu);
                Findpath(p,hangL,lieL,s1,s2,egderow,egdecol,js); 
            }
        }        
    }    
    pop(hangL); //一次递归结束需要对栈进行弹栈操作!!!
    pop(lieL);  //目的是进行回溯
}

void Printsum(stack*hangL,stack*lieL)  //将线路打印出来
{
    cout<<"路线为：";
    for(int i=hangL->total;i>=0;i--)    //将迷宫路线打印出来
    {
        if(i==0)
            cout<<"("<<hangL->number[i]<<","<<lieL->number[i]<<")"<<endl;       
        else
            cout<<"("<<hangL->number[i]<<","<<lieL->number[i]<<")->";
        
    }
}
