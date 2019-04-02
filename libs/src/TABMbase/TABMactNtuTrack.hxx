#ifndef _TABMactNtuTrack_HXX
#define _TABMactNtuTrack_HXX
/*!
  \file
  \version $Id: TABMactNtuTrack.hxx,v 1.3 2003/06/29 21:52:35 mueller Exp $
  \brief   Declaration of TABMactNtuTrack.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
#include "TAGroot.hxx"
#include "TABMntuTrack.hxx"
#include "TABMntuTrackTr.hxx"
#include "TABMparCon.hxx"
#include "TABMparGeo.hxx"
#include "TAGparGeo.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuHit.hxx"
#include "TABMntuTrack.hxx" 

#include <KalmanFitterRefTrack.h>
#include <KalmanFitter.h>
#include <DAF.h>
#include "FieldManager.h"
#include "MaterialEffects.h"
#include "ConstField.h"
#include "Track.h"
#include "WireMeasurement.h"
#include "EventDisplay.h"
#include "TDatabasePDG.h"
#include "DAF.h"
#include <TDecompChol.h>

#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TGeoManager.h"
#include "TGeoMaterialInterface.h"
#include <TDatabasePDG.h>

#include "Math/Minimizer.h"
#include "TFitter.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom2.h"
#include "TError.h"

#include <math.h>
#include <iostream>

#define wire_err  0.03 //is the error on the wire position... maybe this value is not correct
#define det_Id    1 //beam monitor Id (useless parameter necessary to genfit)

using namespace genfit;

/* lines defined in the plane by  y=mx+q*/  //used for old tracking
typedef struct{
  Double_t mm[4];
  Double_t qq[4];
} TG_STRUCT;

class TABMactNtuTrack : public TAGaction {
public:
  explicit        TABMactNtuTrack(const char* name=0,
                                  TAGdataDsc* dscntutrk=0,
                                  TAGdataDsc* dscnturaw=0,
                                  TAGparaDsc* dscbmgeo=0,
                                  TAGparaDsc* dscbmcon=0,
                                  TAGparaDsc* dsctggeo=0);
  virtual         ~TABMactNtuTrack();
  virtual Bool_t  Action();
  
  virtual  void   CreateHistogram();
   
  //new tracking
  void Print_matrix(vector<vector<int>>& vec);
  bool ToBeConsider(const Int_t cell, const Int_t view, const Int_t lay);
  void SetInitPos(TVector3 &init_pos, Int_t fit_index, Double_t xwire, Double_t xrdrift, Double_t ywire, Double_t yrdrift, Double_t init_z);
  void PruneNotConvTrack(vector<vector<Int_t>> &prunedhit,vector< vector<Int_t> > &hitxtrack, Int_t index);
  void ChargePrunedTrack(vector<Int_t> &prunedhit, Int_t &firedUview, Int_t &firedVview, vector< vector<Int_t> > &hitxtrack, Int_t index);  
  void ChargeHits4GenfitTrack(vector<Int_t> &singlehittrack,Int_t &firedUview,Int_t &firedVview, vector<Double_t> &hit_res, TMatrixDSym &hitCov, TVectorD &hitCoords, Track *&fitTrack, Double_t &wire_a_x, Double_t &wire_a_y, Double_t &rdrift_a_x, Double_t &rdrift_a_y);
  void PrefitTracking(Int_t &prefit_status,Int_t &firedUview,Int_t &firedVview, vector<Int_t> singlehittrack, vector< vector<Int_t> > &hitxtrack, vector< vector<Int_t> > &hitxplane, vector<Double_t> &hit_res, TMatrixDSym &hitCov, TVectorD &hitCoords, TABMntuTrackTr &best_trackTr, Track *&fitTrack, AbsTrackRep *&rep);
  void MyGenfitFitting(vector<Int_t> &singlehittrack,Int_t &firedUview,Int_t &firedVview, vector<Double_t> &hit_res, TMatrixDSym &hitCov, TVectorD &hitCoords, Track *&fitTrack, AbsTrackRep *&rep);
  Int_t EstimateFIRSTTrackPar(vector<Int_t> &singlehittrack, Int_t &firedSingleUview, Int_t &firedSingleVview,TABMntuTrackTr *&tmp_trackTr);
  TG_STRUCT Circles2Tangents(Double_t xc1, Double_t yc1, Double_t r1, Double_t xc2, Double_t yc2, Double_t r2);
  void Chi2Fit(vector<Int_t> &singlehittrack, vector<vector<Int_t>> &prunedhit, TABMntuTrackTr *&tmp_trackTr,Bool_t converged);
  void ComputeDy(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, TVectorD &Dy);
  void ComputeVV(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, TMatrixD &VV);
  Double_t ComputeChiQua(TVectorD &Dy, TMatrixD &VV);
  void ComputeAA(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, TVectorD &alpha, TMatrixD &AA, Bool_t sign);
  void ComputeDataAll(TABMntuTrackTr *&tmp_trackTr, vector<Int_t> &singlehittrack);
  Int_t Mini(Int_t nmeas,TMatrixD &AA, TMatrixD &VV, TVectorD &Dy, TVectorD &Eta);
  void SetChi2H(TVectorD &dy, TVectorD &au, vector<Int_t> &singlehittrack);
  void Update(TABMntuTrackTr *&tmp_trackTr, vector<Int_t> &singlehittrack, TVectorD Dy, TVectorD& alpha, TVectorD &Eta, TMatrixD &VV, Int_t &worst_hit);  
  void UpdateHitsFromTrack(vector<Double_t> &best_mysqrtchi2, TABMntuTrackTr &best_trackTr, vector<Int_t> &singlehittrack);
  Int_t SortFirstDoubleHits(TABMntuTrackTr *&tmp_trackTr, vector< vector<Int_t> > &hitxplane, vector< vector<Int_t> > &hitxtrack);
  void ChargeAllTracks(vector< vector<Int_t> > &hitxtrack,vector< vector<Int_t> > &hitxplane, Int_t tracknum, Int_t firedPlane);
  void NewCircles2Tan(Double_t xc1, Double_t yc1, Double_t r1, Double_t xc2, Double_t yc2, Double_t r2);  
  
  void NewChi2MyFit(vector<Int_t> &singlehittrack, vector<vector<Int_t>> &prunedhit, TABMntuTrackTr *&tmp_trackTr,Bool_t converged);
  void NumericalMinimization(vector<Int_t> &singlehittrack, TABMntuTrackTr *&tmp_trackTr, vector<Double_t> &newpar);
  //~ Double_t EvaluateChi2(const Double_t *xx);
  
  
  
  ClassDef(TABMactNtuTrack,0)
    
  private:
  
  //~ TABMntuTrackTr tmp_trackTr;          //traccia provvisoria, così almeno non me lo cancella...
  AbsKalmanFitter*  simpleFitter;    	 //KalmanFitterRefTrack()
  AbsKalmanFitter*  refFitter;    	 //KalmanFitterRefTrack()
  AbsKalmanFitter*  dafRefFitter;    	 //DAF with kalman ref
  AbsKalmanFitter*  dafSimpleFitter;     //DAF with simple kalman
  //~ Track* 	    fitTrack;
  //~ AbsTrackRep* 	    rep;
  SharedPlanePtr  mylar1_plane; //mylar1 plane, adopted by Genfit to extrapolate track      
  SharedPlanePtr  central_plane; //central plane, adopted by Genfit to extrapolate track      
  SharedPlanePtr  mylar2_plane; //mylar2 plane, adopted by Genfit to extrapolate track     
  SharedPlanePtr  target_plane; //target plane, adopted by Genfit to extrapolate track
  
  //~ Int_t             nIter;    		 // max number of iterations (for Genfit)
  //~ Double_t 	    dPVal;      	 // convergence criterion (for Genfit)
  
  TGeoManager*      f_BMgeoMan;
  TAGdataDsc*       fpNtuTrk;		    // output data dsc
  TAGdataDsc*       fpNtuHit;		    // input data dsc
  TAGparaDsc*       fpBMGeo;		    // input data dsc
  TAGparaDsc*       fpBMCon;		    // input data dsc
  TAGparaDsc*       fpTGgeo;		    // input data dsc

  //ntu objects
  TABMntuRaw*   p_nturaw;
  TABMntuHit*   p_hit;
  //par objects
  TABMparCon* p_bmcon;
  TABMparGeo* p_bmgeo;
  TAGparGeo*  p_tggeo;
   
  TH2F*            fpHisR02d;
  TH2F*            fpResTot;

};


#endif
