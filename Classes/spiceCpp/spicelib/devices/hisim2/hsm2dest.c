/***********************************************************************

 HiSIM (Hiroshima University STARC IGFET Model)
 Copyright (C) 2014 Hiroshima University & STARC

 MODEL NAME : HiSIM
 ( VERSION : 2  SUBVERSION : 8  REVISION : 0 )
 
 FILE : hsm2dest.c

 Date : 2014.6.5

 released by 
                Hiroshima University &
                Semiconductor Technology Academic Research Center (STARC)
***********************************************************************/

/**********************************************************************

The following source code, and all copyrights, trade secrets or other
intellectual property rights in and to the source code in its entirety,
is owned by the Hiroshima University and the STARC organization.

All users need to follow the "HiSIM2 Distribution Statement and
Copyright Notice" attached to HiSIM2 model.

-----HiSIM2 Distribution Statement and Copyright Notice--------------

Software is distributed as is, completely without warranty or service
support. Hiroshima University or STARC and its employees are not liable
for the condition or performance of the software.

Hiroshima University and STARC own the copyright and grant users a perpetual,
irrevocable, worldwide, non-exclusive, royalty-free license with respect 
to the software as set forth below.   

Hiroshima University and STARC hereby disclaim all implied warranties.

Hiroshima University and STARC grant the users the right to modify, copy,
and redistribute the software and documentation, both within the user's
organization and externally, subject to the following restrictions

1. The users agree not to charge for Hiroshima University and STARC code
itself but may charge for additions, extensions, or support.

2. In any product based on the software, the users agree to acknowledge
Hiroshima University and STARC that developed the software. This
acknowledgment shall appear in the product documentation.

3. The users agree to reproduce any copyright notice which appears on
the software on any copy or modification of such made available
to others."


*************************************************************************/

#include "ngspice/ngspice.h"
#include "hsm2def.h"
#include "ngspice/suffix.h"

void HSM2destroy(
     GENmodel **inModel)
{
  HSM2model **model = (HSM2model**)inModel;
  HSM2instance *here;
  HSM2instance *prev = NULL;
  HSM2model *mod = *model;
  HSM2model *oldmod = NULL;
  
  for ( ;mod ;mod = mod->HSM2nextModel ) {
    if (oldmod) FREE(oldmod);
    oldmod = mod;
    prev = (HSM2instance *)NULL;
    for ( here = mod->HSM2instances ;here ;here = here->HSM2nextInstance ) {
      if (prev) FREE(prev);
      prev = here;
    }
    if (prev) FREE(prev);
  }
  if (oldmod) {
#ifdef USE_OMP
      /* free just once for all models */
      FREE(oldmod->HSM2InstanceArray);
#endif
      FREE(oldmod);
  }
  *model = NULL;
}

