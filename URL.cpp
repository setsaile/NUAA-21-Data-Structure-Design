#include<iostream>
#include<string>
#include<string.h>
#include<algorithm>
#include<vector>
using namespace std;
//URL映射匹配规则

struct URL
{
    string address; //收录URL匹配名称
    string Value;   //URL名称对应的值
};

string FindSum(URL *L,string str,int len);  //将这个值得URL映射给找出来
bool judgeHeFa(string str); //检测这个输入是否合法
bool judgeisPipei(string str1,string str2); //判断是不是满足匹配
int judgeXiangdeng(string str1,string str2);   //判断两个前面的地址是不是相等

int main()
{
    int n,m;    //n代表编码规则,m代表你要测试的数据
    cin>>n>>m;
    URL model[n];   //将规则存在结构体里面
    string tempAdd,tempVa;
    for(int i=0;i<n;i++)    //将这些值存在结构体里面
    {
        cin>>tempAdd>>tempVa;
        model[i].address=tempAdd;   //将这个值赋再结构体里面
        model[i].Value=tempVa;
    }
    string tempsum;
    vector<string> Totalsum;    //用来储存最终结果便于直接输出
    for(int i=0;i<m;i++)
    {
        cin>>tempsum;
        Totalsum.push_back(FindSum(model,tempsum,n)); //将这些值存入到一个新的系统里面
    }
    for(int i=0;i<m;i++)
    {
        cout<<Totalsum[i]<<endl;
    }
    return 0;
}

string FindSum(URL *L,string str,int len)  //将这个值的URL映射给找出来
{
    int judge=0;
    string sumstr="";   //空串开始
    if(!judgeHeFa(str))    //先检查返回得是否合法,如果不合法直接返回404
    {   
        sumstr="404";   
        return sumstr;
    }
    for(int i=0;i<len;i++)  //当这两个字符串完全一致时
    {
        if(str==L[i].address) //如果两个地址完全一样则直接输出原来得返回值
        {
            judge=1;
            sumstr=L[i].Value;
            break;
        }
        int x=judgeXiangdeng(L[i].address,str); //判断前面的取值是不是一样

        if(x==-1)   //不一样直接到下一个.一样继续进去找
        {
            continue;
        }

        else
        {
            if(L[i].address[x+1]!='<')  //如果不是包含参数肯定已经不一样了,直接返回
            {
                continue;   
            }

            if(L[i].address[x+2]=='p')  //将path的也截取出来
            {
                judge=1;
                string temp1=str.substr(x+1);   //将后面的字符串截取出来
                if(judgeHeFa(temp1))
                {
                    sumstr=L[i].Value;
                    sumstr+=" ";
                    sumstr+=temp1;
                    break;
                }
            }

            else    //将<int><str>类型的也截取出来
            {
                string linshi1=L[i].address.substr(x+1);
                string linshi2=str.substr(x+1);
                if(judgeisPipei(linshi1,linshi2))
                {
                    judge=1;
                    sumstr=L[i].Value;
                    sumstr+=" ";
                    int js=count(linshi1.begin(),linshi1.end(),'/');
                    int x1=0,x2=0;  //x1和x2是用来测这个字符串范围
                    for(int k=0;k<js;k++)
                    {
                        x1=linshi2.find('/',x2+1);
                        if(k==0)
                        {
                            for(int h=x2;h<x1;h++)
                            {
                                if(linshi2[h]=='0'&&h==x2)
                                {
                                    continue;
                                }                               
                                sumstr+=linshi2[h];                                                             
                            }
                        }
                        else
                        {
                            for(int h=x2+1;h<x1;h++)
                            {
                                if(linshi2[h]=='0'&&h==x2+1)
                                {
                                    continue;
                                }                        
                                sumstr+=linshi2[h];                                                             
                            }
                        }        
                        if(k!=js-1)
                        {
                            sumstr+=" ";
                        }                                    
                        x2=x1;
                    }
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
    }
    if(judge==0)    //没有满足规则的直接返回404
    {
        sumstr="404";
    }
    return sumstr;

}

bool judgeHeFa(string str) //检测这个输入是否合法
{
    int length=str.length();  
    int count=0;  
    for(int i=0;i<length;i++)
    {
        if((str[i]>=45&&str[i]<=57)||(str[i]>=65&&str[i]<=90)||str[i]==95||(str[i]>=97&&str[i]<=122))
        {
            count++;
        }
        else
        {
            return false;
        }
    }
    return true;
}

int judgeXiangdeng(string str1,string str2)   //判断两个前面的地址是不是相等,如果值是-1则不相等
{
    int sum=-1;
    int js=0;
    for(int i=0;i<str2.length();i++)
    {
        if(str1[i]==str2[i]&&str1[i]=='/')  //判断最开始的地址是不是一样的
        {
            js++;
            if(js==2)
            {
                sum=i;
                return sum;
            }
        }
        if(str1[i]!=str2[i])    //二者不相同
        {
            break;
        }
    }
    return sum;
}

bool judgeisPipei(string str1,string str2) //判断是不是满足匹配
{
    int js1=count(str1.begin(),str1.end(),'/'); //有几个js1就是有几个判断要求
    int js2=count(str2.begin(),str2.end(),'/');
    //cout<<js1<<" "<<js2<<endl;
    if(js1!=js2)    //将这两个中的/进行比较如果个数一样则可能满足规则,如果个数不一样则不满足规则
    {
        return false;
    }
    if(!judgeHeFa(str2))    //如果不合法则仍然返回错误
    {
        return false;
    }
    int x1=0,x2=0;  //x是用来划字符串范围的
    int y1=0;   //y用来化规则范围
    for(int i=0;i<js1;i++)  //根据/来设置的循环总设置
    {
        x1=str2.find('/',x2+1);
        if(i!=0)
        {
            y1=str1.find('/',y1+1);
        }       
        if(i==0)
        {
            if(str1[1]=='i')    //如果这个映射是int型
            {
                for(int j=0;j<x1;j++)
                {
                    if(str2[j]<48||str2[j]>57)  //不满足纯数字要求
                    {
                        return false;
                    }
                    //cout<<"1"<<endl;
                }
            }
            x2=x1;
        }
        else
        {
            if(str1[y1+2]=='i')
            {
                for(int j=x2+1;j<x1;j++)
                {
                    if(str2[j]<48||str2[j]>57)  //不满足纯数字要求
                    {
                        return false;
                    }
                }
            }
            x2=x1;
        }
    }
    return true;    //全部没有问题则返回true
}
