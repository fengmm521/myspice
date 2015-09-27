/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "isrcdefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
ISRCmDelete(GENmodel **inModel, IFuid modname, GENmodel *fast)
{
    ISRCmodel **model = (ISRCmodel **) inModel;
    ISRCmodel *modfast = (ISRCmodel *) fast;
    ISRCinstance *here;
    ISRCinstance *prev = NULL;
    ISRCmodel **oldmod;
    oldmod = model;
    for( ; *model ; model = &((*model)->ISRCnextModel)) {
        if( (*model)->ISRCmodName == modname ||
                (modfast && *model == modfast) ) goto delgot;
        oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->ISRCnextModel; /* cut deleted device out of list */
    for(here = (*model)->ISRCinstances ; here ; here = here->ISRCnextInstance) {
        if(prev) FREE(prev);
        prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);

}
