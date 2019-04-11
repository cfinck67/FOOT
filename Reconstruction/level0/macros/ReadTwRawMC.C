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

#include "TATWparGeo.hxx"
#include "TATWparCal.hxx"
#include "TATWntuRaw.hxx"
#include "TATWntuPoint.hxx"

#include "TATWactNtuMC.hxx"
#include "TATWactNtuPoint.hxx"

#include "GlobalPar.hxx"

#endif

// main
TAGactTreeWriter* outFile = 0x0;
TATWactNtuMC* twActRaw = 0x0;
TATWactNtuPoint* twActRec = 0x0;

void FillMCTw(EVENT_STRUCT *myStr) {
   
   /*Ntupling the MC Vertex information*/
   TAGparaDsc* twGeo    = new TAGparaDsc(TATWparGeo::GetDefParaName(), new TATWparGeo());
   TATWparGeo* geomap   = (TATWparGeo*) twGeo->Object();
   geomap->FromFile();
   
   TAGparaDsc* twCal   = new TAGparaDsc("twCal", new TATWparCal());
   TATWparCal* calmap   = (TATWparCal*) twCal->Object();
   calmap->FromFile();

   TAGdataDsc* twRec = new TAGdataDsc("containerPoint", new TATWntuPoint());
   TAGdataDsc* twRaw = new TAGdataDsc("containerHit", new TATWntuRaw());
   
   twActRaw  = new TATWactNtuMC("twActRaw", twRaw, myStr);
   twActRaw->CreateHistogram();

   twActRec  = new TATWactNtuPoint("twActRec", twRaw, twRec, twGeo, twCal);
   twActRec->CreateHistogram();

   
   //outFile->SetupElementBranch(twRaw, TATWntuRaw::GetBranchName());
}

void ReadTwRawMC(TString name = "16O_C2H4_200_15.root")
//void ReadTwRawMC(TString name = "p_80_vtx.root")
//void ReadTwRawMC(TString name = "12C_80_vtx.root")
//void ReadTwRawMC(TString name = "12C_400_vtx.root")
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
   
   FillMCTw(&evStr);
   
   tagr.AddRequiredItem("twActRaw");
   tagr.AddRequiredItem("twActRec");
   tagr.AddRequiredItem("outFile");
   tagr.Print();
   
   Int_t pos = name.Last('.');
   TString nameOut = name(0, pos);
   nameOut.Append("_OutTw.root");
   
   if (outFile->Open(nameOut.Data(), "RECREATE")) return;
   twActRaw->SetHistogramDir(outFile->File());
   twActRec->SetHistogramDir(outFile->File());
   
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
      
      if (ientry == 5000)
//      if (ientry == 20000)
         break;
      
      if (!tagr.NextEvent()) break;
   }
   
   tagr.EndEventLoop();
   
   outFile->Print();
   outFile->Close();
   
   watch.Print();
}


