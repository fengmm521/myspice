/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1987 Kanwar Jit Singh
**********/

#include "ngspice/ngspice.h"
#include "asrcdefs.h"
#include "ngspice/suffix.h"
#include "ngspice/inpdefs.h"


void
ASRCdestroy(GENmodel **model)
{
    ASRCmodel **start = (ASRCmodel**) model; /* starting model */
    ASRCinstance *here;     /* current instance */
    ASRCinstance *next;
    ASRCmodel *mod = *start;    /* current model */
    ASRCmodel *nextmod;

    for (; mod ; mod = nextmod) {
        for (here = mod->ASRCinstances; here; here = next) {
            next = here->ASRCnextInstance;
            FREE(here->ASRCacValues);
            INPfreeTree(here->ASRCtree);
            if (here->ASRCposptr)
                free(here->ASRCposptr);
            if (here->ASRCvars)
                free(here->ASRCvars);
            FREE(here);
        }
        nextmod = mod->ASRCnextModel;
        FREE(mod);
    }

    *model = NULL;
}
