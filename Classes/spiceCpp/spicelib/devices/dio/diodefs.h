/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified by Paolo Nenzi 2003 and Dietmar Warning 2012
**********/
#ifndef DIO
#define DIO

#include "ngspice/ifsim.h"
#include "ngspice/gendefs.h"
#include "ngspice/cktdefs.h"
#include "ngspice/complex.h"
#include "ngspice/noisedef.h"

            /* data structures used to describe diodes */


/* information needed per instance */

typedef struct sDIOinstance {
    struct sDIOmodel *DIOmodPtr;    /* backpointer to model */
    struct sDIOinstance *DIOnextInstance;   /* pointer to next instance of 
                                             * current model*/
    IFuid DIOname;      /* pointer to character string naming this instance */
    int DIOstate;   /* pointer to start of state vector for diode */
    int DIOposNode;     /* number of positive node of diode */
    int DIOnegNode;     /* number of negative node of diode */
    int DIOposPrimeNode;    /* number of positive prime node of diode */

    double *DIOposPosPrimePtr;      /* pointer to sparse matrix at 
                                     * (positive,positive prime) */
    double *DIOnegPosPrimePtr;      /* pointer to sparse matrix at 
                                     * (negative,positive prime) */
    double *DIOposPrimePosPtr;      /* pointer to sparse matrix at 
                                     * (positive prime,positive) */
    double *DIOposPrimeNegPtr;      /* pointer to sparse matrix at 
                                     * (positive prime,negative) */
    double *DIOposPosPtr;   /* pointer to sparse matrix at 
                             * (positive,positive) */
    double *DIOnegNegPtr;   /* pointer to sparse matrix at 
                             * (negative,negative) */
    double *DIOposPrimePosPrimePtr; /* pointer to sparse matrix at 
                                     * (positive prime,positive prime) */

    double DIOcap;   /* stores the diode capacitance */

    double *DIOsens; /* stores the perturbed values of geq and ceq in ac
                         sensitivity analyis */

    int DIOsenParmNo ;   /* parameter # for sensitivity use;
                          * set equal to  0 if not a design parameter*/

    unsigned DIOoff : 1;   /* 'off' flag for diode */
    unsigned DIOareaGiven : 1;   /* flag to indicate area was specified */
    unsigned DIOpjGiven : 1;   /* flag to indicate perimeter was specified */
    unsigned DIOwGiven : 1;   /* flag to indicate width was specified */
    unsigned DIOlGiven : 1;   /* flag to indicate length was specified */
    unsigned DIOmGiven : 1;   /* flag to indicate multiplier was specified */

    unsigned DIOinitCondGiven : 1;  /* flag to indicate ic was specified */
    unsigned DIOsenPertFlag :1; /* indictes whether the the parameter of
                               the particular instance is to be perturbed */
    unsigned DIOtempGiven : 1;  /* flag to indicate temperature was specified */
    unsigned DIOdtempGiven : 1; /* flag to indicate dtemp given */

    double DIOarea;     /* area factor for the diode */
    double DIOpj;       /* perimeter for the diode */
    double DIOw;        /* width for the diode */
    double DIOl;        /* length for the diode */
    double DIOm;        /* multiplier for the diode */

    double DIOinitCond;      /* initial condition */
    double DIOtemp;          /* temperature of the instance */
    double DIOdtemp;         /* delta temperature of instance */
    double DIOtJctPot;       /* temperature adjusted junction potential */
    double DIOtJctCap;       /* temperature adjusted junction capacitance */
    double DIOtJctSWPot;     /* temperature adjusted sidewall junction potential */
    double DIOtJctSWCap;     /* temperature adjusted sidewall junction capacitance */
    double DIOtTransitTime;  /* temperature adjusted transit time */
    double DIOtGradingCoeff; /* temperature adjusted grading coefficient (MJ) */
    double DIOtConductance;  /* temperature adjusted series conductance */

    double DIOtDepCap;  /* temperature adjusted transition point in */
                        /* the curve matching (Fc * Vj ) */
    double DIOtDepSWCap;  /* temperature adjusted transition point in */
                          /* the curve matching (Fcs * Vjs ) */
    double DIOtSatCur;  /* temperature adjusted saturation current */
    double DIOtSatSWCur;  /* temperature adjusted side wall saturation current */
    double DIOtTunSatCur;        /* tunneling saturation current */
    double DIOtTunSatSWCur;      /* sidewall tunneling saturation current */

    double DIOtVcrit;   /* temperature adjusted V crit */
    double DIOtF1;      /* temperature adjusted f1 */
    double DIOtBrkdwnV; /* temperature adjusted breakdown voltage */
    
    double DIOtF2;     /* coeff. for capacitance equation precomputation */
    double DIOtF3;     /* coeff. for capacitance equation precomputation */
    double DIOtF2SW;   /* coeff. for capacitance equation precomputation */
    double DIOtF3SW;   /* coeff. for capacitance equation precomputation */

    double DIOforwardKneeCurrent; /* Forward Knee current */
    double DIOreverseKneeCurrent; /* Reverse Knee current */
    double DIOjunctionCap;     /* geometry adjusted junction capacitance */
    double DIOjunctionSWCap;     /* geometry adjusted junction sidewall capacitance */

/*
 * naming convention:
 * x = vdiode
 */

/* the following are relevant to s.s. sinusoidal distortion analysis */

#define DIONDCOEFFS        6

#ifndef NODISTO
        double DIOdCoeffs[DIONDCOEFFS];
#else /* NODISTO */
        double *DIOdCoeffs;
#endif /* NODISTO */

#ifndef CONFIG

#define        id_x2                DIOdCoeffs[0]
#define        id_x3                DIOdCoeffs[1]
#define        cdif_x2                DIOdCoeffs[2]
#define        cdif_x3                DIOdCoeffs[3]
#define        cjnc_x2                DIOdCoeffs[4]
#define        cjnc_x3                DIOdCoeffs[5]

#endif

/* indices to array of diode noise  sources */

#define DIORSNOIZ    0
#define DIOIDNOIZ    1
#define DIOFLNOIZ    2
#define DIOTOTNOIZ   3

#define DIONSRCS     4

#ifndef NONOISE
     double DIOnVar[NSTATVARS][DIONSRCS];
#else /* NONOISE */
        double **DIOnVar;
#endif /* NONOISE */

} DIOinstance ;

#define DIOsenGeq DIOsens /* stores the perturbed values of geq */
#define DIOsenCeq DIOsens + 3 /* stores the perturbed values of ceq */
#define DIOdphidp DIOsens + 6 


#define DIOvoltage DIOstate
#define DIOcurrent DIOstate+1
#define DIOconduct DIOstate+2
#define DIOcapCharge DIOstate+3
#define DIOcapCurrent DIOstate+4
#define DIOsensxp DIOstate+5    /* charge sensitivities and their derivatives.
                                 * +6 for the derivatives - pointer to the
                                 * beginning of the array */


/* per model data */

typedef struct sDIOmodel {       /* model structure for a diode */
    int DIOmodType; /* type index of this device type */
    struct sDIOmodel *DIOnextModel; /* pointer to next possible model in 
                                     * linked list */
    DIOinstance * DIOinstances; /* pointer to list of instances 
                                * that have this model */
    IFuid DIOmodName; /* pointer to character string naming this model */

    /* --- end of generic struct GENmodel --- */

    unsigned DIOlevelGiven : 1;
    unsigned DIOsatCurGiven : 1;
    unsigned DIOsatSWCurGiven : 1;

    unsigned DIOresistGiven : 1;
    unsigned DIOresistTemp1Given : 1;
    unsigned DIOresistTemp2Given : 1;
    unsigned DIOemissionCoeffGiven : 1;
    unsigned DIOswEmissionCoeffGiven : 1;
    unsigned DIObrkdEmissionCoeffGiven : 1;
    unsigned DIOtransitTimeGiven : 1;
    unsigned DIOtranTimeTemp1Given : 1;
    unsigned DIOtranTimeTemp2Given : 1;
    unsigned DIOjunctionCapGiven : 1;
    unsigned DIOjunctionPotGiven : 1;
    unsigned DIOgradingCoeffGiven : 1;
    unsigned DIOgradCoeffTemp1Given : 1;
    unsigned DIOgradCoeffTemp2Given : 1;
    unsigned DIOjunctionSWCapGiven : 1;
    unsigned DIOjunctionSWPotGiven : 1;
    unsigned DIOgradingSWCoeffGiven : 1;
    unsigned DIOforwardKneeCurrentGiven : 1;
    unsigned DIOreverseKneeCurrentGiven : 1;

    unsigned DIOtlevGiven : 1;
    unsigned DIOtlevcGiven : 1;
    unsigned DIOactivationEnergyGiven : 1;
    unsigned DIOsaturationCurrentExpGiven : 1;
    unsigned DIOctaGiven : 1;
    unsigned DIOctpGiven : 1;
    unsigned DIOtpbGiven : 1;
    unsigned DIOtphpGiven : 1;
    unsigned DIOdepletionCapCoeffGiven : 1;
    unsigned DIOdepletionSWcapCoeffGiven :1;
    unsigned DIObreakdownVoltageGiven : 1;
    unsigned DIObreakdownCurrentGiven : 1;
    unsigned DIOtcvGiven : 1;
    unsigned DIOnomTempGiven : 1;
    unsigned DIOfNcoefGiven : 1;
    unsigned DIOfNexpGiven : 1;
    unsigned DIOareaGiven : 1;
    unsigned DIOpjGiven : 1;

    unsigned DIOtunSatCurGiven : 1;
    unsigned DIOtunSatSWCurGiven : 1;
    unsigned DIOtunEmissionCoeffGiven : 1;
    unsigned DIOtunSaturationCurrentExpGiven : 1;
    unsigned DIOtunEGcorrectionFactorGiven : 1;
    unsigned DIOfv_maxGiven : 1;
    unsigned DIObv_maxGiven : 1;

    int    DIOlevel;   /* level selector */
    double DIOsatCur;   /* saturation current */
    double DIOsatSWCur;   /* Sidewall saturation current */

    double DIOresist;             /* ohmic series resistance */ 
    double DIOresistTemp1;        /* series resistance 1st order temp. coeff. */
    double DIOresistTemp2;        /* series resistance 2nd order temp. coeff. */
    double DIOconductance;        /* conductance corresponding to ohmic R */
    double DIOemissionCoeff;      /* emission coefficient (N) */
    double DIOswEmissionCoeff;    /* Sidewall emission coefficient (NS) */
    double DIObrkdEmissionCoeff;  /* Breakdown emission coefficient (NBV) */
    double DIOtransitTime;        /* transit time (TT) */
    double DIOtranTimeTemp1;      /* transit time 1st order coefficient */
    double DIOtranTimeTemp2;      /* transit time 2nd order coefficient */
    double DIOjunctionCap;        /* Junction Capacitance (Cj0) */
    double DIOjunctionPot;        /* Junction Potential (Vj) or (PB) */
    double DIOgradingCoeff;       /* grading coefficient (m) or (mj) */
    double DIOgradCoeffTemp1;     /* grading coefficient 1st order temp. coeff.*/
    double DIOgradCoeffTemp2;     /* grading coefficient 2nd order temp. coeff.*/
    double DIOjunctionSWCap;      /* Sidewall Junction Capacitance (Cjsw) */
    double DIOjunctionSWPot;      /* Sidewall Junction Potential (Vjsw) or (PBSW) */
    double DIOgradingSWCoeff;     /* Sidewall grading coefficient (mjsw) */
    double DIOforwardKneeCurrent; /* Forward Knee current (IKF) */
    double DIOreverseKneeCurrent; /* Reverse Knee current (IKR) */

    int    DIOtlev; /* Diode temperature equation selector */
    int    DIOtlevc; /* Diode temperature equation selector */
    double DIOactivationEnergy; /* activation energy (EG) */
    double DIOsaturationCurrentExp; /* Saturation current exponential (XTI) */
    double DIOcta; /* Area junction temperature coefficient */
    double DIOctp; /* Perimeter junction temperature coefficient */
    double DIOtpb; /* Area junction potential temperature coefficient */
    double DIOtphp; /* Perimeter junction potential temperature coefficient */
    double DIOdepletionCapCoeff;    /* Depletion Cap fraction coefficient (FC)*/
    double DIOdepletionSWcapCoeff;    /* Depletion sw-Cap fraction coefficient (FCS)*/
    double DIObreakdownVoltage; /* Voltage at reverse breakdown */
    double DIObreakdownCurrent; /* Current at above voltage */
    double DIOtcv; /* Reverse breakdown voltage temperature coefficient */
    double DIOarea;     /* area factor for the diode */
    double DIOpj;       /* perimeter for the diode */

    double DIOnomTemp;  /* nominal temperature at which parms measured */
    double DIOfNcoef;
    double DIOfNexp;

    double DIOtunSatCur;        /* tunneling saturation current (JTUN) */
    double DIOtunSatSWCur;      /* sidewall tunneling saturation current (JTUNSW) */
    double DIOtunEmissionCoeff; /* tunneling emission coefficient (NTUN) */
    double DIOtunSaturationCurrentExp; /* exponent for the tunneling current temperature (XTITUN) */
    double DIOtunEGcorrectionFactor; /* EG correction factor for tunneling (KEG) */
    double DIOfv_max; /* maximum voltage in forward direction */
    double DIObv_max; /* maximum voltage in reverse direction */

} DIOmodel;

/* device parameters */
#define DIO_AREA 1 
#define DIO_IC 2
#define DIO_OFF 3
#define DIO_CURRENT 4
#define DIO_VOLTAGE 5
#define DIO_CHARGE 6
#define DIO_CAPCUR 7
#define DIO_CONDUCT 8
#define DIO_AREA_SENS 9
#define DIO_POWER 10
#define DIO_TEMP 11
#define DIO_QUEST_SENS_REAL  12
#define DIO_QUEST_SENS_IMAG  13
#define DIO_QUEST_SENS_MAG   14
#define DIO_QUEST_SENS_PH    15
#define DIO_QUEST_SENS_CPLX  16
#define DIO_QUEST_SENS_DC    17
#define DIO_CAP 18
#define DIO_PJ 19 
#define DIO_W 20 
#define DIO_L 21 
#define DIO_M 22 
#define DIO_DTEMP 23

/* model parameters */
#define DIO_MOD_LEVEL 100
#define DIO_MOD_IS 101
#define DIO_MOD_RS 102
#define DIO_MOD_N 103
#define DIO_MOD_TT 104
#define DIO_MOD_CJO 105
#define DIO_MOD_VJ 106
#define DIO_MOD_M 107
#define DIO_MOD_EG 108
#define DIO_MOD_XTI 109
#define DIO_MOD_FC 110
#define DIO_MOD_BV 111
#define DIO_MOD_IBV 112
#define DIO_MOD_D 113
#define DIO_MOD_COND 114
#define DIO_MOD_TNOM 115
#define DIO_MOD_KF 116
#define DIO_MOD_AF 117
#define DIO_MOD_JSW 118
#define DIO_MOD_CJSW 119
#define DIO_MOD_VJSW 120
#define DIO_MOD_MJSW 121
#define DIO_MOD_IKF 122
#define DIO_MOD_IKR 123
#define DIO_MOD_FCS 124
#define DIO_MOD_TTT1 125
#define DIO_MOD_TTT2 126
#define DIO_MOD_TM1 127
#define DIO_MOD_TM2 128
#define DIO_MOD_TRS 129
#define DIO_MOD_TRS2 130
#define DIO_MOD_TLEV 131
#define DIO_MOD_TLEVC 132
#define DIO_MOD_CTA 133
#define DIO_MOD_CTP 134
#define DIO_MOD_TPB 135
#define DIO_MOD_TPHP 136
#define DIO_MOD_TCV 137
#define DIO_MOD_NBV 138
#define DIO_MOD_AREA 139
#define DIO_MOD_PJ 140
#define DIO_MOD_NS 141
#define DIO_MOD_JTUN 142
#define DIO_MOD_JTUNSW 143
#define DIO_MOD_NTUN 144
#define DIO_MOD_XTITUN 145
#define DIO_MOD_KEG 146
#define DIO_MOD_FV_MAX 147
#define DIO_MOD_BV_MAX 148

#include "dioext.h"
#endif /*DIO*/
