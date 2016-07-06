#ifndef _TAIRactNtuMC_HXX
#define _TAIRactNtuMC_HXX
/*!
  \file
  \version $Id: TAIRactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TAIRactNtuMC.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.h"
#include "gsi_geo.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

class TAIRactNtuMC : public TAGaction {
  public:
    explicit        TAIRactNtuMC(const char* name=0,
				 TAGdataDsc* p_datraw=0, 
				 EVENT_STRUCT* evStr=0);
    virtual         ~TAIRactNtuMC();

    virtual Bool_t  Action();

    ClassDef(TAIRactNtuMC,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    EVENT_STRUCT*   fpEvtStr;
};

#endif
