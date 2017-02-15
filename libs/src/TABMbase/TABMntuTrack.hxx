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

#include "Track.h"

#define MAXNWIRE 36
#define MAXNWIRE_VIEW 18
#define NUMPAR 4

using namespace std;

/* lines defined in the plane by  y=mx+q*/  //used for old tracking
typedef struct{
  Double_t mm[4];
  Double_t qq[4];
} TG_STRUCT;

class TABMntuTrackTr : public TObject {
  public:
                    TABMntuTrackTr();
    virtual         ~TABMntuTrackTr();

    //*********OLD TRACKING: DA CANCELLARE?? se lo faccio devo modificare un sacco di altre librerie (TAVT, etc.) collegate..******
    Int_t Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy);
    Int_t Set(TVectorD ftrackpar);
    Int_t Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy, Double_t fuz);
    Int_t SetR0(Double_t fx0, Double_t fy0);
    Int_t SetPvers(Double_t fux, Double_t fuy);
    void  GetPvers(Double_t &fux, Double_t &fuy);
    Int_t SetPvers(Double_t fux, Double_t fuy, Double_t fuz);
    void SetAssHits( int nhi) { nass = nhi; };
    Int_t AddHit(TABMntuHit *wr, TABMparGeo *f_bmgeo);
    Int_t ComputeDataWire(TABMntuHit *wr, Int_t fwire);
    Int_t EstimateTrackPar(TABMntuRaw *hitp, TABMparGeo *f_bmgeo);
    TG_STRUCT Circles2Tangents(double xc1, double yc1, double r1,
			       double xc2, double yc2, double r2);
    Int_t ComputeDataAll(TABMntuRaw *hitp);
    Int_t  GetNwire() {return nwire; };
    TVectorD GetTrackPar(){ return trackpar; };   //Returns a vector containing x0,y0 ux and uy  
    TVectorD GetTrackRho(){ return trackrho; };
    TVector3 PointAtLocalZ(double zloc);
    Double_t GetX0(){return x0;};
    Double_t GetY0(){return y0;};
    Double_t GetZ0(){return z0;};
    Double_t GetUx(){return ux;};
    Double_t GetUy(){return uy;};
    double   GetChi2() { return tr_chi2; }
    void     SetChi2(double chi2) {tr_chi2 = chi2;};    
    TVector3 GetDirection(){return Pvers;};
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

    //*************************************In common or to be modified for new tracking*************************
    void Calibrate(TF1* mypol, TF1* mypol2); //DA CAPIRE COME MODIFICARE!!!!!
    void Clean(); //da modificare ogni volta che aggiungo variabile etc., come costruttore default, SetNew etc.
    void Dump() const; //idem come sopra

  //******************************************NEW TRACKING:*********************************************

    //~ Int_t SetNew(int nhi, double chi);

    //Setters
    void SetNhit (Int_t nhi) { nhit = nhi;};
    void SetChi2New(Double_t chi) {chi2 = chi;};
    void SetChi2NewRed(Double_t chi2_re) {chi2Red = chi2_re;};
    void SetNdf(Double_t nd) {ndf = nd;};
    void SetFailedPoint(Int_t fail) {failedPoint = fail;};
    
    //Getters
    Int_t  GetNhit() {return nhit;};
    Double_t GetChi2New() {return chi2;};
    Double_t GetChi2NewRed() {return chi2Red;};
    Double_t GetNdf() {return ndf;};
    Int_t GetFailedPoint() {return failedPoint;};
    Int_t GetCheck(){return check;};
    Double_t GetResiduo(){return residuo;};
    
    void MCcheckTr(Double_t chi2cut, bool onlyPrimary, bool converged, TVector3 priexit, Double_t mylar2_cut, genfit::Track fitTrack, Int_t& BMdebug, Double_t maxError);
    bool MCcheckLastPoint(genfit::Track fitTrack, TVector3 realpos, Double_t maxError, Int_t BMdebug);
    void MCcheckFewHit(TVector3 priexit, Int_t BMdebug);
    
    //parameters
    Int_t         nhit;	          //number of associated hits (different from nwire because of Double_t hits in the same cell)
    Double_t 		  chi2;           //new tracking chi2
    Double_t      chi2Red;        //chi2/ndf
    Double_t      ndf;            //degree of freedom (I don't know why Genfit uses double instead of int)
    Int_t         failedPoint;    //number of point rejected by the fit (only for DAF fitters)
    
    //MC parameters:
    Double_t    residuo;          //temporary: difference between fitted position and "real" position (from MC) of the last hit 
    Int_t       check;            //look in TABMntuTrack::checkTr
    
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


//OLD TRACKING METHODS:
    void Chi2Fit(TABMntuRaw *hitp, TABMntuTrackTr *p_trk, int n_ite);
    Double_t ComputeChiQua(TVectorD Dy, TMatrixD VV);
    void Update(TVectorD Dy, TVectorD& alpha, TVectorD Eta, TMatrixD VV, TABMntuTrackTr* ptraccia, TABMntuRaw *hitp);
    TVectorD Mini(int nmeas, TMatrixD AA, TMatrixD VV, TVectorD Dy);
    TMatrixD ComputeVV(TABMntuRaw *hitp, TABMntuTrackTr* p_trk);
    TMatrixD ComputeAA(TABMntuRaw *hitp, TABMntuTrackTr *p_trk, TVectorD alpha);
    TVectorD ComputeDy(TABMntuRaw *hitp, TABMntuTrackTr* p_trk);

    ClassDef(TABMntuTrack,2)

  public:
    Short_t         ntrk;		    // number of tracks old tracking 
    Int_t           ntrack;       // number of tracks new tracking
    
    
    TClonesArray*   t;			    // tracks
};

#include "TABMntuTrack.icc"

#endif
