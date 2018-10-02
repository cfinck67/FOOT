
#ifndef _LocalRecoMC_HXX_
#define _LocalRecoMC_HXX_

#include "BaseLocalReco.h"

#include "TFile.h"

#include "TAGdataDsc.hxx"

#include "TAVTactNtuMC.hxx"
#include "TAITactNtuMC.hxx"
#include "TAMSDactNtuMC.hxx"

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
   
   //! Open File
   virtual void OpenFileIn();
   
   //! Close File in
   virtual void CloseFileIn();

   //! Loop events
   virtual void LoopEvent(Int_t nEvents);

private:
   EVENT_STRUCT*         fEvtStruct;
   
   TAVTactNtuMC*         fActNtuRawVtx;  // action for ntu data
   TAITactNtuMC*         fActNtuRawIt;  // action for ntu data
   TAMSDactNtuMC*        fActNtuRawMsd;  // action for ntu data
   
   TTree*                fTree;         // tree for MC
   TFile*                fActEvtReader; // file for MC
   
   ClassDef(LocalRecoMC, 1); // Base class for event display
};


#endif
