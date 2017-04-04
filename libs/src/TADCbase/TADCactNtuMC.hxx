#ifndef _TADCactNtuMC_HXX
#define _TADCactNtuMC_HXX
/*!
  \file
  \version $Id: adapted from TABMactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TADCactNtuMC.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.h"
#include "foot_geo.h"

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

class TADCactNtuMC : public TAGaction {
  public:
    explicit        TADCactNtuMC(const char* name=0,
				 TAGdataDsc* p_nturaw=0, 
				 TAGparaDsc* p_parcon=0, 
         TAGparaDsc* p_pargeo=0, 
				 EVENT_STRUCT* evStr=0);
    virtual         ~TADCactNtuMC();

    virtual Bool_t  Action();

    ClassDef(TADCactNtuMC,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    TAGparaDsc*     fpParCon;		    // DC config params.
    TAGparaDsc*     fpParGeo;        // geo params.
    EVENT_STRUCT*   fpEvtStr;
};

#endif
