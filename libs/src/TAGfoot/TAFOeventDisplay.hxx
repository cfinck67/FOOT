
#ifndef _TAFOeventDisplay_HXX_
#define _TAFOeventDisplay_HXX_

#include "TAGbaseEventDisplay.hxx"

/** TAFOeventDisplay HIT class to work on event display
 
 */
#include <map>

#include "TVirtualMagField.h"

#include "TATRparGeo.hxx"
#include "TABMparGeo.hxx"
#include "TAGparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TADIparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"

#include "TABMparCon.hxx"
#include "TAVTparConf.hxx"
#include "TAITparConf.hxx"
#include "TAMSDparConf.hxx"

#include "TAGclusterDisplay.hxx"
#include "TAGtrackDisplay.hxx"
#include "TAGwireDisplay.hxx"
#include "TAGglbTrackDisplay.hxx"
#include "TAGbaseEventDisplay.hxx"
#include "TAGactionFile.hxx"

#include "TAVTactNtuClusterF.hxx"
#include "TAITactNtuClusterF.hxx"
#include "TAMSDactNtuCluster.hxx"

#include "TAVTactNtuRaw.hxx"
#include "TAITactNtuRaw.hxx"
//#include "TATWactNtuRaw.hxx"


#include "TABMactNtuTrack.hxx"
#include "TAVTactBaseNtuTrack.hxx"
#include "TAVTactNtuVertex.hxx"

#include "TADIeveField.hxx"
#include "FootField.hxx"

class TEveDigitSet;
class TGCheckButton;

class TAFOeventDisplay : public TAGbaseEventDisplay
{
   
protected:
   //! default constructor
   TAFOeventDisplay(Int_t type, const TString expName);
   
protected:
   static TAFOeventDisplay* fgInstance; // static instance of class

public:
   //! Instance of class
   static TAFOeventDisplay* Instance(Int_t type = 0, const TString name = "");
   
   virtual ~TAFOeventDisplay();
      
   virtual void ResetHistogram();
   
   virtual void BuildDefaultGeometry();
   
   virtual void UpdateElements(); 

   //! Create canvases
   virtual void CreateCanvases();
   
   //! Update normal Canvases
   virtual void UpdateDefCanvases();
   
   //! Update Canvases at a given frequence (e.g.: rates)
   virtual void UpdateFreqCanvases();
   
   //! Add required items
   virtual void AddRequiredItem();

   //! Create raw data action
   virtual void CreateRawAction();
   
   //! Create raw reco action
   virtual void CreateRecAction();
   
   //! Open File
   virtual void OpenFile(const TString fileName); 
   
   //! Add elements
   virtual void AddElements();
   
   //! Connect elements
   virtual void ConnectElements();
   
   void UpdateHitInfo(TEveDigitSet* qs, Int_t idx);
   void UpdateTrackInfo(TEveDigitSet* qs, Int_t idx);
   
protected:
   void CreateRecActionBm();
   void CreateRecActionVtx();
   void CreateRecActionIt();
   void CreateRecActionMsd();

   void AddRequiredItemSt();
   void AddRequiredItemBm();
   void AddRequiredItemVtx();
   void AddRequiredItemIt();
   void AddRequiredItemMsd();
   void AddRequiredItemTw();
   void AddRequiredItemCa();
   
   void ReadParFiles();

   void UpdateStcElements();
   void UpdateWireElements();
   void UpdateBarElements();
   void UpdateCrystalElements();
   void UpdateQuadElements(const TString prefix);
   void UpdateTrackElements(const TString prefix);
   void UpdateGlbTrackElements();
   void UpdateElements(const TString prefix);
   void CreateCanvases(const TString prefix);
   void DrawReco(const TString prefix);
   void DrawRawdata(const TString prefix);
   void DrawRate(const TString prefix);
   void ResetRate(const TString prefix);
   void UpdateFreqCanvases(const TString prefix);
   void UpdateDefCanvases(const TString prefix);

   
   //! Update histo of clusters and tracks
   virtual void DrawReco();
   
public:
   //! Set Tracking algorithm
   static void SetTrackingAlgo(char c)
   {
      switch (c) {
         case 'S':
            fgTrackingAlgo = "Std";
            break;
         case 'F':
            fgTrackingAlgo = "Full";
            break;
         case 'H':
            fgTrackingAlgo = "Hough";
            break;
         default:
            printf("SetTrackingAlgo: Wrongly set tracking algorithm");
      }
   }
   
   //! Disable/Enable tracking
   static void DisableTracking()   { fgTrackFlag = false;  }
   static void EnableTracking()    { fgTrackFlag = true;   }
   
   //! Disable/Enable draw vertex
   static void DisableDrawVertex() { fgDrawVertex = false; }
   static void EnableDrawVertex()  { fgDrawVertex = true;  }

protected:
   TAGparaDsc*           fpParGeoSt;
   TAGparaDsc*           fpParGeoG;
   TAGparaDsc*           fpParGeoDi;
   TAGparaDsc*           fpParGeoBm;
   
   TAGparaDsc*           fpParGeoIt;
   TAGparaDsc*           fpParGeoVtx;
   TAGparaDsc*           fpParGeoMsd;
   TAGparaDsc*           fpParGeoTw;
   TAGparaDsc*           fpParGeoCa;
   
   TAGparaDsc*           fpParConfBm;
   TAGparaDsc*           fpParConfIt;
   TAGparaDsc*           fpParConfVtx;
   TAGparaDsc*           fpParConfMsd;
   
   TAGdataDsc*           fpDatRawSt;    // input data dsc
   TAGdataDsc*           fpNtuRawSt;    // input data dsc
   TAGdataDsc*           fpDatRawBm;    // input data dsc
   TAGdataDsc*           fpNtuRawBm;    // input data dsc
   TAGdataDsc*           fpNtuTrackBm;  // input track data dsc

   TAGdataDsc*           fpDatRawVtx;    // input data dsc
   TAGdataDsc*           fpNtuRawVtx;	  // input ntu data dsc
   TAGdataDsc*           fpNtuClusVtx;	  // input cluster data dsc
   TAGdataDsc*           fpNtuTrackVtx;  // input track data dsc
   TAGdataDsc*           fpNtuVtx;		  // input Vtx data dsc
   
   TAGdataDsc*           fpDatRawIt;    // input data dsc
   TAGdataDsc*           fpNtuRawIt;	  // input ntu data dsc
   TAGdataDsc*           fpNtuClusIt;	  // input cluster data dsc
   
   TAGdataDsc*           fpDatRawMsd;    // input data dsc
   TAGdataDsc*           fpNtuRawMsd;	  // input ntu data dsc
   TAGdataDsc*           fpNtuClusMsd;	  // input cluster data dsc

   TAGdataDsc*           fpNtuRawTw;    // input data dsc
   TAGdataDsc*           fpNtuRawCa;    // input data dsc

   TAGaction*            fActDatRawVtx;     // action for raw data
   TAGactionFile*        fActEvtReader;
   
   TABMactNtuTrack*  fActTrackBm;   // action for tracks

   TAVTactNtuRaw*        fActNtuRawVtx;  // action for ntu data
   TAVTactNtuClusterF*   fActClusVtx;    // action for clusters
   TAVTactBaseNtuTrack*  fActTrackVtx;   // action for tracks
   TAVTactBaseNtuVertex* fActVtx;        // action for vertex
   
   TAITactNtuRaw*        fActNtuRawIt;  // action for ntu data
   TAITactNtuClusterF*   fActClusIt;    // action for clusters
   
   TAVTactNtuRaw*        fActNtuRawMsd;  // action for ntu data
   TAMSDactNtuCluster*   fActClusMsd;    // action for clusters
   
  // TATWactNtuRaw*        fActNtuRawTw;  // action for ntu data

   Int_t                 fType;         // type of sensor
   
   //Display
   TAGclusterDisplay*    fStClusDisplay;  // list of quad to display hits

   TAGclusterDisplay*    fVtxClusDisplay;  // list of quad to display hits
   TAGtrackDisplay*      fVtxTrackDisplay; // list of line to display tracks
   
   TAGclusterDisplay*    fItClusDisplay;  // list of quad to display hits
   
   TAGclusterDisplay*    fMsdClusDisplay;  // list of quad to display hits
   
   TAGclusterDisplay*    fTwClusDisplay;  // list of quad to display hits
   
   TAGclusterDisplay*    fCaClusDisplay;  // list of quad to display hits
   
   TAGwireDisplay*       fBmClusDisplay;  // list of line to display wires
   TAGtrackDisplay*      fBmTrackDisplay; // list of line to display tracks

   TAGglbTrackDisplay*   fGlbTrackDisplay;  // list of global tracks to display

   // Magnet
   FootField*            fFieldImpl;       // magnetic field implementation
   TADIeveField*         fField;           // Eve magnetic field

   // TW
   map< pair<Int_t, Int_t>, Int_t > fFiredTofBar;       // list of fired bar per event
   
   // CA
   map< Int_t, Int_t >  fFiredCaCrystal;       // list of fired bar per event

   // GUI
   TGCheckButton*       fClusterButton;    // toggle clusters plots
   TGCheckButton*       fRawDataButton;    // toggle rawdata plots
   TGCheckButton*       fRateButton;       // toggle recompute parameters at each plane
   
private:
   static Bool_t         fgDrawVertex;     // flag for vertex drawing
   static Bool_t         fgTrackFlag;      // flag for tracking
   static TString        fgTrackingAlgo;   // tracking algorithm ("std" with BM, "Full" combinatory and "Hough" Hough transformation)
 
   ClassDef(TAFOeventDisplay, 1); // Base class for event display
};


#endif
