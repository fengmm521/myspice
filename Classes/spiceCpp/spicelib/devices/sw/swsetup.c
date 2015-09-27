/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Gordon Jacobs
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "ngspice/smpdefs.h"
#include "ngspice/cktdefs.h"
#include "swdefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
SWsetup(SMPmatrix *matrix, GENmodel *inModel, CKTcircuit *ckt, int *states)
        /* load the switch conductance with those pointers needed later 
         * for fast matrix loading 
         */
{
    SWmodel *model = (SWmodel *)inModel;
    SWinstance *here;

    /*  loop through all the current source models */
    for( ; model != NULL; model = model->SWnextModel ) {
        /* Default Value Processing for Switch Model */
        if (!model->SWthreshGiven) {
            model->SWvThreshold = 0;
        } 
        if (!model->SWhystGiven) {
            model->SWvHysteresis = 0;
        } 
        if (!model->SWonGiven)  {
            model->SWonConduct = SW_ON_CONDUCTANCE;
            model->SWonResistance = 1.0/model->SWonConduct;
        } 
        if (!model->SWoffGiven)  {
            model->SWoffConduct = SW_OFF_CONDUCTANCE;
            model->SWoffResistance = 1.0/model->SWoffConduct;
        }

        /* loop through all the instances of the model */
        for (here = model->SWinstances; here != NULL ;
                here=here->SWnextInstance) {

            here->SWstate = *states;
            *states += SW_NUM_STATES;

            /* Default Value Processing for Switch Instance */
                    /* none */

/* macro to make elements with built in test for out of memory */
#define TSTALLOC(ptr,first,second) \
do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\
    return(E_NOMEM);\
} } while(0)

            TSTALLOC(SWposPosptr, SWposNode, SWposNode);
            TSTALLOC(SWposNegptr, SWposNode, SWnegNode);
            TSTALLOC(SWnegPosptr, SWnegNode, SWposNode);
            TSTALLOC(SWnegNegptr, SWnegNode, SWnegNode);
        }
    }
    return(OK);
}
