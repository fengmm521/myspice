/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 S. Hwang
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "mesdefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
MEStrunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    MESmodel *model = (MESmodel*)inModel;
    MESinstance *here;

    for( ; model != NULL; model = model->MESnextModel) {
        for(here=model->MESinstances;here!=NULL;here = here->MESnextInstance){

            CKTterr(here->MESqgs,ckt,timeStep);
            CKTterr(here->MESqgd,ckt,timeStep);
        }
    }
    return(OK);
}
