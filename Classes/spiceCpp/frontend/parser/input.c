/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1988 Jeffrey M. Hsu
**********/

/*
 * Stand-alone input routine.
 */
#include "ngspice/ngspice.h"

#include <errno.h>

#include "ngspice/fteinput.h"
#include "input.h"
#include "ngspice/cpextern.h"
#include "../display.h"
#ifdef _MSC_VER
#include "BaseTsd.h" /* for SSIZE_T */
#define ssize_t SSIZE_T
#ifndef HAS_WINGUI
#define read _read /* only for console */
#endif
#endif


/* A special 'getc' so that we can deal with ^D properly. There is no way for
 * stdio to know if we have typed a ^D after some other characters, so
 * don't use buffering at all
 */

extern void pauseThread();

FILE* cp_nIn = NULL;
FILE* cp_nOut = NULL;
FILE* cp_nErr = NULL;
void setCP_nIN(FILE* nin)
{
    cp_nIn = nin;
}

void setCP_nOUT(FILE* nout)
{
    cp_nOut = nout;
    cp_out = cp_nOut;
}

void setCP_nErr(FILE* nerr)
{
    cp_nErr = nerr;
    cp_err = cp_nErr;
}



int
inchar(FILE *fp)
{
#ifndef HAS_WINGUI
    char c;
    ssize_t i;

    if (cp_interactive && !cp_nocc) {
        do{
            i = read(fileno(cp_nIn), &c, 1);
        }while (i == -1 && errno == EINTR);

        if (i == 0 || c == '\004') {
            pauseThread();
            c = '\n';
            return ((int) c);
            //return (EOF);
        } else if (i == -1) {
            perror("read");
            return (EOF);
        } else {
            return ((int) c);
        }
    } else
#endif
        return (getc(fp));
}


int
input(FILE *fp)
{
    REQUEST request;
    RESPONSE response;

    request.option = char_option;
    request.fp = fp;

    Input(&request, &response);
    return (inchar(fp));
}
