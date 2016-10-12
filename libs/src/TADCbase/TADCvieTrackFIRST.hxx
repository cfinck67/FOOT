#ifndef _TADCvieTrackFIRST_HXX
#define _TADCvieTrackFIRST_HXX
/*!
  \file
  \version $Id: adapted from TABMvieTrackFIRST.hxx,v 1.3 2003/06/22 11:05:58 mueller Exp $
  \brief   Declaration of TADCvieTrackFIRST.
*/
/*------------------------------------------+---------------------------------*/

#include "TPad.h"

#include "TAGview.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

class TADCvieTrackFIRST : public TAGview {
  public:
    explicit        TADCvieTrackFIRST(TAGdataDsc* p_ntutrk=0,
					  TAGdataDsc* p_nturaw=0,
					  TAGparaDsc* p_dcgeo=0);
    virtual         ~TADCvieTrackFIRST();

    virtual char*   GetObjectInfo(Int_t px, Int_t py) const;
    virtual void    ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py);

    virtual void    Paint(Option_t* option="");

    ClassDef(TADCvieTrackFIRST,0)

  private:
    TAGdataDsc*     fpNtuTrk;
    TAGdataDsc*     fpNtuRaw;
    TAGparaDsc*     fpParDCGeo;
};

#endif
