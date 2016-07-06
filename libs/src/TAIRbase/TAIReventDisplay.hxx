

#ifndef _TAIReventDisplay_HXX_
#define _TAIReventDisplay_HXX_

#include <map>

#ifndef TAVTeventDisplay_HXX
#include "TAVTeventDisplay.hxx"
#endif

#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef ROOT_TArrayI
#include "TArrayI.h"
#endif

class TAGactMbsReader;
class TAVTactDatRaw;
class TAVTactNtuRaw;
class TAGwireDisplay;
class TAKEtrack;
/** TAIReventDisplay a class to work on the raw event display
 
 */
class TAIReventDisplay : public TAVTeventDisplay
{
   
private:
   static TAIReventDisplay* fgInstance; // static instance of class
   
public:
   //! Instance of class
   static TAIReventDisplay* Instance(const TString name = "provaout0020.lmd");  
   
public:   
   virtual ~TAIReventDisplay();
   
   void BuildDefaultGeometry();

   virtual void UpdateElements(); 
   
   void UpdateBmInfo(TEveDigitSet* qs, Int_t idx);
   
   void UpdateKeInfo(TEveDigitSet* qs, Int_t idx);
   
   void EnableKentros()  { fKeFlag = true;  }
   void DisableKentros() { fKeFlag = false; }
   
   void EnableBM()       { fBmFlag = true;  }
   void DisableBM()      { fBmFlag = false; }

protected:
   TAIReventDisplay(const TString name);
   
protected:
   
   //! Create raw data action
   virtual void CreateRawAction();
   
   //! Create raw reco action
   virtual void CreateRecAction();   
   
   //! Add elements
   virtual void AddElements();
   
   //! Add required items
   virtual void AddRequiredItem();
   
   //! Create canvases
   virtual void CreateCanvases();
   
   //! Update normal Canvases
   virtual void UpdateDefCanvases();
      
   //! Update histo of clusters and tracks
   virtual void DrawReco(); 
   
   //! Add elements
   virtual void ConnectElements();
      
protected:

   TAGdataDsc*     fpTrRaw;		        // trigger raw
   TAGdataDsc*     fpIrRaw;		        // IR raw
   TAGdataDsc*     fpBmRaw;		        // BM raw
   TAGdataDsc*     fpBmNtu;		        // BM ntu
   TAGdataDsc*     fpBmTrack;	        // BM track
   
   TAGdataDsc*     fpKeRaw;		        // Kentros raw
   TAGdataDsc*     fpKeNtu;		        // Kentros ntu
   TAGdataDsc*     fpKeTrack;	        // Kentros track
   
   TAGaction*      fActTrRaw;       // Action trigger raw
   TAGaction*      fActIrRaw;       // Action IR raw
   TAGaction*      fActBmRaw;       // Action BM raw
   TAGaction*      fActBmNtu;       // Action BM ntu raw
   TAGaction*      fActBmTrack;     // Action BM track
   TAGaction*      fActKeRaw;       // Action Kentros raw
   TAGaction*      fActKeNtu;       // Action Kentros ntu raw
   TAGaction*      fActKeTrack;     // Action Kentros track
   

   TAGparaDsc*     fpTrMap;		    // parameter map Trigger
   TAGparaDsc*     fpIrMap;		    // parameter map IR  
   TAGparaDsc*     fpBmMap;		    // parameter map BM 
   TAGparaDsc*     fpBmGeo;		    // geometry para BM 
   TAGparaDsc*     fpBmConf;        // configuration BM  
   TAGparaDsc*     fpKeGeo;		    // geometry para Kentros
   TAGparaDsc*     fpKeMap;		    // map para Kentros

   //Display
   TAGwireDisplay*  fBmClusDisplay;  // list of line to display wires
   TAGtrackDisplay* fBmTrackDisplay; // list of line to display BM tracks
   TAGtrackDisplay* fKeTrackDisplay; // list of line to display Kentros tracks
   
   Bool_t           fKeFlag;         // flag to enable kentros action
   Bool_t           fBmFlag;         // flag to enable BM action
   TArrayI          fKeFiredMod;      // list of fired module per event
   
   ClassDef(TAIReventDisplay, 0);   
};

#endif
