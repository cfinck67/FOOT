#ifndef _TAVTalign_HXX
#define _TAVTalign_HXX
/*!
 \file
 \version $Id: TAVTalign.hxx,v $
 \brief   Declaration of TAVTalign.
 */
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TCanvas.h"
#include "TH2F.h"
#include "TString.h"

#include "TAGobject.hxx"
#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
#include "TAGroot.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuAlign.hxx"


class TAGactionFile;
class TAGaction;
class TAVTactDatRaw;
class TAVTactNtuRaw;
class TAVTactNtuClusterF;
class TAVTactNtuTrack;
class TAVTtrack;

class TAVTalign : public TAGobject {
   
public:
   virtual ~TAVTalign();
   
   //! Update alignment
   Bool_t Update();
   
   //! loop
   void  LoopEvent(Int_t nEvts = 5000);
   
   //! Align algo
   void  Align(Int_t idx, TAVTtrack* aTrack);  
   
   //! Show correction
   void  ShowCorrection();
  
   //! flag for full alignment
   void  SetFullAlignment(Bool_t align) { fIsFullAlignment = align; }
   
   //! Set boundings
   void   SetBounding(Float_t bound);
   
public:
   //! Instance of class
   static TAVTalign* Instance(const TString name = "provaout0020.lmd", Int_t type = 0);  
   
private:
   static TAVTalign* fgInstance; // static instance of class
   
private:
   //! ctr
   TAVTalign(const TString name, Int_t type);
   
   //! Update transformation matrix for a given sensor
   void UpdateTransfo(Int_t idx);
   
   //! create histograms
   void CreateHistograms(); 
	
   //Update geo maps file
   void UpdateGeoMaps();
	
   //Update geo maps file for a given sensor
   void UpdateGeoMapsUVW(fstream &fileIn, fstream &fileOut, Int_t idx);
  
   //! Check for full alignment
   Bool_t IsForAlignment(TAVTtrack* track);
   
private:
   TAGroot*        fAGRoot;             // pointer to TAGroot
   TAGdataDsc*     fpEvent;		        // event data dsc
   TAGdataDsc*     fpDatRaw;		    // input data dsc
   TAGdataDsc*     fpNtuRaw;		    // input ntu data dsc
   TAGdataDsc*     fpNtuTrack;		    // input data dsc
   TAGdataDsc*     fpNtuClus;		    // output data dsc
   TAGparaDsc*     fpConfig;		    // configuration dsc
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc
   TAGparaDsc*     fpParMap;		    // geometry para dsc  

   TAGactionFile *      fActEvtReader;  // action for raw data
   TAGaction*           fActDatRaw;     // action for raw data
   TAVTactNtuRaw*       fActNtuRaw;     // action for ntu data
   TAVTactNtuClusterF*  fActClus;       // action for clusters
   TAVTactNtuTrack*     fActTrack;      // action for tracks
   
   TAVTntuAlign*   fAlign[8];           // pointer to align para
   TH2F*           fhPosu[8];           // deviation in U versus # events
   TH2F*           fhPosv[8];           // deviation in V versus # events
   TH2F*           fhTiltz[8];          // deviation in Tiltz versus # events
   TCanvas*        fAlignMonitor;       // canvas
   TLegend*        fLegend;             // legend of canvas
   TArrayI         fSecArray;           // contains the sensor number to be aligned

   Int_t    fAlignTracks; // nb of track for alignment
   Float_t  fTinyBound;   // bounding for align
   Float_t  fOffsetU[8];
   Float_t  fOffsetV[8];
   Int_t    fIteration[8];
   
   Float_t* fPposU;
   Float_t* fdifU;
   Float_t* fPposUold;
   Float_t* fPosV;
   Float_t* fdifV;
   Float_t* fPosVold;
   Float_t* fTiltW;
   Float_t* fAlignmentU;
   Float_t* fAlignmentV;
   Float_t* fdiftiltW;
   Float_t* fTiltWold;
   Bool_t*  fStopAlign;
   
   Bool_t fIsDrawn;
   Bool_t fIsFullAlignment;
   Int_t  fDebugLevel;
   
   ClassDef(TAVTalign,0)
};

#endif
