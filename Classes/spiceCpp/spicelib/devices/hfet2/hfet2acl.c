/**********
Imported from MacSpice3f4 - Antony Wilson
Modified: Paolo Nenzi
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "hfet2defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int HFET2acLoad(GENmodel *inModel, CKTcircuit *ckt)
{
  
  HFET2model *model = (HFET2model*)inModel;
  HFET2instance *here;
  double gdpr;
  double gspr;
  double gm;
  double gds;
  double ggs;
  double xgs;
  double ggd;
  double xgd;

  double m;

  for( ; model != NULL; model = model->HFET2nextModel ) 
  {
    for( here = model->HFET2instances; here != NULL; 
         here = here->HFET2nextInstance) {

      m = here->HFET2m;

      gdpr=model->HFET2drainConduct;
      gspr=model->HFET2sourceConduct;
      gm= *(ckt->CKTstate0 + here->HFET2gm) ;
      gds= *(ckt->CKTstate0 + here->HFET2gds) ;
      ggs= *(ckt->CKTstate0 + here->HFET2ggs) ;
      xgs= *(ckt->CKTstate0 + here->HFET2qgs) * ckt->CKTomega ;
      ggd= *(ckt->CKTstate0 + here->HFET2ggd) ;
      xgd= *(ckt->CKTstate0 + here->HFET2qgd) * ckt->CKTomega ;

      *(here->HFET2drainDrainPtr )                 += m * (gdpr);
      *(here->HFET2gateGatePtr )                   += m * (ggd+ggs);
      *(here->HFET2gateGatePtr +1)                 += m * (xgd+xgs);
      *(here->HFET2sourceSourcePtr )               += m * (gspr);
      *(here->HFET2drainPrimeDrainPrimePtr )       += m * (gdpr+gds+ggd);
      *(here->HFET2drainPrimeDrainPrimePtr +1)     += m * (xgd);
      *(here->HFET2sourcePriHFET2ourcePrimePtr )   += m * (gspr+gds+gm+ggs);
      *(here->HFET2sourcePriHFET2ourcePrimePtr +1) += m * (xgs);
      *(here->HFET2drainDrainPrimePtr )            -= m * (gdpr);
      *(here->HFET2gateDrainPrimePtr )             -= m * (ggd);
      *(here->HFET2gateDrainPrimePtr +1)           -= m * (xgd);
      *(here->HFET2gateSourcePrimePtr )            -= m * (ggs);
      *(here->HFET2gateSourcePrimePtr +1)          -= m * (xgs);
      *(here->HFET2sourceSourcePrimePtr )          -= m * (gspr);
      *(here->HFET2drainPrimeDrainPtr )            -= m * (gdpr);
      *(here->HFET2drainPrimeGatePtr )             += m * (-ggd+gm);
      *(here->HFET2drainPrimeGatePtr +1)           -= m * (xgd);
      *(here->HFET2drainPriHFET2ourcePrimePtr )    += m * (-gds-gm);
      *(here->HFET2sourcePrimeGatePtr )            += m * (-ggs-gm);
      *(here->HFET2sourcePrimeGatePtr +1)          -= m * (xgs);
      *(here->HFET2sourcePriHFET2ourcePtr )        -= m * (gspr);
      *(here->HFET2sourcePrimeDrainPrimePtr )      -= m * (gds);
    }
  }
  return(OK);
  
}
