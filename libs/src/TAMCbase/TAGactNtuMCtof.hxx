#ifndef _TAGactNtuMCtof_HXX
#define _TAGactNtuMCtof_HXX
/*!
  \file
  \version $Id: TAGactNtuMCtof.hxx,v 
  \brief   Declaration of TAGactNtuMCtof.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.hxx"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

class TAGactNtuMCtof : public TAGaction {
  public:
    explicit        TAGactNtuMCtof(const char* name=0,
				   TAGdataDsc* p_nturaw=0, 
				   EVENT_STRUCT* evStr=0);
    virtual         ~TAGactNtuMCtof();

    virtual Bool_t  Action();

    ClassDef(TAGactNtuMCtof,0)

  private:
    TAGdataDsc*     fpNtuMC;		    // output data dsc
    EVENT_STRUCT*   fpEvtStr;
};

#endif
