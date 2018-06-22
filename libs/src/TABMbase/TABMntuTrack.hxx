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
#include "TAGgeoTrafo.hxx"
#include "TAGdata.hxx"
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuHit.hxx"

#include "TObject.h"
#include "TVectorD.h"
#include "TVector3.h"
#include "TF1.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TMath.h"

#include "Track.h"
#include "DetPlane.h"
#include "StateOnPlane.h"
#include "SharedPlanePtr.h"
#include "KalmanFitterInfo.h"
#include <TDecompChol.h>

#include "math.h"

//~ #define MAXNWIRE 36
//~ #define MAXNWIRE_VIEW 18
//~ #define NUMPAR 4

using namespace std;
using namespace genfit;

/* lines defined in the plane by  y=mx+q*/  //used for old tracking
//~ typedef struct{
  //~ Double_t mm[4];
  //~ Double_t qq[4];
//~ } TG_STRUCT;

class TABMntuTrackTr : public TObject {
  public:
                    TABMntuTrackTr();
    virtual         ~TABMntuTrackTr();

    //*********OLD TRACKING: DA CANCELLARE?? se lo faccio devo modificare un sacco di altre librerie (TAVT, etc.) collegate..******
    //~ Int_t Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy);
    //~ Int_t Set(TVectorD ftrackpar);
    //~ Int_t Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy, Double_t fuz);
    //~ Int_t SetR0(Double_t fx0, Double_t fy0);
    //~ Int_t SetPvers(Double_t fux, Double_t fuy);
    //~ void  GetPvers(Double_t &fux, Double_t &fuy);
    //~ Int_t SetPvers(Double_t fux, Double_t fuy, Double_t fuz);
    //~ void SetAssHits( int nhi) { nass = nhi; };
    //~ Int_t AddHit(TABMntuHit *wr, TABMparGeo *f_bmgeo);
    //~ Int_t ComputeDataWire(TABMntuHit *wr, Int_t fwire);
    //~ Int_t EstimateTrackPar(TABMntuRaw *hitp, TABMparGeo *f_bmgeo);
    //~ TG_STRUCT Circles2Tangents(double xc1, double yc1, double r1,
			       //~ double xc2, double yc2, double r2);
    //~ Int_t ComputeDataAll(TABMntuRaw *hitp);
    //~ Int_t  GetNwire() {return nwire; };
    //~ TVectorD GetTrackPar(){ return trackpar; };   //Returns a vector containing x0,y0 ux and uy  
    //~ TVectorD GetTrackRho(){ return trackrho; };
    //~ TVector3 PointAtLocalZ(double zloc);
    //~ Double_t GetX0(){return x0;};
    //~ Double_t GetY0(){return y0;};
    //~ Double_t GetZ0(){return z0;};
    //~ Double_t GetUx(){return ux;};
    //~ Double_t GetUy(){return uy;};
    //~ double   GetChi2() { return tr_chi2; }
    //~ void     SetChi2(double chi2) {tr_chi2 = chi2;};    
    //~ TVector3 GetDirection(){return Pvers;};
    //~ void     SetChi2H(TVectorD dy, TVectorD au, TABMntuRaw *hp);
    //~ Short_t         nwire;	    // number of wires fired per track
    //~ Int_t           nass;	    // Associated hits
    //~ Float_t         x0;		    // x offset of track fit (computed @ chmb center)
    //~ Float_t         y0;		    // y offset of track fit (computed @ chmb center)
    //~ Float_t         z0;		    // z offset of track fit (computed @ chmb center)
    //~ Float_t         ux;		    // x slope  of track fit
    //~ Float_t         uy;		    // y slope  of track fit
    //~ Float_t         uz;		    // z slope  of track fit
    //~ TVectorD   trackpar;
    //~ TVectorD   trackrho;
    //~ TVector3  R0;
    //~ TVector3  Pvers;
    //~ double tr_chi2;

    //*************************************In common or to be modified for new tracking*************************
    void Calibrate(TF1* mypol, TF1* mypol2); //DA CAPIRE COME MODIFICARE!!!!!
    void Clean(); //da modificare ogni volta che aggiungo variabile etc., come costruttore default, SetNew etc.
    void Dump() const; //idem come sopra
    TABMntuTrackTr(const TABMntuTrackTr &tr_in);//copy constructor definito da me

  //******************************************NEW TRACKING:*********************************************

    //~ Int_t SetNew(int nhi, double chi);

    //Setters
    void SetNhit (Int_t nhi) { nhit = nhi;};
    void SetChi2New(Double_t chi) {chi2 = chi;};
    void SetChi2NewRed(Double_t chi2_re) {chi2Red = chi2_re;};
    void SetNdf(Double_t nd) {ndf = nd;};
    void SetFailedPoint(Int_t fail) {failedPoint = fail;};
    void SetIsConverged(Int_t conv) {isConverged = conv;};
    
    //Getters
    Int_t  GetNhit() {return nhit;};
    Double_t GetChi2New() {return chi2;};
    Double_t GetChi2NewRed() {return chi2Red;};
    Double_t GetMyChi2() {return mychi2;};
    Double_t GetMyChi2Red() {return mychi2Red;};
    Double_t GetNdf() {return ndf;};
    Int_t GetFailedPoint() {return failedPoint;};
    Int_t GetIsConverged(){return isConverged;};
    Double_t GetAngZ(){return AngZ;};
    Double_t GetAngZRes(){return AngZRes;};
    Double_t GetAngZResAv(){return AngZResAv;};
    Double_t GetAngPhi(){return AngPhi;};
    Double_t GetAngPhiRes(){return AngPhiRes;};
    TVector3 GetMylar1Pos(){return mylar1_pos;};
    TVector3 GetMylar2Pos(){return mylar2_pos;};
    TVector3 GetTargetPos(){return target_pos;};
    
    void CalculateFitPar(Track* fitTrack, vector<Double_t>& hit_res, vector<Double_t>& hit_mychi2red, vector<Int_t> &prunedhit, TABMparCon* p_bmcon, TABMparGeo* p_bmgeo);
    Double_t FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers);    
    
    //parameters
    Int_t         nhit;	          //number of associated hits (different from nwire because of Double_t hits in the same cell)
    Double_t      ndf;            //degree of freedom (I don't know why Genfit uses double instead of int)
    Int_t         failedPoint;    //number of point rejected by the fit (only for DAF fitters)
    Double_t 		  chi2;           //new tracking chi2 from Genfit
    Double_t      chi2Red;        // chi2/ndf (from Genfit)
    Double_t      mychi2;         //chi2 calculated from difference between measured rdrift and fitted rdrift (set by CalculateMyChi2)
    Double_t      mychi2Red;      //mychi2 reduced
    Int_t         isConverged;    
    Double_t      MaxRdriftErr;   //highest difference between fitted rdrift and rdrift from data
    Double_t      AngZ;           //angle of fitted track respect to z (calculated from average of all fitted state momentum)
    Double_t      AngZRes;        //resolution of AngZ (calculated as AngZmax-AngZmin)
    Double_t      AngZResAv;      //resolution of AngZ (calculated as average of AngZ_i-AngZ for all fitted state momentum i)
    Double_t      AngPhi;         //angle of fitted track on xy plane (calculated from average of all fitted state momentum)
    Double_t      AngPhiRes;      //resolution of AngZ (calculated as AngPhimax-AngPhimin)
    TVector3      target_pos;     //position of the track extrapolate to the target plane
    TVector3      mylar1_pos;     //position of the track extrapolate to the first mylar plane
    TVector3      mylar2_pos;     //position of the track extrapolate to the second mylar plane
    //~ Int_t         trktr_status;   //0=ok, 1=to be refitted pruning hits with mychi2Red over cut level 
    
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
    Int_t           trk_status; //-1000=notset, 0=ok, 1=firedUplane<plane_mincut, 2=firedVplane<plane_mincut, 3=hits in too different cell positions
        
    TClonesArray*   t;			    // list of tracks
};

#include "TABMntuTrack.icc"

#endif
