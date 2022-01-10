#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include<string.h>
using namespace std;
//哈希表实现飞机vip客户的发掘
//采用除留余数法(取1510的模运算)1600个数据必定会产生冲突

struct Passenger1   //采用链式来解决冲突
{
    char Idcard[20];    //客户的身份证号码
    char Name[6];   //客户的姓名
    char Flightid[6];   //航班号信息
    char year[6]; //起飞时间年月日
    char month[4];
    char day[4];
    char Distance[6];    //此次飞行的旅行时间
    int judge;  //判断这个位置是不是已经有乘客,便于查看是否使用冲突算法
    Passenger1 *next;
};
struct HashTable1
{
    Passenger1 *elem;   //存储动态的基地址
    int count;  //此时存入了多少个数据
    int tablesize;  //哈希表的总长度
};  

struct K_Passenger  //采用开放式来解决冲突
{
    char Idcard[20];    //客户的身份证号码
    char Name[6];   //客户的姓名
    char Flightid[6];   //航班号信息
    char year[6]; //起飞时间年月日
    char month[4];
    char day[4];
    char Distance[6];    //此次飞行的旅行时间
    int judge;  //判断这个位置是不是已经有乘客,便于查看是否使用冲突算法
};
struct K_HashTable
{
    K_Passenger *elem;   //存储动态的基地址
    int count;  //此时存入了多少个数据
    int tablesize;  //哈希表的总长度
};  

 

int InsertData(HashTable1 *H,int&sum,K_HashTable*KH);  //向这个哈希表中取存入数据
bool collision(HashTable1 *&H,int x,Passenger1 *p); //向这个哈希表这个位置存入数据
void Findvip(K_HashTable *H);    //去数据中发掘vip
int Judgevip(K_HashTable *H,int x,char *p);   //判断是不是vip客户
int CollisionChongxuan(K_HashTable*H,int x,K_Passenger *p,int jJs);

int main()
{
    cout<<"       航空公司Hash表发生冲突次数,及重新定位一览表"<<endl;
    //链式解决冲突
    HashTable1 *H1=new HashTable1;    
    H1->elem=new Passenger1[1600]; //哈希表的长度
    H1->count=0;
    H1->tablesize=1600;
    for(int i=0;i<1600;i++)  //进一步初始化这个哈希表
    {
        H1->elem[i].next=NULL;  //初始化时都是指向了空指针
        H1->elem[i].judge=0; //判断这个位置是否已经有了占位
    }

    //开放选址解决冲突
    K_HashTable* K_H=new K_HashTable;
    K_H->elem=new K_Passenger[1600]; //哈希表的长度
    K_H->count=0;
    K_H->tablesize=1600;
    for(int i=0;i<1600;i++)  //进一步初始化这个哈希表
    {
        K_H->elem[i].judge=0; //初始化这里没有人占位
    }
    int chongxindingwei;
    int jsCollison=1;
    chongxindingwei=InsertData(H1,jsCollison,K_H);
    jsCollison--;
    cout<<"发生冲突的次数:";
    cout<<jsCollison<<endl;
    cout<<"进行重定位的总次数：";
    cout<<chongxindingwei<<endl;
    cout<<"         航空公司发掘vip用户系统"<<endl;
   
    cout<<"以下是潜在vip贵宾客户(乘机频率大于等于两次)"<<endl;
    Findvip(K_H);
    delete []K_H->elem; //释放相应得空间
    delete []H1->elem;   //及时释放我给其分配的储存空间
    delete H1;
    return 0;
}

int InsertData(HashTable1 *H,int&tongji,K_HashTable * KH)  //向这个哈希表中取存入数据
{
    int sumChong=0;
    vector<string> Name;    //将乘客的不重复的身份证写入到新的数组之中
    fstream file;
    file.open("Hashvip\\Passenger.txt",ios::in);
    if(!file)
    {
        cout<<"文件打开失败";
        exit(0);
    }
    Passenger1 *temp=new Passenger1;
    temp->judge=1;  //这个位置已经有值出现了
    temp->next=0;   //下一个链表节点是空指针
    K_Passenger *linshi=new K_Passenger;
    int js=0;
    int sum=1;
    while(!file.eof())
    {
        int xiabiao=0;   //查看具体放在哪一个位置之上
        file>>temp->Idcard;
        file>>temp->Name;
        file>>temp->Flightid;
        file>>temp->year;
        file>>temp->month;
        file>>temp->day;
        file>>temp->Distance;  
        
        strcpy(linshi->day,temp->day);
        strcpy(linshi->Distance,temp->Distance);
        strcpy(linshi->Flightid,temp->Flightid);
        strcpy(linshi->Idcard,temp->Idcard);
        strcpy(linshi->year,temp->year);
        strcpy(linshi->month,temp->month);
        strcpy(linshi->Name,temp->Name);
        if(file.fail()) //读到结尾需要直接退出
        {
            break;
        }

        if(js==0)
        {
            Name.push_back(temp->Idcard);
        }
        else
        {
            int judeg=0;    //判断是否是重复的情况
            for(int k=0;k<Name.size();k++)
            {
                if(Name[k]==temp->Idcard)
                {
                    judeg=1;    //表明已经有相同的id,不用再次写入数据中去
                    break;
                }
            }
            if(judeg==0)    //不重复可以再次写入进去
            {
                Name.push_back(temp->Idcard);
            }
        }
        for(int i=14;i<18;i++)
        {
            xiabiao=(xiabiao+(int(temp->Idcard[i])-48))*10;
        }
        xiabiao=xiabiao/10;
        xiabiao=xiabiao%1510;  //除留取余是取得1510
        //cout<<xiabiao<<endl;
        if(collision(H,xiabiao,temp))  //向哈希表中取存入数据    
        {
            sum++;
        }   
        int linshix=sumChong;       
        sumChong=sumChong+CollisionChongxuan(KH,xiabiao,linshi,tongji);
        if(sumChong>linshix)
        {
            tongji++;
        }
        js++;
    }
    file.close();   //存完之后关闭文件且释放空间
    delete temp; 
    file.open("Hashvip\\IdNorepeat.txt",ios::out);
    for(int j=0;j<Name.size();j++) //将不重复的身份证信息存入到新的文件中去,便于利用哈希查找发掘vip
    {
        file<<Name[j]<<endl;
    }
    file.close();   //再次关闭文件
    return sumChong;
}

bool collision(HashTable1 *&H,int x,Passenger1 *p) //向这个哈希表这个位置存入数据
{
    if(H->elem[x].judge==0) //之前是空位直接赋值进去
    {
        H->elem[x].judge++;
        strcpy(H->elem[x].year,p->year);
        strcpy(H->elem[x].month,p->month);
        strcpy(H->elem[x].day,p->day);
        strcpy(H->elem[x].Flightid,p->Flightid);
        strcpy(H->elem[x].Distance,p->Distance);
        strcpy(H->elem[x].Idcard,p->Idcard);
        strcpy(H->elem[x].Name,p->Name);
        H->elem[x].next=NULL;
        return false;   //没有发生冲突
    }
    else    //直接卡在表头有后面即可
    {
        H->elem[x].judge++; //测试这个位置究竟有多少的数据
        p->next=H->elem[x].next;
        H->elem[x].next=p;
        return true;
    }
}
int CollisionChongxuan(K_HashTable*H,int x,K_Passenger *p,int jJs)  //将值存进开放定址,并且返回重定位次数
{
    int js=0;
    int i;
    if(H->elem[x].judge==0)
    {
        H->elem[x].judge++;
        strcpy(H->elem[x].year,p->year);
        strcpy(H->elem[x].month,p->month);
        strcpy(H->elem[x].day,p->day);
        strcpy(H->elem[x].Flightid,p->Flightid);
        strcpy(H->elem[x].Distance,p->Distance);
        strcpy(H->elem[x].Idcard,p->Idcard);
        strcpy(H->elem[x].Name,p->Name);
    }
    else
    {
        for(i=1;;i++)
        {
            js++;   //只要有冲突就需要重新定位
            if(H->elem[x+i*i].judge==0) //发生重新定位的次数
            {
                break;
            }           
        }
        H->elem[x+i*i].judge++;
        strcpy(H->elem[x+i*i].year,p->year);
        strcpy(H->elem[x+i*i].month,p->month);
        strcpy(H->elem[x+i*i].day,p->day);
        strcpy(H->elem[x+i*i].Flightid,p->Flightid);
        strcpy(H->elem[x+i*i].Distance,p->Distance);
        strcpy(H->elem[x+i*i].Idcard,p->Idcard);
        strcpy(H->elem[x+i*i].Name,p->Name);
    }
    if(js>=1)
    {
        cout<<"第"<<jJs<<"次冲突发生,进行重新定位："<<js<<"次。"<<endl;
    }
    return js;
}



void Findvip(K_HashTable *H)    //去数据中发掘vip
{
    fstream file;
    file.open("Hashvip\\IdNorepeat.txt",ios::in);
    if(!file)
    {
        cout<<"文件打开失败";
        exit(0);
    }
    char id[20];
    while(!file.eof())
    {
        file>>id;
        if(file.fail())
        {
            break;
        }
        int xiabiao=0;
        for(int i=14;i<18;i++)
        {
            xiabiao=(xiabiao+(int(id[i])-48))*10;
        }
        xiabiao=xiabiao/10;
        xiabiao=xiabiao%1510;   //除留取余取得1510,否则会超过数组得范围!!!
        int xjs=Judgevip(H,xiabiao,id);
        if(xjs>1)  //如果是vip
        {
            int total=0;
            int linshi=0;   //记录总的距离的值                     
            for(int i=0;;i++)
            {
                if(H->elem[xiabiao+i*i].judge==0)   //此时这个位置为空,则跳出循环
                {
                    break;
                }
                if(strcmp(H->elem[xiabiao+i*i].Idcard,id)==0)
                {
                    linshi=0;   //将临时的值进行初始化
                    for(int j=0;H->elem[xiabiao+i*i].Distance[j]!='\0';j++)
                    {
                        linshi=(linshi+int(H->elem[xiabiao+i*i].Distance[j])-48)*10;
                    }
                    linshi=linshi/10; 
                    total=total+linshi;                 
                }
            }
            for(int i=0;;i++)
            {
                if(strcmp(H->elem[xiabiao+i*i].Idcard,id)==0)
                {
                    cout<<"用户姓名："<<H->elem[xiabiao+i*i].Name<<" ";
                    cout<<"用户id："<<H->elem[xiabiao+i*i].Idcard<<" ";
                    cout<<"近两年乘机频率："<<xjs<<" ";                 
                    cout<<"总里程数："<<total<<endl;
                    break;
                }
            }
        }
    }
    file.close();
    return;
}

int Judgevip(K_HashTable *H,int x,char *p)   //判断是不是vip客户
{
    
    if(H->elem[x].judge==0)   //只有一次乘机记录不是vip
    {
        return (-1);
    }
    else
    {
        int js=0;
        if(strcmp(H->elem[x].Idcard,p)==0)  //这个值查看冲突
        {
            js++;
        }
        for(int i=1;;i++)
        {
            if(H->elem[x+i*i].judge==0) //空位就跳出循环
            {
                break;
            }
            if(strcmp(H->elem[x+i*i].Idcard,p)==0)  //一样就js++
            {
                js++;
            }
        }
        return js;
    }
}
