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
#include "foot_geo.h"
#include "TAGroot.hxx"
#include "TABMntuTrack.hxx"
#include "TABMntuTrackTr.hxx"
#include "TABMparCon.hxx"
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
#include "TGeoManager.h"
#include "TGeoMaterialInterface.h"
#include <TDatabasePDG.h>

#include <math.h>
#include <iostream>


using namespace genfit;

class TABMactNtuTrack : public TAGaction {
public:
  explicit        TABMactNtuTrack(const char* name=0,
				  TAGdataDsc* p_ntutrk=0,
				  TAGdataDsc* p_ntuhit=0,
				  TAGparaDsc* bmgeo=0,
				  TAGparaDsc* bmcon=0);
  virtual         ~TABMactNtuTrack();
  virtual Bool_t  Action();
  
  //old tracking
  //~ void sortDoubleHits(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, TABMntuTrackTr & aRTr, TABMntuTrackTr & RefTr, int add[36], int ndou);
  //~ void refitImprovedTrackSide(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, int mulT, int mulS, double trchi2, TABMntuTrackTr & aRTr, int j);
  //~ void refitImprovedTrackTop(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, int mulT, int mulS, double trchi2, TABMntuTrackTr & aRTr, int j);

  //new tracking
  void Print_matrix(vector<vector<int>>& vec);
  bool ToBeConsider(const Int_t cell, const Int_t view, const Int_t lay);
  void SetInitPos(TVector3 &init_pos, Int_t &fit_index, Double_t &xwire, Double_t &xrdrift, Double_t &ywire, Double_t &yrdrift, Double_t init_z);
  //~ Bool_t PlaneCounter(vector<Int_t> &hitxtrack_vec, TABMparCon *p_bmcon);
  Bool_t Refit(vector<Double_t> &hit_mychi2, vector< vector<Int_t> > &hitxtrack, TABMparCon* p_bmcon);//return true if it add another possible track to hitxtrack
  void RejectSlopedTrack(vector< vector<Int_t> > &hitxtrack, vector<Bool_t>&possiblePrimary, TABMntuHit* p_hit, TABMntuRaw* p_ntuhit, Int_t &trk_index);
  void PruneNotConvTrack(vector<vector<Int_t>> &prunedhit,vector< vector<Int_t> > &hitxtrack, Int_t index);
  void ChargePrunedTrack(vector<Int_t> &prunedhit, Int_t prunedUview, Int_t prunedVview, vector< vector<Int_t> > &hitxtrack, vector<Bool_t> &possiblePrimary, TABMparCon* p_bmcon, TABMntuRaw* p_ntuhit, TABMntuHit* p_hit, Int_t index);  
  
  ClassDef(TABMactNtuTrack,0)
    
  private:
  
  //~ TABMntuTrackTr tmp_trackTr;          //traccia provvisoria, così almeno non me lo cancella...
  //~ AbsKalmanFitter*  simpleFitter;    	 //KalmanFitterRefTrack()
  AbsKalmanFitter*  refFitter;    	 //KalmanFitterRefTrack()
  AbsKalmanFitter*  dafRefFitter;    	 //DAF with kalman ref
  AbsKalmanFitter*  dafSimpleFitter;     //DAF with simple kalman
  //~ Track* 	    fitTrack;
  //~ AbsTrackRep* 	    rep;
  SharedPlanePtr  mylar1_plane; //mylar1 plane, adopted by Genfit to extrapolate track      
  SharedPlanePtr  mylar2_plane; //mylar2 plane, adopted by Genfit to extrapolate track     
  SharedPlanePtr  target_plane; //target plane, adopted by Genfit to extrapolate track
  
  //~ Int_t             nIter;    		 // max number of iterations (for Genfit)
  //~ Double_t 	    dPVal;      	 // convergence criterion (for Genfit)
  
  TGeoManager*      f_BMgeoMan;
  TAGdataDsc*       fpNtuTrk;		    // output data dsc
  TAGdataDsc*       fpNtuHit;		    // input data dsc
  TAGparaDsc*       fpBMGeo;		    // input data dsc
  TAGparaDsc*       fpBMCon;		    // input data dsc
  
  //par objects
  TABMparCon* p_bmcon;
  TABMparGeo* p_bmgeo;
};

#endif
