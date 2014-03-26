#define DEFINE_VARDEF
#include "global.h"
#include "utils.h"
#include "load.h"
#include "process.h"
#include "report.h"
#include "stdlib.h"
#include "cfgfile.h"
#include<stdio.h>
#include<time.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>

#ifdef UNIX
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#endif

int is_valid_color(char *clr);

main(int argc,char *argv[]){
FILE *gedin;
time_t runtime;
char yn;
char StrBuffer[LONGSTRLEN];
long ctr; 
long ctr2; 
int fname_good;
char *cptr;



pctr = 0;

_log = 0;
interactive_mode=1;

strcpy(cfgfile_name,"g2w_opts.txt");


time(&runtime);
strcpy(datestring,ctime(&runtime));
cptr=datestring;
while(*cptr!='\0')
  cptr++;
cptr-=2;
while(isdigit(*cptr))
  cptr--;
cptr++;
cur_year=atoi(cptr);


printf("===============================================\n");
printf("GED2WWW version %s (c) 1996-1999 Leslie Howard.\nCompiled %s %s.\n",
    VERSION,__DATE__,__TIME__);

for(ctr=1;ctr<argc;ctr++)
  process_arg(argv[ctr]);

log_init();

load_options(cfgfile_name);

log_msg(__FILE__,__LINE__,"GED2WWW start");


#ifdef LH_DEBUG
printf("sizeof(individual_s) = %4i\n",sizeof(struct individual_s));
printf("sizeof(family_s)     = %4i\n\n",sizeof(struct family_s));
#endif

printf("GED2WWW is free software; you can redistribute it and/or modify it\n");
printf("under the term of the GNU General Public Liscense.  This program is\n");
printf("distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;\n");
printf("without even the implied warranty of MERCHANTABILITY or FITNESS FOR A\n");
printf("PARTICULAR PURPOSE.  See the GNU General Public Liscense for details.\n");
printf("A copy of the liscense can be found in the COPYING file.\n\n");

printf("GED2WWW converts GEDCOM files to HTML with an emphasis\n");
printf("on producing as little HTML as possible without sacraficing\n");
printf("vital content.\n\n");
printf("To learn more about GED2WWW visit the GED2WWW homepage at\n");
printf("http://www.lesandchris.com/ged2www\n");


if(interactive_mode==1){
  printf("===============================================\n");
  printf("ATTENTION! GED2WWW needs to create a place to put the HTML\n");
  printf("files it produces.  GED2WWW will create a directory called\n");
  printf(".\\html\\ under the current directory.  If this directory already\n");
  printf("exists, all HTML files in it will be deleted (such as the results\n");
  printf("from a previous GED2WWW database conversion).\n");
  printf("Do you want to continue (Y/N) ? ");
  gets(StrBuffer);
}else{
  strcpy(StrBuffer,"Y");
}
if(toupper(StrBuffer[0])!='Y'){
  printf("thanks for using GED2WWW!\n");
  exit(0);
 }else{
#ifdef DOS
    freopen("nul","wb",stderr);
    system("mkdir html > nul");
    system("del .\\html\\*.txt > nul");
    system("del .\\html\\*.htm > nul");
#endif
#ifdef UNIX
    mkdir("./html",0755);
    system("rm ./html/*.txt -f");
    system("rm ./html/*.htm* -f");
#endif
  }

yn='Y';
if(interactive_mode==1){
 do{
  yn='N';

  printf("\n=====================techinl database info========================\n\n");
  printf("Default/present values of parameters are displayed inside of []\n");
  printf("To accept the default just hit <enter>\n\n");

 
  printf("Enter the name of the input GEDCOM file [%s]\n : ",cfg_value[TOK_GEDCOM_NAME]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_GEDCOM_NAME]);
    cfg_value[TOK_GEDCOM_NAME]=allocstr(StrBuffer);
  }

  gedin=fopen(cfg_value[TOK_GEDCOM_NAME],"r");
  while(gedin==NULL){
    printf("\nThe filename you entered [%s] was not found.\n",cfg_value[TOK_GEDCOM_NAME]);
    printf("Please try another filename : ");
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      FreeIfNotNull(cfg_value[TOK_GEDCOM_NAME]);
      cfg_value[TOK_GEDCOM_NAME]=allocstr(StrBuffer);
    }
    gedin=fopen(cfg_value[TOK_GEDCOM_NAME],"r");
  }
  fclose(gedin);
 
  printf("\nEnter the number of family groups per output data file.\n");
  printf("This controls the size and number of output files produced by GED2WWW.\n");
  printf("Smaller numbers create more files with fewer individuals per file\n");
  printf("larger numbers create fewer files but the files are larger [%s]\n : ",cfg_value[TOK_FAM_PER_PG]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0')
    if(atol(StrBuffer)>0){
      FreeIfNotNull(cfg_value[TOK_FAM_PER_PG]);
      cfg_value[TOK_FAM_PER_PG]=allocstr(StrBuffer);
    } 



  printf("\nDo you want to create a gendex.txt file [%s] (Y/N) ? ",cfg_value[TOK_GENTEXT_FILE]);
  StrBuffer[0]='L';
  while((StrBuffer[0]!='Y')&&(StrBuffer[0]!='N')&&(StrBuffer[0]!='\0')){
    gets(StrBuffer); 
    StrBuffer[0]=toupper(StrBuffer[0]);
  }
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_GENTEXT_FILE]);
    cfg_value[TOK_GENTEXT_FILE]=allocstr(StrBuffer); 
  }


  printf("\nDo you want display the gender of individuals [%s] (Y/N) ? ",cfg_value[TOK_DISPLAY_GENDERS]);
  StrBuffer[0]='L';
  while((StrBuffer[0]!='Y')&&(StrBuffer[0]!='N')&&(StrBuffer[0]!='\0')){
    gets(StrBuffer); 
    StrBuffer[0]=toupper(StrBuffer[0]);
  }
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_DISPLAY_GENDERS]);
    cfg_value[TOK_DISPLAY_GENDERS]=allocstr(StrBuffer);
  }


  printf("\nDo you want to eclipse(hide) dates on living individuals [%s] (Y/N) ?",cfg_value[TOK_HIDE_LIVING]);
  StrBuffer[0]='L';
  while((StrBuffer[0]!='Y')&&(StrBuffer[0]!='N')&&(StrBuffer[0]!='\0')){
    gets(StrBuffer); 
    StrBuffer[0]=toupper(StrBuffer[0]);
  }
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_HIDE_LIVING]);
    cfg_value[TOK_HIDE_LIVING]=allocstr(StrBuffer);
  }

  if(strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0){
    printf("\nGED2WWW relies on the GEDCOM file containing LVG tags in order to\n");
    printf("determine if people are alive or not.  If your GEDCOM does not have\n");
    printf("LVG tags (many do not) GED2WWW can try and guess which people\n");
    printf("may be alive based on their birth and/or marriage dates.\n");

    printf("\nAssume people born after ???? may be alive [%s] : ",cfg_value[TOK_DEAD_BIRTH]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0')
      if(atol(StrBuffer)>0){
        FreeIfNotNull(cfg_value[TOK_DEAD_BIRTH]);
	cfg_value[TOK_DEAD_BIRTH]=allocstr(StrBuffer);
      }
       
    printf("\nAssume people married after ???? may be alive [%s] : ",cfg_value[TOK_DEAD_MARRIAGE]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0')
      if(atol(StrBuffer)>0){
        FreeIfNotNull(cfg_value[TOK_DEAD_MARRIAGE]);
	cfg_value[TOK_DEAD_MARRIAGE]=allocstr(StrBuffer);
      }
  }

  printf("\nDo you want to display individual's birth and death dates on the\n");
  printf("Index pages [%s] (Y/N) ?",cfg_value[TOK_DATES_ON_INDEX]);
  StrBuffer[0]='L';
  while((StrBuffer[0]!='Y')&&(StrBuffer[0]!='N')&&(StrBuffer[0]!='\0')){
    gets(StrBuffer); 
    StrBuffer[0]=toupper(StrBuffer[0]);
  }
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_DATES_ON_INDEX]);
    cfg_value[TOK_DATES_ON_INDEX]=allocstr(StrBuffer);
  }

  printf("\nDo you want to select custom colors for your HTML pages [%s] (Y/N) ?",cfg_value[TOK_COLOR_YN]);
  StrBuffer[0]='L';
  while((StrBuffer[0]!='Y')&&(StrBuffer[0]!='N')&&(StrBuffer[0]!='\0')){
    gets(StrBuffer); 
    StrBuffer[0]=toupper(StrBuffer[0]);
  }
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_COLOR_YN]);
    cfg_value[TOK_COLOR_YN]=allocstr(StrBuffer);
  }

  if(strcmp(cfg_value[TOK_COLOR_YN],"Y")==0){
    printf("\nColors are entered as a 6 character string.  Each character must be a\n");
    printf("number 0-9 or a letter A-F.  These codes corespond to colors.  Be sure to\n");
    printf("check out your page's apearance before uploading, some color choices can\n");
    printf("render a page unreadable.  Some sample color codes are listed below:\n\n");
    printf("FFFFFF - White       000000 - Black        BEBEBE - Grey\n");
    printf("8000FF - Purple      0000FF - Blue         00FFFF - Cyan\n");
    printf("00FF00 - Green       FFFF00 - Yellow       A52A2A - Brown\n");
    printf("FF8000 - Orange      FF0000 - Red\n\n");

    printf("\nEnter the background color [%s] :",cfg_value[TOK_COLOR_BACKGROUND]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      while(!is_valid_color(StrBuffer)){
        printf("  That is not a valid color code.\n");
        gets(StrBuffer);
      }
      FreeIfNotNull(cfg_value[TOK_COLOR_BACKGROUND]);
      cfg_value[TOK_COLOR_BACKGROUND]=allocstr(StrBuffer);
    }
 
    printf("\nEnter the color for normal text [%s] :",cfg_value[TOK_COLOR_NORMAL]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      while(!is_valid_color(StrBuffer)){
        printf("  That is not a valid color code.\n");
        gets(StrBuffer);
      }
      FreeIfNotNull(cfg_value[TOK_COLOR_NORMAL]);
      cfg_value[TOK_COLOR_NORMAL]=allocstr(StrBuffer);
    }

    printf("\nEnter the color for links that have not yet been followed [%s] :",cfg_value[TOK_COLOR_NOT_FOLLOWED]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      while(!is_valid_color(StrBuffer)){
        printf("  That is not a valid color code.\n");
        gets(StrBuffer);
      }
      FreeIfNotNull(cfg_value[TOK_COLOR_NOT_FOLLOWED]);
      cfg_value[TOK_COLOR_NOT_FOLLOWED]=allocstr(StrBuffer);
    }

    printf("\nEnter the color for links that have already been followed [%s] :",cfg_value[TOK_COLOR_FOLLOWED]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      while(!is_valid_color(StrBuffer)){
        printf("  That is not a valid color code.\n");
        gets(StrBuffer);
      }
      FreeIfNotNull(cfg_value[TOK_COLOR_FOLLOWED]);
      cfg_value[TOK_COLOR_FOLLOWED]=allocstr(StrBuffer);
    }
  }

  printf("\nWhat extension do you want to use for the pages produced\n");
  printf("  1.. .htm\n");
  printf("  2.. .html\n");
  printf(" [%s] : ",cfg_value[TOK_EXTENSION]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    long idx;
    idx=atol(StrBuffer);
    if(idx==1){
      FreeIfNotNull(cfg_value[TOK_EXTENSION]);
      cfg_value[TOK_EXTENSION]=allocstr(".htm");
    } 
    if(idx==2){
      FreeIfNotNull(cfg_value[TOK_EXTENSION]);
      cfg_value[TOK_EXTENSION]=allocstr(".html");
    }
  }

  do{
    printf("\nEnter the filename for the base page.\n ");
    printf("Do not enter the trailing .htm or .html suffix [%s] : ",cfg_value[TOK_BASEPAGE]);

    gets(StrBuffer);

    fname_good=0;
    StrBuffer[8]='\0';

    cptr=StrBuffer;
    while(*cptr!='\0'){
      if((*cptr!='_')&&(!isalnum(*cptr)))
        if(fname_good==0){    
          printf("\nError. You have entered an invalid filename for the base page.\n"); 
          printf("The filename must consist of all letters, numbers and underscores.\n"); 
          fname_good=1; 
        }
      cptr++;
    }

    if((StrBuffer[0]!='\0')&&(fname_good==0)){
      FreeIfNotNull(cfg_value[TOK_BASEPAGE]);
      cfg_value[TOK_BASEPAGE]=allocstr(StrBuffer);
    }

  }while(fname_good==1);

  printf("\nTo use a background image on the HTML pages, enter the image file\n");
  printf("name here.  Note: the image file should be placed in the same\n");
  printf("location as the HTML pages [%s] : ",cfg_value[TOK_BACKGROUNDIMAGE]);

  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_BACKGROUNDIMAGE]);
    cfg_value[TOK_BACKGROUNDIMAGE]=allocstr(StrBuffer);
  }

  printf("==================database owner information====================\n");


  printf("Enter the database title [%s]\n : ",cfg_value[TOK_TITLE]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_TITLE]);
    cfg_value[TOK_TITLE]=allocstr(StrBuffer);
  }
  while(strlen(cfg_value[TOK_TITLE])==0){
    printf("You must enter a title for your page.\n");
    printf("Enter the database title [%s]\n : ",cfg_value[TOK_TITLE]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      FreeIfNotNull(cfg_value[TOK_TITLE]);
      cfg_value[TOK_TITLE]=allocstr(StrBuffer);
    }
  }

  printf("Enter the name of the database owner [%s]\n : ",cfg_value[TOK_OWNER_NAME]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_OWNER_NAME]);
    cfg_value[TOK_OWNER_NAME]=allocstr(StrBuffer);
  }
  while(strlen(cfg_value[TOK_OWNER_NAME])==0){
    printf("You must enter the name of the database owner.\n");
    printf("Enter the name of the database owner [%s]\n : ",cfg_value[TOK_OWNER_NAME]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      FreeIfNotNull(cfg_value[TOK_OWNER_NAME]);
      cfg_value[TOK_OWNER_NAME]=allocstr(StrBuffer);
    }
  }



  strcpy(UserAddress[0],cfg_value[TOK_OWNER_ADD_0]);
  strcpy(UserAddress[1],cfg_value[TOK_OWNER_ADD_1]);
  strcpy(UserAddress[2],cfg_value[TOK_OWNER_ADD_2]);
  strcpy(UserAddress[3],cfg_value[TOK_OWNER_ADD_3]);
  strcpy(UserAddress[4],cfg_value[TOK_OWNER_ADD_4]);
  strcpy(UserAddress[5],cfg_value[TOK_OWNER_ADD_5]);
  strcpy(UserAddress[6],cfg_value[TOK_OWNER_ADD_6]);
  strcpy(UserAddress[7],cfg_value[TOK_OWNER_ADD_7]);
  strcpy(UserAddress[8],cfg_value[TOK_OWNER_ADD_8]);
  strcpy(UserAddress[9],cfg_value[TOK_OWNER_ADD_9]);

  StrBuffer[0]='X';
  printf("Press <ENTER> on a blank line to stop entering the address.\n");
  for(ctr=0;(ctr<ADDRLEN)&&((StrBuffer[0]!='\0')||(UserAddress[ctr][0]!='\0'));ctr++){
    printf("Enter Address line %li [%s]\n : ",ctr+1,UserAddress[ctr]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0')
      strcpy(UserAddress[ctr],StrBuffer);
  }

  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_0]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_1]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_2]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_3]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_4]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_5]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_6]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_7]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_8]);
  FreeIfNotNull(cfg_value[TOK_OWNER_ADD_9]);
  
  cfg_value[TOK_OWNER_ADD_0]=allocstr(UserAddress[0]);
  cfg_value[TOK_OWNER_ADD_1]=allocstr(UserAddress[1]);
  cfg_value[TOK_OWNER_ADD_2]=allocstr(UserAddress[2]);
  cfg_value[TOK_OWNER_ADD_3]=allocstr(UserAddress[3]);
  cfg_value[TOK_OWNER_ADD_4]=allocstr(UserAddress[4]);
  cfg_value[TOK_OWNER_ADD_5]=allocstr(UserAddress[5]);
  cfg_value[TOK_OWNER_ADD_6]=allocstr(UserAddress[6]);
  cfg_value[TOK_OWNER_ADD_7]=allocstr(UserAddress[7]);
  cfg_value[TOK_OWNER_ADD_8]=allocstr(UserAddress[8]);
  cfg_value[TOK_OWNER_ADD_9]=allocstr(UserAddress[9]);

  printf("Enter the Phone Number of the database owner [%s]\n : ",cfg_value[TOK_OWNER_PHONE]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_OWNER_PHONE]);
    cfg_value[TOK_OWNER_PHONE]=allocstr(StrBuffer);
  }

  printf("Enter the e-mail address of the database owner [%s]\n : ",cfg_value[TOK_OWNER_EMAIL]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_OWNER_EMAIL]);
    cfg_value[TOK_OWNER_EMAIL]=allocstr(StrBuffer);
  }
  while(strlen(cfg_value[TOK_OWNER_EMAIL])==0){
    printf("You must enter the e-mail address of the database owner.\n");
    printf("Enter the e-mail address of the database owner [%s]\n : ",cfg_value[TOK_OWNER_EMAIL]);
    gets(StrBuffer);
    if(StrBuffer[0]!='\0'){
      FreeIfNotNull(cfg_value[TOK_OWNER_EMAIL]);
      cfg_value[TOK_OWNER_EMAIL]=allocstr(StrBuffer);
    }
  }

  printf("Enter the homepage of the database owner [%s]\n : ",cfg_value[TOK_OWNER_HOMEPAGE]);
  gets(StrBuffer);
  if(StrBuffer[0]!='\0'){
    FreeIfNotNull(cfg_value[TOK_OWNER_HOMEPAGE]);
    cfg_value[TOK_OWNER_HOMEPAGE]=allocstr(StrBuffer);
  }

  printf("===============================================\n");

  printf("GEDCOM FILENAME:\n  %s\n",cfg_value[TOK_GEDCOM_NAME]);
  printf("FAMILY GROUPS PER PAGE           : %s\n",cfg_value[TOK_FAM_PER_PG]);
  printf("GENERATE GENDEX.TXT FILE         :    %s\n",cfg_value[TOK_GENTEXT_FILE]);
  printf("SHOW INDIVIDUAL'S GENDER         :    %s\n",cfg_value[TOK_DISPLAY_GENDERS]);
  printf("HIDE DATES ON LIVING INDIVIDUALS :    %s\n",cfg_value[TOK_HIDE_LIVING]);
  if(strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0){ 
    printf("ASSUME ALIVE IF BORN AFTER       : %s\n",cfg_value[TOK_DEAD_BIRTH]);
    printf("ASSUME ALIVE IF MARRIED AFTER    : %s\n",cfg_value[TOK_DEAD_MARRIAGE]);
  }
  printf("SHOW DATES ON INDEX PAGES        :    %s\n",cfg_value[TOK_DATES_ON_INDEX]);
  printf("USE CUSTOM COLORS                :    %s\n",cfg_value[TOK_COLOR_YN]);
  if(strcmp(cfg_value[TOK_COLOR_YN],"Y")==0){
    printf("  BACKGROUND COLOR        : %s\n",cfg_value[TOK_COLOR_BACKGROUND]);
    printf("  TEXT COLOR              : %s\n",cfg_value[TOK_COLOR_NORMAL]);
    printf("  LINK COLOR              : %s\n",cfg_value[TOK_COLOR_NOT_FOLLOWED]);
    printf("  FOLLOWED LINK COLOR     : %s\n",cfg_value[TOK_COLOR_FOLLOWED]);
  }
  printf("EXTENSION FOR HTML FILES : %s\n",cfg_value[TOK_EXTENSION]);
  printf("BASE PAGE WILL BE : %s%s\n",cfg_value[TOK_BASEPAGE],cfg_value[TOK_EXTENSION]);
  if(cfg_value[TOK_BACKGROUNDIMAGE][0]=='\0')
    printf("BACKGROUND IMAGE : (NONE)\n");
   else
    printf("BACKGROUND IMAGE : %s\n",cfg_value[TOK_BACKGROUNDIMAGE]);
  printf("DATABASE TITLE:\n");
  if(strlen(cfg_value[TOK_TITLE])>0) 
    printf("  %s\n",cfg_value[TOK_TITLE]);
  printf("OWNER NAME:\n");
  if(strlen(cfg_value[TOK_OWNER_NAME])>0) 
    printf("  %s\n",cfg_value[TOK_OWNER_NAME]);
  printf("OWNER ADDRESS:\n");
  for(ctr=0;ctr<ADDRLEN;ctr++)
    if(UserAddress[ctr][0]!='\0')
      printf("  %s\n",UserAddress[ctr]);
  printf("OWNER PHONE:\n");
  if(strlen(cfg_value[TOK_OWNER_PHONE])>0) 
    printf("  %s\n",cfg_value[TOK_OWNER_PHONE]);
  printf("OWNER E-MAIL:\n");
  if(strlen(cfg_value[TOK_OWNER_EMAIL])>0) 
    printf("  %s\n",cfg_value[TOK_OWNER_EMAIL]);
  printf("OWNER HOMEPAGE:\n");
  if(strlen(cfg_value[TOK_OWNER_HOMEPAGE])>0) 
    printf("  %s\n",cfg_value[TOK_OWNER_HOMEPAGE]);
  printf("===============================================\n");

  printf("Is this info correct (Y/N) ? ");
  StrBuffer[0]='L';
  while((StrBuffer[0]!='Y')&&(StrBuffer[0]!='N')){
    gets(StrBuffer); 
    StrBuffer[0]=toupper(StrBuffer[0]);
  }
  yn=StrBuffer[0]; 
 }while(yn=='N');
}

printf("===============================================\n");
log_msg(__FILE__,__LINE__,"Starting processing");

log_msg(__FILE__,__LINE__,"Saving Options");
save_options(cfgfile_name);

NumFamily=0;
NumIndivid=0;

log_msg(__FILE__,__LINE__,"Allocating Filebuffer");
filebuffer=(char *)malloc(sizeof(char)*FILEBUFFER_SIZE);
if(filebuffer==NULL){
  printf("ERROR allocating file buffer, GED2WWW exiting\n");
  exit(0);
}

log_msg(__FILE__,__LINE__,"opening input file, pass 1");
GedcomLineCtr=0;
WarningCtr=0;
gedin=fopen(cfg_value[TOK_GEDCOM_NAME],"r");
setvbuf(gedin,filebuffer,_IOFBF,FILEBUFFER_SIZE);
if(gedin==NULL){
  printf("**************************************************\n");
  printf("Error opening '%s' file for read\n",cfg_value[TOK_GEDCOM_NAME]);
  printf("GED2WWW aborting\n");
  exit(1);
}


log_msg(__FILE__,__LINE__,"processing gedcom file, pass 1 - start");
printf("...Processing GEDCOM (pass 1/2)\n");
PreProcessGed(gedin);
log_msg(__FILE__,__LINE__,"processing gedcom file, pass 1 - complete");
printf("   Max number of Individuals = %7li\n",MAX_INDIVIDUALS-1);
printf("   Max number of Families    = %7li\n",MAX_FAMILIES-1);

if((MAX_FAMILIES<2)||(MAX_INDIVIDUALS<3)){
  printf("Error detected reading GEDCOM file, possible incorrect\n");
  printf("gedcom format, empty file, or non-gedcom file.\n");
  printf("GED2WWW aborting\n");
  exit(1);
}

log_msg(__FILE__,__LINE__,"Allocating internal data structures - start");
printf("...Allocating internal data structures\n");

log_msg(__FILE__,__LINE__,"  Allocating individual array %li",MAX_INDIVIDUALS);
individual=(struct individual_s *)malloc(sizeof(struct individual_s)*MAX_INDIVIDUALS);

log_msg(__FILE__,__LINE__,"  Allocating individual sort %li",MAX_INDIVIDUALS);
individual_sort=(long *)malloc(sizeof(long)*MAX_INDIVIDUALS);

log_msg(__FILE__,__LINE__,"  Allocating page location array %li",MAX_INDIVIDUALS);
page_location=(long *)malloc(sizeof(long)*MAX_INDIVIDUALS);

log_msg(__FILE__,__LINE__,"  Allocating families array %li",MAX_FAMILIES);
family=(struct family_s *)malloc(sizeof(struct family_s)*MAX_FAMILIES);

log_msg(__FILE__,__LINE__,"  Creating HASH table\n");
HashCreate(MAX_INDIVIDUALS+MAX_FAMILIES);

log_msg(__FILE__,__LINE__,"Allocating internal data structures - done");


if((individual==NULL)||
   (individual_sort==NULL)||
   (page_location==NULL)||
   (family==NULL)){
  printf("****************************************************\n");
  printf("Error allocating internal data structures\n");
#ifdef DOS
  printf("If you are running MS Windows, you may wish to exit it to\n");
  printf("increase available memory.\n");  
#endif
  printf("GED2WWW aborting\n");
  exit(0); 
}


log_msg(__FILE__,__LINE__,"Cleaning up individual arrays 0..%li",MAX_INDIVIDUALS-1);
for(ctr=0;ctr<MAX_INDIVIDUALS;ctr++){
  assert(ctr>=0);
  assert(ctr<MAX_INDIVIDUALS);
  individual[ctr].key=NULL;
  individual[ctr].raw_name=NULL;
  individual[ctr].title=NULL;
  individual[ctr].famc=NULL;
  individual[ctr].sortname=NULL;
  individual[ctr].surname=NULL;
  individual[ctr].remname=NULL;
  individual[ctr].birth.date=NULL;
  individual[ctr].birth.place=NULL;
  individual[ctr].death.date=NULL;
  individual[ctr].death.place=NULL;
  for(ctr2=0;ctr2<MAX_FAMS;ctr2++) {
    assert(ctr2>=0);
    assert(ctr2<MAX_FAMS);
    individual[ctr].fams[ctr2]=NULL;
  }
}
log_msg(__FILE__,__LINE__,"Cleaning up family arrays 0..%li",MAX_FAMILIES-1);
for(ctr=0;ctr<MAX_FAMILIES;ctr++){
  assert(ctr>=0);
  assert(ctr<MAX_FAMILIES);
  family[ctr].married.date=NULL;
  family[ctr].married.place=NULL;
  family[ctr].key=NULL;
  family[ctr].husband=NULL;
  family[ctr].wife=NULL;
  for(ctr2=0;ctr2<MAX_CHILDREN;ctr2++) {
    assert(ctr2>=0);
    assert(ctr2<MAX_CHILDREN);
    family[ctr].child[ctr2]=NULL;
  }
}


log_msg(__FILE__,__LINE__,"Done cleaning arrays, preparing for pass 2");

if(char_set==CHAR_ASCII)
  printf("   GEDCOM file using ASCII character set.\n");
if(char_set==CHAR_ANSEL)
  printf("   GEDCOM file using ANSEL character set.\n");
if(char_set==CHAR_UNICODE){
  printf("   GEDCOM file using UNICODE character set.  GED2WWW does not fully\n");
  printf("   support UNICODE.\n");
  char_set=CHAR_ASCII;
}
if(char_set==CHAR_UNKNOWN){
  printf("   GEDCOM file using UNKNOWN character set.  Treating as ASCII.\n");
  char_set=CHAR_ASCII;
}


printf("...Processing GEDCOM (pass 2/2)\n");
rewind(gedin);
GedcomLineCtr=0;
LoadGed(gedin);
if(fclose(gedin)!=0){
  printf("****************************************************\n");
  printf("Error colsing gedcom file\n");
  printf("GED2WWW aborting\n");
  exit(0);
}

if((strcmp(cfg_value[TOK_HIDE_LIVING],"Y")==0)&&(Lvg_Flags_Detected=='N')){
  GuessLiving(atol(cfg_value[TOK_DEAD_BIRTH]),
	      atol(cfg_value[TOK_DEAD_MARRIAGE]));
}

printf("...Parsing names\n");
BreakNames();
printf("...Determining page locations\n");
DeterminePage();
printf("...Sorting Database\n");
SortIndividuals(); 

ProducePages();


for(ctr=0;ctr<MAX_INDIVIDUALS;ctr++){
  FreeIfNotNull((void *)individual[ctr].key);
  FreeIfNotNull((void *)individual[ctr].raw_name);
  FreeIfNotNull((void *)individual[ctr].title);
  FreeIfNotNull((void *)individual[ctr].famc);
  FreeIfNotNull((void *)individual[ctr].sortname);
  FreeIfNotNull((void *)individual[ctr].surname);
  FreeIfNotNull((void *)individual[ctr].remname);
  FreeIfNotNull((void *)individual[ctr].birth.date);
  FreeIfNotNull((void *)individual[ctr].birth.place);
  FreeIfNotNull((void *)individual[ctr].death.date);
  FreeIfNotNull((void *)individual[ctr].death.place);
  for(ctr2=0;ctr2<MAX_FAMS;ctr2++) 
    FreeIfNotNull((void *)individual[ctr].fams[ctr2]);
}
for(ctr=0;ctr<MAX_FAMILIES;ctr++){
  FreeIfNotNull((void *)family[ctr].married.date);
  FreeIfNotNull((void *)family[ctr].married.place);
  FreeIfNotNull((void *)family[ctr].key);
  FreeIfNotNull((void *)family[ctr].husband);
  FreeIfNotNull((void *)family[ctr].wife);
  for(ctr2=0;ctr2<MAX_CHILDREN;ctr2++) 
    FreeIfNotNull((void *)family[ctr].child[ctr2]);
}
FreeIfNotNull(individual);
FreeIfNotNull(family);
FreeIfNotNull(individual_sort);
FreeIfNotNull(page_location);
for(ctr=0;ctr<NUM_TOP_SURN;ctr++){
   FreeIfNotNull(surname_name[ctr]);
}
HashDestroy();
FreeIfNotNull(filebuffer);
printf("...Done!\n");
printf("===============================================\n");
printf("The output HTML files can be found in the HTML subdirectory.\n");
printf("Thank you for using GED2WWW!\n");
}




int is_valid_color(char *clr){
int x;

for(x=0;x<6;x++){
  clr[x] = toupper(clr[x]);  
  if((char *)NULL==(char *)strchr("1234567890ABCDEF",clr[x]))
    return 0;
}
if(strlen(clr)!=6)
  return 0;
return 1;
}
