#ifndef h_rpttool
#define h_rpttool

#include<stdio.h>

int HTMLAlphabet(FILE *fn);
FILE *HTMLOpen(char *fname,char *title,int meta);
int HTMLClose(FILE *fn);
int HTMLBreak(FILE *fn);
void IndiPage(long indnum,char *ch);

int HTMLParents(FILE *f,long i);
int HTMLIndiLink(FILE *f,long i);
int HTMLIndiDateLink(FILE *f,long i);
int HTMLBirthDeath(FILE *f,long i);

void HTMLLinkToBase(FILE *f);

#endif
