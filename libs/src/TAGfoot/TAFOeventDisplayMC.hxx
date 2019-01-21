
#ifndef _TAFOeventDisplayMC_HXX_
#define _TAFOeventDisplayMC_HXX_


/** TAFOeventDisplay HIT class to work on event display
 
 */

#include "TAFOeventDisplay.hxx"

#include "TFile.h"
#include "TTree.h"

#include "TAGdataDsc.hxx"
#include "TATRactNtuMC.hxx"
#include "TABMactNtuMC.hxx"
#include "TAVTactNtuMC.hxx"
#include "TAITactNtuMC.hxx"
#include "TAMSDactNtuMC.hxx"
#include "TATWactNtuMC.hxx"
#include "TACAactNtuMC.hxx"

#include "TAMCntuHit.hxx"
#include "TAMCntuEve.hxx"
#include "TAMCactNtuStc.hxx"
#include "TAMCactNtuBm.hxx"
#include "TAMCactNtuVtx.hxx"
#include "TAMCactNtuItr.hxx"
#include "TAMCactNtuMsd.hxx"
#include "TAMCactNtuTof.hxx"
#include "TAMCactNtuCal.hxx"
#include "TAMCactNtuEve.hxx"

#include "TAGpointDisplay.hxx"

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

   //! Add required items
   void AddRequiredItem();

   //! Tree entry
   Bool_t GetEntry(Int_t entry);
   
   // Update elements
   void UpdateElements();

   //! Add elements
   void AddElements();

   //! Connect elements
   void ConnectElements();

   //! Update MC info
   void UpdateStInfo(Int_t idx);
   void UpdateBmInfo(Int_t idx);
   void UpdateVtInfo(Int_t idx);
   void UpdateItInfo(Int_t idx);
   void UpdateMsInfo(Int_t idx);
   void UpdateTwInfo(Int_t idx);
   void UpdateCaInfo(Int_t idx);
   
private:
   EVENT_STRUCT*         fEvtStruct;
   
   TATRactNtuMC*         fActNtuRawSt;  // action for ntu data
   TABMactNtuMC*         fActNtuRawBm;  // action for ntu data
   TAVTactNtuMC*         fActNtuRawVtx;  // action for ntu data
   TAITactNtuMC*         fActNtuRawIt;  // action for ntu data
   TAMSDactNtuMC*        fActNtuRawMsd;  // action for ntu data
   TATWactNtuMC*         fActNtuRawTw;  // action for ntu data
   TACAactNtuMC*         fActNtuRawCa;  // action for ntu data
   
   TAGdataDsc*           fpNtuMcEve;    // input data dsc
   TAGdataDsc*           fpNtuMcSt;    // input data dsc
   TAGdataDsc*           fpNtuMcBm;    // input data dsc
   TAGdataDsc*           fpNtuMcVt;    // input data dsc
   TAGdataDsc*           fpNtuMcIt;    // input data dsc
   TAGdataDsc*           fpNtuMcMsd;    // input data dsc
   TAGdataDsc*           fpNtuMcTw;    // input data dsc
   TAGdataDsc*           fpNtuMcCa;    // input data dsc

   TAMCactNtuEve*        fActNtuMcEve;
   TAMCactNtuStc*        fActNtuMcSt;
   TAMCactNtuBm*         fActNtuMcBm;
   TAMCactNtuVtx*        fActNtuMcVt;
   TAMCactNtuItr*        fActNtuMcIt;
   TAMCactNtuMsd*        fActNtuMcMsd;
   TAMCactNtuTof*        fActNtuMcTw;
   TAMCactNtuCal*        fActNtuMcCa;
   
   TTree*                fTree;         // tree for MC
   TFile*                fActEvtFile; // file for MC

   //Display
   TAGpointDisplay*      fStMcDisplay;
   TAGpointDisplay*      fBmMcDisplay;
   TAGpointDisplay*      fVtMcDisplay;
   TAGpointDisplay*      fItMcDisplay;
   TAGpointDisplay*      fMsdMcDisplay;
   TAGpointDisplay*      fTwMcDisplay;
   TAGpointDisplay*      fCaMcDisplay;

private:
   void  UpdateMcElements(const TString prefix);
   void  UpdateMcInfo(TString prefix, Int_t idx);
   
   void AddRequiredMcItemSt();
   void AddRequiredMcItemBm();
   void AddRequiredMcItemVt();
   void AddRequiredMcItemIt();
   void AddRequiredMcItemMs();
   void AddRequiredMcItemTw();
   void AddRequiredMcItemCa();

   ClassDef(TAFOeventDisplayMC, 1); // Base class for event display
};


#endif
