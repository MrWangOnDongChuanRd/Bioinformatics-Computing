# 上机实验5：字符串匹配算法与GDB进阶

- 学号：520111910178
- 姓名：王宇尘

[TOC]



## 一、学习目的

- 掌握GDB设置断点与调试程序的技巧
- 熟悉二维字符数组与字符指针数组的区别和联系
- 掌握字符串模式匹配的暴力算法和KMP算法



## 二、上机实验内容

### 1. 理清字符二维数组的使用

- 注意使用`strcpy`写字符数组的时候，如果目标字符数组的长度不足，有溢出的问题，这是`strcpy()`函数的bug。

```c
[bio@bioinfo lab5]$ gdb -q charr
Reading symbols from /home/bio/course/bi290/lab5/charr...done.
(gdb) list
1       #include <stdio.h>
2       #include <stdlib.h>
3       #include <string.h>
4
5       int main()
6       {
7               char ch[5][10];
8
9               char buff[20];
10              int i;
11              for (i=0; i<5; i++){
12                      scanf("%s", buff);
13                      strcpy(ch[i], buff);
14              }
15
16              for (i=0; i<5; i++)
17                      printf("%s\n", ch[i]);
18
19              return 0;
20      }
(gdb) break 11
Breakpoint 1 at 0x4005d5: file charr.c, line 11.
(gdb) break 13
Breakpoint 2 at 0x4005f4: file charr.c, line 13.
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000004005d5 in main at charr.c:11
2       breakpoint     keep y   0x00000000004005f4 in main at charr.c:13
(gdb) run
Starting program: /home/bio/course/bi290/lab5/charr
Breakpoint 1, main () at charr.c:11
11              for (i=0; i<5; i++){
(gdb) p ch[0]
$1 = "\001\000\000\000\000\000\000\000\275\006"
(gdb) p ch[1]
$2 = "@\000\000\000\000\000\300\305\377\377"
(gdb) p ch[2]
$3 = "\377\177\000\000\000\000\000\000\000"
(gdb) p ch[3]
$4 = "\000\000p\006@\000\000\000\000"
(gdb) p ch[4]
$5 = "\340\004@\000\000\000\000\000\240", <incomplete sequence \306>
(gdb) c
Continuing.
CCATGGACTTTACG

Breakpoint 2, main () at charr.c:13
13                      strcpy(ch[i], buff);
(gdb) n
11              for (i=0; i<5; i++){
(gdb) p ch[0]
$6 = "CCATGGACTT"
(gdb) p ch[1]
$7 = "TACG\000\000\300\305\377\377"
(gdb) n
12                      scanf("%s", buff);
(gdb) n
AACAGGATTACG 

Breakpoint 2, main () at charr.c:13
13                      strcpy(ch[i], buff);
(gdb) n
11              for (i=0; i<5; i++){
(gdb) p ch[1]
$8 = "AACAGGATTA"
(gdb) p ch[2]
$9 = "CG\000\000\000\000\000\000\000"
(gdb) n
12                      scanf("%s", buff);
(gdb) n
ACCAG

Breakpoint 2, main () at charr.c:13
13                      strcpy(ch[i], buff);
(gdb) n
11              for (i=0; i<5; i++){
(gdb) p ch[2]
$10 = "ACCAG\000\000\000\000"
(gdb) n
12                      scanf("%s", buff);
(gdb) n
ACAAGATT

Breakpoint 2, main () at charr.c:13
13                      strcpy(ch[i], buff);
(gdb) n
11              for (i=0; i<5; i++){
(gdb) p ch[3]
$11 = "ACAAGATT\000"
(gdb) n
12                      scanf("%s", buff);
(gdb) n
GGACTTACTTAT

Breakpoint 2, main () at charr.c:13
13                      strcpy(ch[i], buff);
(gdb) n
11              for (i=0; i<5; i++){
(gdb) p ch[4]
$12 = "GGACTTACTT"
(gdb) p ch[5]
$13 = "AT\000\177\000\000\000\000\000"
(gdb) p printf("%s\n", ch[0])
CCATGGACTTAACAGGATTAACCAG
$14 = 26
(gdb) p printf("%s\n", ch[1])
AACAGGATTAACCAG
$15 = 16
(gdb) p printf("%s\n", ch[2])
ACCAG
$16 = 6
(gdb) p printf("%s\n", ch[3])
ACAAGATT
$17 = 9
(gdb) p printf("%s\n", ch[4])
GGACTTACTTAT
$18 = 13
(gdb) p printf("%s\n", ch[5])
AT
$19 = 3
```



### 2. 字符串的匹配算法

> 有一个字符串$S$和一个模式字符串$P$，如何查找$P$在$S$中的位置？

#### 2.1 暴力匹配算法

假设当前字符串$S$匹配到了$i$位置，模式串$P$匹配到$j$位置，分为两种情况：

- 如果当前字符匹配成功，也就是`S[i]=P[j]​`，则`i++, j++`，继续匹配下一个字符。

- 如果匹配失败，也就是`S[i]!=P[j]`，则`i-=(j-1), j=0`。也就是将`i`回溯，`j`置零。

针对上述暴力匹配算法，

1. 写出实现算法的C程序，并简单分析算法复杂度。

2. 编写C程序，生成多个随机DNA序列，保存到FASTA文件中。用一个模式序列测试上述算法，在文件中搜索该模式，保存各序列的模式匹配位置（-1表示没有找到匹配）：

   ```
   seq1: -1
   seq2: 23
   seq3: 3
   ...
   ```

   其中`seq1`为FASTA文件中序列的名称，而`23`则表示匹配到序列的23位置（坐标从0开始）。

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <time.h>
   #define LINE 80

   int generate(char S[])
   {
       int n,j=0;
       FILE *fp;
       fp = fopen("fasta","w+");
       if(fp!=NULL)
       {
           printf("The fasta file is established successfully\n");
       }
       if(fputs(">gene_name | ", fp) <0 || fputs("\n", fp) < 0 )     
       {
           printf("Fail to write\n");
       }
       printf("How many random sequence do you want?\n");
       scanf("%d",&n);
       srand((unsigned)time(NULL));
       for(int i = 0;i < n;i++)
       {
           for(int i = 0;i < LINE;i++)
           {
               switch(rand()%4)
               {
                   case 0:
                       fputs("A",fp);
                       S[j] = 'A';
                       break;
                   case 1:
                       fputs("T",fp);
                       S[j] = 'T';
                       break;
                   case 2:
                       fputs("G",fp);
                       S[j] = 'G';
                       break;
                   case 3:
                       fputs("C",fp);
                       S[j] = 'C';
                       break;
               }
               j++;
           }
           fputs("\n",fp);
       }
       printf("The file has been created successfully!\n");
       fclose(fp);
       return n;
   }

   void brute(int num,char S[])
   {
       char SS[100] = {0};
       char P[100]={0};
       printf("Please input the pattern string:\n");
       scanf("%s",P);
       int m = strlen(P);
       int a,i,j,i2,flag;
       for(int k = 0;k < num;k++)
       {
           for(a = 0;a < LINE;a++)
           {
               SS[a]=S[a+k*LINE];
           }
           int n = strlen(SS);
           for(i=0;i<n-2;i++)
           {
               flag = 0;
               j = 0;
               if(P[0]==SS[i])
               {
                   j++;
                   flag=1;
                   i2=i+1;
                   for(;j<m;j++)
                   {
                       if(P[j]!=SS[i2])
                       {
                           flag = 0;
                           break;
                       }
                       i2++;
                   }
               }
               if(flag == 1)
               break;
           }
           if(flag == 1)
           {
               printf("seq %d: %d\n",k+1,i);
           }
           else
           {
               printf("seq %d: -1\n",k+1);
           }
       }
   }

   int main()
   {
       char S[2000]={0};
       brute(generate(S),S);
       return 0;
   }
   ```

   ​



#### 2.2 字符串匹配的KMP算法

按照课上讲述的内容，尝试用C实现KMP算法，同样用上述的随机序列文件进行测试。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LINE 80

int generate(char S[])
{
    int n,j=0;
    FILE *fp;
    fp = fopen("fasta","w+");
    if(fp!=NULL)
    {
        printf("The fasta file is established successfully\n");
    }
    if(fputs(">gene_name | ", fp) <0 || fputs("\n", fp) < 0 )     
    {
        printf("Fail to write\n");
    }
    printf("How many random sequence do you want?\n");
    scanf("%d",&n);
    srand((unsigned)time(NULL));
    for(int i = 0;i < n;i++)
    {
        for(int i = 0;i < LINE;i++)
        {
            switch(rand()%4)
            {
                case 0:
                    fputs("A",fp);
                    S[j] = 'A';
                    break;
                case 1:
                    fputs("T",fp);
                    S[j] = 'T';
                    break;
                case 2:
                    fputs("G",fp);
                    S[j] = 'G';
                    break;
                case 3:
                    fputs("C",fp);
                    S[j] = 'C';
                    break;
            }
            j++;
        }
        fputs("\n",fp);
    }
    printf("The file has been created successfully!\n");
    fclose(fp);
    return n;
}

void kmp(int num,char S[])
{
    char F[100] = {0};
    char SS[100] = {0};
    char P[100] = {0};
    printf("Please input the pattern string:\n");
    scanf("%s",P);
    int m = strlen(P);
    int i=1,j=0;
    while(i < m)
    {
        if(P[i] == P[j])
        {
            F[i] = j + 1;
            i++;
            j++;
        }
        else if (j > 0)
        {    
            j = F[j-1];
        }
        else
        {   
            F[i]=0;
            i++;
        }
    } 
    for(int k = 0;k < num;k++)
    {
        for(int i = 0;i < LINE;i++)
        {
            SS[i]=S[i+k*LINE];
        }
        int n,res = -1;
        n = strlen(SS);    
        i=j=0;
        while(i < n)
        {
            if(P[j] == SS[i])
            {
                if(j == m-1)
                {    
                    res = i+1-m;
                }
                    i++;
                    j++;
            }
            else if(j>0)
                j = F[j-1];
            else
                i++;
        }
        if(res < 0)
        {
            printf("seq %d: -1\n",k+1);
        }
        else
        printf("seq %d: %d\n",k+1,res);
    }
}

int main()
{
    char S[2000]={0};
    kmp(generate(S),S);
    return 0;
}
```



## 三、上机实验总结

​	暴力算法虽然想起来容易，但是效率却比较低，遇上数据量大的文件会显得稍显吃力。而KMP算法则设计的很巧妙，通过构造F表来减少大量不必要的回溯，运行速度比较快

​	在写代码的过程中，我复习了文件操作的内容，并且自学了`rand()`和`srand（）`函数的用法，对KMP算法有了更深入的理解