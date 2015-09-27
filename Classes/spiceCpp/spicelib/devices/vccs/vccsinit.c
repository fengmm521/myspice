#include "ngspice/config.h"

#include "ngspice/devdefs.h"

#include "vccsitf.h"
#include "vccsext.h"
#include "vccsinit.h"


SPICEdev VCCSinfo = {
    {
        "VCCS",
        "Voltage controlled current source",

        &VCCSnSize,
        &VCCSnSize,
        VCCSnames,

        &VCCSpTSize,
        VCCSpTable,

        NULL,
        NULL,

#ifdef XSPICE
/*----  Fixed by SDB 5.2.2003 to enable XSPICE/tclspice integration  -----*/
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */

        0,     /* This is a SPICE device, it has no MIF info data */
        NULL,  /* This is a SPICE device, it has no MIF info data */
/*---------------------------  End of SDB fix   -------------------------*/
#endif

	DEV_DEFAULT
    },

 /* DEVparam      */ VCCSparam,
 /* DEVmodParam   */ NULL,
 /* DEVload       */ VCCSload,
 /* DEVsetup      */ VCCSsetup,
 /* DEVunsetup    */ NULL,
 /* DEVpzSetup    */ VCCSsetup,
 /* DEVtemperature*/ NULL,
 /* DEVtrunc      */ NULL,
 /* DEVfindBranch */ NULL,
 /* DEVacLoad     */ VCCSload,   /* ac and normal loads are identical */
 /* DEVaccept     */ NULL,
 /* DEVdestroy    */ VCCSdestroy,
 /* DEVmodDelete  */ VCCSmDelete,
 /* DEVdelete     */ VCCSdelete,
 /* DEVsetic      */ NULL,
 /* DEVask        */ VCCSask,
 /* DEVmodAsk     */ NULL,
 /* DEVpzLoad     */ VCCSpzLoad,
 /* DEVconvTest   */ NULL,
 /* DEVsenSetup   */ VCCSsSetup,
 /* DEVsenLoad    */ VCCSsLoad,
 /* DEVsenUpdate  */ NULL,
 /* DEVsenAcLoad  */ VCCSsAcLoad,
 /* DEVsenPrint   */ VCCSsPrint,
 /* DEVsenTrunc   */ NULL,
 /* DEVdisto      */ NULL,	/* DISTO */
 /* DEVnoise      */ NULL,	/* NOISE */
 /* DEVsoaCheck   */ NULL,
#ifdef CIDER
 /* DEVdump       */ NULL,
 /* DEVacct       */ NULL,
#endif                        
 /* DEVinstSize   */ &VCCSiSize,
 /* DEVmodSize    */ &VCCSmSize


};


SPICEdev *
get_vccs_info(void)
{
    return &VCCSinfo;
}
