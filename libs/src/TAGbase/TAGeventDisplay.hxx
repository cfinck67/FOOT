
#ifndef _TAGeventDisplay_HXX_
#define _TAGeventDisplay_HXX_

#ifndef ROOT_TEveEventManager
#include "TEveEventManager.h"
#endif

#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif

#ifndef ROOT_TH1F
#include "TH1F.h"
#endif

#ifndef ROOT_TH2F
#include "TH2F.h"
#endif

#ifndef ROOT_TGeoMatrix
#include "TGeoMatrix.h"
#endif

#ifndef ROOT_TGeoVolume
#include "TGeoVolume.h"
#endif

#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif

#ifndef ROOT_TGProgressBar
#include "TGProgressBar.h"
#endif

#ifndef ROOT_TGTextView
#include "TGTextView.h"
#endif

#ifndef ROOT_TGTextEntry
#include "TGTextEntry.h"
#endif

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"
#include "TAGroot.hxx"

class TEveDigitSet;
class TGCheckButton;
class TGMainFrame;
class TList;
class TGeoVolume;

/** TAGeventDisplay base class to work on event display
 
 */
class TAGeventDisplay : public TEveEventManager
{
protected:
   //! default constructor
   TAGeventDisplay(Int_t type = 0);
      
protected:
   static Bool_t fgIsGeoLoaded;       // flag if geometry loaded
   static Bool_t fgIsDisplayed;       // flag if a display already opened  
   
   static Bool_t fgGeoDone;           // geometry done flag
   static Bool_t fgGUIFlag;           // flag to disable or enable gui interface
   static Bool_t fgDisplayFlag;       // do not display event and do not make clustering/tracking, define before running
   
public:
   virtual ~TAGeventDisplay();
      
   virtual void ResetHistogram() = 0;
   
   virtual void BuildDefaultGeometry() = 0;
   
   virtual void UpdateElements() = 0; 

   virtual void NextEvent(); //*MENU*
   
   virtual void LoopEvent(Int_t nEvts = 0);
   
   virtual void SetTransparency(Char_t transparency = 50); //*MENU*
   
   virtual void ShowDisplay();
   
   virtual void LoadGeometry(const Char_t* fileName);
   
   virtual void AddGeometry(TGeoVolume* volume, TGeoMatrix* matrix = gGeoIdentity);
   
   //! Clear info view
   void ClearInfoView();
   
   //! Make GUI 
   void MakeGUI();   
   
   Int_t GetCurrentEventId() const { return fCurrentEventId; }
   
   void    SetMaxEnergy(Float_t e) { fMaxEnergy = e;    } 
   Float_t GetMaxEnergy()    const { return fMaxEnergy; } 
   
   void SetTAGroot(TAGroot* root)  { fAGRoot = root;    }
   TAGroot* GetTAGroot()           { return fAGRoot;    }
   
   TList* GetPadList()       const { return fListOfPads; }
   TList* GetCanvasList()    const { return fListOfCanvases; }
   
   //! Set width/height of box
   void SetBoxDefWidth(Float_t w)  { fBoxDefWidth = w;  }
   void SetBoxDefHeight(Float_t h) { fBoxDefHeight = h; }
   
   //! Set width/height of quad
   void SetQuadDefWidth(Float_t w)  { fQuadDefWidth = w;  }
   void SetQuadDefHeight(Float_t h) { fQuadDefHeight = h; }
   
   //! Get width/height of box
   Float_t GetBoxDefWidth()   const { return fBoxDefWidth;  }
   Float_t GetBoxDefHeight()  const { return fBoxDefHeight; }
   
   //! Get width/height of quad
   Float_t GetQuadDefWidth()  const { return fQuadDefWidth;  }
   Float_t GetQuadDefHeight() const { return fQuadDefHeight; }
         
public:
   //! Disable GUI
   static void DisableGUI() { fgGUIFlag = false; }
   
   //! Disable Display
   static void DisableDisplay() { fgDisplayFlag = false; }
   
protected:
   
   //! update event progress bar
   void UpdateEventBar();
   
   //! Set next event
   virtual Bool_t SetNextEvent();
   
   //! Create canvases
   virtual void CreateCanvases() = 0;
   
   //! Update normal Canvases
   virtual void UpdateDefCanvases() = 0;
   
   //! Update Canvases at a given frequence (e.g.: rates)
   virtual void UpdateFreqCanvases() = 0;
   
   //! Add required items
   virtual void AddRequiredItem() = 0;

   //! Create raw data action
   virtual void CreateRawAction() = 0;
   
   //! Create raw reco action
   virtual void CreateRecAction() = 0; 
   
   //! Add elements
   virtual void AddElements() = 0;
   
   //! Add elements
   virtual void ConnectElements() = 0;   
   
protected:
   
   TAGroot*           fAGRoot;             // pointer to TAGroot
   TGeoVolume*        fTopVolume;          // top volume of geometry
   
   Int_t              fCurrentEventId;     // Current event id
   Bool_t             fFirstEventDone;     // flag for first event processed
   Float_t            fMaxEnergy;          // maximum energy fo palette
   
   //Display
   Float_t             fBoxDefWidth;      // default width of track box
   Float_t             fBoxDefHeight;     // default height of track box
   Float_t             fQuadDefWidth;     // default width of hit quad
   Float_t             fQuadDefHeight;    // default height of hit quad
   
   // GUI
   TGMainFrame*       frmMain;           // main frame
   TGTextView*        fInfoView;         // text view for hit/track info
   TGTextEntry*       fEventEntry;       // text entry for event number
   TGNumberEntry*     fNumberEvent;      // number of events to loop
   TGNumberEntry*     fRefreshEvent;     // refreshing frequency
   TGCheckButton*     fRefreshButton;    // refresh display for each event otherwise superimpose events
   TGHProgressBar*    fEventProgress;    // progress event bar
   Int_t              fDebugLevel;       // debug level
   
   //histos
   TList*          fListOfCanvases;   // list of canvases
   TList*          fListOfPads;       // list of pads for histo
      
   ClassDef(TAGeventDisplay, 1); // Base class for event display
};


#endif
