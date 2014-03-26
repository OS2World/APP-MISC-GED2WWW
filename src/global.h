#include"defs.h" 

#ifndef h_global
#define h_global

#ifdef DEFINE_VARDEF
#define VARDEF
#else
#define VARDEF extern
#endif

VARDEF int _log;

VARDEF long MAX_INDIVIDUALS;
VARDEF long MAX_FAMILIES;

VARDEF long pctr;
VARDEF long NumFamily;   
VARDEF long NumIndivid;
VARDEF long SortCtr;

VARDEF long GedcomLineCtr;
VARDEF long WarningCtr;

VARDEF struct individual_s *individual;

VARDEF struct family_s *family;

VARDEF long *individual_sort ;
VARDEF long *page_location ;
VARDEF char alpha_usage[27];

VARDEF long num_surnames;

VARDEF char UserAddress[ADDRLEN][LONGSTRLEN];

VARDEF char Lvg_Flags_Detected;

VARDEF char datestring[STRLEN];
VARDEF int cur_year;

VARDEF int interactive_mode;
VARDEF char cfgfile_name[LONGSTRLEN];

VARDEF char *filebuffer;

VARDEF int char_set;

VARDEF int surname_count[NUM_TOP_SURN];
VARDEF char *surname_name[NUM_TOP_SURN];

VARDEF char *cfg_value[TOK_LAST];


#endif
