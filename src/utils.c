#include "global.h"
#include "utils.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<stdarg.h>
#include<time.h>

typedef struct hash_node_struct{
  char *key;
  void *data;
  struct hash_node_struct *next;
} hash_node;

long hash_table_size;

hash_node **hash_table;

long Hash(char *key);
void *HashLookup(char *key);



int my_strcasecmp(char *s1,char *s2){
  char t1[LONGSTRLEN];
  char t2[LONGSTRLEN];
  strcpylow(s1,t1);
  strcpylow(s2,t2);
  return strcmp(t1,t2);
}




char *allocstr(char *s){
char *r;

r=(char *)malloc(strlen(s)+1);
if(r==NULL){
  printf("ERROR Allocatiing string, insufficient memory, GED2WWW exiting\n");
  exit(1);
}
strcpy(r,s);
return r;
}


void HashCreate(long size){
long ctr;

  hash_table_size = size*2;
  log_msg(__FILE__,__LINE__,"Creating hash table %ld entries",hash_table_size);
  hash_table = (hash_node **)malloc(sizeof(hash_node *)*hash_table_size);
  if(hash_table==NULL){
    printf("ERROR Allocatiing hash table, insufficient memory, GED2WWW exiting\n");
    exit(1);
  }
  for(ctr=0;ctr<hash_table_size;ctr++) 
    hash_table[ctr] = NULL;  
}


void HashDestroy(){
long ctr;
hash_node *tmp;

for(ctr=0;ctr<hash_table_size;ctr++){
  while(hash_table[ctr]!=NULL){
    tmp=hash_table[ctr]->next;
    FreeIfNotNull(hash_table[ctr]->key);
    FreeIfNotNull(hash_table[ctr]);
    hash_table[ctr]=tmp;
  } 
}
FreeIfNotNull(hash_table);
}


void HashInsert(char *key, void *ptr){
hash_node *hn;
long index;

if(HashLookup(key)!=NULL){
 printf("ERROR, duplicate key value in hash table \"%s\"!\n",key);
 exit(1);
}
hn=(hash_node *)malloc(sizeof(hash_node));
if(hn==NULL){
  printf("ERROR Allocatiing hash node, insufficient memory, GED2WWW exiting\n");
  exit(1);
}
hn->key = allocstr(key);
hn->data = ptr;
index = Hash(key);
hn->next = hash_table[index];
hash_table[index] = hn;
}


void *HashLookup(char *key){
long hkey;
hash_node *hn;

hkey=Hash(key);
hn=hash_table[hkey];
while(hn!=NULL){
  if(strcmp(key,hn->key)==0){
    return hn->data; 
  }
  hn=hn->next;
}
return NULL;
}


long Hash(char *key){
long hash;
char *tmp;

tmp=key;

hash=0;

while(*key!='\0'){
  hash=(hash*5479+(*key)*5647)%hash_table_size;
  key++;
}

hash=hash%hash_table_size;


return hash;

}



void HashStats(){
long x;
long acc[11];
long ctr;
hash_node *ptr;

for(x=0;x<11;x++)
  acc[x]=0;

for(x=0;x<hash_table_size;x++){
  ptr=hash_table[x];
  ctr=0;
  while(ptr!=NULL){
    ptr = ptr -> next;
    ctr++;
  }
  if(ctr>10)
    ctr=10;
  acc[ctr]++;
}

printf("Hash stats---%li\n",hash_table_size);
for(x=0;x<11;x++)
 printf("%li   --   %li\n",x,acc[x]);

}




struct individual_s *FindIndi(char *key){
struct individual_s *iptr;

if(key==NULL) 
  return NULL;
iptr = (struct individual_s *)HashLookup(key);
if(iptr==NULL){
  printf("\nStructural problem in GEDCOM file.  A family references\n");
  printf("individual \"%s\", but an individual with that ID was\n",key);
  printf("not found in the GEDCOM file.\n");
  printf("Please check your genealogy database to ensure that all\n");
  printf("family/individual links are correct.\n\n");
  printf("GED2WWW exiting.\n"); 
  exit(0);
}
if(iptr->identifier!='I'){
  printf("Record \"%s\" is not an individual... error",key);
  exit(0);
}
return iptr;
}



struct family_s *FindFamily(char *key,struct individual_s *i){
struct family_s *fptr;

if(key==NULL) 
  return NULL;
fptr = (struct family_s *)HashLookup(key);
if(fptr==NULL){
  printf("\nError in GEDCOM structure\n");
  printf("Individual \"%s\" (id \"%s\") refrences family \"%s\"\n",
          i->raw_name,i->key,key);
  printf("but no family with that ID was found in GEDCOM file.\n");
  printf("Please check the family links around that individual,\n");
  printf("re-extract your GEDCOM file, and try again.\n\n");
  printf("GED2WWW exiting.\n"); 
  exit(0);
}
if(fptr->identifier!='F'){
  printf("Record \"%s\" is not a Family... error",key);
  exit(0);
}
return fptr;
}



void FreeIfNotNull(void *ptr){

if(ptr!=NULL){
 free(ptr); 
}
}



int NullString(char *s){

if(s==NULL)
  return 1;
else
  if(*s=='\0')
    return 1;
  else
    return 0;
}


long ExtractYear(char *s){
long year = 0;
char *c;

if(s==NULL)
  return 0;
if(s=='\0')
  return 0;

c=s;
while((*c)!='\0')
  c++;
while(!isdigit(*c))
  c--;
while(isdigit(*c))
  c--;
c++;
while(isdigit(*c)){
  year=year*10+((*c)-'0');
  c++;
}

return year;

}


char *ASCII_to_html(char *s){ /******** TO DO **********/
char buffer[LONGSTRLEN];
char *b;

b=buffer;
memset(buffer,'\0',LONGSTRLEN);
while(*s!='\0'){
  if(((*s)&0x80)==0){
    switch(*s){
      case '>':
        strcat(buffer,"&gt;");
        b+=4;
        break;
      case '<':
        strcat(buffer,"&lt;");
        b+=4;
        break;
      case '&':
        strcat(buffer,"&amp;");
        b+=5;
        break;
      case '"':
        strcat(buffer,"&quot;");
        b+=6;
        break;
      default: 
        *b=*s;
        b++;
    }
  }
  s++;
}
*b='\0';

return allocstr(buffer);
}


char *ASCII_to_text(char *s){
char buffer[LONGSTRLEN];
char *b;

b=buffer;
memset(buffer,'\0',LONGSTRLEN);
while(*s!='\0'){
  if(((*s)&0x80)==0){
    *b=*s;
    b++;
  }
  s++;
}
*b='\0';

return allocstr(buffer);
}


char *ANSEL_to_html(char *s){    /****** TO DO*****/
char buffer[LONGSTRLEN];
char *b;

b=buffer;
memset(buffer,'\0',LONGSTRLEN);
while(*s!='\0'){
  if(((*s)&0x80)==0){
    switch(*s){
      case '>':
        strcat(buffer,"&gt;");
        b+=4;
        break;
      case '<':
        strcat(buffer,"&lt;");
        b+=4;
        break;
      case '&':
        strcat(buffer,"&amp;");
        b+=5;
        break;
      case '"':
        strcat(buffer,"&quot;");
        b+=6;
        break;
      default: 
        *b=*s;
        b++;
    }
  }else{
    switch(*s){
      case (char)0xEA:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"ring;");
        b+=6;
        break;
      case (char)0xE8:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"uml;");
        b+=5;
        break;
      case (char)0xE2:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"acute;");
        b+=7;
        break;
      case (char)0xE1:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"grave;");
        b+=7;
        break;
      case (char)0xE4:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"tilde;");
        b+=7;
        break;
      case (char)0xE3:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"circ;");
        b+=6;
        break;
      case (char)0xF0:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"cedil;");
        b+=7;
        break;
      case (char)0xE5:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"marc;");
        b+=6;
        break;
      case (char)0xE6:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"brev;");
        b+=6;
        break;
      case (char)0xE7:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"dot;");
        b+=5;
        break;
      case (char)0xEE:
        strcat(buffer,"&");
        b++;
        s++;  
        *b=*s;
        strcat(buffer,"dblac;");
        b+=7;
        break;
      case (char)0xCF:
        strcat(buffer,"&szlig;");
        b+=7;
        break;
      case (char)0xA4:
        strcat(buffer,"&THORN;");
        b+=7;
        break;
      case (char)0xB4:
        strcat(buffer,"&thorn;");
        b+=7;
        break;
      case (char)0xA1:
        strcat(buffer,"&Lstrok;");
        b+=8;
        break;
      case (char)0xA2:
        strcat(buffer,"&Oslash;");
        b+=8;
        break;
      case (char)0xA3:
        strcat(buffer,"&Dstrok;");
        b+=8;
        break;
      case (char)0xA5:
        strcat(buffer,"&AElig;");
        b+=7;
        break;
      case (char)0xA6:
        strcat(buffer,"&OElig;");
        b+=7;
        break;
      case (char)0xB1:
        strcat(buffer,"&lstrok;");
        b+=8;
        break;
      case (char)0xB2:
        strcat(buffer,"&oslash;");
        b+=8;
        break;
      case (char)0xB3:
        strcat(buffer,"&dstrok;");
        b+=8;
        break;
      case (char)0xB5:
        strcat(buffer,"&aelig;");
        b+=7;
        break;
      case (char)0xB6:
        strcat(buffer,"&oelig;");
        b+=7;
        break;
      case (char)0xB8:
        strcat(buffer,"&inodot;");
        b+=8;
        break;
      case (char)0xBA:
        strcat(buffer,"&eth;");
        b+=5;
        break;
      case (char)0xC3:
        strcat(buffer,"&copy;");
        b+=6;
        break;
      default:
        log_msg(__FILE__,__LINE__,"Unrecognized ANSEL char 0x%X",(char)*s);
        break;
    }
  }
  s++;
}
*b='\0';

return allocstr(buffer);
}



char *ANSEL_to_text(char *s){    
char buffer[LONGSTRLEN];
char *b;

b=buffer;
memset(buffer,'\0',LONGSTRLEN);
while(*s!='\0'){
  if(((*s)&0x80)==0){
    *b=*s;
    b++;
  }
  s++;
}
*b='\0';

return allocstr(buffer);
}



char *to_html(char *s){
  switch (char_set){
    case CHAR_ASCII:
      return ASCII_to_html(s);
      break;
    case CHAR_ANSEL:
      return ANSEL_to_html(s);
      break;
    case CHAR_UNICODE:
      return ASCII_to_html(s);
      break;
    case CHAR_UNKNOWN:
      return ASCII_to_html(s);
      break;
    default:
      return ASCII_to_html(s);
      break;
  }
}

char *to_text(char *s){
  switch (char_set){
    case CHAR_ASCII:
      return ASCII_to_text(s);
      break;
    case CHAR_ANSEL:
      return ANSEL_to_text(s);
      break;
    case CHAR_UNICODE:
      return ASCII_to_text(s);
      break;
    case CHAR_UNKNOWN:
      return ASCII_to_text(s);
      break;
    default:
      return ASCII_to_text(s);
      break;
  }
}



void strcpylow(char *in,char *out){
  while(*in!='\0'){
    *out=tolower(*in);
    out++;
    in++;
  }
  *out='\0';
}



void log_init(){
  FILE *logfile;

  if (_log){
    logfile=fopen("log.txt","w");
    fclose(logfile);
  }
}



void log_msg(char *file,int line,const char *format, ...){
  FILE *logfile;
  va_list ap;
  char strbuf[200];


  if (_log){
    va_start(ap,format);
    logfile=fopen("log.txt","a");
    sprintf(strbuf,"%s(%04i) - %s\n",file,line,format);
    vfprintf(logfile,strbuf,ap);
    va_end(ap);
    fclose(logfile);
  }
}




void warning(const char *format, ...){
  va_list ap;
  char strbuf[200];

  WarningCtr++;

  va_start(ap,format);
  sprintf(strbuf,"\nWARNING : %s in gedcom line %ld\n",format,GedcomLineCtr);
  vprintf(strbuf,ap);
  va_end(ap);
}




void process_arg(char *s){

  if(strcmp(s,"-log")==0){
    _log = 1;
    return;
  }  
  if(strncmp(s,"-frontend:",10)==0){
    interactive_mode=0;
    strcpy(cfgfile_name,s+10);
    return;
  }
  if(strcmp(s,"-frontend")==0){
    interactive_mode=0;
    return;
  }



  printf("ERROR*******************\n");
  printf("Unrecognized command line parameter\n");
  printf(" \"%s\"\n",s);

  printf("Valid command line options are:\n");
  printf("  -log\n");
  printf("  -frontend\n");
  printf("  -frontend:<filename>\n");

  exit(1);

}
