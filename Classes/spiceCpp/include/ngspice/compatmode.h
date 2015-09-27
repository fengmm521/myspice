#ifndef ngspice_COMPATMODE_H
#define ngspice_COMPATMODE_H

#include "ngspice/config.h"

typedef enum {
  COMPATMODE_NATIVE = 0,
  COMPATMODE_HS = 1,
  COMPATMODE_SPICE3 = 2,
  COMPATMODE_ALL = 3,
  COMPATMODE_PS = 4
} COMPATMODE_T ;

extern COMPATMODE_T ngspice_compat_mode(void) ;

#endif
