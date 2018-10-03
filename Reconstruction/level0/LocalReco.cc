

#include "LocalReco.h"

#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"

#include "TAVTdatRaw.hxx"
#include "TAITdatRaw.hxx"
#include "TAMSDdatRaw.hxx"

ClassImp(LocalReco)


//__________________________________________________________
LocalReco::LocalReco(TString fileNameIn, TString fileNameout)
 : BaseLocalReco(fileNameIn, fileNameout),
   fActNtuRawVtx(0x0),
   fActNtuRawIt(0x0),
   fActNtuRawMsd(0x0),
   fpDatRawVtx(0x0),
   fpDatRawIt(0x0),
   fpDatRawMsd(0x0),
   fActEvtReader(0x0)
{
}

//__________________________________________________________
LocalReco::~LocalReco()
{
   // default destructor
   if (fActEvtReader) delete fActEvtReader;
}

//__________________________________________________________
void LocalReco::LoopEvent(Int_t nEvents)
{
   
   for (Int_t ientry = 0; ientry < nEvents; ientry++) {
      
      if(ientry % 100 == 0)
         cout<<" Loaded Event:: " << ientry << endl;
      
      if (!fTAGroot->NextEvent()) break;;
   }
}

//__________________________________________________________
void LocalReco::CreateRawAction()
{
   fpDaqEvent    = new TAGdataDsc("daqEvt", new TAGdaqEvent());

   if (fFlagVtx) {
      fpDatRawVtx   = new TAGdataDsc("vtDat", new TAVTdatRaw());
      fpNtuRawVtx   = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      fActDatRawVtx = new TAVTactDaqRaw("vtAcDat", fpDatRawVtx, fpDaqEvent, fpParGeoVtx);
      if (fFlagHisto)
         fActDatRawVtx->CreateHisto();
      fActNtuRawVtx = new TAVTactNtuRaw("vtActNtu", fpNtuRawVtx, fpDatRawVtx, fpParGeoVtx);
      if (fFlagHisto)
         fActNtuRawVtx->CreateHisto();
   }
   
   if (fFlagIt) {
      fpDatRawIt   = new TAGdataDsc("itDat", new TAITdatRaw());
      fpNtuRawIt   = new TAGdataDsc("itRaw", new TAITntuRaw());
      fActDatRawIt = new TAITactDaqRaw("itAcDat", fpDatRawIt, fpDaqEvent, fpParGeoIt);
      if (fFlagHisto)
         fActDatRawIt->CreateHisto();
      fActNtuRawIt = new TAITactNtuRaw("itActNtu", fpNtuRawIt, fpDatRawIt, fpParGeoIt);
      if (fFlagHisto)
         fActNtuRawIt->CreateHisto();
   }
   
   if (fFlagMsd) {
      fpDatRawMsd   = new TAGdataDsc("msdDat", new TAMSDdatRaw());
      fpNtuRawMsd   = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
      fActDatRawMsd = new TAMSDactDaqRaw("msdAcDat", fpDatRawMsd, fpDaqEvent, fpParGeoMsd);
      if (fFlagHisto)
         fActDatRawMsd->CreateHisto();
      fActNtuRawMsd = new TAMSDactNtuRaw("msdActNtu", fpNtuRawMsd, fpDatRawMsd, fpParGeoMsd);
      if (fFlagHisto)
         fActNtuRawMsd->CreateHisto();
   }
}

//__________________________________________________________
void LocalReco::OpenFileIn()
{
   fActEvtReader = new TAGactDaqReader("daqAct");
   
   fActEvtReader->SetupChannel(fpDaqEvent);
   fActEvtReader->Open(GetName());
}

//__________________________________________________________
void LocalReco::SetRawHistogramDir()
{
   // VTX
   if (fFlagVtx) {
      fActDatRawVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      fActNtuRawVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // IT
   if (fFlagIt) {
      fActDatRawIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      fActNtuRawIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // MSD
   if (fFlagMsd) {
      fActDatRawMsd->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      fActNtuRawMsd->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
}

//__________________________________________________________
void LocalReco::CloseFileIn()
{
   fActEvtReader->Close();
}

//__________________________________________________________
void LocalReco::AddRawRequiredItem()
{
   if (fFlagVtx) {
      fTAGroot->AddRequiredItem("vtActDat");
      fTAGroot->AddRequiredItem("vtActNtu");
   }
   
   if (fFlagIt) {
      fTAGroot->AddRequiredItem("itActDat");
      fTAGroot->AddRequiredItem("itActNtu");
   }
   
   if (fFlagMsd) {
      fTAGroot->AddRequiredItem("msdActDat");
      fTAGroot->AddRequiredItem("msdActNtu");
   }
}