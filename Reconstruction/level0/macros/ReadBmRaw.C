
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

#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMdatRaw.hxx"

#include "TAGdaqEvent.hxx"
#include "TAGactDaqReader.hxx"
#include "TABMactDatRaw.hxx"

#endif

// main
TAGactTreeWriter*   outFile   = 0x0;
TAGactDaqReader*    daqActReader = 0x0;
TABMactDatRaw*      bmActRaw  = 0x0;

void FillBM()
{
   TAGparaDsc* bmGeo    = new TAGparaDsc("bmGeo", new TABMparGeo());
   TABMparGeo* geomap   = (TABMparGeo*) bmGeo->Object();
   geomap->FromFile("./geomaps/TABMdetector.map");
   
   TAGparaDsc*  bmConf  = new TAGparaDsc("bmConf", new TABMparCon());
   TABMparCon* parconf = (TABMparCon*) bmConf->Object();
   parconf->FromFile("./config/beammonitor.cfg");
   parconf->LoadReso("./config/bmreso_vs_r.root");

   TAGparaDsc* bmMap = new TAGparaDsc("bmMap", new TABMparMap());

   TAGdataDsc* bmDaq    = new TAGdataDsc("bmDaq", new TAGdaqEvent());
   TAGdataDsc* bmDat    = new TAGdataDsc("bmDat", new TABMdatRaw());

   daqActReader  = new TAGactDaqReader("daqActReader", bmDaq);

   bmActRaw  = new TABMactDatRaw("bmActRaw", bmDat, bmDaq, bmMap, bmConf, bmGeo);
   bmActRaw->CreateHistogram();

}

void ReadBmRaw(TString filename = "data_test.00001431.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data",
                Int_t nMaxEvts = 3)
{

   TAGroot tagr;
   tagr.SetCampaignNumber(100);
   tagr.SetRunNumber(1);
   
   outFile = new TAGactTreeWriter("outFile");

   FillBM();
   daqActReader->Open(filename);
   
   tagr.AddRequiredItem(daqActReader);
   tagr.AddRequiredItem(bmActRaw);
   tagr.AddRequiredItem(outFile);

   tagr.Print();
   
   Int_t pos = filename.First(".");
   
   TString outFileName = filename(pos+1, 8);
   outFileName.Prepend("run.");

   outFileName.Append(".root");
   if (outFile->Open(outFileName.Data(), "RECREATE")) return;
   bmActRaw->SetHistogramDir(outFile->File());

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


