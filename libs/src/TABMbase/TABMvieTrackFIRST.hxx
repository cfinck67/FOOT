#ifndef _TABMvieTrackFIRST_HXX
#define _TABMvieTrackFIRST_HXX
/*!
  \file
  \version $Id: TABMvieTrackFIRST.hxx,v 1.3 2003/06/22 11:05:58 mueller Exp $
  \brief   Declaration of TABMvieTrackFIRST.
*/
/*------------------------------------------+---------------------------------*/

#include "TPad.h"

#include "TAGview.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

class TABMvieTrackFIRST : public TAGview {
  public:
    explicit        TABMvieTrackFIRST(TAGdataDsc* p_ntutrk=0,
					  TAGdataDsc* p_nturaw=0,
					  TAGparaDsc* p_bmgeo=0);
    virtual         ~TABMvieTrackFIRST();

    virtual char*   GetObjectInfo(Int_t px, Int_t py) const;
    virtual void    ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py);

    virtual void    Paint(Option_t* option="");

    ClassDef(TABMvieTrackFIRST,0)

  private:
    TAGdataDsc*     fpNtuTrk;
    TAGdataDsc*     fpNtuRaw;
    TAGparaDsc*     fpParBMGeo;
};

#endif
