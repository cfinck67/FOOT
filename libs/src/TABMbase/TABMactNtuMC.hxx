#ifndef _TABMactNtuMC_HXX
#define _TABMactNtuMC_HXX
/*!
  \file
  \version $Id: TABMactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TABMactNtuMC.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.h"
#include "foot_geo.h"

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

class TABMactNtuMC : public TAGaction {
  public:
    explicit        TABMactNtuMC(const char* name=0,
				 TAGdataDsc* p_nturaw=0, 
				 TAGparaDsc* p_parcon=0, 
				 EVENT_STRUCT* evStr=0);
    virtual         ~TABMactNtuMC();

    virtual Bool_t  Action();

    ClassDef(TABMactNtuMC,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    TAGparaDsc*     fpParCon;		    // BM config params.
    EVENT_STRUCT*   fpEvtStr;
};

#endif
