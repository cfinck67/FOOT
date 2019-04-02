
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

#include "TASTparMap.hxx"

#include "TASTdatRaw.hxx"

#include "TAGdaqEvent.hxx"
#include "TAGactDaqReader.hxx"
#include "TASTactDatRaw.hxx"

#endif

// main
TAGactTreeWriter*   outFile   = 0x0;
TAGactDaqReader*    daqActReader = 0x0;
TASTactDatRaw*      stActRaw  = 0x0;

void FillST()
{
   TAGparaDsc* stMap = new TAGparaDsc("stMap", new TASTparMap());
   TAGdataDsc* stDaq    = new TAGdataDsc("stDaq", new TAGdaqEvent());
   TAGdataDsc* stDat    = new TAGdataDsc("stDat", new TASTdatRaw());

   daqActReader  = new TAGactDaqReader("daqActReader", stDaq);

   stActRaw  = new TASTactDatRaw("stActRaw", stDat, stDaq, stMap);
   stActRaw->CreateHistogram();

}

void ReadStRaw(TString filename = "data_test.00001313.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data",
                Int_t nMaxEvts = 3)
{

   TAGroot tagr;
   tagr.SetCampaignNumber(100);
   tagr.SetRunNumber(1);
   
   outFile = new TAGactTreeWriter("outFile");

   FillST();
   daqActReader->Open(filename);
   
   tagr.AddRequiredItem(daqActReader);
   tagr.AddRequiredItem(stActRaw);
   tagr.AddRequiredItem(outFile);

   tagr.Print();
   
   Int_t pos = filename.First(".");
   
   TString outFileName = filename(pos+1, 8);
   outFileName.Prepend("run.");

   outFileName.Append(".root");
   if (outFile->Open(outFileName.Data(), "RECREATE")) return;
   stActRaw->SetHistogramDir(outFile->File());

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


