
#ifndef _LocalRecoMC_HXX_
#define _LocalRecoMC_HXX_

#include "BaseLocalReco.h"

#include "TFile.h"

#include "TAGdataDsc.hxx"
#include "TASTactNtuMC.hxx"
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

#include "TATW_ContainerHit.hxx"
#include "TATW_ContainerPoint.hxx"

#include "Evento.hxx"

class TTree;

class LocalRecoMC : public BaseLocalReco
{
public:
   //! default constructor
   LocalRecoMC(TString fileNameIn, TString fileNameout);
   
   virtual ~LocalRecoMC();
   
   //! Add required items
   virtual void AddRawRequiredItem();

   //! Create raw data action
   virtual void CreateRawAction();
   
   //! Set raw histogram directory
   virtual void SetRawHistogramDir();
   
   //! Create branch in tree
   virtual void SetTreeBranches();
   
   //! Open File
   virtual void OpenFileIn();
   
   //! Close File in
   virtual void CloseFileIn();

   //! Loop events
   virtual void LoopEvent(Int_t nEvents);
   
private:
   void AddRequiredMcItemSt();
   void AddRequiredMcItemBm();
   void AddRequiredMcItemVt();
   void AddRequiredMcItemIt();
   void AddRequiredMcItemMs();
   void AddRequiredMcItemTw();
   void AddRequiredMcItemCa();

private:
   EVENT_STRUCT*         fEvtStruct;
   
   TASTactNtuMC*         fActNtuRawSt;  // action for ntu data
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

   TAGdataDsc*           containerHit;    // input data dsc
   TAGdataDsc*           containerPoint;    // input data dsc
   
   TAMCactNtuEve*        fActNtuMcEve;
   TAMCactNtuStc*        fActNtuMcSt;
   TAMCactNtuBm*         fActNtuMcBm;
   TAMCactNtuVtx*        fActNtuMcVt;
   TAMCactNtuItr*        fActNtuMcIt;
   TAMCactNtuMsd*        fActNtuMcMsd;
   TAMCactNtuTof*        fActNtuMcTw;
   TAMCactNtuCal*        fActNtuMcCa;

   
   TTree*                fTree;         // tree for MC
   TFile*                fActEvtReader; // file for MC
   
   ClassDef(LocalRecoMC, 1); // Base class for event display
};


#endif
