#ifndef _TABMvieTrackFOOT_HXX
#define _TABMvieTrackFOOT_HXX
/*!
  \file
  \version $Id: TABMvieTrackFOOT.hxx,v 1.3 2003/06/22 11:05:58 mueller Exp $
  \brief   Declaration of TABMvieTrackFOOT.
*/
/*------------------------------------------+---------------------------------*/

#include "TPad.h"

#include "TAGview.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

class TABMvieTrackFOOT : public TAGview {
  public:
    explicit        TABMvieTrackFOOT(TAGdataDsc* p_ntutrk=0,
					  TAGdataDsc* p_nturaw=0,
					  TAGparaDsc* p_bmgeo=0);
    virtual         ~TABMvieTrackFOOT();

    virtual char*   GetObjectInfo(Int_t px, Int_t py) const;
    virtual void    ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py);

    virtual void    Paint(Option_t* option="");

    ClassDef(TABMvieTrackFOOT,0)

  private:
    TAGdataDsc*     fpNtuTrk;
    TAGdataDsc*     fpNtuRaw;
    TAGparaDsc*     fpParBMGeo;
};

#endif
