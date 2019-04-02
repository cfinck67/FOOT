

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

Bool_t  LocalReco::fgStdAloneFlag = false;

//__________________________________________________________
LocalReco::LocalReco(TString fileNameIn, TString fileNameout)
 : BaseLocalReco(fileNameIn, fileNameout),
   fpDaqEvent(0x0),
   fActDatRawSt(0x0),
   fActDatRawBm(0x0),
   fActNtuRawBm(0x0),
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
   if (!fgStdAloneFlag) {
      fpDaqEvent = new TAGdataDsc("daqEvt", new TAGdaqEvent());
      fActEvtReader = new TAGactDaqReader("daqActReader", fpDaqEvent);
   }

   if (GlobalPar::GetPar()->IncludeST() ||GlobalPar::GetPar()->IncludeBM()) {
      fpParTimeSt  = new TAGparaDsc("stTime", new TASTparTime());
      fpDatRawSt   = new TAGdataDsc("stDat", new TASTdatRaw());
      fActDatRawSt = new TASTactDatRaw("stActNtu", fpDatRawSt, fpDaqEvent, fpParTimeSt);
      fActDatRawSt->CreateHistogram();
   }

   if (GlobalPar::GetPar()->IncludeBM()) {
      fpDatRawBm = new TAGdataDsc("bmDat", new TABMdatRaw());
      fpNtuRawBm = new TAGdataDsc("bmNtu", new TABMntuRaw());
      
      if (fgStdAloneFlag) {
         fActVmeReaderBm  = new TABMactVmeReader("bmActNtu", fpDatRawBm, fpParMapBm, fpParConfBm, fpParGeoBm, fpDatRawSt);
         fActVmeReaderBm->CreateHistogram();
         
      } else {
         fActDatRawBm = new TABMactDatRaw("bmActDat", fpDatRawBm, fpDaqEvent, fpParMapBm, fpParConfBm, fpParGeoBm, fpDatRawSt);
         fActDatRawBm->CreateHistogram();
         fActNtuRawBm = new TABMactNtuRaw("bmActNtu", fpNtuRawBm, fpDatRawBm, fpDatRawSt, fpParGeoBm, fpParConfBm);
         fActNtuRawBm->CreateHistogram();
      }
   }

   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpNtuRawVtx   = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      
      if (fgStdAloneFlag) {
         fActVmeReaderVtx  = new TAVTactVmeReader("vtActNtu", fpNtuRawVtx, fpParGeoVtx, fpParConfVtx);
         fActVmeReaderVtx->CreateHistogram();
         
      } else {
         fActNtuRawVtx = new TAVTactNtuRaw("vtActNtu", fpNtuRawVtx, fpDaqEvent, fpParGeoVtx, fpParConfVtx);
         fActNtuRawVtx->CreateHistogram();
      }
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
   if (fgStdAloneFlag) {
      if (GlobalPar::GetPar()->IncludeVertex())
         fActVmeReaderVtx->Open(GetName());
      
      if (GlobalPar::GetPar()->IncludeBM())
         fActVmeReaderBm->Open(GetName());
      
   } else
      fActEvtReader->Open(GetName());
}

//__________________________________________________________
void LocalReco::SetRawHistogramDir()
{
   // ST
   if (GlobalPar::GetPar()->IncludeST()) {
      fActDatRawSt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // BM
   if (GlobalPar::GetPar()->IncludeBM()) {
      fActDatRawBm->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      fActNtuRawBm->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // VTX
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fActNtuRawVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }
   
   // IT
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fActNtuRawIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   }

   // TW
   if (GlobalPar::GetPar()->IncludeTW()) {
      fActDatRawTw->SetHistogramDir((TDirectory*)fActEvtWriter->File());
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
   fTAGroot->AddRequiredItem("daqActReader");

   if (GlobalPar::GetPar()->IncludeST() || GlobalPar::GetPar()->IncludeBM()) {
      fTAGroot->AddRequiredItem("stActNtu");
   }

   if (GlobalPar::GetPar()->IncludeBM()) {
      fTAGroot->AddRequiredItem("bmActDat");
      fTAGroot->AddRequiredItem("bmActNtu");
   }

   if (GlobalPar::GetPar()->IncludeVertex()) {
      fTAGroot->AddRequiredItem("vtActNtu");
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fTAGroot->AddRequiredItem("itActNtu");
   }
   
   if (GlobalPar::GetPar()->IncludeTW()) {
      fTAGroot->AddRequiredItem("twActDat");
   }

   
//   if (GlobalPar::GetPar()->IncludeMSD()) {
//      fTAGroot->AddRequiredItem("msdActDat");
//      fTAGroot->AddRequiredItem("msdActNtu");
//   }
}

//__________________________________________________________
void LocalReco::SetTreeBranches()
{
   BaseLocalReco::SetTreeBranches();
   
   if (GlobalPar::GetPar()->IncludeST()) {
      fActEvtWriter->SetupElementBranch(fpNtuRawSt, TASTntuRaw::GetBranchName());
   }
   
   if (GlobalPar::GetPar()->IncludeBM()) {
      if (fFlagHits) {
         fActEvtWriter->SetupElementBranch(fpNtuRawBm, TABMntuRaw::GetBranchName());
      }
   }
   
   if (GlobalPar::GetPar()->IncludeVertex()) {
      if (fFlagHits)
         fActEvtWriter->SetupElementBranch(fpNtuRawVtx, TAVTntuRaw::GetBranchName());
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      if (fFlagHits)
         fActEvtWriter->SetupElementBranch(fpNtuRawIt, TAITntuRaw::GetBranchName());
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      if (fFlagHits)
         fActEvtWriter->SetupElementBranch(fpNtuRawMsd, TAMSDntuRaw::GetBranchName());
   }
   
   if (GlobalPar::GetPar()->IncludeTW()) {
      if (fFlagHits)
         fActEvtWriter->SetupElementBranch(fpNtuRawTw, TATW_ContainerHit::GetBranchName());
   }
   
   if (GlobalPar::GetPar()->IncludeCA()) {
      if (fFlagHits)
         fActEvtWriter->SetupElementBranch(fpNtuRawCa, TACAntuRaw::GetBranchName());
   }
}
