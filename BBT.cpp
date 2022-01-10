#include<iostream>
#include<fstream>
#include<math.h>
using namespace std;
//平衡二叉树的操作的演示
//查找插入和删除
struct  BBT
{
    int num;    //这个平衡二叉树的值
    int judge;  //这个平衡二叉树左右树深度的差值
    BBT * lchild,* rchild;  
};
int BBTDepth(BBT *T);   //返回这棵树的高度
int BBTDifhigh(BBT *T); //判断这个点左右子树的差值
BBT * LLAdjust(BBT *A); //LL型调整过程
BBT * RRAdjust(BBT *A); //RR型调整过程
BBT * LRAdjust(BBT *A); //LR型调整过程
BBT * RLAdjust(BBT *A); //RL型调整过程
BBT * BSTInsert(BBT * T,int x); //向进行二叉排序树的插入中去插入相应的值
void Update(BBT * T);  //更新每一个节点的平衡度
bool Isbanlance(BBT * T); //判断此时这棵树是不是并不平衡了
BBT * FindNoBanlance(BBT * T);  //找到最下面不满足平衡的点
BBT * BBTInsert(BBT * T,int x);  //植入平衡二叉树
BBT * EditTree(BBT * T); //修正这个平衡树
BBT * FindDingdian(BBT *T,int x); //找到特定顶点的父亲
void Printxianxu(BBT *T);
BBT *BSTDelete(BBT*T,int key);  //删除二叉排序树的节点,递归算法
BBT *BBTDelete(BBT*T,int key);  //删除平衡二叉树的节点,递归算法
BBT *FindBenshen(BBT *T,int key);   //返回这个值
BBT* DELETEBST(BBT* T);
int main()
{
    int choice;
    fstream file;
    file.open("BBT\\BBT.txt",ios::in);  
    if(!file)
    {
        cout<<"error";
        exit(0);
    }
    int temp;
    BBT *T=NULL;
    BBT *T1=0;
    while(!file.eof())  //对这个平衡二叉树进行创建
    {
        file>>temp;
        if(file.fail())
        {
            break;
        }        
        T=BBTInsert(T,temp);
        T1=BSTInsert(T1,temp);       
    }
    while(1)
    {
        cout<<"         平衡二叉树的演示"<<endl;
        cout<<"1.展示现在这棵平衡二叉树"<<endl;
        cout<<"2.查找你想要的根节点并输出它和它的子树"<<endl;
        cout<<"3.插入任意值"<<endl;
        cout<<"4.删除其中存在的值"<<endl;
        cout<<"5.退出演示"<<endl;
        cout<<endl;
        cout<<"请输入你想要的操作:";
        cin>>choice;
        if(choice==5)
        {
            break;
        }
        switch(choice)
        {
            case 1:
            {
                Printxianxu(T1);
                cout<<endl;
                if(Isbanlance(T1))
                {
                    cout<<"这是一棵平衡二叉树"<<endl;
                }
                else
                {
                    cout<<"这只是一棵普通的排序二叉树"<<endl;
                }
                Printxianxu(T);
                cout<<endl;
                if(Isbanlance(T))
                {
                    cout<<"这是一棵平衡二叉树"<<endl;
                }
                else
                {
                    cout<<"这只是一棵普通的排序二叉树"<<endl;
                }
                cout<<endl;
                cout<<endl;
                break;
            }
            case 2:
            {
                cout<<"你想要查找的值：";
                int x1;
                
                cin>>x1;
                BBT * newtree=new BBT;
                newtree=FindBenshen(T,x1);
                if(newtree==0)
                {
                    cout<<"这棵平衡二叉树不存在这个值";
                }
                else
                {
                    cout<<"这个顶点及它的子孩子的值：";
                    Printxianxu(newtree);
                }
                cout<<endl;
                cout<<endl;
                break;
            }
            case 3:
            {
                cout<<"请输入你想要加入的值：";
                int xxx;
                cin>>xxx;
                T1=BSTInsert(T1,xxx);
                T=BBTInsert(T,xxx);
                cout<<"加入后的排序二叉树展示:";
                Printxianxu(T1);
                cout<<endl;
                cout<<"加入后的平衡二叉树展示：";
                Printxianxu(T);
                cout<<endl;
                cout<<endl;
                cout<<endl;
                break;
            }     
            case 4:
            {
                cout<<"你想要删除的值：";
                int zhi;
                cin>>zhi;
                BBT *TTemp;
                TTemp=FindBenshen(T,zhi);
                if(TTemp==0)
                {
                    cout<<"没有找到你想要删除的值"<<endl;
                }
                else
                {
                    cout<<"删除后的平衡二叉树:";
                    BBTDelete(T,zhi);
                    Printxianxu(T);
                    cout<<endl;
                    cout<<endl;
                }
                break;
            }         
        }
    }

    file.close();   
    delete T;
    delete T1; 
    return 0;
}

int BBTDifhigh(BBT *T) //判断这个点左右子树的差值
{
    int sum;
    int Lhigh,Rhigh;
    //探测左边子树的深度
    if(T->lchild==0)    
    {
        Lhigh=0;
    }
    else
    {
        Lhigh=BBTDepth(T->lchild);
    }
    //探测右边子树的深度
    if(T->rchild==0)
    {
        Rhigh=0;
    }
    else
    {
        Rhigh=BBTDepth(T->rchild);
    }
    sum=Lhigh-Rhigh;   //sum为左右两子树的高度差
    return sum;
}
int BBTDepth(BBT *T)   //返回这棵树的高度
{
    int lefthigh,righthigh;
    if(T==0)
    {
        return 0;
    }
    else
    {
        lefthigh=BBTDepth(T->lchild);
        righthigh=BBTDepth(T->rchild);
        return (lefthigh>righthigh)?(lefthigh+1):(righthigh+1);   //返回这棵树的高度
    }
}

BBT * LLAdjust(BBT *A) //LL型调整过程
{
    BBT * B=A->lchild;      //将B初始为A的左孩子
    A->lchild=B->rchild;    //将A的左孩子替换成B的右孩子
    B->rchild=A;    //将B的右孩子指向A,即为将B向右进行旋转一圈达到平衡
    //调整完成,下面是更新这两个节点的平衡差值
    int Diff;
    Diff=BBTDifhigh(B);
    B->judge=Diff;
    Diff=BBTDifhigh(B->rchild);
    B->rchild->judge=Diff;
    return B;
}

BBT * RRAdjust(BBT *A) //RR型调整过程
{
    BBT * B=A->rchild;
    A->rchild=B->lchild;
    B->lchild=A;
    //及时更新A和B的深度值
    B->judge=BBTDifhigh(B);
    B->lchild->judge=BBTDifhigh(B->lchild);
    return B;
}

BBT * LRAdjust(BBT *A) //LR型调整过程
{
    A->lchild=RRAdjust(A->lchild);  //先进行左旋,即为RR型
    return LLAdjust(A); //再进行右选,即为LL型
}

BBT * RLAdjust(BBT *A)  //RL型调整过程
{
    A->rchild=LLAdjust(A->rchild);  //先进右边进行右旋,即为LL
    return RRAdjust(A); //再进行左旋,即为RR
}

BBT * BSTInsert(BBT * T,int x) //向进行二叉排序树的插入中去插入相应的值
{
    if(T==NULL) //当是空时则需要进行插入,但插入的值肯定是叶子结点,所以左右子树都没有
    {
        T=new BBT;
        T->num=x;
        T->lchild=T->rchild=NULL;
        return T;
    }
    else if(T->num==x)
    {
        return T;   //直接返回这个树节点,不需要插入任何值
    }
    else if(T->num<x) //进入右节点去进行查找
    {
        T->rchild=BSTInsert(T->rchild,x);
    }
    else    //进入左节点进行查找
    {
        T->lchild=BSTInsert(T->lchild,x);
    }
    return T;
}

void Update(BBT * T)  //更新每一个节点的平衡度
{
    if(T==0)    //按照先序遍历的结果去更新这个平衡度
    {
        return;
    }
    T->judge=BBTDifhigh(T);
    Update(T->lchild);
    Update(T->rchild);
}   

bool Isbanlance(BBT * T) //判断此时这棵树是不是并不平衡了
{
    if(T)
    {
        if(abs(T->judge)>1)  //此时发现不平衡
        {
            return false;
        }
        if(!Isbanlance(T->lchild))    //如果左边不满足,则直接返回false
        {
            return false;
        }
        if(!Isbanlance(T->rchild))    //如果右边不满足,也直接返回false
        {
            return false;
        }
    }
    return true;    //当T为NULL直接返回true,或者正常走完了也是返回true
}

BBT * FindNoBanlance(BBT * T)  //找到最下面不满足平衡的点
{
    BBT *temp=NULL;
    if(T)   //当从下往上找,找到第一个不满足的值就是需要调整的平衡点
    {
        temp=FindNoBanlance(T->lchild); //先找左边的值
        if(temp!=0)
        {
            return temp;
        }
        temp=FindNoBanlance(T->rchild); //再找右边的值
        if(temp!=0)
        {
            return temp;
        }
        if(abs(T->judge)>1)
        {
            temp=T;
            return temp;
        }
    }
    return temp;
}

BBT * BBTInsert(BBT *T,int x)  //植入平衡二叉树
{
    T=BSTInsert(T,x);
    Update(T);  //更新这棵树的平衡点信息
    BBT * Linshi=T; //用一棵临时树代替这个树
    if(Isbanlance(T))   //如果平衡直接返回
    {
        return T;
    }   
    else    //如果不平衡,将其转换成平衡
    {
        int cjudge;     //判断这棵树需要改变的位置是不是再根节点
        int jss;    //判断是这个父亲的左孩子还是右孩子需要改变
        BBT *tempT=FindNoBanlance(Linshi);  //找到最下面不平衡的点(后序遍历)
        BBT *fathertemp;    //需要修改的点的父亲节点
        if(T==tempT)    
        {
            cjudge=0;   //=0是根节点被修改
        }
        else
        {
            cjudge=1;   
            fathertemp=FindDingdian(Linshi,tempT->num); //找到修改点的父亲
            if(fathertemp->lchild->num==tempT->num) //当是左边被修改记作0
            {
                jss=0;  
            }
            else    //当是右边被修改记作1
            {
                jss=1;
            }               
        }                   
        tempT=EditTree(tempT);  //将这棵树进行平衡修正
        if(cjudge==0)   //根节点直接返回这个这个修改后的值
        {
            T=tempT;
        }
        if(cjudge==1)   //不是根节点
        {
            if(jss==0)  //如果是左边则直接左孩子指向被修改的值
            {
                fathertemp->lchild=tempT;
            }
            if(jss==1)  //右边右孩子被指向修改后的值
            {
                fathertemp->rchild=tempT;
            }
        }       
    }
    Update(T);
    return T;
}

BBT * EditTree(BBT * T) //修正这个平衡树
{
    
    if(T->judge>1)  //左边-右边>1(LL或者LR)
    {
        int num;
        num=BBTDifhigh(T->lchild);
        if(num==1)  //为LL类型的不平衡
        {
            T=LLAdjust(T);            
        }
        else    //为LR类型的不平衡
        {
            T=LRAdjust(T);            
        }
    }
    if(T->judge<-1)  //左边-右边<1(RR或者RL)
    {
        int num;
        num=BBTDifhigh(T->rchild);
        if(num==-1) //为RR型不平衡
        {
            T=RRAdjust(T);           
        }
        else    //为RL不平衡
        {
            T=RLAdjust(T);   
        }
    }
    return T;
}

void Printxianxu(BBT *T)
{
    if(T)
    {
        cout<<T->num<<" ";      
        Printxianxu(T->lchild);
        Printxianxu(T->rchild);
    }
}

BBT * FindDingdian(BBT *T,int x) //找到特定顶点的父亲,便于修改后将后面的指针指向修改后的值
{
    BBT *temp=NULL;
    if(T==0)
    {
        return NULL;
    }
    
    else if(T->lchild->num==x||T->rchild->num==x)
    {
        temp=T;
        return temp;
    }
    else
    {
        temp=FindDingdian(T->lchild,x); //左边找到这个值就不用再找了
        if(temp)
        {
            return temp;
        }
        temp=FindDingdian(T->rchild,x); //否则进入右边去找
    }
    return temp;
}

BBT *BSTDelete(BBT*T,int key)  //删除二叉排序树的节点,递归算法
{
    BBT *p;
    if(T==NULL)  //这个点已经是空指针了
    {
        return T;
    }
    else if(T->num==key)    //当这个点是要删除的节点
    {
        if(T->lchild==NULL&&T->rchild==NULL)    //这个点是叶子节点直接删除
        {
            delete T;
            return NULL;    
        }
        else if(T->lchild==NULL&&T->rchild!=NULL)   //这个节点有一个右叶子节点
        {
            p=T->rchild;    //直接指向它的右指针
            delete T;
            return p;   //将这个节点删除,直接指向自己的右子树
        }
        else if(T->lchild!=NULL&&T->rchild==NULL)   //这个节点只有一个左孩子节点
        {
            p=T->lchild;    //直接指向它的左指针
            delete T;
            return p;
        }
        else    //这个有左孩子也有右孩子
        {
            p=T->lchild;    //进入这个点的左孩子,用这个左孩子的最大值去替换这个顶点节点
            while(p->rchild)    //移到最右下角,即为这个左树中的最大值
            {
                p=p->rchild;
            }
            T->num=p->num;  //用这个值去替换这个顶点
            T->lchild=BSTDelete(T->lchild,p->num);  //进左子树,去把这个替换的值删除
            return T;   //需要将这个T作为返回值
        }
    }
    else if(T->num<key)
    {
        T->rchild=BSTDelete(T->rchild,key);
    }
    else
    {
        T->lchild=BSTDelete(T->lchild,key);
    }
    return T;
}

BBT *BBTDelete(BBT*T,int key)  //删除平衡二叉树的节点,递归算法
{
    T=BSTDelete(T,key);
    Update(T);  //更新这棵树的平衡点信息
    BBT * Linshi=T; //用一棵临时树代替这个树
    if(Isbanlance(T))   //如果平衡直接返回
    {
        return T;
    }   
    else    //如果不平衡,将其转换成平衡
    {
        int cjudge;     //判断这棵树需要改变的位置是不是再根节点
        int jss;    //判断是这个父亲的左孩子还是右孩子需要改变
        BBT *tempT=FindNoBanlance(Linshi);  //找到最下面不平衡的点(后序遍历)
        BBT *fathertemp;    //需要修改的点的父亲节点
        if(T==tempT)    
        {
            cjudge=0;   //=0是根节点被修改
        }
        else
        {
            cjudge=1;   
            fathertemp=FindDingdian(Linshi,tempT->num); //找到修改点的父亲
            if(fathertemp->lchild->num==tempT->num) //当是左边被修改记作0
            {
                jss=0;  
            }
            else    //当是右边被修改记作1
            {
                jss=1;
            }               
        }                   
        tempT=EditTree(tempT);  //将这棵树进行平衡修正
        if(cjudge==0)   //根节点直接返回这个这个修改后的值
        {
            T=tempT;
        }
        if(cjudge==1)   //不是根节点
        {
            if(jss==0)  //如果是左边则直接左孩子指向被修改的值
            {
                fathertemp->lchild=tempT;
            }
            if(jss==1)  //右边右孩子被指向修改后的值
            {
                fathertemp->rchild=tempT;
            }
        }       
    }
    Update(T);
    return T;
}



BBT *FindBenshen(BBT *T,int key)   //返回这个值
{
    if(T==NULL) //等于空的时候就是返回空
    {
        return NULL;
    }
    else if(T->num==key)    //相等的时候就返回这个值
    {
        return T;
    }
    else if(T->num<key) //如果此时这个数据更小则进入右子树去寻找这个节点的位置
    {
        return (FindBenshen(T->rchild,key));  
    }
    else
    {
        return FindBenshen(T->lchild,key);    //如果此时这个数据更大则进入左子树去查找
    }   
}
BBT* DELETEBST(BBT* T) 
{
    if(T)
    {
        T->lchild=DELETEBST(T->lchild);
        T->rchild=DELETEBST(T->rchild);
        delete T;
        return NULL;
    }
    return NULL;
}
