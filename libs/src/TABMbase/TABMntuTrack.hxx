#ifndef _TABMntuTrack_HXX
#define _TABMntuTrack_HXX
/*!
  \file
  \version $Id: TABMntuTrack.hxx,v 1.9 2003/07/07 18:42:17 mueller Exp $
  \brief   Declaration of TABMntuTrack.
*/
/*------------------------------------------+---------------------------------*/

#include "TObject.h"
#include "TVectorD.h"
#include "TVector3.h"
#include "TF1.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"
#include "TABMparGeo.hxx"
#include "TABMntuRaw.hxx"

#define MAXNWIRE 36
#define MAXNWIRE_VIEW 18
#define NUMPAR 4

using namespace std;

/* lines defined in the plane by  y=mx+q*/
typedef struct{
  Double_t mm[4];
  Double_t qq[4];
} TG_STRUCT;

class TABMntuTrackTr : public TObject {
  public:
                    TABMntuTrackTr();
    virtual         ~TABMntuTrackTr();

    Int_t Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy);
    Int_t Set(TVectorD ftrackpar);
    Int_t Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy, 
	      Double_t fuz);
    Int_t SetR0(Double_t fx0, Double_t fy0);
    Int_t SetPvers(Double_t fux, Double_t fuy);
    void  GetPvers(Double_t &fux, Double_t &fuy);
    Int_t SetPvers(Double_t fux, Double_t fuy, Double_t fuz);

    void Clean();
    void Dump() const;
  void SetAssHits( int nhi) { nass = nhi; };

    Int_t AddHit(TABMntuHit *wr, TABMparGeo *f_bmgeo);
    Int_t ComputeDataWire(TABMntuHit *wr, Int_t fwire);
    Int_t EstimateTrackPar(TABMntuRaw *hitp, TABMparGeo *f_bmgeo);
    TG_STRUCT Circles2Tangents(double xc1, double yc1, double r1,
			       double xc2, double yc2, double r2);
    Int_t ComputeDataAll(TABMntuRaw *hitp);

    //Getters
    Int_t  GetNwire() {return nwire; };

    TVectorD GetTrackPar(){ return trackpar; };   //Returns a vector containing x0,y0 ux and uy  
    TVectorD GetTrackRho(){ return trackrho; };
  
    TVector3 PointAtLocalZ(double zloc);
  void Calibrate(TF1* mypol, TF1* mypol2);
    Double_t GetX0(){return x0;};
    Double_t GetY0(){return y0;};
    Double_t GetZ0(){return z0;};
    Double_t GetUx(){return ux;};
    Double_t GetUy(){return uy;};

    TVector3 GetDirection(){return Pvers;};

    double   GetChi2() { return tr_chi2; }
    void     SetChi2(double chi2) {tr_chi2 = chi2;};

    void     SetChi2H(TVectorD dy, TVectorD au, TABMntuRaw *hp);

    Short_t         nwire;	    // number of wires fired per track
    Int_t           nass;	    // Associated hits
    Float_t         x0;		    // x offset of track fit (computed @ chmb center)
    Float_t         y0;		    // y offset of track fit (computed @ chmb center)
    Float_t         z0;		    // z offset of track fit (computed @ chmb center)
    Float_t         ux;		    // x slope  of track fit
    Float_t         uy;		    // y slope  of track fit
    Float_t         uz;		    // z slope  of track fit

    TVectorD   trackpar;
    TVectorD   trackrho;

    TVector3  R0;
    TVector3  Pvers;

    double tr_chi2;

  private:

    ClassDef(TABMntuTrackTr,2)

};

//##############################################################################

class TABMntuTrack : public TAGdata {
  public:
                    TABMntuTrack();
    virtual         ~TABMntuTrack();

    TABMntuTrackTr*       Track(Int_t i);
    const TABMntuTrackTr* Track(Int_t i) const;

    virtual void    SetupClones();
    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    void Chi2Fit(TABMntuRaw *hitp, TABMntuTrackTr *p_trk, int n_ite);

    Double_t ComputeChiQua(TVectorD Dy, TMatrixD VV);
  void Update(TVectorD Dy, TVectorD& alpha, TVectorD Eta, TMatrixD VV, TABMntuTrackTr* ptraccia, TABMntuRaw *hitp);
    TVectorD Mini(int nmeas, TMatrixD AA, TMatrixD VV, TVectorD Dy);
  TMatrixD ComputeVV(TABMntuRaw *hitp, TABMntuTrackTr* p_trk);
    TMatrixD ComputeAA(TABMntuRaw *hitp, TABMntuTrackTr *p_trk, TVectorD alpha);
  TVectorD ComputeDy(TABMntuRaw *hitp, TABMntuTrackTr* p_trk);

    ClassDef(TABMntuTrack,2)

  public:
    Short_t         ntrk;		    // number of tracks
    TClonesArray*   t;			    // tracks
};

#include "TABMntuTrack.icc"

#endif
