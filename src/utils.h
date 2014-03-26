#ifndef H_UTILS
#define H_UTILS

#include<stdarg.h>

int my_strcasecmp(char *s1,char *s2);

char *allocstr(char *s);

void HashCreate(long size);
void HashDestroy();
void HashInsert(char *key,void *ptr);
void HashStats();

struct individual_s *FindIndi(char *key);
struct family_s *FindFamily(char *key,struct individual_s *i);
void FreeIfNotNull(void *ptr);

int NullString(char *s);

long ExtractYear(char *s);

char *ASCII_to_html(char *s);
char *ASCII_to_text(char *s);
char *ANSEL_to_html(char *s);
char *ANSEL_to_text(char *s);

char *to_html(char *s);
char *to_text(char *s);

void strcpylow(char *in,char *out);

void log_init();
void log_msg(char *file,int line,const char *format, ...);

void warning(const char *format, ...);

void process_arg(char *s);

#endif
