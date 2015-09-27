/**********
STAG version 2.7
Copyright 2000 owned by the United Kingdom Secretary of State for Defence
acting through the Defence Evaluation and Research Agency.
Developed by :     Jim Benson,
                   Department of Electronics and Computer Science,
                   University of Southampton,
                   United Kingdom.
With help from :   Nele D'Halleweyn, Ketan Mistry, Bill Redman-White,
						 and Craig Easson.

Based on STAG version 2.1
Developed by :     Mike Lee,
With help from :   Bernard Tenbroek, Bill Redman-White, Mike Uren, Chris Edwards
                   and John Bunyan.
Acknowledgements : Rupert Howes and Pete Mole.
**********/

/********** 
Modified by Paolo Nenzi 2002
ngspice integration
**********/

#ifndef SOI3
#define SOI3

#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/gendefs.h"
#include "ngspice/complex.h"
#include "ngspice/noisedef.h"


/* declarations for SOI3 MOSFETs */

/* information needed for each instance */

typedef struct sSOI3instance {

    struct sSOI3model *sSOI3modPtr; /* backpointer to model */
    struct sSOI3instance *SOI3nextInstance;  /* pointer to next instance of
                                              *current model*/
    IFuid SOI3name;     /* pointer to character string naming this instance */
    int SOI3states;     /* index into state table for this device */


    int SOI3dNode;  /* number of the drain node of the mosfet */
    int SOI3gfNode;  /* number of the front gate node of the mosfet */
    int SOI3sNode;  /* number of the source node of the mosfet */
    int SOI3gbNode;   /* number of the back gate node of the mosfet */
    int SOI3bNode;  /* number of the bulk node of the mosfet */
    int SOI3toutNode;  /* number of thermal output node (tout) */

    int SOI3branch; /* branch number for zero voltage source if no thermal */

    int SOI3dNodePrime; /* number of the internal drain node of the mosfet */
    int SOI3sNodePrime; /* number of the internal source node of the mosfet */


    int SOI3tout1Node; /* first internal thermal node */
    int SOI3tout2Node; /* second internal thermal node */
    int SOI3tout3Node; /* third internal thermal node */
    int SOI3tout4Node; /* fourth internal thermal node */

    double SOI3l;   /* the length of the channel region */
    double SOI3w;   /* the width of the channel region */
    double SOI3m;   /* the parallel multiplier parameter */

    double SOI3as;			  /* Area of source region */
    double SOI3ad;           /* Area of drain region  */
    double SOI3ab;           /* Area of body region   */
    
    double SOI3drainSquares;    /* the length of the drain in squares */
    double SOI3sourceSquares;   /* the length of the source in squares */
    
    double SOI3sourceConductance;   /*conductance of source(or 0):set in setup*/
    double SOI3drainConductance;    /*conductance of drain(or 0):set in setup*/
    double SOI3temp;    /* operating temperature of this instance */
    double SOI3rt;          /* Thermal resistance */
    double SOI3ct;          /* Thermal capacitance */
    double SOI3rt1;          /* 1st internal Thermal resistance */
    double SOI3ct1;          /* 1st internal Thermal capacitance */
    double SOI3rt2;          /* 2nd internal Thermal resistance */
    double SOI3ct2;          /* 2nd internal Thermal capacitance */
    double SOI3rt3;          /* 3rd internal Thermal resistance */
    double SOI3ct3;          /* 3rd internal Thermal capacitance */
    double SOI3rt4;          /* 4th internal Thermal resistance */
    double SOI3ct4;          /* 4th internal Thermal capacitance */

    double SOI3tTransconductance;   /* temperature corrected transconductance (KP param) */
    double SOI3ueff;                /* passed on to noise model */
    double SOI3tSurfMob;            /* temperature corrected surface mobility */
    double SOI3tPhi;                /* temperature corrected Phi */
    double SOI3tVto;                /* temperature corrected Vto */
    double SOI3tVfbF;               /* temperature corrected Vfb */
    double SOI3tVfbB;               /* temperature corrected Vfb (back gate) */
    double SOI3tSatCur;             /* temperature corrected jnct saturation Cur. */
    double SOI3tSatCur1;             /* temperature corrected jnct saturation Cur. */
    double SOI3tSatCurDens;         /* temperature corrected jnct saturation Cur. density */
    double SOI3tSatCurDens1;         /* temperature corrected jnct saturation Cur. density */
    double SOI3tCbd;                /* temperature corrected B-D Capacitance */
    double SOI3tCbs;                /* temperature corrected B-S Capacitance */
    double SOI3tCjsw;       /* temperature corrected Bulk side Capacitance */
    double SOI3tBulkPot;    /* temperature corrected Bulk potential */
    double SOI3tDepCap;     /* temperature adjusted transition point in */
                            /* the curve matching Fc * Vj */
    double SOI3tVbi;        /* temperature adjusted Vbi diode built-in voltage */

    double SOI3icVBS;   /* initial condition B-S voltage */
    double SOI3icVDS;   /* initial condition D-S voltage */
    double SOI3icVGFS;  /* initial condition GF-S voltage */
    double SOI3icVGBS;  /* initial condition GB-S voltage */
    double SOI3von;
    double SOI3vdsat;
    double SOI3sourceVcrit; /* Vcrit for pos. vds */
    double SOI3drainVcrit;  /* Vcrit for pos. vds */
    double SOI3id;          /* DC drain current */
    double SOI3ibs;         /* bulk source current */
    double SOI3ibd;         /* bulk drain current */
    double SOI3iMdb;        /* drain bulk impact ionisation current */
    double SOI3iMsb;        /* source bulk impact ionisation cur. (rev mode) */
    double SOI3iPt;         /* heat 'current' in thermal circuit */
    double SOI3gmbs;
    double SOI3gmf;
    double SOI3gmb;
    double SOI3gds;
    double SOI3gt;          /* change of channel current wrt deltaT */
    double SOI3gdsnotherm;  /* gds0 at elevated temp - ac use only) */
    double SOI3gMmbs;
    double SOI3gMmf;
    double SOI3gMmb;
    double SOI3gMd;
    double SOI3gMdeltaT;
    double SOI3iBJTdb;
    double SOI3gBJTdb_bs;
    double SOI3gBJTdb_deltaT;
    double SOI3iBJTsb;
    double SOI3gBJTsb_bd;
    double SOI3gBJTsb_deltaT;
    double SOI3gPmf;        /* change of Pt wrt vgfs */
    double SOI3gPmb;        /* change of Pt wrt vgbs */
    double SOI3gPmbs;       /* change of Pt wrt vbs */
    double SOI3gPds;        /* change of Pt wrt vds */
    double SOI3gPdT;        /* change of Pt wrt deltaT */
    double SOI3gbd;         /* for body drain current */
    double SOI3gbdT;        /* for body drain current */
    double SOI3gbs;         /* for body source current */
    double SOI3gbsT;        /* for body source current */
    double SOI3capbd;
    double SOI3capbs;
    double SOI3Cbd;
    double SOI3Cbs;
    double SOI3f2d;
    double SOI3f3d;
    double SOI3f4d;
    double SOI3f2s;
    double SOI3f3s;
    double SOI3f4s;
    double SOI3dDT_dVds;    /* sm-sig gT term */
    double SOI3dId_dDT;     /* sm-sig source term */
/*debug stuff*/
    double SOI3debug1;
    double SOI3debug2;
    double SOI3debug3;
    double SOI3debug4;
    double SOI3debug5;
    double SOI3debug6;
/* extra stuff for newer model - msll Jan96 */

/*
 * naming convention:
 * x = vgs
 * y = vbs
 * z = vds
 * cdr = cdrain
 */
    int SOI3mode;       /* device mode : 1 = normal, -1 = inverse */
    int SOI3backstate;  /* indicates charge condition of back surface */
    int SOI3numThermalNodes; /* Number of thermal nodes required */

    unsigned SOI3off:1;  /* non-zero to indicate device is off for dc analysis*/
    unsigned SOI3tempGiven :1;  /* instance temperature specified */
    unsigned SOI3lGiven :1;
    unsigned SOI3wGiven :1;
    unsigned SOI3mGiven :1;
    unsigned SOI3asGiven:1;
    unsigned SOI3adGiven:1;
    unsigned SOI3abGiven:1;
    unsigned SOI3drainSquaresGiven  :1;
    unsigned SOI3sourceSquaresGiven :1;
    unsigned SOI3dNodePrimeSet  :1;
    unsigned SOI3sNodePrimeSet  :1;
    unsigned SOI3icVBSGiven :1;
    unsigned SOI3icVDSGiven :1;
    unsigned SOI3icVGFSGiven:1;
    unsigned SOI3icVGBSGiven:1;
    unsigned SOI3rtGiven:1;
    unsigned SOI3ctGiven:1;
    unsigned SOI3rt1Given:1;
    unsigned SOI3ct1Given:1;
    unsigned SOI3rt2Given:1;
    unsigned SOI3ct2Given:1;
    unsigned SOI3rt3Given:1;
    unsigned SOI3ct3Given:1;
    unsigned SOI3rt4Given:1;
    unsigned SOI3ct4Given:1;
    unsigned SOI3vonGiven   :1;
    unsigned SOI3vdsatGiven :1;
    unsigned SOI3modeGiven  :1;


    double *SOI3D_dPtr;      /* pointer to sparse matrix element at
                                     * (Drain node,drain node) */
    double *SOI3D_dpPtr;     /* pointer to sparse matrix element at
                                     * (drain node,drain prime node) */
    double *SOI3DP_dPtr;     /* pointer to sparse matrix element at
                                     * (drain prime node,drain node) */
    double *SOI3S_sPtr;      /* pointer to sparse matrix element at
                                     * (source node,source node) */
    double *SOI3S_spPtr;     /* pointer to sparse matrix element at
                                     * (source node,source prime node) */
    double *SOI3SP_sPtr;     /* pointer to sparse matrix element at
                                     * (source prime node,source node) */
    double *SOI3GF_gfPtr;    /* pointer to sparse matrix element at
                                     * (front gate node,front gate node) */
    double *SOI3GF_gbPtr;    /* pointer to sparse matrix element at
                                     * (front gate node,back gate node) */
    double *SOI3GF_dpPtr;    /* pointer to sparse matrix element at
                                     * (front gate node,drain prime node) */
    double *SOI3GF_spPtr;    /* pointer to sparse matrix element at
                                     * (front gate node,source prime node) */
    double *SOI3GF_bPtr;     /* pointer to sparse matrix element at
                                     * (front gate node,bulk node) */
    double *SOI3GB_gfPtr;    /* pointer to sparse matrix element at
                                     * (back gate node,front gate node) */
    double *SOI3GB_gbPtr;    /* pointer to sparse matrix element at
                                     * (back gate node,back gate node) */
    double *SOI3GB_dpPtr;    /* pointer to sparse matrix element at
                                     * (back gate node,drain prime node) */
    double *SOI3GB_spPtr;    /* pointer to sparse matrix element at
                                     * (back gate node,source prime node) */
    double *SOI3GB_bPtr;     /* pointer to sparse matrix element at
                                     * (back gate node,bulk node) */
    double *SOI3DP_gfPtr;    /* pointer to sparse matrix element at
                                     * (drain prime node,front gate node) */
    double *SOI3DP_gbPtr;    /* pointer to sparse matrix element at
                                     * (drain prime node,back gate node) */
    double *SOI3DP_dpPtr;    /* pointer to sparse matrix element at
                                     * (drain prime node,drain prime node) */
    double *SOI3DP_spPtr;    /* pointer to sparse matrix element at
                                     * (drain prime node,source prime node) */
    double *SOI3DP_bPtr;     /* pointer to sparse matrix element at
                                     * (drain prime node,bulk node) */
    double *SOI3SP_gfPtr;    /* pointer to sparse matrix element at
                                     * (source prime node,front gate node) */
    double *SOI3SP_gbPtr;    /* pointer to sparse matrix element at
                                     * (source prime node,back gate node) */
    double *SOI3SP_dpPtr;    /* pointer to sparse matrix element at
                                     * (source prime node,drain prime node) */
    double *SOI3SP_spPtr;    /* pointer to sparse matrix element at
                                     * (source prime node,source prime node) */
    double *SOI3SP_bPtr;     /* pointer to sparse matrix element at
                                     * (source prime node,bulk node) */
    double *SOI3B_gfPtr;     /* pointer to sparse matrix element at
                                     * (bulk node,front gate node) */
    double *SOI3B_gbPtr;     /* pointer to sparse matrix element at
                                     * (bulk node,back gate node) */
    double *SOI3B_dpPtr;     /* pointer to sparse matrix element at
                                     * (bulk node,drain prime node) */
    double *SOI3B_spPtr;     /* pointer to sparse matrix element at
                                     * (bulk node,source prime node) */
    double *SOI3B_bPtr;      /* pointer to sparse matrix element at
                                     * (bulk node,bulk node) */

/** Now for Thermal Node **/

    double *SOI3TOUT_toutPtr;
    double *SOI3TOUT_dpPtr;
    double *SOI3TOUT_gfPtr;
    double *SOI3TOUT_gbPtr;
    double *SOI3TOUT_bPtr;
    double *SOI3TOUT_spPtr;

    double *SOI3GF_toutPtr;
    double *SOI3GB_toutPtr;
    double *SOI3DP_toutPtr;
    double *SOI3SP_toutPtr;
    
    double *SOI3TOUT_ibrPtr;  /* these are for zero voltage source should */
    double *SOI3IBR_toutPtr;  /* no thermal behaviour be specified */

    double *SOI3B_toutPtr;    /* for impact ionisation current source */

    double *SOI3TOUT_tout1Ptr;
    double *SOI3TOUT1_toutPtr;
    double *SOI3TOUT1_tout1Ptr;
    double *SOI3TOUT1_tout2Ptr;
    double *SOI3TOUT2_tout1Ptr;
    double *SOI3TOUT2_tout2Ptr;
    double *SOI3TOUT2_tout3Ptr;
    double *SOI3TOUT3_tout2Ptr;
    double *SOI3TOUT3_tout3Ptr;
    double *SOI3TOUT3_tout4Ptr;
    double *SOI3TOUT4_tout3Ptr;
    double *SOI3TOUT4_tout4Ptr;


/* indices to the array of SOI(3) noise sources */

#define SOI3RDNOIZ       0
#define SOI3RSNOIZ       1
#define SOI3IDNOIZ       2
#define SOI3FLNOIZ       3
#define SOI3TOTNOIZ      4

#define SOI3NSRCS     5     /* the number of SOI(3) noise sources */

#ifndef NONOISE
    double SOI3nVar[NSTATVARS][SOI3NSRCS];
#else /* NONOISE */
	double **SOI3nVar;
#endif /* NONOISE */

} SOI3instance ;

#define SOI3vbd  SOI3states+ 0   /* bulk-drain voltage */
#define SOI3vbs  SOI3states+ 1   /* bulk-source voltage */
#define SOI3vgfs SOI3states+ 2   /* front gate-source voltage */
#define SOI3vgbs SOI3states+ 3   /* back gate-source voltage */
#define SOI3vds  SOI3states+ 4   /* drain-source voltage */
#define SOI3deltaT SOI3states+ 5  /* final temperature difference */

#define SOI3qgf SOI3states + 6  /* front gate charge */
#define SOI3iqgf SOI3states +7  /* front gate current */

#define SOI3qgb SOI3states+  8  /* back gate charge */
#define SOI3iqgb SOI3states+ 9  /* back gate current */

#define SOI3qd SOI3states+  10 /* drain charge */
#define SOI3iqd SOI3states+ 11 /* drain current */

#define SOI3qs SOI3states+  14 /* body charge */
#define SOI3iqs SOI3states+ 15 /* body current */

#define SOI3cgfgf SOI3states+     16 
#define SOI3cgfd SOI3states+      17 
#define SOI3cgfs SOI3states+      18 
#define SOI3cgfdeltaT SOI3states+ 19
#define SOI3cgfgb SOI3states+     20 

#define SOI3cdgf SOI3states+      21
#define SOI3cdd SOI3states+       22
#define SOI3cds SOI3states+       23
#define SOI3cddeltaT SOI3states+  24
#define SOI3cdgb SOI3states+      25

#define SOI3csgf SOI3states+      26
#define SOI3csd SOI3states+       27
#define SOI3css SOI3states+       28
#define SOI3csdeltaT SOI3states+  29
#define SOI3csgb SOI3states+      30

#define SOI3cgbgf SOI3states +    31
#define SOI3cgbd SOI3states +     32
#define SOI3cgbs SOI3states +     33
#define SOI3cgbdeltaT SOI3states+ 34
#define SOI3cgbgb SOI3states +    35

#define SOI3qbd SOI3states+       36  /* body-drain capacitor charge */
#define SOI3iqbd SOI3states+      37  /* body-drain capacitor current */

#define SOI3qbs SOI3states+       38  /* body-source capacitor charge */
#define SOI3iqbs SOI3states+      39  /* body-source capacitor current */

#define SOI3qt SOI3states+        40      /* Energy or 'charge' associated with ct */
#define SOI3iqt SOI3states+       41      /* equiv current source for ct */
#define SOI3qt1 SOI3states+       42      /* Energy or 'charge' associated with ct */
#define SOI3iqt1 SOI3states+      43      /* equiv current source for ct */
#define SOI3qt2 SOI3states+       44      /* Energy or 'charge' associated with ct */
#define SOI3iqt2 SOI3states+      45      /* equiv current source for ct */
#define SOI3qt3 SOI3states+       46      /* Energy or 'charge' associated with ct */
#define SOI3iqt3 SOI3states+      47      /* equiv current source for ct */
#define SOI3qt4 SOI3states+       48      /* Energy or 'charge' associated with ct */
#define SOI3iqt4 SOI3states+      49      /* equiv current source for ct */

#define SOI3qBJTbs SOI3states+    50
#define SOI3iqBJTbs SOI3states+   51

#define SOI3qBJTbd SOI3states+    52
#define SOI3iqBJTbd SOI3states+   53

#define SOI3cBJTbsbs SOI3states+     54
#define SOI3cBJTbsdeltaT SOI3states+ 55

#define SOI3cBJTbdbd SOI3states+     56
#define SOI3cBJTbddeltaT SOI3states+ 57

#define SOI3idrain SOI3states+    58  /* final drain current at timepoint (no define) */

#define SOI3deltaT1 SOI3states+   59  /* final temperature difference */
#define SOI3deltaT2 SOI3states+   60  /* final temperature difference */
#define SOI3deltaT3 SOI3states+   61  /* final temperature difference */
#define SOI3deltaT4 SOI3states+   62  /* final temperature difference */
#define SOI3deltaT5 SOI3states+   63  /* final temperature difference */

#define SOI3numStates 64

/* per model data */

    /* NOTE:  parameters marked 'input - use xxxx' are paramters for
     * which a temperature correction is applied in SOI3temp, thus
     * the SOI3xxxx value in the per-instance structure should be used
     * instead in all calculations
     */


typedef struct sSOI3model {       /* model structure for an SOI3 MOSFET  */

    int SOI3modType;    /* type index to this device type */
    struct sSOI3model *SOI3nextModel;    /* pointer to next possible model
                                          *in linked list */
    SOI3instance * SOI3instances; /* pointer to list of instances
                                   * that have this model */
    IFuid SOI3modName;       /* pointer to character string naming this model */

    /* --- end of generic struct GENmodel --- */

    int SOI3type;       /* device type : 1 = nsoi,  -1 = psoi */
    double SOI3tnom;        /* temperature at which parameters measured */
    double SOI3latDiff;
    double SOI3jctSatCurDensity;    /* input - use tSatCurDens (jnct)*/
    double SOI3jctSatCurDensity1;    /* input - use tSatCurDens1 (jnct)*/
    double SOI3jctSatCur;   /* input - use tSatCur (jnct Is)*/
    double SOI3jctSatCur1;   /* input - use tSatCur1 (jnct Is)*/
    double SOI3drainResistance;
    double SOI3sourceResistance;
    double SOI3sheetResistance;
    double SOI3transconductance;    /* (KP) input - use tTransconductance */
    double SOI3frontGateSourceOverlapCapFactor;
    double SOI3frontGateDrainOverlapCapFactor;
    double SOI3frontGateBulkOverlapCapFactor;
    double SOI3backGateSourceOverlapCapAreaFactor;
    double SOI3backGateDrainOverlapCapAreaFactor;
    double SOI3backGateBulkOverlapCapAreaFactor;
    double SOI3frontOxideCapFactor; /* Cof   NO DEFINES      */
    double SOI3backOxideCapFactor;  /* Cob       OR          */
    double SOI3bodyCapFactor;       /* Cb       FLAGS        */
    double SOI3C_bb;                /* Cb in series with Cob */
    double SOI3C_fb;                /* Cb in series with Cof */
    double SOI3C_ssf;               /* q*NQFF */
    double SOI3C_ssb;               /* q*NQFB */
    double SOI3C_fac;		    /* C_ob/(C_ob+C_b+C_ssb) */
    double SOI3vt0;    /* input - use tVto */
    double SOI3vfbF;   /* flat-band voltage. input - use tVfbF */
    double SOI3vfbB;   /* back flat-band voltage. input - use tVfbB */
    double SOI3gamma;   /* gamma */
    double SOI3gammaB;   /* back gamma */
    double SOI3capBD;   /* input - use tCbd */
    double SOI3capBS;   /* input - use tCbs */
    double SOI3sideWallCapFactor;   /* input - use tCjsw */
    double SOI3bulkJctPotential;    /* input - use tBulkPot */
    double SOI3bulkJctSideGradingCoeff; /* MJSW */
    double SOI3fwdCapDepCoeff;          /* FC */
    double SOI3phi; /* input - use tPhi */
    double SOI3vbi; /* input - use tVbi */
    double SOI3lambda;
    double SOI3theta;
    double SOI3substrateDoping;          /* Nsub */
    double SOI3substrateCharge;          /* Qb  - no define/flag */
    int SOI3gateType;        /* +1=same, -1=different, 0=Al */
    double SOI3frontFixedChargeDensity;
    double SOI3backFixedChargeDensity;
    double SOI3frontSurfaceStateDensity;
    double SOI3backSurfaceStateDensity;
    double SOI3frontOxideThickness;
    double SOI3backOxideThickness;
    double SOI3bodyThickness;
    double SOI3surfaceMobility; /* input - use tSurfMob */
    double SOI3oxideThermalConductivity;
    double SOI3siliconSpecificHeat;
    double SOI3siliconDensity;
    double SOI3fNcoef;
    double SOI3fNexp;
/* new stuff for newer model - msll Jan96 */
    double SOI3sigma; /* DIBL factor */
    double SOI3chiFB;   /* temperature coeff of flatband voltage */
    double SOI3chiPHI;  /* temperature coeff of PHI */
    double SOI3deltaW; /* narrow width effect factor */
    double SOI3deltaL; /* short channel effect factor */
    double SOI3vsat;   /* input - saturation velocity, use tVsat */
    double SOI3TVF0;   /* internal use - precalculation of exp to save time */
    double SOI3k;      /* thermal exponent for mobility factor */
    double SOI3lx;     /* channel length modulation factor */
    double SOI3vp;     /* channel length modulation empirical voltage */
    double SOI3eta;    /* Imp. ion. field adjustment factor */
    double SOI3alpha0;   /* 1st impact ionisation coeff */
    double SOI3beta0;   /* 2nd impact ionisation coeff */
    double SOI3lm;     /* impact ion. drain region length cf LX */
    double SOI3lm1;    /* impact ion. drain region coeff */
    double SOI3lm2;    /* impact ion. drain region coeff */
    double SOI3etad;   /* diode ideality factor */
    double SOI3etad1;   /* 2nd diode ideality factor */
    double SOI3chibeta; /* temp coeff of BETA0 */
    double SOI3chid;    /* temp factor for junction 1 */
    double SOI3chid1;   /* temp factor for junction 2 */
    int    SOI3dvt;     /* switch for temp dependence of vt in diodes */
    int    SOI3nLev;    /* level switch for noise model */
    double SOI3betaBJT; /* beta for Eber Moll BJT model */
    double SOI3tauFBJT;   /* forward BJT transit time */
    double SOI3tauRBJT;   /* reverse BJT transit time */
    double SOI3betaEXP;
    double SOI3tauEXP;
    double SOI3rsw;      /* source resistance width scaling factor */
    double SOI3rdw;      /* drain resistance width scaling factor */
    double SOI3minimumFeatureSize;     /* minimum feature size of simulated process technology */
    double SOI3vtex;       /* Extracted threshold voltage */
    double SOI3vdex;       /* Drain bias at which vtex extracted */
    double SOI3delta0;     /* Surface potential factor for vtex conversion */
    double SOI3satChargeShareFactor;     /* Saturation region charge sharing factor */
    double SOI3nplusDoping;     /* Doping concentration of N+ or P+ regions */
    double SOI3rta;     /* thermal resistance area scaling factor */
    double SOI3cta;     /* thermal capacitance area scaling factor */
    double SOI3mexp;    /* exponent for CLM smoothing */

    unsigned SOI3typeGiven  :1;
    unsigned SOI3latDiffGiven   :1;
    unsigned SOI3jctSatCurDensityGiven  :1;
    unsigned SOI3jctSatCurDensity1Given  :1;
    unsigned SOI3jctSatCurGiven :1;
    unsigned SOI3jctSatCur1Given :1;
    unsigned SOI3drainResistanceGiven   :1;
    unsigned SOI3sourceResistanceGiven  :1;
    unsigned SOI3sheetResistanceGiven   :1;
    unsigned SOI3transconductanceGiven  :1;
    unsigned SOI3frontGateSourceOverlapCapFactorGiven    :1;
    unsigned SOI3frontGateDrainOverlapCapFactorGiven :1;
    unsigned SOI3frontGateBulkOverlapCapFactorGiven  :1;
    unsigned SOI3backGateSourceOverlapCapAreaFactorGiven    :1;
    unsigned SOI3backGateDrainOverlapCapAreaFactorGiven :1;
    unsigned SOI3backGateBulkOverlapCapAreaFactorGiven  :1;
    unsigned SOI3subsBiasFactorGiven  :1;
    unsigned SOI3bodyFactorGiven      :1;
    unsigned SOI3vt0Given   :1;
    unsigned SOI3vfbFGiven  :1;
    unsigned SOI3vfbBGiven  :1;
    unsigned SOI3gammaGiven :1;
    unsigned SOI3gammaBGiven :1;
    unsigned SOI3capBDGiven :1;
    unsigned SOI3capBSGiven :1;
    unsigned SOI3sideWallCapFactorGiven   :1;
    unsigned SOI3bulkJctPotentialGiven  :1;
    unsigned SOI3bulkJctSideGradingCoeffGiven   :1;
    unsigned SOI3fwdCapDepCoeffGiven    :1;
    unsigned SOI3phiGiven   :1;
    unsigned SOI3lambdaGiven    :1;
    unsigned SOI3thetaGiven     :1;
    unsigned SOI3substrateDopingGiven   :1;
    unsigned SOI3gateTypeGiven  :1;
    unsigned SOI3frontFixedChargeDensityGiven   :1;
    unsigned SOI3backFixedChargeDensityGiven   :1;
    unsigned SOI3frontSurfaceStateDensityGiven   :1;
    unsigned SOI3backSurfaceStateDensityGiven  :1;
    unsigned SOI3frontOxideThicknessGiven   :1;
    unsigned SOI3backOxideThicknessGiven   :1;
    unsigned SOI3bodyThicknessGiven   :1;
    unsigned SOI3surfaceMobilityGiven   :1;
    unsigned SOI3tnomGiven  :1;
    unsigned SOI3oxideThermalConductivityGiven  :1;
    unsigned SOI3siliconSpecificHeatGiven  :1;
    unsigned SOI3siliconDensityGiven :1;
    unsigned SOI3fNcoefGiven :1;
    unsigned SOI3fNexpGiven :1;
/* extra stuff for newer model - msll Jan96 */
    unsigned SOI3sigmaGiven                :1;
    unsigned SOI3chiFBGiven                :1;
    unsigned SOI3chiPHIGiven               :1;
    unsigned SOI3deltaWGiven               :1;
    unsigned SOI3deltaLGiven               :1;
    unsigned SOI3vsatGiven                 :1;
    unsigned SOI3kGiven                    :1;
    unsigned SOI3lxGiven                   :1;
    unsigned SOI3vpGiven                   :1;
    unsigned SOI3useLAMBDA                 :1;
    unsigned SOI3etaGiven                  :1;
    unsigned SOI3alpha0Given               :1;
    unsigned SOI3beta0Given                :1;
    unsigned SOI3lmGiven                   :1;
    unsigned SOI3lm1Given                  :1;
    unsigned SOI3lm2Given                  :1;
    unsigned SOI3etadGiven                 :1;
    unsigned SOI3etad1Given                :1;
    unsigned SOI3chibetaGiven              :1;
    unsigned SOI3chidGiven                 :1;
    unsigned SOI3chid1Given                :1;
    unsigned SOI3dvtGiven                  :1;
    unsigned SOI3nLevGiven                 :1;
    unsigned SOI3betaBJTGiven              :1;
    unsigned SOI3tauFBJTGiven              :1;
    unsigned SOI3tauRBJTGiven              :1;
    unsigned SOI3betaEXPGiven              :1;
    unsigned SOI3tauEXPGiven               :1;
    unsigned SOI3rswGiven                  :1;
    unsigned SOI3rdwGiven                  :1;
    unsigned SOI3minimumFeatureSizeGiven   :1;
    unsigned SOI3vtexGiven                 :1;
    unsigned SOI3vdexGiven                 :1;
    unsigned SOI3delta0Given               :1;
    unsigned SOI3satChargeShareFactorGiven :1;
    unsigned SOI3nplusDopingGiven          :1;
    unsigned SOI3rtaGiven                  :1;
    unsigned SOI3ctaGiven                  :1;
    unsigned SOI3mexpGiven                 :1;

} SOI3model;

#ifndef NSOI3
#define NSOI3 1
#define PSOI3 -1
#endif /*NSOI3*/

/* device parameters */
#define SOI3_W 			 1
#define SOI3_L 			 2
#define SOI3_M			25
#define SOI3_AS 		 3
#define SOI3_AD 		 4
#define SOI3_AB 		 5
#define SOI3_PS 		 6
#define SOI3_PD 	 	 7
#define SOI3_PB 		 8
#define SOI3_NRS 		 9
#define SOI3_NRD 		10
#define SOI3_OFF 		11
#define SOI3_IC 		12
#define SOI3_IC_VBS 		13
#define SOI3_IC_VDS 		14
#define SOI3_IC_VGFS 		15
#define SOI3_IC_VGBS      	16
#define SOI3_W_SENS 		17
#define SOI3_L_SENS 		18
#define SOI3_IB 		19
#define SOI3_IGF 		20
#define SOI3_IGB           	21
#define SOI3_IS 		22
#define SOI3_POWER 		23
#define SOI3_TEMP 		24

/* model parameters */
#define SOI3_MOD_VTO      	101
#define SOI3_MOD_VFBF           149
#define SOI3_MOD_KP       	102
#define SOI3_MOD_GAMMA    	103
#define SOI3_MOD_PHI      	104
#define SOI3_MOD_LAMBDA   	105
#define SOI3_MOD_THETA          139
#define SOI3_MOD_RD       	106
#define SOI3_MOD_RS       	107
#define SOI3_MOD_CBD      	108
#define SOI3_MOD_CBS      	109
#define SOI3_MOD_IS       	110
#define SOI3_MOD_PB      	111
#define SOI3_MOD_CGFSO    	112
#define SOI3_MOD_CGFDO    	113
#define SOI3_MOD_CGFBO    	114
#define SOI3_MOD_CGBSO          144
#define SOI3_MOD_CGBDO          145
#define SOI3_MOD_CGBBO          146
#define SOI3_MOD_CJ       	115
#define SOI3_MOD_MJ       	116
#define SOI3_MOD_CJSW     	117
#define SOI3_MOD_MJSW     	118
#define SOI3_MOD_JS       	119
#define SOI3_MOD_TOF      	120
#define SOI3_MOD_TOB    	133
#define SOI3_MOD_TB          	134
#define SOI3_MOD_LD       	121
#define SOI3_MOD_RSH      	122
#define SOI3_MOD_U0       	123
#define SOI3_MOD_FC       	124
#define SOI3_MOD_NSUB     	125
#define SOI3_MOD_TPG      	126
#define SOI3_MOD_NQFF           147
#define SOI3_MOD_NQFB           148
#define SOI3_MOD_NSSF     	127
#define SOI3_MOD_NSSB         	135
#define SOI3_MOD_NSOI3    	128
#define SOI3_MOD_PSOI3    	129
#define SOI3_MOD_TNOM     	130
#define SOI3_MOD_KF       	131
#define SOI3_MOD_AF		132
#define SOI3_MOD_KOX		142
#define SOI3_MOD_SHSI		143
/* extra stuff for newer model - msll Jan96 */
#define SOI3_MOD_SIGMA          150
#define SOI3_MOD_CHIFB          151
#define SOI3_MOD_CHIPHI         152
#define SOI3_MOD_DELTAW         153
#define SOI3_MOD_DELTAL         154
#define SOI3_MOD_VSAT           155
#define SOI3_MOD_K              156
#define SOI3_MOD_LX             157
#define SOI3_MOD_VP             158
#define SOI3_MOD_ETA            159
#define SOI3_MOD_ALPHA0         140
#define SOI3_MOD_BETA0          141
#define SOI3_MOD_LM             160
#define SOI3_MOD_LM1            161
#define SOI3_MOD_LM2            162
#define SOI3_MOD_ETAD           163
#define SOI3_MOD_ETAD1          164
#define SOI3_MOD_IS1            165
#define SOI3_MOD_JS1            166
#define SOI3_MOD_CHIBETA        167
#define SOI3_MOD_VFBB           168
#define SOI3_MOD_GAMMAB         169
#define SOI3_MOD_CHID           170
#define SOI3_MOD_CHID1          171
#define SOI3_MOD_DVT            172
#define SOI3_MOD_NLEV           173
#define SOI3_MOD_BETABJT        174
#define SOI3_MOD_TAUFBJT        176
#define SOI3_MOD_TAURBJT        177
#define SOI3_MOD_BETAEXP        178
#define SOI3_MOD_TAUEXP         179
#define SOI3_MOD_RSW            180
#define SOI3_MOD_RDW            181
#define SOI3_MOD_FMIN           382
#define SOI3_MOD_VTEX           383
#define SOI3_MOD_VDEX           384
#define SOI3_MOD_DELTA0         385
#define SOI3_MOD_CSF            386
#define SOI3_MOD_DSI            387
#define SOI3_MOD_NPLUS          388
#define SOI3_MOD_RTA            389
#define SOI3_MOD_CTA            390
#define SOI3_MOD_MEXP           391

/* device questions */
#define SOI3_DNODE              201
#define SOI3_GFNODE             202
#define SOI3_SNODE              203
#define SOI3_GBNODE             204
#define SOI3_BNODE              205
#define SOI3_DNODEPRIME         206
#define SOI3_SNODEPRIME         207
#define SOI3_TNODE              208
#define SOI3_BRANCH             209
#define SOI3_SOURCECONDUCT      210
#define SOI3_DRAINCONDUCT       211
#define SOI3_VON                212
#define SOI3_VFBF               213
#define SOI3_VDSAT              214
#define SOI3_SOURCEVCRIT        215
#define SOI3_DRAINVCRIT         216
#define SOI3_ID                 217
#define SOI3_IBS                218
#define SOI3_IBD                219
#define SOI3_GMBS               220
#define SOI3_GMF                221
#define SOI3_GMB                222
#define SOI3_GDS                223
#define SOI3_GBD                224
#define SOI3_GBS                225
#define SOI3_CAPBD              226
#define SOI3_CAPBS              227
#define SOI3_CAPZEROBIASBD      228
#define SOI3_CAPZEROBIASBDSW    229
#define SOI3_CAPZEROBIASBS      230
#define SOI3_CAPZEROBIASBSSW    231
#define SOI3_VBD                232
#define SOI3_VBS                233
#define SOI3_VGFS               234
#define SOI3_VGBS               235
#define SOI3_VDS                236
#define SOI3_QGF                237
#define SOI3_IQGF               238
#define SOI3_QGB                239
#define SOI3_IQGB               240
#define SOI3_QD                 241
#define SOI3_IQD                242
#define SOI3_QS                 243
#define SOI3_IQS                244
#define SOI3_QBD                245
#define SOI3_IQBD               246
#define SOI3_QBS                247
#define SOI3_IQBS               248
#define SOI3_CGFGF              249
#define SOI3_CGFD               250
#define SOI3_CGFS               251
#define SOI3_CGFDELTAT          252
#define SOI3_CGFGB              253
#define SOI3_CDGF               254
#define SOI3_CDD                255
#define SOI3_CDS                256
#define SOI3_CDDELTAT           257
#define SOI3_CDGB               258
#define SOI3_CSGF               259
#define SOI3_CSD                260
#define SOI3_CSS                261
#define SOI3_CSDELTAT           262
#define SOI3_CSGB               263
#define SOI3_CGBGF              264
#define SOI3_CGBD               265
#define SOI3_CGBS               266
#define SOI3_CGBDELTAT          267
#define SOI3_CGBGB              268
#define SOI3_L_SENS_REAL        269
#define SOI3_L_SENS_IMAG        270
#define SOI3_L_SENS_MAG         271
#define SOI3_L_SENS_PH          272
#define SOI3_L_SENS_CPLX        273
#define SOI3_W_SENS_REAL        274
#define SOI3_W_SENS_IMAG        275
#define SOI3_W_SENS_MAG         276
#define SOI3_W_SENS_PH          277
#define SOI3_W_SENS_CPLX        278
#define SOI3_L_SENS_DC          279
#define SOI3_W_SENS_DC          280
#define SOI3_RT                 281
#define SOI3_CT                 282
#define SOI3_VFBB               283
#define SOI3_RT1                284
#define SOI3_CT1                285
#define SOI3_RT2                286
#define SOI3_CT2                287
#define SOI3_RT3                288
#define SOI3_CT3                289
#define SOI3_RT4                290
#define SOI3_CT4                291
#define SOI3_ITOT		292

/* model questions */

#include "soi3ext.h"

#endif /*SOI3*/

