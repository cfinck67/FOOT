
#ifndef _LocalReco_HXX_
#define _LocalReco_HXX_

#include "BaseLocalReco.h"

#include "TAGdataDsc.hxx"

#include "TASTactDatRaw.hxx"
#include "TABMactDatRaw.hxx"

#include "TAVTactNtuRaw.hxx"
#include "TAITactNtuRaw.hxx"
//#include "TAMSDactNtuRaw.hxx"

//#include "TAMSDactDatRaw.hxx"

#include "TAGdaqEvent.hxx"
#include "TAGactDaqReader.hxx"

class LocalReco : public BaseLocalReco
{
public:
   //! default constructor
   LocalReco(TString fileNameIn, TString fileNameout);
   
   virtual ~LocalReco();
   
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
   TAGdataDsc*           fpDaqEvent;
 //  TAGdataDsc*           fpDatRawMsd;    // input data dsc

   TASTactDatRaw*        fActDatRawSt;  // action for ntu data
   TABMactDatRaw*        fActDatRawBm;
   //   TAMSDactDatuRaw*      fActDatRawMsd;  // action for raw data

   TAVTactNtuRaw*        fActNtuRawVtx;  // action for ntu data
   TAITactNtuRaw*        fActNtuRawIt;  // action for ntu data
//   TAMSDactNtuRaw*       fActNtuRawMsd;  // action for ntu data

   TAGactDaqReader*      fActEvtReader; // reader for real data (DAQ)
   
   ClassDef(LocalReco, 1); // Base class for event display
};


#endif
