/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1987 Gary W. Ng
**********/

/*
 * NevalSrc (noise, lnNoise, ckt, type, node1, node2, param)
 *   This routine evaluates the noise due to different physical
 *   phenomena.  This includes the "shot" noise associated with dc
 *   currents in semiconductors and the "thermal" noise associated with
 *   resistance.  Although semiconductors also display "flicker" (1/f)
 *   noise, the lack of a unified model requires us to handle it on a
 *   "case by case" basis.  What we CAN provide, though, is the noise
 *   gain associated with the 1/f source.
 */


#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "ngspice/const.h"
#include "ngspice/noisedef.h"


void
NevalSrc (double *noise, double *lnNoise, CKTcircuit *ckt, int type, int node1, int node2, double param)
{
    double realVal;
    double imagVal;
    double gain;

    realVal = ckt->CKTrhs [node1] - ckt->CKTrhs [node2];
    imagVal = ckt->CKTirhs [node1] - ckt->CKTirhs [node2];
    gain = (realVal*realVal) + (imagVal*imagVal);
    switch (type) {

    case SHOTNOISE:
        *noise = gain * 2 * CHARGE * fabs(param);          /* param is the dc current in a semiconductor */
          *lnNoise = log( MAX(*noise,N_MINLOG) );
        break;

    case THERMNOISE:
        *noise = gain * 4 * CONSTboltz * ckt->CKTtemp * param;         /* param is the conductance of a resistor */
        *lnNoise = log( MAX(*noise,N_MINLOG) );
        break;

    case N_GAIN:
        *noise = gain;
        break;

    }
}

/*
 * NevalSrc2 (noise, lnNoise, ckt, type, node1, node2, param, node3, node4, param2)
 *   This routine is a modified version of NevalSrc() that computes
 *   the output noise due to two fully-correlated noise sources. It is
 *   useful for implementing correlated gate and drain noises in MOSFETs.
 *
 *   Like NevalSrc(), it evaluates the noise due to different physical
 *   phenomena.  This includes the "shot" noise associated with dc
 *   currents in semiconductors and the "thermal" noise associated with
 *   resistance.  Although semiconductors also display "flicker" (1/f)
 *   noise, the lack of a unified model requires us to handle it on a
 *   "case by case" basis.  What we CAN provide, though, is the noise
 *   gain associated with the 1/f source.
 */
/* Modified by Darsen Lu for BSIM4 tnoiMod=2 10/10/2010
*/

void
NevalSrc2 (
double *noise,
double *lnNoise,
CKTcircuit *ckt,
int type,
int node1,
int node2,
double param1,
int node3,
int node4,
double param2,
double phi21)     /* Phase of signal 2 relative to signal 1 */

{
    double realVal1, imagVal1;
    double realVal2, imagVal2;
    double realOut, imagOut, param_gain;
    double T0, T1, T2, T3;

    realVal1 = ckt->CKTrhs [node1] - ckt->CKTrhs [node2];
    imagVal1 = ckt->CKTirhs [node1] - ckt->CKTirhs [node2];
    realVal2 = ckt->CKTrhs [node3] - ckt->CKTrhs [node4];
    imagVal2 = ckt->CKTirhs [node3] - ckt->CKTirhs [node4];
    T0 = sqrt(param1);
    T1 = sqrt(param2);
    T2 = T1 * cos(phi21);
    T3 = T1 * sin(phi21);
    realOut = T0 * realVal1 + T2 * realVal2 - T3 * imagVal2;
    imagOut = T0 * imagVal1 + T2 * imagVal2 + T3 * realVal2;
    param_gain = (realOut*realOut) + (imagOut*imagOut);
    switch (type) {

    case SHOTNOISE:
        *noise = 2.0 * CHARGE * fabs(param_gain);          /* param is the dc current in a semiconductor */
          *lnNoise = log( MAX(*noise,N_MINLOG) );
        break;

    case THERMNOISE:
        *noise = 4.0 * CONSTboltz * ckt->CKTtemp * param_gain;         /* param is the conductance of a resistor */
        *lnNoise = log( MAX(*noise,N_MINLOG) );
        break;

    case N_GAIN:
        *noise = 0.0;
        break;

    }
}


/*
PN 2003:
The following function includes instance dtemp in
thermal noise calculation.
It will replace NevalSrc as soon as all devices
will implement dtemp feature.
*/

void
NevalSrcInstanceTemp (double *noise, double *lnNoise, CKTcircuit *ckt, int type,
           int node1, int node2, double param, double param2)
{
    double realVal;
    double imagVal;
    double gain;

    realVal = ckt->CKTrhs [node1] - ckt->CKTrhs [node2];
    imagVal = ckt->CKTirhs [node1] - ckt->CKTirhs [node2];
    gain = (realVal*realVal) + (imagVal*imagVal);
    switch (type) {

    case SHOTNOISE:
        *noise = gain * 2 * CHARGE * fabs(param);          /* param is the dc current in a semiconductor */
          *lnNoise = log( MAX(*noise,N_MINLOG) );
        break;

    case THERMNOISE:
        *noise = gain * 4 * CONSTboltz * (ckt->CKTtemp + param2)  /* param2 is the instance temperature difference */
                 * param;                                         /* param is the conductance of a resistor */
        *lnNoise = log( MAX(*noise,N_MINLOG) );
        break;

    case N_GAIN:
        *noise = gain;
        break;

    }
}
