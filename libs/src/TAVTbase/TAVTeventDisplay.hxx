
#ifndef _TAVTeventDisplay_HXX_
#define _TAVTeventDisplay_HXX_

#ifndef TAVTbaseEventDisplay_HXX
#include "TAVTbaseEventDisplay.hxx"
#endif

#include "TAGaction.hxx"
#include "TAGactionFile.hxx"
#include "TAGdataDsc.hxx"

/** TAVTeventDisplay class to work on event display
 
 */
class TAVTeventDisplay : public TAVTbaseEventDisplay
{
   
private:
   static TAVTeventDisplay* fgInstance; // static instance of class
   
public:
   //! Instance of class
   static TAVTeventDisplay* Instance(const TString name = "provaout0020.lmd", Int_t type = 0); 
   
protected:
   //! default constructor
   TAVTeventDisplay(const TString name, Int_t type = 0);
      
public:
   virtual ~TAVTeventDisplay();

protected:
   
   //! Draw histo for raw data
   virtual void DrawRawdata();
   
   //! Draw histo for rates
   virtual void DrawRate();
   
   //! Reset rate
   virtual void ResetRate();
   
   //! Reset histograms
   virtual void ResetHistogram();
   
   //! Create raw data action
   virtual void CreateRawAction();
   
   //! Add required items
   virtual void AddRequiredItem();
   
protected:
   // TAG
   TAGdataDsc*     fpEvent;	       // evt data dsc
   TAGaction*      fActDatRaw;     // action for raw data
   TAGactionFile*  fActEvtReader;  // action for raw data reader
   
   ClassDef(TAVTeventDisplay, 1); // class for event display
};


#endif
