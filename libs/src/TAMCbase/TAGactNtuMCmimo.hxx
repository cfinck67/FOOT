#ifndef _TAGactNtuMCmimo_HXX
#define _TAGactNtuMCmimo_HXX
/*!
  \file
  \version $Id: TAGactNtuMCmimo.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TAGactNtuMCmimo.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.hxx"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

class TAGactNtuMCmimo : public TAGaction {
  public:
    explicit        TAGactNtuMCmimo(const char* name=0,
				   TAGdataDsc* p_nturaw=0, 
				   EVENT_STRUCT* evStr=0);
    virtual         ~TAGactNtuMCmimo();

    virtual Bool_t  Action();

    ClassDef(TAGactNtuMCmimo,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    EVENT_STRUCT*   fpEvtStr;
};

#endif
