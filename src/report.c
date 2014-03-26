#include"global.h"
#include"report.h"
#include"rptutil.h"
#include"utils.h" 
#include<ctype.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>



void  MakeSurname(); 
void  MakeIndex();
void  MakePages();      
void  MakeIndividual(FILE *f,long pgidx);
void  MakeGendexTxt();



int ProducePages(){
FILE *f;
int ctr;
char tempstring[LONGSTRLEN];
char *ptr;

 printf("...Generating index files\n");
 MakeIndex(); 
 printf("...Generating surname list\n");
 MakeSurname(); 
 printf("...Generating data pages\n");
 MakePages();
 if(strcmp(cfg_value[TOK_GENTEXT_FILE],"Y")==0){
   printf("...Generating gendex.txt file\n");
   MakeGendexTxt();
 }

printf("...Generating main page\n");


  sprintf(tempstring,"./html/%s%s",cfg_value[TOK_BASEPAGE],cfg_value[TOK_EXTENSION]);

 f=HTMLOpen(tempstring,cfg_value[TOK_TITLE],1);

 fprintf(f,"<!--GED2WWW-->\n");
 fprintf(f,"<!--Version:\"%s\"-->\n",VERSION);
 fprintf(f,"<!--Title:\"%s\"-->\n",cfg_value[TOK_TITLE]);
 fprintf(f,"<!--Owner:\"%s\"-->\n",cfg_value[TOK_OWNER_NAME]);
 fprintf(f,"<!--Email:\"%s\"-->\n",cfg_value[TOK_OWNER_EMAIL]);
 ptr = datestring;
 while(*ptr!='\0'){
   if((*ptr=='\n')||(*ptr=='\r')){
     *ptr='\0';
   } 
   ptr++;
 }
 fprintf(f,"<!--LastUpdate:\"%s\"-->\n",datestring);
 fprintf(f,"<!--NumFamily:%li-->\n",NumFamily);
 fprintf(f,"<!--NumIndividuals:%li-->\n",SortCtr);
 fprintf(f,"<!--NumSurnames:%li-->\n",num_surnames);

 fprintf(f,"<center><p>There are %li individuals and %li families \n",
          SortCtr,NumFamily);
 fprintf(f,"representing %li surnames in this database.</p>\n",num_surnames);

 fprintf(f,"The most common surnames in this database (in no particular order) are : \n");
 
 for(ctr=0;ctr<NUM_TOP_SURN;ctr++){
   if(surname_count[ctr]!=-1){
     fprintf(f,"<b>%s</b>(%d)",surname_name[ctr],surname_count[ctr]);
     if(ctr != (NUM_TOP_SURN-1)){
       fprintf(f,", ");
     }
   }
 }


 fprintf(f,"<p>These WWW pages were produced on %s.</p></center>\n",datestring);


 fprintf(f,"<center><h3>INDEX</h3></center>\n");
 HTMLAlphabet(f);
 fprintf(f,"<center><h3><a href=\"surname%s\">Surname List</a></h3></center>\n",cfg_value[TOK_EXTENSION]);


 fprintf(f,"<center><h3>Database Owner Contact Information</h3>\n");
 fprintf(f,"%s<br>\n",cfg_value[TOK_OWNER_NAME]);
 for(ctr=0;ctr<ADDRLEN;ctr++) 
   if(UserAddress[ctr][0]!='\0') 
     fprintf(f,"%s<br>",UserAddress[ctr]);
 if(strlen(cfg_value[TOK_OWNER_PHONE])) 
   fprintf(f,"%s<br>",cfg_value[TOK_OWNER_PHONE]);
 if(strlen(cfg_value[TOK_OWNER_EMAIL])>0)
   fprintf(f,"<a href=\"mailto:%s\">%s</a><br>",cfg_value[TOK_OWNER_EMAIL],cfg_value[TOK_OWNER_EMAIL]);
 if(strlen(cfg_value[TOK_OWNER_HOMEPAGE])>=0) 
   fprintf(f,"<a href=\"%s\">%s</a><br>",cfg_value[TOK_OWNER_HOMEPAGE],cfg_value[TOK_OWNER_HOMEPAGE]);
 fprintf(f,"</center>\n");

 HTMLClose(f);

/* printf("Done Generating WWW Pages\n");*/
 return 1;
}



void  MakeSurname(){
long ctr;
char *HoldName;
long HoldCount;
FILE *surn;
char HoldLetter;
char tempstr[LONGSTRLEN];
int ctr2;
int surn_index;

for(ctr2=0;ctr2<NUM_TOP_SURN;ctr2++){
  surname_count[ctr2]=-1;
  surname_name[ctr2]=NULL;
}


num_surnames = 0;

sprintf(tempstr,"./html/surname%s",cfg_value[TOK_EXTENSION]);
surn=HTMLOpen(tempstr,"Genealogy Surname List",0);

HTMLLinkToBase(surn);

HTMLBreak(surn);
HoldCount=0;

HoldName=individual[individual_sort[0]].surname;

for(ctr=0;ctr<SortCtr;ctr++){
  if((my_strcasecmp(HoldName,individual[individual_sort[ctr]].surname)==0)){
    HoldCount++;
  }else{
    num_surnames++;
    if(isalpha(HoldName[0]))  
      HoldLetter=tolower(HoldName[0]);
    else{
      if(HoldName[0]=='&')
        HoldLetter=tolower(HoldName[1]); 
      else
        HoldLetter='0';
    }
    strcpylow(HoldName,tempstr);

    surn_index=0;
    for(ctr2=1;ctr2<NUM_TOP_SURN;ctr2++){
      if(surname_count[ctr2]<surname_count[surn_index]) 
        surn_index = ctr2; 
    }   
    if(HoldCount>surname_count[surn_index]){
      surname_count[surn_index]=HoldCount;
      FreeIfNotNull(surname_name[surn_index]);
      surname_name[surn_index]=allocstr(HoldName);
    }

    fprintf(surn,"<a href=\"idx%c%s#%s\">%s</a>(%li),\n",
       HoldLetter,cfg_value[TOK_EXTENSION],tempstr,HoldName,HoldCount);
    HoldCount=1;
    HoldName=individual[individual_sort[ctr]].surname; 
  }
}

if(isalpha(HoldName[0]))  
  HoldLetter=tolower(HoldName[0]);
else{
  if(HoldName[0]=='&')
    HoldLetter=tolower(HoldName[1]); 
  else
    HoldLetter='0';
}
strcpylow(HoldName,tempstr);


if(HoldName[0]=='\0'){
  fprintf(surn,"<a href=\"idx0%s\">(none)</a>(%li)\n",
       cfg_value[TOK_EXTENSION],HoldCount);
}else{
  num_surnames++;
  fprintf(surn,"<a href=\"idx%c%s#%s\">%s</a>(%li)\n",
       HoldLetter,cfg_value[TOK_EXTENSION],tempstr,HoldName,HoldCount);
}
HTMLClose(surn);
} 



void MakeIndex(){
long ctr;
char HoldName[STRLEN];
char HoldLetter;
char str1[STRLEN],str2[STRLEN];
FILE *idx;
char t1;
char tempstr[LONGSTRLEN];

HoldName[0]='\0';
if(isalpha(individual[individual_sort[0]].sortname[0]))
  HoldLetter=toupper(individual[individual_sort[0]].sortname[0]);
else
  HoldLetter='~';

if(HoldLetter=='~'){
  sprintf(str1,"./html/idx0%s",cfg_value[TOK_EXTENSION]);
  sprintf(str2,"Index for surnames beginning with nothing or non-alphabetic character");
 }else{ 
  sprintf(str1,"./html/idx%c%s",tolower(HoldLetter),cfg_value[TOK_EXTENSION]); 
  sprintf(str2,"Genealogy Index for surnames beginning with %c",HoldLetter);
 }
idx=HTMLOpen(str1,str2,0);

HTMLLinkToBase(idx);

HTMLBreak(idx);
for(ctr=0;ctr<SortCtr;ctr++){
  if(isalpha(individual[individual_sort[ctr]].sortname[0]))
    t1=toupper(individual[individual_sort[ctr]].sortname[0]); 
  else
    t1='~';
  if(HoldLetter!=t1){
    HoldLetter=t1;
    if(HoldLetter=='~'){
      sprintf(str1,"./html/idx0%s",cfg_value[TOK_EXTENSION]);
      sprintf(str2,"Genealogy Index for surnames beginning with nothing or non-alphabetic character");
     }else{ 
      sprintf(str1,"./html/idx%c%s",tolower(HoldLetter),cfg_value[TOK_EXTENSION]);
      sprintf(str2,"Genealogy Index for surnames beginning with %c",HoldLetter);
    }
    HTMLClose(idx);
    idx=HTMLOpen(str1,str2,0);

  HTMLLinkToBase(idx);

  HTMLBreak(idx);
  }
  if(my_strcasecmp(HoldName,individual[individual_sort[ctr]].surname)!=0){
    if(individual[individual_sort[ctr]].surname[0]!='\0'){ 
      strcpy(HoldName,individual[individual_sort[ctr]].surname);
      strcpylow(individual[individual_sort[ctr]].surname,tempstr);
      fprintf(idx,"<a name=\"%s\"> </a>\n",tempstr);
    } 
  }
  if(strcmp(cfg_value[TOK_DATES_ON_INDEX],"Y")==0) 
    HTMLIndiDateLink(idx,individual_sort[ctr]);
  else
    HTMLIndiLink(idx,individual_sort[ctr]);
}
HTMLClose(idx);

}




void  MakePages(){
long pgctr;
FILE *FPage=NULL;
char str1[STRLEN];

for(pgctr=0;pgctr<pctr;pgctr++){
  if(pgctr%atol(cfg_value[TOK_FAM_PER_PG])==0){
    if(pgctr!=0)  
      HTMLClose(FPage);
    sprintf(str1,"./html/dat%li%s",pgctr/atol(cfg_value[TOK_FAM_PER_PG]),cfg_value[TOK_EXTENSION]);
    FPage=HTMLOpen(str1,"Genealogy Data",0);
  }
  MakeIndividual(FPage,pgctr);
  if(pgctr%atol(cfg_value[TOK_FAM_PER_PG])!=(atol(cfg_value[TOK_FAM_PER_PG])-1))
    HTMLBreak(FPage);
}
HTMLClose(FPage);
}


void  MakeIndividual(FILE *f,long pgidx){
long i;  
long fctr;
long cctr;

long spouse;

struct family_s *famptr;
struct individual_s *husbptr,*wifeptr,*childptr;

  i=page_location[pgidx];
  fprintf(f,"<blockquote>\n");

  HTMLLinkToBase(f);

  fprintf(f,"<a name=\"%li\">\n",pgidx%atol(cfg_value[TOK_FAM_PER_PG]));

  fprintf(f,"<b>");
  if(individual[i].surname!=NULL)
    fprintf(f,"%s",individual[i].surname);
  fprintf(f,", ");
  if(individual[i].remname!=NULL)
    fprintf(f,"%s ",individual[i].remname);
  if(individual[i].title!=NULL)
    fprintf(f,"%s ",individual[i].title);
  fprintf(f,"</b></a><br>\n");

  HTMLBirthDeath(f,i);
  
  HTMLParents(f,i);

/* do FAMILIES in here */
  for(fctr=0;fctr<individual[i].num_fams;fctr++){
    /* figure out spouse */
    famptr=FindFamily(individual[i].fams[fctr],&(individual[i])); 
    husbptr=FindIndi(famptr->husband);
    wifeptr=FindIndi(famptr->wife);
    spouse=-1;
    if((husbptr!=NULL)&&(husbptr->index==i)){
      if(wifeptr!=NULL)
        spouse=wifeptr->index;
    }else{
      if(husbptr!=NULL)
        spouse=husbptr->index;
    }

    fprintf(f,"%s:<blockquote>\n",cfg_value[TOK_LANG_FAMILY]);

/*PRINT MARIAGE INFO HERE*/
    if((famptr->married.flag=='Y')&&
       (((famptr->married.place!=NULL)&&(famptr->married.place[0]!='\0'))||
        ((famptr->married.date!=NULL)&&(famptr->married.date[0]!='\0')))){
      fprintf(f,"%s:\n",cfg_value[TOK_LANG_MARRIAGE]); 
      if(famptr->married.date!=NULL){
        if(strcmp(cfg_value[TOK_HIDE_LIVING],"N")==0){
          fprintf(f," %s\n",famptr->married.date);
        }else{ 
          if((strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0)&&
            (((husbptr!=NULL)&&(husbptr->death.flag=='N'))||
             ((wifeptr!=NULL)&&(wifeptr->death.flag=='N')))
            ){
            fprintf(f," --%s--\n",cfg_value[TOK_LANG_LIVING]); 
          }else{
            fprintf(f," %s\n",famptr->married.date);
          }
        }
      }
      if((famptr->married.date!=NULL)&&(famptr->married.place!=NULL))
        fprintf(f,"in ");
      if(famptr->married.place!=NULL)
        fprintf(f," %s\n",famptr->married.place);
      fprintf(f,"<br>\n");       
    }

    /* ifa print spouse info */
    if((spouse!=-1)&&(individual[spouse].inuse=='Y')){
      fprintf(f,"%s:<blockquote>\n",cfg_value[TOK_LANG_SPOUSE]);
      if(individual[spouse].page_index==pgidx){
        fprintf(f,"<b>%s, %s %s</b><br>\n",
             individual[spouse].surname,
             individual[spouse].remname,
             individual[spouse].title);
        HTMLBirthDeath(f,spouse);
        HTMLParents(f,spouse);
      }else{
        HTMLIndiLink(f,spouse);
      }
      fprintf(f,"</blockquote>\n");
    }




/* for each child */
    if(famptr->num_child>0){
      fprintf(f,"%s:<blockquote>\n",cfg_value[TOK_LANG_CHILDREN]);
      for(cctr=0;cctr<famptr->num_child;cctr++){
/* print child info */ 
        childptr=FindIndi(famptr->child[cctr]);
  
        if(childptr->page_index==pgidx){
          fprintf(f,"<b>%s, %s %s</b><br>\n",
               childptr->surname,
               childptr->remname,
               childptr->title);
          HTMLBirthDeath(f,childptr->index);
        }else{
          HTMLIndiLink(f,childptr->index);
        }
  
      }
      fprintf(f,"</blockquote>\n");
    }


    fprintf(f,"</blockquote>\n");
  }

  fprintf(f,"</blockquote>\n");
}


void MakeGendexTxt(){
FILE *f;
long indctr;
char str[STRLEN];
char gd_name[LONGSTRLEN];
char gd_surname[LONGSTRLEN];

char *gdn,*gdsn,*cptr;
char flag;

if((f=fopen("./html/gendex.txt","w"))==NULL){
  printf("**************************************************************\n");
  printf("Error opening gendex.txt file for create/write\n");
  printf("GED2WWW aborting.\n");
  exit(0);
}
for(indctr=0;indctr<MAX_INDIVIDUALS;indctr++){
  if(individual[indctr].inuse=='Y'){
    IndiPage(indctr,str);
/*    fprintf(f,"%s|%s|%s|%s|%s|%s|%s|\n",
      str,
      individual[indctr].surname,
      individual[indctr].raw_name,
      individual[indctr].birth.date,
      individual[indctr].birth.place,
      individual[indctr].death.date,
      individual[indctr].death.place); */

     memset(gd_surname,'\0',LONGSTRLEN);
     memset(gd_name,'\0',LONGSTRLEN);
  
     if(!NullString(individual[indctr].raw_name)){
       cptr=individual[indctr].raw_name;
       gdsn=gd_surname;
       gdn=gd_name;
       flag=0;

       while((*cptr)!='\0'){
         if(((*cptr)&0x80)==0){
           *gdn=*cptr;
           gdn++; 
           if((flag==1)&&((*cptr)!='/')){
             *gdsn=toupper(*cptr);
             gdsn++;
           }
         }
         if(*cptr=='/')
           flag++;
         cptr++;
       }
     }


     fprintf(f,"%s",str);

     fprintf(f,"|");
     if(!NullString(gd_surname))
       fprintf(f,"%s",gd_surname);

     fprintf(f,"|");
     if(!NullString(gd_name))
       fprintf(f,"%s",gd_name);

     fprintf(f,"|");
     if(!NullString(individual[indctr].birth.date)){
       if(!((strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0)&&(individual[indctr].death.flag=='N')))
         fprintf(f,"%s",individual[indctr].birth.date);
       else
         if(strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0)
           fprintf(f,"--%s--",cfg_value[TOK_LANG_LIVING]);
     }
     fprintf(f,"|");
     if(!NullString(individual[indctr].birth.place))
       fprintf(f,"%s",individual[indctr].birth.place);

     fprintf(f,"|");
     if(!NullString(individual[indctr].death.date))
       fprintf(f,"%s",individual[indctr].death.date);
     fprintf(f,"|");
     if(!NullString(individual[indctr].death.place))
       fprintf(f,"%s",individual[indctr].death.place);

     fprintf(f,"|\n");

  }
}
if(fclose(f)!=0){
  printf("**************************************************************\n");
  printf("Error closing gendex.txt file\n");
  printf("GED2WWW aborting\n");
  exit(0);
}

}
