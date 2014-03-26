#include "cfgfile.h"
#include "global.h"
#include "utils.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define TOK_UNKNOWN              0

char *cfg_name[TOK_LAST];



int parse_line(char *line,char *tag,char *value);
int determine_tag(char *tag);
void initialize_cfg();
void initialize_entry(int idx,char *nam,char *def);


void initialize_entry(int idx,char *nam,char *def){
  cfg_value[idx]=allocstr(def);
  cfg_name[idx]=allocstr(nam);
}

void initialize_cfg(){

  initialize_entry( TOK_GEDCOM_NAME , "GEDCOM_Name" ,"");
  initialize_entry( TOK_OWNER_NAME , "Owner_Name" ,"");
  initialize_entry( TOK_OWNER_ADD_0 , "Owner_Add0" ,"");
  initialize_entry( TOK_OWNER_ADD_1 , "Owner_Add1" ,"");
  initialize_entry( TOK_OWNER_ADD_2 , "Owner_Add2" ,"");
  initialize_entry( TOK_OWNER_ADD_3 , "Owner_Add3" ,"");
  initialize_entry( TOK_OWNER_ADD_4 , "Owner_Add4" ,"");
  initialize_entry( TOK_OWNER_ADD_5 , "Owner_Add5" ,"");
  initialize_entry( TOK_OWNER_ADD_6 , "Owner_Add6" ,"");
  initialize_entry( TOK_OWNER_ADD_7 , "Owner_Add7" ,"");
  initialize_entry( TOK_OWNER_ADD_8 , "Owner_Add8" ,"");
  initialize_entry( TOK_OWNER_ADD_9 , "Owner_Add9" ,"");
  initialize_entry( TOK_OWNER_PHONE , "Owner_Phone" ,"");
  initialize_entry( TOK_OWNER_EMAIL , "Owner_Email" ,"");
  initialize_entry( TOK_OWNER_HOMEPAGE , "Owner_HomePage" ,"");
  initialize_entry( TOK_TITLE , "Title" ,"");
  initialize_entry( TOK_BASEPAGE , "BasePage" ,"index");
  initialize_entry( TOK_BACKGROUNDIMAGE , "Background_Image" ,"");
  initialize_entry( TOK_GENTEXT_FILE , "Gentext_File" ,"N");
  initialize_entry( TOK_DISPLAY_GENDERS , "Display_Genders" ,"N");
  initialize_entry( TOK_HIDE_LIVING , "Hide_Living" ,"N");
  initialize_entry( TOK_DATES_ON_INDEX , "Dates_On_Index" ,"N");
  initialize_entry( TOK_DEAD_MARRIAGE , "Assume_Dead_Marriage" ,"1900");
  initialize_entry( TOK_DEAD_BIRTH , "Assume_Dead_Birth" ,"1880");
  initialize_entry( TOK_FAM_PER_PG, "Familiy_Groups_Per_Page" ,"20");
  initialize_entry( TOK_COLOR_BACKGROUND , "Color_Background" ,"FFFFFF");
  initialize_entry( TOK_COLOR_NORMAL , "Color_Normal" ,"000000");
  initialize_entry( TOK_COLOR_FOLLOWED , "Color_Followed" ,"FF0000");
  initialize_entry( TOK_COLOR_NOT_FOLLOWED , "Color_Not_Followed" ,"0000FF");
  initialize_entry( TOK_COLOR_YN, "Custom_Colors" ,"N");

  initialize_entry( TOK_HTML_DIR , "Html_Dir" ,"");
  initialize_entry( TOK_EXTENSION , "Html_Extension" ,".html");

  initialize_entry( TOK_LANG_PARENTS , "LANG_PARENTS" ,"Parents");
  initialize_entry( TOK_LANG_FATHER , "LANG_FATHER" ,"Father");
  initialize_entry( TOK_LANG_MOTHER , "LANG_MOTHER" ,"Mother");
  initialize_entry( TOK_LANG_LIVING , "LANG_LIVING" ,"living");
  initialize_entry( TOK_LANG_BIRTH , "LANG_BIRTH" ,"Birth");
  initialize_entry( TOK_LANG_DEATH , "LANG_DEATH" ,"Death");
  initialize_entry( TOK_LANG_GENDER , "LANG_GENDER" ,"Gender");
  initialize_entry( TOK_LANG_MALE , "LANG_MALE" ,"Male");
  initialize_entry( TOK_LANG_FEMALE , "LANG_FEMALE" ,"Female");
  initialize_entry( TOK_LANG_FAMILY , "LANG_FAMILY" ,"Family");
  initialize_entry( TOK_LANG_MARRIAGE , "LANG_MARRIAGE" ,"Marriage");
  initialize_entry( TOK_LANG_IN , "LANG_IN" ,"");
  initialize_entry( TOK_LANG_SPOUSE , "LANG_SPOUSE" ,"Spouse");
  initialize_entry( TOK_LANG_CHILDREN , "LANG_CHILDREN" ,"Children");
} 

int parse_line(char *line,char *tag,char *value){
char *lp,*tp,*vp;

lp=line;
while(*lp!='\0'){
  if(*lp=='\r')
    *lp=' ';
  if(*lp=='\t')
    *lp=' ';
  if(*lp=='\n')
    *lp=' ';
  lp++;
}

lp=line;
tp=tag;
vp=value;


while((*lp!='\0')&&((*lp==' ')||(*lp=='\t'))) 
  lp++;

while((*lp!='\0')&&(*lp!=' ')&&(*lp!='\t')&&(*lp!=':')){
  *tp=*lp;
  lp++;
  tp++;
}

*tp='\0';

while((*lp!='\0')&&((*lp==' ')||(*lp=='\t'))) 
  lp++;

/* *lp should == ':' */
lp++;


while((*lp!='\0')&&((*lp==' ')||(*lp=='\t'))) 
  lp++;

while(*lp!='\0'){
  *vp=*lp;
  lp++;
  vp++;
}

*vp='\0';
vp--;
while(*vp==' '){
  *vp='\0';
  vp--;
}

return 0;
}



int determine_tag(char *tag){
  int cnt;

  for(cnt=1;cnt<TOK_LAST;cnt++){
    if(my_strcasecmp(tag,cfg_name[cnt])==0){
      return cnt;
    }
  }
  return TOK_UNKNOWN;
}




void load_options(char *filename){
  FILE *f;
  char cfg_line[LONGSTRLEN];
  char tag[LONGSTRLEN];
  char value[LONGSTRLEN];
  int  tag_num;

  initialize_cfg();
  f=fopen(filename,"r");
  if(f==NULL){
    printf("Config file not found \"%s\"\n",filename);
    return;
  }

  fgets(cfg_line,LONGSTRLEN,f);
  while(!feof(f)){
    if((cfg_line[0]!='\0')&&(cfg_line[0]!='#')&&(strlen(cfg_line)>2)){
      parse_line(cfg_line,tag,value);
      tag_num=determine_tag(tag);
      if(strlen(tag)>4){ 
        if(tag_num == TOK_UNKNOWN){
          printf("ERROR!  Unrecognized Tag in g2w_opts.cfg \"%s\" \"%s\"\n",tag,value);
          exit(0);
        }else{
          FreeIfNotNull(cfg_value[tag_num]);
          cfg_value[tag_num]=allocstr(value);
        }
      } 
    }
    fgets(cfg_line,LONGSTRLEN,f);
  }
  fclose(f);
}



void save_options(char *filename){
  FILE *f;
  int cnt;

  f=fopen(filename,"w");
  if(f==NULL){
    printf("Can't open config file \"%s\" for writing\n",filename);
    return;
  }
  for(cnt=1;cnt<TOK_LAST;cnt++){
    if(strlen(cfg_value[cnt])>0){
      fprintf(f,"%s : %s\n",cfg_name[cnt],cfg_value[cnt]);
    } 
  }
  fclose(f);
}

