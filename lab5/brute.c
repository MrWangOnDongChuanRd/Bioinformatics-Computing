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