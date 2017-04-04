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
#include "TABMntuTrack.hxx"

#include "TGeoManager.h"
#include <KalmanFitterRefTrack.h>
#include <KalmanFitter.h>
#include <DAF.h>

using namespace genfit;

class TABMactNtuTrack : public TAGaction {
public:
  explicit        TABMactNtuTrack(const char* name=0,
				  TAGdataDsc* p_ntutrk=0,
				  TAGdataDsc* p_ntuhit=0,
				  TAGparaDsc* p_bmgeo=0,
				  TAGparaDsc* p_bmcon=0);
  virtual         ~TABMactNtuTrack();
  virtual Bool_t  Action();
  
  //old tracking
  void sortDoubleHits(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, TABMntuTrackTr & aRTr, TABMntuTrackTr & RefTr, int add[36], int ndou);
  void refitImprovedTrackSide(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, int mulT, int mulS, double trchi2, TABMntuTrackTr & aRTr, int j);
  void refitImprovedTrackTop(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, int mulT, int mulS, double trchi2, TABMntuTrackTr & aRTr, int j);

  //new tracking
  void Print_matrix(vector<vector<int>>& vec);
  bool ToBeConsider(const Int_t cell, const Int_t view, const Int_t lay);
  void SetInitPos(TVector3 &init_pos, Int_t fit_index, Double_t xwire, Double_t xrdrift, Double_t ywire, Double_t yrdrift, Double_t init_z);
  ClassDef(TABMactNtuTrack,0)
    
  private:

  AbsKalmanFitter*  simpleFitter;    	 //KalmanFitterRefTrack()
  AbsKalmanFitter*  refFitter;    	 //KalmanFitterRefTrack()
  AbsKalmanFitter*  dafRefFitter;    	 //DAF with kalman ref
  AbsKalmanFitter*  dafSimpleFitter;    	 //DAF with simple kalman
  
  Int_t 				  nIter;      		 // max number of iterations (for Genfit)
  Double_t 			  dPVal; 			 // convergence criterion (for Genfit)
  
  TGeoManager*      f_BMgeoMan;
  TAGdataDsc*       fpNtuTrk;		    // output data dsc
  TAGdataDsc*       fpNtuHit;		    // input data dsc
  TAGparaDsc*       fpBMGeo;		    // input data dsc
  TAGparaDsc*       fpBMCon;		    // input data dsc
};

#endif
