#include<iostream>
#include<string.h>
#include<stdbool.h>
#include<string.h>
#include<fstream>
using namespace std;
//用兄弟孩子树来得到相应的键所对应的值的结果
struct NormalTree
{
    char key[256];  //储存键
    char value[256];    //储存键所对应的值
    int judgestring;   //判断值是一个对象还是字符串(1是字符串,0是对象)
    NormalTree *leftChild;  //进入左孩子,只有嵌套的时候才会进入左孩子
    NormalTree *rightBrother;   //并列的就进入右边孩子
};
bool InsertData(NormalTree *&T,char *p,int high,int judge,int nowhigh); //进行递归存数据
void DeleteTree(NormalTree *&T);    //删除树释放内存
bool Findvalue(NormalTree*T,char *p,NormalTree *&Rtree);  //返回它的值
int main()
{
    int n,m;
    char ch;
    fstream file;
    file.open("C:\\Codefield\\CODE_C\\CDesign\\JSON\\JSON.txt",ios::in);
    if(!file)
    {
        cout<<"打开文件失败。";
        exit(0);
    }
    file>>n;
    file>>m;    
    file>>ch;    //将第一个左括号存入进去
    file.ignore();
    NormalTree *tree=NULL;  //这是一个空树
    char shuzu[256];
    int cengci=0;
    for(int i=1;i<n-1;i++)  
    {
        int judgevalue; //判断值是一个对象还是字符串
        file.getline(shuzu,256);   
        if(shuzu[0]==125)   //中括号不进行任何操作
        {
            cengci--;   //向左边进入的深度层次减小
            continue;
        }   
        for(int j=0;;j++)
        {
            if(shuzu[j]==58&&shuzu[j+1]==32)
            {
                if(shuzu[j+2]==123) //如果是中括号说明值不是一个字符串而是一个对象
                {
                    judgevalue=0;   //如果值是对象为0
                }
                else 
                {
                    judgevalue=1;    //如果值是字符串为1
                }
                break;
            }
        }
        InsertData(tree,shuzu,cengci,judgevalue,0);
        if(judgevalue==0)   //在后面一层才需要向左边进行移动,这次这一层还是存在原来那一层去
        {
            cengci++;
        }
    }
    file>>ch;
    file.ignore();   //将换行符号进行去掉
    for(int i=0;i<m;i++)
    {
        int judge=0;
        file.getline(shuzu,256);
        for(int j=0;;j++)
        {
            if(shuzu[j]=='\0')
            {
                break;
            }
            if(shuzu[j]==46)
            {
                judge++;    //judge++但是不应该退出,应该记录总的.的个数
            }
        }
        NormalTree * ppt=NULL;

        if(judge==0)    //如果中间没有出现.深度查找时
        {
            Findvalue(tree,shuzu,ppt);
            if(ppt==NULL)
            {
                cout<<"NOTEXIST"<<endl;
            }
            else
            {
                if(ppt->judgestring==0)
                {
                    cout<<"OBJECT"<<endl;
                }
                else
                {
                    cout<<"STRING "<<ppt->value<<endl;
                }
            }
        }

        else    //如果中间出先.深度查找时
        {
            NormalTree *PPT=NULL;
            char **mvp=new char*[judge+1];    //直接构建二维指针 
            for(int i=0;i<=judge;i++)
            {
                mvp[i]=new char[256];
            }
            int js=0;   //给指针赋值空间
            int deep=0;    //判断到达第几个指针
            int isz=0;  //数组的顺序
            while(1)
            {
                if(shuzu[isz]=='\0')    //什么时候将这个数组读完
                {
                    mvp[deep][js]='\0'; //将这个值进行封存,才能更好的比较
                    break;
                }
                if(shuzu[isz+1]==46)    //当读到有点的时候,需要将数组换到下一行
                {
                    mvp[deep][js]=shuzu[isz];
                    js++;
                    mvp[deep][js]='\0';
                    js=0;   //重新给下一个数组赋值
                    deep++;
                    isz=isz+2;
                }
                else
                {
                    mvp[deep][js]=shuzu[isz]; //普通的存入数据
                    js++;
                    isz++;
                }
            }
            int judgeyn=1;  //判断能不能找到相应的值
            NormalTree *linshiPPT=NULL;
            for(int i=0;i<=judge;i++)
            {                               
                if(i==0)
                {
                    if(!Findvalue(tree,mvp[i],PPT))   //表示没有找到相应的值
                    {
                        judgeyn=0;  
                        break;
                    }
                }
                else
                {
                    linshiPPT=PPT->leftChild;   //向左边进行深度遍历
                    if(!Findvalue(linshiPPT,mvp[i],PPT))
                    {
                        judgeyn=0;  //没有在相应的层次找到相应的值
                        break;
                    }
                }
            }
            if(judgeyn==0)
            {
                cout<<"NOTEXIST"<<endl;
            }
            else
            {                
                if(PPT->judgestring==0)
                {
                    cout<<"OBJECT"<<endl;
                }
                else
                {
                    cout<<"STRING "<<PPT->value<<endl;
                }
            }
            for(int i=0;i<=judge;i++)
            {
                delete []mvp[i];    //将每一个指针的空间给释放
            }
            delete mvp; //将整个指针都释放
        }
    }
    DeleteTree(tree);
    return 0;
    
}
//T是要存入的树,p是这一行字符串,high是走几次左子树,judge是判断是否存值
//nowhigh是判断此时在第几层
bool InsertData(NormalTree *&T,char *p,int high,int judge,int nowhigh) //进行递归存数据
{
    if(T==NULL&&nowhigh!=high)  //不在那一层且直接来到了尾部
    {
        return false;   
    }
    if(T==NULL&&judge==1)   //直接存键和值的组合
    {
        T=new NormalTree;
        T->leftChild=T->rightBrother=0;
        T->judgestring=1;
        int i=1;
        int js=0;
        //记录键的具体值
        while(1)
        {
            if((p[i]==34&&p[i-1]!=92)||(p[i]==34&&p[i-1]==92&&p[i-2]==92))   //当第一个数据记录完成时,双引号前面不是反斜杠
            {   //("\"\\")和("hello")两种结尾的方式
                T->key[js]='\0';
                js=0;   //为它的值进行初始化
                i=i+4;
                break;
            }
            if(p[i]==92)    //出现反斜杠的模式
            {
                T->key[js]=p[i+1];
                js++;
                i=i+2;
            }
            else    //正常计数模式
            {
                T->key[js]=p[i];
                js++;
                i++;
            }
        }
        //记录值的具体值
        while(1)
        {
            if((p[i]==34&&p[i-1]!=92)||(p[i]==34&&p[i-1]==92&&p[i-2]==92))   //当第一个数据记录完成时,双引号前面不是反斜杠
            {   //("\"\\")和("hello")两种结尾的方式
                T->value[js]='\0';
                js=0;   //为它的值进行初始化
                break;
            }
            if(p[i]==92)    //出现反斜杠的模式
            {
                T->value[js]=p[i+1];
                js++;
                i=i+2;
            }
            else    //正常计数模式
            {
                T->value[js]=p[i];
                js++;
                i++;
            }
        }
        return true;
    }


    if(T==NULL&&judge==0)   //键的值对应是对象的情况
    {
        T=new NormalTree;
        T->leftChild=T->rightBrother=0;
        T->judgestring=0;
        int i=1;
        int js=0;
        //记录键的具体值
        while(1)
        {
            if((p[i]==34&&p[i-1]!=92)||(p[i]==34&&p[i-1]==92&&p[i-2]==92))   //当第一个数据记录完成时,双引号前面不是反斜杠
            {   //("\"\\")和("hello")两种结尾的方式
                T->key[js]='\0';
                break;
            }
            if(p[i]==92)    //出现反斜杠的模式
            {
                T->key[js]=p[i+1];
                js++;
                i=i+2;
            }
            else    //正常计数模式
            {
                T->key[js]=p[i];
                js++;
                i++;
            }
        }
        return true;
    }
    //直接向右边探到底部
    //不满足相应的层度,则会进入左孩子中去存储
    if(!InsertData(T->rightBrother,p,high,judge,nowhigh))   
    {
        while(1)
        {
            nowhigh++;
            if(InsertData(T->leftChild,p,high,judge,nowhigh))   //直到存入了数据才进行弹出
            {
                return true;    //直接跳出这个递归循环
            }
        }
    }   
    return true;    //从这里结束的应该直接返回true,即真实的情况
}

void DeleteTree(NormalTree *&T)    //删除树释放内存
{
    if(T)
    {
        DeleteTree(T->leftChild);
        DeleteTree(T->rightBrother);
        delete T;
    }
}

bool Findvalue(NormalTree*T,char *p,NormalTree * &Rtree)  //返回它的值
{
    NormalTree *linshiT=T;  //只在这一层中去寻找
    while(linshiT)
    {
        if(strcmp(linshiT->key,p)==0)
        {
            Rtree=linshiT;
            return true;    //找到则返回true
        }
        linshiT=linshiT->rightBrother;
    }
    return false;   //没找到则返回false
    
}

