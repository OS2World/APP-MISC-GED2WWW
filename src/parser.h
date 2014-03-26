#ifndef h_parser
#define h_parser

#include<stdio.h>

#define TKN_UNKNOWN 0
#define TKN_INDIVID 1
#define TKN_FAMILY  2
#define TKN_BIRTH   3
#define TKN_DEATH   4
#define TKN_FAMS    5
#define TKN_FAMC    6
#define TKN_SEX     7
#define TKN_LVG     8
#define TKN_HUSB    9
#define TKN_WIFE   10
#define TKN_MARR   11
#define TKN_CHIL   12
#define TKN_DATE   13
#define TKN_PLACE  14
#define TKN_NAME   15
#define TKN_TITLE  16
#define TKN_CHAR   17

#define TKN_OBJE   18
#define TKN_NOTE   19
#define TKN_REPO   20
#define TKN_SOUR   21
#define TKN_SUBN   22
#define TKN_SUBM   23
#define TKN_OTH_0  24



int ReadStatment(FILE *f);
int GetToken(char *st);
int GetEol(char *st); 
int TokenType(char *st);
long ExtractNum(char *st);
char *ExtractPtr(char *st,char fam_individ_type);

#endif
