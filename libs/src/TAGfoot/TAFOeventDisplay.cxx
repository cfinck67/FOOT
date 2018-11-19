

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

#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"
#include "TATW_ContainerHit.hxx"

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
   fpParGeoTr(0x0),
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
   fVtxClusDisplay(new TAGclusterDisplay("Vertex Cluster")),
   fVtxTrackDisplay(new TAGtrackDisplay("Vertex Tracks")),
   fItClusDisplay(new TAGclusterDisplay("Inner tracker Cluster")),
   fMsdClusDisplay(new TAGclusterDisplay("Inner tracker Cluster")),
   fTwClusDisplay(new TAGclusterDisplay("Tof Wall hit")),
   fGlbTrackDisplay(new TAGglbTrackDisplay("Global Tracks"))
{
   
  // default constructon
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
   
   fGlbTrackDisplay->SetMaxMomentum(fMaxMomentum);
   
   GlobalPar::Instance();
}

//__________________________________________________________
TAFOeventDisplay::~TAFOeventDisplay()
{
   // default destructor
   if (fpParGeoVtx) delete fpParGeoVtx;
   if (fpParGeoIt)  delete fpParGeoIt;
   if (fpParGeoMsd) delete fpParGeoMsd;
   if (fpParGeoTw)  delete fpParGeoTw;
   if (fpParGeoG)   delete fpParGeoG;
   if (fpParGeoDi)  delete fpParGeoDi;
   
   delete fVtxClusDisplay;
   delete fVtxTrackDisplay;
   
   delete fItClusDisplay;
   delete fMsdClusDisplay;
   delete fTwClusDisplay;
   
   delete fGlbTrackDisplay;
   
   if (fField) delete fField;
   if (fFieldImpl) delete fFieldImpl;
}

//__________________________________________________________
void TAFOeventDisplay::ReadParFiles()
{
   // initialise par files for target
   if (GlobalPar::GetPar()->IncludeTG()) {
      fpParGeoG = new TAGparaDsc(TAGparGeo::GetDefParaName(), new TAGparGeo());
      TAGparGeo* parGeo = (TAGparGeo*)fpParGeoG->Object();
      parGeo->InitGeo();
   }
   
   // initialise par files for start counter
   if (GlobalPar::GetPar()->IncludeST()) {
      fpParGeoTr = new TAGparaDsc(TATRparGeo::GetDefParaName(), new TATRparGeo());
      TATRparGeo* parGeo = (TATRparGeo*)fpParGeoTr->Object();
      parGeo->InitGeo();

   }

   // initialise par files for Beam Monitor
   if (GlobalPar::GetPar()->IncludeBM()) {
      fpParGeoBm = new TAGparaDsc("bmGeo", new TABMparGeo());
      TABMparGeo* parGeo = (TABMparGeo*)fpParGeoBm->Object();
      parGeo->InitGeo();
   }

   // initialise par files for vertex
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpParGeoVtx = new TAGparaDsc(TAVTparGeo::GetDefParaName(), new TAVTparGeo());
      TAVTparGeo* parGeo = (TAVTparGeo*)fpParGeoVtx->Object();
      parGeo->InitGeo();
      
      fpParConfVtx = new TAGparaDsc("vtConf", new TAVTparConf());
      TAVTparConf* parConf = (TAVTparConf*)fpParConfVtx->Object();
      TString mapVtxFileName = Form("./config/TAVTdetector%s.cfg", fExpName.Data());
      parConf->FromFile(mapVtxFileName.Data());
   }
   
   // initialise par files for Magnet
   if (GlobalPar::GetPar()->IncludeDI()) {
      fpParGeoDi = new TAGparaDsc("diGeo", new TADIparGeo());
      TADIparGeo* parGeo = (TADIparGeo*)fpParGeoDi->Object();
      parGeo->InitGeo();
      if (GlobalPar::GetPar()->IncludeKalman()) {
         fFieldImpl  = new FootField("NewDoubleDipole.table");
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
      parGeo->InitGeo();
      
      fpParConfIt = new TAGparaDsc("itConf", new TAITparConf());
      TAITparConf* parConf = (TAITparConf*)fpParConfIt->Object();
      TString mapItFileName = Form("./config/TAITdetector%s.cfg", fExpName.Data());
      parConf->FromFile(mapItFileName.Data());
   }
   
   // initialise par files for multi strip detector
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fpParGeoMsd = new TAGparaDsc(TAMSDparGeo::GetDefParaName(), new TAMSDparGeo());
      TAMSDparGeo* parGeo = (TAMSDparGeo*)fpParGeoMsd->Object();
      parGeo->InitGeo();
      
      fpParConfMsd = new TAGparaDsc("msdConf", new TAMSDparConf());
      TAMSDparConf* parConf = (TAMSDparConf*)fpParConfMsd->Object();
      TString mapMsdFileName = Form("./config/TAMSDdetector%s.cfg", fExpName.Data());
      parConf->FromFile(mapMsdFileName.Data());
   }
   
   // initialise par files for Tof Wall
   if (GlobalPar::GetPar()->IncludeTW()) {
      fpParGeoTw = new TAGparaDsc(TATWparGeo::GetDefParaName(), new TATWparGeo());
      TATWparGeo* parGeo = (TATWparGeo*)fpParGeoTw->Object();
      parGeo->InitGeo();
   }
   
   // initialise par files for caloriomter
   if (GlobalPar::GetPar()->IncludeCA()) {
      fpParGeoCa = new TAGparaDsc(TACAparGeo::GetDefParaName(), new TACAparGeo());
      TACAparGeo* parGeo = (TACAparGeo*)fpParGeoCa->Object();
      parGeo->InitGeo();
   }

   TAVTparConf::SetHistoMap();
}

//__________________________________________________________
void TAFOeventDisplay::BuildDefaultGeometry()
{
   TAGbaseEventDisplay::BuildDefaultGeometry();
   
   // ST
   if (GlobalPar::GetPar()->IncludeST()) {
      TATRparGeo* parGeo = (TATRparGeo*)fpParGeoTr->Object();
      TGeoVolume* irVol  = parGeo->BuildStartCounter();
   
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {STC_X, STC_Y, STC_Z};
      transf->SetTranslation(vec);
      AddGeometry(irVol, transf);
   }

   // BM
   if (GlobalPar::GetPar()->IncludeBM()) {
      TABMparGeo* parGeo = (TABMparGeo*)fpParGeoBm->Object();
      TGeoVolume* bmVol  = parGeo->BuildBeamMonitor();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {BMN_X, BMN_Y, BMN_Z};
      transf->SetTranslation(vec);
      AddGeometry(bmVol, transf);
   }

   // target
   if (GlobalPar::GetPar()->IncludeTG()) {
      TAGparGeo* parGeo = (TAGparGeo*)fpParGeoG->Object();
      TGeoVolume* tgVol = parGeo->BuildTarget();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {TG_X, TG_Y, TG_Z};
      transf->SetTranslation(vec);
      AddGeometry(tgVol, transf);
   }

   // Vertex
   if (GlobalPar::GetPar()->IncludeVertex()) {
      TAVTparGeo* parGeo = (TAVTparGeo*)fpParGeoVtx->Object();
      TGeoVolume* vtVol  = parGeo->BuildVertex();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {VTX_X, VTX_Y, VTX_Z};
      transf->SetTranslation(vec);
      AddGeometry(vtVol, transf);
   }

   // Magnet
   if (GlobalPar::GetPar()->IncludeDI()) {
      TADIparGeo* parGeo = (TADIparGeo*)fpParGeoDi->Object();
      TGeoVolume* vtVol = parGeo->BuildMagnet();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {MAG_X, MAG_Y, MAG_Z};
      transf->SetTranslation(vec);
      AddGeometry(vtVol, transf);
   }

   // IT
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      TAITparGeo* parGeo = (TAITparGeo*)fpParGeoIt->Object();
      TGeoVolume* itVol  = parGeo->BuildInnerTracker();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {ITR_X, ITR_Y, ITR_Z};
      transf->SetTranslation(vec);
      AddGeometry(itVol, transf);
   }
   
   // MSD
   if (GlobalPar::GetPar()->IncludeMSD()) {
      TAMSDparGeo* parGeo = (TAMSDparGeo*)fpParGeoMsd->Object();
      TGeoVolume* msdVol = parGeo->BuildMultiStripDetector();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {MSD_X, MSD_Y, MSD_Z};
      transf->SetTranslation(vec);
      AddGeometry(msdVol, transf);
   }

   // TW
   if (GlobalPar::GetPar()->IncludeTW()) {
      TATWparGeo* parGeo = (TATWparGeo*)fpParGeoTw->Object();
      TGeoVolume* twVol = parGeo->BuildTofWall();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {SCN_X, SCN_Y, SCN_Z};
      transf->SetTranslation(vec);
      AddGeometry(twVol, transf);
   }

   // CA
   if (GlobalPar::GetPar()->IncludeCA()) {
      TACAparGeo* parGeo = (TACAparGeo*)fpParGeoCa->Object();
      TGeoVolume* twVol = parGeo->BuildCalorimeter();
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      Double_t vec[3] = {CAL_X, CAL_Y, CAL_Z};
      transf->SetTranslation(vec);
      AddGeometry(twVol, transf);
   }
}

//__________________________________________________________
void TAFOeventDisplay::CreateRecAction()
{
   if (GlobalPar::GetPar()->IncludeVertex())
      CreateRecActionVtx();
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      CreateRecActionIt();
   
   if (GlobalPar::GetPar()->IncludeMSD())
      CreateRecActionMsd();
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
         fActVtx    = new TAVTactNtuVertexPD("vtActVtx", fpNtuTrackVtx, fpNtuVtx, fpParConfVtx, fpParGeoVtx);
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
//      fpNtuRawTw   = new TAGdataDsc("twdDat", new TATWdatRaw());
//      fpNtuRawTw   = new TAGdataDsc("twRaw", new TATWntuRaw());
//      fActNtuRawTw = new TAVTactNtuRaw("twActNtu", fpNtuRawTw, fpNtuRawTw, fpParGeoTw);
//      fActNtuRawTw->CreateHistogram();
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
   if (GlobalPar::GetPar()->IncludeVertex())
      AddRequiredItemVtx();
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      AddRequiredItemIt();
   
   if (GlobalPar::GetPar()->IncludeMSD())
      AddRequiredItemMsd();
   
   if (GlobalPar::GetPar()->IncludeTW())
      AddRequiredItemTw();

   fAGRoot->BeginEventLoop();
   fAGRoot->Print();
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
void TAFOeventDisplay::AddElements()
{
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
   
   fGlbTrackDisplay->ResetTracks();
   gEve->AddElement(fGlbTrackDisplay);
}

//__________________________________________________________
void TAFOeventDisplay::ConnectElements()
{
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
      fInfoView->AddLine( Form("at position: (%7.1f %7.1f)\n", pos.X(), pos.Y()) );
   } else if (obj->InheritsFrom("TAVTvertex")) {
      TAVTvertex* vtx = (TAVTvertex*)obj;
      if (vtx == 0x0) return;
      TVector3 pos = vtx->GetVertexPosition();
      fInfoView->AddLine( Form("Vertex# %d at position:\n", idx) );
      fInfoView->AddLine( Form(" (%7.1f %7.1f %7.1f)\n", pos.X(), pos.Y(), pos.Z()) );
      fInfoView->AddLine( Form(" BM Matched %d\n", vtx->IsBmMatched()) );
      
   } else {
      return;
   }
}

//__________________________________________________________
void TAFOeventDisplay::UpdateTrackInfo(TEveDigitSet* qs, Int_t idx)
{
   TAGtrackDisplay* lineTracks = dynamic_cast<TAGtrackDisplay*> (qs);
   TAVTtrack* track =  (TAVTtrack*)lineTracks->GetId(idx);
   if (track == 0x0) return;
   fInfoView->AddLine( Form("Track # %2d:", track->GetNumber()) );
   fInfoView->AddLine( Form(" with %3d clusters\n", track->GetClustersN()) );
   
   for (Int_t i = 0; i < track->GetClustersN(); i++) {
      TAVTcluster* clus = track->GetCluster(i);
      TVector3 posG = clus->GetPositionG();
      fInfoView->AddLine( Form(" for plane %d\n", clus->GetPlaneNumber()));
      fInfoView->AddLine( Form(" at position: (%7.1f %7.1f) \n", posG.X(), posG.Y()) );
      fInfoView->AddLine( Form(" with %d pixels\n", clus->GetPixelsN()));
   }
}

//__________________________________________________________
void TAFOeventDisplay::UpdateElements()
{
   if (fgGUIFlag)
      fEventEntry->SetText(Form("Run %d Event %d", fAGRoot->CurrentRunInfo().RunNumber(), fAGRoot->CurrentEventId().EventNumber()));
   
   if (GlobalPar::GetPar()->IncludeVertex())
      UpdateElements("vt");
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      UpdateElements("it");
   
   if (GlobalPar::GetPar()->IncludeMSD())
      UpdateElements("ms");
   
   if (GlobalPar::GetPar()->IncludeTW())
      UpdateElements("tw");

   gEve->FullRedraw3D(kFALSE);
}

//__________________________________________________________
void TAFOeventDisplay::UpdateElements(const TString prefix)
{
   if (prefix == "tw" || prefix == "ca")
      UpdateBarElements(prefix);
   else {
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
   
   if (fgTrackFlag && GlobalPar::GetPar()->IncludeTG()) {
      // vertex
      if (fgDrawVertex && prefix == "vt") {
         pNtuTrack = (TAVTntuTrack*)  fpNtuTrackVtx->Object();
         TAVTvertex*    vtxPD   = 0x0;//NEW
         TVector3 vtxPositionPD = pNtuTrack->GetBeamPosition();
         
         if (prefix == "vt") {
            if (fpNtuVtx->Valid()) {
               TAVTntuVertex* pNtuVtxPD = (TAVTntuVertex*) fpNtuVtx->Object();
               for (Int_t iVtx = 0; iVtx < pNtuVtxPD->GetVertexN(); ++iVtx) {
                  vtxPD = pNtuVtxPD->GetVertex(iVtx);
                  if (vtxPD == 0x0) continue;
                  vtxPositionPD = vtxPD->GetVertexPosition();
                  parGeo->Local2Global(&vtxPositionPD);
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
            parGeo->Local2Global(&posG);
         else if (prefix == "it")
            parGeo->Local2Global(&posG);
         else if (prefix == "ms")
            parGeo->Local2Global(&posG);

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
   if (prefix != "vt")
      return;
   
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
      if (prefix == "vt")
         fVtxTrackDisplay->ResetTracks();
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   Float_t x1 = 0.,  y1 =0., z1 = 0.;
   
   TAVTparGeo* parGeo = 0x0;
   
   if (prefix == "vt")
      parGeo = (TAVTparGeo*) fpParGeoVtx->Object();
 
   
   Int_t nPlanes         = parGeo->GetNSensors();
   Float_t posfirstPlane = parGeo->GetSensorPosition(0)[2];
   Float_t posLastPlane  = parGeo->GetSensorPosition(nPlanes-1)[2];
   
   TAVTntuTrack*  pNtuTrack = 0x0;
   
   if (prefix == "vt")
      pNtuTrack = (TAVTntuTrack*)  fpNtuTrackVtx->Object();
   
   if( pNtuTrack->GetTracksN() > 0 ) {
      for( Int_t iTrack = 0; iTrack < pNtuTrack->GetTracksN(); ++iTrack ) {
         if (prefix == "vt")
            fVtxTrackDisplay->AddNewTrack();
         
         TAVTtrack* track = pNtuTrack->GetTrack(iTrack);
         TVector3 pos;
         TVector3 posG;
         
         if ( prefix == "vt") {
            if (fgDrawVertex && GlobalPar::GetPar()->IncludeTG())
               pos = track->Intersection(track->GetVertexZ());
            else
               pos = track->Intersection(posfirstPlane);
            
            posG = pos;
            parGeo->Local2Global(&posG);
            
            x = posG(0); y = posG(1); z = posG(2)*0.9;
            
            pos = track->Intersection(posLastPlane);
            posG = pos;
            parGeo->Local2Global(&posG);
            
            x1 = posG(0); y1 = posG(1); z1 = posG(2)*1.1;
            
            Int_t nClus = track->GetClustersN();
            fVtxTrackDisplay->AddTracklet(nClus*100, x, y, z, x1, y1, z1);
            fVtxTrackDisplay->TrackId(track);
         }
      } // end loop on tracks
      
   } // nTracks > 0
   
   if (prefix == "vt")
      fVtxTrackDisplay->RefitPlex();
}

//__________________________________________________________
void TAFOeventDisplay::UpdateBarElements(const TString prefix)
{
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
      if (prefix == "tw") {
         fTwClusDisplay->ResetHits();
      }
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   
   TATWparGeo* parGeo = 0x0;
   
   if (prefix == "tw")
      parGeo = (TATWparGeo*) fpParGeoTw->Object();
   else
      return;

   // reset previous fired bar
   if ( fRefreshButton->IsOn()) {
      map< pair<int, int>, int >::iterator it;
      for (it = fFiredTofBar.begin(); it != fFiredTofBar.end(); it++) {
         pair<int, int> idx = it->first;
         Int_t iBar = idx.first;
         Int_t iLayer = idx.second;
         parGeo->SetSlatColorOff(iBar, iLayer);
      }
   }

   fFiredTofBar.clear();

   TATW_ContainerHit* pNtuHit  =  0x0;
   
   if (prefix == "tw")
      pNtuHit = (TATW_ContainerHit*) fpNtuRawTw->Object();
   
   for( Int_t iLayer = 0; iLayer < 1; iLayer++) {
      

      Int_t nHits = pNtuHit->GetHitN(iLayer);
      if (nHits == 0) continue;

      for (Int_t iHit = 0; iHit < nHits; ++iHit) {
         
         TATW_Hit *hit = pNtuHit->GetHit(iLayer, iHit);

         Int_t iBar = 22-hit->GetBar()-2;
         pair<int, int> idx(iBar, iLayer);
         
         fFiredTofBar[idx] = 1;
         parGeo->SetSlatColorOn(iBar, iLayer);

         TVector3 pos = hit->GetMCPosition_detectorFrame();
         TVector3 posG = pos;
         if (prefix == "tw")
            parGeo->Local2Global(&posG);
         
         x = posG(0);
         y = posG(1);
         z = posG(2);
         
         Double_t eloss = hit->GetEnergyLoss();
         
         if (prefix == "tw") {
            fTwClusDisplay->AddHit(eloss*10, x, y, z);
            fTwClusDisplay->QuadId(hit);
         }
      } //end loop on hits
      
   } //end loop on planes
   
   if (prefix == "tw")
      fTwClusDisplay->RefitPlex();
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
void TAFOeventDisplay::CreateCanvases()
{
   // GUI
   TGHorizontalFrame *histoFrame = new TGHorizontalFrame(frmMain);
   
   fClusterButton = new TGCheckButton(histoFrame, "Cluster 2D", 1);
   fClusterButton->SetToolTipText("Enable rawdata histo");
   fClusterButton->SetState(kButtonUp);
   histoFrame->AddFrame(fClusterButton, new TGLayoutHints(kLHintsLeft | kLHintsLeft, 5, 0, 5, 0));
   
   fRawDataButton = new TGCheckButton(histoFrame, "Rawdata 2D", 1);
   fRawDataButton->SetToolTipText("Enable rawdata histo");
   fRawDataButton->SetState(kButtonUp);
   histoFrame->AddFrame(fRawDataButton, new TGLayoutHints(kLHintsLeft | kLHintsLeft, 5, 0, 5, 0));
   
   frmMain->AddFrame(histoFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 0, 5, 5));
   
   // rate button
   TGHorizontalFrame *rateFrame = new TGHorizontalFrame(frmMain);
   fRateButton = new TGCheckButton(rateFrame, "Rates 2D", 1);
   fRateButton->SetToolTipText("Enable rates histo");
   fRateButton->SetState(kButtonUp);
   rateFrame->AddFrame(fRateButton, new TGLayoutHints(kLHintsLeft | kLHintsLeft, 5, 0, 5, 0));
   fRefreshEvent  = new TGNumberEntry(rateFrame, 0, 4, -1,
                                      TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
                                      TGNumberFormat::kNELLimitMinMax, 1, 1500);
   
   fRefreshEvent->Resize(60,20);
   fRefreshEvent->SetNumber(100);
   rateFrame->AddFrame(fRefreshEvent, new TGLayoutHints(kLHintsLeft | kLHintsLeft, 5, 0, 5, 0));
   
   frmMain->AddFrame(rateFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 0, 5, 5));
   
   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();

   
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   if (GlobalPar::GetPar()->IncludeVertex())
      CreateCanvases("vt");
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      CreateCanvases("it");
   
   if (GlobalPar::GetPar()->IncludeMSD())
      CreateCanvases("ms");
}

//__________________________________________________________
void TAFOeventDisplay::CreateCanvases(const TString prefix)
{
   // histo 2D
   TAVTparGeo* parGeo = 0x0;
   
   if (prefix == "vt")
      parGeo= (TAVTparGeo*) fpParGeoVtx->Object();
   else if (prefix == "it")
      parGeo= (TAVTparGeo*) fpParGeoIt->Object();
   else if (prefix == "ms")
      parGeo= (TAVTparGeo*) fpParGeoMsd->Object();

   Int_t nPlanes = parGeo->GetNSensors();
   
   TCanvas* canvas = 0x0;
   TVirtualPad* pad    = 0x0;
   TEveWindowSlot* slot0 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab0 = slot0->MakeTab();
   tab0->SetElementName(Form("Histo2D %s", prefix.Data()));
   tab0->SetShowTitleBar(kFALSE);
   
   // canvas tab
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas00 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame00 = slot0->MakeFrame(eCanvas00);
   frame00->SetElementName(Form("Clusters 2D %s", prefix.Data()));
   canvas = eCanvas00->GetCanvas();
   canvas->SetName(Form("%sClus2D", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sClus2D", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas01 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame01 = slot0->MakeFrame(eCanvas01);
   frame01->SetElementName(Form("Pixel Pos 2D %s", prefix.Data()));
   canvas = eCanvas01->GetCanvas();
   canvas->SetName(Form("%sPixelPos2D", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sPixelPos2D", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas02 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame02 = slot0->MakeFrame(eCanvas02);
   frame02->SetElementName(Form("Pixel Raw 2D %s", prefix.Data()));
   canvas = eCanvas02->GetCanvas();
   canvas->SetName(Form("%sPixelRaw2D", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sPixelRaw2D", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas03 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame03 = slot0->MakeFrame(eCanvas03);
   frame03->SetElementName(Form("Beam Profile %s", prefix.Data()));
   canvas = eCanvas03->GetCanvas();
   canvas->SetName(Form("%sBeam2D", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sBeam2D", prefix.Data()),"",0.,0.,1.,1);
   pad->Draw();
   fListOfPads->Add(pad);
   
   // other histo tab
   TEveWindowSlot* slot1 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab1 = slot1->MakeTab();
   tab1->SetElementName(Form("Histos %s", prefix.Data()));
   tab1->SetShowTitleBar(kFALSE);
   
   // Residuals
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas10 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame10 = slot1->MakeFrame(eCanvas10);
   frame10->SetElementName(Form("Total Residual %s", prefix.Data()));
   canvas = eCanvas10->GetCanvas();
   canvas->SetName(Form("%sTotRes", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sTotRes", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide(2, 1);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas11 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame11 = slot1->MakeFrame(eCanvas11);
   frame11->SetElementName(Form("Individual Residual X %s", prefix.Data()));
   canvas = eCanvas11->GetCanvas();
   canvas->SetName(Form("%sIndResX", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sIndResX", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas12 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame12 = slot1->MakeFrame(eCanvas12);
   frame12->SetElementName(Form("Individual Residual Y %s", prefix.Data()));
   canvas = eCanvas12->GetCanvas();
   canvas->SetName(Form("%sIndResY", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sIndResY", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   // rates tab
   TEveWindowSlot* slot10 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab10 = slot10->MakeTab();
   tab10->SetElementName(Form("Rates %s", prefix.Data()));
   tab10->SetShowTitleBar(kFALSE);
   
   slot10 = tab10->NewSlot();
   TRootEmbeddedCanvas* eCanvas13 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame13 = slot10->MakeFrame(eCanvas13);
   frame13->SetElementName(Form("Individual Rates %s", prefix.Data()));
   canvas = eCanvas13->GetCanvas();
   canvas->SetName(Form("%sIndRates", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sIndRates", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas14 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame14 = slot1->MakeFrame(eCanvas14);
   frame14->SetElementName(Form("Individual # pixels/Cluster %s", prefix.Data()));
   canvas = eCanvas14->GetCanvas();
   canvas->SetName(Form("%sIndPixClus", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sIndPixClus", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   // canvas Chi2
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas20 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame20 = slot1->MakeFrame(eCanvas20);
   frame20->SetElementName(Form("Global Histo %s", prefix.Data()));
   canvas = eCanvas20->GetCanvas();
   canvas->SetName(Form("%sChi2", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sChi2", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide(2, 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot10 = tab10->NewSlot();
   TRootEmbeddedCanvas* eCanvas15 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame15 = slot10->MakeFrame(eCanvas15);
   frame15->SetElementName(Form("Individual length/evt %s", prefix.Data()));
   canvas = eCanvas15->GetCanvas();
   canvas->SetName(Form("%sIndLengthEvt", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sIndLengthEvt", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot10 = tab10->NewSlot();
   TRootEmbeddedCanvas* eCanvas16 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame16 = slot10->MakeFrame(eCanvas16);
   frame16->SetElementName(Form("Individual QRates %s", prefix.Data()));
   canvas = eCanvas16->GetCanvas();
   canvas->SetName(Form("%sIndQRates", prefix.Data()));
   canvas->Resize();
   fListOfCanvases->Add(canvas);
   pad = new TPad(Form("%sIndQRates", prefix.Data()),"",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
}

//______________________________________________________________________________
void TAFOeventDisplay::DrawReco()
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   if (GlobalPar::GetPar()->IncludeVertex())
      DrawReco("vt");
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      DrawReco("it");
   
   if (GlobalPar::GetPar()->IncludeMSD())
      DrawReco("ms");
}

//______________________________________________________________________________
void TAFOeventDisplay::DrawReco(const TString prefix)
{
   TAVTbaseParGeo* parGeo = 0x0;
   TList* histoClus   = 0x0;
   TList* histoTrack  = 0x0;
   
   if (prefix == "vt") {
      parGeo = (TAVTparGeo*) fpParGeoVtx->Object();
      histoClus  = fActClusVtx->GetHistogrammList();
      if (fActTrackVtx)
         histoTrack = fActTrackVtx->GetHistogrammList();

   } else if (prefix == "it"){
      parGeo = (TAITparGeo*) fpParGeoIt->Object();
      histoClus  = fActClusIt->GetHistogrammList();
      
   } else if (prefix == "ms") {
      parGeo = (TAMSDparGeo*) fpParGeoMsd->Object();
      histoClus  = fActClusMsd->GetHistogrammList();
   }
   
   Int_t nPlanes = parGeo->GetNSensors();
   
   TVirtualPad* pad = 0x0;
   
   pad = (TVirtualPad*)fListOfPads->FindObject(Form("%sChi2", prefix.Data()));
   TH1F* pHisPixelTot = (TH1F*)histoClus->FindObject(Form("%sClusPixelTot", prefix.Data()));
   pad->cd(4);
   pHisPixelTot->Draw();
   
   if (fActTrackVtx && prefix == "vt") {
      TH1F* pHisChi2TotX = (TH1F*)histoTrack->FindObject(Form("%sChi2TotX", prefix.Data()));
      pad->cd(1);
      pHisChi2TotX->Draw();
      
      TH1F* pHisChi2TotY = (TH1F*)histoTrack->FindObject(Form("%sChi2TotY", prefix.Data()));
      pad->cd(2);
      pHisChi2TotY->Draw();
      
      TH1F* pHisTrackEvt = (TH1F*)histoTrack->FindObject(Form("%sTrackEvt", prefix.Data()));
      pad->cd(3);
      pHisTrackEvt->Draw();
      
      pad = (TVirtualPad*)fListOfPads->FindObject(Form("%sTotRes", prefix.Data()));
      TH1F* pHisResTotX = (TH1F*)histoTrack->FindObject(Form("%sResTotX", prefix.Data()));
      pad->cd(1);
      pHisResTotX->Draw();
      
      TH1F* pHisResTotY = (TH1F*)histoTrack->FindObject(Form("%sResTotY", prefix.Data()));
      pad->cd(2);
      pHisResTotY->Draw();
      
      pad = (TVirtualPad*)fListOfPads->FindObject(Form("%sBeam2D", prefix.Data()));
      TH2F* pHisBeamProf = (TH2F*)histoTrack->FindObject(Form("%sBeamProf", prefix.Data()));
      pad->cd();
      pHisBeamProf->Draw("colz");
   }
   
   TVirtualPad* pad0 = (TVirtualPad*)fListOfPads->FindObject(Form("%sClus2D", prefix.Data()));
   TVirtualPad* pad4 = (TVirtualPad*)fListOfPads->FindObject(Form("%sIndResX", prefix.Data()));
   TVirtualPad* pad5 = (TVirtualPad*)fListOfPads->FindObject(Form("%sIndResY", prefix.Data()));
   TVirtualPad* pad7 = (TVirtualPad*)fListOfPads->FindObject(Form("%sIndPixClus", prefix.Data()));
   
   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
      
      TH2F* pHisClusMap = (TH2F*)histoClus->FindObject(Form("%sClusMap%d", prefix.Data(), iPlane+1));
      pad0->cd(iPlane+1);
      pHisClusMap->Draw("P");
      
      if (fActTrackVtx && prefix == "vt") {
         TH2F* pHisTrackMap = (TH2F*)histoTrack->FindObject(Form("%sTrackMap%d", prefix.Data(), iPlane+1));
         pad0->cd(iPlane+1);
         pHisTrackMap->Draw("SAME");
         
         TH1F* pHisResX = (TH1F*)histoTrack->FindObject(Form("%sResX%d", prefix.Data(), iPlane+1));
         pad4->cd(iPlane+1);
         pHisResX->Draw("");
         
         TH1F* pHisResY = (TH1F*)histoTrack->FindObject(Form("%sResY%d", prefix.Data(), iPlane+1));
         pad5->cd(iPlane+1);
         pHisResY->Draw("");
      }
      
      TH1F* pHisPixel = (TH1F*)histoClus->FindObject(Form("%sClusPixel%d", prefix.Data(), iPlane+1));
      pad7->cd(iPlane+1);
      pHisPixel->Draw();
   }
}

//__________________________________________________________
void TAFOeventDisplay::UpdateFreqCanvases()
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   if (GlobalPar::GetPar()->IncludeVertex())
      UpdateFreqCanvases("vt");
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      UpdateFreqCanvases("it");
   
   if (GlobalPar::GetPar()->IncludeMSD())
      UpdateFreqCanvases("ms");

}

//__________________________________________________________
void TAFOeventDisplay::UpdateFreqCanvases(const TString prefix)
{
   TCanvas* canvas = 0x0;
   DrawRate(prefix);
   
   if (fRateButton->IsOn()) {
      canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sIndRates", prefix.Data()));
      canvas->Modified();
      canvas->Update();
      
      canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sIndQRates", prefix.Data()));
      canvas->Modified();
      canvas->Update();
      
      canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sIndLengthEvt", prefix.Data()));
      canvas->Modified();
      canvas->Update();
   }
   
   ResetRate(prefix);
}

//__________________________________________________________
void TAFOeventDisplay::UpdateDefCanvases()
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   if (GlobalPar::GetPar()->IncludeVertex())
      UpdateDefCanvases("vt");
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      UpdateDefCanvases("it");
   
   if (GlobalPar::GetPar()->IncludeMSD())
      UpdateDefCanvases("ms");
}

//__________________________________________________________
void TAFOeventDisplay::UpdateDefCanvases(const TString prefix)
{
   DrawReco(prefix);
   DrawRawdata(prefix);
   
   TCanvas* canvas = 0x0;
   if (fClusterButton->IsOn()) {
      canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sClus2D", prefix.Data()));
      canvas->Modified();
      canvas->Update();
   }
   
   if (fRawDataButton->IsOn()) {
      
      canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sPixelPos2D", prefix.Data()));
      canvas->Modified();
      canvas->Update();
      
      canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sPixelRaw2D", prefix.Data()));
      canvas->Modified();
      canvas->Update();
   }
   
   canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sTotRes", prefix.Data()));
   canvas->Modified();
   canvas->Update();
   
   canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sIndResX", prefix.Data()));
   canvas->Modified();
   canvas->Update();
   
   canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sIndResY", prefix.Data()));
   canvas->Modified();
   canvas->Update();
   
   canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sIndPixClus", prefix.Data()));
   canvas->Modified();
   canvas->Update();
   
   canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sChi2", prefix.Data()));
   canvas->Modified();
   canvas->Update(); 
   
   canvas = (TCanvas*)fListOfCanvases->FindObject(Form("%sBeam2D", prefix.Data()));
   canvas->Modified();
   canvas->Update(); 
}




//__________________________________________________________
void TAFOeventDisplay::DrawRawdata(const TString prefix)
{
   if (fType == 0) {
      TAVTbaseParGeo* parGeo = 0x0;
      TList* histoDat    = 0x0;
   
      if (prefix == "vt") {
         parGeo   = (TAVTparGeo*) fpParGeoVtx->Object();
         histoDat = fActNtuRawVtx->GetHistogrammList();
      } else if (prefix == "it") {
         parGeo   = (TAITparGeo*) fpParGeoIt->Object();
         histoDat = fActNtuRawIt->GetHistogrammList();
      } else if (prefix == "ms") {
         parGeo   = (TAMSDparGeo*) fpParGeoMsd->Object();
         histoDat = fActNtuRawMsd->GetHistogrammList();
      }
   
      Int_t nPlanes = parGeo->GetNSensors();
      TVirtualPad* pad2 = (TVirtualPad*)fListOfPads->FindObject(Form("%sPixelRaw2D", prefix.Data()));
      for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
         TH2F* pHisPixel = (TH2F*)histoDat->FindObject(Form("%sPosMap%d", prefix.Data(), iPlane+1));
         pad2->cd(iPlane+1);
         pHisPixel->Draw("colz");
      }
   }
}

//__________________________________________________________
void TAFOeventDisplay::DrawRate(const TString prefix)
{
   TAVTbaseParGeo* parGeo = 0x0;
   
   if (prefix == "vt")
      parGeo = (TAVTparGeo*) fpParGeoVtx->Object();
   else if (prefix == "it")
      parGeo   = (TAITparGeo*) fpParGeoIt->Object();
   else if (prefix == "ms")
      parGeo = (TAMSDparGeo*) fpParGeoMsd->Object();
   
   Int_t nPlanes = parGeo->GetNSensors();
   
   if (fType == -1) {
      TList* histoDat  = fActEvtReader->GetHistogrammList();
      TVirtualPad* pad9 = (TVirtualPad*)fListOfPads->FindObject(Form("%sIndLengthEvt", prefix.Data()));
      
      for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
         
         TH1F* pHisEvtLength = (TH1F*)histoDat->FindObject(Form("%sEvtLength%d", prefix.Data(), iPlane+1));
         pad9->cd(iPlane+1);
         pHisEvtLength->Draw(); 
      }
   }
}

//__________________________________________________________
void TAFOeventDisplay::ResetRate(const TString prefix)
{
   TAVTbaseParGeo* parGeo = 0x0;
   if (prefix == "vt")
      parGeo = (TAVTparGeo*) fpParGeoVtx->Object();
   else if (prefix == "it")
      parGeo = (TAITparGeo*) fpParGeoIt->Object();
   else if (prefix == "ms")
      parGeo = (TAMSDparGeo*) fpParGeoMsd->Object();
   
   Int_t nPlanes = parGeo->GetNSensors();
   
   if (fType == -1) {
      TList* histoDat  = fActDatRawVtx->GetHistogrammList();
      for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
         if (fRateButton->IsOn()) {
            TH1F* pHisRateMap = (TH1F*)histoDat->FindObject(Form("%sRateMap%d", prefix.Data(), iPlane+1));
            pHisRateMap->Reset();
            
            TH1F* pHisQRate = (TH1F*)histoDat->FindObject(Form("%sRateMapQ%d", prefix.Data(), iPlane+1));
            pHisQRate->Reset();
         }
      }
   }
}

