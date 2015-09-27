/*   general.h    */
/*
   include beforehand  the following:
#include <stdio.h>   // NULL FILE fopen feof fgets fclose fputs fputc gets
#include <stdlib.h>
   the function code is in 'mystring.c' .
*/
#include "ngspice/dstring.h"
#include "ngspice/bool.h"


typedef char string[258];


char *pscopy(SPICE_DSTRINGPTR s, const char *a, int i, int j);
char *pscopy_up(SPICE_DSTRINGPTR s, const char *a, int i, int j);
//bool scopyd(SPICE_DSTRINGPTR a, SPICE_DSTRINGPTR b);
int scopyd(SPICE_DSTRINGPTR a, SPICE_DSTRINGPTR b);
void scopys(SPICE_DSTRINGPTR a, const char *b);
void scopy_up(SPICE_DSTRINGPTR a, const char *str);
void scopy_lower(SPICE_DSTRINGPTR a, const char *str);
void ccopy(SPICE_DSTRINGPTR a, char c);
void sadd(SPICE_DSTRINGPTR s, const char *t);
void nadd(SPICE_DSTRINGPTR s, long n);
void cadd(SPICE_DSTRINGPTR s, char c);
void naddll(SPICE_DSTRINGPTR s, long long n);
void cins(SPICE_DSTRINGPTR s, char c);
void sins(SPICE_DSTRINGPTR s, const char *t);
int cpos(char c, char *s);
int spos_(char *sub, const char *s);
//bool ci_prefix(const char *p, const char *s);
int ci_prefix(const char *p, const char *s);
int length(const char *s);
//bool steq(const char *s, const char *t);
//bool stne(const char *s, const char *t);
int steq(const char *s, const char *t);
int stne(const char *s, const char *t);
void stri(long n, SPICE_DSTRINGPTR s);

char upcase(char c);
char lowcase(char c);
//bool alfa(char c);
//bool num(char c);
//bool alfanum(char c);
int alfa(char c);
int num(char c);
int alfanum(char c);
char *stupcase(char *s);

/***** primitive input-output ***/
int yes_or_no(void);
char rc(void);

int freadstr(FILE *f, SPICE_DSTRINGPTR dstr_p);

double np_round(double d);        // sjb to avoid clash with round() in math.h
double np_trunc(double x);        // sjb to avoid clash with trunc() in math.h
double absf(double x); /* abs */
long absi(long i);

void *new(size_t sz);
void dispose(void *p);
