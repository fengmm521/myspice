/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/*
 *
 * General front end stuff.
 */
#ifndef ngspice_FTEDEFS_H
#define ngspice_FTEDEFS_H

#define DEF_WIDTH   80	/* Line printer width. */
#define DEF_HEIGHT  60  /* Line printer height. */
#define IPOINTMIN   20  /* When we start plotting incremental plots. */
#include "ngspice/fteparse.h"
#include "ngspice/fteinp.h"
#include "ngspice/fteoptdefs.h"

struct ccom;

struct save_info {
    char	*name;
    IFuid	analysis;
    int		used;
};

/* The curcuits that are currently available to the user. */

struct circ {
    char *ci_name;    /* What the circuit can be called. */
    CKTcircuit *ci_ckt;      /* The CKTcircuit structure. */
    INPtables *ci_symtab;    /* The INP symbol table. */
    INPmodel *ci_modtab;     /* The INP model table. */
    struct dbcomm *ci_dbs;  /* The database storing save, iplot, stop data */
    struct line *ci_deck;   /* The input deck. */
    struct line *ci_origdeck;/* The input deck, before subckt expansion. */
    struct line *ci_options;/* The .option cards from the deck... */
    struct line *ci_meas;   /* .measure commands to run after simulation */
    struct line *ci_param;  /* .param statements found in deck */
    struct variable *ci_vars; /* ... and the parsed versions. */
//    bool ci_inprogress; /* We are in a break now. */
//    bool ci_runonce;    /* So com_run can to a reset if necessary... */
    int ci_inprogress; /* We are in a break now. */
    int ci_runonce;    /* So com_run can to a reset if necessary... */
    
    wordlist *ci_commands;  /* Things to do when this circuit is done. */
    struct circ *ci_next;   /* The next in the list. */
    struct ccom *ci_nodes;     /* ccom structs for the nodes... */
    struct ccom *ci_devices;   /* and devices in the circuit. */
    char *ci_filename;  /* Where this circuit came from. */
    TSKtask *ci_defTask;   /* the default task for this circuit */
    TSKtask *ci_specTask;  /* the special task for command line jobs */
    TSKtask *ci_curTask;   /* the most recent task for this circuit */
    JOB *ci_defOpt;    /* the default options anal. for this circuit */
    JOB *ci_specOpt;   /* the special options anal. for command line jobs */
    JOB *ci_curOpt;    /* the most recent options anal. for the circuit */
    char *ci_last_an;   /* name of last analysis run */

    FTESTATistics *FTEstats; /* Statistics for the front end */
} ;


#define mylog10(xx) (((xx) > 0.0) ? log10(xx) : (- log10(HUGE)))

#include "ngspice/fteext.h"

#endif
