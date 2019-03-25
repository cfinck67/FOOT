
#ifndef _TAFOeventDisplay_HXX_
#define _TAFOeventDisplay_HXX_

#include "TAGbaseEventDisplay.hxx"

/** TAFOeventDisplay HIT class to work on event display
 
 */
#include <map>

#include "TVirtualMagField.h"

#include "TAGdaqEvent.hxx"
#include "TASTparGeo.hxx"
#include "TABMparGeo.hxx"
#include "TAGparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TADIparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"

#include "TASTparMap.hxx"
#include "TABMparMap.hxx"

#include "TATWparCal.hxx"

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

#include "TASTactDatRaw.hxx"

#include "TABMactVmeReader.hxx"
#include "TABMactDatRaw.hxx"
#include "TABMactNtuRaw.hxx"

#include "TAVTactNtuClusterF.hxx"
#include "TAITactNtuClusterF.hxx"
#include "TAMSDactNtuCluster.hxx"
#include "TATWactNtuPoint.hxx"

#include "TAVTactNtuRaw.hxx"
#include "TAVTactVmeReader.hxx"
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
   void CreateRecActionTw();
   
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
   static void DisableTracking()   { fgTrackFlag = false;     }
   static void EnableTracking()    { fgTrackFlag = true;      }

   //! Disable/Enable stand alone DAQ
   static void DisableStdAlone()   { fgStdAloneFlag = false;  }
   static void EnableStdAlone()    { fgStdAloneFlag = true;   }

protected:
   TAGparaDsc*           fpParGeoSt;
   TAGparaDsc*           fpParGeoG;
   TAGparaDsc*           fpParGeoDi;
   TAGparaDsc*           fpParGeoBm;
   TAGparaDsc*           fpParGeoVtx;
   TAGparaDsc*           fpParGeoIt;
   TAGparaDsc*           fpParGeoMsd;
   TAGparaDsc*           fpParGeoTw;
   TAGparaDsc*           fpParGeoCa;
   
   TAGparaDsc*           fpParMapSt;
   TAGparaDsc*           fpParMapBm;

   TAGparaDsc*           fpParCalBm;
   TAGparaDsc*           fpParCalTw;
   
   TAGparaDsc*           fpParConfBm;
   TAGparaDsc*           fpParConfVtx;
   TAGparaDsc*           fpParConfIt;
   TAGparaDsc*           fpParConfMsd;
   
   TAGdataDsc*           fpDaqEvent;    // input data dsc
   TAGdataDsc*           fpDatRawSt;    // input data dsc
   TAGdataDsc*           fpNtuRawSt;    // input data dsc
   
   TAGdataDsc*           fpDatRawBm;    // input data dsc
   TAGdataDsc*           fpNtuRawBm;    // input data dsc
   TAGdataDsc*           fpNtuTrackBm;  // input track data dsc

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
   TAGdataDsc*           fpNtuRecTw;    // input data dsc
   TAGdataDsc*           fpNtuRawCa;    // input data dsc

   TAGactionFile*        fActEvtReader;

   TASTactDatRaw*        fActDatRawSt;   // action for dat raw ST

   TABMactVmeReader*     fActVmeReaderBm; // action for stand alone reader BM
   TABMactDatRaw*        fActDatRawBm;   // action for dat raw BM
   TABMactNtuRaw*        fActNtuRawBm;   // action for ntu raw BM
   TABMactNtuTrack*      fActTrackBm;   // action for tracks

   TAVTactVmeReader*     fActVmeReaderVtx; // action for stand alone reader VTX
   TAVTactNtuRaw*        fActNtuRawVtx;  // action for ntu data
   TAVTactNtuClusterF*   fActClusVtx;    // action for clusters
   TAVTactBaseNtuTrack*  fActTrackVtx;   // action for tracks
   TAVTactBaseNtuVertex* fActVtx;        // action for vertex
   
   TAITactNtuRaw*        fActNtuRawIt;  // action for ntu data
   TAITactNtuClusterF*   fActClusIt;    // action for clusters
   
   TAVTactNtuRaw*        fActNtuRawMsd;  // action for ntu data
   TAMSDactNtuCluster*   fActClusMsd;    // action for clusters
   
  // TATWactNtuRaw*        fActNtuRawTw;  // action for ntu data
   TATWactNtuPoint*      fActPointTw;    // action for clusters

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
   static Bool_t         fgTrackFlag;      // flag for tracking
   static Bool_t         fgStdAloneFlag;   // flag for standalone DAQ
   static TString        fgTrackingAlgo;   // tracking algorithm ("std" with BM, "Full" combinatory and "Hough" Hough transformation)
 
   ClassDef(TAFOeventDisplay, 1); // Base class for event display
};


#endif
