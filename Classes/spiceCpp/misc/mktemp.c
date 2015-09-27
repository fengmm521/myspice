/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
**********/

/*
 * A more portable version of the standard "mktemp( )" function
 *
 * FIXME: remove smktemp() and adjust all callers to use tmpfile(3).
 */

#include "ngspice/ngspice.h"
#include "mktemp.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


#ifndef TEMPFORMAT
#define TEMPFORMAT "temp%s%d"
#endif

char *
smktemp(char *id)
{
    if (!id)
        id = "sp";

    return tprintf(TEMPFORMAT, id, getpid());
}
