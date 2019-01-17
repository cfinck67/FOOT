#ifndef _TABMntuTrackTr_HXX
#define _TABMntuTrackTr_HXX

#define RAD2DEG 57.2957795130823208768
/*------------------------------------------+---------------------------------*/

#include "TAGgeoTrafo.hxx"
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuHit.hxx"

#include "TString.h"
#include "TMath.h"
#include "TVectorD.h"
#include "TVector3.h"
#include "TF1.h"

#include "Track.h"
#include "DetPlane.h"
#include "StateOnPlane.h"
#include "SharedPlanePtr.h"
#include "KalmanFitterInfo.h"
#include <TDecompChol.h>
//~ #include <DetPlane.h>

#include "math.h"

//~ #define MAXNWIRE 36
//~ #define MAXNWIRE_VIEW 18
#define BM_trackpar 4

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

    //*********OLD TRACKING:******
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
    void Calibrate(TF1* mypol, TF1* mypol2); 
    void Clean(); 
    void Dump() const; 
    TABMntuTrackTr(const TABMntuTrackTr &tr_in);

  //******************************************NEW TRACKING:*********************************************

    //~ Int_t SetNew(int nhi, double chi);
    //~ TABMntuTrackTr& operator=(TABMntuTrackTr const& in);
    TABMntuTrackTr& operator=(TABMntuTrackTr const& in){
      if(this!=&in){
        this->nhit=in.nhit;
        this->ndf=in.ndf;
        this->failedPoint=in.failedPoint;
        this->chi2=in.chi2;
        this->chi2Red=in.chi2Red;
        this->mychi2=in.mychi2;
        this->mychi2Red=in.mychi2Red;
        this->isConverged=in.isConverged;
        this->MaxRdriftErr=in.MaxRdriftErr;
        this->AngZ=in.AngZ;
        this->AngZRes=in.AngZRes;
        this->AngZResAv=in.AngZResAv;
        this->AngPhi=in.AngPhi;
        this->AngPhiRes=in.AngPhiRes;
        this->target_pos=in.target_pos;
        this->mylar1_pos=in.mylar1_pos;
        this->mylar2_pos=in.mylar2_pos;
        this->prefit_status=in.prefit_status;
        this->R0=in.R0;
        this->Pvers=in.Pvers;
        this->nite=in.nite;
      }
      return *this;
    }


    //Setters
    void SetNhit (Int_t nhi) { nhit = nhi;};
    void SetChi2New(Double_t chi) {chi2 = chi;};
    void SetChi2NewRed(Double_t chi2_re) {chi2Red = chi2_re;};
    void SetNdf(Double_t nd) {ndf = nd;};
    void SetFailedPoint(Int_t fail) {failedPoint = fail;};
    void SetIsConverged(Int_t conv) {isConverged = conv;};
    void SetPrefitStatus(Int_t status){prefit_status=status;};
    void SetR0(Double_t x,Double_t y, Double_t z){R0.SetXYZ(x,y,z);};
    void SetPvers(Double_t x,Double_t y, Double_t z){Pvers.SetXYZ(x,y,z);};
    void SetPvers(TVector3 pin){Pvers=pin;};
    void SetMyChi2Red(Double_t chi2red_in){mychi2Red=chi2red_in;};
    void NewSet(TVectorD ftrackpar);//set Pvers and R0, used for the FIRST tracking
    void SetNite(Int_t intein){nite=intein;};
    
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
    Int_t GetPrefitStatus(){return prefit_status;};
    TVector3 GetPvers(){return Pvers;};
    TVector3 GetR0(){return R0;};
    Int_t GetNite(){return nite;};
    //~ Double_t GetTrackPar(Int_t index){return trackpar[index];};
    //~ TVectorD GetTrackPar(){return trackpar;};
    
    //~ void SetSharedPlanes(TABMparGeo* p_bmgeo);
    void CalculateFitPar(Track* fitTrack, vector<Double_t>& hit_res, vector<Double_t>& hit_mysqrtchi2, vector<vector<Int_t>> &prunedhit, TABMparCon* p_bmcon, TABMparGeo* p_bmgeo, Int_t rejhit, SharedPlanePtr &mylar1_plane,SharedPlanePtr &central_plane, SharedPlanePtr &mylar2_plane, SharedPlanePtr &target_plane);////evaluate the projections etc. from genfit tracking parameters
    void CalculateFromFirstPar(TABMparCon* p_bmcon, TABMparGeo* p_bmgeo);//evaluate the projections etc. from FIRST tracking parameters
    Double_t FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers);    
    void PrintR0Pvers();
   
   TVector3 PointAtLocalZ(double zloc);

private:  
    //parameters
    Int_t         nhit;	          //number of associated hits (different from nwire because of hits in the same cell)
    Double_t      ndf;            //degree of freedom (I don't know why Genfit uses double instead of int)
    Int_t         failedPoint;    //number of point rejected by the fit (only for DAF fitters)
    Double_t 		  chi2;           //new tracking chi2 from Genfit
    Double_t      chi2Red;        // chi2/ndf (from Genfit)
    Double_t      mychi2;         //chi2 calculated from difference between measured rdrift and fitted rdrift (set by CalculateMyChi2)
    Double_t      mychi2Red;      //mychi2 reduced
    Int_t         isConverged;    //Genfit fit converged flag: 0=not set, 1=converged, 2=not converged
    Double_t      MaxRdriftErr;   //highest difference between fitted rdrift and rdrift from data
    Double_t      AngZ;           //angle of fitted track respect to z (calculated from average of all fitted state momentum)
    Double_t      AngZRes;        //resolution of AngZ (calculated as AngZmax-AngZmin)
    Double_t      AngZResAv;      //resolution of AngZ (calculated as average of AngZ_i-AngZ for all fitted state momentum i)
    Double_t      AngPhi;         //angle of fitted track on xy plane (calculated from average of all fitted state momentum)
    Double_t      AngPhiRes;      //resolution of AngZ (calculated as AngPhimax-AngPhimin)
    TVector3      target_pos;     //position of the track extrapolate to the target plane
    TVector3      mylar1_pos;     //position of the track extrapolate to the first mylar plane
    TVector3      mylar2_pos;     //position of the track extrapolate to the second mylar plane
    Int_t         prefit_status;  //status of the prefit: -5=no prefit; -1=prefit not converged; 0=prefit performed no need to refit; 1=prefit performed, there are hits added, need to refit; -10= prefit without fitterinfo
    //~ TVectorD      trackpar;    //track parameter: 0=m, 1=q for U view (yz plane); 2=m, 3=q for V view (xz plane) IN THE DETECTOR SYSTEM OF REFERENCE
    TVector3      Pvers;           //direction of the track from mylar1_pos to mylar2_pos
    TVector3      R0;              //position of the track on the xy plane at z=0
    Int_t         nite;            //number of iteration with the leastchi2 method, +=normal, -=reversed
    

    ClassDef(TABMntuTrackTr,2)

};


#endif



