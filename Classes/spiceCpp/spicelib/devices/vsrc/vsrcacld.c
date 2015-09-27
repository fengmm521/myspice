/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "vsrcdefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
VSRCacLoad(GENmodel *inModel, CKTcircuit *ckt)
{
    VSRCmodel *model = (VSRCmodel *) inModel;
    VSRCinstance *here;

    for( ; model != NULL; model = model->VSRCnextModel ) {

        /* loop through all the instances of the model */
        for (here = model->VSRCinstances; here != NULL ;
                here=here->VSRCnextInstance) {

            double acReal, acImag;

            if (ckt->CKTmode & MODEACNOISE) {
                if ((GENinstance *) here == ckt->noise_input) {
                    acReal = 1.0;
                    acImag = 0.0;
                } else {
                    acReal = 0.0;
                    acImag = 0.0;
                }
            } else {
                acReal = here->VSRCacReal;
                acImag = here->VSRCacImag;
            }

            *(here->VSRCposIbrptr) += 1.0 ;
            *(here->VSRCnegIbrptr) -= 1.0 ;
            *(here->VSRCibrPosptr) += 1.0 ;
            *(here->VSRCibrNegptr) -= 1.0 ;
            *(ckt->CKTrhs + (here->VSRCbranch)) += acReal;
            *(ckt->CKTirhs + (here->VSRCbranch)) += acImag;
        }
    }

    return(OK);
}
