/*
    dnatranspro.c
    Function TRANS translates the DNA sequence into the amino acid sequence.
    Function EXTRACT extracts the information from the *.gb file.
    Function CONVERT generates the FASTA file.
    author:Wang Chaoxian(TRANS), Wang Yuchen(CONVERT,EXTRACT), Pang Chongchen(CONVERT,EXTRACT), Zhu Xiangyu(TRANS)
    date:2022.3.28
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trans(char a[])
{
    char base1[64], base2[64], base3[64], pro[64];          // define the data structure of the triplet codon table
    strcpy(base1, "TTTTTTTTTTTTTTTTCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAGGGGGGGGGGGGGGGG");      // the first base
    strcpy(base2, "TTTTCCCCAAAAGGGGTTTTCCCCAAAAGGGGTTTTCCCCAAAAGGGGTTTTCCCCAAAAGGGG");      // the second base
    strcpy(base3, "TCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAGTCAG");      // the third base
    strcpy(pro, "FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG");        // amino acid
    int len = strlen(a);
    int i, j;
    int flag = -1;
    for(i = 0; i < len; i++)
    {
        if(flag == -1)                                   // find the beginning of the translation
        {
            if((a[i] == 'a' || a[i] == 'A') && (a[i + 1] == 't' || a[i + 1] == 'T') && (a[i + 2] == 'g' || a[i + 2] == 'G'))
            {
                flag = i;
            }
        }
        if(a[i] >= 'a' && a[i] <= 'z')                   // change case
        {
            a[i] = a[i] - 32;
        }

    }
    if(flag != -1)
    {
        for(i = flag; i < len; i += 3)                      // start the translation
        {
            int ed = 0;
            for(j = 0; j < 64; j++)
            {
                if(a[i] == base1[j])                       // find the aiming triplet codon
                {
                    if(a[i + 1] == base2[j] && a[i + 2] == base3[j])
                    {
                        if(pro[j] != '*')
                        {
                            printf("%c", pro[j]);          // translate it
                        }
                        else
                            ed = 1;                        // the end of translation
                        break;
                    }
                }
            }
            if(ed)
                break;                                    // finish the translation
        }
    }
}
void convert(char a[], char l[], char b[] )
{
    printf("There is the string of amino acid:\n");
    trans(b);
    printf("\n");
    FILE *fp;       //define the file pointer
    char path[100] = {0};
    strcpy(path, a);
    strcat(path, ".fa");
    fp = fopen(path,"w+");      //create a new file
    if(fp!=NULL)        //make sure the file is successfully made
    {
        printf("The fasta file is established successfully.\n");
    }
    int len = strlen(b);
    if(fputs(">gb | ", fp) <0 || fputs(a, fp) < 0 || fputs(" | ", fp) < 0 || fputs(l, fp) < 0)      //write the information neccessary into the fasta file
    {
        printf("Fail to write.\n");
        fclose(fp);
        exit(0);
    }
    int i;
    for(i = 0; i < len; i++)        //write the DNA sequence into the fasta file
    {
        fprintf(fp, "%c", b[i]);
        if((i + 1) % 60 == 0)       //60 bases in a row
        {
            fprintf(fp, "\n");
        }
    }
    fprintf(fp, "\n");
    fclose(fp);     //close the file
}
void extract()
{
    FILE *fp;
    printf("Please input a filename and its path:\n");      //access the *.gb file
    char path[100] = {0};
    scanf("%s",path);
    fp = fopen(path, "r");      //open the file
    if(fp!=NULL)        //make sure the file is successfully opened
    {
        printf("File oepn successfully\n");
    }
    int k = 0;
    int m = 0;
    char s[800]={0};        //the s array stores the information temporally
    char b[100]={0};        //the b array stores ACCESSION
    char c[800]={0};        //the c array stores the DNA sequence
    char l[100]={0};        //the l array stores LOCUS
    int i, j;
    fgets(s, 200, fp);
    for(i = 0;i <= 100;i++)     //get ACCESSION
    {
        if(s[i] == 'N'&&s[i+1] == 'M')      //locate NM_000207
        {
            for(j = 0;s[i]!=32;j++)
            {
                b[j] = s[i];
                i++;
            }
            break;
        }
    }
    printf("%s\n", b);
    fseek(fp, 0, 0);        //bring the file pointer back to the start of the file
    fgets(s, 200, fp);
    for(i = 12;i < 100;i++)     //get LOCUS
    {
        l[m] = s[i];
        m++;
    }
    printf("%s", l);
    while(1)        //locate the DNA sequence
    {
        fgets(s, 200, fp);
        if(s[0]=='O')
        {
            if(s[1]=='R')
            break;
        }
    }
    /*printf("Now at:%ld\n", ftell(fp));*/
    for(i = 0;i <= 10;i++)//get the DNA sequence
    {
        fgets(s, 300, fp);
        for(j =0;s[j] != '\n';j++)
        {
            if(s[j]!=32&&(s[j] < 48||s[j] > 57)&&s[j]!=47)
            {
                c[k] = s[j];
                k++;
            }
        }
    }
    fclose(fp);     //close the file
    printf("%s\n",c);
    convert(b, l, c);
}
