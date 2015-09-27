/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "isrcdefs.h"
#include "ngspice/suffix.h"


void
ISRCdestroy(GENmodel **inModel)
{
    ISRCmodel **model = (ISRCmodel**)inModel;
    ISRCinstance *here;
    ISRCinstance *prev = NULL;
    ISRCmodel *mod = *model;
    ISRCmodel *oldmod = NULL;

    for( ; mod ; mod = mod->ISRCnextModel) {
        if(oldmod) FREE(oldmod);
        oldmod = mod;
        prev = NULL;
        for(here = mod->ISRCinstances ; here ; here = here->ISRCnextInstance) {
            if(prev) {
                tfree(prev->ISRCcoeffs);
                FREE(prev);
            }
            prev = here;
        }
        if(prev) FREE(prev);
    }
    if(oldmod) FREE(oldmod);
    *model = NULL;
}
