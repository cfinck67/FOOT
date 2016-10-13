#ifndef _TATWactNtuMC_HXX
#define _TATWactNtuMC_HXX
/*!
  \file
  \version $Id: TATWactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TATWactNtuMC.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

class TATWactNtuMC : public TAGaction {
  public:
    explicit        TATWactNtuMC(const char* name=0,
				 TAGdataDsc* p_datraw=0, 
				 EVENT_STRUCT* evStr=0);
    virtual         ~TATWactNtuMC();

    virtual Bool_t  Action();

    ClassDef(TATWactNtuMC,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    EVENT_STRUCT*   fpEvtStr;
};

#endif
