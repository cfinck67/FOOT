// Macro to make reconstruction from MC data
// Ch. Finck


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TROOT.h>
#include <TStopwatch.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>

#include "Evento.hxx"

#include "TAGaction.hxx"
#include "TAGroot.hxx"
#include "TAGactTreeWriter.hxx"
#include "TAGgeoTrafo.hxx"

#include "TACAparGeo.hxx"
#include "TACAntuRaw.hxx"

#include "TACAactNtuMC.hxx"

#include "GlobalPar.hxx"

#endif

// main
TAGactTreeWriter* outFile = 0x0;
TACAactNtuMC* caActRaw = 0x0;

void FillMCMsd(EVENT_STRUCT *myStr) {
   
   /*Ntupling the MC Vertex information*/
   TAGparaDsc* caGeo    = new TAGparaDsc(TACAparGeo::GetDefParaName(), new TACAparGeo());
   TACAparGeo* geomap   = (TACAparGeo*) caGeo->Object();
   geomap->FromFile();
   
   TAGdataDsc* caRaw    = new TAGdataDsc("caRaw", new TACAntuRaw());
   
   caActRaw  = new TACAactNtuMC("caActRaw", caRaw, caGeo, myStr);
   caActRaw->CreateHistogram();
   
   outFile->SetupElementBranch(caRaw, TACAntuRaw::GetBranchName());
}

void ReadCaRawMC(TString name = "16O_C2H4_200_1.root")
//void ReadCaRawMC(TString name = "p_80_vtx.root")
//void ReadCaRawMC(TString name = "12C_80_vtx.root")
//void ReadCaRawMC(TString name = "12C_400_vtx.root")
{
   GlobalPar::Instance();
   GlobalPar::GetPar()->Print();

   TAGroot tagr;
   TAGgeoTrafo geoTrafo;
   geoTrafo.FromFile();

   tagr.SetCampaignNumber(-1);
   tagr.SetRunNumber(1);
   
   
   TFile* f = new TFile(name.Data());
   f->ls();
   
   TTree* tree = (TTree*)gDirectory->Get("EventTree");
   
   Evento *ev  = new Evento();
   
   EVENT_STRUCT evStr;
   
   ev->FindBranches(tree,&evStr);
   
   outFile = new TAGactTreeWriter("outFile");
   
   FillMCMsd(&evStr);
   
   tagr.AddRequiredItem("caActRaw");
   tagr.AddRequiredItem("outFile");
   tagr.Print();
   
   Int_t pos = name.Last('.');
   TString nameOut = name(0, pos);
   nameOut.Append("_OutCa.root");
   
   if (outFile->Open(nameOut.Data(), "RECREATE")) return;
   caActRaw->SetHistogramDir(outFile->File());
   
   cout<<" Beginning the Event Loop "<<endl;
   tagr.BeginEventLoop();
   TStopwatch watch;
   watch.Start();
   
   Long64_t nentries = tree->GetEntries();
   
   cout<<"Running against ntuple with:: "<<nentries<<" entries!"<<endl;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t ientry = 0; ientry < nentries; ientry++) {
      
      nb = tree->GetEntry(ientry);
      nbytes += nb;
      
      if(ientry % 100 == 0)
         cout<<" Loaded Event:: " << ientry << endl;
      
//      if (ientry == 500)
      if (ientry == 50000)
         break;
      
      if (!tagr.NextEvent()) break;
   }
   
   tagr.EndEventLoop();
   
   outFile->Print();
   outFile->Close();
   
   watch.Print();
}


