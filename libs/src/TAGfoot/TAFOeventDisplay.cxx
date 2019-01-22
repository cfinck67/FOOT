

#include "TAFOeventDisplay.hxx"

// root include
#include "Riostream.h"
#include "TRootEmbeddedCanvas.h"
#include "TEveManager.h"
#include "TEveWindow.h"
#include "TEveBrowser.h"
#include "TEveBrowser.h"

#include "GlobalPar.hxx"
#include "TAGactTreeReader.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGparGeo.hxx"

#include "TATRntuRaw.hxx"
#include "TABMntuRaw.hxx"
#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"
#include "TATW_ContainerHit.hxx"
#include "TACAntuRaw.hxx"

#include "TAVTntuCluster.hxx"
#include "TAITntuCluster.hxx"
#include "TAMSDntuCluster.hxx"

#include "TAVTntuTrack.hxx"
#include "TAVTntuVertex.hxx"

#include "TAVTdatRaw.hxx"
#include "TAVTactNtuRaw.hxx"
#include "TAVTactNtuTrackF.hxx"
#include "TAVTactNtuTrack.hxx"
#include "TAVTactNtuVertexPD.hxx"


ClassImp(TAFOeventDisplay)

Bool_t  TAFOeventDisplay::fgTrackFlag    = true;
TString TAFOeventDisplay::fgTrackingAlgo = "Std";
Bool_t  TAFOeventDisplay::fgDrawVertex   = true;

TAFOeventDisplay* TAFOeventDisplay::fgInstance = 0x0;

//__________________________________________________________
TAFOeventDisplay* TAFOeventDisplay::Instance(Int_t type, const TString name)
{
   if (fgInstance == 0x0)
      fgInstance = new TAFOeventDisplay(type, name);
   
   return fgInstance;
}

//__________________________________________________________
TAFOeventDisplay::TAFOeventDisplay(Int_t type, const TString expName)
 : TAGbaseEventDisplay(expName),
   fpParGeoSt(0x0),
   fpParGeoG(0x0),
   fpParGeoDi(0x0),
   fpParGeoBm(0x0),
   fpParGeoIt(0x0),
   fpParGeoMsd(0x0),
   fpParGeoTw(0x0),
   fpParGeoCa(0x0),
   fpParGeoVtx(0x0),
   fpParConfIt(0x0),
   fpParConfVtx(0x0),
   fpDatRawSt(0x0),
   fpDatRawBm(0x0),
   fpNtuRawBm(0x0),
   fpDatRawVtx(0x0),
   fpNtuRawVtx(0x0),
   fpNtuClusVtx(0x0),
   fpNtuTrackVtx(0x0),
   fpNtuVtx(0x0),
   fpNtuRawIt(0x0),
   fpNtuClusIt(0x0),
   fpDatRawMsd(0x0),
   fpNtuRawMsd(0x0),
   fpNtuClusMsd(0x0),
   fpNtuRawTw(0x0),
   fActDatRawVtx(0x0),
   fActEvtReader(0x0),
   fActNtuRawVtx(0x0),
   fActClusVtx(0x0),
   fActTrackVtx(0x0),
   fActVtx(0x0),
   fActNtuRawIt(0x0),
   fActClusIt(0x0),
   fActNtuRawMsd(0x0),
   fActClusMsd(0x0),
   fType(type),
   fStClusDisplay(new TAGclusterDisplay("Start counter hit")),
   fBmClusDisplay(new TAGwireDisplay("Beam Monitoring Wires")),
   fBmTrackDisplay(new TAGtrackDisplay("Beam Monitoring Tracks")),
   fVtxClusDisplay(new TAGclusterDisplay("Vertex Cluster")),
   fVtxTrackDisplay(new TAGtrackDisplay("Vertex Tracks")),
   fItClusDisplay(new TAGclusterDisplay("Inner tracker Cluster")),
   fMsdClusDisplay(new TAGclusterDisplay("Multi Strip Cluster")),
   fTwClusDisplay(new TAGclusterDisplay("Tof Wall hit")),
   fCaClusDisplay(new TAGclusterDisplay("Calorimeter hit")),
   fGlbTrackDisplay(new TAGglbTrackDisplay("Global Tracks"))
{
   
   // default constructon
   fStClusDisplay->SetMaxEnergy(fMaxEnergy);
   fStClusDisplay->SetDefWidth(fQuadDefWidth/2.);
   fStClusDisplay->SetDefHeight(fQuadDefHeight/2.);
   fStClusDisplay->SetPickable(true);

   fBmClusDisplay->SetPickable(true);
   fBmTrackDisplay->SetMaxEnergy(fMaxEnergy);
   fBmTrackDisplay->SetDefWidth(fBoxDefWidth);
   fBmTrackDisplay->SetDefHeight(fBoxDefHeight);
   fBmTrackDisplay->SetPickable(true);

   
   fVtxClusDisplay->SetMaxEnergy(fMaxEnergy);
   fVtxClusDisplay->SetDefWidth(fQuadDefWidth/2.);
   fVtxClusDisplay->SetDefHeight(fQuadDefHeight/2.);
   fVtxClusDisplay->SetPickable(true);
   
   fVtxTrackDisplay->SetMaxEnergy(fMaxEnergy);
   fVtxTrackDisplay->SetDefWidth(fBoxDefWidth);
   fVtxTrackDisplay->SetDefHeight(fBoxDefHeight);
   fVtxTrackDisplay->SetPickable(true);
   
   fItClusDisplay->SetMaxEnergy(fMaxEnergy);
   fItClusDisplay->SetDefWidth(fQuadDefWidth);
   fItClusDisplay->SetDefHeight(fQuadDefHeight);
   fItClusDisplay->SetPickable(true);
   
   fMsdClusDisplay->SetMaxEnergy(fMaxEnergy);
   fMsdClusDisplay->SetDefWidth(fQuadDefWidth);
   fMsdClusDisplay->SetDefHeight(fQuadDefHeight);
   fMsdClusDisplay->SetPickable(true);
   
   fTwClusDisplay->SetMaxEnergy(fMaxEnergy);
   fTwClusDisplay->SetDefWidth(fQuadDefWidth*4);
   fTwClusDisplay->SetDefHeight(fQuadDefHeight*4);
   fTwClusDisplay->SetPickable(true);
   
   fCaClusDisplay->SetMaxEnergy(fMaxEnergy);
   fCaClusDisplay->SetDefWidth(fQuadDefWidth*4);
   fCaClusDisplay->SetDefHeight(fQuadDefHeight*4);
   fCaClusDisplay->SetPickable(true);
   
   fGlbTrackDisplay->SetMaxMomentum(fMaxMomentum);
   
   GlobalPar::Instance();
}

//__________________________________________________________
TAFOeventDisplay::~TAFOeventDisplay()
{
   // default destructor
   if (fpParGeoSt)  delete fpParGeoSt;
   if (fpParGeoBm)  delete fpParGeoBm;
   if (fpParGeoVtx) delete fpParGeoVtx;
   if (fpParGeoIt)  delete fpParGeoIt;
   if (fpParGeoMsd) delete fpParGeoMsd;
   if (fpParGeoTw)  delete fpParGeoTw;
   if (fpParGeoCa)  delete fpParGeoCa;
   if (fpParGeoG)   delete fpParGeoG;
   if (fpParGeoDi)  delete fpParGeoDi;
   
   delete fStClusDisplay;
   delete fBmClusDisplay;
   delete fBmTrackDisplay;
   delete fVtxClusDisplay;
   delete fVtxTrackDisplay;
   delete fItClusDisplay;
   delete fMsdClusDisplay;
   delete fTwClusDisplay;
   delete fCaClusDisplay;
   
   delete fGlbTrackDisplay;
   
   if (fField) delete fField;
   if (fFieldImpl) delete fFieldImpl;
}

//__________________________________________________________
void TAFOeventDisplay::ReadParFiles()
{
   // initialise par files for target
   if (GlobalPar::GetPar()->IncludeTG() || GlobalPar::GetPar()->IncludeBM()) {
      fpParGeoG = new TAGparaDsc(TAGparGeo::GetDefParaName(), new TAGparGeo());
      TAGparGeo* parGeo = (TAGparGeo*)fpParGeoG->Object();
      TString parFileName = Form("./geomaps/TAGdetector%s.map", fExpName.Data());
      parGeo->FromFile(parFileName.Data());
   }
   
   // initialise par files for start counter
   if (GlobalPar::GetPar()->IncludeST()) {
      fpParGeoSt = new TAGparaDsc(TATRparGeo::GetDefParaName(), new TATRparGeo());
      TATRparGeo* parGeo = (TATRparGeo*)fpParGeoSt->Object();
      TString parFileName = Form("./geomaps/TATRdetector%s.map", fExpName.Data());
      parGeo->FromFile(parFileName.Data());
   }

   // initialise par files for Beam Monitor
   if (GlobalPar::GetPar()->IncludeBM()) {
      fpParGeoBm = new TAGparaDsc("bmGeo", new TABMparGeo());
      TABMparGeo* parGeo = (TABMparGeo*)fpParGeoBm->Object();
      TString parFileName = Form("./geomaps/TABMdetector%s.map", fExpName.Data());
      parGeo->FromFile(parFileName.Data());
      
      fpParConfBm = new TAGparaDsc("bmConf", new TABMparCon());
      TABMparCon* parConf = (TABMparCon*)fpParConfBm->Object();
      parFileName = Form("./config/beammonitor%s.cfg", fExpName.Data());
      parConf->FromFile(parFileName.Data());
      
      parFileName = "./config/bmreso_vs_r.root";
      parConf->LoadReso(parFileName);
   }

   // initialise par files for vertex
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpParGeoVtx = new TAGparaDsc(TAVTparGeo::GetDefParaName(), new TAVTparGeo());
      TAVTparGeo* parGeo = (TAVTparGeo*)fpParGeoVtx->Object();
      TString parVtxFileName = Form("./geomaps/TAVTdetector%s.map", fExpName.Data());
      parGeo->FromFile(parVtxFileName.Data());
      
      fpParConfVtx = new TAGparaDsc("vtConf", new TAVTparConf());
      TAVTparConf* parConf = (TAVTparConf*)fpParConfVtx->Object();
      parVtxFileName = Form("./config/TAVTdetector%s.cfg", fExpName.Data());
      parConf->FromFile(parVtxFileName.Data());
   }
   
   // initialise par files for Magnet
   if (GlobalPar::GetPar()->IncludeDI()) {
      fpParGeoDi = new TAGparaDsc("diGeo", new TADIparGeo());
      TADIparGeo* parGeo = (TADIparGeo*)fpParGeoDi->Object();
      TString parFileName = Form("./geomaps/TADIdetector%s.map", fExpName.Data());
      parGeo->FromFile(parFileName.Data());
      
      if (GlobalPar::GetPar()->IncludeKalman()) {
         const Char_t* fieldFileName = parGeo->GetMapName().Data();
         fFieldImpl  = new FootField(fieldFileName);
         fField = new TADIeveField(fFieldImpl);
         fGlbTrackDisplay->GetPropagator()->SetMagFieldObj(fField);
         fGlbTrackDisplay->GetPropagator()->SetMaxZ(fWorldSizeZ);
         fGlbTrackDisplay->GetPropagator()->SetMaxR(fWorldSizeXY);
      }
   }

   // initialise par files for inner tracker
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fpParGeoIt = new TAGparaDsc(TAITparGeo::GetDefParaName(), new TAITparGeo());
      TAITparGeo* parGeo = (TAITparGeo*)fpParGeoIt->Object();
      TString parItFileName = Form("./geomaps/TAITdetector%s.map", fExpName.Data());
      parGeo->FromFile(parItFileName.Data());
      
      fpParConfIt = new TAGparaDsc("itConf", new TAITparConf());
      TAITparConf* parConf = (TAITparConf*)fpParConfIt->Object();
      parItFileName = Form("./config/TAITdetector%s.cfg", fExpName.Data());
      parConf->FromFile(parItFileName.Data());
   }
   
   // initialise par files for multi strip detector
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fpParGeoMsd = new TAGparaDsc(TAMSDparGeo::GetDefParaName(), new TAMSDparGeo());
      TAMSDparGeo* parGeo = (TAMSDparGeo*)fpParGeoMsd->Object();
      TString parMsdFileName = Form("./geomaps/TAMSDdetector%s.map", fExpName.Data());
      parGeo->FromFile(parMsdFileName.Data());
      
      fpParConfMsd = new TAGparaDsc("msdConf", new TAMSDparConf());
      TAMSDparConf* parConf = (TAMSDparConf*)fpParConfMsd->Object();
      parMsdFileName = Form("./config/TAMSDdetector%s.cfg", fExpName.Data());
     // parConf->FromFile(parMsdFileName.Data());
   }
   
   // initialise par files for Tof Wall
   if (GlobalPar::GetPar()->IncludeTW()) {
      fpParGeoTw = new TAGparaDsc(TATWparGeo::GetDefParaName(), new TATWparGeo());
      TATWparGeo* parGeo = (TATWparGeo*)fpParGeoTw->Object();
      TString parFileName = Form("./geomaps/TATWdetector%s.map", fExpName.Data());
      parGeo->FromFile(parFileName);
   }
   
   // initialise par files for caloriomter
   if (GlobalPar::GetPar()->IncludeCA()) {
      fpParGeoCa = new TAGparaDsc(TACAparGeo::GetDefParaName(), new TACAparGeo());
      TACAparGeo* parGeo = (TACAparGeo*)fpParGeoCa->Object();
      TString parFileName = Form("./geomaps/TACAdetector%s.map", fExpName.Data());
      parGeo->FromFile(parFileName);
   }

   TAVTparConf::SetHistoMap();
}

//__________________________________________________________
void TAFOeventDisplay::BuildDefaultGeometry()
{
   TAGbaseEventDisplay::BuildDefaultGeometry();
   
   // ST
   if (GlobalPar::GetPar()->IncludeST()) {
      TATRparGeo* parGeo = (TATRparGeo*)fpParGeoSt->Object();
      TGeoVolume* irVol  = parGeo->BuildStartCounter();
   
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TATRparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(irVol, transf);
   }

   // BM
   if (GlobalPar::GetPar()->IncludeBM()) {
      TABMparGeo* parGeo = (TABMparGeo*)fpParGeoBm->Object();
      TGeoVolume* bmVol  = parGeo->BuildBeamMonitor();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TABMparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(bmVol, transf);
   }

   // target
   if (GlobalPar::GetPar()->IncludeTG()) {
      TAGparGeo* parGeo = (TAGparGeo*)fpParGeoG->Object();
      TGeoVolume* tgVol = parGeo->BuildTarget();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TAGparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(tgVol, transf);
   }

   // Vertex
   if (GlobalPar::GetPar()->IncludeVertex()) {
      TAVTparGeo* parGeo = (TAVTparGeo*)fpParGeoVtx->Object();
      TGeoVolume* vtVol  = parGeo->BuildVertex();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TAVTparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(vtVol, transf);
   }

   // Magnet
   if (GlobalPar::GetPar()->IncludeDI()) {
      TADIparGeo* parGeo = (TADIparGeo*)fpParGeoDi->Object();
      TGeoVolume* vtVol = parGeo->BuildMagnet();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TADIparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(vtVol, transf);
   }

   // IT
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      TAITparGeo* parGeo = (TAITparGeo*)fpParGeoIt->Object();
      TGeoVolume* itVol  = parGeo->BuildInnerTracker();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TAITparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(itVol, transf);
   }
   
   // MSD
   if (GlobalPar::GetPar()->IncludeMSD()) {
      TAMSDparGeo* parGeo = (TAMSDparGeo*)fpParGeoMsd->Object();
      TGeoVolume* msdVol = parGeo->BuildMultiStripDetector();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TAMSDparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(msdVol, transf);
   }

   // TW
   if (GlobalPar::GetPar()->IncludeTW()) {
      TATWparGeo* parGeo = (TATWparGeo*)fpParGeoTw->Object();
      TGeoVolume* twVol = parGeo->BuildTofWall();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TATWparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(twVol, transf);
   }

   // CA
   if (GlobalPar::GetPar()->IncludeCA()) {
      TACAparGeo* parGeo = (TACAparGeo*)fpParGeoCa->Object();
      TGeoVolume* caVol = parGeo->BuildCalorimeter();
      
      const TGeoHMatrix* transfo = fpFootGeo->GetTrafo(TACAparGeo::GetBaseName());
      TGeoHMatrix* transf        = (TGeoHMatrix*)transfo->Clone();
      AddGeometry(caVol, transf);
   }
}

//__________________________________________________________
void TAFOeventDisplay::CreateRecAction()
{
   if (GlobalPar::GetPar()->IncludeBM())
      CreateRecActionBm();

   if (GlobalPar::GetPar()->IncludeVertex())
      CreateRecActionVtx();
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      CreateRecActionIt();
   
   if (GlobalPar::GetPar()->IncludeMSD())
      CreateRecActionMsd();
}

//__________________________________________________________
void TAFOeventDisplay::CreateRecActionBm()
{
   if(fgTrackFlag) {
      fpNtuTrackBm = new TAGdataDsc("bmTrack", new TABMntuTrack());
      fActTrackBm  = new TABMactNtuTrack("bmActTrack", fpNtuTrackBm, fpNtuRawBm, fpParGeoBm, fpParConfBm, fpParGeoG);
   }
}

//__________________________________________________________
void TAFOeventDisplay::CreateRecActionVtx()
{
   if(fgTrackFlag) {
      fpNtuTrackVtx = new TAGdataDsc("vtTrack", new TAVTntuTrack());
      if (GlobalPar::GetPar()->IncludeTG())
         fpNtuVtx      = new TAGdataDsc("vtVtx",   new TAVTntuVertex());
   }
   
   fpNtuClusVtx  = new TAGdataDsc("vtClus", new TAVTntuCluster());
   fActClusVtx   = new TAVTactNtuClusterF("vtActClus", fpNtuRawVtx, fpNtuClusVtx, fpParConfVtx, fpParGeoVtx);
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
      
      if (GlobalPar::GetPar()->IncludeTG()) {
         fActVtx    = new TAVTactNtuVertexPD("vtActVtx", fpNtuTrackVtx, fpNtuVtx, fpParConfVtx, fpParGeoVtx, fpParGeoG);
         fActVtx->CreateHistogram();
      }
   }
}

//__________________________________________________________
void TAFOeventDisplay::CreateRecActionIt()
{
   
   fpNtuClusIt  = new TAGdataDsc("itClus", new TAITntuCluster());
   fActClusIt   = new TAITactNtuClusterF("itActClus", fpNtuRawIt, fpNtuClusIt, fpParConfIt, fpParGeoIt);
   fActClusIt->CreateHistogram();
}

//__________________________________________________________
void TAFOeventDisplay::CreateRecActionMsd()
{
   fpNtuClusMsd  = new TAGdataDsc("msdClus", new TAMSDntuCluster());
   fActClusMsd   = new TAMSDactNtuCluster("msdActClus", fpNtuRawMsd, fpNtuClusMsd, fpParConfMsd, fpParGeoMsd);
   fActClusMsd->CreateHistogram();
}


//__________________________________________________________
void TAFOeventDisplay::CreateRawAction()
{
   ReadParFiles();

   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpNtuRawVtx   = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      fpDatRawVtx   = new TAGdataDsc("vtDat", new TAVTdatRaw());
      fActNtuRawVtx = new TAVTactNtuRaw("vtActNtu", fpNtuRawVtx, fpDatRawVtx, fpParGeoVtx);
      fActNtuRawVtx->CreateHistogram();
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fpDatRawIt   = new TAGdataDsc("itDat", new TAVTdatRaw());
      fpNtuRawIt   = new TAGdataDsc("itRaw", new TAITntuRaw());
      fActNtuRawIt = new TAITactNtuRaw("itActNtu", fpNtuRawIt, fpDatRawIt, fpParGeoIt);
      fActNtuRawIt->CreateHistogram();
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fpDatRawMsd   = new TAGdataDsc("msdDat", new TAVTdatRaw());
      fpNtuRawMsd   = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
      fActNtuRawMsd = new TAVTactNtuRaw("msdActNtu", fpNtuRawMsd, fpDatRawMsd, fpParGeoMsd);
      fActNtuRawMsd->CreateHistogram();
   }
   
//   if(GlobalPar::GetPar()->IncludeTW()) {
//      fpDatRawTw   = new TAGdataDsc("twdDat", new TATWdatRaw());
//      fpNtuRawTw   = new TAGdataDsc("twRaw", new TATWntuRaw());
//      fActNtuRawTw = new TAVTactNtuRaw("twActNtu", fpNtuRawTw, fpNtuRawTw, fpParGeoTw);
//      fActNtuRawTw->CreateHistogram();
//   }
   
//   if(GlobalPar::GetPar()->IncludeCA()) {
//      fpDatRawCa   = new TAGdataDsc("cadDat", new TACAdatRaw());
//      fpNtuRawCa   = new TAGdataDsc("caRaw", new TACAntuRaw());
//      fActNtuRawCa = new TAVTactNtuRaw("caActNtu", fpNtuRawCa, fpNtuRawCa, fpParGeoCa);
//      fActNtuRawCa->CreateHistogram();
//   }
}

//__________________________________________________________
void TAFOeventDisplay::OpenFile(const TString fileName)
{
   fActEvtReader->Open(fileName.Data());
}

//__________________________________________________________
void TAFOeventDisplay::ResetHistogram()
{
   TList* histoList  = 0x0;
   
   if (GlobalPar::GetPar()->IncludeVertex()) {
      histoList = fActClusVtx->GetHistogrammList();
      for (Int_t i = 0; i < histoList->GetEntries(); ++i) {
         TH1* h = (TH1*)histoList->At(i);
         h->Reset();
      }
      
      if (fgTrackFlag) {
         histoList = fActTrackVtx->GetHistogrammList();
         for (Int_t i = 0; i < histoList->GetEntries(); ++i) {
            TH1* h = (TH1*)histoList->At(i);
            h->Reset();
         }
      }
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      
      histoList = fActClusIt->GetHistogrammList();
      for (Int_t i = 0; i < histoList->GetEntries(); ++i) {
         TH1* h = (TH1*)histoList->At(i);
         h->Reset();
      }
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      
      histoList = fActClusMsd->GetHistogrammList();
      for (Int_t i = 0; i < histoList->GetEntries(); ++i) {
         TH1* h = (TH1*)histoList->At(i);
         h->Reset();
      }
   }
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItem()
{
   if (GlobalPar::GetPar()->IncludeST())
      AddRequiredItemSt();
   
   if (GlobalPar::GetPar()->IncludeBM())
      AddRequiredItemBm();

   if (GlobalPar::GetPar()->IncludeVertex())
      AddRequiredItemVtx();
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      AddRequiredItemIt();
   
   if (GlobalPar::GetPar()->IncludeMSD())
      AddRequiredItemMsd();
   
   if (GlobalPar::GetPar()->IncludeTW())
      AddRequiredItemTw();
   
   if (GlobalPar::GetPar()->IncludeCA())
      AddRequiredItemCa();

   fAGRoot->BeginEventLoop();
   fAGRoot->Print();
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItemSt()
{
   fAGRoot->AddRequiredItem("stActNtu");
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItemBm()
{
   fAGRoot->AddRequiredItem("bmActNtu");
   if (fgTrackFlag)
      fAGRoot->AddRequiredItem("bmActTrack");
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItemVtx()
{
   fAGRoot->AddRequiredItem("vtActNtu");
   
   if (fgDisplayFlag) {
      fAGRoot->AddRequiredItem("vtActClus");
      if (fgTrackFlag) {
         fAGRoot->AddRequiredItem("vtActTrack");
         if (GlobalPar::GetPar()->IncludeTG())
            fAGRoot->AddRequiredItem("vtActVtx");
      }
   }
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItemIt()
{
   fAGRoot->AddRequiredItem("itActNtu");
   
   if (fgDisplayFlag) {
      fAGRoot->AddRequiredItem("itActClus");
   }
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItemMsd()
{
   fAGRoot->AddRequiredItem("msdActNtu");
   
   if (fgDisplayFlag) {
      fAGRoot->AddRequiredItem("msdActClus");
   }
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItemTw()
{
   fAGRoot->AddRequiredItem("twActNtu");
}

//__________________________________________________________
void TAFOeventDisplay::AddRequiredItemCa()
{
   fAGRoot->AddRequiredItem("caActNtu");
}

//__________________________________________________________
void TAFOeventDisplay::AddElements()
{
   fStClusDisplay->ResetHits();
   gEve->AddElement(fStClusDisplay);
   
   fBmClusDisplay->ResetWires();
   gEve->AddElement(fBmClusDisplay);

   fBmTrackDisplay->ResetTracks();
   gEve->AddElement(fBmTrackDisplay);

   fVtxClusDisplay->ResetHits();
   gEve->AddElement(fVtxClusDisplay);
	  
   fVtxTrackDisplay->ResetTracks();
   gEve->AddElement(fVtxTrackDisplay);
   
   fItClusDisplay->ResetHits();
   gEve->AddElement(fItClusDisplay);
   
   fMsdClusDisplay->ResetHits();
   gEve->AddElement(fMsdClusDisplay);
   
   fTwClusDisplay->ResetHits();
   gEve->AddElement(fTwClusDisplay);
   
   fCaClusDisplay->ResetHits();
   gEve->AddElement(fCaClusDisplay);
   
   fGlbTrackDisplay->ResetTracks();
   gEve->AddElement(fGlbTrackDisplay);
}

//__________________________________________________________
void TAFOeventDisplay::ConnectElements()
{
   fStClusDisplay->SetEmitSignals(true);
   fStClusDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAFOeventDisplay", this, "UpdateHitInfo(TEveDigitSet*, Int_t)");

   fBmTrackDisplay->SetEmitSignals(true);
   fBmTrackDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAFOeventDisplay", this, "UpdateTrackInfo(TEveDigitSet*, Int_t)");

   fVtxClusDisplay->SetEmitSignals(true);
   fVtxClusDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAFOeventDisplay", this, "UpdateHitInfo(TEveDigitSet*, Int_t)");
   
   fVtxTrackDisplay->SetEmitSignals(true);
   fVtxTrackDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAFOeventDisplay", this, "UpdateTrackInfo(TEveDigitSet*, Int_t)");
   
   fItClusDisplay->SetEmitSignals(true);
   fItClusDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAFOeventDisplay", this, "UpdateHitInfo(TEveDigitSet*, Int_t)");
   
   fMsdClusDisplay->SetEmitSignals(true);
   fMsdClusDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAFOeventDisplay", this, "UpdateHitInfo(TEveDigitSet*, Int_t)");
}

//__________________________________________________________
void TAFOeventDisplay::UpdateHitInfo(TEveDigitSet* qs, Int_t idx)
{
   TAGclusterDisplay* quadHits = dynamic_cast<TAGclusterDisplay*> (qs);
   TObject* obj = quadHits->GetId(idx);
   
   if(obj == 0x0) return;
   
   if (obj->InheritsFrom("TAVTbaseCluster")) {
      TAVTcluster* clus = (TAVTcluster*)obj;
      if (clus == 0x0) return;
      TVector3 pos = clus->GetPositionG();
      fInfoView->AddLine( Form("Cluster # %3d\n", idx) );
      fInfoView->AddLine( Form("with %3d pixels in sensor %d\n", clus->GetPixelsN(), clus->GetPlaneNumber()) );
      fInfoView->AddLine( Form("at position: (%.3g %.3g) cm\n", pos.X(), pos.Y()) );
      
   } else if (obj->InheritsFrom("TAVTvertex")) {
      TAVTvertex* vtx = (TAVTvertex*)obj;
      if (vtx == 0x0) return;
      TVector3 pos = vtx->GetVertexPosition();
      fInfoView->AddLine( Form("Vertex# %d at position:\n", idx) );
      fInfoView->AddLine( Form(" (%.3g %.3gf %.3gf) cm\n", pos.X(), pos.Y(), pos.Z()) );
      fInfoView->AddLine( Form(" BM Matched %d\n", vtx->IsBmMatched()) );
      
   } else if (obj->InheritsFrom("TATRntuHit")) {
      TATRntuHit* hit = (TATRntuHit*)obj;
      fInfoView->AddLine( Form("Charge: %.3g u.a.\n", hit->GetCharge()) );
      fInfoView->AddLine( Form("Time: %.3g ps \n", hit->GetTime()) );

   } else {
      return;
   }
}

//__________________________________________________________
void TAFOeventDisplay::UpdateTrackInfo(TEveDigitSet* qs, Int_t idx)
{
   TAGtrackDisplay* lineTracks = dynamic_cast<TAGtrackDisplay*> (qs);
   TObject* obj = lineTracks->GetId(idx);
   if (obj == 0x0) return;
   
   if (obj->InheritsFrom("TAVTtrack")) {
      
      TAVTtrack* track =  (TAVTtrack*)obj;
      if (track == 0x0) return;

      fInfoView->AddLine( Form("Track # %2d:", track->GetNumber()) );
      fInfoView->AddLine( Form(" with %3d clusters\n", track->GetClustersN()) );
      
      for (Int_t i = 0; i < track->GetClustersN(); i++) {
         TAVTcluster* clus = track->GetCluster(i);
         TVector3 posG = clus->GetPositionG();
         fInfoView->AddLine( Form(" for plane %d\n", clus->GetPlaneNumber()));
         fInfoView->AddLine( Form(" at position: (%.3g %.3g) \n", posG.X(), posG.Y()) );
         fInfoView->AddLine( Form(" with %d pixels\n", clus->GetPixelsN()));
      }
      
   } else if (obj->InheritsFrom("TABMntuTrackTr")) {
      TABMntuTrackTr* track =  (TABMntuTrackTr*)obj;
      if (track == 0x0) return;

      fInfoView->AddLine( Form("Track:") );
      fInfoView->AddLine( Form(" with %3d hit\n", track->GetNhit()) );
      fInfoView->AddLine( Form(" at Pvers: (%.3g %.3g) \n", track->GetPvers()[0], track->GetPvers()[1]) );
      fInfoView->AddLine( Form(" and R0 (%.3g %.3g)\n",  track->GetR0()[0], track->GetR0()[1]));
   }
}

//__________________________________________________________
void TAFOeventDisplay::UpdateElements()
{
   if (fgGUIFlag)
      fEventEntry->SetText(Form("Run %d Event %d", fAGRoot->CurrentRunInfo().RunNumber(), fAGRoot->CurrentEventId().EventNumber()));
   
   if (GlobalPar::GetPar()->IncludeST())
      UpdateElements("st");
   
   if (GlobalPar::GetPar()->IncludeBM())
      UpdateElements("bm");

   if (GlobalPar::GetPar()->IncludeVertex())
      UpdateElements("vt");
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      UpdateElements("it");
   
   if (GlobalPar::GetPar()->IncludeMSD())
      UpdateElements("ms");
   
   if (GlobalPar::GetPar()->IncludeTW())
      UpdateElements("tw");
   
   if (GlobalPar::GetPar()->IncludeCA())
      UpdateElements("ca");

   gEve->FullRedraw3D(kFALSE);
}

//__________________________________________________________
void TAFOeventDisplay::UpdateElements(const TString prefix)
{
   if (prefix == "tw")
      UpdateBarElements();
   else if (prefix == "ca")
      UpdateCrystalElements();
   else if (prefix == "st")
      UpdateStcElements();
   else if (prefix == "bm") {
      UpdateWireElements();
      if (fgTrackFlag)
         UpdateTrackElements(prefix);
   } else {
      UpdateQuadElements(prefix);
      if (fgTrackFlag) {
         UpdateTrackElements(prefix);
         if (GlobalPar::GetPar()->IncludeKalman())
            UpdateGlbTrackElements();
      }
   }
}

//__________________________________________________________
void TAFOeventDisplay::UpdateQuadElements(const TString prefix)
{
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
      if (prefix == "vt") {
         fVtxClusDisplay->ResetHits();
      }  else if (prefix == "it") {
         fItClusDisplay->ResetHits();
      }  else if (prefix == "ms") {
         fMsdClusDisplay->ResetHits();
      } else
         return;
   }

   if (!fgDisplayFlag) // do not update event display
      return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   Float_t x1 = 0.,  y1 =0., z1 = 0.;
   
   TAVTbaseParGeo* parGeo = 0x0;
   
   if (prefix == "vt")
      parGeo = (TAVTparGeo*) fpParGeoVtx->Object();
   else if (prefix == "it")
      parGeo = (TAITparGeo*) fpParGeoIt->Object();
   else if (prefix == "ms")
      parGeo = (TAMSDparGeo*) fpParGeoMsd->Object();

   
   Int_t nPlanes = parGeo->GetNSensors();
   
   TAVTntuTrack*  pNtuTrack = 0x0;
   
   if (prefix == "vt") {
      if (fgTrackFlag && GlobalPar::GetPar()->IncludeTG()) {
         // vertex
         if (fgDrawVertex) {
            pNtuTrack = (TAVTntuTrack*)  fpNtuTrackVtx->Object();
            TAVTvertex*    vtxPD   = 0x0;//NEW
            TVector3 vtxPositionPD = pNtuTrack->GetBeamPosition();
            
            if (fpNtuVtx->Valid()) {
               TAVTntuVertex* pNtuVtxPD = (TAVTntuVertex*) fpNtuVtx->Object();
               for (Int_t iVtx = 0; iVtx < pNtuVtxPD->GetVertexN(); ++iVtx) {
                  vtxPD = pNtuVtxPD->GetVertex(iVtx);
                  if (vtxPD == 0x0) continue;
                  vtxPositionPD = vtxPD->GetVertexPosition();
                  vtxPositionPD = fpFootGeo->FromVTLocalToGlobal(vtxPositionPD);
                  fVtxClusDisplay->AddHit(50, vtxPositionPD.X(), vtxPositionPD.Y(), vtxPositionPD.Z());
                  fVtxClusDisplay->QuadId(vtxPD);
               }
            }
         }
      }
   }
   
   TAVTntuCluster* pNtuClus  =  0x0;
   
   if (prefix == "vt")
      pNtuClus = (TAVTntuCluster*) fpNtuClusVtx->Object();
   else if (prefix == "it")
      pNtuClus = (TAVTntuCluster*) fpNtuClusIt->Object();
   else if (prefix == "ms")
      pNtuClus = (TAVTntuCluster*) fpNtuClusMsd->Object();

   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
      
      Int_t nclus = pNtuClus->GetClustersN(iPlane);
      
      if (nclus == 0) continue;
      for (Int_t iClus = 0; iClus < nclus; ++iClus) {
         TAVTcluster *clus = pNtuClus->GetCluster(iPlane, iClus);
         if (!clus->IsValid()) continue;
         TVector3 pos = clus->GetPositionG();
         TVector3 posG = pos;
         Int_t nPix = clus->GetListOfPixels()->GetEntries();
         if (prefix == "vt")
            posG = fpFootGeo->FromVTLocalToGlobal(posG);
         else if (prefix == "it")
            posG = fpFootGeo->FromITLocalToGlobal(posG);
         else if (prefix == "ms")
            posG = fpFootGeo->FromMSDLocalToGlobal(posG);


         x = posG(0);
         y = posG(1);
         z = posG(2);
         
         if (prefix == "vt") {
            fVtxClusDisplay->AddHit(nPix*10, x, y, z);
            fVtxClusDisplay->QuadId(clus);
         } else if (prefix == "it") {
            fItClusDisplay->AddHit(nPix*10, x, y, z);
            fItClusDisplay->QuadId(clus);
         } else if (prefix == "ms") {
            fMsdClusDisplay->AddHit(nPix*10, x, y, z);
            fMsdClusDisplay->QuadId(clus);
         }

      } //end loop on hits
      
   } //end loop on planes
   
   if (prefix == "vt")
      fVtxClusDisplay->RefitPlex();
   else if (prefix == "it")
      fItClusDisplay->RefitPlex();
   else if (prefix == "ms")
      fMsdClusDisplay->RefitPlex();
}

//__________________________________________________________
void TAFOeventDisplay::UpdateTrackElements(const TString prefix)
{
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
      if (prefix == "bm")
         fBmTrackDisplay->ResetTracks();

      if (prefix == "vt")
         fVtxTrackDisplay->ResetTracks();
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   Float_t x1 = 0.,  y1 =0., z1 = 0.;
   
   
   if (prefix == "vt") {
      
      TAVTparGeo*  parGeo = (TAVTparGeo*) fpParGeoVtx->Object();
      
      Int_t nPlanes         = parGeo->GetNSensors();
      Float_t posfirstPlane = parGeo->GetSensorPosition(0)[2];
      Float_t posLastPlane  = parGeo->GetSensorPosition(nPlanes-1)[2];
      
      TAVTntuTrack*  pNtuTrack = (TAVTntuTrack*)  fpNtuTrackVtx->Object();
      
      if( pNtuTrack->GetTracksN() > 0 ) {
         for( Int_t iTrack = 0; iTrack < pNtuTrack->GetTracksN(); ++iTrack ) {
            fVtxTrackDisplay->AddNewTrack();
            
            TAVTtrack* track = pNtuTrack->GetTrack(iTrack);
            TVector3 pos;
            TVector3 posG;
            
            if (fgDrawVertex && GlobalPar::GetPar()->IncludeTG())
               pos = track->Intersection(track->GetVertexZ());
            else
               pos = track->Intersection(posfirstPlane);
            
            posG = fpFootGeo->FromVTLocalToGlobal(pos);
            
            x = posG(0); y = posG(1); z = posG(2)*0.9;
            
            pos = track->Intersection(posLastPlane);
            posG = fpFootGeo->FromVTLocalToGlobal(pos);
            
            x1 = posG(0); y1 = posG(1); z1 = posG(2)*1.1;
            
            Int_t nClus = track->GetClustersN();
            fVtxTrackDisplay->AddTracklet(nClus*100, x, y, z, x1, y1, z1);
            fVtxTrackDisplay->TrackId(track);
            
         } // end loop on tracks
         
      } // nTracks > 0
      fVtxTrackDisplay->RefitPlex();
   }
   
   if (prefix == "bm") {
      TABMparGeo*  parGeo = (TABMparGeo*) fpParGeoBm->Object();

      TABMntuTrack*  pNtuTrack = (TABMntuTrack*)  fpNtuTrackBm->Object();
      
      if( pNtuTrack->GetTracksN() > 0 ) {
         
         for( Int_t iTrack = 0; iTrack < pNtuTrack->GetTracksN(); ++iTrack ) {
               fBmTrackDisplay->AddNewTrack();
            
            TABMntuTrackTr* track = pNtuTrack->Track(iTrack);
            TVector3 A0 = track->PointAtLocalZ(-parGeo->GetLength());
            TVector3 A1 = track->PointAtLocalZ(+parGeo->GetLength());
            
            TVector3 A0G = fpFootGeo->FromBMLocalToGlobal(A0);
            TVector3 A1G = fpFootGeo->FromBMLocalToGlobal(A1);
            
            x  = A0G(0); y  = A0G(1); z  = A0G(2)*1.1;
            x1 = A1G(0); y1 = A1G(1); z1 = A1G(2)*1.1;
            
            Int_t nHits = track->GetNhit();
            fBmTrackDisplay->AddTracklet(nHits*100, x, y, z, x1, y1, z1);
            fBmTrackDisplay->TrackId(track);
            
         } // end loop on tracks
         
      } // nTracks > 0
      fBmTrackDisplay->RefitPlex();
   }
}

//__________________________________________________________
void TAFOeventDisplay::UpdateBarElements()
{
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
         fTwClusDisplay->ResetHits();
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   
   TATWparGeo* parGeo = parGeo = (TATWparGeo*) fpParGeoTw->Object();

   // reset previous fired bar
   if ( fRefreshButton->IsOn()) {
      map< pair<int, int>, int >::iterator it;
      for (it = fFiredTofBar.begin(); it != fFiredTofBar.end(); it++) {
         pair<int, int> idx = it->first;
         Int_t iBar = idx.first;
         Int_t iLayer = idx.second;
         parGeo->SetBarColorOff(iBar, iLayer);
      }
   }

   fFiredTofBar.clear();

   TATW_ContainerHit* pNtuHit = (TATW_ContainerHit*) fpNtuRawTw->Object();
   
   for( Int_t iLayer = 0; iLayer < 2; iLayer++) {
      

      Int_t nHits = pNtuHit->GetHitN(iLayer);
      if (nHits == 0) continue;

      for (Int_t iHit = 0; iHit < nHits; ++iHit) {
         
         TATW_Hit *hit = pNtuHit->GetHit(iLayer, iHit);

         Int_t iBar = hit->GetBar();
        // printf("re %d %d\n", iLayer, iBar);

         Int_t layer = 0;
         
         if (iLayer == 0) layer = 1;
         if (iLayer == 1) layer = 0;
         
         pair<int, int> idx(iBar, layer);
         
         fFiredTofBar[idx] = 1;
         parGeo->SetBarColorOn(iBar, layer);

      } //end loop on hits
      
   } //end loop on planes
   
   fTwClusDisplay->RefitPlex();
}


//__________________________________________________________
void TAFOeventDisplay::UpdateCrystalElements()
{
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
         fCaClusDisplay->ResetHits();
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   
   TACAparGeo* parGeo = (TACAparGeo*) fpParGeoCa->Object();
   
   // reset previous fired bar
   if ( fRefreshButton->IsOn()) {
      map<int, int >::iterator it;
      for (it = fFiredCaCrystal.begin(); it != fFiredCaCrystal.end(); it++) {
         Int_t idx = it->first;
         parGeo->SetCrystalColorOff(idx);
      }
   }
   
   fFiredCaCrystal.clear();
   
   TACAntuRaw* pNtuHit = (TACAntuRaw*) fpNtuRawCa->Object();
   
   Int_t nHits = pNtuHit->GetHitsN();
   if (nHits == 0) return;
   
   for (Int_t iHit = 0; iHit < nHits; ++iHit) {
      
      TACAntuHit *hit = pNtuHit->GetHit(iHit);
      
      Int_t idx = hit->GetCrystalId();
      
      fFiredCaCrystal[idx] = 1;
      parGeo->SetCrystalColorOn(idx);
      
   } //end loop on hits
   

   fCaClusDisplay->RefitPlex();
}

//__________________________________________________________
void TAFOeventDisplay::UpdateStcElements()
{
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
      fStClusDisplay->ResetHits();
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;

   //STC
   TATRntuRaw* pSTntu = (TATRntuRaw*) fpNtuRawSt->Object();
   Int_t       nHits  = pSTntu->GetHitsN();

   //hits
   for (Int_t i = 0; i < nHits; i++) {
      
      TATRntuHit* hit = pSTntu->Hit(i);
      Float_t charge = hit->GetCharge();
   
      TVector3 posHit(0,0,0); // center
      
      TVector3 posHitG = fpFootGeo->FromSTLocalToGlobal(posHit);
      
      fStClusDisplay->AddHit(charge, posHitG[0], posHitG[1], posHitG[2]);
      fStClusDisplay->QuadId(hit);
   }
   
   fStClusDisplay->RefitPlex();
}

//__________________________________________________________
void TAFOeventDisplay::UpdateWireElements()
{
   
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
      fBmClusDisplay->ResetWires();
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;

   TABMntuRaw* pBMntu = (TABMntuRaw*) fpNtuRawBm->Object();
   Int_t       nHits  = pBMntu->nhit;
   double bm_h_side;
   
   TABMparGeo* pbmGeo = (TABMparGeo*) fpParGeoBm->Object();
   
   //hits
   for (Int_t i = 0; i < nHits; i++) {
      TABMntuHit* hit = pBMntu->Hit(i);
      
      Int_t view  = hit->View();
      Int_t lay  = hit->Plane();
      Int_t cell  = hit->Cell();
      
      Float_t x = pbmGeo->GetWireX(pbmGeo->GetSenseId(cell),lay,view);
      Float_t y = pbmGeo->GetWireY(pbmGeo->GetSenseId(cell),lay,view);
      Float_t z = pbmGeo->GetWireZ(pbmGeo->GetSenseId(cell),lay,view);
      
      TVector3 posHit(x, y, z);
      TVector3 posHitG = fpFootGeo->FromBMLocalToGlobal(posHit);
      
      bm_h_side   = pbmGeo->GetWidth();
      if(view == 1) {
         //X,Z, top view
         fBmClusDisplay->AddWire(posHitG(0), posHitG(1), posHitG(2), posHitG(0), posHitG(1)+bm_h_side, posHitG(2));
      } else {
         //Y,Z, side view
         fBmClusDisplay->AddWire(posHitG(0), posHitG(1), posHitG(2), posHitG(0)+bm_h_side, posHitG(1), posHitG(2));
      }
   }
   
   fBmClusDisplay->RefitPlex();
}

//__________________________________________________________
void TAFOeventDisplay::UpdateGlbTrackElements()
{
   TVector3 vtx(0,0,0);
   TVector3 momentum(0,0,3);
   Int_t charge = 6;
   fGlbTrackDisplay->AddTrack(vtx, momentum, charge);
   
   fGlbTrackDisplay->MakeTracks();
}

//__________________________________________________________
void TAFOeventDisplay::UpdateDefCanvases()
{
   Int_t nCanvas = fListOfCanvases->GetEntries();
   Int_t nHisto = fHistoList->GetEntries();

   for (Int_t i = 0; i < nCanvas; ++i) {
      TCanvas* canvas = (TCanvas*)fListOfCanvases->At(i);
      if (!canvas) continue;

      for (Int_t k = 0; k < nHisto; ++k) {
         TH1* h = (TH1*)fHistoList->At(k);
         if (nHisto == 1)
            canvas->cd();
         else
            canvas->cd(k+1);
         h->Draw();
      }
      
      canvas->Update();
   }
}

//__________________________________________________________
void TAFOeventDisplay::CreateCanvases()
{
   // GUI
   // histo
   TCanvas* canvas = 0x0;
   TVirtualPad* pad    = 0x0;
   TEveWindowSlot* slot0 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab0 = slot0->MakeTab();
   tab0->SetElementName("Histogram");
   tab0->SetShowTitleBar(kFALSE);
   
   // canvas tab
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas00 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame00 = slot0->MakeFrame(eCanvas00);
   frame00->SetElementName("Histograms");
   canvas = eCanvas00->GetCanvas();
   canvas->SetName("HistoCanvas");
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad("HistoPad","",0.,0.,1.,1);
   
   fHistoListBox->GetSelectedEntries(fSelecHistoList);
   Int_t nHisto = fSelecHistoList->GetEntries();
   
   if (nHisto != 1)
      pad->Divide(nHisto/2, 2);
   
   pad->Draw();
   fListOfPads->Add(pad);
   
   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();

}
