

#ifndef _GlbEventDisplay_HXX_
#define _GlbEventDisplay_HXX_


#ifndef EventDisplay_HXX
#include "EventDisplay.hxx"
#endif

#ifndef TAGactTreeReader_HXX
#include "TAGactTreeReader.hxx"
#endif

#ifndef ROOT_TString
#include "TString.h"
#endif

/** GlbEventDisplay a class to work on the event display
 
 */
class GlbEventDisplay : public EventDisplay
{
   
private:
   static GlbEventDisplay* fgInstance; // static instance of class
   
public:
   //! Instance of class
   static GlbEventDisplay* Instance(const TString name = "provaout0020.lmd");  
   
public:   
   virtual ~GlbEventDisplay();
         
   void EnableVtx()  { fVtxFlag = true;  }
   void DisableVtx() { fVtxFlag = false; }

   void SetMagCurrent(Double_t cur) { fMagCurrent = cur; }
   
   void EnableGlb()  { fGlbFlag = true;  }
   void DisableGlb() { fGlbFlag = false; }

   virtual void UpdateElements(); 
   
   void UpdateGlbInfo(TEveDigitSet* qs, Int_t idx);


private:
   GlbEventDisplay(const TString name);
   
protected:
   //! Create raw data action
   virtual void CreateRawAction();
   
   //! Create raw reco action
   virtual void CreateRecAction();   
   
   //! Add elements
   virtual void AddElements();
   
   //! Add required items
   virtual void AddRequiredItem();
   
   //! Update normal Canvases
   virtual void UpdateDefCanvases() { return; }
      
   //! Update histo of clusters and tracks
   virtual void DrawReco() { return; }
   
   //! Draw histo for raw data
   virtual void DrawRawdata() { return; }
   
   //! Draw histo for rates
   virtual void DrawRate() { return; }
   
   //! Reset rate
   virtual void ResetRate() { return; }
   
   //! Reset histograms
   virtual void ResetHistogram() { return; }
   
   //! Add elements
   virtual void ConnectElements();
      
private:
   
   TAGdataDsc*  fpGlbTrack;   // Glb track ntu
   TAGaction*   fActGlbTrack;  // Action Glb tracking

   //Display
   TAGtrackDisplay*    fGlbTrackDisplay; // list of line to display tracks
   
   Bool_t       fVtxFlag; // vertex flag
   Bool_t       fGlbFlag; // flag to enable Glb tracking action
   Double_t     fMagCurrent; //Current of magnet

   ClassDef(GlbEventDisplay, 0);   
};

#endif
