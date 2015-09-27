/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: September 2003 Paolo Nenzi
**********/

#ifndef CAP
#define CAP


#include "ngspice/ifsim.h"
#include "ngspice/complex.h"
#include "ngspice/gendefs.h"
#include "ngspice/cktdefs.h"

/* structures used to describe capacitors */


/* information to describe each instance */

typedef struct sCAPinstance {
    struct sCAPmodel *CAPmodPtr;    /* backpointer to model */
    struct sCAPinstance *CAPnextInstance;   /* pointer to next instance of
                                             * current model*/
    IFuid CAPname;  /* pointer to character string naming this instance */
    int CAPstate;   /* pointer to start of capacitor state vector */
    int CAPposNode; /* number of positive node of capacitor */
    int CAPnegNode; /* number of negative node of capacitor */

    double CAPtemp;     /* temperature at which this capacitor operates */
    double CAPdtemp;    /* delta-temperature of this instance */
    double CAPcapac;    /* capacitance */
    double CAPinitCond; /* initial capacitor voltage if specified */
    double CAPwidth;    /* width of the capacitor */
    double CAPlength;   /* length of the capacitor */
    double CAPscale;    /* scale factor */
    double CAPm;        /* parallel multiplier */
    double CAPtc1;      /* first temperature coefficient of capacitors */
    double CAPtc2;      /* second temperature coefficient of capacitors */
    double CAPbv_max;   /* Maximum capacitor voltage */

    double *CAPposPosptr;    /* pointer to sparse matrix diagonal at
                              * (positive,positive) */
    double *CAPnegNegptr;    /* pointer to sparse matrix diagonal at
                              * (negative,negative) */
    double *CAPposNegptr;    /* pointer to sparse matrix offdiagonal at
                              * (positive,negative) */
    double *CAPnegPosptr;    /* pointer to sparse matrix offdiagonal at
                              * (negative,positive) */
    unsigned CAPcapGiven    : 1;   /* flag to indicate capacitance was specified */
    unsigned CAPicGiven     : 1;   /* flag to indicate init. cond. was specified */
    unsigned CAPwidthGiven  : 1;   /* flag to indicate capacitor width given */
    unsigned CAPlengthGiven : 1;   /* flag to indicate capacitor length given*/
    unsigned CAPtempGiven   : 1;   /* flag to indicate operating temp given */
    unsigned CAPdtempGiven  : 1;   /* flag to indicate delta temp given */
    unsigned CAPscaleGiven  : 1;   /* flag to indicate scale factor given */
    unsigned CAPmGiven      : 1;   /* flag to indicate parallel multiplier given */
    unsigned CAPtc1Given    : 1;    /* flag indicates tc1 was specified */
    unsigned CAPtc2Given    : 1;    /* flag indicates tc2 was specified */
    unsigned CAPbv_maxGiven : 1;    /* flags indicates maximum voltage is given */
    int    CAPsenParmNo;         /* parameter # for sensitivity use;
                set equal to  0 if not a design parameter*/

} CAPinstance ;

#define CAPqcap CAPstate    /* charge on the capacitor */
#define CAPccap CAPstate+1  /* current through the capacitor */
#define CAPsensxp CAPstate+2 /* charge sensitivities and their derivatives.
+3 for the derivatives - pointer to the
beginning of the array */


/* data per model */

typedef struct sCAPmodel {      /* model structure for a capacitor */
    int CAPmodType; /* type index of this device type */
    struct sCAPmodel *CAPnextModel; /* pointer to next possible model in
                                     * linked list */
    CAPinstance * CAPinstances; /* pointer to list of instances that have this
                                 * model */
    IFuid CAPmodName;   /* pointer to character string naming this model */

    /* --- end of generic struct GENmodel --- */

    double CAPtnom;       /* temperature at which capacitance measured */
    double CAPtempCoeff1; /* linear temperature coefficient */
    double CAPtempCoeff2; /* quadratic temperature coefficient */
    double CAPmCap;       /* Model default capacitance */
    double CAPcj;         /* Unit Area Capacitance ( F/ M**2 ) */
    double CAPcjsw;       /* Unit Length Sidewall Capacitance ( F / M ) */
    double CAPdefWidth;   /* the default width of a capacitor */
    double CAPdefLength;  /* the default length of a capacitor */
    double CAPnarrow;     /* amount by which width are less than drawn */
    double CAPshort;      /* amount by which length are less than drawn */
    double CAPdel;        /* amount by which length and width are less than drawn */
    double CAPdi;         /* Relative dielectric constant */
    double CAPthick;      /* Insulator thickness */
    double CAPbv_max;     /* Maximum capacitor voltage */
    unsigned CAPmCapGiven      : 1;    /* flag indicates default capacitance given */
    unsigned CAPcjGiven        : 1;    /* Unit Area Capacitance ( F/ M**2 ) */
    unsigned CAPcjswGiven      : 1;    /* Unit Length Sidewall Capacitance( F/M )*/
    unsigned CAPdefWidthGiven  : 1;    /* flag indicates default width given*/
    unsigned CAPdefLengthGiven : 1;    /* flag indicates deafult lenght given */
    unsigned CAPnarrowGiven    : 1;    /* flag indicates narrowing factor given */
    unsigned CAPshortGiven     : 1;    /* flag indicates shortening factor given */
    unsigned CAPdelGiven       : 1;    /* flag indicates del factor given */
    unsigned CAPtnomGiven      : 1;    /* flag indicates nominal temp. given */
    unsigned CAPtc1Given       : 1;    /* flag indicates tc1 was specified */
    unsigned CAPtc2Given       : 1;    /* flag indicates tc2 was specified */
    unsigned CAPdiGiven        : 1;    /* flag indicates epsilon-ins given */
    unsigned CAPthickGiven     : 1;    /* flags indicates insulator thickness given */
    unsigned CAPbv_maxGiven    : 1;    /* flags indicates maximum voltage is given */

} CAPmodel;

/* device parameters */
#define CAP_CAP 1
#define CAP_IC 2
#define CAP_WIDTH 3
#define CAP_LENGTH 4
#define CAP_CAP_SENS 5
#define CAP_CURRENT 6
#define CAP_POWER 7
#define CAP_TEMP 8
#define CAP_DTEMP 9
#define CAP_SCALE 10
#define CAP_M 11
#define CAP_TC1 12
#define CAP_TC2 13
#define CAP_BV_MAX 14

/* model parameters */
#define CAP_MOD_CJ 101
#define CAP_MOD_CJSW 102
#define CAP_MOD_DEFWIDTH 103
#define CAP_MOD_C 104
#define CAP_MOD_NARROW 105
#define CAP_MOD_SHORT 106
#define CAP_MOD_DEL 107
#define CAP_MOD_TC1 108
#define CAP_MOD_TC2 109
#define CAP_MOD_TNOM 110
#define CAP_MOD_DI 111
#define CAP_MOD_THICK 112
#define CAP_MOD_CAP 113
#define CAP_MOD_DEFLENGTH 114
#define CAP_MOD_BV_MAX 115

/* device questions */
#define CAP_QUEST_SENS_REAL      201
#define CAP_QUEST_SENS_IMAG      202
#define CAP_QUEST_SENS_MAG       203
#define CAP_QUEST_SENS_PH        204
#define CAP_QUEST_SENS_CPLX      205
#define CAP_QUEST_SENS_DC        206

#include "capext.h"

#endif /*CAP*/
