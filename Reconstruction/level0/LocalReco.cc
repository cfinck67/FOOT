

#include "LocalReco.h"

#include "TASTntuRaw.hxx"
#include "TABMntuRaw.hxx"
#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"
#include "TATW_ContainerHit.hxx"
#include "TATW_ContainerPoint.hxx"
#include "TACAntuRaw.hxx"

#include "TASTdatRaw.hxx"
#include "TABMdatRaw.hxx"
#include "TAMSDdatRaw.hxx"


ClassImp(LocalReco)


//__________________________________________________________
LocalReco::LocalReco(TString fileNameIn, TString fileNameout)
 : BaseLocalReco(fileNameIn, fileNameout),
   fpDaqEvent(0x0),
   fActDatRawSt(0x0),
   fActDatRawBm(0x0),
   fActNtuRawVtx(0x0),
   fActNtuRawIt(0x0),
//   fActNtuRawMsd(0x0),
//   fpDatRawVtx(0x0),
//   fpDatRawIt(0x0),
//   fpDatRawMsd(0x0),
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

   if (GlobalPar::GetPar()->IncludeST() ||GlobalPar::GetPar()->IncludeBM()) {
      fpDatRawSt   = new TAGdataDsc("stDat", new TASTdatRaw());
      fActDatRawSt = new TASTactDatRaw("stActNtu", fpDatRawSt, fpDaqEvent, fpParMapSt);
      fActDatRawSt->CreateHistogram();
   }

   if (GlobalPar::GetPar()->IncludeBM()) {
      fpDatRawBm   = new TAGdataDsc("bmDat", new TAVTdatRaw());
      fActDatRawBm = new TABMactDatRaw("bmActNtu", fpDatRawBm, fpDaqEvent, fpParMapBm, fpParConfBm, fpParGeoBm);
      fActDatRawBm->CreateHistogram();
   }

   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpNtuRawVtx   = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      fActNtuRawVtx = new TAVTactNtuRaw("vtActNtu", fpNtuRawVtx, fpDaqEvent, fpParGeoVtx, fpParConfVtx);
      fActNtuRawVtx->CreateHistogram();
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fpNtuRawIt   = new TAGdataDsc("itRaw", new TAITntuRaw());
      fActNtuRawIt = new TAITactNtuRaw("itActNtu", fpNtuRawIt, fpDaqEvent, fpParGeoIt, fpParConfIt);
      fActNtuRawIt->CreateHistogram();
   }
   
//   if (GlobalPar::GetPar()->IncludeMSD()) {
//      fpDatRawMsd   = new TAGdataDsc("msdDat", new TAVTdatRaw());
//      fpNtuRawMsd   = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
//      fActDatRawMsd = new TAMSDactDaqRaw("msdAcDat", fpDatRawMsd, fpDaqEvent, fpParGeoMsd);
//      fActNtuRawMsd = new TAVTactNtuRaw("msdActNtu", fpNtuRawMsd, fpDatRawMsd, fpParGeoMsd);
//      if (fFlagHisto)
//         fActNtuRawMsd->CreateHistogram();
//   }
   
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
   fpDaqEvent    = new TAGdataDsc("daqEvt", new TAGdaqEvent());
   fActEvtReader = new TAGactDaqReader("daqAct", fpDaqEvent);
   fActEvtReader->Open(GetName());
}

//__________________________________________________________
void LocalReco::SetRawHistogramDir()
{
   // VTX
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fActNtuRawVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // IT
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fActNtuRawIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // MSD
//   if (GlobalPar::GetPar()->IncludeMSD()) {
//      fActDatRawMsd->SetHistogramDir((TDirectory*)fActEvtWriter->File());
//      fActNtuRawMsd->SetHistogramDir((TDirectory*)fActEvtWriter->File());
//   }
}

//__________________________________________________________
void LocalReco::CloseFileIn()
{
   fActEvtReader->Close();
}

//__________________________________________________________
void LocalReco::AddRawRequiredItem()
{
   fTAGroot->AddRequiredItem("daqAct");

   if (GlobalPar::GetPar()->IncludeST()) {
      fTAGroot->AddRequiredItem("stActNtu");
   }

   if (GlobalPar::GetPar()->IncludeBM()) {
      fTAGroot->AddRequiredItem("bmActNtu");
   }

   if (GlobalPar::GetPar()->IncludeVertex()) {
      fTAGroot->AddRequiredItem("vtActNtu");
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fTAGroot->AddRequiredItem("itActNtu");
   }
   
//   if (GlobalPar::GetPar()->IncludeMSD()) {
//      fTAGroot->AddRequiredItem("msdActDat");
//      fTAGroot->AddRequiredItem("msdActNtu");
//   }
}