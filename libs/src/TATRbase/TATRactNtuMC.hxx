#ifndef _TATRactNtuMC_HXX
#define _TATRactNtuMC_HXX
/*!
  \file
  \version $Id: TATRactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TATRactNtuMC.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.h"
#include "gsi_geo.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

class TATRactNtuMC : public TAGaction {
  public:
    explicit        TATRactNtuMC(const char* name=0,
				 TAGdataDsc* p_datraw=0, 
				 EVENT_STRUCT* evStr=0);
    virtual         ~TATRactNtuMC();

    virtual Bool_t  Action();

    ClassDef(TATRactNtuMC,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    EVENT_STRUCT*   fpEvtStr;
};

#endif
