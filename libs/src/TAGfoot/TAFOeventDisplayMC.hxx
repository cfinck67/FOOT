
#ifndef _TAFOeventDisplayMC_HXX_
#define _TAFOeventDisplayMC_HXX_


/** TAFOeventDisplay HIT class to work on event display
 
 */

#include "TAFOeventDisplay.hxx"

#include "TFile.h"
#include "TTree.h"

#include "TAGdataDsc.hxx"
#include "TAVTactNtuMC.hxx"
#include "TAITactNtuMC.hxx"
#include "TAMSDactNtuMC.hxx"
#include "TATWactNtuMC.hxx"

#include "Evento.hxx"

class TTree;

class TAFOeventDisplayMC : public TAFOeventDisplay
{
   
protected:
   //! default constructor
   TAFOeventDisplayMC(Int_t type, const TString expName);
   
public:
   //! Instance of class
   static TAFOeventDisplay* Instance(Int_t type = 0, const TString name = "");
   
   virtual ~TAFOeventDisplayMC();
   
   //! Open File
   virtual void OpenFile(const TString fileName);

   //! Create raw data action
   void CreateRawAction();

   //! Tree entry
   Bool_t GetEntry(Int_t entry);

private:
   EVENT_STRUCT*         fEvtStruct;
   
   TAVTactNtuMC*         fActNtuRawVtx;  // action for ntu data
   TAITactNtuMC*         fActNtuRawIt;  // action for ntu data
   TAMSDactNtuMC*        fActNtuRawMsd;  // action for ntu data
   TATWactNtuMC*         fActNtuRawTw;  // action for ntu data
   
   TTree*                fTree;         // tree for MC
   TFile*                fActEvtFile; // file for MC

 
   ClassDef(TAFOeventDisplayMC, 1); // Base class for event display
};


#endif
