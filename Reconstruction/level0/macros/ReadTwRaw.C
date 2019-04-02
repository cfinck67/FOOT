
// Macro to read catania raw data
// Ch. Finck, sept 11.


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TROOT.h>
#include <TStopwatch.h>

#include "Evento.hxx"
#include "TAGaction.hxx"
#include "TAGroot.hxx"
#include "TAGactTreeWriter.hxx"

#include "TATWparMap.hxx"
#include "TATWparTime.hxx"

#include "TATWdatRaw.hxx"

#include "TAGdaqEvent.hxx"
#include "TAGactDaqReader.hxx"
#include "TATWactDatRaw.hxx"

#endif

// main
TAGactTreeWriter*   outFile   = 0x0;
TAGactDaqReader*    daqActReader = 0x0;
TATWactDatRaw*      twActRaw  = 0x0;

void FillTW()
{
   TAGparaDsc* twMap = new TAGparaDsc("twMap", new TATWparMap());
   TAGdataDsc* twDaq    = new TAGdataDsc("twDaq", new TAGdaqEvent());
   TAGdataDsc* twDat    = new TAGdataDsc("twDat", new TATWdatRaw());
   TAGparaDsc* twTim    = new TAGparaDsc("twTim", new TATWparTime());

   daqActReader  = new TAGactDaqReader("daqActReader", twDaq);

   twActRaw  = new TATWactDatRaw("twActRaw", twDat, twDaq, twMap, twTim);
   twActRaw->CreateHistogram();

}

void ReadTwRaw(TString filename = "data_test.00001313.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data",
                Int_t nMaxEvts = 3)
{

   TAGroot tagr;
   tagr.SetCampaignNumber(100);
   tagr.SetRunNumber(1);
   
   outFile = new TAGactTreeWriter("outFile");

   FillTW();
   daqActReader->Open(filename);
   
   tagr.AddRequiredItem(daqActReader);
   tagr.AddRequiredItem(twActRaw);
   tagr.AddRequiredItem(outFile);

   tagr.Print();
   
   Int_t pos = filename.First(".");
   
   TString outFileName = filename(pos+1, 8);
   outFileName.Prepend("run.");

   outFileName.Append(".root");
   if (outFile->Open(outFileName.Data(), "RECREATE")) return;
   twActRaw->SetHistogramDir(outFile->File());

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


