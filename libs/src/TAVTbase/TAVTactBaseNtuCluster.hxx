#ifndef _TAVTactBaseNtuCluster_HXX
#define _TAVTactBaseNtuCluster_HXX
/*!
 \file
 \version $Id: TAVTactBaseNtuCluster.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAVTactBaseNtuCluster.
 */
/*------------------------------------------+---------------------------------*/

#include "TClonesArray.h"
#include "TVector3.h"
#include "TString.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"


class TAVTcluster;
class TAVTntuHit;
class TH1F;
class TH2F;
class TAVTactBaseNtuCluster : public TAGaction {
   
public:
   explicit  TAVTactBaseNtuCluster(const char* name     =  0,
								   TAGdataDsc* p_nturaw =  0, 
								   TAGdataDsc* p_ntuclus = 0, 
								   TAGparaDsc* p_config =  0,
								   TAGparaDsc* p_geomap =  0);
   
   virtual ~TAVTactBaseNtuCluster();
   
   //! Action
   virtual  Bool_t Action();
   
   //! Base creation of histogram
   virtual  void   CreateHistogram();
   
   //! Apply basic cuts
   virtual Bool_t  ApplyCuts(TAVTcluster* cluster); 
   
   //! Find cluster for a given sensor
   virtual  Bool_t FindClusters(Int_t /*iSensor*/) = 0;
   
    //! Get list of pixels for a given plane
   TClonesArray*   GetListOfPixels()   const { return fListOfPixels; }
    //! Return seed pixel
   TAVTntuHit*     GetSeedPixel()      const { return fPSeed; }  
   
   //! Get position of current cluster
   TVector3*       GetCurrentPosition()        { return fCurrentPosition; }
   //! Get position error of current cluster
   TVector3*       GetCurrentPosError()        { return fCurrentPosError; }
   
   //! Set list of pixels
   void SetListOfPixels(TClonesArray* list)    { fListOfPixels = list;    }
   //! Set debug level
   void SetDebugLevel(Int_t debug)             { fDebugLevel = debug;     }
   //! Set position of current cluster
   void SetCurrentPosition(Float_t u, Float_t v, Float_t z)   { fCurrentPosition->SetXYZ(u,v,z); }
   
   //! Compute position
   virtual void ComputePosition();
   
protected:
   TAGdataDsc*     fpNtuRaw;		  // input data dsc
   TAGdataDsc*     fpNtuClus;		  // output data dsc
   TAGparaDsc*     fpConfig;		  // config para dsc
   TAGparaDsc*     fpGeoMap;		  // geometry para dsc
   
   TAVTntuHit*    fPSeed;             // seed pixel
   TVector3*      fCurrentPosition;   // pointer to current position
   TVector3*      fCurrentPosError ;  // pointer to current position error
   TClonesArray*  fListOfPixels;      // list of pixels 
   TClonesArray*  fCurListOfPixels;   // list of pixels in current cluster
   
   Int_t          fDebugLevel;        // debug level
   
   TH1F*          fpHisPixelTot;	     // Total number of pixels per cluster
   TH1F*          fpHisPixel[32];	  // number of pixels per cluster per sensor
   TH2F*          fpHisClusMap[32];   // cluster map per sensor
   
   TString        fPrefix;

protected:
   void ComputeSeedPosition();
   void ComputeCoGPosition();
   
   ClassDef(TAVTactBaseNtuCluster,0)
};

#endif
