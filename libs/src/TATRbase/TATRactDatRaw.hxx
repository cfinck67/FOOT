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

class TATRactDatRaw : public TAGaction {
  public:

    explicit        TATRactDatRaw(const char* name=0,
				 TAGdataDsc* p_datraw=0,
				 TAGdataDsc* p_datmbs=0,
				 TAGparaDsc* p_parmap=0);
    virtual         ~TATRactDatRaw();

    virtual Bool_t  Action();

    ClassDef(TATRactDatRaw,0)

  private:
    TAGdataDsc*     fpDatRaw;		    // output data dsc
    TAGdataDsc*     fpDatMbs;		    // input data dsc
    TAGparaDsc*     fpParMap;		    // parameter dsc
  
};

#endif
