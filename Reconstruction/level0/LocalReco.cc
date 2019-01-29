

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

   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpNtuRawVtx   = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      fpDatRawVtx   = new TAGdataDsc("vtDat", new TAVTdatRaw());
      fActDatRawVtx = new TAVTactDaqRaw("vtAcDat", fpDatRawVtx, fpDaqEvent, fpParGeoVtx);
      fActNtuRawVtx = new TAVTactNtuRaw("vtActNtu", fpNtuRawVtx, fpDatRawVtx, fpParGeoVtx);
      if (fFlagHisto)
         fActNtuRawVtx->CreateHistogram();
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fpDatRawIt   = new TAGdataDsc("itDat", new TAVTdatRaw());
      fpNtuRawIt   = new TAGdataDsc("itRaw", new TAITntuRaw());
      fActDatRawIt = new TAITactDaqRaw("itAcDat", fpDatRawIt, fpDaqEvent, fpParGeoIt);
      fActNtuRawIt = new TAITactNtuRaw("itActNtu", fpNtuRawIt, fpDatRawIt, fpParGeoIt);
      if (fFlagHisto)
         fActNtuRawIt->CreateHistogram();
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fpDatRawMsd   = new TAGdataDsc("msdDat", new TAVTdatRaw());
      fpNtuRawMsd   = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
      fActDatRawMsd = new TAMSDactDaqRaw("msdAcDat", fpDatRawMsd, fpDaqEvent, fpParGeoMsd);
      fActNtuRawMsd = new TAVTactNtuRaw("msdActNtu", fpNtuRawMsd, fpDatRawMsd, fpParGeoMsd);
      if (fFlagHisto)
         fActNtuRawMsd->CreateHistogram();
   }
   
   //   if(GlobalPar::GetPar()->IncludeTW()) {
   //      fpDatRawTw   = new TAGdataDsc("twdDat", new TATWdatRaw());
   //      fpNtuRawTw   = new TAGdataDsc("twRaw", new TATWntuRaw());
   //      fActNtuRawTw = new TAVTactNtuRaw("twActNtu", fpNtuRawTw, fpNtuRawTw, fpParGeoTw);
   //      fActNtuRawTw->CreateHistogram();
   //   }
   
   //   if(GlobalPar::GetPar()->IncludeCA()) {
   //      fpDatRawCa   = new TAGdataDsc("cadDat", new TACAdatRaw());
   //      fpNtuRawCa   = new TAGdataDsc("caRaw", new TACAntuRaw());
   //      fActNtuRawCa = new TAVTactNtuRaw("caActNtu", fpNtuRawCa, fpNtuRawCa, fpParGeoCa);
   //      fActNtuRawCa->CreateHistogram();
   //   }

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
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fActDatRawVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      fActNtuRawVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // IT
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fActDatRawIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      fActNtuRawIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // MSD
   if (GlobalPar::GetPar()->IncludeMSD()) {
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
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fTAGroot->AddRequiredItem("vtActDat");
      fTAGroot->AddRequiredItem("vtActNtu");
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fTAGroot->AddRequiredItem("itActDat");
      fTAGroot->AddRequiredItem("itActNtu");
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fTAGroot->AddRequiredItem("msdActDat");
      fTAGroot->AddRequiredItem("msdActNtu");
   }
}