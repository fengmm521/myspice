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
MESgetic(GENmodel *inModel, CKTcircuit *ckt)
{
    MESmodel *model = (MESmodel*)inModel;
    MESinstance *here;
    /*
     * grab initial conditions out of rhs array.   User specified, so use
     * external nodes to get values
     */

    for( ; model ; model = model->MESnextModel) {
        for(here = model->MESinstances; here ; here = here->MESnextInstance) {

            if(!here->MESicVDSGiven) {
                here->MESicVDS = 
                        *(ckt->CKTrhs + here->MESdrainNode) - 
                        *(ckt->CKTrhs + here->MESsourceNode);
            }
            if(!here->MESicVGSGiven) {
                here->MESicVGS = 
                        *(ckt->CKTrhs + here->MESgateNode) - 
                        *(ckt->CKTrhs + here->MESsourceNode);
            }
        }
    }
    return(OK);
}
