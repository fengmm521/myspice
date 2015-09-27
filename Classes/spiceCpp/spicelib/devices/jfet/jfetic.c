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
JFETgetic(GENmodel *inModel, CKTcircuit *ckt)
{
    JFETmodel *model = (JFETmodel*)inModel;
    JFETinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->JFETnextModel) {
        for(here = model->JFETinstances; here ; here = here->JFETnextInstance) {

            if(!here->JFETicVDSGiven) {
                here->JFETicVDS = 
                        *(ckt->CKTrhs + here->JFETdrainNode) - 
                        *(ckt->CKTrhs + here->JFETsourceNode);
            }
            if(!here->JFETicVGSGiven) {
                here->JFETicVGS = 
                        *(ckt->CKTrhs + here->JFETgateNode) - 
                        *(ckt->CKTrhs + here->JFETsourceNode);
            }
        }
    }
    return(OK);
}
