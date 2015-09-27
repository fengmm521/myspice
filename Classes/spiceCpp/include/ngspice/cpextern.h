/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1986 Wayne A. Christopher, U. C. Berkeley CAD Group
Modified: 2000  AlansFixes
**********/

/*
 * Definitions for all external symbols in CP.
 */

#ifndef ngspice_CPEXTERN_H
#define ngspice_CPEXTERN_H

#include "ngspice/wordlist.h"
#include "ngspice/bool.h"
//typedef int bool;
#include <stdarg.h>

struct ccom;

/* com_alias.c */

extern struct alias *cp_aliases;
extern void cp_paliases(char *word);
extern void cp_setalias(char *word, wordlist *wlist);
extern void cp_unalias(char *word);

extern wordlist *cp_doalias(wordlist *wlist);

/* backquote.c */

extern char cp_back;
extern wordlist *cp_bquote(wordlist *wlist);

/* complete.c */

//extern bool cp_nocc;
//extern bool cp_comlook(char *word);

extern int cp_nocc;
extern int cp_comlook(char *word);

extern struct ccom *cp_kwswitch(int kw_class, struct ccom *tree);
extern void cp_addcomm(char *word, long int bits0, long int bits1, long int bits2, long int bits3);
extern void cp_addkword(int kw_class, char *word);
//extern void cp_ccom(wordlist *wlist, char *buf, bool esc);
//extern void cp_ccon(bool on);
//extern void cp_ccrestart(bool kwords);

extern void cp_ccom(wordlist *wlist, char *buf, int esc);
extern void cp_ccon(int on);
extern void cp_ccrestart(int kwords);

extern void cp_remcomm(char *word);
extern void cp_remkword(int kw_class, char *word);
extern void cp_destroy_keywords(void);

extern wordlist *cp_cctowl(struct ccom *stuff);

/* cshpar.c */

extern FILE *cp_in;
extern FILE *cp_out;
extern FILE *cp_err;
extern FILE *cp_curin;
extern FILE *cp_curout;
extern FILE *cp_curerr;
//extern bool cp_debug;
extern int cp_debug;
extern char cp_amp;
extern char cp_gt;
extern char cp_lt;
extern void cp_ioreset(void);
extern wordlist *cp_redirect(wordlist *wlist);
extern wordlist *cp_parse(char *string);

/* control.c */

//extern bool cp_cwait;
//extern bool cp_dounixcom;

extern int cp_cwait;
extern int cp_dounixcom;

extern char *cp_csep;
extern char * get_alt_prompt(void);
extern int cp_evloop(char *string);
extern void cp_resetcontrol(void);
extern void cp_toplevel(void);
extern void cp_popcontrol(void);
extern void cp_pushcontrol(void);

/* glob.c */

//extern bool cp_globmatch(char *p, char *s);
extern int cp_globmatch(char *p, char *s);

extern char *cp_tildexpand(char *string);
extern char cp_cbrac;
extern char cp_ccurl;
extern char cp_comma;
extern char cp_huh;
extern char cp_obrac;
extern char cp_ocurl;
extern char cp_star;
extern char cp_til;
extern wordlist *cp_doglob(wordlist *wlist);

/* history.c */

//extern bool cp_didhsubst;
extern int cp_didhsubst;

extern char cp_bang;
extern char cp_hat;
extern int cp_maxhistlength;
extern struct histent *cp_lastone;
extern void cp_addhistent(int event, wordlist *wlist);
//void cp_hprint(int eventhi, int eventlo, bool rev);
void cp_hprint(int eventhi, int eventlo, int rev);

extern wordlist *cp_histsubst(wordlist *wlist);

/* lexical.c */

extern FILE *cp_inp_cur;
//extern bool cp_bqflag;
//extern bool cp_interactive;

extern int cp_bqflag;
extern int cp_interactive;

extern char *cp_altprompt;
extern char *cp_promptstring;
extern char cp_hash;
extern int cp_event;
extern wordlist *cp_lexer(char *string);
extern int inchar(FILE *fp);

/* modify.c */

extern char cp_chars[];
extern void cp_init(void);

/* output.c */

//extern bool out_moremode;
//extern bool out_isatty;
extern int out_moremode;
extern int out_isatty;

extern void out_init(void);

#ifdef __GNUC__
extern void out_printf(char *fmt, ...) __attribute__ ((format (__printf__, 1, 2)));
#else
extern void out_printf(char *fmt, ...);
#endif

extern void out_vprintf(const char *fmt, va_list ap);
extern void out_send(char *string);

/* quote.c */

extern char *cp_unquote(char *string);
extern void cp_quoteword(char *str);
extern void cp_striplist(wordlist *wlist);
extern void cp_wstrip(char *str);



/* unixcom.c */

//extern bool cp_unixcom(wordlist *wlist);
extern int cp_unixcom(wordlist *wlist);

extern void cp_hstat(void);
//void cp_rehash(char *pathlist, bool docc);
void cp_rehash(char *pathlist, int docc);

/* variable.c */

enum cp_types {
  CP_BOOL,
  CP_NUM,
  CP_REAL,
  CP_STRING,
  CP_LIST
};

//extern bool cp_ignoreeof;
//extern bool cp_noclobber;
//extern bool cp_noglob;
//extern bool cp_nonomatch;

extern int cp_ignoreeof;
extern int cp_noclobber;
extern int cp_noglob;
extern int cp_nonomatch;

extern char cp_dol;
extern void cp_remvar(char *varname);
extern void cp_vset(char *varname, enum cp_types type, void *value);
extern struct variable *cp_setparse(wordlist *wl);
extern wordlist *vareval(char *string);
extern char *span_var_expr(char *t);

/* var2.c */
extern void cp_vprint(void);
//extern bool cp_getvar(char *name, enum cp_types type, void *retval);
extern int cp_getvar(char *name, enum cp_types type, void *retval);


/* cpinterface.c etc -- stuff CP needs from FTE */

//extern bool cp_istrue(wordlist *wl);
//extern bool cp_oddcomm(char *s, wordlist *wlist);
extern int cp_istrue(wordlist *wl);
extern int cp_oddcomm(char *s, wordlist *wlist);

extern void cp_doquit(void);
extern void cp_periodic(void);
extern void ft_cpinit(void);
extern struct comm *cp_coms;
extern char *cp_program;
extern struct variable *cp_enqvar(char *word);
extern void cp_usrvars(struct variable **v1, struct variable **v2);
//int cp_usrset(struct variable *var, bool isset);
int cp_usrset(struct variable *var, int isset);

extern void fatal(void);

#endif
