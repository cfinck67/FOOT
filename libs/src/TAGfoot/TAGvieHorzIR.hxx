#ifndef _TAGvieHorzIR_HXX
#define _TAGvieHorzIR_HXX
/*!
  \file
  \version $Id: TAGvieHorzIR.hxx,v 1.3 2003/06/22 11:05:58 mueller Exp $
  \brief   Declaration of TAGvieHorzIR.
*/
/*------------------------------------------+---------------------------------*/

#include "TPad.h"
#include "TVector3.h"

#include "TAGview.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

class TAGvieHorzIR : public TAGview {
  public:
    explicit        TAGvieHorzIR(TAGdataDsc* p_ntutrk=0,
				 TAGdataDsc* p_nturaw=0,
				 TAGparaDsc* p_bmgeo=0,
				 TAGdataDsc* p_ntuvtcl=0,
				 TAGparaDsc* p_vtgeo=0,
				 TAGdataDsc* p_vttrk=0,
				 bool zoom=false);

    virtual         ~TAGvieHorzIR();

    virtual char*   GetObjectInfo(Int_t px, Int_t py) const;
    virtual void    ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py);

    virtual void    Paint(Option_t* option="");

    ClassDef(TAGvieHorzIR,0)

  private:
    TAGdataDsc*     fpNtuTrk;
    TAGdataDsc*     fpNtuBMRaw;
    TAGparaDsc*     fpParBMGeo;
    TAGdataDsc*     fpNtuClust;
    TAGparaDsc*     fpParVTGeo;
    TAGdataDsc*     fpNtuTrack;
  bool m_zoom;
};

#endif
