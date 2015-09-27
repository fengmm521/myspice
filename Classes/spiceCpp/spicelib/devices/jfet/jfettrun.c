/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "jfetdefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
JFETtrunc(GENmodel *inModel, CKTcircuit *ckt, double *timeStep)
{
    JFETmodel *model = (JFETmodel*)inModel;
    JFETinstance *here;

    for( ; model != NULL; model = model->JFETnextModel) {
        for(here=model->JFETinstances;here!=NULL;here = here->JFETnextInstance){

            CKTterr(here->JFETqgs,ckt,timeStep);
            CKTterr(here->JFETqgd,ckt,timeStep);
        }
    }
    return(OK);
}
