#include"rptutil.h"
#include"global.h"
#include"utils.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

int HTMLAlphabet(FILE *fn){
int ctr;

fprintf(fn,"<center>\n");
for(ctr=0;ctr<27;ctr++){
  if(alpha_usage[ctr]==0){
    if(ctr==26){
      fprintf(fn,"(NONE)");
    }else{
      fprintf(fn,"%c ",ctr+65);
    }
  }else{
    if(ctr==26){
      fprintf(fn,"<a href=\"idx0%s\">(NONE/OTHER)</a>\n",cfg_value[TOK_EXTENSION]);
    }else{
      fprintf(fn,"<a href=\"idx%c%s\">%c</a> \n",tolower(ctr+65),cfg_value[TOK_EXTENSION],ctr+65);
    }
  }
}
fprintf(fn,"</center>\n");
return 1;
}


FILE *HTMLOpen(char *fname,char *title,int meta){
FILE *fn;
int ctr;

if((fn=fopen(fname,"w"))==NULL){
  printf("***********************************************************\n");
  printf("Error opening file '%s' for creation/write.\n",fname);
  printf("GED2WWW aborted\n");
  exit(0);
}
setvbuf(fn,filebuffer,_IOFBF,FILEBUFFER_SIZE);
fprintf(fn,"<html><head>\n");
if(meta==1){
  fprintf(fn,"<meta name=\"description\" content=\"");
  fprintf(fn,"%s Genealogy Database containing %ld individuals, %s %s GED2WWW, Major surnames include ",cfg_value[TOK_TITLE],SortCtr,cfg_value[TOK_OWNER_NAME],cfg_value[TOK_OWNER_EMAIL]); 
  for(ctr=0;ctr<NUM_TOP_SURN;ctr++){
    if(surname_count[ctr]!=-1){
      fprintf(fn,"%s ",surname_name[ctr]);
    } 
  }
  fprintf(fn,"\">\n");

  fprintf(fn,"<meta name=\"keywords\" content=\"");
  fprintf(fn,"%s, Genealogy, Database, %s, %s, GED2WWW",cfg_value[TOK_TITLE],cfg_value[TOK_OWNER_NAME],cfg_value[TOK_OWNER_EMAIL]); 
  for(ctr=0;ctr<NUM_TOP_SURN;ctr++){
    if(surname_count[ctr]!=-1){
      fprintf(fn,"%s, ",surname_name[ctr]);
    } 
  }
  fprintf(fn,"\">\n");
}
fprintf(fn,"<title>%s</title>\n",title);
fprintf(fn,"</head>");

fprintf(fn,"<body");
if(strcmp(cfg_value[TOK_COLOR_YN],"Y")==0)
  fprintf(fn," BGCOLOR=\"%s\" TEXT=\"%s\" LINK=\"%s\" VLINK=\"%s\"",
    cfg_value[TOK_COLOR_BACKGROUND],
    cfg_value[TOK_COLOR_NORMAL],
    cfg_value[TOK_COLOR_NOT_FOLLOWED],
    cfg_value[TOK_COLOR_FOLLOWED]);
if(cfg_value[TOK_BACKGROUNDIMAGE]!='\0')
  fprintf(fn," background=\"%s\"",cfg_value[TOK_BACKGROUNDIMAGE]);
fprintf(fn,">\n");

fprintf(fn,"<center><h2>%s</h2></center>\n",title);
HTMLBreak(fn);
return fn;
}


int HTMLClose(FILE *fn){

HTMLBreak(fn);
 fprintf(fn,"<center><p>\n");
 fprintf(fn,"This page, and all genealogical data contained on it are\n");
 fprintf(fn,"&copy; %d %s\n",cur_year,cfg_value[TOK_OWNER_NAME]);
 if(strlen(cfg_value[TOK_OWNER_EMAIL])>0) 
   fprintf(fn,"(<a href=\"mailto:%s\">%s</a>)",cfg_value[TOK_OWNER_EMAIL],cfg_value[TOK_OWNER_EMAIL]);
 fprintf(fn,".</p>\n");

 fprintf(fn,"<p><i>This page was produced by GED2WWW version %s \n",VERSION);
 fprintf(fn,"compiled on %s at %s.\n",__DATE__,__TIME__);
 fprintf(fn,"GED2WWW &copy; 1996-1999 Leslie Howard.\n");
 fprintf(fn,"GED2WWW is free software, distributed under the terms\n");
 fprintf(fn,"of the GNU General Public License.\n");
 fprintf(fn,"For more information about GED2WWW or the GNU General\n");
 fprintf(fn,"Public License visit the GED2WWW webpage at\n");
 fprintf(fn,"<a href=\"http://www.lesandchris.com/ged2www\">\n");
 fprintf(fn,"http://www.lesandchris.com/ged2www</a>\n");
 fprintf(fn,"</i>.</p></center>\n");

fprintf(fn,"</body></html>\n");

if(fclose(fn)!=0){
  printf("***********************************************************\n");
  printf("Error closing file, GED2WWW aborted.\n");
  exit(0);
}
return 1;
}


int HTMLBreak(FILE *fn){
  fprintf(fn,"<hr size=\"4\" width=\"80%%\">\n");
return 1;
}


void IndiPage(long indnum,char *ch){
  sprintf(ch,"dat%li%s#%li",
       individual[indnum].page_index/atol(cfg_value[TOK_FAM_PER_PG]),
       cfg_value[TOK_EXTENSION],
       individual[indnum].page_index%atol(cfg_value[TOK_FAM_PER_PG]));
}


int HTMLParents(FILE *f,long i){
struct family_s *fa; 
struct individual_s *husb,*wife; 


if(individual[i].num_famc!=0){

  fa=FindFamily(individual[i].famc,&(individual[i]));
  husb=FindIndi(fa->husband);
  wife=FindIndi(fa->wife);

  if((husb!=NULL)||(wife!=NULL)){
    fprintf(f,"%s:<blockquote>\n",cfg_value[TOK_LANG_PARENTS]);
    if((husb!=NULL)&&(husb->inuse=='Y')){
      fprintf(f,"%s: ",cfg_value[TOK_LANG_FATHER]);
      HTMLIndiLink(f,husb->index);
    }
    if((wife!=NULL)&&(wife->inuse=='Y')){
      fprintf(f,"%s: ",cfg_value[TOK_LANG_MOTHER]);
      HTMLIndiLink(f,wife->index);
    }
    fprintf(f,"</blockquote>\n");
  }
}


return 1;
}



int HTMLIndiLink(FILE *f, long i){
char str[STRLEN];

IndiPage(i,str);

fprintf(f,"<a href=\"%s\">",str);

if(!NullString(individual[i].surname))
  fprintf(f,"%s, ",individual[i].surname);
 else
  fprintf(f,", ");

if(!NullString(individual[i].remname))
  fprintf(f,"%s ",individual[i].remname);

if(!NullString(individual[i].title))
  fprintf(f,"%s</a><br>\n",individual[i].title);
 else
  fprintf(f,"</a><br>\n");

return 1;
}



int HTMLIndiDateLink(FILE *f, long i){
char str[STRLEN];

IndiPage(i,str);

fprintf(f,"<a href=\"%s\">",str);

if(individual[i].surname!=NULL)
  fprintf(f,"%s, ",individual[i].surname);
 else
  fprintf(f,", ");

if(individual[i].remname!=NULL)
  fprintf(f,"%s ",individual[i].remname);

if(individual[i].title!=NULL)
  fprintf(f,"%s",individual[i].title);

fprintf(f,"</a> (");

if(!NullString(individual[i].birth.date)){
  if(!((strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0)&&(individual[i].death.flag=='N')))
    fprintf(f,"%s",individual[i].birth.date);
  else
    if(strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0)
      fprintf(f,"--%s-",cfg_value[TOK_LANG_LIVING]); 
}

fprintf(f,"-");

if(!NullString(individual[i].death.date))
  fprintf(f,"%s",individual[i].death.date);

fprintf(f,")<br>\n");

return 1;
}









int HTMLBirthDeath(FILE *f,long i){

if(individual[i].birth.flag=='Y'){
  if(((individual[i].birth.date!=NULL)&&(individual[i].birth.date[0]!='\0'))||
     ((individual[i].birth.place!=NULL)&&(individual[i].birth.place[0]!='\0'))){
    fprintf(f,"%s : ",cfg_value[TOK_LANG_BIRTH]);
    if(individual[i].birth.date!=NULL)
    if(strcmp(cfg_value[TOK_HIDE_LIVING],"N")==0){
      fprintf(f,"%s ",individual[i].birth.date);
    }else{
      if(individual[i].death.flag=='N'){
        fprintf(f,"--%s-- ",cfg_value[TOK_LANG_LIVING]);
      }else{
        fprintf(f,"%s ",individual[i].birth.date);
      }
    }
    if(individual[i].birth.place!=NULL)
      fprintf(f,"%s",individual[i].birth.place);
    fprintf(f,"<br>\n");
  }
}



if(individual[i].death.flag=='Y'){
  if(((individual[i].death.date!=NULL)&&(individual[i].death.date[0]!='\0'))||
     ((individual[i].death.place!=NULL)&&(individual[i].death.place[0]!='\0'))){
    fprintf(f,"%s : ",cfg_value[TOK_LANG_DEATH]);
    if(individual[i].death.date!=NULL)
      fprintf(f,"%s ",individual[i].death.date);
    if(individual[i].death.place!=NULL)
      fprintf(f,"%s",individual[i].death.place);
    fprintf(f,"<br>\n");
  }
}

if(strcmp(cfg_value[TOK_DISPLAY_GENDERS],"Y")==0){
  if(individual[i].gender=='F'){
    fprintf(f,"%s: %s<BR>\n",cfg_value[TOK_LANG_GENDER],cfg_value[TOK_LANG_FEMALE]);
  }else{
    if(individual[i].gender=='M'){
      fprintf(f,"%s: %s<BR>\n",cfg_value[TOK_LANG_GENDER],cfg_value[TOK_LANG_MALE]);
    }
  }
}

return 1;
}


void HTMLLinkToBase(FILE *f){
  fprintf(f,"<center><h3><a href=\"%s%s\">Back to Main Page</a></h3></center>\n",cfg_value[TOK_BASEPAGE],cfg_value[TOK_EXTENSION]);
}



