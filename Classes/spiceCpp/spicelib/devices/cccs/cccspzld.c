/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "ngspice/complex.h"
#include "ngspice/sperror.h"
#include "cccsdefs.h"
#include "ngspice/suffix.h"


/*ARGSUSED*/
int
CCCSpzLoad(GENmodel *inModel, CKTcircuit *ckt, SPcomplex *s)

        /* actually load the current voltage value into the 
         * sparse matrix previously provided 
         */
{
    CCCSmodel *model = (CCCSmodel*)inModel;
    CCCSinstance *here;

    NG_IGNORE(ckt);
    NG_IGNORE(s);

    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCCSnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->CCCSinstances; here != NULL ;
                here=here->CCCSnextInstance) {

            *(here->CCCSposContBrptr) += here->CCCScoeff ;
            *(here->CCCSnegContBrptr) -= here->CCCScoeff ;
        }
    }
    return(OK);
}
