

#include "TTree.h"

#include "LocalRecoMC.h"

#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"




ClassImp(LocalRecoMC)


//__________________________________________________________
LocalRecoMC::LocalRecoMC(TString fileNameIn, TString fileNameout)
 : BaseLocalReco(fileNameIn, fileNameout),
   fActNtuRawVtx(0x0),
   fActNtuRawIt(0x0),
   fActNtuRawMsd(0x0),
   fTree(0x0),
   fActEvtReader(0x0)
{
   fEvtStruct = new EVENT_STRUCT;
}

//__________________________________________________________
LocalRecoMC::~LocalRecoMC()
{
   // default destructor
   if (fTree) delete fTree;
   if (fActEvtReader) delete fActEvtReader;
}


//__________________________________________________________
void LocalRecoMC::LoopEvent(Int_t nEvents)
{
   if (nEvents == 0)
      nEvents = fTree->GetEntries();
   
   if (nEvents > fTree->GetEntries())
      nEvents = fTree->GetEntries();
   
   for (Long64_t ientry = 0; ientry < nEvents; ientry++) {
      
      fTree->GetEntry(ientry);
      
      if(ientry % 100 == 0)
         cout<<" Loaded Event:: " << ientry << endl;
      
      if (!fTAGroot->NextEvent()) break;
   }
}

//__________________________________________________________
void LocalRecoMC::CreateRawAction()
{
   if (fFlagVtx) {
      fpNtuRawVtx = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      fActNtuRawVtx = new TAVTactNtuMC("vtActNtu", fpNtuRawVtx, fpParGeoVtx, fEvtStruct);
      if (fFlagHisto)
         fActNtuRawVtx->CreateHistogram();
   }
   
   if (fFlagIt) {
      fpNtuRawIt = new TAGdataDsc("itRaw", new TAITntuRaw());
      fActNtuRawIt = new TAITactNtuMC("itActNtu", fpNtuRawIt, fpParGeoIt, fEvtStruct);
      if (fFlagHisto)
         fActNtuRawIt->CreateHistogram();
   }
   
   if (fFlagMsd) {
      fpNtuRawMsd = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
      fActNtuRawMsd = new TAMSDactNtuMC("msdActNtu", fpNtuRawMsd, fpParGeoMsd, fEvtStruct);
      if (fFlagHisto)
         fActNtuRawMsd->CreateHistogram();
   }
}

//__________________________________________________________
void LocalRecoMC::OpenFileIn()
{
   fActEvtReader = new TFile(GetName());
   fTree = (TTree*)fActEvtReader->Get("EventTree");
   
   Evento *ev  = new Evento();
   ev->FindBranches(fTree, fEvtStruct);
}

//__________________________________________________________
void LocalRecoMC::SetRawHistogramDir()
{
   // VTX
   if (fFlagVtx)
      fActNtuRawVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   
   // IT
   if (fFlagIt)
      fActNtuRawIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   
   
   // MSD
   if (fFlagMsd)
      fActNtuRawMsd->SetHistogramDir((TDirectory*)fActEvtWriter->File());
}

//__________________________________________________________
void LocalRecoMC::CloseFileIn()
{
   fActEvtReader->Close();
}

//__________________________________________________________
void LocalRecoMC::AddRawRequiredItem()
{
   if (fFlagVtx)
      fTAGroot->AddRequiredItem("vtActNtu");
   
   if (fFlagIt)
      fTAGroot->AddRequiredItem("itActNtu");
   
   if (fFlagMsd)
      fTAGroot->AddRequiredItem("msdActNtu");
}