#include<iostream>
using namespace std;
#include<fstream> 
#include<stdlib.h>
#include<string.h>
#define N  58
#define MAX 10000
#define M 30
#define MAXSIZE 2000

struct Hfmnode  //哈夫曼树的构建
{//权重，父亲孩子序号，存储的字符 
	int weight; 
	int parent,lchild,rchild;
	char c;     //这个叶子节点储存的字符
};
//霍夫曼编码
struct Hfmcode 
{
	char* bit ; 
};

//遍历文件,统计文件中各个字符出现的次数
void get_ch_count(int*&counts) 
{//counts:存储字符出现的次数 
	counts=new int[N];//申请空间
	if(!counts)
	{
		cout<<"申请失败！"<<endl;
		exit(0);
	} 
	for(int i=0;i<N;i++)//计数初始化 
	{
		counts[i]=0;
	}
	fstream file;
	char ch;
	file.open("Huffman\\source.txt",ios::in);
	if(!file)
	{
		cout<<"文件打开失败！"<<endl;
		exit(0);
	}
	while(!file.eof())//从文件中读数据，计算其出现的次数 
	{
		file.get(ch);//get()读取，可以读取空白字符 
		if(ch<='z'&&ch>='a')//[0,25]：[a,z] 
		{
			counts[ch-'a']++;
		}
		else if(ch<='Z'&&ch>='A')//[26,51]:[A,Z]
		{
			counts[ch-'A'+26]++;
		}
		else//统计标点符号  
		{
			switch(ch)//[52,57]之内的字符的量的多少
			{
				case ' ':counts[52]++;break;
				case ',':counts[53]++;break;
				case '.':counts[54]++;break;
				case '\n':counts[55]++;break;
				case '(':counts[56]++;break;
				case ')':counts[57]++;break;	
			}
		} 
	}
	file.close();//关闭文件 
} 

//通过数组的方式构建哈夫曼树 
void HufmanTree(Hfmnode*&h,int*counts)
{//h:哈夫曼树节点数组，counts:每个叶子节点出现次数 
	h=new Hfmnode[N*2-1];//建立2*N-1个哈夫曼树节点
	if(!h)
	{
		cout<<"空间申请失败！"<<endl;
		exit(0); 
	} 
	for(int i=0;i<2*N-1;i++)//给每个节点初始化 
	{
		h[i].weight=0;
		h[i].parent=-1;
		h[i].lchild=-1;
		h[i].rchild=-1;
		h[i].c='\0';
	}
	for(int i=0;i<N;i++)//初始化霍夫曼树的叶子节点 
	{//根据序号赋值
		if(i<26)   
			h[i].c = i + 'a';//[a,z]
		else if(i<52&&i>=26)
			h[i].c = i + 'A'-26;//[A,Z]
		else
		{
			switch(i)//标点符号以及空白字符 
			{
				case 52:h[i].c=' ';break;
				case 53:h[i].c=',';break;
				case 54:h[i].c='.';break;
				case 55:h[i].c='\n';break;
				case 56:h[i].c='(';break;
				case 57:h[i].c=')';break;
			}
		}
		h[i].weight=counts[i];//每个叶子节点的权重实际上是在文档中出现的次数 
	}
	for(int i=0;i<N-1;i++)//有N-1个非叶子节点 
	{
		int min1,min2,x1,x2; 
		x1=x2=-1;
		min1=min2=MAX;
		//min1,min2表示前N+i个节点权重最小的两个数
		//x1,x2表示最小权重两个节点的编号 
		//min1<min2<others| h[x1].weight<h[x2].weight
		//获取N+i个节点里权重最小的两个节点 
		for(int j=0;j<N+i;j++)
		{ 
			if(h[j].weight<min1 && h[j].parent==-1)
			{//parent=-1还没有成为树枝或叶子 
				min2=min1;
				x2=x1;
				min1=h[j].weight;
				x1=j;
			}
			else if(h[j].weight<min2&&h[j].parent==-1)
			{
				
				min2=h[j].weight;
				x2=j;
			}
		}
		//根据每个节点信息, 将其信息存储到节点数组中
		//确定父亲孩子关系，计算父节点权重 
		h[x1].parent=N+i; 
		h[x2].parent=N+i; 
		h[N+i].lchild=x1;
		h[N+i].rchild=x2;
		h[N+i].weight=h[x1].weight+h[x2].weight;
	}
}
//哈夫曼编码，根据叶子节点的位置, 将其路径0，1数字填充到编码数组中
void  HuffmandeCode(Hfmnode*h, int* counts, Hfmcode*& hcode)
{//h:哈夫曼树节点数组，counts:每个叶子节点的权重 
 //hcode:用来存储哈夫曼编码结构体数组 
	int i,j,t,parent;
	char codestr[M]={'\0'};//暂存数组赋初值 
	hcode=new Hfmcode[N];//申请空间 
	if(!hcode)
	{
		cout<<"空间申请失败！"<<endl;
		exit(0);
	}
	for(i=0;i<N;i++)//遍历叶子节点 
	{
		j=0;
		parent=h[i].parent;//记录当前节点的父亲
		t=i; 
		while(1)//前一个parent已经为根节点 
		{ 
			if(h[parent].lchild==t)//根据路径左右从下到上存储0，1 
				codestr[j]='0';
			else
				codestr[j]='1';
			j++;
			t=parent;
			if(h[t].parent==-1)//已经到达根节点 
				break; 
			parent=h[parent].parent;
		}
		hcode[i].bit=new char[j+1];//申请j+1个空间存储第i个叶子节点信息 
		hcode[i].bit[j]='\0';//末尾赋\0 
		//codestr从叶子节点到根节点与编码顺序相反，逆序存储
		for(int k=0;k<j;k++)  
		{
			hcode[i].bit[k]=codestr[j-k-1];
		}
	}
	fstream outfile;
	outfile.open("Huffman\\Huffman.txt", ios::out);
	if(!outfile)
	{
		cout<<"文件打开失败！"<<endl;
		exit(0);
	}
	for(i=0;i<N;i++)//存入文件并输出 
	{
		if(i==52)//空格 
		{
			cout<<"\\40";//输出到屏幕 
			outfile<<"\\40:\t";//输出到文件 
		}
		else if(i==55)//换行符 
		{
			cout<<"\\n";
			outfile<<"\\n:\t";
		}
		else//其他符号 
		{
			cout<<h[i].c;
			outfile<<h[i].c<<":\t";
		}
		cout<<"\t:"<<"出现"<<counts[i]<<"次\t"<<"编码为："; 
		cout<<hcode[i].bit;//输出编码 
		outfile<<hcode[i].bit<<"     "<<counts[i]<<endl;
		cout<<endl;
	}
	outfile.close(); 
}
//译码 
void transcode(char*s,Hfmcode*hcode)//string->code 
{// s:字符串，hcode编码表 
	int index;//对用编码索引 
	cout<<"\n"<<s<<" 对应的哈夫曼编码为："<<endl; 
	for(int i=0;i<strlen(s);i++)//遍历每一个字符,计算索引 
	{
		if(s[i]<='z'&&s[i]>='a')
			index=s[i]-'a';
		else if(s[i]<='Z'&&s[i]>='A')
			index=s[i]-'A'+26;
		else
		{
			switch(s[i])
			{
				case ' ':index=52;break;
				case ',':index=53;break;
				case '.':index=54;break;
				case '\n':index=55;break;
				case '(':index=56;break;
				case ')':index=57;break;
			}	
		} 
		cout<<hcode[index].bit;//逐个字符输出霍夫曼编码 
	}
	cout<<endl;
}
//对文件前n段进行编码 
void traslate(Hfmcode*hcode)
{//hcode:霍夫曼编码表 
	fstream file,outfile,Datfile;
	char ch;
	cout<<"编码整个文章："<<endl;
	outfile.open("Huffman\\code2.txt",ios::out);
	if(!outfile)
	{
		cout<<"文件打开失败！"<<endl;
		exit(0); 
	}
    Datfile.open("Huffman\\code.dat",ios::out);
    if(!Datfile)
    {
        cout<<"打开文件失败！"<<endl;
        exit(0);
    }
	file.open("Huffman\\source.txt",ios::in);
	if(!file)
	{
		cout<<"文件打开失败！"<<endl;
		exit(0); 
	}
	char cch[8];
	int index;//对应编码索引，计数器 
	while(!file.eof())
	{
		file.get(ch);//逐个读取字符，包括空白字符 ，然后计算索引 
        if(file.fail())
        {
            break;
        }
		if(ch<='z'&&ch>='a')
			index=ch-'a';
		else if(ch<='Z'&&ch>='A')
			index=ch-'A'+26;
		else
		{
			switch(ch)
			{
				case ' ':index=52;break;
				case ',':index=53;break;
				case '.':index=54;break;
				case '\n':index=55;break;
				case '(':index=56;break;
				case ')':index=57;break;
			}	
		} 
		Datfile.write((char*)hcode[index].bit,sizeof(hcode[index].bit));//将编码结果储存到二进制文件
		outfile<<hcode[index].bit;//写入文件 
		cout<<hcode[index].bit;//输出到显示器 
	}
	cout<<endl; 
	file.close();
	outfile.close(); 

}
//哈夫曼译码
void  HuffmanenCode(Hfmnode* h)//code->string 
{//h霍夫曼树数组 
	int child=N*2-2;//2*N-2是根节点的序号
    char ch;
	fstream file;
	file.open("Huffman\\code2.txt",ios::in);
	if(!file)
	{
		cout<<"文件打开失败！"<<endl;
		exit(0);
	}
	cout<<"解码整个文章："<<endl;
	while(!file.eof())
	{
		file>>ch; //在文件中逐字符读取0，1 ，从头向下找直到叶子节点 
        if(file.fail())
        {
            break;
        }
		if(ch=='0')//0左1右
			child=h[child].lchild;
		else if(ch=='1')
			child=h[child].rchild;
		if(h[child].lchild==-1 && h[child].rchild==-1)//叶子节点 
		{
			cout<<h[child].c;//查找的对应的叶子节点 
			child=N*2-2;//重新回到根节点
		}
		
	}
	file.close();
}
int main()
{
    cout<<"         Huffman树编码与解码的展示"<<endl;
	int* counts;//字符次数 
	//char s[]="Data Structure";
	Hfmnode*h;//霍夫曼树 
	Hfmcode* hcode;//霍夫曼编码
	 
	get_ch_count(counts);//统计字符次数 
	HufmanTree(h,counts);//构建哈夫曼树 
	HuffmandeCode(h,counts,hcode);//哈夫曼编码 
	
	traslate(hcode);//编码全文 
	HuffmanenCode(h);//解码全文
	
	delete counts;//释放空间 
	delete hcode;
	delete h; 
	
	return 0;
}

