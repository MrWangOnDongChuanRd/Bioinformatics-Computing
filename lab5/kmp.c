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