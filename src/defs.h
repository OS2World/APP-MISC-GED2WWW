#ifndef h_defs
#define h_defs

/*#define LH_DEBUG*/

#define VERSION         "0.31"

/* In MAKEFILE do                                   */
/* -DUNIX to compile unxer UNIX                     */
/* -DDOS  to compile under DOS                      */

#define MAX_CHILDREN    50 
#define MAX_FAMS        20
#define STRLEN         128
#define LONGSTRLEN    1024  
#define ADDRLEN         10
#define PERIOD_EVERY   500

#define FILEBUFFER_SIZE 8192

#define NUM_TOP_SURN 10

#define CHAR_ASCII       1
#define CHAR_UNKNOWN     0
#define CHAR_ANSEL       2 
#define CHAR_UNICODE     3 

#define TOK_GEDCOM_NAME          1
#define TOK_OWNER_NAME           2
#define TOK_OWNER_ADD_0          3
#define TOK_OWNER_ADD_1          4
#define TOK_OWNER_ADD_2          5
#define TOK_OWNER_ADD_3          6
#define TOK_OWNER_ADD_4          7
#define TOK_OWNER_ADD_5          8
#define TOK_OWNER_ADD_6          9
#define TOK_OWNER_ADD_7         10 
#define TOK_OWNER_ADD_8         11
#define TOK_OWNER_ADD_9         12
#define TOK_OWNER_PHONE         13
#define TOK_OWNER_EMAIL         14
#define TOK_OWNER_HOMEPAGE      15
#define TOK_TITLE               16
#define TOK_BASEPAGE            17
#define TOK_BACKGROUNDIMAGE     18
#define TOK_GENTEXT_FILE        19
#define TOK_DISPLAY_GENDERS     20
#define TOK_HIDE_LIVING         21
#define TOK_DATES_ON_INDEX      22
#define TOK_COLOR_BACKGROUND    23
#define TOK_COLOR_NORMAL        24
#define TOK_COLOR_FOLLOWED      25
#define TOK_COLOR_NOT_FOLLOWED  26
#define TOK_HTML_DIR            27
#define TOK_LANG_PARENTS        28
#define TOK_LANG_FATHER         29
#define TOK_LANG_MOTHER         30
#define TOK_LANG_LIVING         31
#define TOK_LANG_BIRTH          32
#define TOK_LANG_DEATH          33
#define TOK_LANG_GENDER         34
#define TOK_LANG_MALE           35
#define TOK_LANG_FEMALE         36
#define TOK_LANG_FAMILY         37
#define TOK_LANG_MARRIAGE       38
#define TOK_LANG_IN             39 
#define TOK_LANG_SPOUSE         40
#define TOK_LANG_CHILDREN       41

#define TOK_DEAD_MARRIAGE	42
#define TOK_DEAD_BIRTH		43
#define TOK_FAM_PER_PG		44
#define TOK_COLOR_YN		45
#define TOK_EXTENSION		46

#define TOK_LAST                47



typedef struct {
  char flag;
  char *place;
  char *date;
} detail;

struct individual_s{
  char    identifier;
  long    index;
  char    inuse;
  char    *key;
  char    *raw_name;
  char    *title;
  char    gender;
  detail  death;
  char    *famc;
  char    *fams[MAX_FAMS];
  detail  birth;
 /* COMPUTED FIELDS */
  long    page_index;
  char    detail_format;
  char    *surname;
  char    *remname;
  int     num_fams;
  int     num_famc;
  char    *sortname;
};

struct family_s{
  char    identifier;
  long    index;
  char    inuse;
  char    *key;
  char    *husband;
  char    *wife;
  char    *child[MAX_CHILDREN];
  detail  married;
 /* computed fields */
  int     num_child;
};

#endif
