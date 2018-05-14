#ifndef _TAGactNtuMCeve_HXX
#define _TAGactNtuMCeve_HXX
/*!
  \file
  \version $Id: TAGactNtuMCeve.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TAGactNtuMCeve.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

#include "GlobalPar.hxx"

class TAGactNtuMCeve : public TAGaction {
  public:
    explicit        TAGactNtuMCeve(const char* name=0,
				   TAGdataDsc* p_nturaw=0, 
				   EVENT_STRUCT* evStr=0);
    virtual         ~TAGactNtuMCeve();

    virtual Bool_t  Action();

    ClassDef(TAGactNtuMCeve,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    EVENT_STRUCT*   fpEvtStr;
};

#endif
