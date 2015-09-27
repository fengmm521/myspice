/*************
 * Header file for parse.c
 * 1999 E. Rouat
 ************/

#ifndef ngspice_PARSE_H
#define ngspice_PARSE_H

#include "ngspice/pnode.h"
#include "ngspice/wordlist.h"

#ifndef free_pnode
#define free_pnode(ptr)  free_pnode_x(ptr); ptr=NULL;
#endif


#endif
