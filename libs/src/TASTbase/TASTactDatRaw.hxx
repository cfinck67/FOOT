#ifndef _TASTactDatRaw_HXX
#define _TASTactDatRaw_HXX
/*!
  \file
  \version $Id: TASTactDatRaw.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
  \brief   Declaration of TASTactDatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

class TASTactDatRaw : public TAGaction {

public:

  explicit        TASTactDatRaw(const char* name=0,
				TAGdataDsc* p_datraw=0,
				TAGdataDsc* p_datdaq=0,
				TAGparaDsc* p_parmap=0);
  virtual         ~TASTactDatRaw();

  virtual Bool_t  Action();

  ClassDef(TASTactDatRaw,0)
    
  private:
    
  TAGdataDsc*     fpDatRaw;		    // output data dsc
  TAGdataDsc*     fpDatDaq;		    // input data dsc
  TAGparaDsc*     fpParMap;		    // parameter dsc
  
};

#endif
