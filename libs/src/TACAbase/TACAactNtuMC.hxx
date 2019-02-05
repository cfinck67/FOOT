#ifndef _TACAactNtuMC_HXX
#define _TACAactNtuMC_HXX
/*!
  \file
  \version $Id: TACAactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TACAactNtuMC.
*/
/*------------------------------------------+---------------------------------*/

#include "TVector3.h"

#include "Evento.h"

#include "TACA_Hit.hxx"
#include "TACA_ContainerHit.hxx"
#include "TACAparGeo.hxx"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

class TACAactNtuMC : public TAGaction {
  public:
    explicit        TACAactNtuMC(const char* name=0, TAGdataDsc* p_datraw=0, EVENT_STRUCT* evStr=0);
    virtual         ~TACAactNtuMC();

    virtual Bool_t  Action();

    TAGparaDsc* geoMap;

    ClassDef(TACAactNtuMC,0)

  private:
    TAGdataDsc*     m_hitContainer;		    // output data dsc
    EVENT_STRUCT*   m_eventStruct;
};

#endif
