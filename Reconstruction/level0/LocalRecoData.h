
#ifndef _LocalRecoData_HXX_
#define _LocalRecoData_HXX_

#include "BaseLocalReco.h"

#include "TFile.h"

#include "TABMparGeo.hxx"

#include "TAGdataDsc.hxx"
#include "TASTactNtuMC.hxx"
#include "TABMactNtuMC.hxx"

#include "TAGactDaqReader.hxx"
// #include "TAGactDaqReader.hxx"

#include "TABMactDatRaw.hxx"
#include "TABMactNtuRaw.hxx"
#include "TABMactNtuTrack.hxx"






#include "Evento.hxx"

class TTree;

class LocalRecoData : public BaseLocalReco {

public:
   //! default constructor
   LocalRecoData(TString fileNameIn, TString fileNameout);
   
   virtual ~LocalRecoData();
   
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
   // void AddRequiredMcItemSt();
   void AddRequiredMcItemBm();
   

private:
   EVENT_STRUCT*         fEvtStruct;
   
   TABMactDatRaw*         m_actDatRaw_BM;  // action for ntu data
   TABMactNtuRaw*         m_actNtuRaw_BM;  // action for ntu data
   TABMactNtuTrack*       m_actNtuTrack_BM;  // action for ntu data
   
   // TAGdataDsc*           fpNtuMcSt;    // input data dsc
   TAGdataDsc *   m_datRaw_BM ;
   TAGdataDsc *   m_ntuRaw_BM;
   TAGdataDsc *   m_ntuTrack_BM;

   TAGparaDsc*    m_parMap_BM;
   
   
   TTree*                fTree;         // tree for MC

   TAGdataDsc*          m_daqEvent;
   TAGactDaqReader*      fActEvtReader; // reader for real data (DAQ)
   
   ClassDef(LocalRecoData, 1); // Base class for event display
};


#endif






























