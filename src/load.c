#include "global.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


int PreProcessGed(FILE *gf){
int basetype;
int level2;
char str1[STRLEN],str2[LONGSTRLEN];
int reccount = 0;

MAX_INDIVIDUALS = 0;
MAX_FAMILIES = 0;

char_set = CHAR_UNKNOWN;

basetype=ReadStatment(gf);
while(basetype!=-1){
  GetToken(str1);
  level2=TokenType(str1);  
  if(basetype==0){
      switch (level2){
        case TKN_INDIVID:
          MAX_INDIVIDUALS++;
        break;
        case TKN_FAMILY: 
          MAX_FAMILIES++;
        break;
      }
  }else{
    if((basetype==1)&&(level2==TKN_CHAR)){
      /*should be in header*/
      GetEol(str2);
      if(strcmp(str2,"ASCII")==0)
        char_set = CHAR_ASCII;
      if(strcmp(str2,"ANSI")==0)
        char_set = CHAR_ASCII;
      if(strcmp(str2,"ANSEL")==0)
        char_set = CHAR_ANSEL;
      if(strcmp(str2,"UNICODE")==0)
        char_set = CHAR_UNICODE;
    }
  }
  basetype=ReadStatment(gf);
  reccount++;  
  if(reccount > PERIOD_EVERY){
    reccount = 0;
    printf(".");
    fflush(stdout);
  }
}


  printf("\n");

  MAX_FAMILIES++;
  MAX_INDIVIDUALS++;
  return 1;
}

int LoadGed(FILE *gf){
char str1[LONGSTRLEN],str2[LONGSTRLEN];
int basetype;
long currid;
char famorindivid='\0';
char detailbase='\0';
int level2;
int reccount = 0;
char *ptr;

Lvg_Flags_Detected='N';

memset(individual,0,sizeof(struct individual_s)*MAX_INDIVIDUALS);
memset(family,0,sizeof(struct family_s)*MAX_FAMILIES);
for(currid=0;currid<MAX_INDIVIDUALS;currid++){
  individual[currid].page_index = -1;
}
for(currid=0;currid<MAX_FAMILIES;currid++){
  family[currid].husband = NULL;
  family[currid].wife = NULL;
}

basetype=ReadStatment(gf);
while(basetype!=-1){
  GetToken(str1);
  level2=TokenType(str1);  
  GetEol(str2);

  #ifdef LH_DEBUG 
  printf("  %i %s(%i) \n\"%s\"\n",basetype,str1,level2,str2); 
  #endif

  switch (basetype){
    case 0:
      switch (level2){
        case TKN_INDIVID:
          famorindivid='I';
          currid=NumIndivid;
          NumIndivid++;
          ptr=ExtractPtr(str1,'i');
          HashInsert(ptr,&(individual[currid]));
     /*     printf("..%i\n",currid);   */
                                  /* check for array overflow here */
          if(currid>=MAX_INDIVIDUALS){
            printf("ERROR, INDIVIDUAL TABLE OVERFOW!\n");
            exit(0);
          }          
                        /* check for dup individid here */
          if(individual[currid].inuse=='Y'){
            printf("ERROR, Duplicate individual %li\n",currid);
            exit(0);
          }          
          individual[currid].inuse='Y'; 
          individual[currid].identifier='I'; 
          individual[currid].index=currid; 
          individual[currid].key=ptr; 
        break;
        case TKN_FAMILY: 
          famorindivid='F';
          currid=NumFamily;
          NumFamily++;
          ptr=ExtractPtr(str1,'f');
          HashInsert(ptr,&(family[currid]));
                                  /* check for array overflow here */
          if(currid>=MAX_FAMILIES){
            printf("ERROR, family TABLE OVERFOW!\n");
            exit(0);
          }          
                                  /* check for dup famid here */
          if(family[currid].inuse=='Y'){
            printf("ERROR, Duplicate family\n");
            exit(0);
          }          
          family[currid].inuse='Y';
          family[currid].identifier='F'; 
          family[currid].index=currid;
          family[currid].key=ptr;
        break;
        default:
          famorindivid='X';
        break;
      }
    break;
    case 1:
      detailbase='\0';
      if(famorindivid!='X')
      switch(level2){
        case TKN_BIRTH:
          /*should be individual */
          if(famorindivid=='I'){
            individual[currid].birth.flag='Y';
            detailbase='B';
          }else{
            warning("BIRTH tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break;
        case TKN_DEATH:
          /*should be individual */
          if(famorindivid=='I'){
            individual[currid].death.flag='Y';
            detailbase='D';
          }else{
            warning("DEATH tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break;
        case TKN_FAMS:
          /*should be individual*/
          /*should not exceed MAX_FAMS*/
          if(famorindivid=='I'){
            if(individual[currid].num_fams>=MAX_FAMS){
              printf("ERROR, individual has too many FAMS\n");
              exit(1);
            }
            individual[currid].fams[individual[currid].num_fams]=ExtractPtr(str2,'f');
            individual[currid].num_fams++;
          }else{
            warning("FAMS tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break; 
        case TKN_FAMC:
          /*should be individual*/
          /*should be 0-1 FAMC*/
          if(famorindivid=='I'){
            individual[currid].famc=ExtractPtr(str2,'f');
            individual[currid].num_famc++;
          }else{
            warning("FAMC tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break;
        case TKN_SEX:
          /*should be individual*/
          if(famorindivid=='I'){
            individual[currid].gender=str2[0];
          }else{
            warning("SEX tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break;
        case TKN_LVG:
          /*should be individual*/
          if(famorindivid=='I'){
            individual[currid].death.flag='N';
            Lvg_Flags_Detected='Y';
          }else{
            warning("LVG tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break;
        case TKN_NAME:
          /*should be individual*/
          if(famorindivid=='I'){
            individual[currid].raw_name=allocstr(str2);
          }else{
            warning("NAME tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break;
        case TKN_TITLE:
          /*should be individual*/
          if(famorindivid=='I'){
            individual[currid].title=to_html(str2);
          }else{
            warning("TITLE tag found in FAMILY section (family ID \"%s\")",family[currid].key);
          }
        break;
        case TKN_HUSB:
          /*should be FAMILY*/
          if(famorindivid=='F'){
            family[currid].husband=ExtractPtr(str2,'i');
          }else{
            warning("HUSB tag found in INDIVIDUAL section (individual ID \"%s\")",individual[currid].key);
          }
        break;
        case TKN_WIFE:                       
          /*should be FAMILY*/
          if(famorindivid=='F'){
            family[currid].wife=ExtractPtr(str2,'i');
          }else{
            warning("WIFE tag found in INDIVIDUAL section (individual ID \"%s\")",individual[currid].key);
          }
        break;
        case TKN_MARR:
          /*should be FAMILY*/
          if(famorindivid=='F'){
            family[currid].married.flag='Y';
            detailbase='M';
          }else{
            warning("MARR tag found in INDIVIDUAL section (individual ID \"%s\")",individual[currid].key);
          }
        break;
        case TKN_CHIL:
          /*should be family*/
          /*not exceed MAX_CHILDREN*/
          if(famorindivid=='F'){
            if(family[currid].num_child>=MAX_CHILDREN){
              printf("ERROR, family has too many CHILdren\n");
              exit(1);
            }
            family[currid].child[family[currid].num_child]=ExtractPtr(str2,'i');
            family[currid].num_child++;
          }else{
            warning("CHIL tag found in INDIVIDUAL section (individual ID \"%s\")",individual[currid].key);
          }
        break;
      }
    break;
    case 2:
      if(famorindivid!='X') 
      switch(level2){
        case TKN_DATE:
          switch(detailbase){
            case 'B':
              /*should be individual*/  
              if(famorindivid=='I'){
                individual[currid].birth.date=to_html(str2); 
              }else{
                warning("BIRTH DATE tag found in FAMILY section (family ID \"%s\")",family[currid].key);
              }
            break;
            case 'M':
              /*should be FAILY */  
              if(famorindivid=='F'){
                family[currid].married.date=to_html(str2); 
              }else{
                warning("MARRIAGE DATE tag found in INDIVIDUAL section (individual ID \"%s\")",individual[currid].key);
              }
            break;
            case 'D':
              /*should be individual*/  
              if(famorindivid=='I'){
                individual[currid].death.date=to_html(str2); 
              }else{
                warning("DEATH DATE tag found in FAMILY section (family ID \"%s\")",family[currid].key);
              }
            break;
          }
        break;
        case TKN_PLACE:
          switch(detailbase){
            case 'B':
              /*should be individual*/  
              if(famorindivid=='I'){
                individual[currid].birth.place=to_html(str2); 
              }else{
                warning("BIRTH PLACE tag found in FAMILY section (family ID \"%s\")",family[currid].key);
              }
            break;
            case 'M':
              /*should be FAILY */  
              if(famorindivid=='F'){
                family[currid].married.place=to_html(str2); 
              }else{
                warning("MARRIAGE PLACE tag found in INDIVIDUAL section (individual ID \"%s\")",individual[currid].key);
              }
            break;
            case 'D':
              /*should be individual*/ 
              if(famorindivid=='I'){
                individual[currid].death.place=to_html(str2); 
              }else{
                warning("DEATH PLACE tag found in FAMILY section (family ID \"%s\")",family[currid].key);
              }
            break;
          }
        break;
      }
    break;
  }
  basetype=ReadStatment(gf);
  reccount ++;
  if(reccount > PERIOD_EVERY){
    reccount = 0;
    printf(".");
    fflush(stdout);
  }
}

printf("\n");

/*HashStats();*/

return 0;

}




