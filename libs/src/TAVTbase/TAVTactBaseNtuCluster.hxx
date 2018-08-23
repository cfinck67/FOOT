#ifndef _TAVTactBaseNtuCluster_HXX
#define _TAVTactBaseNtuCluster_HXX
/*!
 \file
 \version $Id: TAVTactBaseNtuCluster.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAVTactBaseNtuCluster.
 */
/*------------------------------------------+---------------------------------*/
#include <map>

#include "TArrayI.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TString.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"


class TAVTbaseCluster;
class TAVTntuHit;
class TAVTbaseParGeo;
class TH1F;
class TH2F;
class TAVTactBaseNtuCluster : public TAGaction {
   
public:
   explicit  TAVTactBaseNtuCluster(const char* name     =  0,
								           TAGparaDsc* p_config =  0,
								           TAGparaDsc* p_geomap =  0);
   
   virtual ~TAVTactBaseNtuCluster();
   
   //! Action
   virtual  Bool_t Action() { return false; }
   
   //! Apply basic cuts
   virtual Bool_t  ApplyCuts(TAVTbaseCluster* cluster);

   //! Base creation of histogram
   virtual  void   CreateHistogram();
   
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
   TAGparaDsc*     fpConfig;		  // config para dsc
   TAGparaDsc*     fpGeoMap;		  // geometry para dsc
   
   TAVTntuHit*    fPSeed;             // seed pixel
   TVector3*      fCurrentPosition;   // pointer to current position
   TVector3*      fCurrentPosError ;  // pointer to current position error
   TClonesArray*  fListOfPixels;      // list of pixels 
   TClonesArray*  fCurListOfPixels;   // list of pixels in current cluster
   
   map<Int_t, Int_t> fPixelMap; // pixel map;
   map<Int_t, Int_t> fIndexMap; // index map of the pixel;
   TArrayI fFlagMap;
   
   Int_t          fClustersN;     // number of cluster

   Int_t          fDebugLevel;        // debug level
   
   TH1F*          fpHisPixelTot;	     // Total number of pixels per cluster
   TH1F*          fpHisPixel[32];	  // number of pixels per cluster per sensor
   TH2F*          fpHisClusMap[32];   // cluster map per sensor
   
   TString        fPrefix;

protected:
   void  SearchCluster(TAVTbaseParGeo* pGeoMap);
   void  FillMaps(TAVTbaseParGeo* pGeoMap);
   Bool_t ShapeCluster(Int_t noClus, Int_t IndX, Int_t IndY, TAVTbaseParGeo* pGeoMap);
   void   ComputeSeedPosition();
   void   ComputeCoGPosition();
   
   ClassDef(TAVTactBaseNtuCluster,0)
};

#endif
