/**********
Copyright 2003 Paolo Nenzi
Author: 2003 Paolo Nenzi
**********/
/*
 */


#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "inddefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


/*ARGSUSED*/
int
MUTtemp(GENmodel *inModel, CKTcircuit *ckt)
{
    MUTmodel *model = (MUTmodel*)inModel;
    MUTinstance *here;
    double ind1, ind2;

    NG_IGNORE(ckt);

    /*  loop through all the inductor models */
    for( ; model != NULL; model = model->MUTnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->MUTinstances; here != NULL ;
                here=here->MUTnextInstance) {

            /* Value Processing for mutual inductors */
	   
	    ind1 = here->MUTind1->INDinduct;
	    ind2 = here->MUTind2->INDinduct;
	    
	    /*           _______
	 * M = k * \/l1 * l2 
	 */
            here->MUTfactor = here->MUTcoupling * sqrt(ind1 * ind2); 
		     
	}
    }
    return(OK);
}
