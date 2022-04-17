/*
    dnatranspro.h
    This is the header file.
    author:Wang Chaoxian, Wang Yuchen, Pang Chongchen, Zhu Xiangyu
    date:2022.3.28
**/
#ifndef __DNATRANSPRO__
#define __DNATRANSPRO__

void trans(char a[]);                                                  // translate DNA sequence into amino acid sequence
void extract();                                                        // extract the information needed
void convert(char a[], char l[], char b[]);                            // generate the FASTA file

#endif // __DNATRANSPRO__
