#ifndef _TATWactDatRaw_HXX
#define _TATWactDatRaw_HXX
/*!
  \file
  \version $Id: TATWactDatRaw.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
  \brief   Declaration of TATWactDatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

class WDEvent;

class TATWactDatRaw : public TAGaction {

public:

  explicit        TATWactDatRaw(const char* name=0,
				TAGdataDsc* p_datraw=0,
				TAGdataDsc* p_datdaq=0,
				TAGparaDsc* p_parmap=0);
  virtual         ~TATWactDatRaw();

  virtual Bool_t  Action();

  ClassDef(TATWactDatRaw,0)
    
  private:
    
  TAGdataDsc*     fpDatRaw;		    // output data dsc
  TAGdataDsc*     fpDatDaq;		    // input data dsc
  TAGparaDsc*     fpParMap;		    // parameter dsc
  
 private:
   Bool_t DecodeHits(const WDEvent* evt);
};

#endif
