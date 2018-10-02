

#include "BaseLocalReco.hxx"

// root include

#include "TAGactTreeReader.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGparGeo.hxx"

#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"
#include "TATRparGeo.hxx"

#include "TAVTntuHit.hxx"
#include "TAITntuHit.hxx"
#include "TAMSDntuHit.hxx"

#include "TAVTntuTrack.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTntuVertex.hxx"

#include "TAVTdatRaw.hxx"
#include "TAVTactNtuRaw.hxx"
#include "TAVTactNtuCluster.hxx"
#include "TAVTactNtuTrackF.hxx"
#include "TAVTactNtuTrackH.hxx"
#include "TAVTactNtuVertexPD.hxx"


ClassImp(BaseLocalReco)

Bool_t  BaseLocalReco::fgTrackFlag    = true;

BaseLocalReco* BaseLocalReco::fgInstance = 0x0;

//__________________________________________________________
BaseLocalReco* BaseLocalReco::Instance(Int_t type, const TString name)
{
   if (fgInstance == 0x0)
      fgInstance = new BaseLocalReco(type, name);
   
   return fgInstance;
}

//__________________________________________________________
BaseLocalReco::BaseLocalReco(Int_t type, const TString expName)
 : TNamed(expName, "Local Reconstruction"),
   fpParGeoTr(0x0),
   fpParGeoBm(0x0),
   fpParGeoIt(0x0),
   fpParGeoMsd(0x0),
   fpParGeoTw(0x0),
   fpParGeoCa(0x0),
   fpParGeoVtx(0x0),
   fpParConfIt(0x0),
   fpParConfVtx(0x0),
   fpDatRawVtx(0x0),
   fpNtuRawVtx(0x0),
   fpNtuClusVtx(0x0),
   fpNtuTrackVtx(0x0),
   fpNtuVtx(0x0),
   fpDatRawIt(0x0),
   fpNtuRawIt(0x0),
   fpNtuClusIt(0x0),
   fpDatRawMsd(0x0),
   fpNtuRawMsd(0x0),
   fpNtuClusMsd(0x0),
   fActDatRawVtx(0x0),
   fActEvtReader(0x0),
   fActNtuRawVtx(0x0),
   fActClusVtx(0x0),
   fActTrackVtx(0x0),
   fActVtx(0x0),
   fActNtuRawIt(0x0),
   fActClusIt(0x0),
   fActNtuRawMsd(0x0),
   fActClusMsd(0x0)
   fFlagTr(false),
   fFlagBm(false),
   fFlagVtx(false),
   fFlagIt(false),
   fFlagMsd(false),
   fFlagTw(false),
   fFlagCa(false)
{
}

//__________________________________________________________
BaseLocalReco::~BaseLocalReco()
{
   // default destructor
   if (fpParGeoVtx) delete fpParGeoVtx;
   if (fpParGeoIt)  delete fpParGeoIt;
   if (fpParGeoG)   delete fpParGeoG;
   if (fpParGeoEm)  delete fpParGeoEm;
   
}

//__________________________________________________________
void BaseLocalReco::InitParameters()
{
   // initialise parameters for start counter
   if (fFlagTr)) {
      fpParGeoIr = new TAGparaDsc("irGeo", new TATRparGeo());
      TATRparGeo* parGeo = (TATRparGeo*)fpParGeoIr->Object();
      parGeo->InitGeo();
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
      geomap->InitGeo();
      
      fpParConfVtx =  new TAGparaDsc("vtConf", new TAVTparConf());
      TAVTparConf* parconf = (TAVTparConf*) fpParConfVtx->Object();
      parconf->FromFile("./config/TAVTdetector.cfg");
   }
   

   // initialise parameters for inner tracker
   if (fFlagIt)) {
      fpParGeoIt =new TAGparaDsc(TAITparGeo::GetDefParaName(), new TAITparGeo());
      TAITparGeo* geomap   = (TAITparGeo*) fpParGeoIt->Object();
      geomap->InitGeo();

      
      fpParConfIt = new TAGparaDsc("itConf", new TAITparConf());
      TAITparConf* parconf = (TAITparConf*) fpParConfIt->Object();
      parconf->FromFile("./config/TAITdetector.cfg");
   }
   
   // initialise parameters for multi strip detector
   if (fFlagMsd)) {
      fpParGeoMsd = new TAGparaDsc(TAMSDparGeo::GetDefParaName(), new TAMSDparGeo());
      TAMSDparGeo* geomap   = (TAMSDparGeo*) fpParGeoMsd->Object();
      geomap->InitGeo();
      
//      fpParConfMsd = = new TAGparaDsc("msdConf", new TAMSDparConf());
//      TAMSDparConf* parconf = (TAMSDparConf*) fpParConfMsd->Object();
//      parconf->FromFile("./config/TAMSDdetector.cfg");
   }
   
   // initialise parameters for Tof Wall
   if (fFlagTw) {
      fpParGeoTw = new TAGparaDsc("twGeo", new TATWparGeo());
      TATWparGeo* parGeo = (TATWparGeo*)fpParGeoTw->Object();
      parGeo->InitGeo();
   }
   
   // initialise parameters for caloriomter
   if (fFlagCa)) {
      fpParGeoCa = new TAGparaDsc("caGeo", new TACAparGeo());
      TACAparGeo* parGeo = (TACAparGeo*)fpParGeoCa->Object();
      parGeo->InitGeo();
   }

   TAVTparConf::SetHistoMap();
}

//__________________________________________________________
void BaseLocalReco::CreateRecAction()
{
   InitParameters();
   
   if (fFlagVtx))
      CreateRecActionVtx();
   
   if fFlagIt)
      CreateRecActionIt();
   
   if (fFlagMsd))
      CreateRecActionMsd();
}

//__________________________________________________________
void BaseLocalReco::CreateRecActionVtx()
{
   if(fgTrackFlag) {
      fpNtuTrackVtx = new TAGdataDsc("vtTrack", new TAVTntuTrack());
      fpNtuVtx      = new TAGdataDsc("vtVtx",   new TAVTntuVertex());
   }
   
   fpNtuClusVtx  = new TAGdataDsc("vtClus", new TAVTntuCluster());
   fActClusVtx   = new TAVTactNtuCluster("vtActClus", fpNtuRawVtx, fpNtuClusVtx, fpParConfVtx, fpParGeoVtx);
   fActClusVtx->CreateHistogram();
   
   if (fgTrackFlag) {
      if (fgTrackingAlgo.Contains("Std") )
         fActTrackVtx  = new TAVTactNtuTrack("vtActTrack", fpNtuClusVtx, fpNtuTrackVtx, fpParConfVtx, fpParGeoVtx);
      else if (fgTrackingAlgo.Contains("Full"))
         fActTrackVtx  = new TAVTactNtuTrackF("vtActTrack", fpNtuClusVtx, fpNtuTrackVtx, fpParConfVtx, fpParGeoVtx);
      else {
         Error("CreateRecActionVtx()", "No Tracking algorithm defined !");
      }
      fActTrackVtx->CreateHistogram();
      
      fActVtx    = new TAVTactNtuVertexPD("vtActVtx", fpNtuTrackVtx, fpNtuVtx, fpParConfVtx, fpParGeoVtx);
      fActVtx->CreateHistogram();
      
   }
}

//__________________________________________________________
void BaseLocalReco::CreateRecActionIt()
{
   
   fpNtuClusIt  = new TAGdataDsc("itClus", new TAVTntuCluster());
   fActClusIt   = new TAVTactNtuCluster("itActClus", fpNtuRawIt, fpNtuClusIt, fpParConfIt, fpParGeoIt);
   fActClusIt->CreateHistogram();
}

//__________________________________________________________
void BaseLocalReco::CreateRecActionMsd()
{
   fpNtuClusMsd  = new TAGdataDsc("msdClus", new TAVTntuCluster());
   fActClusMsd   = new TAVTactNtuCluster("msdActClus", fpNtuRawMsd, fpNtuClusMsd, fpParConfMsd, fpParGeoMsd);
   fActClusMsd->CreateHistogram();
}


//__________________________________________________________
void BaseLocalReco::CreateRawAction()
{
   fpNtuRawVtx = new TAGdataDsc("vtRaw", new TAVTntuHit());
   fpNtuRawIt  = new TAGdataDsc("itRaw", new TAITntuHit());
   fpNtuRawMsd = new TAGdataDsc("msdRaw", new TAMSDntuHit());
   
   if (fType == 0) {
      fpDatRawVtx   = new TAGdataDsc("vtDat", new TAVTdatRaw());
      fpDatRawIt    = new TAGdataDsc("itDat", new TAVTdatRaw());
      fpDatRawMsd   = new TAGdataDsc("msdDat", new TAVTdatRaw());
      fActNtuRawVtx = new TAVTactNtuRaw("vtActNtu", fpNtuRawVtx, fpDatRawVtx, fpParGeoVtx);
      fActNtuRawVtx->CreateHistogram();
      fActNtuRawIt = new TAVTactNtuRaw("itActNtu", fpNtuRawIt, fpDatRawIt, fpParGeoIt);
      fActNtuRawIt->CreateHistogram();
      fActNtuRawMsd = new TAVTactNtuRaw("msdActNtu", fpNtuRawMsd, fpDatRawMsd, fpParGeoMsd);
      fActNtuRawMsd->CreateHistogram();
   }
   
   if (fType == 0) { // pxi reader to be changed to MBS ?
      fActEvtReader = new TAVTactPxiReader("vtAcPxiReader", fpDatRawVtx, fpParGeoVtx, fpParConfVtx);
      
   } else if (fType == 1) { // MC reader
      fActEvtReader = new TAGactTreeReader("vtActMcReader");
      fActEvtReader->SetupBranch(fpNtuRawVtx, TAVTntuHit::GetBranchName());
      fActEvtReader->SetupBranch(fpNtuRawIt,  TAITntuHit::GetBranchName());
      fActEvtReader->SetupBranch(fpNtuRawMsd, TAMSDntuHit::GetBranchName());
      
   } else {
      Error("CreateRawAction()", "Unkown type %d\n", fType);
   }
}

//__________________________________________________________
void BaseLocalReco::OpenFile(const TString fileName)
{
   fActEvtReader->Open(fileName.Data());
}

//__________________________________________________________
void BaseLocalReco::AddRequiredItem()
{
   if (fFlagVtx))
      AddRequiredItemVtx();
   
   if (fFlagIt))
      AddRequiredItemIt();
   
   if (fFlagMsd)
      AddRequiredItemMsd();

   fAGRoot->BeginEventLoop();
   fAGRoot->Print();
}

//__________________________________________________________
void BaseLocalReco::AddRequiredItemVtx()
{
   fAGRoot->AddRequiredItem("vtActNtu");
   
   fAGRoot->AddRequiredItem("vtActClus");
   if (fgTrackFlag) {
      fAGRoot->AddRequiredItem("vtActTrack");
      if (geoTrafo->GetDeviceStatus(TAGparGeo::GetDevBaseName()))
         fAGRoot->AddRequiredItem("vtActVtx");
   }
}

//__________________________________________________________
void BaseLocalReco::AddRequiredItemIt()
{
      fAGRoot->AddRequiredItem("itActNtu");
      fAGRoot->AddRequiredItem("itActClus");
}

//__________________________________________________________
void BaseLocalReco::AddRequiredItemMsd()
{
   fAGRoot->AddRequiredItem("msdActNtu");
   fAGRoot->AddRequiredItem("msdActClus");
}
