/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher
**********/

/*
  For dealing with spice input decks and command scripts

  Central function is inp_readall()
*/

#include "ngspice/ngspice.h"

#include "ngspice/cpdefs.h"
#include "ngspice/ftedefs.h"
#include "ngspice/fteext.h"
#include "ngspice/dvec.h"
#include "ngspice/fteinp.h"
#include "ngspice/compatmode.h"

#include <limits.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#if !defined(__MINGW32__) && !defined(_MSC_VER)
#include <unistd.h>
#endif

#include "inpcom.h"
#include "variable.h"
#include "subckt.h"
#include "../misc/util.h" /* ngdirname() */
#include "ngspice/stringutil.h"
#include "ngspice/wordlist.h"

#ifdef XSPICE
/* gtri - add - 12/12/90 - wbk - include new stuff */
#include "ngspice/ipctiein.h"
#include "ngspice/enh.h"
/* gtri - end - 12/12/90 */
#endif

/* SJB - Uncomment this line for debug tracing */
/*#define TRACE*/

/* globals -- wanted to avoid complicating inp_readall interface */
#define N_LIBRARIES       1000
#define N_PARAMS          1000
#define N_SUBCKT_W_PARAMS 4000

#define VALIDCHARS "!$%_#?@.[]&"

static struct library {
    char *realpath;
    char *habitat;
    struct line *deck;
} libraries[N_LIBRARIES];

static int  num_libraries;

struct names {
    char *names[N_SUBCKT_W_PARAMS];
    int  num_names;
};

struct function_env
{
    struct function_env *up;

    struct function {
        struct function *next;
        char *name;
        char *body;
        char *params[N_PARAMS];
        int   num_parameters;
        const char *accept;
    } *functions;
};

struct func_temper
{
    char* funcname;
    int subckt_depth;
    int subckt_count;
    struct func_temper *next;
};

//extern void line_free_x(struct line *deck, bool recurse);
extern void line_free_x(struct line *deck, int recurse);

static COMPATMODE_T inp_compat_mode;

/* Collect information for dynamic allocation of numparam arrays */
/* number of lines in input deck */
int dynmaxline;  /* inpcom.c 1529 */
/* number of lines in deck after expansion */
int dynMaxckt = 0; /* subckt.c 307 */
/* number of parameter substitutions */
long dynsubst; /* spicenum.c 221 */

/* Expression handling with 'temper' parameter required */
//bool expr_w_temper = FALSE;
int expr_w_temper = FALSE;


static char *readline(FILE *fd);
static int  get_number_terminals(char *c);
//static void inp_stripcomments_deck(struct line *deck, bool cs);
//static void inp_stripcomments_line(char *s, bool cs);
static void inp_stripcomments_deck(struct line *deck, int cs);
static void inp_stripcomments_line(char *s, int cs);
static void inp_fix_for_numparam(struct names *subckt_w_params, struct line *deck);
static void inp_remove_excess_ws(struct line *deck);
static void expand_section_references(struct line *deck, char *dir_name);
static void inp_grab_func(struct function_env *, struct line *deck);
static void inp_fix_inst_calls_for_numparam(struct names *subckt_w_params, struct line *deck);
static void inp_expand_macros_in_func(struct function_env *);
static struct line *inp_expand_macros_in_deck(struct function_env *, struct line *deck);
static void inp_fix_param_values(struct line *deck);
static void inp_reorder_params(struct names *subckt_w_params, struct line *list_head);
static int  inp_split_multi_param_lines(struct line *deck, int line_number);
static void inp_sort_params(struct line *param_cards, struct line *card_bf_start, struct line *s_c, struct line *e_c);
static char *inp_remove_ws(char *s);
static void inp_compat(struct line *deck);
static void inp_bsource_compat(struct line *deck);
static void inp_temper_compat(struct line *card);
static void inp_dot_if(struct line *deck);
static char *inp_modify_exp(char* expression);
static struct func_temper *inp_new_func(char *funcname, char *funcbody, struct line *card,
                                        int *sub_count, int subckt_depth);
static void inp_delete_funcs(struct func_temper *funcs);

//static bool chk_for_line_continuation(char *line);
static int chk_for_line_continuation(char *line);
static void comment_out_unused_subckt_models(struct line *start_card);
static void inp_fix_macro_param_func_paren_io(struct line *begin_card);
static void inp_fix_gnd_name(struct line *deck);
static void inp_chk_for_multi_in_vcvs(struct line *deck, int *line_number);
static void inp_add_control_section(struct line *deck, int *line_number);
static char *get_quoted_token(char *string, char **token);
static void replace_token(char *string, char *token, int where, int total);
static void inp_add_series_resistor(struct line *deck);
static void subckt_params_to_param(struct line *deck);
static void inp_fix_temper_in_param(struct line *deck);

static char *skip_back_non_ws(char *d) { while (d[-1] && !isspace(d[-1])) d--; return d; }
static char *skip_back_ws(char *d)     { while (isspace(d[-1]))           d--; return d; }
static char *skip_non_ws(char *d)      { while (*d && !isspace(*d)) d++; return d; }
static char *skip_ws(char *d)          { while (isspace(*d))        d++; return d; }

static char *skip_back_non_ws_(char *d, char *start) { while (d > start && !isspace(d[-1])) d--; return d; }
static char *skip_back_ws_(char *d, char *start)     { while (d > start && isspace(d[-1])) d--; return d; }

static char *inp_spawn_brace(char *s);

static char *inp_pathresolve(const char *name);
static char *inp_pathresolve_at(char *name, char *dir);
static char *search_plain_identifier(char *str, const char *identifier);
void tprint(struct line *deck);

struct inp_read_t
{ struct line *cc;
    int line_number;
};

//static struct inp_read_t inp_read(FILE *fp, int call_depth, char *dir_name, bool comfile, bool intfile);
static struct inp_read_t inp_read(FILE *fp, int call_depth, char *dir_name, int comfile, int intfile);


#ifndef XSPICE
static void inp_poly_err(struct line *deck);
#endif


static struct line *
xx_new_line(struct line *next, char *line, int linenum, int linenum_orig)
{
    struct line *x = TMALLOC(struct line, 1);

    x->li_next = next;
    x->li_error = NULL;
    x->li_actual = NULL;
    x->li_line = line;
    x->li_linenum = linenum;
    x->li_linenum_orig = linenum_orig;

    return x;
}


static struct library *
new_lib(void)
{
    if (num_libraries >= N_LIBRARIES) {
        fprintf(stderr, "ERROR, N_LIBRARIES overflow\n");
        controlled_exit(EXIT_FAILURE);
    }

    return & libraries[num_libraries++];
}


static void
delete_libs(void)
{
    int i;

    for (i = 0; i < num_libraries; i++) {
        tfree(libraries[i].realpath);
        tfree(libraries[i].habitat);
        line_free_x(libraries[i].deck, TRUE);
    }
}


static struct library *
find_lib(char *name)
{
    int i;

    for (i = 0; i < num_libraries; i++)
        if (cieq(libraries[i].realpath, name))
            return & libraries[i];

    return NULL;
}


static struct line *
find_section_definition(struct line *c, char *name)
{
    for (; c; c = c->li_next) {

        char *line = c->li_line;

        if (ciprefix(".lib", line)) {

            char *s, *t, *y;

            s = skip_non_ws(line);
            while (isspace(*s) || isquote(*s))
                s++;
            for (t = s; *t && !isspace(*t) && !isquote(*t); t++)
                ;
            y = t;
            while (isspace(*y) || isquote(*y))
                y++;

            if (!*y) {
                /* library section definition: `.lib <section-name>' .. `.endl' */

                char keep_char = *t;
                *t = '\0';

                if (strcasecmp(name, s) == 0) {
                    *t = keep_char;
                    return c;
                }

                *t = keep_char;
            }
        }
    }

    return NULL;
}


static struct library *
read_a_lib(char *y, char *dir_name)
{
    char *yy, *y_resolved;

    struct library *lib;

    y_resolved = inp_pathresolve_at(y, dir_name);

    if (!y_resolved) {
        fprintf(cp_err, "Error: Could not find library file %s\n", y);
        return NULL;
    }

#if defined(__MINGW32__) || defined(_MSC_VER)
    yy = _fullpath(NULL, y_resolved, 0);
#else
    yy = realpath(y_resolved, NULL);
#endif

    if (!yy) {
        fprintf(cp_err, "Error: Could not `realpath' library file %s\n", y);
        controlled_exit(EXIT_FAILURE);
    }

    lib = find_lib(yy);

    if (!lib) {

        FILE *newfp = fopen(y_resolved, "r");

        if (!newfp) {
            fprintf(cp_err, "Error: Could not open library file %s\n", y);
            return NULL;
        }

        /* lib points to a new entry in global lib array libraries[N_LIBRARIES] */
        lib = new_lib();

        lib->realpath = strdup(yy);
        lib->habitat = ngdirname(yy);

        lib->deck = inp_read(newfp, 1 /*dummy*/, lib->habitat, FALSE, FALSE) . cc;

        fclose(newfp);
    }

    free(yy);
    free(y_resolved);

    return lib;
}


static struct names *
new_names(void)
{
    struct names *p = TMALLOC(struct names, 1);
    p -> num_names = 0;

    return p;
}


static void
delete_names(struct names *p)
{
    int i;
    for (i = 0; i < p->num_names; i++)
        tfree(p->names[i]);
    tfree(p);
}



/* line1
   + line2
   ---->
   line1 line 2
   Proccedure: store regular card in prev, skip comment lines (*..) and some others
   */

static void
inp_stitch_continuation_lines(struct line *working)
{
    struct line *prev = NULL;

    while (working) {
        char *s, c, *buffer;

        for (s = working->li_line; (c = *s) != '\0' && c <= ' '; s++)
            ;

#ifdef TRACE
        /* SDB debug statement */
        printf("In inp_read, processing linked list element line = %d, s = %s . . . \n", working->li_linenum, s);
#endif

        switch (c) {
        case '#':
        case '$':
        case '*':
        case '\0':
            /* skip these cards, and keep prev as the last regular card */
            working = working->li_next;  /* for these chars, go to next card */
            break;

        case '+':   /* handle continuation */
            if (!prev) {
                working->li_error = copy("Illegal continuation line: ignored.");
                working = working->li_next;
                break;
            }

            /* We now may have lept over some comment lines, which are located among
            the continuation lines. We have to delete them here to prevent a memory leak */
            while (prev->li_next != working) {
                struct line *tmpl = prev->li_next->li_next;
                line_free_x(prev->li_next, FALSE);
                prev->li_next = tmpl;
            }

            /* create buffer and write last and current line into it. */
            buffer = tprintf("%s %s", prev->li_line, s + 1);

            /* replace prev->li_line by buffer */
            s = prev->li_line;
            prev->li_line = buffer;
            prev->li_next = working->li_next;
            working->li_next = NULL;
            /* add original line to prev->li_actual */
            if (prev->li_actual) {
                struct line *end;
                for (end = prev->li_actual; end->li_next; end = end->li_next)
                    ;
                end->li_next = working;
                tfree(s);
            } else {
                prev->li_actual = xx_new_line(working, s, prev->li_linenum, 0);
            }
            working = prev->li_next;
            break;

        default:  /* regular one-line card */
            prev = working;
            working = working->li_next;
            break;
        }
    }
}


/*
 * search for `=' assignment operator
 *   take care of `!=' `<=' `==' and `>='
 */

static char *
find_assignment(char *str)
{
    char *p = str;

    while ((p = strchr(p, '=')) != NULL) {

        // check for equality '=='
        if (p[1] == '=') {
            p += 2;
            continue;
        }

        // check for '!=', '<=', '>='
        if (p > str)
            if (p[-1] == '!' || p[-1] == '<' || p[-1] == '>') {
                p += 1;
                continue;
            }

        return p;
    }

    return NULL;
}


/*-------------------------------------------------------------------------
  Read the entire input file and return  a pointer to the first line of
  the linked list of 'card' records in data.  The pointer is stored in
  *data.
  Called from fcn inp_spsource() in inp.c to load circuit or command files.
  Called from fcn com_alter_mod() in device.c to load model files.
  Called from here to load .library or .include files.

  Procedure:
  read in all lines & put them in the struct cc
  read next line
  process .TITLE line
  store contents in string new_title
  process .lib lines
  read file and library name, open file using fcn inp_pathopen()
  read file contents and put into struct libraries[].deck, one entry per .lib line
  process .inc lines
  read file and library name, open file using fcn inp_pathopen()
  read file contents and add lines to cc
  make line entry lower case
  allow for shell end of line continuation (\\)
  add '+' to beginning of next line
  add line entry to list cc
  add '.global gnd'
  add libraries
  find library section
  add lines
  add .end card
  strip end-of-line comments
  make continuation lines a single line
  *** end of processing for command files ***
  start preparation of input deck for numparam
  ...
  debug printout to debug-out.txt
  *-------------------------------------------------------------------------*/

//struct line *
//inp_readall(FILE *fp, char *dir_name, bool comfile, bool intfile)
struct line *
inp_readall(FILE *fp, char *dir_name, int comfile, int intfile)
{
    struct line *cc;
    struct inp_read_t rv;

    num_libraries = 0;
    inp_compat_mode = ngspice_compat_mode();

    rv = inp_read(fp, 0, dir_name, comfile, intfile);
    cc = rv . cc;

    /* The following processing of an input file is not required for command files
       like spinit or .spiceinit, so return command files here. */

    if (!comfile && cc) {

        unsigned int no_braces; /* number of '{' */
        size_t max_line_length; /* max. line length in input deck */
        struct line *tmp_ptr1;
        struct names *subckt_w_params = new_names();

        struct line *working = cc->li_next;

        delete_libs();
        inp_fix_for_numparam(subckt_w_params, working);


        inp_remove_excess_ws(working);

        comment_out_unused_subckt_models(working);

        subckt_params_to_param(working);

        rv . line_number = inp_split_multi_param_lines(working, rv . line_number);

        inp_fix_macro_param_func_paren_io(working);
        inp_fix_temper_in_param(working);

        inp_expand_macros_in_deck(NULL, working);
        inp_fix_param_values(working);

        inp_reorder_params(subckt_w_params, cc);
        inp_fix_inst_calls_for_numparam(subckt_w_params, working);

        delete_names(subckt_w_params);
        subckt_w_params = NULL;

        inp_fix_gnd_name(working);
        inp_chk_for_multi_in_vcvs(working, &rv. line_number);

        if (cp_getvar("addcontrol", CP_BOOL, NULL))
            inp_add_control_section(working, &rv . line_number);
#ifndef XSPICE
        inp_poly_err(working);
#endif
        if (inp_compat_mode != COMPATMODE_SPICE3) {
            /* Do all the compatibility stuff here */
            working = cc->li_next;
            /* E, G, L, R, C compatibility transformations */
            inp_compat(working);
            working = cc->li_next;
            /* B source numparam compatibility transformation */
            inp_bsource_compat(working);
            inp_dot_if(working);
            inp_temper_compat(working);
        }

        inp_add_series_resistor(working);

        /* get max. line length and number of lines in input deck,
           and renumber the lines,
           count the number of '{' per line as an upper estimate of the number
           of parameter substitutions in a line*/
        dynmaxline = 0;
        max_line_length = 0;
        no_braces = 0;
        for (tmp_ptr1 = cc; tmp_ptr1; tmp_ptr1 = tmp_ptr1->li_next) {
            char *s;
            unsigned int braces_per_line = 0;
            /* count number of lines */
            dynmaxline++;
            /* renumber the lines of the processed input deck */
            tmp_ptr1->li_linenum = dynmaxline;
            if (max_line_length < strlen(tmp_ptr1->li_line))
                max_line_length = strlen(tmp_ptr1->li_line);
            /* count '{' */
            for (s = tmp_ptr1->li_line; *s; s++)
                if (*s == '{')
                    braces_per_line++;
            if (no_braces <  braces_per_line)
                no_braces = braces_per_line;
        }

        if (ft_ngdebug) {
            /*debug: print into file*/
            FILE *fd = fopen("debug-out.txt", "w");
            struct line *t;
            fprintf(fd, "**************** uncommented deck **************\n\n");
            /* always print first line */
            fprintf(fd, "%6d  %6d  %s\n", cc->li_linenum_orig, cc->li_linenum, cc->li_line);
            /* here without out-commented lines */
            for (t = cc->li_next; t; t = t->li_next) {
                if (*(t->li_line) == '*')
                    continue;
                fprintf(fd, "%6d  %6d  %s\n", t->li_linenum_orig, t->li_linenum, t->li_line);
            }
            fprintf(fd, "\n****************** complete deck ***************\n\n");
            /* now completely */
            for (t = cc; t; t = t->li_next)
                fprintf(fd, "%6d  %6d  %s\n", t->li_linenum_orig, t->li_linenum, t->li_line);
            fclose(fd);

            fprintf(stdout, "max line length %d, max subst. per line %d, number of lines %d\n",
                    (int) max_line_length, no_braces, dynmaxline);
        }
    }

    return cc;
}


//struct inp_read_t
//inp_read(FILE *fp, int call_depth, char *dir_name, bool comfile, bool intfile)
struct inp_read_t
inp_read(FILE *fp, int call_depth, char *dir_name, int comfile, int intfile)
/* fp: in, pointer to file to be read,
   call_depth: in, nested call to fcn
   dir_name: in, name of directory of file to be read
   comfile: in, TRUE if command file (e.g. spinit, .spiceinit)
   intfile: in, TRUE if deck is generated from internal circarray
*/
{
    struct inp_read_t rv;
    struct line *end = NULL, *cc = NULL;
    char *buffer = NULL;
    /* segfault fix */
#ifdef XSPICE
    char big_buff[5000];
    int line_count = 0;
#endif
    char *new_title = NULL;
    int line_number = 1; /* sjb - renamed to avoid confusion with struct line */
    int line_number_orig = 1;
    int cirlinecount = 0; /* length of circarray */
    static int is_control = 0; /* We are reading from a .control section */

    //bool found_end = FALSE, shell_eol_continuation = FALSE;
    int found_end = FALSE, shell_eol_continuation = FALSE;

    /* First read in all lines & put them in the struct cc */
    for (;;) {
        /* derive lines from circarray */
        if (intfile) {
            char *p;
            buffer = circarray[cirlinecount++];
            if (!buffer) {
                tfree(circarray);
                break;
            }
            p = skip_ws(buffer);
            if (buffer < p)
                memmove(buffer, p, strlen(p) + 1);
        }
        /* read lines from file fp */
        else {

#ifdef XSPICE
            /* gtri - modify - 12/12/90 - wbk - read from mailbox if ipc enabled */

            /* If IPC is not enabled, do equivalent of what SPICE did before */
            if (! g_ipc.enabled) {
                if (call_depth == 0 && line_count == 0) {
                    line_count++;
                    if (fgets(big_buff, 5000, fp))
                        buffer = copy(big_buff);
                } else {
                    buffer = readline(fp);
                    if (!buffer)
                        break;
                }
            } else {
                /* else, get the line from the ipc channel. */
                /* We assume that newlines are not sent by the client */
                /* so we add them here */
                char         ipc_buffer[1025];  /* Had better be big enough */
                int          ipc_len;
                Ipc_Status_t ipc_status =
                    ipc_get_line(ipc_buffer, &ipc_len, IPC_WAIT);
                if (ipc_status == IPC_STATUS_END_OF_DECK) {
                    buffer = NULL;
                    break;
                } else if (ipc_status == IPC_STATUS_OK) {
                    buffer = TMALLOC(char, strlen(ipc_buffer) + 3);
                    strcpy(buffer, ipc_buffer);
                    strcat(buffer, "\n");
                } else {            /* No good way to report this so just die */
                    controlled_exit(EXIT_FAILURE);
                }
            }

            /* gtri - end - 12/12/90 */
#else

            buffer = readline(fp);
            if(!buffer)
                break;

#endif
        }

#ifdef TRACE
        /* SDB debug statement */
        printf("in inp_read, just read   %s", buffer);
#endif

        if (!buffer)
            continue;

        /* OK -- now we have loaded the next line into 'buffer'.  Process it. */
        /* If input line is blank, ignore it & continue looping.  */
        if ((strcmp(buffer, "\n") == 0) || (strcmp(buffer, "\r\n") == 0))
            if (call_depth != 0 || (call_depth == 0 && cc != NULL)) {
                line_number_orig++;
                tfree(buffer);  /* was allocated by readline() */
                continue;
            }

        if (*buffer == '@') {
            tfree(buffer);      /* was allocated by readline() */
            break;
        }

        /* now check if we are in a .control section */
        if (ciprefix(".control", buffer))
            is_control++;
        else if (ciprefix(".endc", buffer))
            is_control--;

        /* now handle .title statement */
        if (ciprefix(".title", buffer)) {
            char *s;
            s = skip_non_ws(buffer);   /* skip over .title */
            s = skip_ws(s);            /* advance past space chars */

            /* only the last title line remains valid */
            tfree(new_title);
            new_title = copy(s);
            if ((s = strchr(new_title, '\n')) != NULL)
                *s = ' ';
            *buffer = '*';      /* change .TITLE line to comment line */
        }

        /* now handle old style .lib entries */
        /* new style .lib entries handling is in expand_section_references() */
        if (ciprefix(".lib", buffer))
            if (inp_compat_mode == COMPATMODE_PS) {
                /* compatibility mode,
                 *   this is neither a libray section definition nor a reference
                 * interpret as old style
                 *   .lib <file name> (no lib name given)
                 */
                char *s = skip_non_ws(buffer); /* skip over .lib */
                fprintf(cp_err, "  File included as:   .inc %s\n", s);
                memcpy(buffer, ".inc", 4);
            }

        /* now handle .include statements */
        if (ciprefix(".include", buffer) || ciprefix(".inc", buffer)) {

            char *y = NULL;
            char *s;

            struct line *newcard;

            inp_stripcomments_line(buffer, FALSE);

            s = skip_non_ws(buffer);               /* advance past non-space chars */

            s = get_quoted_token(s, &y);

            if (!y) {
                fprintf(cp_err, "Error: .include filename missing\n");
                tfree(buffer);  /* was allocated by readline() */
                controlled_exit(EXIT_FAILURE);
            }

            {
                char *y_resolved = inp_pathresolve_at(y, dir_name);
                char *y_dir_name;
                FILE *newfp;

                if (!y_resolved) {
                    fprintf(cp_err, "Error: Could not find include file %s\n", y);
                    rv . line_number = line_number;
                    rv . cc = NULL;
                    return rv;
                }

                newfp = fopen(y_resolved, "r");

                if (!newfp) {
                    fprintf(cp_err, "Error: .include statement failed.\n");
                    tfree(buffer);          /* allocated by readline() above */
                    controlled_exit(EXIT_FAILURE);
                }

                y_dir_name = ngdirname(y_resolved);

                newcard = inp_read(newfp, call_depth+1, y_dir_name, FALSE, FALSE) . cc;  /* read stuff in include file into netlist */

                tfree(y_dir_name);
                tfree(y_resolved);

                (void) fclose(newfp);
            }

            /* Make the .include a comment */
            *buffer = '*';

            /* append `buffer' to the (cc, end) chain of decks */
            {
                struct line *x = xx_new_line(NULL, copy(buffer), line_number, line_number);

                if (end)
                    end->li_next = x;
                else
                    cc = x;

                end = x;

                line_number++;
            }

            if (newcard) {
                int line_number_inc = 1;
                end->li_next = newcard;
                /* Renumber the lines */
                for (end = newcard; end && end->li_next; end = end->li_next) {
                    end->li_linenum = line_number++;
                    end->li_linenum_orig = line_number_inc++;
                }
                end->li_linenum = line_number++;        /* SJB - renumber the last line */
                end->li_linenum_orig = line_number_inc++;       /* SJB - renumber the last line */
            }

            /* Fix the buffer up a bit. */
            (void) strncpy(buffer + 1, "end of: ", 8);
        }   /*  end of .include handling  */

        /* loop through 'buffer' until end is reached.  Then test for
           premature end.  If premature end is reached, spew
           error and zap the line. */
        {
            char *s;
            /* no lower case letters for lines beginning with: */
            if ( !ciprefix("write", buffer) &&
                 !ciprefix("wrdata", buffer) &&
                 !ciprefix(".lib", buffer) &&
                 !ciprefix(".inc", buffer) &&
                 !ciprefix("codemodel", buffer) &&
                 !ciprefix("echo", buffer) &&
                 !ciprefix("shell", buffer) &&
                 !ciprefix("source", buffer) &&
                 !ciprefix("load", buffer)
                )
            {
                /* lower case for all lines (exceptions see above!) */
                for (s = buffer; *s && (*s != '\n'); s++)
                    *s = (char) tolower(*s);
            } else {
                /* exclude some commands to preserve filename case */
                for (s = buffer; *s && (*s != '\n'); s++)
                    ;
            }

            if (!*s) {
                // fprintf(cp_err, "Warning: premature EOF\n");
            }
            *s = '\0';      /* Zap the newline. */

            if ((s-1) >= buffer && *(s-1) == '\r') /* Zop the carriage return under windows */
                *(s-1) = '\0';
        }

        /* find the true .end command out of .endc, .ends, .endl, .end (comments may follow) */
        if (ciprefix(".end", buffer))
            if ((buffer[4] == '\0') || isspace(buffer[4])) {
                found_end = TRUE;
                *buffer   = '*';
            }

        if (shell_eol_continuation) {
            char *new_buffer = tprintf("+%s", buffer);

            tfree(buffer);
            buffer = new_buffer;
        }

        /* If \\ at end of line is found, next line in loop will get + (see code above) */
        shell_eol_continuation = chk_for_line_continuation(buffer);

        {
            struct line *x = xx_new_line(NULL, copy(buffer), line_number++, line_number_orig++);

            if (end)
                end->li_next = x;
            else
                cc = x;

            end = x;
        }

        tfree(buffer);
    }  /* end while ((buffer = readline(fp)) != NULL) */

    if (!end) /* No stuff here */
    {
        rv . line_number = line_number;
        rv . cc = NULL;
        return rv;
    }

    if (call_depth == 0 && !comfile) {
        cc->li_next = xx_new_line(cc->li_next, copy(".global gnd"), 1, 0);

        if (inp_compat_mode == COMPATMODE_ALL ||
            inp_compat_mode == COMPATMODE_HS  ||
            inp_compat_mode == COMPATMODE_NATIVE)
        {
            /* process all library section references */
            expand_section_references(cc, dir_name);
        }
    }

    /*
      add a terminal ".end" card
    */

    if (call_depth == 0 && !comfile) {
        if (found_end == TRUE) {
            struct line *x = xx_new_line(NULL, copy(".end"), line_number++, line_number_orig++);
            end->li_next = x;
            end = x;
        }
    }

    /* Replace first line with the new title, if available */
    if (call_depth == 0 && !comfile && new_title) {
        tfree(cc->li_line);
        cc->li_line = new_title;
    }

    /* Strip or convert end-of-line comments.
       Afterwards stitch the continuation lines.
       If the line only contains an end-of-line comment then it is converted
       into a normal comment with a '*' at the start.  Some special handling
       if this is a command file or called from within a .control section. */
    inp_stripcomments_deck(cc->li_next, comfile || is_control);

    inp_stitch_continuation_lines(cc->li_next);

    rv . line_number = line_number;
    rv . cc = cc;
    return rv;
}


//static bool
//is_absolute_pathname(const char *p)
static int
is_absolute_pathname(const char *p)
{
#if defined(__MINGW32__) || defined(_MSC_VER)
    /* /... or \... or D:\... or D:/... */
    return
        p[0] == DIR_TERM  ||
        p[0] == DIR_TERM_LINUX  ||
        (isalpha(p[0]) && p[1] == ':' &&
         (p[2] == DIR_TERM_LINUX || p[2] == DIR_TERM));
#else
    return
        p[0] == DIR_TERM;
#endif
}


#if 0

static bool
is_plain_filename(const char *p)
{
#if defined(__MINGW32__) || defined(_MSC_VER)
    return
        !strchr(p, DIR_TERM) &&
        !strchr(p, DIR_TERM_LINUX);
#else
    return
        !strchr(p, DIR_TERM);
#endif
}

#endif


FILE *
inp_pathopen(char *name, char *mode)
{
    char *path = inp_pathresolve(name);

    if (path) {
        FILE *fp = fopen(path, mode);
        tfree(path);
        return fp;
    }

    return NULL;
}


/*-------------------------------------------------------------------------*
  Look up the variable sourcepath and try everything in the list in order
  if the file isn't in . and it isn't an abs path name.
  *-------------------------------------------------------------------------*/

static char *
inp_pathresolve(const char *name)
{
    char buf[BSIZE_SP];
    struct variable *v;
    struct stat st;

#if defined(__MINGW32__) || defined(_MSC_VER)

    /* If variable 'mingwpath' is set: convert mingw /d/... to d:/... */
    if (cp_getvar("mingwpath", CP_BOOL, NULL) && name[0] == DIR_TERM_LINUX && isalpha(name[1]) && name[2] == DIR_TERM_LINUX) {
        strcpy(buf, name);
        buf[0] = buf[1];
        buf[1] = ':';
        return inp_pathresolve(buf);
    }

#endif

    /* just try it */
    if (stat(name, &st) == 0)
        return copy(name);

    /* fail if this was an absolute filename or if there is no sourcepath var */
    if (is_absolute_pathname(name) || !cp_getvar("sourcepath", CP_LIST, &v))
        return NULL;

    for (; v; v = v->va_next) {

        switch (v->va_type) {
        case CP_STRING:
            cp_wstrip(v->va_string);
            (void) sprintf(buf, "%s%s%s", v->va_string, DIR_PATHSEP, name);
            break;
        case CP_NUM:
            (void) sprintf(buf, "%d%s%s", v->va_num, DIR_PATHSEP, name);
            break;
        case CP_REAL:           /* This is foolish */
            (void) sprintf(buf, "%g%s%s", v->va_real, DIR_PATHSEP, name);
            break;
        default:
            fprintf(stderr, "ERROR: enumeration value `CP_BOOL' or `CP_LIST' not handled in inp_pathresolve\nAborting...\n");
            controlled_exit(EXIT_FAILURE);
            break;
        }

        if (stat(buf, &st) == 0)
            return copy(buf);
    }

    return (NULL);
}


static char *
inp_pathresolve_at(char *name, char *dir)
{
    char buf[BSIZE_SP], *end;

    /* if name is an absolute path name,
     *   or if we haven't anything to prepend anyway
     */

    if (is_absolute_pathname(name) || !dir || !dir[0])
        return inp_pathresolve(name);

    if (name[0] == '~' && name[1] == '/') {
        char *y = cp_tildexpand(name);
        if (y) {
            char *r = inp_pathresolve(y);
            tfree(y);
            return r;
        }
    }

    /* concatenate them */

    strcpy(buf, dir);

    end = strchr(buf, '\0');
    if (end[-1] != DIR_TERM)
        *end++ = DIR_TERM;

    strcpy(end, name);

    return inp_pathresolve(buf);
}


/*-------------------------------------------------------------------------*
 *  This routine reads a line (of arbitrary length), up to a '\n' or 'EOF' *
 *  and returns a pointer to the resulting null terminated string.         *
 *  The '\n' if found, is included in the returned string.                 *
 *  From: jason@ucbopal.BERKELEY.EDU (Jason Venner)                        *
 *  Newsgroups: net.sources                                                *
 *-------------------------------------------------------------------------*/

#define STRGROW 256

static char *
readline(FILE *fd)
{
    int c;
    int memlen;
    char *strptr;
    int strlen;

    strlen = 0;
    memlen = STRGROW;
    strptr = TMALLOC(char, memlen);
    memlen -= 1;                /* Save constant -1's in while loop */

    while ((c = getc(fd)) != EOF) {

        if (strlen == 0 && (c == '\t' || c == ' ')) /* Leading spaces away */
            continue;

        strptr[strlen++] = (char) c;

        if (strlen >= memlen) {
            memlen += STRGROW;
            if ((strptr = TREALLOC(char, strptr, memlen + 1)) == NULL)
                return (NULL);
        }

        if (c == '\n')
            break;
    }

    if (!strlen) {
        tfree(strptr);
        return (NULL);
    }

    // strptr[strlen] = '\0';
    /* Trim the string */
    strptr = TREALLOC(char, strptr, strlen + 1);
    strptr[strlen] = '\0';

    return (strptr);
}


/* replace "gnd" by " 0 "
   Delimiters of gnd may be ' ' or ',' or '(' or ')' */

static void
inp_fix_gnd_name(struct line *c)
{
    for (; c; c = c->li_next) {

        char *gnd = c->li_line;

        // if there is a comment or no gnd, go to next line
        if ((*gnd == '*') || !strstr(gnd, "gnd"))
            continue;

        // replace "?gnd?" by "? 0 ?", ? being a ' '  ','  '('  ')'.
        while ((gnd = strstr(gnd, "gnd")) != NULL) {
            if ((isspace(gnd[-1]) || gnd[-1] == '(' || gnd[-1] == ',') &&
                (isspace(gnd[3]) || gnd[3] == ')' || gnd[3] == ',')) {
                memcpy(gnd, " 0 ", 3);
            }
            gnd += 3;
        }

        // now remove the extra white spaces around 0
        c->li_line = inp_remove_ws(c->li_line);
    }
}


/*
 * transform a VCVS "gate" instance into a XSPICE instance
 *
 *   Exx  out+ out-  <VCVS>  {nand|nor|and|or}(n)
 *   +  in[1]+ in[1]- ... in[n]+ in[n]-
 *   +  x1,y1 x2,y2
 * ==>
 *   Axx  %vd[ in[1]+ in[1]- ... in[n]+ in[n]- ]
 *   +    %vd( out+ out- )  Exx
 *   .model Exx multi_input_pwd ( x = [x1 x2] x = [y1 y2] model = {nand|nor|and|or} )
 *
 * fixme,
 *   `n' is not checked
 *   the x,y list is fixed to length 2
 */

static void
inp_chk_for_multi_in_vcvs(struct line *c, int *line_number)
{
    int skip_control = 0;

    for (; c; c = c->li_next) {

        char *line = c->li_line;

        /* there is no e source inside .control ... .endc */
        if (ciprefix(".control", line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }

        if (*line == 'e') {

            char *fcn_b;

            if ((fcn_b = strstr(line, "nand(")) != NULL ||
                (fcn_b = strstr(line, "and(")) != NULL ||
                (fcn_b = strstr(line, "nor(")) != NULL ||
                (fcn_b = strstr(line, "or(")) != NULL)
            {
                struct line *a_card, *model_card, *next_card;
                char keep, *comma_ptr, *xy_values1[5], *xy_values2[5];
                char *out_str, *ctrl_nodes_str, *xy_values1_b, *ref_str, *fcn_name, *fcn_e, *out_b, *out_e, *ref_e;
                char *m_instance, *m_model;
                char *xy_values2_b, *xy_values1_e, *ctrl_nodes_b, *ctrl_nodes_e;
                int  xy_count1, xy_count2;
                //bool ok = FALSE;
                int ok = FALSE;

                do {
                    ref_e = skip_non_ws(line);

                    out_b = skip_ws(ref_e);

                    out_e = skip_back_ws_(fcn_b, out_b);
                    if (out_e <= out_b)
                        break;

                    fcn_e = strchr(fcn_b, '(');

                    ctrl_nodes_b = strchr(fcn_e, ')');
                    if (!ctrl_nodes_b)
                        break;
                    ctrl_nodes_b = skip_ws(ctrl_nodes_b + 1);

                    comma_ptr = strchr(ctrl_nodes_b, ',');
                    if (!comma_ptr)
                        break;

                    xy_values1_b = skip_back_ws_(comma_ptr, ctrl_nodes_b);
                    if (xy_values1_b[-1] == '}') {
                        while (--xy_values1_b >= ctrl_nodes_b)
                            if (*xy_values1_b == '{')
                                break;
                    } else {
                        xy_values1_b = skip_back_non_ws_(xy_values1_b, ctrl_nodes_b);
                    }
                    if (xy_values1_b <= ctrl_nodes_b)
                        break;

                    ctrl_nodes_e = skip_back_ws_(xy_values1_b, ctrl_nodes_b);
                    if (ctrl_nodes_e <= ctrl_nodes_b)
                        break;

                    xy_values1_e = skip_ws(comma_ptr + 1);
                    if (*xy_values1_e == '{') {
                        xy_values1_e = inp_spawn_brace(xy_values1_e);
                    } else {
                        xy_values1_e = skip_non_ws(xy_values1_e);
                    }
                    if (!xy_values1_e)
                        break;

                    xy_values2_b = skip_ws(xy_values1_e);

                    ok = TRUE;
                } while(0);

                if (!ok) {
                    fprintf(stderr, "ERROR: malformed line: %s\n", line);
                    controlled_exit(EXIT_FAILURE);
                }

                ref_str = copy_substring(line, ref_e);
                out_str = copy_substring(out_b, out_e);
                fcn_name = copy_substring(fcn_b, fcn_e);
                ctrl_nodes_str = copy_substring(ctrl_nodes_b, ctrl_nodes_e);

                keep = *xy_values1_e;
                *xy_values1_e = '\0';
                xy_count1 = get_comma_separated_values(xy_values1, xy_values1_b);
                *xy_values1_e = keep;

                xy_count2 = get_comma_separated_values(xy_values2, xy_values2_b);

                // place restrictions on only having 2 point values; this can change later
                if (xy_count1 != 2 && xy_count2 != 2)
                    fprintf(stderr, "ERROR: only expecting 2 pair values for multi-input vcvs!\n");

                m_instance = tprintf("%s %%vd[ %s ] %%vd( %s ) %s",
                                     ref_str, ctrl_nodes_str, out_str, ref_str);
                m_instance[0] = 'a';
                a_card = xx_new_line(NULL, m_instance, (*line_number)++, 0);

                m_model = tprintf(".model %s multi_input_pwl ( x = [%s %s] y = [%s %s] model = \"%s\" )",
                                  ref_str, xy_values1[0], xy_values2[0],
                                  xy_values1[1], xy_values2[1], fcn_name);
                model_card = xx_new_line(NULL, m_model, (*line_number)++, 0);

                tfree(ref_str);
                tfree(out_str);
                tfree(fcn_name);
                tfree(ctrl_nodes_str);
                tfree(xy_values1[0]);
                tfree(xy_values1[1]);
                tfree(xy_values2[0]);
                tfree(xy_values2[1]);

                *c->li_line = '*';
                next_card   = c->li_next;
                c->li_next  = a_card;
                a_card->li_next     = model_card;
                model_card->li_next = next_card;
            }
        }
    }
}


static void
inp_add_control_section(struct line *deck, int *line_number)
{
    struct line *c, *prev_card = NULL;
//    bool        found_control = FALSE, found_run = FALSE;
//    bool        found_end = FALSE;
    int        found_control = FALSE, found_run = FALSE;
    int        found_end = FALSE;
    char        *op_line  = NULL, rawfile[1000], *line;

    for (c = deck; c; c = c->li_next) {

        if (*c->li_line == '*')
            continue;

        if (ciprefix(".op ", c->li_line)) {
            *c->li_line = '*';
            op_line = c->li_line + 1;
        }

        if (ciprefix(".end", c->li_line))
            found_end = TRUE;

        if (found_control && ciprefix("run", c->li_line))
            found_run = TRUE;

        if (ciprefix(".control", c->li_line))
            found_control = TRUE;

        if (ciprefix(".endc", c->li_line)) {
            found_control = FALSE;

            if (!found_run) {
                prev_card->li_next = xx_new_line(c, copy("run"), (*line_number)++, 0);
                prev_card = prev_card->li_next;
                found_run = TRUE;
            }

            if (cp_getvar("rawfile", CP_STRING, rawfile)) {
                line = tprintf("write %s", rawfile);
                prev_card->li_next = xx_new_line(c, line, (*line_number)++, 0);
                prev_card = prev_card->li_next;
            }
        }

        prev_card = c;
    }

    // check if need to add control section
    if (!found_run && found_end) {

        deck->li_next = xx_new_line(deck->li_next, copy(".endc"), (*line_number)++, 0);

        if (cp_getvar("rawfile", CP_STRING, rawfile)) {
            line = tprintf("write %s", rawfile);
            deck->li_next = xx_new_line(deck->li_next, line, (*line_number)++, 0);
        }

        if (op_line)
            deck->li_next = xx_new_line(deck->li_next, copy(op_line), (*line_number)++, 0);

        deck->li_next = xx_new_line(deck->li_next, copy("run"), (*line_number)++, 0);

        deck->li_next = xx_new_line(deck->li_next, copy(".control"), (*line_number)++, 0);
    }
}


// look for shell-style end-of-line continuation '\\'

//static bool
//chk_for_line_continuation(char *line)
static int
chk_for_line_continuation(char *line)
{
    if (*line != '*' && *line != '$') {

        char *ptr = skip_back_ws_(strchr(line, '\0'), line);

        if ((ptr - 2 >= line) && (ptr[-1] == '\\') && (ptr[-2] == '\\')) {
            ptr[-1] = ' ';
            ptr[-2] = ' ';
            return TRUE;
        }
    }

    return FALSE;
}


//
// change .macro --> .subckt
//        .eom   --> .ends
//        .subckt name 1 2 3 params: w=9u l=180n --> .subckt name 1 2 3 w=9u l=180n
//        .subckt name (1 2 3) --> .subckt name 1 2 3
//        x1 (1 2 3)      --> x1 1 2 3
//        .param func1(x,y) = {x*y} --> .func func1(x,y) {x*y}

static void
inp_fix_macro_param_func_paren_io(struct line *card)
{
    char        *str_ptr, *new_str;

    for (; card; card = card->li_next) {

        if (*card->li_line == '*')
            continue;

        if (ciprefix(".macro", card->li_line) || ciprefix(".eom", card->li_line)) {
            str_ptr = skip_non_ws(card->li_line);

            if (ciprefix(".macro", card->li_line)) {
                new_str = tprintf(".subckt%s", str_ptr);
            } else {
                new_str = tprintf(".ends%s", str_ptr);
            }

            tfree(card->li_line);
            card->li_line = new_str;
        }

        if (ciprefix(".subckt", card->li_line) || ciprefix("x", card->li_line)) {
            /* remove () */
            str_ptr = skip_non_ws(card->li_line);  // skip over .subckt, instance name
            str_ptr = skip_ws(str_ptr);
            if (ciprefix(".subckt", card->li_line)) {
                str_ptr = skip_non_ws(str_ptr);  // skip over subckt name
                str_ptr = skip_ws(str_ptr);
            }
            if (*str_ptr == '(') {
                *str_ptr = ' ';
                while (*str_ptr != '\0') {
                    if (*str_ptr == ')') {
                        *str_ptr = ' ';
                        break;
                    }
                    str_ptr++;
                }
                card->li_line = inp_remove_ws(card->li_line); /* remove the extra white spaces just introduced */
            }
        }

        if (ciprefix(".param", card->li_line)) {
            //bool is_func = FALSE;
            int is_func = FALSE;
            str_ptr = skip_non_ws(card->li_line);  // skip over .param
            str_ptr = skip_ws(str_ptr);
            while (!isspace(*str_ptr) && *str_ptr != '=') {
                if (*str_ptr == '(')
                    is_func = TRUE;
                str_ptr++;
            }

            if (is_func) {
                str_ptr = strchr(card->li_line, '=');
                if (str_ptr)
                    *str_ptr = ' ';
                str_ptr = card->li_line + 1;
                str_ptr[0] = 'f';
                str_ptr[1] = 'u';
                str_ptr[2] = 'n';
                str_ptr[3] = 'c';
                str_ptr[4] = ' ';
            }
        }
    }
}


static char *
get_instance_subckt(char *line)
{
    char *end_ptr, *inst_name_ptr;
    char *equal_ptr = strchr(line, '=');

    // see if instance has parameters
    if (equal_ptr) {
        end_ptr = skip_back_ws_(equal_ptr, line);
        end_ptr = skip_back_non_ws_(end_ptr, line);
    } else {
        end_ptr = strchr(line, '\0');
    }

    end_ptr = skip_back_ws_(end_ptr, line);

    inst_name_ptr = skip_back_non_ws_(end_ptr, line);

    return copy_substring(inst_name_ptr, end_ptr);
}


static char*
get_subckt_model_name(char *line)
{
    char *name, *end_ptr;

    name = skip_non_ws(line);   // eat .subckt|.model
    name = skip_ws(name);

    end_ptr = skip_non_ws(name);

    return copy_substring(name, end_ptr);
}


static char*
get_model_name(char *line, int num_terminals)
{
    char *beg_ptr, *end_ptr;
    int  i = 0;

    beg_ptr = skip_non_ws(line); /* eat device name */
    beg_ptr = skip_ws(beg_ptr);

    for (i = 0; i < num_terminals; i++) { /* skip the terminals */
        beg_ptr = skip_non_ws(beg_ptr);
        beg_ptr = skip_ws(beg_ptr);
    }

    if (*line == 'r')           /* special dealing for r models */
        if ((*beg_ptr == '+') || (*beg_ptr == '-') || isdigit(*beg_ptr)) { /* looking for a value before model */
            beg_ptr = skip_non_ws(beg_ptr); /* skip the value */
            beg_ptr = skip_ws(beg_ptr);
        }

    end_ptr = skip_non_ws(beg_ptr);

    return copy_substring(beg_ptr, end_ptr);
}


static char*
get_model_type(char *line)
{
    char *beg_ptr;

    if (!ciprefix(".model", line))
        return NULL;

    beg_ptr = skip_non_ws(line); /* eat .model */
    beg_ptr = skip_ws(beg_ptr);

    beg_ptr = skip_non_ws(beg_ptr); /* eat model name */
    beg_ptr = skip_ws(beg_ptr);

    return gettok(&beg_ptr);
}


static char *
get_adevice_model_name(char *line)
{
    char *ptr_end, *ptr_beg;

    ptr_end = skip_back_ws_(strchr(line, '\0'), line);
    ptr_beg = skip_back_non_ws_(ptr_end, line);

    return copy_substring(ptr_beg, ptr_end);
}


/*
 * FIXME, this is a mere hack of the worst kind
 *   to distinguish modelname tokens from other tokens
 *   by a simple comparison for well known discrete device name patterns
 */

static int
is_a_modelname(const char *s)
{
    /* first character of model name is character from alphabet */
    if (isalpha(s[0]))
        return TRUE;

    /* e.g. 1N4002 */
    if (isdigit(s[0]) && isalpha(s[1]) && isdigit(s[2]))
        return TRUE;

    /* e.g. 2SK456 */
    if (isdigit(s[0]) && isalpha(s[1]) && isalpha(s[2]) && isdigit(s[3]))
        return TRUE;

    /* e.g. 1SMB4148 */
    if (isdigit(s[0]) && isalpha(s[1]) && isalpha(s[2]) && isalpha(s[3]) && isdigit(s[4]))
        return TRUE;

    return FALSE;
}


struct nlist {
    char **names;
    int num_names;
    int size;
};


static const char *
nlist_find(const struct nlist *nlist, const char *name)
{
    int i;
    for (i = 0; i < nlist->num_names; i++)
        if (strcmp(nlist->names[i], name) == 0)
            return nlist->names[i];
    return NULL;
}


static const char *
nlist_model_find(const struct nlist *nlist, const char *name)
{
    int i;
    for (i = 0; i < nlist->num_names; i++)
        if (model_name_match(nlist->names[i], name))
            return nlist->names[i];
    return NULL;
}


static void
nlist_adjoin(struct nlist *nlist, char *name)
{
    if (nlist_find(nlist, name)) {
        tfree(name);
        return;
    }

    if (nlist->num_names >= nlist->size)
        nlist->names = TREALLOC(char *, nlist->names, nlist->size *= 2);

    nlist->names[nlist->num_names++] = name;
}


static struct nlist *
nlist_allocate(int size)
{
    struct nlist *t = TMALLOC(struct nlist, 1);

    t->names = TMALLOC(char *, size);
    t->size = size;

    return t;
}


static void
nlist_destroy(struct nlist *nlist)
{
    int i;
    for (i = 0; i < nlist->num_names; i++)
        tfree(nlist->names[i]);

    tfree(nlist->names);
    tfree(nlist);
}


//static void
//get_subckts_for_subckt(struct line *start_card, char *subckt_name,
//                       struct nlist *used_subckts, struct nlist *used_models,
//                       bool has_models)
static void
get_subckts_for_subckt(struct line *start_card, char *subckt_name,
                       struct nlist *used_subckts, struct nlist *used_models,
                       int has_models)
{
    struct line *card;
    int first_new_subckt = used_subckts->num_names;

    //bool found_subckt = FALSE;
    int found_subckt = FALSE;
    int  i, fence;

    for (card = start_card; card; card = card->li_next) {

        char *line = card->li_line;

        if (*line == '*')
            continue;

        if ((ciprefix(".ends", line) || ciprefix(".eom", line)) && found_subckt)
            break;

        if (ciprefix(".subckt", line) || ciprefix(".macro", line)) {
            char *curr_subckt_name = get_subckt_model_name(line);

            if (strcmp(curr_subckt_name, subckt_name) == 0)
                found_subckt = TRUE;

            tfree(curr_subckt_name);
        }

        if (found_subckt) {
            if (*line == 'x') {
                char *inst_subckt_name = get_instance_subckt(line);
                nlist_adjoin(used_subckts, inst_subckt_name);
            } else if (*line == 'a') {
                char *model_name = get_adevice_model_name(line);
                nlist_adjoin(used_models, model_name);
            } else if (has_models) {
                int num_terminals = get_number_terminals(line);
                if (num_terminals != 0) {
                    char *model_name = get_model_name(line, num_terminals);
                    if (is_a_modelname(model_name))
                        nlist_adjoin(used_models, model_name);
                    else
                        tfree(model_name);
                }
            }
        }
    }

    // now make recursive call on instances just found above
    fence = used_subckts->num_names;
    for (i = first_new_subckt; i < fence; i++)
        get_subckts_for_subckt(start_card, used_subckts->names[i],
                               used_subckts, used_models, has_models);
}


/*
  iterate through the deck and comment out unused subckts, models
  (don't want to waste time processing everything)
  also comment out .param lines with no parameters defined
*/

static void
comment_out_unused_subckt_models(struct line *start_card)
{
    struct line *card;
    struct nlist *used_subckts, *used_models;
    int  i = 0, fence;
    //bool processing_subckt = FALSE, remove_subckt = FALSE, has_models = FALSE;
    int processing_subckt = FALSE, remove_subckt = FALSE, has_models = FALSE;
    int skip_control = 0, nested_subckt = 0;

    used_subckts = nlist_allocate(100);
    used_models = nlist_allocate(100);

    for (card = start_card; card; card = card->li_next) {
        if (ciprefix(".model", card->li_line))
            has_models = TRUE;
        if (ciprefix(".cmodel", card->li_line))
            has_models = TRUE;
        if (ciprefix(".param", card->li_line) && !strchr(card->li_line, '='))
            *card->li_line = '*';
    }

    for (card = start_card; card; card = card->li_next) {

        char *line = card->li_line;

        if (*line == '*')
            continue;

        /* there is no .subckt, .model or .param inside .control ... .endc */
        if (ciprefix(".control", line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }

        if (ciprefix(".subckt", line) || ciprefix(".macro", line))
            processing_subckt = TRUE;
        if (ciprefix(".ends", line) || ciprefix(".eom", line))
            processing_subckt = FALSE;

        if (!processing_subckt) {
            if (*line == 'x') {
                char *subckt_name = get_instance_subckt(line);
                nlist_adjoin(used_subckts, subckt_name);
            } else if (*line == 'a') {
                char *model_name = get_adevice_model_name(line);
                nlist_adjoin(used_models, model_name);
            } else if (has_models) {
                /* This is a preliminary version, until we have found a reliable
                   method to detect the model name out of the input line (Many
                   options have to be taken into account.). */
                int num_terminals = get_number_terminals(line);
                if (num_terminals != 0) {
                    char *model_name = get_model_name(line, num_terminals);
                    if (is_a_modelname(model_name))
                        nlist_adjoin(used_models, model_name);
                    else
                        tfree(model_name);
                }
            } /* if (has_models)  */
        } /* if (!processing_subckt) */
    } /* for loop through all cards */

    fence = used_subckts->num_names;
    for (i = 0; i < fence; i++)
        get_subckts_for_subckt(start_card, used_subckts->names[i],
                               used_subckts, used_models, has_models);

    /* comment out any unused subckts, currently only at top level */
    for (card = start_card; card; card = card->li_next) {

        char *line = card->li_line;

        if (*line == '*')
            continue;

        if (ciprefix(".subckt", line) || ciprefix(".macro", line)) {
            char *subckt_name = get_subckt_model_name(line);
            /* check if unused, only at top level */
            if (nested_subckt++ == 0)
                remove_subckt = !nlist_find(used_subckts, subckt_name);
            tfree(subckt_name);
        }

        if (ciprefix(".ends", line) || ciprefix(".eom", line)) {
            if (remove_subckt)
                *line = '*';
            if (--nested_subckt == 0)
                remove_subckt = FALSE;
        }

        if (remove_subckt)
            *line = '*';
        else if (has_models &&
                 (ciprefix(".model", line) || ciprefix(".cmodel", line)))
        {
            char *model_type = get_model_type(line);
            char *model_name = get_subckt_model_name(line);

            /* keep R, L, C models because in addition to no. of terminals the value may be given,
               as in RE1 1 2 800 newres dtemp=5, so model name may be token no. 4 or 5,
               and, if 5, will not be detected by get_subckt_model_name()*/
            if (!cieq(model_type, "c") &&
                !cieq(model_type, "l") &&
                !cieq(model_type, "r") &&
                !nlist_model_find(used_models, model_name))
            {
                *line = '*';
            }

            tfree(model_type);
            tfree(model_name);
        }
    }

    nlist_destroy(used_subckts);
    nlist_destroy(used_models);
}


#if 0
// find closing paren
static char *
inp_search_closing_paren(char *s)
{
    int count = 0;
    // assert(*s == '(')
    while (*s) {
        if (*s == '(')
            count++;
        if (*s == ')')
            count--;
        if (count == 0)
            return s + 1;
        s++;
    }

    return NULL;
}
#endif


#if 0
/* search backwards for opening paren */
static char *
inp_search_opening_paren(char *s, char *start)
{
    int count = 0;
    // assert(*s == ')')
    while (s >= start) {
        if (*s == '(')
            count--;
        if (*s == ')')
            count++;
        if (count == 0)
            return s;
        s--;
    }

    return NULL;
}
#endif


/* search forward for closing brace */
static char *
inp_spawn_brace(char *s)
{
    int count = 0;
    // assert(*s == '{')
    while (*s) {
        if (*s == '{')
            count++;
        if (*s == '}')
            count--;
        if (count == 0)
            return s + 1;
        s++;
    }

    return NULL;
}


/*-------------------------------------------------------------------------*
  removes  " " quotes, returns lower case letters,
  replaces non-printable characterss with '_'                                                                       *
  *-------------------------------------------------------------------------*/

void
inp_casefix(char *string)
{
#ifdef HAVE_CTYPE_H
    if (string)
        while (*string) {
#ifdef HAS_ASCII
            /* ((*string) & 0177): mask off all but the first seven bits, 0177: octal */
            *string = (char) strip(*string);
#endif
            if (*string == '"') {
                *string++ = ' ';
                while (*string && *string != '"')
                    string++;
                if (*string == '\0')
                    continue; /* needed if string is "something ! */
                if (*string == '"')
                    *string = ' ';
            }
            if (!isspace(*string) && !isprint(*string))
                *string = '_';
            if (isupper(*string))
                *string = (char) tolower(*string);
            string++;
        }
#endif
}


/* Strip all end-of-line comments from a deck
   For cf == TRUE (script files, command files like spinit, .spiceinit)
   and for .control sections only '$ ' is accepted as end-of-line comment,
   to avoid conflict with $variable definition, otherwise we accept '$'. */
//static void
//inp_stripcomments_deck(struct line *c, bool cf)
static void
inp_stripcomments_deck(struct line *c, int cf)
{
    //bool found_control = FALSE;
    int found_control = FALSE;
    for (; c; c = c->li_next) {
        /* exclude lines between .control and .endc from removing white spaces */
        if (ciprefix(".control", c->li_line))
            found_control = TRUE;
        if (ciprefix(".endc", c->li_line))
            found_control = FALSE;
        inp_stripcomments_line(c->li_line, found_control|cf);
    }
}


/*
 * Support for end-of-line comments that begin with any of the following:
 *   ';'
 *   '$' (only outside of a .control section)
 *   '$ '
 *   '//' (like in c++ and as per the numparam code)
 * Any following text to the end of the line is ignored.
 * Note requirement for $ to be followed by a space, if we are inside of a
 * .control section or in a command file. This is to avoid conflict
 * with use of $ in front of a variable.
 * Comments on a continuation line (i.e. line begining with '+') are allowed
 * and are removed before lines are stitched.
 * Lines that contain only an end-of-line comment with or without leading white
 * space are also allowed.

 If there is only white space before the end-of-line comment the
 the whole line is converted to a normal comment line (i.e. one that
 begins with a '*').
 BUG: comment characters in side of string literals are not ignored
 ('$' outside of .control section is o.k. however). */

//static void
//inp_stripcomments_line(char *s, bool cs)
static void
inp_stripcomments_line(char *s, int cs)
{
    char c = ' ';               /* anything other than a comment character */
    char *d = s;
    if (*s == '\0')
        return;                 /* empty line */
    if (*s == '*')
        return;                 /* line is already a comment */
    /* look for comments */
    while ((c = *d) != '\0') {
        d++;
        if (*d == ';') {
            break;
        } else if (!cs && (c == '$')) { /* outside of .control section */
            /* The character before '&' has to be ',' or ' ' or tab.
               A valid numerical expression directly before '$' is not yet supported. */
            if ((d - 2 >= s) && ((d[-2] == ' ') || (d[-2] == ',') || (d[-2] == '\t'))) {
                d--;
                break;
            }
        } else if (cs && (c == '$') && (*d == ' ')) {/* inside of .control section or command file */
            d--;                /* move d back to first comment character */
            break;
        } else if ((c == '/') && (*d == '/')) {
            d--;                /* move d back to first comment character */
            break;
        }
    }
    /* d now points to the first comment character or the null at the string end */

    /* check for special case of comment at start of line */
    if (d == s) {
        *s = '*';               /* turn into normal comment */
        return;
    }

    if (d > s) {
        d--;
        /* d now points to character just before comment */

        /* eat white space at new end of line */
        while (d >= s) {
            if ((*d != ' ') && (*d != '\t'))
                break;
            d--;
        }
        d++;
        /* d now points to the first white space character before the
           end-of-line or end-of-line comment, or it points to the first
           end-of-line comment character, or to the begining of the line */
    }

    /* Check for special case of comment at start of line
       with or without preceeding white space */
    if (d <= s) {
        *s = '*'; /* turn the whole line into normal comment */
        return;
    }

    *d = '\0'; /* terminate line in new location */
}


static void
inp_change_quotes(char *s)
{
    //bool first_quote = FALSE;
    int first_quote = FALSE;

    for (; *s; s++)
        if (*s == '\'') {
            if (first_quote == FALSE) {
                *s = '{';
                first_quote = TRUE;
            } else {
                *s = '}';
                first_quote = FALSE;
            }
        }
}


static void
add_name(struct names *p, char *name)
{
    if (p->num_names >= N_SUBCKT_W_PARAMS) {
        fprintf(stderr, "ERROR, N_SUBCKT_W_PARMS overflow\n");
        controlled_exit(EXIT_FAILURE);
    }

    p->names[p->num_names++] = name;
}


static char **
find_name(struct names *p, char *name)
{
    int i;

    for (i = 0; i < p->num_names; i++)
        if (strcmp(p->names[i], name) == 0)
            return & p->names[i];

    return NULL;
}


static char*
inp_fix_subckt(struct names *subckt_w_params, char *s)
{
    struct line *head = NULL, *first_param_card = NULL, *last_param_card = NULL, *c = NULL;
    char *equal, *beg, *buffer, *ptr1, *ptr2, *new_str = NULL;

    equal = strchr(s, '=');
    if (equal && !strstr(s, "params:")) {
        /* get subckt name (ptr1 will point to name) */
        ptr1 = skip_non_ws(s);
        ptr1 = skip_ws(ptr1);
        for (ptr2 = ptr1; *ptr2 && !isspace(*ptr2) && !isquote(*ptr2); ptr2++)
            ;

        add_name(subckt_w_params, copy_substring(ptr1, ptr2));

        /* go to beginning of first parameter word  */
        /* s    will contain only subckt definition */
        /* beg  will point to start of param list   */
        beg = skip_back_ws_(equal, s);
        beg = skip_back_non_ws_(beg, s);
        beg[-1] = '\0';         /* fixme can be < s */

        head = xx_new_line(NULL, NULL, 0, 0);
        /* create list of parameters that need to get sorted */
        while ((ptr1 = strchr(beg, '=')) != NULL) {
            ptr2 = skip_ws(ptr1 + 1);
            ptr1 = skip_back_ws_(ptr1, beg);
            ptr1 = skip_back_non_ws_(ptr1, beg);
            /* ptr1 points to beginning of parameter */

            if (*ptr2 == '{')
                ptr2 = inp_spawn_brace(ptr2);
            else
                ptr2 = skip_non_ws(ptr2);

            if (!ptr2) {
                fprintf(stderr, "Error: Missing } in line %s\n", s);
                controlled_exit(EXIT_FAILURE);
            }

            beg = ptr2;

            c  = xx_new_line(NULL, copy_substring(ptr1, ptr2), 0, 0);

            if (last_param_card)
                last_param_card->li_next = c;
            else
                first_param_card = c;

            last_param_card = c;
        }
        /* now sort parameters in order of dependencies */
        inp_sort_params(first_param_card, head, NULL, NULL);

        /* create new ordered parameter string for subckt call */
        for (c = head->li_next; c; c = c->li_next)
            if (new_str == NULL) {
                new_str = strdup(c->li_line);
            } else {
                char *x = tprintf("%s %s", new_str, c->li_line);
                tfree(new_str);
                new_str = x;
            }

        line_free_x(head, TRUE);

        /* create buffer and insert params: */
        buffer = tprintf("%s params: %s", s, new_str);

        tfree(s);
        tfree(new_str);

        s = buffer;
    }

    return s;
}


/*
 * this function shall:
 *   reduce sequences of whitespace to one space
 *   and to drop even that if it seems to be at a `safe' place to do so
 * safe place means:
 *   before or behind a '='
 *   before or behind an operator within a {} expression
 *     whereby `operator' is classified by `is_arith_char()'
 * fixme:
 *   thats odd and very naive business
 */

static char*
inp_remove_ws(char *s)
{
    char *x = s;
    char *d = s;

    int brace_level = 0;

    /* preserve at least one whitespace at beginning of line
     * fixme,
     *   is this really necessary ?
     *   or is this an artefact of original inp_remove_ws() implementation ?
     */
    if (isspace(*s))
        *d++ = *s++;

    while (*s != '\0') {
        if (*s == '{')
            brace_level++;
        if (*s == '}')
            brace_level--;

        if (isspace(*s)) {
            s = skip_ws(s);
            if (!(*s == '\0' || *s == '=' || ((brace_level > 0) && (is_arith_char(*s) || *s == ','))))
                *d++ = ' ';
            continue;
        }

        if (*s == '=' || ((brace_level > 0) && (is_arith_char(*s) || *s == ','))) {
            *d++ = *s++;
            s = skip_ws(s);
            continue;
        }

        *d++ = *s++;
    }

    *d = '\0';

    if (d == s)
        return x;

    s = copy(x);
    tfree(x);

    return s;
}


/*
  change quotes from '' to {}
  .subckt name 1 2 3 params: l=1 w=2 --> .subckt name 1 2 3 l=1 w=2
  x1 1 2 3 params: l=1 w=2 --> x1 1 2 3 l=1 w=2
  modify .subckt lines by calling inp_fix_subckt()
  No changes to lines in .control section !
*/

static void
inp_fix_for_numparam(struct names *subckt_w_params, struct line *c)
{
    //bool found_control = FALSE;
    int found_control = FALSE;

    for (; c; c = c->li_next) {

        if (*(c->li_line) == '*' || ciprefix(".lib", c->li_line))
            continue;

        /* exclude lines between .control and .endc from getting quotes changed */
        if (ciprefix(".control", c->li_line))
            found_control = TRUE;
        if (ciprefix(".endc", c->li_line))
            found_control = FALSE;

        if (found_control)
            continue;

        inp_change_quotes(c->li_line);

        if ((inp_compat_mode == COMPATMODE_ALL) || (inp_compat_mode == COMPATMODE_PS))
            if (ciprefix(".subckt", c->li_line) || ciprefix("x", c->li_line)) {
                /* remove params: */
                char *str_ptr = strstr(c->li_line, "params:");
                if (str_ptr)
                    memcpy(str_ptr, "       ", 7);
            }

        if (ciprefix(".subckt", c->li_line))
            c->li_line = inp_fix_subckt(subckt_w_params, c->li_line);
    }
}


static void
inp_remove_excess_ws(struct line *c)
{
    //bool found_control = FALSE;
    int found_control = FALSE;

    for (; c; c = c->li_next) {

        if (*c->li_line == '*')
            continue;

        /* exclude echo lines between .control and .endc from removing white spaces */
        if (ciprefix(".control", c->li_line))
            found_control = TRUE;
        if (ciprefix(".endc", c->li_line))
            found_control = FALSE;

        if (found_control && ciprefix("echo", c->li_line))
            continue;

        c->li_line = inp_remove_ws(c->li_line); /* freed in fcn */
    }
}


static struct line *
expand_section_ref(struct line *c, char *dir_name)
{
    char *line = c->li_line;

    char *s, *t, *y;

    s = skip_non_ws(line);
    while (isspace(*s) || isquote(*s))
        s++;
    for (t = s; *t && !isspace(*t) && !isquote(*t); t++)
        ;
    y = t;
    while (isspace(*y) || isquote(*y))
        y++;

    if (*y) {
        /* library section reference: `.lib <library-file> <section-name>' */

        struct line *section_def;
        char keep_char1, keep_char2;
        char *z;
        struct library *lib;

        for (z = y; *z && !isspace(*z) && !isquote(*z); z++)
            ;
        keep_char1 = *t;
        keep_char2 = *z;
        *t = '\0';
        *z = '\0';

        lib = read_a_lib(s, dir_name);

        if (!lib) {
            fprintf(stderr, "ERROR, library file %s not found\n", s);
            controlled_exit(EXIT_FAILURE);
        }

        section_def = find_section_definition(lib->deck, y);

        if (!section_def) {
            fprintf(stderr, "ERROR, library file %s, section definition %s not found\n", s, y);
            controlled_exit(EXIT_FAILURE);
        }

        /* recursively expand the refered section itself */
        {
            struct line *t = section_def;
            for (; t; t = t->li_next) {
                if (ciprefix(".endl", t->li_line))
                    break;
                if (ciprefix(".lib", t->li_line))
                    t = expand_section_ref(t, lib->habitat);
            }
            if (!t) {
                fprintf(stderr, "ERROR, .endl not found\n");
                controlled_exit(EXIT_FAILURE);
            }
        }

        /* insert the library section definition into `c' */
        {
            struct line *cend = NULL, *newl;
            struct line *rest = c->li_next;
            struct line *t = section_def;
            for (; t; t=t->li_next) {
                newl = xx_new_line(NULL, copy(t->li_line), t->li_linenum, t->li_linenum_orig);
                if (cend)
                    cend->li_next = newl;
                else {
                    c->li_next = newl;
                    newl->li_line[0] = '*';
                    newl->li_line[1] = '<';
                }
                cend = newl;
                if(ciprefix(".endl", t->li_line))
                    break;
            }
            if (!t) {
                fprintf(stderr, "ERROR, .endl not found\n");
                controlled_exit(EXIT_FAILURE);
            }
            cend->li_line[0] = '*';
            cend->li_line[1] = '>';
            cend->li_next = rest;

            c = cend;
        }

        *line = '*';  /* comment out .lib line */
        *t = keep_char1;
        *z = keep_char2;
    }

    return c;
}


/*
 * recursively expand library section references,
 * either
 *    every library section reference (when the given section_name_ === NULL)
 * or
 *    just those references occuring in the given library section definition
 */

static void
expand_section_references(struct line *c, char *dir_name)
{
    for (; c; c = c->li_next)
        if (ciprefix(".lib", c->li_line))
            c = expand_section_ref(c, dir_name);
}


static char*
inp_get_subckt_name(char *s)
{
    char *subckt_name, *end_ptr = strchr(s, '=');

    if (end_ptr) {
        end_ptr = skip_back_ws_(end_ptr, s);
        end_ptr = skip_back_non_ws_(end_ptr, s);
    } else {
        end_ptr = strchr(s, '\0');
    }

    end_ptr = skip_back_ws_(end_ptr, s);
    subckt_name = skip_back_non_ws_(end_ptr, s);

    return copy_substring(subckt_name, end_ptr);
}


static int
inp_get_params(char *line, char *param_names[], char *param_values[])
{
    char *equal_ptr;
    char *end, *name, *value;
    int  num_params = 0;
    char keep;

    while ((equal_ptr = find_assignment(line)) != NULL) {

        /* get parameter name */
        end = skip_back_ws_(equal_ptr, line);
        name = skip_back_non_ws_(end, line);

        param_names[num_params++] = copy_substring(name, end);

        /* get parameter value */
        value = skip_ws(equal_ptr + 1);

        if (*value == '{')
            end = inp_spawn_brace(value);
        else
            end = skip_non_ws(value);

        if (!end) {
            fprintf(stderr, "Error: Missing } in %s\n", line);
            controlled_exit(EXIT_FAILURE);
        }

        keep = *end;
        *end = '\0';

        if (*value == '{' ||
            isdigit(*value) ||
            (*value == '.' && isdigit(value[1])))
        {
            value = copy(value);
        } else {
            value = tprintf("{%s}", value);
        }

        param_values[num_params-1] = value;
        *end = keep;

        line = end;
    }

    return num_params;
}


static char*
inp_fix_inst_line(char *inst_line,
                  int num_subckt_params, char *subckt_param_names[], char *subckt_param_values[],
                  int num_inst_params, char *inst_param_names[], char *inst_param_values[])
{
    char *end, *inst_name, *inst_name_end;
    char *curr_line = inst_line, *new_line = NULL;
    int i, j;

    inst_name_end = skip_non_ws(inst_line);
    inst_name = copy_substring(inst_line, inst_name_end);

    end = strchr(inst_line, '=');
    if (end) {
        end = skip_back_ws_(end, inst_line);
        end = skip_back_non_ws_(end, inst_line);
        end[-1] = '\0';         /* fixme can be < inst_line */
    }

    for (i = 0; i < num_subckt_params; i++)
        for (j = 0; j < num_inst_params; j++)
            if (strcmp(subckt_param_names[i], inst_param_names[j]) == 0) {
                tfree(subckt_param_values[i]);
                subckt_param_values[i] = strdup(inst_param_values[j]);
            }

    for (i = 0; i < num_subckt_params; i++) {
        new_line = tprintf("%s %s", curr_line, subckt_param_values[i]);

        tfree(curr_line);
        tfree(subckt_param_names[i]);
        tfree(subckt_param_values[i]);

        curr_line = new_line;
    }

    for (i = 0; i < num_inst_params; i++) {
        tfree(inst_param_names[i]);
        tfree(inst_param_values[i]);
    }

    tfree(inst_name);

    return curr_line;
}


/* If multiplier parameter 'm' is found on a X line, flag is set
   to TRUE.
   Function is called from inp_fix_inst_calls_for_numparam() */

//static bool
//found_mult_param(int num_params, char *param_names[])
static int
found_mult_param(int num_params, char *param_names[])
{
    int i;

    for (i = 0; i < num_params; i++)
        if (strcmp(param_names[i], "m") == 0)
            return TRUE;

    return FALSE;
}


/* If a subcircuit invocation (X-line) is found, which contains the
   multiplier parameter 'm', m is added to all lines inside
   the corresponding subcircuit except of some excluded in the code below
   (FIXME: It may be necessary to exclude more of them, at least
   for all devices that are not supporting the 'm' parameter).

   Function is called from inp_fix_inst_calls_for_numparam() */

static int
inp_fix_subckt_multiplier(struct names *subckt_w_params, struct line *subckt_card,
                          int num_subckt_params, char *subckt_param_names[], char *subckt_param_values[])
{
    struct line *card;
    char *new_str;

    subckt_param_names[num_subckt_params]  = strdup("m");
    subckt_param_values[num_subckt_params] = strdup("1");
    num_subckt_params ++;

    if (!strstr(subckt_card->li_line, "params:")) {
        new_str = tprintf("%s params: m=1", subckt_card->li_line);
        add_name(subckt_w_params, get_subckt_model_name(subckt_card->li_line));
    } else {
        new_str = tprintf("%s m=1", subckt_card->li_line);
    }

    tfree(subckt_card->li_line);
    subckt_card->li_line = new_str;

    for (card = subckt_card->li_next;
         card && !ciprefix(".ends", card->li_line);
         card = card->li_next) {
        /* no 'm' for B, V, E, H or comment line */
        if ((*(card->li_line) == '*') || (*(card->li_line) == 'b') || (*(card->li_line) == 'v') ||
            (*(card->li_line) == 'e') || (*(card->li_line) == 'h'))
            continue;
        /* no 'm' for model cards */
        if (ciprefix(".model", card->li_line))
            continue;
        new_str = tprintf("%s m={m}", card->li_line);

        tfree(card->li_line);
        card->li_line = new_str;
    }

    return num_subckt_params;
}


static void
inp_fix_inst_calls_for_numparam(struct names *subckt_w_params, struct line *deck)
{
    struct line *c;
    char *subckt_param_names[1000];
    char *subckt_param_values[1000];
    char *inst_param_names[1000];
    char *inst_param_values[1000];
    char name_w_space[1000];
    int  i;

    // first iterate through instances and find occurences where 'm' multiplier needs to be
    // added to the subcircuit -- subsequent instances will then need this parameter as well
    for (c = deck; c; c = c->li_next) {
        char *inst_line = c->li_line;

        if (*inst_line == '*')
            continue;

        if (ciprefix("x", inst_line)) {
            int num_inst_params = inp_get_params(inst_line, inst_param_names, inst_param_values);
            char *subckt_name   = inp_get_subckt_name(inst_line);

            if (found_mult_param(num_inst_params, inst_param_names)) {
                struct line *d, *p = NULL;

                // iterate through the deck to find the subckt (last one defined wins)
                for (d = deck; d; d = d->li_next) {
                    char *subckt_line = d->li_line;
                    if (ciprefix(".subckt", subckt_line)) {
                        subckt_line = skip_non_ws(subckt_line);
                        subckt_line = skip_ws(subckt_line);

                        sprintf(name_w_space, "%s ", subckt_name);
                        if (strncmp(subckt_line, name_w_space, strlen(name_w_space)) == 0)
                            p = d;
                    }
                }

                if (p) {
                    int num_subckt_params = inp_get_params(p->li_line, subckt_param_names, subckt_param_values);

                    if (num_subckt_params == 0 || !found_mult_param(num_subckt_params, subckt_param_names))
                        inp_fix_subckt_multiplier(subckt_w_params, p, num_subckt_params, subckt_param_names, subckt_param_values);

                    for (i = 0; i < num_subckt_params; i++) {
                        tfree(subckt_param_names[i]);
                        tfree(subckt_param_values[i]);
                    }
                }
            }

            tfree(subckt_name);
            for (i = 0; i < num_inst_params; i++) {
                tfree(inst_param_names[i]);
                tfree(inst_param_values[i]);
            }
        }
    }

    for (c = deck; c; c = c->li_next) {
        char *inst_line = c->li_line;

        if (*inst_line == '*')
            continue;

        if (ciprefix("x", inst_line)) {

            char *subckt_name = inp_get_subckt_name(inst_line);

            if (find_name(subckt_w_params, subckt_name)) {
                struct line *d;

                /* find .subckt line */

                sprintf(name_w_space, "%s ", subckt_name);

                for (d = deck; d; d = d->li_next) {
                    char *subckt_line = d->li_line;
                    if (ciprefix(".subckt", subckt_line)) {
                        subckt_line = skip_non_ws(subckt_line);
                        subckt_line = skip_ws(subckt_line);

                        if (strncmp(subckt_line, name_w_space, strlen(name_w_space)) == 0) {
                            int num_subckt_params = inp_get_params(subckt_line, subckt_param_names, subckt_param_values);
                            int num_inst_params   = inp_get_params(inst_line, inst_param_names, inst_param_values);

                            // make sure that if have inst params that one matches subckt
                            if (num_inst_params != 0) {
                                //bool found_param_match = FALSE;
                                int found_param_match = FALSE;
                                int j, k;

                                for (j = 0; j < num_inst_params; j++) {
                                    for (k = 0; k < num_subckt_params; k++)
                                        if (strcmp(subckt_param_names[k], inst_param_names[j]) == 0) {
                                            found_param_match = TRUE;
                                            break;
                                        }
                                    if (found_param_match)
                                        break;
                                }

                                if (!found_param_match) {
                                    // comment out .subckt and continue
                                    while (d != NULL && !ciprefix(".ends", d->li_line)) {
                                        *(d->li_line) = '*';
                                        d = d->li_next;
                                    }
                                    *(d->li_line) = '*';
                                    continue;
                                }
                            }

                            c->li_line = inp_fix_inst_line(inst_line, num_subckt_params, subckt_param_names, subckt_param_values, num_inst_params, inst_param_names, inst_param_values);
                            for (i = 0; i < num_subckt_params; i++) {
                                tfree(subckt_param_names[i]);
                                tfree(subckt_param_values[i]);
                            }

                            for (i = 0; i < num_inst_params; i++) {
                                tfree(inst_param_names[i]);
                                tfree(inst_param_values[i]);
                            }

                            break;
                        }
                    }
                }
            }

            tfree(subckt_name);
        }
    }
}


static struct function *
new_function(struct function_env *env, char *name)
{
    struct function *f = TMALLOC(struct function, 1);

    f -> name = name;
    f -> num_parameters = 0;

    f -> next = env->functions;
    env -> functions  = f;

    return f;
}


static struct function *
find_function(struct function_env *env, char *name)
{
    struct function *f;

    for (; env; env = env->up)
        for (f = env->functions; f; f = f->next)
            if (strcmp(f->name, name) == 0)
                return f;

    return NULL;
}


static void
free_function(struct function *fcn)
{
    int i;

    tfree(fcn->name);
    tfree(fcn->body);

    for (i = 0; i < fcn->num_parameters; i++)
        tfree(fcn->params[i]);
}


static void
new_function_parameter(struct function *fcn, char *parameter)
{
    if (fcn->num_parameters >= N_PARAMS) {
        fprintf(stderr, "ERROR, N_PARAMS overflow\n");
        controlled_exit(EXIT_FAILURE);
    }

    fcn->params[fcn->num_parameters++] = parameter;
}


//static bool
//inp_strip_braces(char *s)
static int
inp_strip_braces(char *s)
{
    int nesting = 0;
    char *d = s;

    for (; *s; s++)
        if (*s == '{') {
            nesting++;
        } else if (*s == '}') {
            if (--nesting < 0)
                return FALSE;
        } else if (!isspace(*s)) {
            *d++ = *s;
        }

    *d++ = '\0';

    return TRUE;
}


static void
inp_get_func_from_line(struct function_env *env, char *line)
{
    char *end, *orig_line = line;
    struct function *function;

    /* skip `.func' */
    line = skip_non_ws(line);
    line = skip_ws(line);

    /* get function name */
    end = line;
    while (*end && !isspace(*end) && *end != '(')
        end++;

    function = new_function(env, copy_substring(line, end));

    end = skip_ws(end);

    if (*end != '(')
        goto Lerror;

    end = skip_ws(end + 1);

    /* get function parameters */
    for (;;)  {
        char *beg = end;
        while (*end && !isspace(*end) && *end != ',' && *end != ')')
            end++;
        if (end == beg)
            break;
        new_function_parameter(function, copy_substring(beg, end));
        end = skip_ws(end);
        if (*end != ',')
            break;
        end = skip_ws(end + 1);
        if (*end == ')')
            goto Lerror;
    }

    if (*end != ')')
        goto Lerror;

    end = skip_ws(end + 1);

    // skip an unwanted and non advertised optional '='
    if (*end == '=')
        end = skip_ws(end + 1);

    function->body = copy(end);

    if (inp_strip_braces(function->body)) {
        int i;

        char *accept = TMALLOC(char, function->num_parameters + 1);
        for (i = 0; i < function->num_parameters; i++)
            accept[i] = function->params[i][0];
        accept[i] = '\0';

        function->accept = accept;
        return;
    }

    tfree(function->body);

 Lerror:
    // fixme, free()
    fprintf(stderr, "ERROR: faild to parse .func in: %s\n", orig_line);
    controlled_exit(EXIT_FAILURE);
}


/*
 * grab functions at the current .subckt nesting level
 */

static void
inp_grab_func(struct function_env *env, struct line *c)
{
    int nesting = 0;

    for (; c; c = c->li_next) {

        if (*c->li_line == '*')
            continue;

        if (ciprefix(".subckt", c->li_line))
            nesting++;
        if (ciprefix(".ends", c->li_line))
            nesting--;

        if (nesting < 0)
            break;

        if (nesting > 0)
            continue;

        if (ciprefix(".func", c->li_line)) {
            inp_get_func_from_line(env, c->li_line);
            *c->li_line = '*';
        }
    }
}


static char*
search_func_arg(char *str, struct function *fcn, int *which, char *str_begin)
{
    for (; (str = strpbrk(str, fcn->accept)) != NULL; str++) {
        char before;

        if (str > str_begin)
            before = str[-1];
        else
            before = '\0';

        if (is_arith_char(before) || isspace(before) || strchr(",=", before)) {
            int i;
            for (i = 0; i < fcn->num_parameters; i++) {
                size_t len = strlen(fcn->params[i]);
                if (strncmp(str, fcn->params[i], len) == 0) {
                    char after = str[len];
                    if (is_arith_char(after) || isspace(after) || strchr(",=", after)) {
                        *which = i;
                        return str;
                    }
                }
            }
        }
    }

    return NULL;
}


static char*
inp_do_macro_param_replace(struct function *fcn, char *params[])
{
    char *str = strdup(fcn->body);
    int  i;

    char *collect_ptr = NULL;
    char *arg_ptr = str;
    char *rest = str;

    while ((arg_ptr = search_func_arg(arg_ptr, fcn, &i, str)) != NULL) {
        char *p;
        int is_vi = 0;

        /* exclude v(nn, parameter), v(parameter, nn), v(parameter),
           and i(parameter) if here 'parameter' is also a node name */

        /* go backwards from 'parameter' and find '(' */
        for (p = arg_ptr; --p > str; )
            if (*p == '(' || *p == ')') {
                if ((*p == '(') && strchr("vi", p[-1]) &&
                    (p - 2 < str || is_arith_char(p[-2]) || isspace(p[-2]) || strchr(",=", p[-2])))
                    is_vi = 1;
                break;
            }

        /* if we have a true v( or i( */
        if (is_vi) {
            /* go forward and find closing ')' */
            for (p = arg_ptr + 1; *p; p++)
                if (*p == '(' || *p == ')')
                    break;
            /* We have a true v(...) or i(...),
               so skip it, and continue searching for new 'parameter' */
            if (*p == ')') {
                arg_ptr = p;
                continue;
            }
        }

        {
            size_t collect_ptr_len = collect_ptr ? strlen(collect_ptr) : 0;
            size_t len = strlen(rest) + strlen(params[i]) + 1;
            int prefix_len = (int) (arg_ptr - rest);
            if (str_has_arith_char(params[i])) {
                collect_ptr = TREALLOC(char, collect_ptr, collect_ptr_len + len + 2);
                sprintf(collect_ptr + collect_ptr_len, "%.*s(%s)", prefix_len, rest, params[i]);
            } else {
                collect_ptr = TREALLOC(char, collect_ptr, collect_ptr_len + len);
                sprintf(collect_ptr + collect_ptr_len, "%.*s%s", prefix_len, rest, params[i]);
            }
        }

        arg_ptr += strlen(fcn->params[i]);
        rest = arg_ptr;
    }

    if (collect_ptr) {
        char *new_str = tprintf("%s%s", collect_ptr, rest);
        tfree(collect_ptr);
        tfree(str);
        str = new_str;
    }

    return str;
}


static char*
inp_expand_macro_in_str(struct function_env *env, char *str)
{
    struct function *function;
    char *c;
    char *open_paren_ptr, *close_paren_ptr, *fcn_name, *params[1000];
    char *curr_ptr, *macro_str, *curr_str = NULL;
    int  num_parens, num_params;
    char *orig_ptr = str, *search_ptr = str, *orig_str = strdup(str);
    char keep;

    // printf("%s: enter(\"%s\")\n", __FUNCTION__, str);
    while ((open_paren_ptr = strchr(search_ptr, '(')) != NULL) {

        fcn_name = open_paren_ptr;
        while (--fcn_name >= search_ptr)
        /* function name consists of numbers, letters and special characters (VALIDCHARS) */
            if (!isalnum(*fcn_name) && !strchr(VALIDCHARS, *fcn_name))
                break;
        fcn_name++;

        search_ptr = open_paren_ptr + 1;
        if (open_paren_ptr == fcn_name)
            continue;

        *open_paren_ptr = '\0';

        function = find_function(env, fcn_name);

        *open_paren_ptr = '(';

        if (!function)
            continue;

        /* find the closing paren */
        num_parens = 1;
        for (c = open_paren_ptr + 1; *c; c++) {
            if (*c == '(')
                num_parens++;
            if (*c == ')' && --num_parens == 0)
                break;
        }

        if (num_parens) {
            fprintf(stderr, "ERROR: did not find closing parenthesis for function call in str: %s\n", orig_str);
            controlled_exit(EXIT_FAILURE);
        }

        close_paren_ptr = c;

        /*
         * if (ciprefix("v(", curr_ptr)) {
         *     // look for any commas and change to ' '
         *     char *str_ptr = curr_ptr;
         *     while (*str_ptr != '\0' && *str_ptr != ')') {
         *         if (*str_ptr == ',' || *str_ptr == '(')
         *             *str_ptr = ' '; str_ptr++; }
         *     if (*str_ptr == ')')
         *         *str_ptr = ' ';
         * }
         */

        /* get the parameters */
        curr_ptr = open_paren_ptr + 1;

        for (num_params = 0; curr_ptr < close_paren_ptr; curr_ptr++) {
            char *beg_parameter;
            int num_parens;
            if (isspace(*curr_ptr))
                continue;
            beg_parameter = curr_ptr;
            num_parens = 0;
            for (; curr_ptr < close_paren_ptr; curr_ptr++) {
                if (*curr_ptr == '(')
                    num_parens++;
                if (*curr_ptr == ')')
                    num_parens--;
                if (*curr_ptr == ',' && num_parens == 0)
                    break;
            }
            params[num_params++] =
                inp_expand_macro_in_str(env, copy_substring(beg_parameter, curr_ptr));
        }

        if (function->num_parameters != num_params) {
            fprintf(stderr, "ERROR: parameter mismatch for function call in str: %s\n", orig_str);
            controlled_exit(EXIT_FAILURE);
        }

        macro_str = inp_do_macro_param_replace(function, params);
        macro_str = inp_expand_macro_in_str(env, macro_str);
        keep  = *fcn_name;
        *fcn_name = '\0';
        {
            size_t curr_str_len = curr_str ? strlen(curr_str) : 0;
            size_t len = strlen(str) + strlen(macro_str) + 3;
            curr_str = TREALLOC(char, curr_str, curr_str_len + len);
            sprintf(curr_str + curr_str_len, "%s(%s)", str, macro_str);
        }
        *fcn_name = keep;
        tfree(macro_str);

        search_ptr = str = close_paren_ptr + 1;
    }

    if (curr_str == NULL) {
        curr_str = orig_ptr;
    } else {
        if (str != NULL) {
            size_t curr_str_len = strlen(curr_str);
            size_t len = strlen(str) + 1;
            curr_str = TREALLOC(char, curr_str, curr_str_len + len);
            sprintf(curr_str + curr_str_len, "%s", str);
        }
        tfree(orig_ptr);
    }

    tfree(orig_str);
    // printf("%s: --> \"%s\"\n", __FUNCTION__, curr_str);

    return curr_str;
}


static void
inp_expand_macros_in_func(struct function_env *env)
{
    struct function *f;

    for (f = env->functions; f ; f = f->next)
        f->body = inp_expand_macro_in_str(env, f->body);
}


static struct function_env *
new_function_env(struct function_env *up)
{
    struct function_env *env = TMALLOC(struct function_env, 1);

    env -> up = up;
    env -> functions = NULL;

    return env;
}


static struct function_env *
delete_function_env(struct function_env *env)
{
    struct function_env *up = env -> up;
    struct function *f;

    for (f = env -> functions; f; ) {
        struct function *here = f;
        f = f -> next;
        free_function(here);
        tfree(here);
    }

    tfree(env);

    return up;
}


static struct line *
inp_expand_macros_in_deck(struct function_env *env, struct line *c)
{
    env = new_function_env(env);

    inp_grab_func(env, c);

    inp_expand_macros_in_func(env);

    for (; c; c = c->li_next) {

        if (*c->li_line == '*')
            continue;

        if (ciprefix(".subckt", c->li_line)) {
            struct line *subckt = c;
            c = inp_expand_macros_in_deck(env, c->li_next);
            if (c)
                continue;

            fprintf(stderr, "Error: line %d, missing .ends\n  %s\n",
                    subckt->li_linenum_orig, subckt->li_line);
            controlled_exit(EXIT_BAD);
        }

        if (ciprefix(".ends", c->li_line))
            break;

        c->li_line = inp_expand_macro_in_str(env, c->li_line);
    }

    env = delete_function_env(env);

    return c;
}


/* Put {} around tokens for handling in numparam.
   Searches for the next '=' in the line to become active.
   Several exceptions (eg. no 'set' or 'b' lines, no .cmodel lines,
   no lines between .control and .endc, no .option lines).
   Special handling of vectors with [] and complex values with < >

   h_vogt 20 April 2008
   * For xspice and num_pram compatibility .cmodel added
   * .cmodel will be replaced by .model in inp_fix_param_values()
   * and then the entire line is skipped (will not be changed by this function).
   * Usage of numparam requires {} around the parameters in the .cmodel line.
   * May be obsolete?
   */

static void
inp_fix_param_values(struct line *c)
{
    char *beg_of_str, *end_of_str, *old_str, *equal_ptr, *new_str;
    char *vec_str, *tmp_str, *natok, *buffer, *newvec, *whereisgt;
    //bool control_section = FALSE;
    int control_section = FALSE;
    wordlist *nwl;
    int parens;

    for (; c; c = c->li_next) {
        char *line = c->li_line;

        if (*line == '*' || (ciprefix(".param", line) && strchr(line, '{')))
            continue;

        if (ciprefix(".control", line)) {
            control_section = TRUE;
            continue;
        }

        if (ciprefix(".endc", line)) {
            control_section = FALSE;
            continue;
        }

        /* no handling of params in "option" lines */
        if (control_section || ciprefix(".option", line))
            continue;

        /* no handling of params in "set" lines */
        if (ciprefix("set", line))
            continue;

        /* no handling of params in B source lines */
        if (*line == 'b')
            continue;

        /* for xspice .cmodel: replace .cmodel with .model and skip entire line) */
        if (ciprefix(".cmodel", line)) {
            *(++line) = 'm';
            *(++line) = 'o';
            *(++line) = 'd';
            *(++line) = 'e';
            *(++line) = 'l';
            *(++line) = ' ';
            continue;
        }

        /* exclude CIDER models */
        if (ciprefix(".model", line) && (strstr(line, "numos") || strstr(line, "numd") ||
                                         strstr(line, "nbjt") || strstr(line, "nbjt2") ||
                                         strstr(line, "numd2")))
        {
            continue;
        }

        /* exclude CIDER devices with ic.file parameter */
        if (strstr(line, "ic.file"))
            continue;

        while ((equal_ptr = find_assignment(line)) != NULL) {

            // special case: .MEASURE {DC|AC|TRAN} result FIND out_variable WHEN out_variable2=out_variable3
            // no braces around out_variable3. out_variable3 may be v(...) or i(...)
            if (ciprefix(".meas", line))
                if (((equal_ptr[1] == 'v') || (equal_ptr[1] == 'i')) &&
                    (equal_ptr[2] == '('))
                {
                    // find closing ')' and skip token v(...) or i(...)
                    while (*equal_ptr != ')' && equal_ptr[1] != '\0')
                        equal_ptr++;
                    line = equal_ptr + 1;
                    continue;
                }

            beg_of_str = skip_ws(equal_ptr + 1);
            /* all cases where no {} have to be put around selected token */
            if (isdigit(*beg_of_str) ||
                *beg_of_str == '{' ||
                *beg_of_str == '.' ||
                *beg_of_str == '"' ||
                ((*beg_of_str == '-' || *beg_of_str == '+') && isdigit(beg_of_str[1])) ||
                ((*beg_of_str == '-' || *beg_of_str == '+') && beg_of_str[1] == '.' && isdigit(beg_of_str[2])) ||
                ciprefix("true", beg_of_str) ||
                ciprefix("false", beg_of_str))
            {
                line = equal_ptr + 1;
            } else if (*beg_of_str == '[') {
                /* A vector following the '=' token: code to put curly brackets around all params
                   inside a pair of square brackets */
                end_of_str = beg_of_str;
                while (*end_of_str != ']')
                    end_of_str++;
                /* string xx yyy from vector [xx yyy] */
                tmp_str = vec_str = copy_substring(beg_of_str + 1, end_of_str);

                /* work on vector elements inside [] */
                nwl = NULL;
                for (;;) {
                    natok = gettok(&vec_str);
                    if (!natok)
                        break;

                    buffer = TMALLOC(char, strlen(natok) + 4);
                    if (isdigit(*natok) || *natok == '{' || *natok == '.' ||
                        *natok == '"' || (*natok == '-' && isdigit(natok[1])) ||
                        ciprefix("true", natok) || ciprefix("false", natok) ||
                        eq(natok, "<") || eq(natok, ">"))
                    {
                        (void) sprintf(buffer, "%s", natok);
                        /* A complex value found inside a vector [< x1 y1> <x2 y2>] */
                        /* < xx and yy > have been dealt with before */
                        /* <xx */
                    } else if (*natok == '<') {
                        if (isdigit(natok[1]) ||
                            (natok[1] == '-' && isdigit(natok[2])))
                        {
                            (void) sprintf(buffer, "%s", natok);
                        } else {
                            *natok = '{';
                            (void) sprintf(buffer, "<%s}", natok);
                        }
                        /* yy> */
                    } else if (strchr(natok, '>')) {
                        if (isdigit(*natok) || (*natok == '-' && isdigit(natok[1]))) {
                            (void) sprintf(buffer, "%s", natok);
                        } else {
                            whereisgt = strchr(natok, '>');
                            *whereisgt = '}';
                            (void) sprintf(buffer, "{%s>", natok);
                        }
                        /* all other tokens */
                    } else {
                        (void) sprintf(buffer, "{%s}", natok);
                    }
                    tfree(natok);
                    nwl = wl_cons(copy(buffer), nwl);
                    tfree(buffer);
                }
                tfree(tmp_str);
                nwl = wl_reverse(nwl);
                /* new vector elements */
                newvec = wl_flatten(nwl);
                wl_free(nwl);
                /* insert new vector into actual line */
                *equal_ptr  = '\0';
                new_str = tprintf("%s=[%s] %s", c->li_line, newvec, end_of_str+1);
                tfree(newvec);

                old_str    = c->li_line;
                c->li_line = new_str;
                line = new_str + strlen(old_str) + 1;
                tfree(old_str);
            } else if (*beg_of_str == '<') {
                /* A complex value following the '=' token: code to put curly brackets around all params
                   inside a pair < > */
                end_of_str = beg_of_str;
                while (*end_of_str != '>')
                    end_of_str++;
                /* string xx yyy from vector [xx yyy] */
                vec_str = copy_substring(beg_of_str + 1, end_of_str);

                /* work on tokens inside <> */
                nwl = NULL;
                for (;;) {
                    natok = gettok(&vec_str);
                    if (!natok)
                        break;

                    buffer = TMALLOC(char, strlen(natok) + 4);
                    if (isdigit(*natok) || *natok == '{' || *natok == '.' ||
                        *natok == '"' || (*natok == '-' && isdigit(natok[1])) ||
                        ciprefix("true", natok) || ciprefix("false", natok))
                    {
                        (void) sprintf(buffer, "%s", natok);
                    } else {
                        (void) sprintf(buffer, "{%s}", natok);
                    }
                    tfree(natok);
                    nwl = wl_cons(copy(buffer), nwl);
                    tfree(buffer);
                }
                nwl = wl_reverse(nwl);
                /* new elements of complex variable */
                newvec = wl_flatten(nwl);
                wl_free(nwl);
                /* insert new complex value into actual line */
                *equal_ptr  = '\0';
                new_str = tprintf("%s=<%s> %s", c->li_line, newvec, end_of_str+1);
                tfree(newvec);

                old_str    = c->li_line;
                c->li_line = new_str;
                line = new_str + strlen(old_str) + 1;
                tfree(old_str);
            } else {
                /* put {} around token to be accepted as numparam */
                end_of_str = beg_of_str;
                parens = 0;
                while (*end_of_str != '\0' &&
                       (!isspace(*end_of_str) || (parens > 0)))
                {
                    if (*end_of_str == '(')
                        parens++;
                    if (*end_of_str == ')')
                        parens--;
                    end_of_str++;
                }

                *equal_ptr  = '\0';

                if (*end_of_str == '\0') {
                    new_str = tprintf("%s={%s}", c->li_line, beg_of_str);

                } else {
                    *end_of_str = '\0';
                    new_str = tprintf("%s={%s} %s", c->li_line, beg_of_str, end_of_str+1);
                }
                old_str    = c->li_line;
                c->li_line = new_str;

                line = new_str + strlen(old_str) + 1;
                tfree(old_str);
            }
        }
    }
}


static char*
get_param_name(char *line)
{
    char *beg;
    char *equal_ptr = strchr(line, '=');

    if (!equal_ptr) {
        fprintf(stderr, "ERROR: could not find '=' on parameter line '%s'!\n", line);
        controlled_exit(EXIT_FAILURE);
        return NULL;
    }

    equal_ptr = skip_back_ws_(equal_ptr, line);

    beg = skip_back_non_ws_(equal_ptr, line);

    return copy_substring(beg, equal_ptr);
}


static char*
get_param_str(char *line)
{
    char *equal_ptr = strchr(line, '=');

    if (equal_ptr)
        return skip_ws(equal_ptr + 1);
    else
        return line;
}


struct dependency
{
    int level;
    int skip;
    char *param_name;
    char *param_str;
    char *depends_on[100];
    struct line *card;
};


static int
inp_get_param_level(int param_num, struct dependency *deps, int num_params)
{
    int i, k, l, level = 0;

    if (deps[param_num].level != -1)
        return deps[param_num].level;

    for (i = 0; deps[param_num].depends_on[i]; i++) {

        for (k = 0; k < num_params; k++)
            if (deps[param_num].depends_on[i] == deps[k].param_name)
                break;

        if (k >= num_params) {
            fprintf(stderr, "ERROR: unable to find dependency parameter for %s!\n", deps[param_num].param_name);
            controlled_exit(EXIT_FAILURE);
        }

        l = inp_get_param_level(k, deps, num_params) + 1;

        if (level < l)
            level = l;
    }

    deps[param_num].level = level;

    return level;
}


static int
get_number_terminals(char *c)
{
    int i, j, k;
    char *name[12];
    char nam_buf[128];
    //bool area_found = FALSE;
    int area_found = FALSE;

    switch (*c) {
    case 'r':
    case 'c':
    case 'l':
    case 'k':
    case 'f':
    case 'h':
    case 'b':
    case 'v':
    case 'i':
    case 'd':
        return 2;
        break;
    case 'u':
    case 'j':
    case 'w':
    case 'z':
        return 3;
        break;
    case 't':
    case 'o':
    case 'g':
    case 'e':
    case 's':
    case 'y':
        return 4;
        break;
    case 'm': /* recognition of 4, 5, 6, or 7 nodes for SOI devices needed */
        i = 0;
        /* find the first token with "off" or "=" in the line*/
        while ((i < 20) && (*c != '\0')) {
            char *inst = gettok_instance(&c);
            strncpy(nam_buf, inst, sizeof(nam_buf) - 1);
            txfree(inst);
            if (strstr(nam_buf, "off") || strchr(nam_buf, '='))
                break;
            i++;
        }
        return i-2;
        break;
    case 'p': /* recognition of up to 100 cpl nodes */
        i = j = 0;
        /* find the last token in the line*/
        while ((i < 100) && (*c != '\0')) {
            char *tmp_inst = gettok_instance(&c);
            strncpy(nam_buf, tmp_inst, 32);
            tfree(tmp_inst);
            if (strchr(nam_buf, '='))
                j++;
            i++;
        }
        if (i == 100)
            return 0;
        return i - j - 2;
        break;
    case 'q': /* recognition of 3/4 terminal bjt's needed */
        /* QXXXXXXX NC NB NE <NS> MNAME <AREA> <OFF> <IC=VBE, VCE> <TEMP=T> */
        /* 12 tokens maximum */
        i = j = 0;
        while ((i < 12) && (*c != '\0')) {
            char *comma;
            name[i] = gettok_instance(&c);
            if (strstr(name[i], "off") || strchr(name[i], '='))
                j++;
            /* If we have IC=VBE, VCE instead of IC=VBE,VCE we need to inc j */
            if ((comma = strchr(name[i], ',')) != NULL && (*(++comma) == '\0'))
                j++;
            /* If we have IC=VBE , VCE ("," is a token) we need to inc j */
            if (eq(name[i], ","))
                j++;
            i++;
        }
        i--;
        area_found = FALSE;
        for (k = i; k > i-j-1; k--) {
            //bool only_digits = TRUE;
            int only_digits = TRUE;
            char *nametmp = name[k];
            /* MNAME has to contain at least one alpha character. AREA may be assumed
               if we have a token with only digits, and where the previous token does not
               end with a ',' */
            while (*nametmp) {
                if (isalpha(*nametmp) || (*nametmp == ','))
                    only_digits = FALSE;
                nametmp++;
            }
            if (only_digits && (strchr(name[k-1], ',') == NULL))
                area_found = TRUE;
        }
        for (k = i; k >= 0; k--)
            tfree(name[k]);
        if (area_found) {
            return i-j-2;
        } else {
            return i-j-1;
        }
        break;
    default:
        return 0;
        break;
    }
}


static char *ya_search_identifier(char *str, const char *identifier, char *str_begin);


static void inp_quote_params(struct line *s_c, struct line *e_c, struct dependency *deps, int num_params);

/* sort parameters based on parameter dependencies */

static void
inp_sort_params(struct line *param_cards, struct line *card_bf_start, struct line *s_c, struct line *e_c)
{
    int  i, j, num_params, ind = 0, max_level;

    struct line *c, *tail;
    int  skipped;
    int arr_size;

    struct dependency *deps;

    if (param_cards == NULL)
        return;

    /* determine the number of lines with .param */

    arr_size = 0;
    for (c = param_cards; c; c = c->li_next)
        if (strchr(c->li_line, '='))
            arr_size ++;

    deps = TMALLOC(struct dependency, arr_size);

    num_params = 0;
    for (c = param_cards; c; c = c->li_next)
        // ignore .param lines without '='
        if (strchr(c->li_line, '=')) {
            deps[num_params].depends_on[0] = NULL;
            deps[num_params].level         = -1;
            deps[num_params].skip          = 0;
            deps[num_params].param_name    = get_param_name(c->li_line); /* strdup in fcn */
            deps[num_params].param_str     = strdup(get_param_str(c->li_line));
            deps[num_params].card          = c;
            num_params ++;
        }

    // look for duplicately defined parameters and mark earlier one to skip
    // param list is ordered as defined in netlist

    skipped = 0;
    for (i = 0; i < num_params; i++) {
        for (j = i + 1; j < num_params; j++)
            if (strcmp(deps[i].param_name, deps[j].param_name) == 0)
                break;
        if (j < num_params) {
            deps[i].skip = 1;
            skipped++;
        }
    }

    for (i = 0; i < num_params; i++)
        if (!deps[i].skip) {
            char *param = deps[i].param_name;
            for (j = 0; j < num_params; j++)
                if (j != i && search_plain_identifier(deps[j].param_str, param)) {
                    for (ind = 0; deps[j].depends_on[ind]; ind++)
                        ;
                    deps[j].depends_on[ind++] = param;
                    deps[j].depends_on[ind]   = NULL;
                }
        }

    max_level = 0;
    for (i = 0; i < num_params; i++) {
        deps[i].level = inp_get_param_level(i, deps, num_params);
        if (max_level < deps[i].level)
            max_level = deps[i].level;
    }

    c = card_bf_start;
    tail = c->li_next;

    ind = 0;
    for (i = 0; i <= max_level; i++)
        for (j = 0; j < num_params; j++)
            if (!deps[j].skip && deps[j].level == i) {
                c = c->li_next = deps[j].card;
                ind++;
            }

    c->li_next = tail;

    num_params -= skipped;
    if (ind != num_params) {
        fprintf(stderr, "ERROR: found wrong number of parameters during levelization ( %d instead of %d parameter s)!\n", ind, num_params);
        controlled_exit(EXIT_FAILURE);
    }

    inp_quote_params(s_c, e_c, deps, num_params);

    // clean up memory
    for (i = 0; i < arr_size; i++) {
        tfree(deps[i].param_name);
        tfree(deps[i].param_str);
    }

    tfree(deps);
}


static void
inp_add_params_to_subckt(struct names *subckt_w_params, struct line *subckt_card)
{
    struct line *card        = subckt_card->li_next;
    char        *subckt_line = subckt_card->li_line;
    char        *new_line, *param_ptr, *subckt_name, *end_ptr;

    for (; card; card = card->li_next) {

        char *curr_line = card->li_line;

        if (!ciprefix(".param", curr_line))
            break;

        param_ptr = strchr(curr_line, ' ');
        param_ptr = skip_ws(param_ptr);

        if (!strstr(subckt_line, "params:")) {
            new_line = tprintf("%s params: %s", subckt_line, param_ptr);

            subckt_name = skip_non_ws(subckt_line);
            subckt_name = skip_ws(subckt_name);
            end_ptr = skip_non_ws(subckt_name);
            add_name(subckt_w_params, copy_substring(subckt_name, end_ptr));
        } else {
            new_line = tprintf("%s %s", subckt_line, param_ptr);
        }

        tfree(subckt_line);
        subckt_line = new_line;

        *curr_line = '*';
    }

    subckt_card->li_line = subckt_line;
}


/*
 * process a sequence of decks
 *   starting from a         `.suckt' deck
 *   upto the corresponding  `.ends'  deck
 * return a pointer to the terminating `.ends' deck
 *
 * recursivly descend
 *   when another `.subckt' is found
 *
 * parameters are removed from the main list
 *   and collected into a local list `first_param_card'
 * then processed and reinserted into the main list
 *
 */

static struct line *
inp_reorder_params_subckt(struct names *subckt_w_params, struct line *subckt_card)
{
    struct line *first_param_card = NULL;
    struct line *last_param_card = NULL;

    struct line *prev_card = subckt_card;
    struct line *c         = subckt_card->li_next;

    /* move .param lines to beginning of deck */
    while (c != NULL) {

        char *curr_line = c->li_line;

        if (*curr_line == '*') {
            prev_card = c;
            c = c->li_next;
            continue;
        }

        if (ciprefix(".subckt", curr_line)) {
            prev_card = inp_reorder_params_subckt(subckt_w_params, c);
            c         = prev_card->li_next;
            continue;
        }

        if (ciprefix(".ends", curr_line)) {
            if (first_param_card) {
                inp_sort_params(first_param_card, subckt_card, subckt_card->li_next, c);
                inp_add_params_to_subckt(subckt_w_params, subckt_card);
            }
            return c;
        }

        if (ciprefix(".param", curr_line)) {
            if (first_param_card)
                last_param_card->li_next = c;
            else
                first_param_card = c;

            last_param_card    = c;
            prev_card->li_next = c->li_next;
            c                  = c->li_next;

            last_param_card->li_next = NULL;
            continue;
        }

        prev_card = c;
        c         = c->li_next;
    }

    /* the terminating `.ends' deck wasn't found */
    controlled_exit(EXIT_FAILURE);
    return NULL;
}


static void
inp_reorder_params(struct names *subckt_w_params, struct line *list_head)
{
    struct line *first_param_card = NULL;
    struct line *last_param_card = NULL;

    struct line *prev_card = list_head;
    struct line *c = prev_card->li_next;

    /* move .param lines to beginning of deck */
    while (c != NULL) {

        char *curr_line = c->li_line;

        if (*curr_line == '*') {
            prev_card = c;
            c = c->li_next;
            continue;
        }

        if (ciprefix(".subckt", curr_line)) {
            prev_card = inp_reorder_params_subckt(subckt_w_params, c);
            c         = prev_card->li_next;
            continue;
        }

        /* check for an unexpected extra `.ends' deck */
        if (ciprefix(".ends", curr_line)) {
            fprintf(stderr, "Error: Unexpected extra .ends in line:\n  %s.\n", curr_line);
            controlled_exit(EXIT_FAILURE);
        }

        if (ciprefix(".param", curr_line)) {
            if (first_param_card)
                last_param_card->li_next = c;
            else
                first_param_card = c;

            last_param_card    = c;
            prev_card->li_next = c->li_next;
            c                  = c->li_next;

            last_param_card->li_next = NULL;
            continue;
        }

        prev_card = c;
        c = c->li_next;
    }

    inp_sort_params(first_param_card, list_head, list_head->li_next, NULL);
}


// iterate through deck and find lines with multiply defined parameters
//
// split line up into multiple lines and place those new lines immediately
// afetr the current multi-param line in the deck

static int
inp_split_multi_param_lines(struct line *card, int line_num)
{
    for (; card; card = card->li_next) {

        char *curr_line = card->li_line;

        if (*curr_line == '*')
            continue;

        if (ciprefix(".param", curr_line)) {

            struct line *param_end, *param_beg;
            char *equal_ptr, **array;
            int i, counter = 0;

            while ((equal_ptr = find_assignment(curr_line)) != NULL) {
                counter++;
                curr_line = equal_ptr + 1;
            }

            if (counter <= 1)
                continue;

            array = TMALLOC(char *, counter);

            // need to split multi param line
            curr_line = card->li_line;
            counter   = 0;
            while ((equal_ptr = find_assignment(curr_line)) != NULL) {

                char *beg_param, *end_param;

//                bool get_expression = FALSE;
//                bool get_paren_expression = FALSE;
                int get_expression = FALSE;
                int get_paren_expression = FALSE;

                beg_param = skip_back_ws_(equal_ptr, curr_line);
                beg_param = skip_back_non_ws_(beg_param, curr_line);
                end_param = skip_ws(equal_ptr + 1);
                while (*end_param != '\0' && (!isspace(*end_param) || get_expression || get_paren_expression)) {
                    if (*end_param == '{')
                        get_expression = TRUE;
                    if (*end_param == '(')
                        get_paren_expression = TRUE;
                    if (*end_param == '}')
                        get_expression = FALSE;
                    if (*end_param == ')')
                        get_paren_expression = FALSE;
                    end_param++;
                }

                if (end_param[-1] == ',')
                    end_param--;

                array[counter++] = tprintf(".param %.*s", (int) (end_param - beg_param), beg_param);

                curr_line = end_param;
            }

            param_beg = param_end = NULL;

            for (i = 0; i < counter; i++) {
                struct line *x = xx_new_line(NULL, array[i], line_num++, 0);

                if (param_end)
                    param_end->li_next = x;
                else
                    param_beg = x;

                param_end = x;
            }

            tfree(array);

            // comment out current multi-param line
            *(card->li_line)   = '*';
            // insert new param lines immediately after current line
            param_end->li_next = card->li_next;
            card->li_next      = param_beg;
            // point 'card' pointer to last in scalar list
            card               = param_end;
        }
    }

    return line_num;
}


static int
identifier_char(int c)
{
    return (c == '_') || isalnum(c);
}


//static bool
//b_transformation_wanted(const char *p)
static int
b_transformation_wanted(const char *p)
{
    const char *start = p;

    for (p = start; (p = strpbrk(p, "vith")) != NULL; p++) {
        if (p > start && identifier_char(p[-1]))
            continue;
        if (strncmp(p, "v(", 2) == 0 || strncmp(p, "i(", 2) == 0)
            return TRUE;
        if (strncmp(p, "temper", 6) == 0 && !identifier_char(p[6]))
            return TRUE;
        if (strncmp(p, "hertz", 5) == 0 && !identifier_char(p[5]))
            return TRUE;
        if (strncmp(p, "time", 4) == 0 && !identifier_char(p[4]))
            return TRUE;
    }

    return FALSE;
}


static char *
search_identifier(char *str, const char *identifier, char *str_begin)
{
    while ((str = strstr(str, identifier)) != NULL) {
        char before;

        if (str > str_begin)
            before = str[-1];
        else
            before = '\0';

        if (is_arith_char(before) || isspace(before) || strchr("=,{", before)) {
            char after = str[strlen(identifier)];
            if (is_arith_char(after) || isspace(after) || strchr(",}", after))
                return str;
        }

        str++;
    }

    return NULL;
}


char *
ya_search_identifier(char *str, const char *identifier, char *str_begin)
{
    while ((str = strstr(str, identifier)) != NULL) {
        char before;

        if (str > str_begin)
            before = str[-1];
        else
            before = '\0';

        if (is_arith_char(before) || isspace(before) || (str <= str_begin)) {
            char after = str[strlen(identifier)];
            if ((is_arith_char(after) || isspace(after) || after == '\0'))
                break;
        }

        str++;
    }

    return str;
}


static char *
search_plain_identifier(char *str, const char *identifier)
{
    char *str_begin = str;

    while ((str = strstr(str, identifier)) != NULL) {
        char before;

        if (str > str_begin)
            before = str[-1];
        else
            before = '\0';

        if (!before || !identifier_char(before)) {
            char after = str[strlen(identifier)];
            if (!after || !identifier_char(after))
                return str;
        }

        str += strlen(identifier);
    }

    return NULL;
}


/* ps compatibility:
   ECOMP 3 0 TABLE {V(1,2)} = (-1 0V) (1, 10V)
   -->
   ECOMP 3 0 int3 int0 1
   BECOMP int3 int0 V = pwl(V(1,2), -2, 0, -1, 0 , 1, 10V, 2, 10V)

   GD16 16 1 TABLE {V(16,1)} ((-100V,-1pV)(0,0)(1m,1u)(2m,1m))
   -->
   GD16 16 1 int_16 int_1 1
   BGD16 int_16 int_1 V = pwl (v(16,1) , -100V , -1pV , 0 , 0 , 1m , 1u , 2m , 1m)
*/

/* hs compatibility:
   Exxx n1 n2 VCVS n3 n4 gain --> Exxx n1 n2 n3 n4 gain
   Gxxx n1 n2 VCCS n3 n4 tr --> Gxxx n1 n2 n3 n4 tr

   Two step approach to keep the original names for reuse,
   i.e. for current measurements like i(Exxx):
   Exxx n1 n2 VOL = {equation}
   -->
   Exxx n1 n2 int1 0 1
   BExxx int1 0 V = {equation}

   Gxxx n1 n2 CUR = {equation}
   -->
   Gxxx n1 n2 int1 0 1
   BGxxx int1 0 V = {equation}

   Do the following transformations only if {equation} contains
   simulation output like v(node), v(node1, node2), i(branch).
   Otherwise let do numparam the substitutions (R=const is handled
   in inp2r.c).

   Rxxx n1 n2 R = {equation} or Rxxx n1 n2 {equation}
   -->
   BRxxx n1 n2 I = V(n1,n2)/{equation}

   Unfortunately the capability for ac noise calculation of
   resistance may be lost.

   Cxxx n1 n2 C = {equation} or Cxxx n1 n2 {equation}
   -->
   Exxx  n-aux 0  n1 n2  1
   Cxxx  n-aux 0         1
   Bxxx  n2 n1  I = i(Exxx) * equation

   Lxxx n1 n2 L = {equation} or Lxxx n1 n2 {equation}
   -->
   Fxxx n-aux 0  Bxxx -1
   Lxxx n-aux 0      1
   Bxxx n1 n2 V = v(n-aux) * 1e-16

*/

static void
inp_compat(struct line *card)
{
    char *str_ptr, *cut_line, *title_tok, *node1, *node2;
    char *out_ptr, *exp_ptr, *beg_ptr, *end_ptr, *copy_ptr, *del_ptr;
    char *xline;
    size_t xlen, i, pai = 0, paui = 0, ii;
    char *ckt_array[100];
    struct line *new_line;

    struct line  *param_end = NULL, *param_beg = NULL;
    int skip_control = 0;

    char *equation, *tc1_ptr = NULL, *tc2_ptr = NULL;
    double tc1 = 0.0, tc2 = 0.0;

    for (; card; card = card->li_next) {

        char *curr_line = card->li_line;

        /* exclude any command inside .control ... .endc */
        if (ciprefix(".control", curr_line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", curr_line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }

        if (*curr_line == '*')
            continue;

        if (*curr_line == 'e') {
            /*    Exxx n1 n2 VCVS n3 n4 gain --> Exxx n1 n2 n3 n4 gain
                  remove vcvs */
            replace_token(curr_line, "vcvs", 4, 7);

            /* Exxx n1 n2 value={equation}
               -->
               Exxx n1 n2   vol={equation} */
            if ((str_ptr = strstr(curr_line, "value=")) != NULL) {
                str_ptr[0] = ' ';
                str_ptr[1] = ' ';
                str_ptr[2] = 'v';
                str_ptr[3] = 'o';
                str_ptr[4] = 'l';
            }
            /* Exxx n1 n2 TABLE {expression} = (x0, y0) (x1, y1) (x2, y2)
               -->
               Exxx n1 n2 int1 0 1
               BExxx int1 0 V = pwl (expression, x0-(x2-x0)/2, y0, x0, y0, x1, y1, x2, y2, x2+(x2-x0)/2, y2)
            */
            if ((str_ptr = strstr(curr_line, "table")) != NULL) {
                char *expression, *firstno, *ffirstno, *secondno, *midline, *lastno, *lastlastno;
                double fnumber, lnumber, delta;
                int nerror;
                cut_line = curr_line;
                /* title and nodes */
                title_tok = gettok(&cut_line);
                node1 =  gettok(&cut_line);
                node2 =  gettok(&cut_line);
                // Exxx  n1 n2 int1 0 1
                ckt_array[0] = tprintf("%s %s %s %s_int1 0 1",
                        title_tok, node1, node2, title_tok);
                // get the expression
                str_ptr = gettok(&cut_line); /* ignore 'table' */
                if (!cieq(str_ptr, "table")) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                tfree(str_ptr);
                str_ptr =  gettok_char(&cut_line, '{', FALSE, FALSE);
                expression = gettok_char(&cut_line, '}', TRUE, TRUE); /* expression */
                if (!expression || !str_ptr) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                tfree(str_ptr);
                /* remove '{' and '}' from expression */
                if ((str_ptr = strchr(expression, '{')) != NULL)
                    *str_ptr = ' ';
                if ((str_ptr = strchr(expression, '}')) != NULL)
                    *str_ptr = ' ';
                /* cut_line may now have a '=', if yes, it will have '{' and '}'
                   (braces around token after '=') */
                if ((str_ptr = strchr(cut_line, '=')) != NULL)
                    *str_ptr = ' ';
                if ((str_ptr = strchr(cut_line, '{')) != NULL)
                    *str_ptr = ' ';
                if ((str_ptr = strchr(cut_line, '}')) != NULL)
                    *str_ptr = ' ';
                /* get first two numbers to establish extrapolation */
                str_ptr = cut_line;
                ffirstno = gettok_node(&cut_line);
                if (!ffirstno) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                firstno = copy(ffirstno);
                fnumber = INPevaluate(&ffirstno, &nerror, TRUE);
                secondno = gettok_node(&cut_line);
                midline = cut_line;
                cut_line = strrchr(str_ptr, '(');
                if (!cut_line) {
                    fprintf(stderr, "Error: bad syntax in line %d (missing parentheses)\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                /* replace '(' with ',' and ')' with ' ' */
                for (; *str_ptr; str_ptr++)
                    if (*str_ptr == '(')
                        *str_ptr = ',';
                    else if (*str_ptr == ')')
                        *str_ptr = ' ';
                /* scan for last two numbers */
                lastno = gettok_node(&cut_line);
                lnumber = INPevaluate(&lastno, &nerror, FALSE);
                /* check for max-min and take half the difference for delta */
                delta = (lnumber-fnumber)/2.;
                lastlastno = gettok_node(&cut_line);
                if (!secondno || (*midline == '\0') || (delta <= 0.) || !lastlastno) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                ckt_array[1] = tprintf("b%s %s_int1 0 v = pwl(%s, %e, %s, %s, %s, %s, %e, %s)",
                        title_tok, title_tok, expression, fnumber-delta, secondno, firstno, secondno,
                        midline, lnumber + delta, lastlastno);

                // insert new B source line immediately after current line
                for (i = 0; i < 2; i++) {
                    struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }
                // comment out current variable e line
                *(card->li_line)   = '*';
                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                tfree(firstno);
                tfree(lastlastno);
                tfree(title_tok);
                tfree(node1);
                tfree(node2);
            }
            /* Exxx n1 n2 VOL = {equation}
               -->
               Exxx n1 n2 int1 0 1
               BExxx int1 0 V = {equation}
            */
            /* search for ' vol=' or ' vol =' */
            if (((str_ptr = strchr(curr_line, '=')) != NULL) && prefix("vol", skip_back_non_ws_(skip_back_ws_(str_ptr, curr_line), curr_line))) {
                cut_line = curr_line;
                /* title and nodes */
                title_tok = gettok(&cut_line);
                node1 =  gettok(&cut_line);
                node2 =  gettok(&cut_line);
                /* Find equation, starts with '{', till end of line */
                str_ptr = strchr(cut_line, '{');
                if (str_ptr == NULL) {
                    fprintf(stderr, "ERROR: mal formed E line: %s\n", curr_line);
                    controlled_exit(EXIT_FAILURE);
                }

                // Exxx  n1 n2 int1 0 1
                ckt_array[0] = tprintf("%s %s %s %s_int1 0 1",
                        title_tok, node1, node2, title_tok);
                // BExxx int1 0 V = {equation}
                ckt_array[1] = tprintf("b%s %s_int1 0 v = %s",
                        title_tok, title_tok, str_ptr);

                // insert new B source line immediately after current line
                for (i = 0; i < 2; i++) {
                    struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }
                // comment out current variable e line
                *(card->li_line)   = '*';
                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                tfree(title_tok);
                tfree(node1);
                tfree(node2);
            }
        } else if (*curr_line == 'g') {
            /* Gxxx n1 n2 VCCS n3 n4 tr --> Gxxx n1 n2 n3 n4 tr
               remove vccs */
            replace_token(curr_line, "vccs", 4, 7);

            /* Gxxx n1 n2 value={equation}
               -->
               Gxxx n1 n2   cur={equation} */
            if ((str_ptr = strstr(curr_line, "value=")) != NULL) {
                str_ptr[0] = ' ';
                str_ptr[1] = ' ';
                str_ptr[2] = 'c';
                str_ptr[3] = 'u';
                str_ptr[4] = 'r';
            }

            /* Gxxx n1 n2 TABLE {expression} = (x0, y0) (x1, y1) (x2, y2)
               -->
               Gxxx n1 n2 int1 0 1
               BGxxx int1 0 V = pwl (expression, x0-(x2-x0)/2, y0, x0, y0, x1, y1, x2, y2, x2+(x2-x0)/2, y2)
            */
            if ((str_ptr = strstr(curr_line, "table")) != NULL) {
                char *expression, *firstno, *ffirstno, *secondno, *midline, *lastno, *lastlastno;
                char *m_ptr, *m_token;
                double fnumber, lnumber, delta;
                int nerror;
                cut_line = curr_line;
                /* title and nodes */
                title_tok = gettok(&cut_line);
                node1 =  gettok(&cut_line);
                node2 =  gettok(&cut_line);
                // Gxxx  n1 n2 int1 0 1
                // or
                // Gxxx  n1 n2 int1 0 m='expr'
                /* find multiplier m at end of line */
                m_ptr = strstr(cut_line, "m=");
                if (m_ptr) {
                    m_token = copy(m_ptr + 2);  // get only the expression
                    *m_ptr = '\0';
                }
                else
                    m_token = copy("1");
                ckt_array[0] = tprintf("%s %s %s %s_int1 0 %s",
                        title_tok, node1, node2, title_tok, m_token);
                // get the expression
                str_ptr = gettok(&cut_line); /* ignore 'table' */
                if (!cieq(str_ptr, "table")) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                tfree(str_ptr);
                str_ptr =  gettok_char(&cut_line, '{', FALSE, FALSE);
                expression = gettok_char(&cut_line, '}', TRUE, TRUE); /* expression */
                if (!expression || !str_ptr) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                tfree(str_ptr);
                /* remove '{' and '}' from expression */
                if ((str_ptr = strchr(expression, '{')) != NULL)
                    *str_ptr = ' ';
                if ((str_ptr = strchr(expression, '}')) != NULL)
                    *str_ptr = ' ';
                /* cut_line may now have a '=', if yes, it will have '{' and '}'
                   (braces around token after '=') */
                if ((str_ptr = strchr(cut_line, '=')) != NULL)
                    *str_ptr = ' ';
                if ((str_ptr = strchr(cut_line, '{')) != NULL)
                    *str_ptr = ' ';
                if ((str_ptr = strchr(cut_line, '}')) != NULL)
                    *str_ptr = ' ';
                /* get first two numbers to establish extrapolation */
                str_ptr = cut_line;
                ffirstno = gettok_node(&cut_line);
                if (!ffirstno) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                firstno = copy(ffirstno);
                fnumber = INPevaluate(&ffirstno, &nerror, TRUE);
                secondno = gettok_node(&cut_line);
                midline = cut_line;
                cut_line = strrchr(str_ptr, '(');
                /* replace '(' with ',' and ')' with ' ' */
                for (; *str_ptr; str_ptr++)
                    if (*str_ptr == '(')
                        *str_ptr = ',';
                    else if (*str_ptr == ')')
                        *str_ptr = ' ';
                /* scan for last two numbers */
                lastno = gettok_node(&cut_line);
                lnumber = INPevaluate(&lastno, &nerror, FALSE);
                /* check for max-min and take half the difference for delta */
                delta = (lnumber-fnumber)/2.;
                lastlastno = gettok_node(&cut_line);
                if (!secondno || (*midline == '\0') || (delta <= 0.) || !lastlastno) {
                    fprintf(stderr, "Error: bad syntax in line %d\n  %s\n",
                            card->li_linenum_orig, card->li_line);
                    controlled_exit(EXIT_BAD);
                }
                /* BGxxx int1 0 V = pwl (expression, x0-(x2-x0)/2, y0, x0, y0, x1, y1, x2, y2, x2+(x2-x0)/2, y2) */
                ckt_array[1] = tprintf("b%s %s_int1 0 v = pwl(%s, %e, %s, %s, %s, %s, %e, %s)",
                        title_tok, title_tok, expression, fnumber-delta, secondno, firstno, secondno,
                        midline, lnumber + delta, lastlastno);

                // insert new B source line immediately after current line
                for (i = 0; i < 2; i++) {
                    struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }
                // comment out current variable e line
                *(card->li_line)   = '*';
                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                tfree(firstno);
                tfree(lastlastno);
                tfree(title_tok);
                tfree(node1);
                tfree(node2);
                tfree(m_token);
            }
            /*
              Gxxx n1 n2 CUR = {equation}
              -->
              Gxxx n1 n2 int1 0 1
              BGxxx int1 0 V = {equation}
            */
            /* search for ' cur=' or ' cur =' */
            if (((str_ptr = strchr(curr_line, '=')) != NULL) && prefix("cur", skip_back_non_ws_(skip_back_ws_(str_ptr, curr_line), curr_line))) {
                char *m_ptr, *m_token;
                cut_line = curr_line;
                /* title and nodes */
                title_tok = gettok(&cut_line);
                node1 =  gettok(&cut_line);
                node2 =  gettok(&cut_line);
                /* Find equation, starts with '{', till end of line */
                str_ptr = strchr(cut_line, '{');
                if (str_ptr == NULL) {
                    fprintf(stderr, "ERROR: mal formed G line: %s\n", curr_line);
                    controlled_exit(EXIT_FAILURE);
                }
                /* find multiplier m at end of line */
                m_ptr = strstr(cut_line, "m=");
                if (m_ptr) {
                    m_token = copy(m_ptr + 2); //get only the expression
                    *m_ptr = '\0';
                }
                else
                    m_token = copy("1");
                // Gxxx  n1 n2 int1 0 1
                // or
                // Gxxx  n1 n2 int1 0 m='expr'
                ckt_array[0] = tprintf("%s %s %s %s_int1 0 %s",
                        title_tok, node1, node2, title_tok, m_token);
                // BGxxx int1 0 V = {equation}
                ckt_array[1] = tprintf("b%s %s_int1 0 v = %s",
                        title_tok, title_tok, str_ptr);

                // insert new B source line immediately after current line
                for (i = 0; i < 2; i++) {
                    struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }
                // comment out current variable g line
                *(card->li_line)   = '*';
                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                tfree(title_tok);
                tfree(m_token);
                tfree(node1);
                tfree(node2);
            }
        }

        /* F element compatibility */
        else if (*curr_line == 'f') {
            char *equastr, *vnamstr;
            /* Fxxx n1 n2 CCCS vnam gain --> Fxxx n1 n2 vnam gain
               remove cccs */
            replace_token(curr_line, "cccs", 4, 6);

            /* Deal with
               Fxxx n1 n2 vnam {equation}
               if equation contains the 'temper' token */
            if (search_identifier(curr_line, "temper", curr_line)) {
                cut_line = curr_line;
                title_tok = gettok(&cut_line);
                node1 =  gettok(&cut_line);
                node2 =  gettok(&cut_line);
                vnamstr = gettok(&cut_line);
                equastr = gettok(&cut_line);
                /*
                Fxxx n1 n2 vnam {equation}
                -->
                Fxxx n1 n2 vbFxxx -1
                bFxxx int1 0 i = i(vnam)*{equation}
                vbFxxx int1 0 0
                */
                //Fxxx n1 n2 VBFxxx -1
                ckt_array[0] = tprintf("%s %s %s vb%s -1",
                        title_tok, node1, node2, title_tok);
                //BFxxx BFxxx_int1 0 I = I(vnam)*{equation}
                ckt_array[1] = tprintf("b%s %s_int1 0 i = i(%s) * (%s)",
                        title_tok, title_tok, vnamstr, equastr);
                //VBFxxx int1 0 0
                ckt_array[2] = tprintf("vb%s %s_int1 0 dc 0",
                        title_tok, title_tok);
                // insert new three lines immediately after current line
                for (i = 0; i < 3; i++) {
                    struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }
                // comment out current variable f line
                *(card->li_line)   = '*';
                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                tfree(title_tok);
                tfree(vnamstr);
                tfree(equastr);
                tfree(node1);
                tfree(node2);
            }
        }
        /* H element compatibility */
        else if (*curr_line == 'h') {
            char *equastr, *vnamstr;
            /* Hxxx n1 n2 CCVS vnam transres --> Hxxx n1 n2 vnam transres
               remove cccs */
            replace_token(curr_line, "ccvs", 4, 6);

            /* Deal with
               Hxxx n1 n2 vnam {equation}
               if equation contains the 'temper' token */
            if (search_identifier(curr_line, "temper", curr_line)) {
                cut_line = curr_line;
                title_tok = gettok(&cut_line);
                node1 =  gettok(&cut_line);
                node2 =  gettok(&cut_line);
                vnamstr = gettok(&cut_line);
                equastr = gettok(&cut_line);
                /*
                Hxxx n1 n2 vnam {equation}
                -->
                Hxxx n1 n2 vbHxxx -1
                bHxxx int1 0 i = i(vnam)*{equation}
                vbHxxx int1 0 0
                */
                //Hxxx n1 n2 VBHxxx -1
                ckt_array[0] = tprintf("%s %s %s vb%s -1",
                        title_tok, node1, node2, title_tok);
                //BHxxx BHxxx_int1 0 I = I(vnam)*{equation}
                ckt_array[1] = tprintf("b%s %s_int1 0 i = i(%s) * (%s)",
                        title_tok, title_tok, vnamstr, equastr);
                //VBHxxx int1 0 0
                ckt_array[2] = tprintf("vb%s %s_int1 0 dc 0",
                        title_tok, title_tok);
                // insert new three lines immediately after current line
                for (i = 0; i < 3; i++) {
                    struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }
                // comment out current variable h line
                *(card->li_line)   = '*';
                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                tfree(title_tok);
                tfree(vnamstr);
                tfree(equastr);
                tfree(node1);
                tfree(node2);
            }
        }

        /* Rxxx n1 n2 R = {equation} or Rxxx n1 n2 {equation}
           -->
           BRxxx pos neg I = V(pos, neg)/{equation}
        */
        else if (*curr_line == 'r') {
            cut_line = curr_line;
            /* make BRxxx pos neg I = V(pos, neg)/{equation}*/
            title_tok = gettok(&cut_line);
            node1 =  gettok(&cut_line);
            node2 =  gettok(&cut_line);
            /* check only after skipping Rname and nodes, either may contain time (e.g. Rtime)*/
            if (!b_transformation_wanted(cut_line)) {
                tfree(title_tok);
                tfree(node1);
                tfree(node2);
                continue;
            }

            /* Find equation, starts with '{', till end of line */
            str_ptr = strchr(cut_line, '{');
            if (str_ptr == NULL) {
                /* if not, equation may start with a '(' */
                str_ptr = strchr(cut_line, '(');
                if (str_ptr == NULL) {
                    fprintf(stderr, "ERROR: mal formed R line: %s\n", curr_line);
                    controlled_exit(EXIT_FAILURE);
                }
                equation = gettok_char(&str_ptr, ')', TRUE, TRUE);
            }
            else
                equation = gettok_char(&str_ptr, '}', TRUE, TRUE);
            str_ptr = strstr(cut_line, "tc1");
            if (str_ptr) {
                /* We need to have 'tc1=something */
                if (str_ptr[3] && (isspace(str_ptr[3]) || (str_ptr[3] == '='))) {
                    tc1_ptr = strchr(str_ptr, '=');
                    if (tc1_ptr)
                        tc1 = atof(tc1_ptr+1);
                }
            }
            str_ptr = strstr(cut_line, "tc2");
            if (str_ptr) {
                /* We need to have 'tc2=something */
                if (str_ptr[3] && (isspace(str_ptr[3]) || (str_ptr[3] == '='))) {
                    tc2_ptr = strchr(str_ptr, '=');
                    if (tc2_ptr)
                        tc2 = atof(tc2_ptr+1);
                }
            }
            if ((tc1_ptr == NULL) && (tc2_ptr == NULL)) {
                xline = tprintf("b%s %s %s i = v(%s, %s)/(%s)", title_tok, node1, node2,
                        node1, node2, equation);
            } else if (tc2_ptr == NULL) {
                xline = tprintf("b%s %s %s i = v(%s, %s)/(%s) tc1=%15.8e reciproctc=1", title_tok, node1, node2,
                        node1, node2, equation, tc1);
            } else {
                xline = tprintf("b%s %s %s i = v(%s, %s)/(%s) tc1=%15.8e tc2=%15.8e reciproctc=1", title_tok, node1, node2,
                        node1, node2, equation, tc1, tc2);
            }
            tc1_ptr = NULL;
            tc2_ptr = NULL;
            new_line = xx_new_line(card->li_next, xline, 0, 0);

            // comment out current old R line
            *(card->li_line)   = '*';
            // insert new B source line immediately after current line
            card->li_next     = new_line;
            // point 'card' pointer to the new line
            card              = new_line;
            tfree(title_tok);
            tfree(node1);
            tfree(node2);
            tfree(equation);
        }
        /* Cxxx n1 n2 C = {equation} or Cxxx n1 n2 {equation}
           -->
           Exxx  n-aux 0  n1 n2  1
           Cxxx  n-aux 0         1
           Bxxx  n2 n1  I = i(Exxx) * equation
        */
        else if (*curr_line == 'c') {
            cut_line = curr_line;
            title_tok = gettok(&cut_line);
            node1 =  gettok(&cut_line);
            node2 =  gettok(&cut_line);
            /* check only after skipping Cname and nodes, either may contain time (e.g. Ctime)*/
            if (!b_transformation_wanted(cut_line)) {
                tfree(title_tok);
                tfree(node1);
                tfree(node2);
                continue;
            }

            /* Find equation, starts with '{', till end of line */
            str_ptr = strchr(cut_line, '{');
            if (str_ptr == NULL) {
                /* if not, equation may start with a '(' */
                str_ptr = strchr(cut_line, '(');
                if (str_ptr == NULL) {
                    fprintf(stderr, "ERROR: mal formed C line: %s\n", curr_line);
                    controlled_exit(EXIT_FAILURE);
                }
                equation = gettok_char(&str_ptr, ')', TRUE, TRUE);
            }
            else
                equation = gettok_char(&str_ptr, '}', TRUE, TRUE);
            str_ptr = strstr(cut_line, "tc1");
            if (str_ptr) {
                /* We need to have 'tc1=something */
                if (str_ptr[3] && (isspace(str_ptr[3]) || (str_ptr[3] == '='))) {
                    tc1_ptr = strchr(str_ptr, '=');
                    if (tc1_ptr)
                        tc1 = atof(tc1_ptr+1);
                }
            }
            str_ptr = strstr(cut_line, "tc2");
            if (str_ptr) {
                /* We need to have 'tc2=something */
                if (str_ptr[3] && (isspace(str_ptr[3]) || (str_ptr[3] == '='))) {
                    tc2_ptr = strchr(str_ptr, '=');
                    if (tc2_ptr)
                        tc2 = atof(tc2_ptr+1);
                }
            }
            // Exxx  n-aux 0  n1 n2  1
            ckt_array[0] = tprintf("e%s %s_int2 0 %s %s 1",
                    title_tok, title_tok, node1, node2);
            // Cxxx  n-aux 0  1
            ckt_array[1] = tprintf("c%s %s_int2 0 1", title_tok, title_tok);
            // Bxxx  n2 n1  I = i(Exxx) * equation
            if ((tc1_ptr == NULL) && (tc2_ptr == NULL)) {
                ckt_array[2] = tprintf("b%s %s %s i = i(e%s) * (%s)",
                        title_tok, node2, node1, title_tok, equation);
            } else if (tc2_ptr == NULL) {
                ckt_array[2] = tprintf("b%s %s %s i = i(e%s) * (%s) tc1=%15.8e reciproctc=1",
                        title_tok, node2, node1, title_tok, equation, tc1);
            } else {
                ckt_array[2] = tprintf("b%s %s %s i = i(e%s) * (%s) tc1=%15.8e tc2=%15.8e reciproctc=1",
                        title_tok, node2, node1, title_tok, equation, tc1, tc2);
            }
            tc1_ptr = NULL;
            tc2_ptr = NULL;
            // insert new B source line immediately after current line
            for (i = 0; i < 3; i++) {
                struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                if (param_end)
                    param_end->li_next = x;
                else
                    param_beg = x;

                param_end = x;
            }
            // comment out current variable capacitor line
            *(card->li_line)   = '*';
            // insert new param lines immediately after current line
            param_end->li_next = card->li_next;
            card->li_next      = param_beg;
            // point 'card' pointer to last in scalar list
            card               = param_end;

            param_beg = param_end = NULL;
            tfree(title_tok);
            tfree(node1);
            tfree(node2);
            tfree(equation);
        }

        /* Lxxx n1 n2 L = {equation} or Lxxx n1 n2 {equation}
           -->
           Fxxx n-aux 0  Bxxx -1
           Lxxx n-aux 0      1
           Bxxx n1 n2 V = v(n-aux) * equation
        */
        else if (*curr_line == 'l') {
            cut_line = curr_line;
            /* title and nodes */
            title_tok = gettok(&cut_line);
            node1 =  gettok(&cut_line);
            node2 =  gettok(&cut_line);
            if (!b_transformation_wanted(cut_line)) {
                tfree(title_tok);
                tfree(node1);
                tfree(node2);
                continue;
            }

            /* Find equation, starts with '{', till end of line */
            str_ptr = strchr(cut_line, '{');
            if (str_ptr == NULL) {
                /* if not, equation may start with a '(' */
                str_ptr = strchr(cut_line, '(');
                if (str_ptr == NULL) {
                    fprintf(stderr, "ERROR: mal formed L line: %s\n", curr_line);
                    controlled_exit(EXIT_FAILURE);
                }
                equation = gettok_char(&str_ptr, ')', TRUE, TRUE);
            }
            else
                equation = gettok_char(&str_ptr, '}', TRUE, TRUE);
            str_ptr = strstr(cut_line, "tc1");
            if (str_ptr) {
                /* We need to have 'tc1=something */
                if (str_ptr[3] && (isspace(str_ptr[3]) || (str_ptr[3] == '='))) {
                    tc1_ptr = strchr(str_ptr, '=');
                    if (tc1_ptr)
                        tc1 = atof(tc1_ptr+1);
                }
            }
            str_ptr = strstr(cut_line, "tc2");
            if (str_ptr) {
                /* We need to have 'tc2=something */
                if (str_ptr[3] && (isspace(str_ptr[3]) || (str_ptr[3] == '='))) {
                    tc2_ptr = strchr(str_ptr, '=');
                    if (tc2_ptr)
                        tc2 = atof(tc2_ptr+1);
                }
            }
            // Fxxx  n-aux 0  Bxxx  1
            ckt_array[0] = tprintf("f%s %s_int2 0 b%s -1",
                    title_tok, title_tok, title_tok);
            // Lxxx  n-aux 0  1
            ckt_array[1] = tprintf("l%s %s_int2 0 1", title_tok, title_tok);
            // Bxxx  n1 n2  V = v(n-aux) * equation
            if ((tc1_ptr == NULL) && (tc2_ptr == NULL)) {
                ckt_array[2] = tprintf("b%s %s %s v = v(%s_int2) * (%s)",
                        title_tok, node1, node2, title_tok, equation);
            } else if (tc2_ptr == NULL) {
                ckt_array[2] = tprintf("b%s %s %s v = v(%s_int2) * (%s) tc1=%15.8e reciproctc=0",
                        title_tok, node2, node1, title_tok, equation, tc1);
            } else {
                ckt_array[2] = tprintf("b%s %s %s v = v(%s_int2) * (%s) tc1=%15.8e tc2=%15.8e reciproctc=0",
                        title_tok, node2, node1, title_tok, equation, tc1, tc2);
            }
            tc1_ptr = NULL;
            tc2_ptr = NULL;
            // insert new B source line immediately after current line
            for (i = 0; i < 3; i++) {
                struct line *x = xx_new_line(NULL, ckt_array[i], 0, 0);

                if (param_end)
                    param_end->li_next = x;
                else
                    param_beg = x;

                param_end = x;
            }
            // comment out current variable inductor line
            *(card->li_line)   = '*';
            // insert new param lines immediately after current line
            param_end->li_next = card->li_next;
            card->li_next      = param_beg;
            // point 'card' pointer to last in scalar list
            card               = param_end;

            param_beg = param_end = NULL;
            tfree(title_tok);
            tfree(node1);
            tfree(node2);
            tfree(equation);
        }
        /* .probe -> .save
           .print, .plot, .save, .four,
           An ouput vector may be replaced by the following:
           myoutput=par('expression')
           .meas
           A vector out_variable may be replaced by
           par('expression')
        */
        else if (*curr_line == '.') {
            // replace .probe by .save
            if ((str_ptr = strstr(curr_line, ".probe")) != NULL)
                memcpy(str_ptr, ".save ", 6);

            /* Various formats for measure statement:
             * .MEASURE {DC|AC|TRAN} result WHEN out_variable=val
             * + <TD=td> <FROM=val> <TO=val>
             * + <CROSS=# | CROSS=LAST> <RISE=#|RISE=LAST> <FALL=#|FALL=LAST>
             *
             * .MEASURE {DC|AC|TRAN} result WHEN out_variable=out_variable2
             * + <TD=td> <FROM=val> <TO=val>
             * + <CROSS=# | CROSS=LAST> <RISE=#|RISE=LAST> <FALL=#|FALL=LAST>
             *
             * .MEASURE {DC|AC|TRAN} result FIND out_variable WHEN out_variable2=val
             * + <TD=td> <FROM=val> <TO=val>
             * + <CROSS=# | CROSS=LAST> <RISE=#|RISE=LAST> <FALL=#|FALL=LAST>
             *
             * .MEASURE {DC|AC|TRAN} result FIND out_variable WHEN out_variable2=out_variable3
             * + <TD=td>
             * + <CROSS=# | CROSS=LAST> <RISE=#|RISE=LAST> <FALL=#|FALL=LAST>
             *
             * .MEASURE {DC|AC|TRAN} result FIND out_variable AT=val
             * + <FROM=val> <TO=val>
             *
             * .MEASURE {DC|AC|TRAN} result {AVG|MIN|MAX|MIN_AT|MAX_AT|PP|RMS} out_variable
             * + <TD=td> <FROM=val> <TO=val>
             *
             * .MEASURE {DC|AC|TRAN} result INTEG<RAL> out_variable
             * + <TD=td> <FROM=val> <TO=val>
             *
             * .MEASURE {DC|AC|TRAN} result DERIV<ATIVE> out_variable AT=val
             *
             * .MEASURE {DC|AC|TRAN} result DERIV<ATIVE> out_variable WHEN out_variable2=val
             * + <TD=td>
             * + <CROSS=# | CROSS=LAST> <RISE=#|RISE=LAST> <FALL=#|FALL=LAST>
             *
             * .MEASURE {DC|AC|TRAN} result DERIV<ATIVE> out_variable WHEN out_variable2=out_variable3
             * + <TD=td>
             * + <CROSS=# | CROSS=LAST> <RISE=#|RISE=LAST> <FALL=#|FALL=LAST>

             The user may set any out_variable to par(' expr ').
             We have to replace this by v(pa_xx) and generate a B source line.

             * ----------------------------------------------------------------- */
            if (ciprefix(".meas", curr_line)) {
                if (strstr(curr_line, "par(") == NULL)
                    continue;
                cut_line = curr_line;
                // search for 'par('
                while ((str_ptr = strstr(cut_line, "par(")) != NULL) {
                    if (pai > 99) {
                        fprintf(stderr, "ERROR: More than 99 function calls to par()\n");
                        fprintf(stderr, "  Limited to 99 per input file\n");
                        controlled_exit(EXIT_FAILURE);
                    }

                    // we have ' par({ ... })', the right delimeter is a ' ' or '='
                    if (ciprefix(" par({", (str_ptr-1))) {
                        // find expression
                        beg_ptr = end_ptr = str_ptr + 5;
                        while ((*end_ptr != ' ') && (*end_ptr != '=') && (*end_ptr != '\0'))
                            end_ptr++;
                        exp_ptr = copy_substring(beg_ptr, end_ptr-2);
                        cut_line = str_ptr;
                        // generate node
                        out_ptr = tprintf("pa_%02d", (int)pai);
                        // Bout_ptr  out_ptr 0  V = v(expr_ptr)
                        ckt_array[pai] = tprintf("b%s %s 0 v = %s",
                                out_ptr, out_ptr, exp_ptr);
                        ckt_array[++pai] = NULL;
                        // length of the replacement V(out_ptr)
                        del_ptr = copy_ptr = tprintf("v(%s)", out_ptr);
                        // length of the replacement part in original line
                        xlen = strlen(exp_ptr) + 7;
                        // copy the replacement without trailing '\0'
                        for (ii = 0; ii < xlen; ii++)
                            if (*copy_ptr)
                                *cut_line++ = *copy_ptr++;
                            else
                                *cut_line++ = ' ';

                        tfree(del_ptr);
                        tfree(exp_ptr);
                        tfree(out_ptr);
                    }
                    // or we have '={par({ ... })}', the right delimeter is a ' '
                    else if (ciprefix("={par({", (str_ptr-2))) {
                        // find expression
                        beg_ptr = end_ptr = str_ptr + 5;
                        while ((*end_ptr != ' ') && (*end_ptr != '\0'))
                            end_ptr++;
                        exp_ptr = copy_substring(beg_ptr, end_ptr-3);
                        // generate node
                        out_ptr = tprintf("pa_%02d", (int)pai);
                        // Bout_ptr  out_ptr 0  V = v(expr_ptr)
                        ckt_array[pai] = tprintf("b%s %s 0 v = %s",
                                out_ptr, out_ptr, exp_ptr);
                        ckt_array[++pai] = NULL;
                        // length of the replacement V(out_ptr)
                        del_ptr = copy_ptr = tprintf("v(%s)", out_ptr);
                        // length of the replacement part in original line
                        xlen = strlen(exp_ptr) + 9;
                        // skip '='
                        cut_line++;
                        // copy the replacement without trailing '\0'
                        for (ii = 0; ii < xlen; ii++)
                            if (*copy_ptr)
                                *cut_line++ = *copy_ptr++;
                            else *cut_line++ = ' ';

                        tfree(del_ptr);
                        tfree(exp_ptr);
                        tfree(out_ptr);
                    } else {
                        // nothing to replace
                        cut_line = str_ptr + 1;
                        continue;
                    }

                } // while 'par'
                // no replacement done, go to next line
                if (pai == paui)
                    continue;
                // remove white spaces
                card->li_line = inp_remove_ws(curr_line);
                // insert new B source line immediately after current line
                for (ii = paui; ii < pai; ii++) {
                    struct line *x = xx_new_line(NULL, ckt_array[ii], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }

                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                paui = pai;
            } else if ((ciprefix(".save", curr_line)) ||
                       (ciprefix(".four", curr_line)) ||
                       (ciprefix(".print", curr_line)) ||
                       (ciprefix(".plot", curr_line)))
            {
                if (strstr(curr_line, "par(") == NULL)
                    continue;
                cut_line = curr_line;
                // search for 'par('
                while ((str_ptr = strstr(cut_line, "par(")) != NULL) {
                    if (pai > 99) {
                        fprintf(stderr, "ERROR: More than 99 function calls to par()\n");
                        fprintf(stderr, "  Limited to 99 per input file\n");
                        controlled_exit(EXIT_FAILURE);
                    }

                    // we have ' par({ ... })'
                    if (ciprefix(" par({", (str_ptr-1))) {

                        // find expression
                        beg_ptr = end_ptr = str_ptr + 5;
                        while ((*end_ptr != ' ') && (*end_ptr != '\0'))
                            end_ptr++;
                        exp_ptr = copy_substring(beg_ptr, end_ptr-2);
                        cut_line = str_ptr;
                        // generate node
                        out_ptr = tprintf("pa_%02d", (int)pai);
                        // Bout_ptr  out_ptr 0  V = v(expr_ptr)
                        ckt_array[pai] = tprintf("b%s %s 0 v = %s",
                                out_ptr, out_ptr, exp_ptr);
                        ckt_array[++pai] = NULL;
                        // length of the replacement V(out_ptr)
                        del_ptr = copy_ptr = tprintf("%s", out_ptr);
                        // length of the replacement part in original line
                        xlen = strlen(exp_ptr) + 7;
                        // copy the replacement without trailing '\0'
                        for (ii = 0; ii < xlen; ii++)
                            if (*copy_ptr)
                                *cut_line++ = *copy_ptr++;
                            else
                                *cut_line++ = ' ';

                        tfree(del_ptr);
                        tfree(exp_ptr);
                        tfree(out_ptr);
                    }
                    // or we have '={par({ ... })}'
                    else if (ciprefix("={par({", (str_ptr-2))) {

                        // find myoutput
                        beg_ptr = end_ptr = str_ptr - 2;
                        while (*beg_ptr != ' ')
                            beg_ptr--;
                        out_ptr = copy_substring(beg_ptr + 1, end_ptr);
                        cut_line = beg_ptr + 1;
                        // find expression
                        beg_ptr = end_ptr = str_ptr + 5;
                        while ((*end_ptr != ' ') && (*end_ptr != '\0'))
                            end_ptr++;
                        exp_ptr = copy_substring(beg_ptr, end_ptr-3);
                        // Bout_ptr  out_ptr 0  V = v(expr_ptr)
                        ckt_array[pai] = tprintf("b%s %s 0 v = %s",
                                out_ptr, out_ptr, exp_ptr);
                        ckt_array[++pai] = NULL;
                        // length of the replacement V(out_ptr)
                        del_ptr = copy_ptr = tprintf("%s", out_ptr);
                        // length of the replacement part in original line
                        xlen = strlen(out_ptr) + strlen(exp_ptr) + 10;
                        // copy the replacement without trailing '\0'
                        for (ii = 0; ii < xlen; ii++)
                            if (*copy_ptr)
                                *cut_line++ = *copy_ptr++;
                            else *cut_line++ = ' ';

                        tfree(del_ptr);
                        tfree(exp_ptr);
                        tfree(out_ptr);
                    }
                    // nothing to replace
                    else
                        cut_line = str_ptr + 1;
                } // while 'par('
                // no replacement done, go to next line
                if (pai == paui)
                    continue;
                // remove white spaces
                card->li_line = inp_remove_ws(curr_line);
                // insert new B source line immediately after current line
                for (ii = paui; ii < pai; ii++) {
                    struct line *x = xx_new_line(NULL, ckt_array[ii], 0, 0);

                    if (param_end)
                        param_end->li_next = x;
                    else
                        param_beg = x;

                    param_end = x;
                }
                // comment out current variable capacitor line
                // *(ckt_array[0])   = '*';
                // insert new param lines immediately after current line
                param_end->li_next = card->li_next;
                card->li_next      = param_beg;
                // point 'card' pointer to last in scalar list
                card               = param_end;

                param_beg = param_end = NULL;
                paui = pai;
                // continue;
            } // if .print etc.
        } // if ('.')
    }
}


/* replace a token (length 4 char) in string by spaces, if it is found
   at the correct position and the total number of tokens is o.k. */

static void
replace_token(char *string, char *token, int wherereplace, int total)
{
    int count = 0, i;
    char *actstring = string;

    /* token to be replaced not in string */
    if (strstr(string, token) == NULL)
        return;

    /* get total number of tokens */
    while (*actstring) {
        txfree(gettok(&actstring));
        count++;
    }
    /* If total number of tokens correct */
    if (count == total) {
        actstring = string;
        for (i = 1; i < wherereplace; i++)
            txfree(gettok(&actstring));
        /* If token to be replaced at right position */
        if (ciprefix(token, actstring)) {
            actstring[0] = ' ';
            actstring[1] = ' ';
            actstring[2] = ' ';
            actstring[3] = ' ';
        }
    }
}


/* lines for B sources: no parsing in numparam code, just replacement of parameters.
   Parsing done in B source parser.
   To achive this, do the following:
   Remove all '{' and '}' --> no parsing of equations in numparam
   Place '{' and '}' directly around all potential parameters,
   but skip function names like exp (search for 'exp(' to detect fcn name),
   functions containing nodes like v(node), v(node1, node2), i(branch)
   and other keywords like TEMPER. --> Only parameter replacement in numparam
*/

static void
inp_bsource_compat(struct line *card)
{
    char *equal_ptr, *str_ptr, *new_str, *final_str;
    struct line *new_line;
    int skip_control = 0;

    for (; card; card = card->li_next) {

        char *curr_line = card->li_line;

        /* exclude any command inside .control ... .endc */
        if (ciprefix(".control", curr_line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", curr_line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }

        if (*curr_line == 'b') {
            /* remove white spaces of everything inside {}*/
            card->li_line = inp_remove_ws(card->li_line);
            curr_line = card->li_line;
            /* store starting point for later parsing, beginning of {expression} */
            equal_ptr = strchr(curr_line, '=');
            /* check for errors */
            if (equal_ptr == NULL) {
                fprintf(stderr, "ERROR: mal formed B line: %s\n", curr_line);
                controlled_exit(EXIT_FAILURE);
            }
            /* find the m={m} token and remove it */
            if ((str_ptr = strstr(curr_line, "m={m}")) != NULL)
                memcpy(str_ptr, "     ", 5);
            new_str = inp_modify_exp(equal_ptr + 1);
            final_str = tprintf("%.*s %s", (int) (equal_ptr + 1 - curr_line), curr_line, new_str);

            /* Copy old line numbers into new B source line */
            new_line = xx_new_line(card->li_next, final_str, card->li_linenum, card->li_linenum_orig);

            // comment out current line (old B source line)
            *(card->li_line)   = '*';
            // insert new B source line immediately after current line
            card->li_next     = new_line;
            // point 'card' pointer to the new line
            card              = new_line;

            tfree(new_str);
        } /* end of if 'b' */
    } /* end of for loop */
}


/* Find all expressions containing the keyword 'temper',
 * except for B lines and some other exclusions. Prepare
 * these expressions by calling inp_modify_exp() and return
 * a modified card->li_line
 */

static void
inp_temper_compat(struct line *card)
{
    int skip_control = 0;
    char *beg_str, *end_str, *beg_tstr, *end_tstr, *exp_str;

    for (; card; card = card->li_next) {

        char *new_str = NULL;
        char *curr_line = card->li_line;

        if (curr_line == NULL)
            continue;
        /* exclude any command inside .control ... .endc */
        if (ciprefix(".control", curr_line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", curr_line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }
        /* exclude some elements */
        if ((*curr_line == '*') || (*curr_line == 'v') || (*curr_line == 'b') || (*curr_line == 'i') ||
            (*curr_line == 'e') || (*curr_line == 'g') || (*curr_line == 'f') || (*curr_line == 'h'))
            continue;
        /* exclude all dot commands except .model */
        if ((*curr_line == '.') && (!prefix(".model", curr_line)))
            continue;
        /* exclude lines not containing 'temper' */
        if (strstr(curr_line, "temper") == NULL)
            continue;
        /* now start processing of the remaining lines containing 'temper' */
        /* remove white spaces of everything inside {}*/
        card->li_line = inp_remove_ws(card->li_line);
        curr_line = card->li_line;

        beg_str = beg_tstr = curr_line;
        while ((beg_tstr = search_identifier(beg_tstr, "temper", curr_line)) != NULL) {
            char *modified_exp;
            /* set the global variable */
            expr_w_temper = TRUE;
            /* find the expression: first go back to the opening '{',
               then find the closing '}' */
            while ((*beg_tstr) != '{')
                beg_tstr--;
            end_str = end_tstr = beg_tstr;
            exp_str = gettok_char(&end_tstr, '}', TRUE, TRUE);
            /* modify the expression string */
            modified_exp = inp_modify_exp(exp_str);
            tfree(exp_str);
            /* add the intermediate string between previous and next expression to the new line */
            new_str = INPstrCat(new_str, copy_substring(beg_str, end_str), " ");
            /* add the modified expression string to the new line */
            new_str = INPstrCat(new_str, modified_exp, " ");
            new_str = INPstrCat(new_str, copy(" "), " ");
            /* move on to the next intermediate string */
            beg_str = beg_tstr = end_tstr;
        }
        if (*beg_str)
            new_str = INPstrCat(new_str, copy(beg_str), " ");
        tfree(card->li_line);
        card->li_line = inp_remove_ws(new_str);
    }
}


/* lines containing expressions with keyword 'temper':
 * no parsing in numparam code, just replacement of parameters.
 * Parsing done with B source parser in function inp_parse_temper
 * in inp.c. Evaluation is the done with fcn inp_evaluate_temper
 * from inp.c, taking the actual temperature into account.
 * To achive this, do the following here:
 * Remove all '{' and '}' --> no parsing of equations in numparam
 * Place '{' and '}' directly around all potential parameters,
 * but skip function names like exp (search for 'exp(' to detect fcn name),
 * functions containing nodes like v(node), v(node1, node2), i(branch)
 * and other keywords like TEMPER. --> Only parameter replacement in numparam
 */

static char *
inp_modify_exp(char* expr)
{
    char *s;
    wordlist *wl = NULL, *wlist = NULL;

    /* scan the expression and remove all '{' and '}' */
    for (s = expr; *s; s++)
        if ((*s == '{') || (*s == '}'))
            *s = ' ';

    /* scan the expression */
    s = expr;
    while (*(s = skip_ws(s))) {

        char c = *s;

        wl_append_word(&wlist, &wl, NULL);

        if ((c == ',') || (c == '(') || (c == ')') ||
            (c == '*') || (c == '/') || (c == '^') ||
            (c == '+') || (c == '?') || (c == ':') ||
            (c == '-'))
        {
            if ((c == '*') && (s[1] == '*')) {
                wl->wl_word = tprintf("**");
                s += 2;
            } else {
                wl->wl_word = tprintf("%c", c);
                s++;
            }
        } else if ((c == '>') || (c == '<') ||
                   (c == '!') || (c == '='))
        {
            /* >=, <=, !=, ==, <>, ... */
            char *beg = s++;
            if ((*s == '=') || (*s == '<') || (*s == '>'))
                s++;
            wl->wl_word = copy_substring(beg, s);
        } else if ((c == '|') || (c == '&')) {
            char *beg = s++;
            if ((*s == '|') || (*s == '&'))
                s++;
            wl->wl_word = copy_substring(beg, s);
        } else if (isalpha(c)) {

            char buf[512];
            int i = 0;

            if (((c == 'v') || (c == 'i')) && (s[1] == '(')) {
                while (*s != ')')
                    buf[i++] = *s++;
                buf[i++] = *s++;
                buf[i] = '\0';
                wl->wl_word = copy(buf);
            } else {
                while (isalnum(*s) ||
                       (*s == '!') || (*s == '#') ||
                       (*s == '$') || (*s == '%') ||
                       (*s == '_') || (*s == '[') ||
                       (*s == ']'))
                {
                    buf[i++] = *s++;
                }
                buf[i] = '\0';
                /* no parens {} around time, hertz, temper, the constants
                   pi and e which are defined in inpptree.c, around pwl and temp. coeffs */
                if ((*s == '(') ||
                    cieq(buf, "hertz") || cieq(buf, "temper") ||
                    cieq(buf, "time") || cieq(buf, "pi") ||
                    cieq(buf, "e") || cieq(buf, "pwl"))
                {
                    wl->wl_word = copy(buf);

                } else if (cieq(buf, "tc1") || cieq(buf, "tc2") ||
                           cieq(buf, "reciproctc"))
                {
                    s = skip_ws(s);
                    /* no {} around tc1 = or tc2 = , these are temp coeffs. */
                    if (s[0] == '='  &&  s[1] != '=') {
                        buf[i++] = '=';
                        buf[i] = '\0';
                        s++;
                        wl->wl_word = copy(buf);
                    } else {
                        wl->wl_word = tprintf("({%s})", buf);
                    }

                } else {
                    /* {} around all other tokens */
                    wl->wl_word = tprintf("({%s})", buf);
                }
            }
        } else if (isdigit(c) || (c == '.')) { /* allow .5 format too */
            int error1;
            /* allow 100p, 5MEG etc. */
            double dvalue = INPevaluate(&s, &error1, 0);
            wl->wl_word = tprintf("%18.10e", dvalue);
            /* skip the `unit', FIXME INPevaluate() should do this */
            while (isalpha(*s))
                s++;
        } else { /* strange char */
            printf("Preparing expression for numparam\nWhat is this?\n%s\n", s);
            wl->wl_word = tprintf("%c", *s++);
        }
    }

    expr = wl_flatten(wlist);
    wl_free(wlist);

    return expr;
}


/*
 * destructively fetch a token from the input string
 *   token is either quoted, or a plain nonwhitespace sequence
 * function will return the place from where to continue
 */

static char *
get_quoted_token(char *string, char **token)
{
    char *s = skip_ws(string);

    if (!*s)            /* nothing found */
        return string;

    if (isquote(*s)) {

        char *t = ++s;

        while (*t && !isquote(*t))
            t++;

        if (!*t) {        /* teriminator quote not found */
            *token = NULL;
            return string;
        }

        *t++ = '\0';

        *token = s;
        return t;

    } else {

        char *t = skip_non_ws(s);

        if (t == s) {     /* nothing found */
            *token = NULL;
            return string;
        }

        if (*t)
            *t++ = '\0';

        *token = s;
        return t;
    }
}


/* Option RSERIES=rval
 * Lxxx n1 n2 Lval
 * -->
 * Lxxx n1 n2_intern__ Lval
 * RLxxx_n2_intern__ n2_intern__ n2 rval
 */

static void
inp_add_series_resistor(struct line *deck)
{
    int skip_control = 0;
    struct line *card;
    char *rval = NULL;

    for (card = deck; card; card = card->li_next) {
        char *curr_line = card->li_line;
        if (*curr_line != '*' && strstr(curr_line, "option")) {
            char *t = strstr(curr_line, "rseries");
            if (t) {
                tfree(rval);

                t += 7;
                if (*t++ == '=')
                    rval = gettok(&t);

                /* default to "1e-3" if no value given */
                if (!rval)
                    rval = copy("1e-3");
            }
        }
    }

    if (!rval)
        return;

    fprintf(stdout,
            "\nOption rseries given: \n"
            "resistor %s Ohms added in series to each inductor L\n\n", rval);

    for (card = deck; card; card = card->li_next) {
        char *cut_line = card->li_line;

        /* exclude any command inside .control ... .endc */
        if (ciprefix(".control", cut_line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", cut_line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }

        if (ciprefix("l", cut_line)) {

            char *title_tok = gettok(&cut_line);
            char *node1 = gettok(&cut_line);
            char *node2 = gettok(&cut_line);

            /* new L line and new R line */
            char *newL = tprintf("%s %s %s_intern__ %s", title_tok, node1, title_tok, cut_line);
            char *newR = tprintf("R%s_intern__ %s_intern__ %s %s", title_tok, title_tok, node2, rval);

            struct line *d;

            // comment out current L line
            // insert new new L and R lines immediately after current line
            *(card->li_line) = '*';
            d = xx_new_line(card->li_next, newR, 0, 0);
            card->li_next = xx_new_line(d, newL, 0, 0);
            card = d;

            tfree(title_tok);
            tfree(node1);
            tfree(node2);
        }
    }

    tfree(rval);
}


/*
 * rewrite
 *   .subckt node1 node2 node3 name params: l={x} w={y}
 * to
 *   .subckt node1 node2 node3 name
 *   .param l={x} w={y}
 */

static void
subckt_params_to_param(struct line *card)
{
    for (; card; card = card->li_next) {
        char *curr_line = card->li_line;
        if (ciprefix(".subckt", curr_line)) {
            char *cut_line, *new_line;
            cut_line = strstr(curr_line, "params:");
            if (!cut_line)
                continue;
            /* new_line starts with "params: " */
            new_line = copy(cut_line);
            /* replace "params:" by ".param " */
            memcpy(new_line, ".param ", 7);
            /* card->li_line ends with subcircuit name */
            cut_line[-1] = '\0';
            /* insert new_line after card->li_line */
            card->li_next = xx_new_line(card->li_next, new_line,
                                        card->li_linenum + 1, 0);
        }
    }
}


/* If XSPICE option is not selected, run this function to alert and exit
   if the 'poly' option is found in e, g, f, or h controlled sources. */

#ifndef XSPICE

static void
inp_poly_err(struct line *card)
{
    size_t skip_control = 0;

    for (; card; card = card->li_next) {

        char *curr_line = card->li_line;

        if (*curr_line == '*')
            continue;

        /* exclude any command inside .control ... .endc */
        if (ciprefix(".control", curr_line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", curr_line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }

        /* get the fourth token in a controlled source line and exit,
           if it is 'poly' */
        if ((ciprefix("e", curr_line)) || (ciprefix("g", curr_line)) ||
            (ciprefix("f", curr_line)) || (ciprefix("h", curr_line)))
        {
            txfree(gettok(&curr_line));
            txfree(gettok(&curr_line));
            txfree(gettok(&curr_line));
            if (ciprefix("poly", curr_line)) {
                fprintf(stderr,
                        "\nError: XSPICE is required to run the 'poly' option in line %d\n",
                        card->li_linenum_orig);
                fprintf(stderr, "  %s\n", card->li_line);
                fprintf(stderr, "\nSee manual chapt. 31 for installation instructions\n");
                controlled_exit(EXIT_BAD);
            }
        }
    }
}

#endif


void
tprint(struct line *t)
{
    struct line *tmp;

    /*debug: print into file*/
    FILE *fd = fopen("tprint-out.txt", "w");
    for (tmp = t; tmp; tmp = tmp->li_next)
        if (*(tmp->li_line) != '*')
            fprintf(fd, "%6d  %6d  %s\n", tmp->li_linenum_orig, tmp->li_linenum, tmp->li_line);
    fprintf(fd, "\n*********************************************************************************\n");
    fprintf(fd, "*********************************************************************************\n");
    fprintf(fd, "*********************************************************************************\n\n");
    for (tmp = t; tmp; tmp = tmp->li_next)
        fprintf(fd, "%6d  %6d  %s\n", tmp->li_linenum_orig, tmp->li_linenum, tmp->li_line);
    fprintf(fd, "\n*********************************************************************************\n");
    fprintf(fd, "*********************************************************************************\n");
    fprintf(fd, "*********************************************************************************\n\n");
    for (tmp = t; tmp; tmp = tmp->li_next)
        if (*(tmp->li_line) != '*')
            fprintf(fd, "%s\n",tmp->li_line);
    fclose(fd);
}


/* prepare .if and .elseif for numparam
   .if(expression) --> .if{expression} */

static void
inp_dot_if(struct line *card)
{
    for (; card; card = card->li_next) {

        char *curr_line = card->li_line;

        if (*curr_line == '*')
            continue;

        if (ciprefix(".if", curr_line) || ciprefix(".elseif", curr_line)) {
            char *firstbr = strchr(curr_line, '(');
            char *lastbr = strrchr(curr_line, ')');
            if ((!firstbr) || (!lastbr)) {
                fprintf(cp_err, "Error in netlist line %d\n", card->li_linenum_orig);
                fprintf(cp_err, "   Bad syntax: %s\n\n", curr_line);
                controlled_exit(EXIT_BAD);
            }
            *firstbr = '{';
            *lastbr = '}';
        }
    }
}


/* Convert .param lines containing keyword 'temper' into .func lines:
 * .param xxx1 = 'temper + 25'  --->  .func xxx1() 'temper + 25'
 * Add info about the functions (name, subcircuit depth, number of
 * subckt) to linked list new_func.
 * Then scan new_func, for each xxx1 scan all lines of deck,
 * find all xxx1 and convert them to a function:
 * xxx1   --->  xxx1()
 * If this happens to be in another .param line, convert it to .func,
 * add info to end of new_func and continue scanning.
 */

static char *inp_functionalise_identifier(char *curr_line, char *identifier);

static void
inp_fix_temper_in_param(struct line *deck)
{
    int skip_control = 0, subckt_depth = 0, j, *sub_count;
    char *funcbody, *funcname;
    struct func_temper *f, *funcs = NULL, **funcs_tail_ptr = &funcs;
    struct line *card;

    sub_count = TMALLOC(int, 16);
    for(j = 0; j < 16; j++)
        sub_count[j] = 0;

    /* first pass: determine all .param with temper inside and replace by .func
       .param xxx1 = 'temper + 25'
       will become
       .func xxx1() 'temper + 25'
    */
    card = deck;
    for (; card; card = card->li_next) {

        char *curr_line = card->li_line;

        if (*curr_line == '*')
            continue;

        /* determine nested depths of subcircuits */
        if (ciprefix(".subckt", curr_line)) {
            subckt_depth ++;
            sub_count[subckt_depth]++;
            continue;
        } else if (ciprefix(".ends", curr_line)) {
            subckt_depth --;
            continue;
        }

        /* exclude any command inside .control ... .endc */
        if (ciprefix(".control", curr_line)) {
            skip_control ++;
            continue;
        } else if (ciprefix(".endc", curr_line)) {
            skip_control --;
            continue;
        } else if (skip_control > 0) {
            continue;
        }

        if (ciprefix(".param", curr_line)) {

            char *p, *temper, *equal_ptr, *lhs_b, *lhs_e;

            temper = search_identifier(curr_line, "temper", curr_line);

            if (!temper)
                continue;

            equal_ptr = find_assignment(curr_line);

            if (!equal_ptr) {
                fprintf(stderr, "ERROR: could not find '=' on parameter line '%s'!\n", curr_line);
                controlled_exit(EXIT_FAILURE);
            }

            /* .param lines with `,' separated multiple parameters
             *    must have been split in inp_split_multi_param_lines()
             */

            if (find_assignment(equal_ptr + 1)) {
                fprintf(stderr, "ERROR: internal error on line '%s'!\n", curr_line);
                controlled_exit(EXIT_FAILURE);
            }

            lhs_b = skip_non_ws(curr_line);   // eat .param
            lhs_b = skip_ws(lhs_b);

            lhs_e = skip_back_ws_(equal_ptr, curr_line);

            /* skip if this is a function already */
            p = strpbrk(lhs_b, "(,)");
            if (p && p < lhs_e)
                continue;

            if (temper < equal_ptr) {
                fprintf(stderr,
                        "Error: you cannot assign a value to TEMPER\n"
                        "  Line no. %d, %s\n",
                        card->li_linenum, curr_line);
                controlled_exit(EXIT_BAD);
            }

            funcname = copy_substring(lhs_b, lhs_e);
            funcbody = copy(equal_ptr + 1);

            *funcs_tail_ptr =
                inp_new_func(funcname, funcbody, card, sub_count, subckt_depth);
            funcs_tail_ptr = & (*funcs_tail_ptr)->next;

            tfree(funcbody);
        }
    }

    /* second pass */
    /* for each .func entry in `funcs' start the insertion operation:
       search each line from the deck which has the suitable subcircuit nesting data.
       for tokens xxx equalling the funcname, replace xxx by xxx().
       if the replacement is done in a .param line then
         convert it to a .func line and append an entry to `funcs'.
       Continue up to the very end of `funcs'.
     */

    for (f = funcs; f; f = f->next) {

        for(j = 0; j < 16; j++)
            sub_count[j] = 0;

        card = deck;
        for (; card; card = card->li_next) {

            char *new_str = NULL; /* string we assemble here */
            char *curr_line = card->li_line;
            char *firsttok_str;

            if (*curr_line == '*')
                continue;

            /* determine nested depths of subcircuits */
            if (ciprefix(".subckt", curr_line)) {
                subckt_depth ++;
                sub_count[subckt_depth]++;
                continue;
            } else if (ciprefix(".ends", curr_line)) {
                subckt_depth --;
                continue;
            }

            /* exclude any command inside .control ... .endc */
            if (ciprefix(".control", curr_line)) {
                skip_control ++;
                continue;
            } else if (ciprefix(".endc", curr_line)) {
                skip_control --;
                continue;
            } else if (skip_control > 0) {
                continue;
            }

            /* exclude lines which do not have the same subcircuit
               nesting depth and number as found in f */
            if (subckt_depth != f->subckt_depth)
                continue;
            if (sub_count[subckt_depth] != f->subckt_count)
                continue;

            /* remove first token, ignore it here, restore it later */
            firsttok_str = gettok(&curr_line);
            if (*curr_line == '\0') {
                tfree(firsttok_str);
                continue;
            }

            new_str = inp_functionalise_identifier(curr_line, f->funcname);

            if (new_str == curr_line) {
                tfree(firsttok_str);
                continue;
            }

            /* restore first part of the line */
            new_str = INPstrCat(firsttok_str, new_str, " ");
            new_str = inp_remove_ws(new_str);

            /* if we have inserted into a .param line, convert to .func */
            if (prefix(".param", new_str)) {
                char *new_tmp_str = new_str;
                txfree(gettok(&new_tmp_str));
                funcname = gettok_char(&new_tmp_str, '=', FALSE, FALSE);
                funcbody = copy(new_tmp_str + 1);
                *funcs_tail_ptr =
                    inp_new_func(funcname, funcbody, card, sub_count, subckt_depth);
                funcs_tail_ptr = & (*funcs_tail_ptr)->next;
                tfree(new_str);
                tfree(funcbody);
            } else {
                /* Or just enter new line into deck */
                card->li_next = xx_new_line(card->li_next, new_str, 0, card->li_linenum);
                *card->li_line = '*';
            }
        }
    }

    /* final memory clearance */
    tfree(sub_count);
    inp_delete_funcs(funcs);
}


static char *
inp_functionalise_identifier(char *curr_line, char *identifier)
{
    size_t len = strlen(identifier);
    char *p, *str = curr_line;

    for (p = str; (p = search_identifier(p, identifier, str)) != NULL; )
        if (p[len] != '(') {
            int prefix_len = (int) (p + len - str);
            char *x = str;
            str = tprintf("%.*s()%s", prefix_len, str, str + prefix_len);
            if (x != curr_line)
                tfree(x);
            p = str + prefix_len + 2;
        } else {
            p ++;
        }

    return str;
}


/* enter function name, nested .subckt depths, and
 * number of .subckt at given level into struct new_func
 * and add line to deck
 */

static struct func_temper *
inp_new_func(char *funcname, char *funcbody, struct line *card,
             int *sub_count, int subckt_depth)
{
    struct func_temper *f;
    char *new_str;

    f = TMALLOC(struct func_temper, 1);
    f->funcname = funcname;
    f->next = NULL;
    f->subckt_depth = subckt_depth;
    f->subckt_count = sub_count[subckt_depth];

    /* replace line in deck */
    new_str = tprintf(".func %s() %s", funcname, funcbody);
    card->li_next = xx_new_line(card->li_next, new_str, 0, card->li_linenum);
    *card->li_line = '*';

    return f;
}


static void
inp_delete_funcs(struct func_temper *f)
{
    while (f) {
        struct func_temper *f_next = f->next;
        tfree(f->funcname);
        tfree(f);
        f = f_next;
    }
}


/* look for unquoted parameters and quote them */
/* FIXME, this function seems to be useless and/or buggy and/or naive */
static void
inp_quote_params(struct line *c, struct line *end_c, struct dependency *deps, int num_params)
{
    //bool in_control = FALSE;
    int in_control = FALSE;

    for (; c && c != end_c; c = c->li_next) {

        int i, j, num_terminals;

        char *curr_line = c->li_line;

        if (ciprefix(".control", curr_line)) {
            in_control = TRUE;
            continue;
        }

        if (ciprefix(".endc", curr_line)) {
            in_control = FALSE;
            continue;
        }

        if (in_control || curr_line[0] == '.' || curr_line[0] == '*')
            continue;

        num_terminals = get_number_terminals(curr_line);

        if (num_terminals <= 0)
            continue;

        for (i = 0; i < num_params; i++) {

            char *s = curr_line;

            for (j = 0; j < num_terminals+1; j++) {
                s = skip_non_ws(s);
                s = skip_ws(s);
            }

            while ((s = ya_search_identifier(s, deps[i].param_name, curr_line)) != NULL) {

                char *rest = s + strlen(deps[i].param_name);

                if ((isspace(s[-1]) || s[-1] == '=') &&
                    (isspace(*rest) || *rest == '\0' || *rest == ')'))
                {
                    int prefix_len;

                    if (isspace(s[-1])) {
                        s = skip_back_ws(s);
                        if (s[-1] == '{')
                            s--;
                    }

                    if (isspace(*rest)) {
                        /* possible case: "{  length }" -> {length} */
                        rest = skip_ws(rest);
                        if (*rest == '}')
                            rest++;
                        else
                            rest--;
                    }

                    prefix_len = (int)(s - curr_line);

                    curr_line = tprintf("%.*s{%s}%s", prefix_len, curr_line, deps[i].param_name, rest);
                    s = curr_line + prefix_len + strlen(deps[i].param_name) + 2;

                    tfree(c->li_line);
                    c->li_line = curr_line;
                } else {
                    s += strlen(deps[i].param_name);
                }
            }
        }
    }
}
