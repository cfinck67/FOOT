#ifndef _TABMntuTrack_HXX
#define _TABMntuTrack_HXX
/*!
  \file
  \version $Id: TABMntuTrack.hxx,v 1.9 2003/07/07 18:42:17 mueller Exp $
  \brief   Declaration of TABMntuTrack.
*/
#define RAD2DEG 57.2957795130823208768
/*------------------------------------------+---------------------------------*/

#include "TAGroot.hxx"
#include "TAGdata.hxx"
#include "TABMntuTrackTr.hxx"

#include "TObject.h"
#include "TClonesArray.h"

//##############################################################################

class TABMntuTrack : public TAGdata {
  public:
                    TABMntuTrack();
    virtual         ~TABMntuTrack();

    TABMntuTrackTr*       Track(Int_t i);
    const TABMntuTrackTr* Track(Int_t i) const;
    
    //~ TABMntuTrackTr*       NewTrack(TABMntuTrackTr& track);//prova

    virtual void    SetupClones();
    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

//OLD TRACKING METHODS:
    //~ void Chi2Fit(TABMntuRaw *hitp, TABMntuTrackTr *p_trk, int n_ite);
    //~ Double_t ComputeChiQua(TVectorD Dy, TMatrixD VV);
    //~ void Update(TVectorD Dy, TVectorD& alpha, TVectorD Eta, TMatrixD VV, TABMntuTrackTr* ptraccia, TABMntuRaw *hitp);
    //~ TVectorD Mini(int nmeas, TMatrixD AA, TMatrixD VV, TVectorD Dy);
    //~ TMatrixD ComputeVV(TABMntuRaw *hitp, TABMntuTrackTr* p_trk);
    //~ TMatrixD ComputeAA(TABMntuRaw *hitp, TABMntuTrackTr *p_trk, TVectorD alpha);
    //~ TVectorD ComputeDy(TABMntuRaw *hitp, TABMntuTrackTr* p_trk);

    ClassDef(TABMntuTrack,2)

  public:
    Short_t         ntrk;		    // number of tracks old tracking 
    Int_t           trk_status; //-1000=notset, 0=ok, 1=firedUplane<plane_mincut, 2=firedVplane<plane_mincut, 3=hits in too different cell positions, 4=chi2red>chi2redcut
        
    TClonesArray*   t;			    // list of tracks
};

#include "TABMntuTrack.icc"

#endif
