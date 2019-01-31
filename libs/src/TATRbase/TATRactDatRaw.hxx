#ifndef _TATRactDatRaw_HXX
#define _TATRactDatRaw_HXX
/*!
 \file
 \version $Id: TATRactDatRaw.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
 \brief   Declaration of TATRactDatRaw.
 */
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"

#include "TDCEvent.hh"

class TATRactDatRaw : public TAGaction {
public:
   
   explicit        TATRactDatRaw(const char* name=0,
                                 TAGdataDsc* p_datraw=0,
                                 TAGdataDsc* p_datqevt=0,
                                 TAGparaDsc* p_parmap=0);
   virtual        ~TATRactDatRaw();
   
   virtual         Bool_t  Action();
   
private:
   void            DecodeHits(const TDCEvent* evt);
   
private:
   TAGdataDsc*     fpDatRaw;		    // output data dsc
   TAGdataDsc*     fpDaqEvent;		    // input data dsc
   TAGparaDsc*     fpParMap;		    // parameter dsc
   
   ClassDef(TATRactDatRaw,0)
};

#endif
