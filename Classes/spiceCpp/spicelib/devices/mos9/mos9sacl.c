/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: Alan Gillespie

This function is obsolete (was used by an old sensitivity analysis)
**********/

/* actually load the current ac sensitivity 
 * information into the  array previously provided 
 */

#include "ngspice/ngspice.h"
#include "ngspice/smpdefs.h"
#include "ngspice/cktdefs.h"
#include "ngspice/const.h"
#include "mos9defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
MOS9sAcLoad(GENmodel *inModel, CKTcircuit *ckt)
{
    MOS9model *model = (MOS9model *)inModel;
    MOS9instance *here;
    int xnrm;
    int xrev;
    double A0;
    double Apert = 0.0;
    double DELA;
    double DELAinv;
    double gdpr0;
    double gspr0;
    double gds0;
    double gbs0;
    double gbd0;
    double gm0;
    double gmbs0;
    double gdpr;
    double gspr;
    double gds;
    double gbs;
    double gbd;
    double gm;
    double gmbs;
    double xcgs0;
    double xcgd0;
    double xcgb0;
    double xbd0;
    double xbs0;
    double xcgs;
    double xcgd;
    double xcgb;
    double xbd;
    double xbs;
    double vbsOp;
    double vbdOp;
    double vspr;
    double vdpr;
    double vgs;
    double vgd;
    double vgb;
    double vbs;
    double vbd;
    double vds;
    double ivspr;
    double ivdpr;
    double ivgs;
    double ivgd;
    double ivgb;
    double ivbs;
    double ivbd;
    double ivds;
    double cspr;
    double cdpr;
    double cgs;
    double cgd;
    double cgb;
    double cbs;
    double cbd;
    double cds;
    double cs0;
    double csprm0;
    double cd0;
    double cdprm0;
    double cg0;
    double cb0;
    double cs;
    double csprm;
    double cd;
    double cdprm;
    double cg;
    double cb;
    double icspr;
    double icdpr;
    double icgs;
    double icgd;
    double icgb;
    double icbs;
    double icbd;
    double icds;
    double ics0;
    double icsprm0;
    double icd0;
    double icdprm0;
    double icg0;
    double icb0;
    double ics;
    double icsprm;
    double icd;
    double icdprm;
    double icg;
    double icb;
    double DvDp = 0.0;
    int i;
    int flag;
    int error;
    int iparmno;
    double arg;
    double sarg;
    double sargsw;
    double SaveState[44];
    int    save_mode;
    SENstruct *info;

#ifdef SENSDEBUG
    printf("MOS9senacload\n");
    printf("CKTomega = %.5e\n",ckt->CKTomega);
#endif /* SENSDEBUG */
    info = ckt->CKTsenInfo;
    info->SENstatus = PERTURBATION;
    for( ; model != NULL; model = model->MOS9nextModel) {
        for(here = model->MOS9instances; here!= NULL;
                here = here->MOS9nextInstance) {

            /* save the unperturbed values in the state vector */
            for(i=0; i <= 16; i++)
                *(SaveState + i) = *(ckt->CKTstate0 + here->MOS9states + i);

            *(SaveState + 17) = here->MOS9sourceConductance;  
            *(SaveState + 18) = here->MOS9drainConductance;  
            *(SaveState + 19) = here->MOS9cd;  
            *(SaveState + 20) = here->MOS9cbs;  
            *(SaveState + 21) = here->MOS9cbd;  
            *(SaveState + 22) = here->MOS9gmbs;  
            *(SaveState + 23) = here->MOS9gm;  
            *(SaveState + 24) = here->MOS9gds;  
            *(SaveState + 25) = here->MOS9gbd;  
            *(SaveState + 26) = here->MOS9gbs;  
            *(SaveState + 27) = here->MOS9capbd;  
            *(SaveState + 28) = here->MOS9capbs;  
            *(SaveState + 29) = here->MOS9Cbd;  
            *(SaveState + 30) = here->MOS9Cbdsw;  
            *(SaveState + 31) = here->MOS9Cbs;  
            *(SaveState + 32) = here->MOS9Cbssw;  
            *(SaveState + 33) = here->MOS9f2d;  
            *(SaveState + 34) = here->MOS9f3d;  
            *(SaveState + 35) = here->MOS9f4d;  
            *(SaveState + 36) = here->MOS9f2s;  
            *(SaveState + 37) = here->MOS9f3s;  
            *(SaveState + 38) = here->MOS9f4s;  
            *(SaveState + 39) = here->MOS9cgs;  
            *(SaveState + 40) = here->MOS9cgd;  
            *(SaveState + 41) = here->MOS9cgb;  
            *(SaveState + 42) = here->MOS9vdsat;  
            *(SaveState + 43) = here->MOS9von;  
            save_mode  = here->MOS9mode;  

            xnrm=1;
            xrev=0;
            if (here->MOS9mode < 0) {
                xnrm=0;
                xrev=1;
            }

            vbsOp = model->MOS9type * ( 
            *(ckt->CKTrhsOp+here->MOS9bNode) -
                *(ckt->CKTrhsOp+here->MOS9sNodePrime));
            vbdOp = model->MOS9type * ( 
            *(ckt->CKTrhsOp+here->MOS9bNode) -
                *(ckt->CKTrhsOp+here->MOS9dNodePrime));
            vspr = *(ckt->CKTrhsOld + here->MOS9sNode) 
                - *(ckt->CKTrhsOld +
                    here->MOS9sNodePrime) ;
            ivspr = *(ckt->CKTirhsOld + here->MOS9sNode) 
                - *(ckt->CKTirhsOld +
                    here->MOS9sNodePrime) ;
            vdpr = *(ckt->CKTrhsOld + here->MOS9dNode) 
                - *(ckt->CKTrhsOld +
                    here->MOS9dNodePrime) ;
            ivdpr = *(ckt->CKTirhsOld + here->MOS9dNode) 
                - *(ckt->CKTirhsOld +
                    here->MOS9dNodePrime) ;
            vgb = *(ckt->CKTrhsOld + here->MOS9gNode) 
                - *(ckt->CKTrhsOld +
                    here->MOS9bNode) ;
            ivgb = *(ckt->CKTirhsOld + here->MOS9gNode) 
                - *(ckt->CKTirhsOld +
                    here->MOS9bNode) ;
            vbs = *(ckt->CKTrhsOld + here->MOS9bNode) 
                - *(ckt->CKTrhsOld +
                    here->MOS9sNodePrime) ;
            ivbs = *(ckt->CKTirhsOld + here->MOS9bNode) 
                - *(ckt->CKTirhsOld +
                    here->MOS9sNodePrime) ;
            vbd = *(ckt->CKTrhsOld + here->MOS9bNode) 
                - *(ckt->CKTrhsOld +
                    here->MOS9dNodePrime) ;
            ivbd = *(ckt->CKTirhsOld + here->MOS9bNode) 
                - *(ckt->CKTirhsOld +
                    here->MOS9dNodePrime) ;
            vds = vbs - vbd ;
            ivds = ivbs - ivbd ;
            vgs = vgb + vbs ;
            ivgs = ivgb + ivbs ;
            vgd = vgb + vbd ;
            ivgd = ivgb + ivbd ;

#ifdef SENSDEBUG
            printf("senacload instance name %s\n",here->MOS9name);
            printf("gate = %d ,drain = %d, drainprm = %d\n",
                    here->MOS9gNode,here->MOS9dNode,here->MOS9dNodePrime);
            printf("source = %d , sourceprm = %d ,body = %d, senparmno = %d\n",
                    here->MOS9sNode ,here->MOS9sNodePrime,
                    here->MOS9bNode,here->MOS9senParmNo);
            printf("\n without  perturbation \n");
#endif /* SENSDEBUG */
            /*  without  perturbation  */
            *(ckt->CKTstate0 + here->MOS9vbs) = vbsOp;
            *(ckt->CKTstate0 + here->MOS9vbd) = vbdOp;

            here->MOS9senPertFlag = ON ;
            if(info->SENacpertflag == 1){
                /* store the  unperturbed values of small signal parameters */
                if((error = MOS9load((GENmodel*)model,ckt)) != 0) return(error);
                *(here->MOS9senCgs) = here->MOS9cgs;
                *(here->MOS9senCgd) = here->MOS9cgd;
                *(here->MOS9senCgb) = here->MOS9cgb;
                *(here->MOS9senCbd) = here->MOS9capbd;
                *(here->MOS9senCbs) = here->MOS9capbs;
                *(here->MOS9senGds) = here->MOS9gds;
                *(here->MOS9senGbs) = here->MOS9gbs;
                *(here->MOS9senGbd) = here->MOS9gbd;
                *(here->MOS9senGm) = here->MOS9gm;
                *(here->MOS9senGmbs) = here->MOS9gmbs;

            }
            xcgs0= *(here->MOS9senCgs) * ckt->CKTomega;
            xcgd0= *(here->MOS9senCgd) * ckt->CKTomega;
            xcgb0= *(here->MOS9senCgb) * ckt->CKTomega;
            xbd0= *(here->MOS9senCbd) * ckt->CKTomega;
            xbs0= *(here->MOS9senCbs) * ckt->CKTomega;
            gds0= *(here->MOS9senGds);
            gbs0= *(here->MOS9senGbs);
            gbd0= *(here->MOS9senGbd);
            gm0= *(here->MOS9senGm);
            gmbs0= *(here->MOS9senGmbs);
            gdpr0 = here->MOS9drainConductance;
            gspr0 = here->MOS9sourceConductance;


            cspr = gspr0 * vspr ;
            icspr = gspr0 * ivspr ;
            cdpr = gdpr0 * vdpr ;
            icdpr = gdpr0 * ivdpr ;
            cgs = ( - xcgs0 * ivgs );
            icgs =  xcgs0 * vgs ;
            cgd = ( - xcgd0 * ivgd );
            icgd =  xcgd0 * vgd ;
            cgb = ( - xcgb0 * ivgb );
            icgb =  xcgb0 * vgb ;
            cbs = ( gbs0 * vbs  - xbs0 * ivbs );
            icbs = ( xbs0 * vbs  + gbs0 * ivbs );
            cbd = ( gbd0 * vbd  - xbd0 * ivbd );
            icbd = ( xbd0 * vbd  + gbd0 * ivbd );
            cds = ( gds0 * vds  + xnrm * (gm0 * vgs + gmbs0 * vbs) 
                - xrev * (gm0 * vgd + gmbs0 * vbd) );
            icds = ( gds0 * ivds + xnrm * (gm0 * ivgs + gmbs0 * ivbs)
                - xrev * (gm0 * ivgd + gmbs0 * ivbd) );

            cs0 = cspr;
            ics0 = icspr;
            csprm0 = ( -cspr - cgs - cbs - cds ) ;
            icsprm0 = ( -icspr - icgs - icbs - icds ) ;
            cd0 = cdpr;
            icd0 = icdpr;
            cdprm0 = ( -cdpr - cgd - cbd + cds ) ;
            icdprm0 = ( -icdpr - icgd - icbd + icds ) ;
            cg0 = cgs + cgd + cgb ;
            icg0 = icgs + icgd + icgb ;
            cb0 = cbs + cbd - cgb ;
            icb0 = icbs + icbd - icgb ;
#ifdef SENSDEBUG
            printf("gspr0 = %.7e , gdpr0 = %.7e , gds0 = %.7e, gbs0 = %.7e\n",
                    gspr0,gdpr0,gds0,gbs0);
            printf("gbd0 = %.7e , gm0 = %.7e , gmbs0 = %.7e\n",gbd0,gm0,gmbs0);
            printf("xcgs0 = %.7e , xcgd0 = %.7e ,", xcgs0,xcgd0);
            printf("xcgb0 = %.7e, xbd0 = %.7e,xbs0 = %.7e\n" ,xcgb0,xbd0,xbs0);
            printf("vbs = %.7e , vbd = %.7e , vgb = %.7e\n",vbs,vbd,vgb);
            printf("ivbs = %.7e , ivbd = %.7e , ivgb = %.7e\n",ivbs,ivbd,ivgb);
            printf("cbs0 = %.7e , cbd0 = %.7e , cgb0 = %.7e\n",cbs,cbd,cgb);
            printf("cb0 = %.7e , cg0 = %.7e , cs0 = %.7e\n",cb0,cg0,cs0);
            printf("csprm0 = %.7e, cd0 = %.7e, cdprm0 = %.7e\n",
                    csprm0,cd0,cdprm0);
            printf("icb0 = %.7e , icg0 = %.7e , ics0 = %.7e\n",icb0,icg0,ics0);
            printf("icsprm0 = %.7e, icd0 = %.7e, icdprm0 = %.7e\n",
                    icsprm0,icd0,icdprm0);
            printf("\nPerturbation of vbs\n");
#endif /* SENSDEBUG */

            /* Perturbation of vbs */
            flag = 1;
            A0 = vbsOp;
            DELA =  info->SENpertfac * CONSTvt0 ;
            DELAinv = 1.0/DELA;

            if(info->SENacpertflag == 1){
                /* store the  values of small signal parameters 
                 * corresponding to perturbed vbs */
                Apert = A0 + DELA;
                *(ckt->CKTstate0 + here->MOS9vbs) = Apert;
                *(ckt->CKTstate0 + here->MOS9vbd) = vbdOp;

                if((error = MOS9load((GENmodel*)model,ckt)) != 0) return(error);

                *(here->MOS9senCgs + 1) = here->MOS9cgs;
                *(here->MOS9senCgd + 1) = here->MOS9cgd;
                *(here->MOS9senCgb + 1) = here->MOS9cgb;
                *(here->MOS9senCbd + 1) = here->MOS9capbd;
                *(here->MOS9senCbs + 1) = here->MOS9capbs;
                *(here->MOS9senGds + 1) = here->MOS9gds;
                *(here->MOS9senGbs + 1) = here->MOS9gbs;
                *(here->MOS9senGbd + 1) = here->MOS9gbd;
                *(here->MOS9senGm + 1) = here->MOS9gm;
                *(here->MOS9senGmbs + 1) = here->MOS9gmbs;

                *(ckt->CKTstate0 + here->MOS9vbs) = A0;


            }

            goto load;


pertvbd:  /* Perturbation of vbd */
#ifdef SENSDEBUG
            printf("\nPerturbation of vbd\n");
#endif /* SENSDEBUG */

            flag = 2;
            A0 = vbdOp;
            DELA =  info->SENpertfac * CONSTvt0 + 1e-8;
            DELAinv = 1.0/DELA;

            if(info->SENacpertflag == 1){
                /* store the  values of small signal parameters 
                 * corresponding to perturbed vbd */
                Apert = A0 + DELA;
                *(ckt->CKTstate0 + here->MOS9vbs) = vbsOp;
                *(ckt->CKTstate0 + here->MOS9vbd) = Apert;

                if((error = MOS9load((GENmodel*)model,ckt)) != 0) return(error);

                *(here->MOS9senCgs + 2) = here->MOS9cgs;
                *(here->MOS9senCgd + 2) = here->MOS9cgd;
                *(here->MOS9senCgb + 2) = here->MOS9cgb;
                *(here->MOS9senCbd + 2) = here->MOS9capbd;
                *(here->MOS9senCbs + 2) = here->MOS9capbs;
                *(here->MOS9senGds + 2) = here->MOS9gds;
                *(here->MOS9senGbs + 2) = here->MOS9gbs;
                *(here->MOS9senGbd + 2) = here->MOS9gbd;
                *(here->MOS9senGm + 2) = here->MOS9gm;
                *(here->MOS9senGmbs + 2) = here->MOS9gmbs;

                *(ckt->CKTstate0 + here->MOS9vbd) = A0;

            }

            goto load;


pertvgb:  /* Perturbation of vgb */
#ifdef SENSDEBUG
            printf("\nPerturbation of vgb\n");
#endif /* SENSDEBUG */

            flag = 3;
            A0 = model->MOS9type * (*(ckt->CKTrhsOp + here->MOS9gNode) 
                -  *(ckt->CKTrhsOp + here->MOS9bNode)); 
            DELA =  info->SENpertfac * A0 + 1e-8;
            DELAinv = model->MOS9type * 1.0/DELA;


            if(info->SENacpertflag == 1){
                /* store the  values of small signal parameters 
                 * corresponding to perturbed vgb */
                *(ckt->CKTstate0 + here->MOS9vbs) = vbsOp;
                *(ckt->CKTstate0 + here->MOS9vbd) = vbdOp;
                *(ckt->CKTrhsOp + here->MOS9bNode) -= DELA; 

                if((error = MOS9load((GENmodel*)model,ckt)) != 0) return(error);

                *(here->MOS9senCgs + 3) = here->MOS9cgs;
                *(here->MOS9senCgd + 3) = here->MOS9cgd;
                *(here->MOS9senCgb + 3) = here->MOS9cgb;
                *(here->MOS9senCbd + 3) = here->MOS9capbd;
                *(here->MOS9senCbs + 3) = here->MOS9capbs;
                *(here->MOS9senGds + 3) = here->MOS9gds;
                *(here->MOS9senGbs + 3) = here->MOS9gbs;
                *(here->MOS9senGbd + 3) = here->MOS9gbd;
                *(here->MOS9senGm + 3) = here->MOS9gm;
                *(here->MOS9senGmbs + 3) = here->MOS9gmbs;


                *(ckt->CKTrhsOp + here->MOS9bNode) += DELA; 
            }
            goto load;

pertl:    /* Perturbation of length */

            if(here->MOS9sens_l == 0){
                goto pertw;
            }
#ifdef SENSDEBUG
            printf("\nPerturbation of length\n");
#endif /* SENSDEBUG */
            flag = 4;
            A0 = here->MOS9l;
            DELA =  info->SENpertfac * A0;
            DELAinv = 1.0/DELA;

            if(info->SENacpertflag == 1){
                /* store the  values of small signal parameters 
                 * corresponding to perturbed length */
                Apert = A0 + DELA;
                here->MOS9l = Apert;

                *(ckt->CKTstate0 + here->MOS9vbs) = vbsOp;
                *(ckt->CKTstate0 + here->MOS9vbd) = vbdOp;

                if((error = MOS9load((GENmodel*)model,ckt)) != 0) return(error);

                *(here->MOS9senCgs + 4) = here->MOS9cgs;
                *(here->MOS9senCgd + 4) = here->MOS9cgd;
                *(here->MOS9senCgb + 4) = here->MOS9cgb;
                *(here->MOS9senCbd + 4) = here->MOS9capbd;
                *(here->MOS9senCbs + 4) = here->MOS9capbs;
                *(here->MOS9senGds + 4) = here->MOS9gds;
                *(here->MOS9senGbs + 4) = here->MOS9gbs;
                *(here->MOS9senGbd + 4) = here->MOS9gbd;
                *(here->MOS9senGm + 4) = here->MOS9gm;
                *(here->MOS9senGmbs + 4) = here->MOS9gmbs;

                here->MOS9l = A0;

            }

            goto load;

pertw:    /* Perturbation of width */
            if(here->MOS9sens_w == 0)
                goto next;
#ifdef SENSDEBUG
            printf("\nPerturbation of width\n");
#endif /* SENSDEBUG */
            flag = 5;
            A0 = here->MOS9w;
            DELA = info->SENpertfac * A0;
            DELAinv = 1.0/DELA;
            Apert = A0 + DELA;

            if(info->SENacpertflag == 1){
                /* store the  values of small signal parameters 
                 * corresponding to perturbed width */
                here->MOS9w = Apert;
                here->MOS9drainArea *= (1 + info->SENpertfac);
                here->MOS9sourceArea *= (1 + info->SENpertfac);
                here->MOS9Cbd *= (1 + info->SENpertfac);
                here->MOS9Cbs *= (1 + info->SENpertfac);
                if(here->MOS9drainPerimiter){
                    here->MOS9Cbdsw += here->MOS9Cbdsw *
                        DELA/here->MOS9drainPerimiter;
                }
                if(here->MOS9sourcePerimiter){
                    here->MOS9Cbssw += here->MOS9Cbssw *
                        DELA/here->MOS9sourcePerimiter;
                }
                if(vbdOp >= here->MOS9tDepCap){
                    arg = 1-model->MOS9fwdCapDepCoeff;
                    sarg = exp( (-model->MOS9bulkJctBotGradingCoeff) * 
                            log(arg) );
                    sargsw = exp( (-model->MOS9bulkJctSideGradingCoeff) * 
                            log(arg) );
                    here->MOS9f2d = here->MOS9Cbd*(1-model->MOS9fwdCapDepCoeff*
                            (1+model->MOS9bulkJctBotGradingCoeff))* sarg/arg
                            +  here->MOS9Cbdsw*(1-model->MOS9fwdCapDepCoeff*
                            (1+model->MOS9bulkJctSideGradingCoeff))*
                            sargsw/arg;
                    here->MOS9f3d = here->MOS9Cbd * 
                            model->MOS9bulkJctBotGradingCoeff * sarg/arg/
                            model->MOS9bulkJctPotential
                            + here->MOS9Cbdsw * 
                            model->MOS9bulkJctSideGradingCoeff * sargsw/arg /
                            model->MOS9bulkJctPotential;
                    here->MOS9f4d = here->MOS9Cbd*model->MOS9bulkJctPotential*
                            (1-arg*sarg)/ (1-model->MOS9bulkJctBotGradingCoeff)
                            + here->MOS9Cbdsw*model->MOS9bulkJctPotential*
                            (1-arg*sargsw)/
                            (1-model->MOS9bulkJctSideGradingCoeff)
                            -here->MOS9f3d/2*
                            (here->MOS9tDepCap*here->MOS9tDepCap)
                            -here->MOS9tDepCap * here->MOS9f2d;
                }
                if(vbsOp >= here->MOS9tDepCap){
                    arg = 1-model->MOS9fwdCapDepCoeff;
                    sarg = exp( (-model->MOS9bulkJctBotGradingCoeff) * 
                            log(arg) );
                    sargsw = exp( (-model->MOS9bulkJctSideGradingCoeff) * 
                            log(arg) );
                    here->MOS9f2s = here->MOS9Cbs*(1-model->MOS9fwdCapDepCoeff*
                        (1+model->MOS9bulkJctBotGradingCoeff))* sarg/arg
                        +  here->MOS9Cbssw*(1-model->MOS9fwdCapDepCoeff*
                        (1+model->MOS9bulkJctSideGradingCoeff))*
                        sargsw/arg;
                    here->MOS9f3s = here->MOS9Cbs * 
                            model->MOS9bulkJctBotGradingCoeff * sarg/arg/
                            model->MOS9bulkJctPotential + here->MOS9Cbssw * 
                            model->MOS9bulkJctSideGradingCoeff * sargsw/arg /
                            model->MOS9bulkJctPotential;
                    here->MOS9f4s = here->MOS9Cbs*model->MOS9bulkJctPotential*
                            (1-arg*sarg)/ (1-model->MOS9bulkJctBotGradingCoeff)
                            + here->MOS9Cbssw*model->MOS9bulkJctPotential*
                            (1-arg*sargsw)/
                            (1-model->MOS9bulkJctSideGradingCoeff)
                            -here->MOS9f3s/2*
                            (here->MOS9tBulkPot*here->MOS9tBulkPot)
                            -here->MOS9tBulkPot * here->MOS9f2s;
                }

                *(ckt->CKTstate0 + here->MOS9vbs) = vbsOp;
                *(ckt->CKTstate0 + here->MOS9vbd) = vbdOp;

                if((error = MOS9load((GENmodel*)model,ckt)) != 0) return(error);

                *(here->MOS9senCgs + 5) = here->MOS9cgs;
                *(here->MOS9senCgd + 5) = here->MOS9cgd;
                *(here->MOS9senCgb + 5) = here->MOS9cgb;
                *(here->MOS9senCbd + 5) = here->MOS9capbd;
                *(here->MOS9senCbs + 5) = here->MOS9capbs;
                *(here->MOS9senGds + 5) = here->MOS9gds;
                *(here->MOS9senGbs + 5) = here->MOS9gbs;
                *(here->MOS9senGbd + 5) = here->MOS9gbd;
                *(here->MOS9senGm + 5) = here->MOS9gm;
                *(here->MOS9senGmbs + 5) = here->MOS9gmbs;

                here->MOS9w = A0;
                here->MOS9drainArea /= (1 + info->SENpertfac);
                here->MOS9sourceArea /= (1 + info->SENpertfac);
            }

load:

            gds= *(here->MOS9senGds + flag);
            gbs= *(here->MOS9senGbs + flag);
            gbd= *(here->MOS9senGbd + flag);
            gm= *(here->MOS9senGm + flag);
            gmbs= *(here->MOS9senGmbs + flag);
            if(flag == 5){
                gdpr = here->MOS9drainConductance * Apert/A0;
                gspr = here->MOS9sourceConductance * Apert/A0;
            }
            else{
                gdpr = here->MOS9drainConductance;
                gspr = here->MOS9sourceConductance;
            }

            xcgs= *(here->MOS9senCgs + flag) * ckt->CKTomega;
            xcgd= *(here->MOS9senCgd + flag) * ckt->CKTomega;
            xcgb= *(here->MOS9senCgb + flag) * ckt->CKTomega;
            xbd= *(here->MOS9senCbd + flag) * ckt->CKTomega;
            xbs= *(here->MOS9senCbs + flag) * ckt->CKTomega;

#ifdef SENSDEBUG
            printf("flag = %d \n",flag);
            printf("gspr = %.7e , gdpr = %.7e , gds = %.7e, gbs = %.7e\n",
                    gspr,gdpr,gds,gbs);
            printf("gbd = %.7e , gm = %.7e , gmbs = %.7e\n",gbd,gm,gmbs);
            printf("xcgs = %.7e , xcgd = %.7e , xcgb = %.7e,", xcgs,xcgd,xcgb);
            printf("xbd = %.7e,xbs = %.7e\n" ,xbd,xbs);
#endif /* SENSDEBUG */
            cspr = gspr * vspr ;
            icspr = gspr * ivspr ;
            cdpr = gdpr * vdpr ;
            icdpr = gdpr * ivdpr ;
            cgs = ( - xcgs * ivgs );
            icgs =  xcgs * vgs ;
            cgd = ( - xcgd * ivgd );
            icgd =  xcgd * vgd ;
            cgb = ( - xcgb * ivgb );
            icgb =  xcgb * vgb ;
            cbs = ( gbs * vbs  - xbs * ivbs );
            icbs = ( xbs * vbs  + gbs * ivbs );
            cbd = ( gbd * vbd  - xbd * ivbd );
            icbd = ( xbd * vbd  + gbd * ivbd );
            cds = ( gds * vds  + xnrm * (gm * vgs + gmbs * vbs) 
                - xrev * (gm * vgd + gmbs * vbd) );
            icds = ( gds * ivds + xnrm * (gm * ivgs + gmbs * ivbs)
                - xrev * (gm * ivgd + gmbs * ivbd) );

            cs = cspr;
            ics = icspr;
            csprm = ( -cspr - cgs - cbs - cds ) ;
            icsprm = ( -icspr - icgs - icbs - icds ) ;
            cd = cdpr;
            icd = icdpr;
            cdprm = ( -cdpr - cgd - cbd + cds ) ;
            icdprm = ( -icdpr - icgd - icbd + icds ) ;
            cg = cgs + cgd + cgb ;
            icg = icgs + icgd + icgb ;
            cb = cbs + cbd - cgb ;
            icb = icbs + icbd - icgb ;

#ifdef SENSDEBUG
            printf("vbs = %.7e , vbd = %.7e , vgb = %.7e\n",vbs,vbd,vgb);
            printf("ivbs = %.7e , ivbd = %.7e , ivgb = %.7e\n",ivbs,ivbd,ivgb);
            printf("cbs = %.7e , cbd = %.7e , cgb = %.7e\n",cbs,cbd,cgb);
            printf("cb = %.7e , cg = %.7e , cs = %.7e\n",cb,cg,cs);
            printf("csprm = %.7e, cd = %.7e, cdprm = %.7e\n",csprm,cd,cdprm);
            printf("icb = %.7e , icg = %.7e , ics = %.7e\n",icb,icg,ics);
            printf("icsprm = %.7e, icd = %.7e, icdprm = %.7e\n",
                    icsprm,icd,icdprm);
#endif /* SENSDEBUG */
            for(iparmno = 1;iparmno<=info->SENparms;iparmno++){
                if( (flag == 4) && (iparmno != here->MOS9senParmNo) ) continue;
                if( (flag == 5) && (iparmno != (here->MOS9senParmNo + 
                        here->MOS9sens_l)) ) continue;

                switch(flag){
                case 1: 
                    DvDp = model->MOS9type * 
                            (info->SEN_Sap[here->MOS9bNode][iparmno]
                            -  info->SEN_Sap[here->MOS9sNodePrime][iparmno]);
                    break;
                case 2: 
                    DvDp = model->MOS9type * 
                            ( info->SEN_Sap[here->MOS9bNode][iparmno]
                            -  info->SEN_Sap[here->MOS9dNodePrime][iparmno]);
                    break;
                case 3: 
                    DvDp = model->MOS9type * 
                            ( info->SEN_Sap[here->MOS9gNode][iparmno]
                            -  info->SEN_Sap[here->MOS9bNode][iparmno]);
                    break;
                case 4: 
                    DvDp = 1;
                    break;
                case 5: 
                    DvDp = 1;
                    break;
                }
                *(info->SEN_RHS[here->MOS9bNode] + iparmno) -=  
                        ( cb  - cb0) * DELAinv * DvDp;
                *(info->SEN_iRHS[here->MOS9bNode] + iparmno) -=  
                        ( icb  - icb0) * DELAinv * DvDp;

                *(info->SEN_RHS[here->MOS9gNode] + iparmno) -=  
                        ( cg  - cg0) * DELAinv * DvDp;
                *(info->SEN_iRHS[here->MOS9gNode] + iparmno) -=  
                        ( icg  - icg0) * DELAinv * DvDp;

                if(here->MOS9sNode != here->MOS9sNodePrime){
                    *(info->SEN_RHS[here->MOS9sNode] + iparmno) -=  
                            ( cs  - cs0) * DELAinv * DvDp;
                    *(info->SEN_iRHS[here->MOS9sNode] + iparmno) -=  
                            ( ics  - ics0) * DELAinv * DvDp;
                }

                *(info->SEN_RHS[here->MOS9sNodePrime] + iparmno) -=  
                        ( csprm  - csprm0) * DELAinv * DvDp;
                *(info->SEN_iRHS[here->MOS9sNodePrime] + iparmno) -=  
                        ( icsprm  - icsprm0) * DELAinv * DvDp;

                if(here->MOS9dNode != here->MOS9dNodePrime){
                    *(info->SEN_RHS[here->MOS9dNode] + iparmno) -=  
                            ( cd  - cd0) * DELAinv * DvDp;
                    *(info->SEN_iRHS[here->MOS9dNode] + iparmno) -=  
                            ( icd  - icd0) * DELAinv * DvDp;
                }

                *(info->SEN_RHS[here->MOS9dNodePrime] + iparmno) -=  
                        ( cdprm  - cdprm0) * DELAinv * DvDp;
                *(info->SEN_iRHS[here->MOS9dNodePrime] + iparmno) -=  
                        ( icdprm  - icdprm0) * DELAinv * DvDp;
#ifdef SENSDEBUG
                printf("after loading\n");  
                printf("DvDp = %.5e , DELAinv = %.5e ,flag = %d ,",
                        DvDp,DELAinv,flag);
                printf("iparmno = %d,senparmno = %d\n",
                        iparmno,here->MOS9senParmNo);
                printf("A0 = %.5e , Apert = %.5e ,CONSTvt0 = %.5e \n",
                        A0,Apert,CONSTvt0);
                printf("senb = %.7e + j%.7e ",
                        *(info->SEN_RHS[here->MOS9bNode] + iparmno),
                        *(info->SEN_iRHS[here->MOS9bNode] + iparmno));
                printf("seng = %.7e + j%.7e ",
                        *(info->SEN_RHS[here->MOS9gNode] + iparmno),
                        *(info->SEN_iRHS[here->MOS9gNode] + iparmno));
                printf("sens = %.7e + j%.7e ",
                        *(info->SEN_RHS[here->MOS9sNode] + iparmno),
                        *(info->SEN_iRHS[here->MOS9sNode] + iparmno));
                printf("sensprm = %.7e + j%.7e ",
                        *(info->SEN_RHS[here->MOS9sNodePrime] + iparmno),
                        *(info->SEN_iRHS[here->MOS9sNodePrime] + iparmno));
                printf("send = %.7e + j%.7e ",
                        *(info->SEN_RHS[here->MOS9dNode] + iparmno),
                        *(info->SEN_iRHS[here->MOS9dNode] + iparmno));
                printf("sendprm = %.7e + j%.7e ",
                        *(info->SEN_RHS[here->MOS9dNodePrime] + iparmno),
                        *(info->SEN_iRHS[here->MOS9dNodePrime] + iparmno));
#endif /* SENSDEBUG */

            }
            switch(flag){
            case 1: 
                goto pertvbd ;
            case 2: 
                goto pertvgb ; 
            case 3: 
                goto pertl ;
            case 4: 
                goto pertw ;
            case 5: 
                break; 
            }
next:                   
            ;

            /* put the unperturbed values back into the state vector */
            for(i=0; i <= 16; i++)
                *(ckt->CKTstate0 + here->MOS9states + i) = *(SaveState + i);

            here->MOS9sourceConductance = *(SaveState + 17) ;
            here->MOS9drainConductance = *(SaveState + 18) ; 
            here->MOS9cd =  *(SaveState + 19) ;  
            here->MOS9cbs =  *(SaveState + 20) ;  
            here->MOS9cbd =  *(SaveState + 21) ;  
            here->MOS9gmbs =  *(SaveState + 22) ;  
            here->MOS9gm =  *(SaveState + 23) ;  
            here->MOS9gds =  *(SaveState + 24) ;  
            here->MOS9gbd =  *(SaveState + 25) ;  
            here->MOS9gbs =  *(SaveState + 26) ;  
            here->MOS9capbd =  *(SaveState + 27) ;  
            here->MOS9capbs =  *(SaveState + 28) ;  
            here->MOS9Cbd =  *(SaveState + 29) ;  
            here->MOS9Cbdsw =  *(SaveState + 30) ;  
            here->MOS9Cbs =  *(SaveState + 31) ;  
            here->MOS9Cbssw =  *(SaveState + 32) ;  
            here->MOS9f2d =  *(SaveState + 33) ;  
            here->MOS9f3d =  *(SaveState + 34) ;  
            here->MOS9f4d =  *(SaveState + 35) ;  
            here->MOS9f2s =  *(SaveState + 36) ;  
            here->MOS9f3s =  *(SaveState + 37) ;  
            here->MOS9f4s =  *(SaveState + 38) ;  
            here->MOS9cgs = *(SaveState + 39) ;  
            here->MOS9cgd = *(SaveState + 40) ;  
            here->MOS9cgb = *(SaveState + 41) ;  
            here->MOS9vdsat = *(SaveState + 42) ;  
            here->MOS9von = *(SaveState + 43) ;  
            here->MOS9mode = save_mode ;  

            here->MOS9senPertFlag = OFF;
        }
    }
    info->SENstatus = NORMAL;
#ifdef SENSDEBUG
    printf("MOS9senacload end\n");
#endif /* SENSDEBUG */
    return(OK);
}


