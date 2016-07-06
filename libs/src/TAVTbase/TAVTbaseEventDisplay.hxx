
#ifndef _TAVTbaseEventDisplay_HXX_
#define _TAVTbaseEventDisplay_HXX_


#ifndef ROOT_TString
#include "TString.h"
#endif

#include "TAGaction.hxx"
#include "TAGactionFile.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"


#include "TAGclusterDisplay.hxx"
#include "TAGtrackDisplay.hxx"
#include "TAGeventDisplay.hxx"

#include "TAVTactNtuRaw.hxx"
#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrack.hxx"
#include "TAVTactNtuTrackF.hxx"
#include "TAVTactNtuTrackH.hxx"
#include "TAVTactNtuVertex.hxx"
#include "TAVTactNtuPostTrack.hxx"


class TEveDigitSet;
class TGCheckButton;
class TList;
class TGeoVolume;

/** TAVTbaseEventDisplay base class to work on event display
 
 */
class TAVTbaseEventDisplay : public TAGeventDisplay
{
   
   
public:
   //! default constructor
   TAVTbaseEventDisplay(const TString name, Int_t type = 0);
      
   virtual ~TAVTbaseEventDisplay();
         
   virtual void BuildDefaultGeometry();
      
   virtual void UpdateElements(); 
   
    //! Update hits info
   void UpdateHitInfo(TEveDigitSet* qs, Int_t idx);
   
   //! Update tracks info
   void UpdateTrackInfo(TEveDigitSet* qs, Int_t idx);
   
public:
   // ! enable post tracking
   static void EnablePostTracking() { fgPostTracking = true; } 
      
protected:
   
   //! Update histo of clusters and tracks
   virtual void DrawReco(); 
   
   //! Draw histo for raw data
   virtual void DrawRawdata()  { return; }
   
   //! Draw histo for rates
   virtual void DrawRate()     { return; }
   
   //! Reset rates
   virtual void ResetRate()    { return; }
   
   //! Reset Histogram
   virtual void ResetHistogram() { return; }
   
   //! Create canvases
   virtual void CreateCanvases();
   
   //! Update normal Canvases
   virtual void UpdateDefCanvases();
   
   //! Update Canvases at a given frequence (e.g.: rates)
   virtual void UpdateFreqCanvases();
   
   //! Create raw data action
   virtual void CreateRawAction() { return; }
   
   //! Create raw reco action
   virtual void CreateRecAction();   
   
   //! Add elements
   virtual void ConnectElements();
   
   //! Add elements
   virtual void AddElements();
   
   //! Add required items
   virtual void AddRequiredItem()  { return; }
   
protected:
   // TAG
   TAGdataDsc*     fpDatRaw;		    // input data dsc
   TAGdataDsc*     fpNtuRaw;		    // input ntu data dsc
   TAGdataDsc*     fpNtuTrack;		    // input track data dsc
   TAGdataDsc*     fpNtuPostTrack;		// input track data dsc
   TAGdataDsc*     fpNtuClus;		    // input cluster data dsc
   TAGdataDsc*     fpNtuVtx;		    // input Vtx data dsc
   TAGparaDsc*     fpConfig;		    // configuration dsc
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc  
   TAGparaDsc*     fpParMap;		    // geometry para dsc  
   
   TAGgeoTrafo*         fpFirstGeo;          // first geometry transformer

   TAVTactNtuRaw*        fActNtuRaw;     // action for ntu data
   TAVTactNtuClusterF*   fActClus;       // action for clusters
   TAVTactNtuTrack*      fActTrack;      // action for tracks
   TAVTactBaseNtuVertex* fActVtx;        // action for vertex
   TAVTactNtuPostTrack*  fActPostTrack;  // post action for tracks

   //Display
   TAGclusterDisplay*  fVtxClusDisplay;  // list of quad to display hits
   TAGtrackDisplay*    fVtxTrackDisplay; // list of line to display tracks
   
   Int_t               fType;            // type of raw data
   TString             fFileName;        // file name of rawdata
   Float_t             fTgZ;             // target Z position

   // GUI
   TGCheckButton*  fClusterButton;    // toggle clusters plots
   TGCheckButton*  fRawDataButton;    // toggle rawdata plots
   TGCheckButton*  fRateButton;       // toggle recompute parameters at each plane
   
protected:
   static Bool_t fgPostTracking;      // post tracking flag
   
   
   ClassDef(TAVTbaseEventDisplay, 1); // Base class for facilities to play with level scheme  
};


#endif
