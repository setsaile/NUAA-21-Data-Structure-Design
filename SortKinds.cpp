#include<iostream>
#include<fstream>
#include<time.h>
#include<queue> 
using namespace std;
//八大排序算法时间花费比较
void InsertSort(int *L);    //直接插入排序
void ShellSort(int *L); //希尔排序
void MaopaoSort(int *L);    //冒泡排序
void Quicksort(int *L); //快速排序
int Partition(int p[],int low,int high);    //进行一趟快排
void Qsort(int p[],int low,int high);    //快排的递归

void SelectSort(int *L);    //选择排序

void THeapSort(int *L);  //堆排序
void Heapadjust(int *p,int low,int high);   //堆调整的算法
void HeapSort(int *p,int high);  //堆排序总算法

void GuibingSort(int *L);   //归并排序算法
void sortArr(int arr[],int low,int hight);    //拆分过程 
void mergeArr(int arr[],int low,int mid,int hight);  //合并和排序过程

void RadixSort(int *L); //基数排序

int main()
{
    fstream file;
    file.open("C:\\Codefield\\CODE_C\\CDesign\\KindsSort\\NumZxu.txt",ios::in);
    int arc[50001];
    int js=1;
    while(!file.eof())  //将这些数字直接读到数组里面去存放
    {
        file>>arc[js];  //
        js++;
    }
    cout<<"直接插入排序所用的时间："<<endl;     //1
    clock_t start=clock();  //程序开始的时间
    InsertSort(arc);
    clock_t end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    cout<<"希尔排序所用的时间："<<endl;     //2         
    start=clock();  //程序开始的时间
    ShellSort(arc);
    end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    cout<<"冒泡排序所用的时间："<<endl;     //3
    start=clock();  //程序开始的时间
    MaopaoSort(arc);
    end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    cout<<"快速排序所用的时间："<<endl;     //4
    start=clock();  //程序开始的时间
    Quicksort(arc);
    end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    cout<<"选择排序所用的时间："<<endl;     //5
    start=clock();  //程序开始的时间
    SelectSort(arc);
    end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    cout<<"堆排序所用的时间："<<endl;       //6
    start=clock();  //程序开始的时间
    THeapSort(arc);
    end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    cout<<"归并排序所用的时间："<<endl;     //7
    start=clock();  //程序开始的时间
    GuibingSort(arc);
    end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    cout<<"基数排序所用的时间："<<endl;     //8
    start=clock();  //程序开始的时间
    RadixSort(arc);
    end=clock();    //程序结束的时间
    cout<<"运行时间："<<(double)(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    cout<<endl; //CLOCKS_PER_SEC是指的一分钟"滴答几次",除以后得到的是秒数!!!

    file.close();
    return 0;
} 

void InsertSort(int *L)    //直接插入排序
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    }
    for(int i=2;i<=50000;i++)   //从第二个开始变成有序的数组
    {
        p[0]=p[i];  //0作为哨兵去储存临时数据
        int j;
        for(j=i;j>1;j--)    //j是记录这个插入值该放在哪里
        {
            if(p[0]<p[j-1]) //更小则向前面移动
            {
                p[j]=p[j-1];
            }
            else
            {
                break;
            }
        }
        p[j]=p[0];
    }
    fstream file;
    file.open("KindsSort\\Insert.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();
}

void ShellSort(int *L) //希尔排序
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    }    
    int k;
    for(k=12501;k>=1;k=k-500)
    {       
        int i;
        for(i=1+k;i<=50000;i++)
        {
            if(p[i]<p[i-k]) //比之前的小则需要进行插入排序
            {
                p[0]=p[i];
                int j;
                for(j=i;j>k;j=j-k)
                {
                    if(p[0]<p[j-k])
                    {
                        p[j]=p[j-k];
                    }
                    else 
                    {
                        break;
                    }
                }
                p[j]=p[0];  //此时的j位置就是p[0]该放置的位置!!!
            }
        }
    }
    fstream file;
    file.open("KindsSort\\Shell.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();
}

void MaopaoSort(int *L)    //冒泡排序
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    }   
    for(int i=1;i<50000;i++)  //冒泡排序核心
    {
        int temp=0;
        for(int j=i+1;j<=50000;j++)
        {
            if(p[i]>p[j])
            {
                temp=p[i];
                p[i]=p[j];
                p[j]=temp;
            }
        }
    }
    fstream file;
    file.open("KindsSort\\Maopao.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();
}

void SelectSort(int *L)    //选择排序
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    }  
    for(int i=1;i<=50000;i++)  //选择排序的核心
    {
        int k=i;
        int temp=0;
        for(int j=i+1;j<=50000;j++)
        {
            if(p[k]>p[j]) 
            {
                k=j;    
            }
        }
        if(i!=k)
        {
            temp=p[i];
            p[i]=p[k];
            p[k]=temp;
        }
    }
    fstream file;
    file.open("KindsSort\\Select.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();
}

void Quicksort(int *L) //快速排序
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    } 
    Qsort(p,1,50000);
    fstream file;
    file.open("KindsSort\\Qsort.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();
}
int Partition(int p[],int low,int high)    //进行一趟快排
{
    p[0]=p[low];    //0是作为临时存放点
    while(low<high)
    {
        while(low<high&&p[high]>=p[0])
        {
            high--;
        }
        p[low]=p[high]; //将这个关键点用这个值顶替掉,low此时是可以被占用的
        while(low<high&&p[low]<=p[0])
        {
            low++;
        }
        p[high]=p[low]; //high这个值之前已经存走了,所以此时是可以被占用的
    }
    p[low]=p[0];
    return low;
}
void Qsort(int p[],int low,int high)    //快排的递归
{
    int linshi;   
    if(low<high)    //对整个进行递归排序
    {
        linshi=Partition(p,low,high);  
        Qsort(p,low,linshi-1);
        Qsort(p,linshi+1,high);
    }
}

void THeapSort(int *L)  //堆排序
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    } 
    HeapSort(p,50000);
    fstream file;
    file.open("KindsSort\\HeapSort.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();

}
void Heapadjust(int *p,int low,int high)   //堆调整的算法
{
    p[0]=p[low];    //从上向下开始筛选
    int i=low;
    int j=2*i;
    while(j<=high)  //向下面开始进行筛选
    {
        if(j+1<=high&&p[j+1]<p[j])  //选择左后子树中更小的部分
        {
            j++;
        }
        //始终是将最开始的根节点去比较为了找到最合适的位置
        if(p[0]<p[j])   //已经是小顶堆
        {
            break;
        }
        else    //不是小顶堆就将小的值赋值给这个根节点,然后继续向下面进行调整
        {
            p[i]=p[j];
            i=j;    //将这个值与更小的值进行交换调整
            j=i*2;  //继续向下面开始判断是否满足小顶堆
        }
    }
    p[i]=p[0];
}
void HeapSort(int *p,int high)  //堆排序总算法
{
    for(int i=high/2;i>0;i--)  //从这个堆的最下面开始依次向上进行堆调整
    {
        Heapadjust(p,i,high);   //堆顶就是现在这个顶点,堆尾始终是末尾
    }
    //到上面这步只是构建了一个小顶堆,数组还不是从小到大的输出,还需要排个序

    for(int i=high;i>1;i--) //将这小顶堆的数据从数组末尾从小到大进行了存储
    {
        p[0]=p[1];
        p[1]=p[i];  //将堆顶用最后一个数字替换
        p[i]=p[0];  //将对顶放在最后一个数字
        Heapadjust(p,1,i-1);
    }
}

void GuibingSort(int *L)   //归并排序算法
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    } 
    sortArr(p,1,50000);
    fstream file;
    file.open("KindsSort\\sorrArr.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();
}
void sortArr(int arr[],int low,int hight)    //拆分过程 
{
    if(low==hight)
    {
        arr[low]=arr[hight];
    }
	else
    {
		int mid=(hight+low)/2;
		sortArr(arr,low,mid);   //先拆解前半部分
		sortArr(arr,mid+1,hight);   //再拆解后半部分
		mergeArr(arr,low,mid,hight); //将这部分的值排列成为新有序数列
	}
}
void mergeArr(int arr[],int low,int mid,int hight)  //合并和排序过程
{
    int*p=new int[hight-low+1]; //用辅助数组进行合并排序,将合并后的有序列在原数组中及时更新!!!
	int i=low;  //low不一定是0,即为一个开始的位置
    int j=mid+1;    //第二个数组开始的位置
    int k=0;    //辅助数组从0开始计数
	while (i <= mid && j <= hight) 
    {
		if (arr[i]<=arr[j])     //先取小的值
        {
			p[k]=arr[i];
			i++;
		}
		else 
        {
			p[k]=arr[j];
			j++;
		}
		k++;    //一轮以后辅助数组下标也要向右移动一位
	}
	while (i<=mid)    //剩下的都是大的直接赋值到最后面
    {
		p[k]=arr[i];
		i++;
		k++;
	}
	while (j<=hight) //剩下的都是大的直接赋值到最后面
    {
		p[k]=arr[j];
		j++;
		k++;
	}
	i=low;   
	for (int num=0;num<k;num++) //将临时数组排好的顺序在原数组之中及时更新!!! 
    {
		arr[i]=p[num];
		i++;
	}
	delete[] p;
}

void RadixSort(int *L) //基数排序
{
    int p[50001];  //赋值在这个新数组中避免原来数据被覆盖
    for(int i=1;i<=50000;i++)
    {
        p[i]=L[i];  
    } 
    queue<int> Duilie[10];  //创建一个队列数组,将本位置值存入相同大小的下标对应的队列中!!!
    int radix=1;
    for(int k=1;k<=5;k++)   //最高位是万位,开始进行计数排序
    {
        radix=radix*10;
        for(int i=1;i<=50000;i++)   //每个数进入相应下标所对应的队列
        {
            int x;
            x=(p[i]%radix)/(radix/10);   //确保能取到特定位置的值
            Duilie[x].push(p[i]);   //将这个值入队
        }

        for(int i=1,m=0;m<10;m++)   //一轮排序之后按顺序进行收集!!!
        {
            while(!Duilie[m].empty())   //队列不为空就继续存值
            {
                p[i]=Duilie[m].front();
                Duilie[m].pop();
                i++;
            }
        }
    }
    fstream file;
    file.open("KindsSort\\Radixsort.txt",ios::out);
    for(int i=1;i<=50000;i++)
    {
        file<<p[i]<<endl;
    }
    file.close();
}
