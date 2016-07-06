#ifndef _TAGvieHorzFIRST_HXX
#define _TAGvieHorzFIRST_HXX
/*!
  \file
  \version $Id: TAGvieHorzFIRST.hxx,v 1.3 2003/06/22 11:05:58 mueller Exp $
  \brief   Declaration of TAGvieHorzFIRST.
*/
/*------------------------------------------+---------------------------------*/

#include "TPad.h"
#include "TVector3.h"

#include "TAGview.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

class TAGvieHorzFIRST : public TAGview {
  public:
    explicit        TAGvieHorzFIRST(TAGdataDsc* p_ntutrk=0,
				    TAGdataDsc* p_nturaw=0,
				    TAGparaDsc* p_bmgeo=0,
				    TAGdataDsc* p_ntutraw=0,
				    TAGparaDsc* p_tofgeo=0);

    virtual         ~TAGvieHorzFIRST();

    virtual char*   GetObjectInfo(Int_t px, Int_t py) const;
    virtual void    ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py);

    virtual void    Paint(Option_t* option="");

    ClassDef(TAGvieHorzFIRST,0)

  private:
    TAGdataDsc*     fpNtuTrk;
    TAGdataDsc*     fpNtuBMRaw;
    TAGparaDsc*     fpParBMGeo;
    TAGdataDsc*     fpNtuTofRaw;
    TAGparaDsc*     fpParTofGeo;
};

#endif
