#ifndef _TABMactNtuRaw_HXX
#define _TABMactNtuRaw_HXX
/*!
  \file
  \version $Id: TABMactNtuRaw.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TABMactNtuRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
#include "TABMdatRaw.hxx"
#include "TABMrawHit.hxx"
#include "TASTdatRaw.hxx"
#include "TASTdatRaw.hxx"
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuHit.hxx"


class TABMactNtuRaw : public TAGaction {
  public:
    explicit        TABMactNtuRaw(const char* name=0,
				 TAGdataDsc* p_nturaw=0, 
				 TAGdataDsc* p_datraw=0, 
				 TAGdataDsc* p_timraw=0, 
				 //~ TAGdataDsc* p_triraw=0, 
				 TAGparaDsc* p_geomap=0, 
				 TAGparaDsc* p_parcon=0);
    virtual         ~TABMactNtuRaw();

    virtual  void   CreateHistogram();
    virtual Bool_t  Action();

    ClassDef(TABMactNtuRaw,0)

  private:
    TAGdataDsc*     fpNtuRaw;		    // output data dsc
    TAGdataDsc*     fpDatRaw;		    // input data dsc
    TAGdataDsc*     fpTimRaw;		    // input time dsc
    //~ TAGdataDsc*     fpTriRaw;		    // input time dsc
    TAGparaDsc*     fpGeoMap;		    // geometry para dsc
    TAGparaDsc*     fpParCon;		    // BM config params.

    //histos

};

#endif
