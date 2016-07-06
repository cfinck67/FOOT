

#ifndef _EventDisplay_HXX_
#define _EventDisplay_HXX_

#include <map>

#ifndef TAIReventDisplay_HXX
#include "TAIReventDisplay.hxx"
#endif

#ifndef ROOT_TArrayI
#include "TArrayI.h"
#endif

#ifndef ROOT_TString
#include "TString.h"
#endif

/** EventDisplay a class to work on the event display
 
 */
class EventDisplay : public TAIReventDisplay
{
   
private:
   static EventDisplay* fgInstance; // static instance of class
   
public:
   //! Instance of class
   static EventDisplay* Instance(const TString name = "provaout0020.lmd");  
   
public:   
   virtual ~EventDisplay();
   
   void BuildDefaultGeometry();

   virtual void UpdateElements(); 
   
   void UpdateTofInfo(TEveDigitSet* qs, Int_t idx);
      
   void EnableToF()  { fTofFlag = true;  }
   void DisableToF() { fTofFlag = false; }
   

protected:
   EventDisplay(const TString name);
   
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
   TAGdataDsc*     fpTofEvent;		 // Tof mbs
   TAGdataDsc*     fpTofRaw;		 // Tof raw
   TAGdataDsc*     fpTofNtu;		     // Tof ntu

   TAGaction*      fActTofRaw;       // Action Tof raw
   TAGaction*      fActTofNtu;       // Action Tof ntu raw
  
   TAGparaDsc*     fpTofMap;		 // parameter map Tof 
   TAGparaDsc*     fpTofCal;		 // parameter cal Tof 
   TAGparaDsc*     fpTofGeo;		 // geometry para Tof
   TAGparaDsc*     fpAlaGeo;		 // geometry para Aladin
   
   //Display
   TAGclusterDisplay*  fTofClusDisplay;  // list of quad to display hit Tof   
   Bool_t              fTofFlag;         // flag to enable tof action
   TArrayI             fFiredSlat;       // list of fired slat per event

   
   ClassDef(EventDisplay, 0);   
};

#endif
