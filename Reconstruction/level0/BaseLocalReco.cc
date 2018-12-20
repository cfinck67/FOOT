

#include "BaseLocalReco.h"

// root include

#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"

#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TABMparGeo.hxx"

#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"
#include "TATRparGeo.hxx"

#include "TAVTparConf.hxx"
#include "TAITparConf.hxx"
#include "TAMSDparConf.hxx"

#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"

#include "TAVTntuCluster.hxx"
#include "TAITntuCluster.hxx"
#include "TAMSDntuCluster.hxx"

#include "TAVTntuTrack.hxx"
#include "TAVTntuVertex.hxx"

#include "GlobalPar.hxx"

ClassImp(BaseLocalReco)

//__________________________________________________________
BaseLocalReco::BaseLocalReco(TString fileNameIn, TString fileNameout)
 : TNamed(fileNameIn.Data(), fileNameout.Data()),
   fpParGeoTr(0x0),
   fpParGeoBm(0x0),
   fpParGeoIt(0x0),
   fpParGeoMsd(0x0),
   fpParGeoTw(0x0),
   fpParGeoCa(0x0),
   fpParGeoVtx(0x0),
   fpParConfIt(0x0),
   fpParConfVtx(0x0),
   fpNtuRawVtx(0x0),
   fpNtuClusVtx(0x0),
   fpNtuTrackVtx(0x0),
   fpNtuVtx(0x0),
   fpNtuRawIt(0x0),
   fpNtuClusIt(0x0),
   fpNtuRawMsd(0x0),
   fpNtuClusMsd(0x0),
   fActEvtWriter(0x0),
   fActClusVtx(0x0),
   fActTrackVtx(0x0),
   fActVtx(0x0),
   fActClusIt(0x0),
   fFlagTr(false),
   fFlagBm(false),
   fFlagVtx(false),
   fFlagIt(false),
   fFlagMsd(false),
   fFlagTw(false),
   fFlagCa(false),
   fFlagTree(false),
   fFlagHisto(false),
   fVtxTrackFlag(false),
   fTrackingAlgo("Full")
{
   // define TAGroot
   fTAGroot = new TAGroot();
   fActEvtWriter = new TAGactTreeWriter("locRecFile");
   
   if( GlobalPar::GetPar()->IncludeST() )           fFlagTr  = true;
   if( GlobalPar::GetPar()->IncludeBM() )           fFlagBm  = true;
   if( GlobalPar::GetPar()->IncludeInnerTracker() ) fFlagIt  = true;
   if( GlobalPar::GetPar()->IncludeVertex() )       fFlagVtx = true;
   if( GlobalPar::GetPar()->IncludeMSD() )          fFlagMsd = true;
   if( GlobalPar::GetPar()->IncludeTW() )           fFlagTw  = true;
   if( GlobalPar::GetPar()->IncludeCA() )           fFlagCa  = true;

}

//__________________________________________________________
BaseLocalReco::~BaseLocalReco()
{
   // default destructor
   if (fpParGeoVtx) delete fpParGeoVtx;
   if (fpParGeoIt)  delete fpParGeoIt;
   if (fpParGeoMsd) delete fpParGeoMsd;
   
   delete fTAGroot;
   delete fActEvtWriter;
}


//__________________________________________________________
void BaseLocalReco::BeforeEventLoop()
{
   InitParameters();
   
   OpenFileIn();
   
   CreateRawAction();
   CreateRecAction();

   AddRawRequiredItem();
   AddRecRequiredItem();
   
   OpenFileOut();
   
   fTAGroot->BeginEventLoop();
   fTAGroot->Print();
}

//__________________________________________________________
void BaseLocalReco::AfterEventLoop()
{
   fTAGroot->EndEventLoop();
   CloseFileOut();
   CloseFileIn();
}

//__________________________________________________________
void BaseLocalReco::OpenFileOut()
{
   if (fFlagTree)
      SetTreeBranches();
   
   fActEvtWriter->Open(GetTitle(), "RECREATE");
   
   if (fFlagHisto) {
      SetRawHistogramDir();
      SetRecHistogramDir();
   }
}


//__________________________________________________________
void BaseLocalReco::SetRecHistogramDir()
{
   if (fFlagVtx) {
      fActClusVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      
      if (fVtxTrackFlag) {
         fActTrackVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
         fActVtx->SetHistogramDir((TDirectory*)fActEvtWriter->File());
      }
   }
   
   // IT
   if (fFlagIt)
      fActClusIt->SetHistogramDir((TDirectory*)fActEvtWriter->File());
   
   
   // MSD
   if (fFlagMsd)
      fActClusMsd->SetHistogramDir((TDirectory*)fActEvtWriter->File());
}

//__________________________________________________________
void BaseLocalReco::CloseFileOut()
{
   fActEvtWriter->Print();
   fActEvtWriter->Close();
}

//__________________________________________________________
void BaseLocalReco::InitParameters()
{
   // initialise parameters for start counter
   if (fFlagTr) {
      fpParGeoTr = new TAGparaDsc(TATRparGeo::GetDefParaName(), new TATRparGeo());
      TATRparGeo* parGeo = (TATRparGeo*)fpParGeoTr->Object();
      TAVTparGeo* geomap  = (TAVTparGeo*) fpParGeoVtx->Object();
      geomap->FromFile("./geomaps/TATRdetector.map");
   }

   // initialise parameters for Beam Monitor
   if (fFlagBm) {
      fpParGeoBm = new TAGparaDsc("bmGeo", new TABMparGeo());
      TABMparGeo* parGeo = (TABMparGeo*)fpParGeoBm->Object();
      parGeo->InitGeo();
   }

   // initialise parameters for vertex
   if (fFlagVtx) {
      fpParGeoVtx =  new TAGparaDsc(TAVTparGeo::GetDefParaName(), new TAVTparGeo());
      TAVTparGeo* geomap  = (TAVTparGeo*) fpParGeoVtx->Object();
      geomap->FromFile("./geomaps/TAVTdetector.map");
      
      fpParConfVtx =  new TAGparaDsc("vtConf", new TAVTparConf());
      TAVTparConf* parconf = (TAVTparConf*) fpParConfVtx->Object();
      parconf->FromFile("./config/TAVTdetector.cfg");
   }
   
   // initialise parameters for inner tracker
   if (fFlagIt) {
      fpParGeoIt =new TAGparaDsc(TAITparGeo::GetDefParaName(), new TAITparGeo());
      TAITparGeo* geomap   = (TAITparGeo*) fpParGeoIt->Object();
      geomap->FromFile("./geomaps/TAITdetector.map");

      
      fpParConfIt = new TAGparaDsc("itConf", new TAITparConf());
      TAITparConf* parconf = (TAITparConf*) fpParConfIt->Object();
      parconf->FromFile("./config/TAITdetector.cfg");
   }
   
   // initialise parameters for multi strip detector
   if (fFlagMsd) {
      fpParGeoMsd = new TAGparaDsc(TAMSDparGeo::GetDefParaName(), new TAMSDparGeo());
      TAMSDparGeo* geomap   = (TAMSDparGeo*) fpParGeoMsd->Object();
      geomap->FromFile("./geomaps/TAMSDdetector.map");
      
      fpParConfMsd = new TAGparaDsc("msdConf", new TAMSDparConf());
//      TAMSDparConf* parconf = (TAMSDparConf*) fpParConfMsd->Object();
//      parconf->FromFile("./config/TAMSDdetector.cfg");
   }
   
   // initialise parameters for Tof Wall
   if (fFlagTw) {
      fpParGeoTw = new TAGparaDsc("twGeo", new TATWparGeo());
      TATWparGeo* geomap = (TATWparGeo*)fpParGeoTw->Object();
      geomap->FromFile("./geomaps/TATWdetector.map");
   }
   
   // initialise parameters for caloriomter
   if (fFlagCa) {
      fpParGeoCa = new TAGparaDsc("caGeo", new TACAparGeo());
      TACAparGeo* geomap = (TACAparGeo*)fpParGeoCa->Object();
      geomap->FromFile("./geomaps/TACAdetector.map");
   }

   TAVTparConf::SetHistoMap();
}

//__________________________________________________________
void BaseLocalReco::CreateRecAction()
{
   // VTX
   if (fFlagVtx) {
      fpNtuClusVtx = new TAGdataDsc("vtClus", new TAVTntuCluster());
      fActClusVtx  = new TAVTactNtuClusterF("vtActClus", fpNtuRawVtx, fpNtuClusVtx, fpParConfVtx, fpParGeoVtx);
      if (fFlagHisto)
         fActClusVtx->CreateHistogram();
     
      if (fVtxTrackFlag) {
         fpNtuTrackVtx = new TAGdataDsc("vtTrack", new TAVTntuTrack());
         fpNtuVtx      = new TAGdataDsc("vtVtx",   new TAVTntuVertex());
         
         if (fTrackingAlgo.Contains("Std") )
            fActTrackVtx  = new TAVTactNtuTrack("vtActTrack", fpNtuClusVtx, fpNtuTrackVtx, fpParConfVtx, fpParGeoVtx);
         else if (fTrackingAlgo.Contains("Full"))
            fActTrackVtx  = new TAVTactNtuTrackF("vtActTrack", fpNtuClusVtx, fpNtuTrackVtx, fpParConfVtx, fpParGeoVtx);
         else {
            Error("CreateRecActionVtx()", "No Tracking algorithm defined !");
         }
         if (fFlagHisto)
            fActTrackVtx->CreateHistogram();
         
         fActVtx    = new TAVTactNtuVertexPD("vtActVtx", fpNtuTrackVtx, fpNtuVtx, fpParConfVtx, fpParGeoVtx);
         if (fFlagHisto)
            fActVtx->CreateHistogram();
      }
   }
   
   // IT
   if (fFlagIt) {
      fpNtuClusIt = new TAGdataDsc("itClus", new TAITntuCluster());
      fActClusIt  = new TAITactNtuClusterF("itActClus", fpNtuRawIt, fpNtuClusIt, fpParConfIt, fpParGeoIt);
      if (fFlagHisto)
         fActClusIt->CreateHistogram();
   }
   
   if (fFlagMsd) {
      fpNtuClusMsd = new TAGdataDsc("msdClus", new TAMSDntuCluster());
      fActClusMsd  = new TAMSDactNtuCluster("msdActClus", fpNtuRawMsd, fpNtuClusMsd, fpParConfMsd, fpParGeoMsd);
      if (fFlagHisto)
         fActClusMsd->CreateHistogram();
   }
}

//__________________________________________________________
void BaseLocalReco::SetTreeBranches()
{
   if (fFlagVtx) {
      fActEvtWriter->SetupElementBranch(fpNtuClusVtx, TAVTntuCluster::GetBranchName());
      if (fVtxTrackFlag) {
         fActEvtWriter->SetupElementBranch(fpNtuTrackVtx, TAVTntuTrack::GetBranchName());
         fActEvtWriter->SetupElementBranch(fpNtuVtx, TAVTntuVertex::GetBranchName());
      }
   }
   
   if (fFlagIt)
      fActEvtWriter->SetupElementBranch(fpNtuClusIt, TAITntuCluster::GetBranchName());

   if (fFlagMsd)
      fActEvtWriter->SetupElementBranch(fpNtuClusMsd, TAMSDntuCluster::GetBranchName());
}

//__________________________________________________________
void BaseLocalReco::AddRecRequiredItem()
{
   fTAGroot->AddRequiredItem("locRecFile");
   
   if (fFlagVtx) {
      
      fTAGroot->AddRequiredItem("vtActClus");
      if (fVtxTrackFlag) {
         fTAGroot->AddRequiredItem("vtActTrack");
         fTAGroot->AddRequiredItem("vtActVtx");
      }
   }
   
   if (fFlagIt)
      fTAGroot->AddRequiredItem("itActClus");
   
   if (fFlagMsd)
      fTAGroot->AddRequiredItem("msdActClus");
}

//__________________________________________________________
void BaseLocalReco::SetTrackingAlgo(char c)
{
   switch (c) {
      case 'S':
         fTrackingAlgo = "Std";
         break;
      case 'F':
         fTrackingAlgo = "Full";
         break;
      default:
         printf("SetTrackingAlgo: Wrongly set tracking algorithm");
   }
}
