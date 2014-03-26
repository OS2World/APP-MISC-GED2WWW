#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

int indi_comp(const void *id1,const void *id2);


void BreakNames(){
int ictr;
char *sn,*rn,*nm,*s,*tt;
long numgood;
char surname[LONGSTRLEN];
char remname[LONGSTRLEN];
char sortname[LONGSTRLEN];
char title[LONGSTRLEN];

for(ictr=0;ictr<27;ictr++)
  alpha_usage[ictr]=0;
numgood=0;

      //individual[ictr].title=allocstr("");
      //  FreeIfNotNull(individual[ictr].title);
      //  if(NullString(individual[ictr].title)){
for(ictr=0;ictr<MAX_INDIVIDUALS;ictr++){
  if(individual[ictr].inuse=='Y'){
    /*compute SURNAME and REMNAME*/
    sn=surname;
    rn=remname; 
    strcpy(surname,"");
    strcpy(remname,"");
    if(!NullString(individual[ictr].raw_name)){
      nm=individual[ictr].raw_name;
      while((*nm!='\0')&&(*nm!='/')){
        *rn=*nm;
        rn++;
        nm++;
      }
      *rn='\0';
      if(*nm!='\0'){
        nm++;
        while((*nm!='\0')&&(*nm!='/')&&(*nm!='\n')&&(*nm!='\r')){
          *sn=*nm;
          sn++;
          nm++;
        }
      }
      *sn='\0';

      if(*nm!='\0'){
        nm++;
      }
      if((*nm)!='\0'){
        tt=to_html(nm);
        if(NullString(individual[ictr].title)){
          sprintf(title,"%s",tt);
        }else{
          sprintf(title,"%s%s",individual[ictr].title,tt);
        }
        FreeIfNotNull(individual[ictr].title);
        individual[ictr].title=allocstr(title);
        FreeIfNotNull(tt);
      }
    }

    if(surname[0]=='\0')
      sprintf(sortname,"~ %s",remname);
    else
      if(isalpha(surname[0]))
        sprintf(sortname,"%s %s",surname,remname);
      else
        sprintf(sortname,"~%s %s",surname,remname);

    s=sortname;
    while(*s!='\0'){
      *s=toupper(*s);
      s++;
    } 

    if(individual[ictr].title==NULL)
      individual[ictr].title=allocstr("");
    individual[ictr].surname=to_html(surname);
    individual[ictr].remname=to_html(remname); 
    individual[ictr].sortname=to_text(sortname);

    if(isalpha(individual[ictr].sortname[0])){ 
      alpha_usage[toupper(individual[ictr].sortname[0])-'A']=1;
    }else{
      alpha_usage[26]=1;
    }

/*    printf("%s -> %s, %s\n",
           individual[ictr].raw_name,
           individual[ictr].surname,
           individual[ictr].remname);*/
  if((individual[ictr].raw_name==NULL)||(individual[ictr].raw_name[0]=='\0'))
    individual[ictr].inuse='N';
 
  if(individual[ictr].inuse=='Y')
    numgood++;

  }

}

/*printf("numgood=%li\n",numgood);*/
/*for(ictr=0;ictr<27;ictr++)
  printf("%c:%i\n",ictr+65,alpha_usage[ictr]);
*/
}


void DeterminePage(){
long ictr;
long numn,nump,nums;
struct individual_s *male,*female,*spouse;
struct family_s *fptr;


for(ictr=0;ictr<MAX_INDIVIDUALS;ictr++) 
  page_location[ictr]=-1;

numn=0;       
nump=0;
nums=0;

/*first pass to fish out multiple marriages*/
for(ictr=0;ictr<MAX_INDIVIDUALS;ictr++)
  if((individual[ictr].inuse=='Y')&&(individual[ictr].detail_format=='\0'))
    if(individual[ictr].num_fams>1){
      individual[ictr].detail_format='N'; 
      page_location[pctr]=ictr;
      individual[ictr].page_index = pctr;
      pctr++;
      numn++;   
    }



/*normall pass to catch rest!*/
for(ictr=0;ictr<MAX_INDIVIDUALS;ictr++){
  if((individual[ictr].inuse=='Y')&&(individual[ictr].detail_format=='\0')){
    individual[ictr].detail_format='N'; 
    if(individual[ictr].num_fams==0){
      individual[ictr].detail_format='P'; 
    }
    if(individual[ictr].num_fams==1){
      individual[ictr].detail_format='S';
    }
    /*depending on detail format*/
    switch(individual[ictr].detail_format){
      /*N..Set page for self*/
      case 'N':
        page_location[pctr]=ictr;
        individual[ictr].page_index = pctr;
        pctr++;
        numn++;   
      break;
      /*P..LookUp/Set page for parents(father), assign there*/
      case 'P':
        fptr=FindFamily(individual[ictr].famc,&(individual[ictr])); 
        if(fptr==NULL){
          male=NULL;
          female=NULL;
        }else{
          male=FindIndi(fptr->husband);
          female=FindIndi(fptr->wife);
        }
        if((male!=NULL)&&(male->detail_format!='\0')){
          individual[ictr].page_index=male->page_index;
          nump++;
        }else{
          if((female!=NULL)&&(female->detail_format!='\0')){
            individual[ictr].page_index=female->page_index;
            nump++;
          }else{
            if(male!=NULL){
              /*assign father and set there*/
              page_location[pctr]=male->index;
              male->detail_format='N';
              male->page_index = pctr;
              pctr++; 
              individual[ictr].page_index=male->page_index;
              numn++;
              nump++; 
            }else{
              if(female!=NULL){
                /*assign mother and set there*/
                page_location[pctr]=female->index;
                female->detail_format='N';
                female->page_index = pctr;
                pctr++; 
                individual[ictr].page_index=female->page_index;
                numn++;
                nump++;
              }else{
                /*change to Normal listing and assign*/
                page_location[pctr]=ictr;
                individual[ictr].detail_format='N';
                individual[ictr].page_index = pctr;
                pctr++; 
                numn++;
              }
            }
          }
        }
      break;
      /*S..LookUp/Set page for Spouse, assign there*/                   
      case 'S':
        fptr=FindFamily(individual[ictr].fams[0],&(individual[ictr])); 
        if(fptr==NULL){
          male=NULL;
          female=NULL;
        }else{
          male=FindIndi(fptr->husband);
          female=FindIndi(fptr->wife);
        }
        if((male!=NULL)&&(male->index==ictr)){
          spouse=female;
        }else{
          spouse=male;
        }
        if(spouse==NULL){
          page_location[pctr]=ictr;
          individual[ictr].detail_format='N';
          individual[ictr].page_index = pctr;
          pctr++; 
          numn++;
        }else{
          if(spouse->detail_format=='\0'){
            page_location[pctr]=spouse->index;
            spouse->detail_format='N';
            spouse->page_index = pctr;
            pctr++; 
            individual[ictr].page_index=spouse->page_index;
            numn++;
            nums++; 
          }else{
            individual[ictr].page_index=spouse->page_index;
            nums++;
          }
        }
      break;
    }
  }
}
/*printf("num Normal : %ld\n",numn);*/
/*printf("num Spouse : %ld\n",nums);*/
/*printf("num Parent : %ld\n",nump);*/
}


void SortIndividuals(){
long x1,x2;

printf("...Determining valid individuals\n");
x2=0;
for(x1=0;x1<MAX_INDIVIDUALS;x1++){
  if(individual[x1].inuse=='Y'){
    individual_sort[x2]=x1;
    x2++;
  } 
}  
SortCtr=x2;

printf("   Number of Individuals : %7li\n",SortCtr);
printf("   Number of Families    : %7li\n",NumFamily);

printf("...Sorting names\n");

qsort(individual_sort,
      SortCtr,
      sizeof(long),
      *indi_comp);


/*for(x1=0;x1<SortCtr;x1++)
  printf("%li '%s', '%s' = '%s'\n",individual_sort[x1],
                    individual[individual_sort[x1]].surname,
                    individual[individual_sort[x1]].remname,
                    individual[individual_sort[x1]].raw_name);
*/       
}



int indi_comp(const void *id1,const void *id2){
long idx1,idx2;

idx1=*((long *)id1);
idx2=*((long *)id2);

if((idx1>=MAX_INDIVIDUALS)||(idx2>=MAX_INDIVIDUALS))
  return 0;

return strcmp(individual[idx1].sortname,
              individual[idx2].sortname);
}



void GuessLiving(long birthyear,long marryear){
long ictr;
long fctr;
struct family_s *famptr;

printf("...Guessing if individuals are alive or not\n");

for(ictr=0;ictr<MAX_INDIVIDUALS;ictr++){
  if((individual[ictr].inuse=='Y')&&
     (individual[ictr].death.flag!='Y')){
    if(ExtractYear(individual[ictr].birth.date)>birthyear){
      individual[ictr].death.flag='N';
    } 
    for(fctr=0;fctr<individual[ictr].num_fams;fctr++){
      famptr=FindFamily(individual[ictr].fams[fctr],&(individual[ictr]));
      if(famptr!=NULL){ 
        if(ExtractYear(famptr->married.date)>marryear){
          individual[ictr].death.flag='N';
        }      
      } 
    }
  }
}


}
      
