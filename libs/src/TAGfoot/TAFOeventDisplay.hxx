
#ifndef _TAFOeventDisplay_HXX_
#define _TAFOeventDisplay_HXX_

#include "TAGbaseEventDisplay.hxx"

/** TAFOeventDisplay FOOT class to work on event display
 
 */
#include "TAGbaseEventDisplay.hxx"

/** TAFOeventnDisplay FOOT class to work on event display
 
 */
#include <map>

#include "TVirtualMagField.h"

#include "TADIeveField.hxx"
#include "FootField.hxx"

#include "LocalReco.hxx"

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

   //! Set local reconstruction
   virtual void SetLocalReco();
   
   //! Add required items
   virtual void AddRequiredItem();

   //! Create raw data action
   virtual void CreateRawAction();
   
   //! Create raw reco action
   virtual void CreateRecAction();
      
   //! Open File
   virtual void SetFileName(const TString fileName);
   
   //! Open File
   virtual void OpenFile();
   
   //! Add elements
   virtual void AddElements();
   
   //! Connect elements
   virtual void ConnectElements();
   
   void UpdateHitInfo(TEveDigitSet* qs, Int_t idx);
   void UpdateTrackInfo(TEveDigitSet* qs, Int_t idx);
    void UpdateDriftCircleInfo(TEveDigitSet* qs, Int_t idx);
   
protected:
   void CreateRecActionBm();
   void CreateRecActionVtx();
   void CreateRecActionIt();
   void CreateRecActionMsd();
   void CreateRecActionTw();
   
   //! Add required items
   void AddRequiredRawItem();
   void AddRequiredRecItem();

   
   void ReadParFiles();

   void UpdateStcElements();
   void UpdateLayerElements();
   void UpdateBarElements();
   void UpdateCrystalElements();
   void UpdateQuadElements(const TString prefix);
   void UpdateTrackElements(const TString prefix);
   void UpdateGlbTrackElements();
   void UpdateElements(const TString prefix);
   
public:
   //! Set VTX Tracking algorithm
   static void SetVtxTrackingAlgo(char c)
   {
      switch (c) {
         case 'S':
            fgVtxTrackingAlgo = "Std";
            break;
         case 'F':
            fgVtxTrackingAlgo = "Full";
            break;
         case 'H':
            fgVtxTrackingAlgo = "Hough";
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

   //! Disable/Enable stand alone DAQ
   static void DisableBmSelectHit()   { fgBmSelectHt = false;  }
   static void EnableBmSelectHit()    { fgBmSelectHt = true;   }

protected:
   BaseLocalReco*        fLocalReco;    // local reco

   Int_t                 fType;         // type of sensor
   
   TAGparaDsc*            fpParGeoDi;

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
    TEveBoxSet*          fBmDriftCircleDisplay;

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
   
protected:
   static Bool_t         fgTrackFlag;       // flag for tracking
   static Bool_t         fgStdAloneFlag;    // flag for standalone DAQ
   static TString        fgVtxTrackingAlgo; // tracking algorithm ("std" with BM, "Full" combinatory and "Hough" Hough transformation)
   static Bool_t         fgBmSelectHt;      // flag BM selected hit

   ClassDef(TAFOeventDisplay, 1); // Base class for event display
};


#endif
