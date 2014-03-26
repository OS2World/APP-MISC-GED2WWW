#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"parser.h"
#include"global.h"
#include"utils.h"


char parser_line[LONGSTRLEN];
char *parser_line_ptr;

int ReadStatment(FILE *f){
/* return of -1 = EOF */
/* return of -2 = unknown */
char *c;

if(feof(f))
  return -1;
fgets(parser_line,LONGSTRLEN,f);
GedcomLineCtr++;

parser_line[STRLEN-1]='\0';
/*parser_line_ptr = parser_line;*/

#ifdef LH_DEBUG
printf("-->%i\"%s\"\n",parser_line[0]-48,parser_line);
#endif

c=parser_line;
while((!isdigit(*c))&&((*c)!='\0')&&((*c)!='\n')&&((*c)!='\r'))
  c++;

if(isdigit(*c)){
  parser_line_ptr = c;
/*  log(__FILE__,__LINE__,"read line \"%s\"",parser_line_ptr);*/
  return atoi(c);
 }else{
  parser_line_ptr = parser_line;
/*  log(__FILE__,__LINE__,"Invalid Line \"%s\"",parser_line_ptr);*/
  return -2;
}


/*return (parser_line[0])-48;*/
}


int GetToken(char *st){
parser_line_ptr = parser_line_ptr + 2;
while(((*parser_line_ptr)!=' ')&&((*parser_line_ptr)!='\n')&&((*parser_line_ptr)!='\r')){
  *st=*parser_line_ptr;
  st++;
  parser_line_ptr++;
}
*st='\0';

return 0;
}


int GetEol(char *st) {
if(((*parser_line_ptr)=='\n')||
   ((*parser_line_ptr)=='\r')){
  *st='\0';
}else{
  parser_line_ptr = parser_line_ptr + 1;
  while(((*parser_line_ptr)!='\n')&&((*parser_line_ptr)!='\r')){
    *st=*parser_line_ptr;
    st++;
    parser_line_ptr++;
  } 
  parser_line_ptr++;
  *st='\0';
} 
return 0;
}


int TokenType(char *st){

if((st[0]=='@')&&(strstr(parser_line,"INDI")!=NULL))
  return TKN_INDIVID;
if((st[0]=='@')&&(strstr(parser_line,"FAM")!=NULL))
  return TKN_FAMILY;
if((st[0]=='@')&&(strstr(parser_line,"OBJE")!=NULL))
  return TKN_OBJE;
if((st[0]=='@')&&(strstr(parser_line,"NOTE")!=NULL))
  return TKN_NOTE;
if((st[0]=='@')&&(strstr(parser_line,"REPO")!=NULL))
  return TKN_REPO;
if((st[0]=='@')&&(strstr(parser_line,"SOUR")!=NULL))
  return TKN_SOUR;
if((st[0]=='@')&&(strstr(parser_line,"SUBN")!=NULL))
  return TKN_SUBN;
if((st[0]=='@')&&(strstr(parser_line,"SUBM")!=NULL))
  return TKN_SUBM;
if(st[0]=='@')
  return TKN_OTH_0;

if((strcmp(st,"BIRT")==0)||(strcmp(st,"BIRTH")==0)) 
  return TKN_BIRTH;
if((strcmp(st,"DEAT")==0)||(strcmp(st,"DEATH")==0))
  return TKN_DEATH;
if((strcmp(st,"FAMS")==0)||(strcmp(st,"FAMILY_SPOUSE")==0))
  return TKN_FAMS;
if((strcmp(st,"FAMC")==0)||(strcmp(st,"FAMILY_CHILD")==0))
  return TKN_FAMC;
if(strcmp(st,"SEX")==0)
  return TKN_SEX;
if((strcmp(st,"LVG")==0)||(strcmp(st,"LIVING")==0))
  return TKN_LVG;
if((strcmp(st,"HUSB")==0)||(strcmp(st,"HUSBAND")==0))
  return TKN_HUSB;
if(strcmp(st,"WIFE")==0)
  return TKN_WIFE;
if((strcmp(st,"MARR")==0)||(strcmp(st,"MARRIAGE")==0))
  return TKN_MARR;
if((strcmp(st,"CHIL")==0)||(strcmp(st,"CHILD")==0))
  return TKN_CHIL;
if(strcmp(st,"DATE")==0)
  return TKN_DATE;
if((strcmp(st,"PLAC")==0)||(strcmp(st,"PLACE")==0))
  return TKN_PLACE;
if(strcmp(st,"NAME")==0)
  return TKN_NAME;
if((strcmp(st,"TITL")==0)||(strcmp(st,"TITLE")==0))
  return TKN_TITLE;
if(strcmp(st,"CHAR")==0)
  return TKN_CHAR;

return 0;
}


long ExtractNum(char *st){
long ret;

ret =0;

while(!isdigit(*st))
   st++;
/* skip forwards to # */
ret=atol(st);
/* skip forwards to nun-number */
/* isdigit() */

return ret;
}


char *ExtractPtr(char *st,char fam_ind_type){
char buf[LONGSTRLEN];
char *beg;

while(*st!='@')
  st++;
st++;
sprintf(buf,"%c%s",fam_ind_type,st);
beg=buf;
while(*beg!='@')
  beg++;
(*beg)='\0';
/*printf("\"%s\"\n",buf);*/
return to_text(buf);
}
