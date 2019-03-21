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

#include "TAGaction.hxx"
#include "TAGroot.hxx"
#include "TAGactTreeWriter.hxx"
#include "TAGgeoTrafo.hxx"

#include "TAGdaqEvent.hxx"
#include "TAGactDaqReader.hxx"

#include "TAGparGeo.hxx"

#include "TASTparMap.hxx"
#include "TASTdatRaw.hxx"
#include "TASTactDatRaw.hxx"

#include "TABMparGeo.hxx"
#include "TABMparMap.hxx"
#include "TABMparCon.hxx"
#include "TABMdatRaw.hxx"
#include "TABMntuRaw.hxx"
#include "TABMactDatRaw.hxx"
#include "TABMactNtuRaw.hxx"
#include "TABMactNtuTrack.hxx"

#include "GlobalPar.hxx"
#include "Materials.hxx"

#endif

// main
TAGactTreeWriter* outFile   = 0x0;

TAGactDaqReader*  daqActReader = 0x0;
TASTactDatRaw* stActDatRaw  = 0x0;
TABMactDatRaw* bmActDatRaw  = 0x0;
TABMactNtuRaw* bmActNtuRaw  = 0x0;
//~ TABMactNtuTrack* bmActTrack = 0x0;

void FillBm(TString name) {
   
   cout<<"start FillBm"<<endl;
   
   //parameters
   TAGparaDsc* tgGeo = new TAGparaDsc(TAGparGeo::GetDefParaName(), new TAGparGeo());
   TAGparGeo* parGeo = (TAGparGeo*)tgGeo->Object();
   parGeo->FromFile();
     
   TAGparaDsc* bmGeo    = new TAGparaDsc("bmGeo", new TABMparGeo());
   TABMparGeo* bmgeomap   = (TABMparGeo*) bmGeo->Object();
   bmgeomap->FromFile();

   TAGparaDsc*  bmConf  = new TAGparaDsc("bmConf", new TABMparCon());
   TABMparCon* parConf = (TABMparCon*)bmConf->Object();
   TString parFileName = "./config/beammonitor.cfg";
   parConf->FromFile(parFileName.Data());
   parFileName = "./config/bmreso_vs_r.root";
   parConf->LoadReso(parFileName);
   vector<double> fakeT0(36,0.);//only for testing the data_test.00001201.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data.moved file
   parConf->SetT0s(fakeT0);
   
   TAGparaDsc*  bmMap  = new TAGparaDsc("bmMap", new TABMparMap());
   TABMparMap*  bmparMap = (TABMparMap*)bmMap->Object();
   parFileName = "./geomaps/";
   parFileName += parConf->GetParmapfile();
   bmparMap->FromFile(parFileName.Data(), bmgeomap);

   TAGparaDsc*  stMap  = new TAGparaDsc("stMap", new TASTparMap());
   TASTparMap*  stparMap = (TASTparMap*)stMap->Object();
   parFileName = "./geomaps/tr_ch.map";
   stparMap->FromFile(parFileName.Data());
  
  //TAGdataDsc
   TAGdataDsc* bmDaq    = new TAGdataDsc("bmDaq", new TAGdaqEvent());
   daqActReader  = new TAGactDaqReader("daqActReader", bmDaq); 
  
   TAGdataDsc* stDatRaw    = new TAGdataDsc("stDat", new TASTdatRaw());
   stActDatRaw  = new TASTactDatRaw("stActDatRaw", stDatRaw,bmDaq,stMap);
   
   TAGdataDsc* bmDatRaw    = new TAGdataDsc("bmDat", new TABMdatRaw());
   bmActDatRaw  = new TABMactDatRaw("bmActDatRaw", bmDatRaw, bmDaq, bmMap, bmConf, bmGeo,stDatRaw);

   TAGdataDsc* bmNtuRaw    = new TAGdataDsc("bmNtuRaw", new TABMntuRaw());
   bmActNtuRaw  = new TABMactNtuRaw("bmActNtuRaw", bmNtuRaw, bmDatRaw, stDatRaw, bmGeo, bmConf);
   bmActNtuRaw->CreateHistogram();   
   
   //~ TAGdataDsc* bmTrack = new TAGdataDsc("bmTrack", new TABMntuTrack());
   //~ bmActTrack  = new TABMactNtuTrack("bmActTrack", bmTrack, bmNtuRaw, bmGeo, bmConf, tgGeo);
   //~ bmActTrack->CreateHistogram();

   cout<<"end of FillBm"<<endl;

   // outFile->SetupElementBranch(bmNtuRaw, TABMntuRaw::GetBranchName());
   // outFile->SetupElementBranch(bmTrck, TABMntuTrack::GetBranchName());
}

//~ void ReadBmRaw(TString name = "data_test.00001462.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data")
void ReadBmRaw(TString name = "data_test.00001201.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data.moved")
{
   GlobalPar::Instance();
   GlobalPar::GetPar()->Print();

   TAGroot tagr;
   TAGgeoTrafo geoTrafo;
   geoTrafo.FromFile();

   tagr.SetCampaignNumber(1);
   tagr.SetRunNumber(1);

   outFile = new TAGactTreeWriter("outFile");
   FillBm(name);
   daqActReader->Open(name);
   cout<<"the daq file is open? "<<daqActReader->IsOpen()<<endl;
   
   tagr.AddRequiredItem("daqActReader");
   tagr.AddRequiredItem("stActDatRaw");
   tagr.AddRequiredItem("bmActDatRaw");
   tagr.AddRequiredItem("bmActNtuRaw");
   //~ tagr.AddRequiredItem("bmActTrack");
   tagr.AddRequiredItem("outFile");
   tagr.Print();
   
   Int_t pos = name.Last('.');
   TString nameOut = name(0, pos);
   nameOut.Append("_readbmraw_Out.root");
   
   if (outFile->Open(nameOut.Data(), "RECREATE")) return;
   //~ bmActTrack->SetHistogramDir(outFile->File());
   bmActNtuRaw->SetHistogramDir(outFile->File());
   
   cout<<" Beginning the Event Loop "<<endl;
   tagr.BeginEventLoop();
   TStopwatch watch;
   watch.Start();
   
   TASTdatRaw* pstdatraw;
   TABMdatRaw* pbmdatraw;
   
   for (Long64_t ientry = 0; ientry < 1000; ientry++) {
   

      //~ if(ientry % 100 == 0)
         cout<<" Loaded Event:: " << ientry << endl;
      
      if (ientry == 100)
//      if (ientry == 20000)
         break;
      
      tagr.NextEvent();
      //~ pstdatraw = (TASTdatRaw*)(tagr.FindDataDsc("stDatRaw", "TASTdatRaw")->Object());
      //~ pstdatraw->SetTrigTime(4500.);//only for testing the data_test.00001201.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data.moved file   
      pbmdatraw = (TABMdatRaw*)(tagr.FindDataDsc("bmDat", "TABMdatRaw")->Object());
      cout<<"number of hits charged in TABMdatRaw="<<pbmdatraw->NHit()<<"  number of tdc hits discharged due to time cuts="<<pbmdatraw->NDrop()<<endl;
   }
   
   tagr.EndEventLoop();
   
   outFile->Print();
   outFile->Close();
   
   watch.Print();
}


