#include<iostream>
#include<string.h>
#include<stdbool.h>
#include<fstream>
using namespace std;
//区块链
//节点中的信息始终都以\0来表示结束,无论是从屏幕输入还是从文件输入
//向其中添加信息且,始终保证这个值是满足要求的
struct LNode
{
    int Idnum;  //这个数据的编号
    char * information; //数据信息
    int Checknum;   //校验码
    LNode * next;
};

struct LInklist
{
    LNode * head;
    int sumnums;
};

void InitLink(LInklist *L); //初始化这个链表
void InsertData(LInklist *L,char *data);    //向这个链表存入信息
bool judgeYes(LInklist*L);  //判断这个链表是否有效
void EditLink(LInklist *L,int num); //串改特定编号信息
void PrintLink(LInklist *L);    //将这个表中的内容全部打印出来
void DeleteLink(LInklist * L);  //释放空间
void strcpylist(LInklist *L,int id,char *a,int cnum);   //将文件中的直接存进去
int main()
{
    fstream file;
    file.open("C:\\Codefield\\CODE_C\\CDesign\\Blockchain\\Blockr.txt",ios::in);
    if(!file)   //检查是否打开文件cc
    {
        cout<<"打开文件失败。";
        exit(0);
    } 
    int id; //编号
    int cnum;   //校验值
    char * p=new char[128];   //信息
    LInklist * list=new LInklist;
    InitLink(list);
    while(!file.eof())  //判断光标是否到达了尾部,.eof是代表结尾
    {
        file>>id;
        file>>p;
        file>>cnum;
        if(file.fail()) //判断是否读取失败
        {
            break;
        }
        strcpylist(list,id,p,cnum); //从文件中原汁原味的输入数据
    }
    file.close();

    int choice;
    while(1)
    {
        cout<<"\t\t区块链"<<endl;
        cout<<"1.展示链表内容"<<endl;
        cout<<"2.检验区块链是否有效"<<endl;
        cout<<"3.修改某个编号内容"<<endl;
        cout<<"4.添加新信息"<<endl;
        cout<<"5.将链表最新内容存到文件中,并且退出程序"<<endl;
        cout<<endl;
        cout<<"选择：";
        cin>>choice;
        switch (choice)
        {
            case 1:
                PrintLink(list);
                break;
            case 2:
                if(judgeYes(list))
                {
                    cout<<"链表有效。"<<endl;
                    cout<<endl;
                }
                break;
            case 3:
                cout<<"你想要修改的数据的编号：";
                int da;
                cin>>da;
                EditLink(list,da);
                break;
            case 4:
                cout<<"新信息内容(包含结束符号)：";
                cin>>p;
                InsertData(list,p); 
                break;
            case 5:
                file.open("C:\\Codefield\\CODE_C\\CDesign\\Blockchain\\Blockw.txt",ios::out);
                LNode * lp=list->head;
                while(lp)   //将修改之后的内容重新存入文件中去
                {
                    file<<lp->Idnum<<" "<<lp->information<<" "<<lp->Checknum<<endl;
                    lp=lp->next;
                }
                file.close();   //关闭文件
                DeleteLink(list);   //释放链表的空间
                exit(0);
        }
    }
}



void InitLink(LInklist *L) //初始化这个链表
{
    L->sumnums=0;
    L->head=0;
}

void InsertData(LInklist *L,char *data)    //向这个链表存入数据
{
    int length=0;   //这个信息的长度
    int sum=0;
    for(int i=0;;i++)   //测量这个信息的长度
    {
        if(data[i]==92&&data[i+1]==48)
        {
            length=i;   
            break;
        }
    }
    if(L->sumnums==0)
    {
        
        LNode *p=new LNode;
        p->Idnum=L->sumnums;    //编号直接是链表长度
        p->next=0;
        p->information=new char[length+1];  //创建空间
        strcpy(p->information,data);
        p->information[length]='\0';    //将最后一位以\0来结束
        //校验值直接靠人工来算
        for(int i=0;i<length;i++)
        {
            sum=sum+int(p->information[i]); //将这个信息转化成为数字
        }
        p->Checknum=sum%113;    //存储校验码
        L->head=p;  //将头指针赋值成为这个
        L->sumnums++;
        return; 
    }
    LNode *p=new LNode;
    p->Idnum=L->sumnums;    //编号直接是长度
    p->next=NULL;
    p->information=new char[length+1];
    strcpy(p->information,data);    //信息
    p->information[length]='\0';    //得将最后一位用\0表示结束
    for(int i=0;i<length;i++)
    {
        sum=sum+int(p->information[i]);
    }
    LNode * linshi=L->head;
    while(linshi->next) //找到最后一个指针节点
    {
        linshi=linshi->next;
    }
    p->Checknum=(linshi->Checknum+p->Idnum+sum)%113;    //校验码的程序计算
    linshi->next=p; //下一个编号就是p
    linshi=p;
    L->sumnums++;   //编号照样加1
    return;
}

bool judgeYes(LInklist*L) //判断这个链表是否有效
{
    LNode * p=L->head;
    int length;
    int sum=0;
    length=strlen(p->information);
    //检查第一个节点
    for(int i=0;i<length;i++)
    {
        sum=sum+p->information[i];
    }
    sum=sum%113;
    if(p->Checknum!=sum)
    {
        cout<<"无效节点："<<p->Idnum<<endl;
        cout<<endl;
        return false;
    }
    //检查之后的节点
    int old=p->Checknum;
    p=p->next;
    while(p)
    {
        int sum=0;
        length=strlen(p->information);
        for(int i=0;i<length;i++)
        {
            sum=sum+p->information[i];
        }
        sum=(sum+old+p->Idnum)%113;
        if(sum!=p->Checknum)
        {
            cout<<"无效节点："<<p->Idnum<<endl;
            cout<<endl;
            return false;
        }
        old=p->Checknum;    //将这个校验值保存下来方便下一个继续使用
        p=p->next;  //需要将链表向下进行移动
    }
    return true;    //全部检查完了,就是有效的
}

void EditLink(LInklist *L,int num) //篡改特定编号信息
{
    int sum=0;
    int js=0;   
    LNode *p=L->head;
    LNode *pa;
    if(num!=0)  //不是首个字母时,才能找它的前一个位置,否则pa是乱码
    {
        while(p)    //将这个特定点找出来
        {
            if(js==num-1)   //将前一个节点找出来;
            {
                pa=p;
            }
            if(js==num)
            {
                break;
            }
            p=p->next;
            js++;
        }
    }
    int a;
    cout<<"新信息的长度(包含结束标志)：";
    cin>>a;
    char * chuan=new char[a+1];
    cout<<"新信息(含结束标志)：";
    for(int i=0;i<a;i++)
    {
        cin>>chuan[i];
        sum=sum+int(chuan[i]);  //将这个的信息值记录下来
    }
    sum=sum-92-48;  //需要将最后的\0所占的ascall码进行减去
    chuan[a-2]='\0';  //将尾部结束

    strcpy(p->information,chuan);   //改变了信息,就需要改变相应的校验值
    int older=p->Checknum;    //将这个的校验值记录
    if(num==0)
    {
        p->Checknum=sum%113;    //当修改位置在首位置时
    }
    else    //不在首个位置修改
    {
        p->Checknum=(pa->Checknum+p->Idnum+sum)%113;    //重新更新校验值
    }
    int chazhi=p->Checknum-older;   //修改前后的差值,便于后续修改相应的校验值
    p=p->next;
    //更新后面的校验值
    while(p)        //是一个循环套住另外一个循环
    {
        older=p->Checknum;  //原先的校验值 
        p->Checknum=(p->Checknum+chazhi)%113;   //进行更新
        chazhi=p->Checknum-older;   //为下一个更新做准备
        p=p->next;
    }
    return ;    //当所有的都更新后篡改完成
}

void PrintLink(LInklist *L)    //将这个表中的内容全部打印出来
{
    LNode * p=L->head;
    while(p)
    {
        cout<<"编号："<<p->Idnum<<endl;
        cout<<"信息："<<p->information<<endl;
        cout<<"校验码："<<p->Checknum<<endl;
        cout<<endl;
        p=p->next;
    }
}
void DeleteLink(LInklist * L)  //释放空间
{
    LNode *p;
    while(L->head->next)
    {
        p=L->head->next;
        delete L->head;
        L->head=p;
    }
    delete L->head; //将所有空间进行释放
}

void strcpylist(LInklist *L,int id,char *a,int cnum)   //将文件中的直接存进去
{
    LNode *p=L->head;
    int length;
    for(int i=0;;i++)   //测量这个信息的长度
    {
        if(a[i]==92&&a[i+1]==48)
        {
            length=i;   
            break;
        }
    }
    if(L->sumnums==0)
    {
        LNode *pp=new LNode;
        pp->Idnum=0;
        pp->Checknum=cnum;
        pp->information=new char[length+1];
        strcpy(pp->information,a);
        pp->information[length]='\0';
        pp->next=0;
        L->head=pp;
        L->sumnums++;
        return ;
    }
    while(p->next)
    {
        p=p->next;
    }
    LNode *pp=new LNode;
    pp->Idnum=id;
    pp->Checknum=cnum;
    pp->information=new char[length+1]; //要及时赋空间
    strcpy(pp->information,a);  
    pp->information[length]='\0';
    pp->next=0;
    p->next=pp;
    p=pp;
    L->sumnums++;
    return;
}
