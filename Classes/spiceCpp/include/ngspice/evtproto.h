#ifndef ngspice_EVTPROTO_H
#define ngspice_EVTPROTO_H

/* ===========================================================================
FILE    EVTproto.h

MEMBER OF process XSPICE

Copyright 1991
Georgia Tech Research Corporation
Atlanta, Georgia 30332
All Rights Reserved

PROJECT A-8503

AUTHORS

    9/12/91  Bill Kuhn

MODIFICATIONS

    <date> <person name> <nature of modifications>

SUMMARY

    This file contains ANSI C function prototypes for functions
    in the event-driven simulation algorithm package.

INTERFACES

    None.

REFERENCED FILES

    None.

NON-STANDARD FEATURES

    None.

=========================================================================== */

#include "ngspice/cktdefs.h"
#include "ngspice/cpstd.h"
#include "ngspice/mifdefs.h"
#include "ngspice/ipc.h"


/* ******************* */
/* Function Prototypes */
/* ******************* */


int EVTinit(CKTcircuit *ckt);
/*int EVTinit2(CKTcircuit *ckt);*/

void EVTtermInsert(
    CKTcircuit      *ckt,
    MIFinstance     *fast,
    char            *node_name,
    char            *type_name,
    int             conn_num,
    int             port_num,
    char            **err_msg);

int EVTsetup(CKTcircuit *ckt);

int EVTiter(CKTcircuit *ckt);

void EVTbackup(CKTcircuit *ckt, double new_time);

double EVTnext_time(CKTcircuit *ckt);

void EVTqueue_output(
    CKTcircuit *ckt,
    int        output_index,
    int        udn_index,
    Evt_Output_Event_t  *new_event,
    double     posted_time,
    double     event_time);


void EVTqueue_inst(
    CKTcircuit *ckt,
    int        inst_index,
    double     posted_time,
    double     event_time);

void EVTdequeue(CKTcircuit *ckt, double time);

int EVTload(CKTcircuit *ckt, int inst_index);

void EVTprint(wordlist *wl);

int EVTop(
    CKTcircuit *ckt,
    long       firstmode,
    long       continuemode,
    int        max_iter,
    Mif_Boolean_t  first_call);

void EVTop_save(
    CKTcircuit    *ckt,
    Mif_Boolean_t op,
    double        step);

void EVTnode_copy(
    CKTcircuit    *ckt,
    int           node_index,
    Evt_Node_t    *from,
    Evt_Node_t    **to);

void EVTcall_hybrids(CKTcircuit *ckt);

void EVTdump(
    CKTcircuit    *ckt,
    Ipc_Anal_t    mode,
    double        step);

void EVTaccept(
    CKTcircuit *ckt,    /* main circuit struct */
    double     time);    /* time at which analog soln was accepted */

struct dvec *EVTfindvec(char *node);

#endif
