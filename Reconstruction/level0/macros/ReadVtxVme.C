
// Macro to read catania raw data
// Ch. Finck, sept 11.


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TROOT.h>
#include <TStopwatch.h>

#include "TAGaction.hxx"
#include "TAGroot.hxx"
#include "TAGactTreeWriter.hxx"

#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTntuRaw.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTntuTrack.hxx"

#include "TAVTactVmeReader.hxx"
#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrackF.hxx"

#endif

// main
TAGactTreeWriter*   outFile   = 0x0;
TAVTactVmeReader*   vmeFile   = 0x0;
TAVTactNtuClusterF* vtActClus = 0x0;
TAVTactNtuTrackF*   vtActTrck = 0x0;

void FillVertex()
{
   
   TAGparaDsc* vtGeo    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   TAVTparGeo* geomap   = (TAVTparGeo*) vtGeo->Object();
   geomap->FromFile("./geomaps/TAVTdetector_GSI.map");
   
   TAGparaDsc*  vtConf  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) vtConf->Object();
   parconf->FromFile("./config/TAVTdetector_GSI.cfg");

   TAVTparConf::SetHistoMap();
   TAGdataDsc* vtNtu    = new TAGdataDsc("vtNtu", new TAVTntuRaw());
   TAGdataDsc* vtClus   = new TAGdataDsc("vtClus", new TAVTntuCluster());
   TAGdataDsc* vtTrck   = new TAGdataDsc("vtTrck", new TAVTntuTrack());


   vtActClus =  new TAVTactNtuClusterF("vtActClus", vtNtu, vtClus, vtConf, vtGeo);
   vtActClus->CreateHistogram();

//   vtActTrck = new TAVTactNtuTrackF("vtActTrck", vtClus, vtTrck, vtConf, vtGeo);
//   vtActTrck->CreateHistogram();
   
//   outFile->SetupElementBranch(vtNtu, "vtrh.");
//   outFile->SetupElementBranch(vtClus, "vtclus.");
//   outFile->SetupElementBranch(vtTrck, "vtTrack.");

   vmeFile = new TAVTactVmeReader("vmeFile", vtNtu, vtGeo, vtConf);
   vmeFile->CreateHistogram();

}

void ReadVtxVme(TString path = "./run_1041", Int_t nMaxEvts = 10000)
{

   TAGroot tagr;
   tagr.SetCampaignNumber(100);
   tagr.SetRunNumber(1);
   
   outFile = new TAGactTreeWriter("outFile");

   FillVertex();
   vmeFile->Open(path);
   
   tagr.AddRequiredItem(vmeFile);
   tagr.AddRequiredItem(vtActClus);
//   tagr.AddRequiredItem(vtActTrck);
   tagr.AddRequiredItem(outFile);

   tagr.Print();
   
   TString outFileName = path;
   outFileName.Append(".root");
   if (outFile->Open(outFileName.Data(), "RECREATE")) return;
   vmeFile->SetHistogramDir(outFile->File());
  vtActClus->SetHistogramDir(outFile->File());
//   vtActTrck->SetHistogramDir(outFile->File());

   cout<<" Beginning the Event Loop "<<endl;
   tagr.BeginEventLoop();
   TStopwatch watch;
   watch.Start();
   
   Int_t nEvents = 0;
   while (tagr.NextEvent() ){
      TAVTactVmeReader::IncrementTrigger();

     // printf("\n");
      if (++nEvents % 100 == 0)
		printf("Event: %d\n", nEvents); 
	  
	  if (nEvents == nMaxEvts)
		 break;
   }
   
   tagr.EndEventLoop();
   
   outFile->Print();
   outFile->Close();
   
   watch.Print();

}


