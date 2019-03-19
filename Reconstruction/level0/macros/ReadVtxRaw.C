
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

#include "TAGdaqEvent.hxx"
#include "TAGactDaqReader.hxx"
#include "TAVTactNtuRaw.hxx"

#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrackF.hxx"

#endif

// main
TAGactTreeWriter*   outFile   = 0x0;
TAGactDaqReader*    daqActReader = 0x0;

TAVTactNtuRaw*      vtActRaw  = 0x0;
TAVTactNtuClusterF* vtActClus = 0x0;
TAVTactNtuTrackF*   vtActTrck = 0x0;

void FillVertex()
{
   
   TAGparaDsc* vtGeo    = new TAGparaDsc("vtGeo", new TAVTparGeo());
   TAVTparGeo* geomap   = (TAVTparGeo*) vtGeo->Object();
   geomap->FromFile("./geomaps/TAVTdetector.map");
   
   TAGparaDsc*  vtConf  = new TAGparaDsc("vtConf", new TAVTparConf());
   TAVTparConf* parconf = (TAVTparConf*) vtConf->Object();
   parconf->FromFile("./config/TAVTdetector.cfg");

   TAVTparConf::SetHistoMap();
   TAGdataDsc* vtDaq    = new TAGdataDsc("vtDaq", new TAGdaqEvent());
   TAGdataDsc* vtNtu    = new TAGdataDsc("vtNtu", new TAVTntuRaw());
   TAGdataDsc* vtClus   = new TAGdataDsc("vtClus", new TAVTntuCluster());
   TAGdataDsc* vtTrck   = new TAGdataDsc("vtTrck", new TAVTntuTrack());

   daqActReader  = new TAGactDaqReader("daqActReader", vtDaq);

   
   vtActRaw  = new TAVTactNtuRaw("vtActRaw", vtNtu, vtDaq, vtGeo, vtConf);
   vtActRaw->CreateHistogram();

   vtActClus =  new TAVTactNtuClusterF("vtActClus", vtNtu, vtClus, vtConf, vtGeo);
   vtActClus->CreateHistogram();

//   vtActTrck = new TAVTactNtuTrackF("vtActTrck", vtClus, vtTrck, vtConf, vtGeo);
//   vtActTrck->CreateHistogram();
   
//   outFile->SetupElementBranch(vtNtu, "vtrh.");
//   outFile->SetupElementBranch(vtClus, "vtclus.");
//   outFile->SetupElementBranch(vtTrck, "vtTrack.");

}

void ReadVtxRaw(TString filename = "data_test.00001431.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data",
                Int_t nMaxEvts = 1)
{

   TAGroot tagr;
   tagr.SetCampaignNumber(100);
   tagr.SetRunNumber(1);
   
   outFile = new TAGactTreeWriter("outFile");

   FillVertex();
   daqActReader->Open(filename);
   
   tagr.AddRequiredItem(daqActReader);
 //  tagr.AddRequiredItem(vtActRaw);
//   tagr.AddRequiredItem(vtActClus);
//   tagr.AddRequiredItem(vtActTrck);
   tagr.AddRequiredItem(outFile);

   tagr.Print();
   
   Int_t pos = filename.First(".");
   
   
   TString outFileName = filename(pos+1, 8);
   outFileName.Prepend("run.");

   outFileName.Append(".root");
   if (outFile->Open(outFileName.Data(), "RECREATE")) return;
   vtActRaw->SetHistogramDir(outFile->File());
  vtActClus->SetHistogramDir(outFile->File());
//   vtActTrck->SetHistogramDir(outFile->File());

   cout<<" Beginning the Event Loop "<<endl;
   tagr.BeginEventLoop();
   TStopwatch watch;
   watch.Start();
   
   Int_t nEvents = 0;
   while (tagr.NextEvent() ){
      
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


