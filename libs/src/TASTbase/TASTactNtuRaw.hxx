#ifndef _TASTactNtuRaw_HXX
#define _TASTactNtuRaw_HXX
/*!
  \file
  \version $Id: TASTactNtuRaw.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
  \brief   Declaration of TASTactNtuRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"
#include "TASTparGeo.hxx"
#include "TASTdatRaw.hxx"
#include "TASTntuRaw.hxx"

class TASTactNtuRaw : public TAGaction {

public:

  explicit        TASTactNtuRaw(const char* name=0,
				TAGdataDsc* p_datraw=0,
				TAGdataDsc* p_datdaq=0);
  virtual         ~TASTactNtuRaw();

  virtual Bool_t  Action();

  ClassDef(TASTactNtuRaw,0)
    
  private:
    
  TAGdataDsc*     fpDatRaw;		    // input data dsc
  TAGdataDsc*     fpNtuRaw;		    // output data dsc

  bool m_debug;
  
 private:

};

#endif
