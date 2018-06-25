
#include "Riostream.h"

#ifndef _TAIReventDisplay_HXX_
#include "TAIReventDisplay.hxx"
#endif

#ifndef ROOT_TEveManager
#include "TEveManager.h"
#endif

#ifndef ROOT_TEveBrowser
#include "TEveBrowser.h"
#endif

#ifndef ROOT_TEveWindow
#include "TEveWindow.h"
#endif

#ifndef ROOT_TFile
#include "TFile.h"
#endif

#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif

#ifndef ROOT_TGeoBBox
#include "TGeoBBox.h"
#endif

#ifndef ROOT_TGeoMatrix
#include "TGeoMatrix.h"
#endif

#ifndef ROOT_TGeoManager
#include "TGeoManager.h"
#endif

// First
#include "gsi_geo.h"
#include "TAGview.hxx" 
#include "TAGgeoTrafo.hxx"

//Trigger
#include "TATRactDatRaw.hxx"
#include "TATRdatRaw.hxx"
#include "TATRparMap.hxx"

//BM
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuTrack.hxx"
#include "TABMntuTrackTr.hxx"
#include "TABMdatRaw.hxx"
#include "TABMrawHit.hxx"
#include "TABMparMap.hxx"
#include "TABMactNtuTrack.hxx"
#include "TABMactDatRaw.hxx"
#include "TABMactNtuRaw.hxx"
#include "TAGwireDisplay.hxx"

//Kentros
#include "TAKEactNtuRaw.hxx"
#include "TAKEactDatRaw.hxx"
#include "TAKEactTrack.hxx"
#include "TAKEdatRaw.hxx"
#include "TAKEntuRaw.hxx"
#include "TAKEntuTrack.hxx"
#include "TAKEparGeo.hxx"
#include "TAKEparMap.hxx"

//IR
#include "TAIRactDatRaw.hxx"
#include "TAIRdatRaw.hxx"
#include "TAIRparMap.hxx"

//MBS
#include "TAGactTreeWriter.hxx"

//VTX
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTactDatRaw.hxx"
#include "TAVTactNtuRaw.hxx"
#include "TAVTactNtuClusterF.hxx"
#include "TAVTactNtuTrack.hxx"

ClassImp(TAIReventDisplay)

TAIReventDisplay* TAIReventDisplay::fgInstance = 0x0;

//__________________________________________________________
TAIReventDisplay* TAIReventDisplay::Instance(const TString name)
{
   if (fgInstance == 0x0) 
	  fgInstance = new TAIReventDisplay(name);

   return fgInstance;
}

//__________________________________________________________
TAIReventDisplay::TAIReventDisplay(const TString name)
: TAVTeventDisplay(name),
  fBmClusDisplay(new TAGwireDisplay("BM Wires")),
  fBmTrackDisplay(new TAGtrackDisplay("BM Tracks")),
  fKeTrackDisplay(new TAGtrackDisplay("KE Tracks")),
  fKeFlag(true),
  fBmFlag(true)
{ 
   // default constructor   

   if (fBmFlag) {
	  fBmClusDisplay->SetPickable(true);
   
	  fBmTrackDisplay->SetMaxEnergy(fMaxEnergy);
	  fBmTrackDisplay->SetDefWidth(fBoxDefWidth);
	  fBmTrackDisplay->SetDefHeight(fBoxDefHeight);
	  fBmTrackDisplay->SetPickable(true);
   } 
   
   if (fKeFlag) {
	  fKeTrackDisplay->SetMaxEnergy(fMaxEnergy);
	  fKeTrackDisplay->SetDefWidth(fBoxDefWidth);
	  fKeTrackDisplay->SetDefHeight(fBoxDefHeight);
	  fKeTrackDisplay->SetPickable(true);
   }
}

//__________________________________________________________
TAIReventDisplay::~TAIReventDisplay()
{
   // default destructor
   delete fBmClusDisplay; 
   delete fBmTrackDisplay;
   delete fKeTrackDisplay;
}

//__________________________________________________________
void TAIReventDisplay::CreateRawAction()
{
   TAVTeventDisplay::CreateRawAction();

   //IR & BM dsc
   fpTrMap = new TAGparaDsc("trMap", new TATRparMap());
   fpIrMap = new TAGparaDsc("irMap", new TAIRparMap());
   fpBmMap  = new TAGparaDsc("bmMap", new TABMparMap());
   fpBmConf = new TAGparaDsc("bmCon", new TABMparCon());
   fpBmGeo  = new TAGparaDsc("bmGeo", new TABMparGeo());
   
   //IR map
   Bool_t b_bad = kTRUE;
   TAIRparMap* pirMap = (TAIRparMap*) fpIrMap->Object();
   pirMap->Clear();
   TString filename = "./geomaps/ir_ch.map";
   b_bad = pirMap->FromFile(filename);
   if (!b_bad) fpIrMap->SetBit(TAGparaDsc::kValid);
   
   //TR map
   b_bad = kTRUE;
   TATRparMap*  ptrMap = (TATRparMap*) fpTrMap->Object();
   ptrMap->Clear();
   filename = "./geomaps/tr_ch.map";
   b_bad    = ptrMap->FromFile(filename);
   if (!b_bad) fpTrMap->SetBit(TAGparaDsc::kValid);
   
   // BM Map
   //Initialize the readout map of the beam monitor: this maps hold info on the
   //TDC channels status: the mapping of TDC channels into Chamber wires happens 
   //in the BMparGeo map.
   b_bad = kTRUE;
   TABMparMap* pbmMap = (TABMparMap*) fpBmMap->Object();
   pbmMap->Clear();
   filename = "./geomaps/beammonitor_geoch.map";
   b_bad    = pbmMap->FromFile(filename);
   if (!b_bad) fpBmMap->SetBit(TAGparaDsc::kValid);   
   
   //BM con
   b_bad = kTRUE;
   TABMparCon* pbmCon = (TABMparCon*) fpBmConf->Object();
   pbmCon->Clear();
   filename = "./config/beammonitor.cfg";
   b_bad    = pbmCon->FromFile(filename);
   filename = "./config/beammonitor_t0s.cfg";
   pbmCon->loadT0s(filename);
   filename = "./config/file_stlist_FIRST.txt";	  
   pbmCon->LoadSTrel(filename);
   pbmCon->SetIsMC(false);
   pbmCon->ConfigureTrkCalib();
   filename = "config/bmreso_vs_r.root";
   pbmCon->LoadReso(filename);
   if (!b_bad) fpBmConf->SetBit(TAGparaDsc::kValid);   
   
   // BM geo
   //Initialize the geometry of the beam monitor: this maps hold info on the
   //TDC channels status: the mapping of TDC channels into Chamber wires happens 
   TABMparGeo* pbmGeo = (TABMparGeo*) fpBmGeo->Object();
   pbmGeo->InitGeo();
   fpBmGeo->SetBit(TAGparaDsc::kValid);
   
   fpBmRaw = new TAGdataDsc("bmRaw", new TABMdatRaw());
   fpBmNtu = new TAGdataDsc("bmNtu", new TABMntuRaw());
   
   
   fpTrRaw = new TAGdataDsc("tRraw", new TATRdatRaw());
   fpIrRaw = new TAGdataDsc("irRaw", new TAIRdatRaw());
 
   
   // Kentros
   if (fKeFlag) {
	  fpKeMap  = new TAGparaDsc("keMap", new TAKEparMap());
	  TAKEparMap* pKeMap = (TAKEparMap*) fpKeMap->Object();
	  pKeMap->Clear();
	  b_bad = kTRUE;
	  filename = "./geomaps/kentros_ch.map";
	  b_bad = pKeMap->FromFile(filename);
	  if (!b_bad) pKeMap->SetBit(TAGparaDsc::kValid);
	  
	  fpKeGeo = new TAGparaDsc("keGeo", new TAKEparGeo());
	  TAKEparGeo* pKeGeo = (TAKEparGeo*) fpKeGeo->Object();
	  //Initialization of KE parameters
	  pKeGeo->InitGeo();
	  pKeGeo->SetBit(TAGparaDsc::kValid);
	  
	  fpKeRaw = new TAGdataDsc("keRaw", new TAKEdatRaw());
	  fpKeNtu = new TAGdataDsc("keNtu", new TAKEntuRaw());
   }
   
   //BM  actions
   fActTrRaw = new TATRactDatRaw("trActRaw", fpTrRaw, fpEvent, fpTrMap);
   fActIrRaw = new TAIRactDatRaw("irActRaw", fpIrRaw, fpEvent, fpIrMap);
   
   fActBmRaw = new TABMactDatRaw("bmActRaw", fpBmRaw, fpEvent, fpBmMap, fpBmConf);
   fActBmNtu = new TABMactNtuRaw("bmActNtu", fpBmNtu, fpBmRaw, fpIrRaw, fpTrRaw, fpBmGeo, fpBmConf);

   
   //Kentris actions
   if (fKeFlag) {
	  fActKeRaw = new TAKEactDatRaw("keActRaw", fpKeRaw, fpEvent, fpKeMap);
	  fActKeNtu = new TAKEactNtuRaw("keActNtu", fpKeNtu, fpKeRaw, fpKeGeo);
   }
}

//__________________________________________________________
void TAIReventDisplay::CreateRecAction()
{
   // BM
   fpBmTrack   = new TAGdataDsc("bmTrk", new TABMntuTrack());
   fActBmTrack = new TABMactNtuTrack("bmActTrk", fpBmTrack, fpBmNtu, fpBmGeo, fpBmConf);
   
   // VTX
   TAVTeventDisplay::CreateRecAction();
   fActTrack->SetBMntuTrack(fpBmTrack);
   fActVtx->SetBMntuTrack(fpBmTrack);
   
   //Kentros
   if (fKeFlag) {
	  fpKeTrack   = new TAGdataDsc("keTrk", new TAKEntuTrack());
	  fActKeTrack = new TAKEactTrack("keActTrk", fpNtuTrack, fpKeTrack, fpKeNtu, 0, 1);
   }
}

//__________________________________________________________
void TAIReventDisplay::BuildDefaultGeometry()
{
   TAVTeventDisplay::BuildDefaultGeometry();
   
   if (fpFirstGeo) {
	  //BM
	  if (fBmFlag) {
		 TABMparGeo* geoBmMap = (TABMparGeo*) fpBmGeo->Object();
		 TGeoVolume* bmVol    = geoBmMap->AddBM();
		 
		 TVector3 bmPos = fpFirstGeo->GetBMCenter();
		 bmPos *= TAGgeoTrafo::CmToMu();
		 TGeoTranslation bmTrans(bmPos[0], bmPos[1], bmPos[2]-fTgZ);
		 
		 TVector3 bmAng = fpFirstGeo->GetBMAngles();
		 TGeoRotation bmRot;
		 bmRot.RotateX(bmAng.Y()); bmRot.RotateY(bmAng.X()); bmRot.RotateZ(bmAng.Z());
		 
		 TGeoHMatrix  bmTransfo;
		 bmTransfo  = bmTrans*bmRot;   
		 AddGeometry(bmVol, new TGeoHMatrix(bmTransfo));
	  }
	  
	  // KE
	  if (fKeFlag) {
		 TAKEparGeo* geoKeMap = (TAKEparGeo*) fpKeGeo->Object();
		 TGeoVolume* keVol    = geoKeMap->BuildKentros();

		 TGeoBBox* shape = (TGeoBBox*)keVol->GetShape();
		 shape->ComputeBBox();
		 Float_t boxSizeZ = shape->GetDZ();
		 
		 TVector3 kePos = fpFirstGeo->GetKECenter();
		 kePos *= TAGgeoTrafo::CmToMu();
		 kePos[2] -= fTgZ;
		 TGeoTranslation keTrans(kePos[0], kePos[1], kePos[2]+boxSizeZ);
		 
		 TVector3 keAng = fpFirstGeo->GetKEAngles();
		 TGeoRotation keRot;
		 keRot.RotateX(keAng.Y()); keRot.RotateY(keAng.X()); keRot.RotateZ(keAng.Z());
		 
		 TGeoHMatrix  keTransfo;
		 keTransfo  = keTrans*keRot; 
		 AddGeometry(keVol, new TGeoHMatrix(keTransfo));
	  }
   }
}

//__________________________________________________________
void TAIReventDisplay::CreateCanvases()
{
   TAVTbaseEventDisplay::CreateCanvases();
   
   TCanvas* canvas = 0x0;
   TVirtualPad* pad    = 0x0;
   TEveWindowSlot* slot0 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab0 = slot0->MakeTab();
   tab0->SetElementName("BM");
   tab0->SetShowTitleBar(kFALSE);
   
   // canvas tab 
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas00 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame00 = slot0->MakeFrame(eCanvas00);
   frame00->SetElementName("Matching");
   canvas = eCanvas00->GetCanvas();
   canvas->SetName("VtxBmRes");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxBmRes","",0.,0.,1.,1);
   pad->Divide(2, 1);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas01 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame01 = slot0->MakeFrame(eCanvas01);
   frame01->SetElementName("Beam Profile");
   canvas = eCanvas01->GetCanvas();
   canvas->SetName("BmBeam2D");
   fListOfCanvases->Add(canvas);
   pad = new TPad("BmBeam2D","",0.,0.,1.,1);
   pad->Draw();
   fListOfPads->Add(pad);
}

//__________________________________________________________
void TAIReventDisplay::UpdateDefCanvases()
{
   TAVTbaseEventDisplay::UpdateDefCanvases();
   
   TCanvas* canvas = 0x0;
   canvas = (TCanvas*)GetCanvasList()->FindObject("VtxBmRes");
   canvas->Modified();
   canvas->Update();   
   
   canvas = (TCanvas*)GetCanvasList()->FindObject("BmBeam2D");
   canvas->Modified();
   canvas->Update();   
}

//______________________________________________________________________________
void TAIReventDisplay::DrawReco()
{
   TAVTbaseEventDisplay::DrawReco();
   
   TList* histoTrack = fActTrack->GetHistogrammList();
   TVirtualPad* pad = 0x0;
   
   pad = (TVirtualPad*)GetPadList()->FindObject("VtxBmRes");
   TH1F* pHisResX = (TH1F*)histoTrack->FindObject("vtBmResX");
   pad->cd(1);
   pHisResX->Draw();
   
   TH1F* pHisResY = (TH1F*)histoTrack->FindObject("vtBmResY");
   pad->cd(2);
   pHisResY->Draw();
   
   pad = (TVirtualPad*)GetPadList()->FindObject("BmBeam2D");
   TH2F* pHisBeamProf = (TH2F*)histoTrack->FindObject("bmBeamProf");
   pad->cd();
   pHisBeamProf->Draw("colz");
}   

//__________________________________________________________
void TAIReventDisplay::AddElements()
{
   TAVTeventDisplay::AddElements();
   
   if (fBmFlag) {
	  fBmClusDisplay->ResetWires();
	  gEve->AddElement(fBmClusDisplay);
	  
	  fBmTrackDisplay->ResetTracks();
	  gEve->AddElement(fBmTrackDisplay);
   }
   
   if (fKeFlag) {
	  fKeTrackDisplay->ResetTracks();
	  gEve->AddElement(fKeTrackDisplay);
   }
}
  
//__________________________________________________________
void TAIReventDisplay::ConnectElements()
{  
   TAVTeventDisplay::ConnectElements();

   fBmTrackDisplay->SetEmitSignals(true);
   fBmTrackDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAIReventDisplay", this, "UpdateBmInfo(TEveDigitSet*, Int_t)");
   
   fKeTrackDisplay->SetEmitSignals(true);
   fKeTrackDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAIReventDisplay", this, "UpdateKeInfo(TEveDigitSet*, Int_t)");
}

//__________________________________________________________
void TAIReventDisplay::UpdateBmInfo(TEveDigitSet* qs, Int_t idx)
{
   TAGtrackDisplay* lineTracks = dynamic_cast<TAGtrackDisplay*> (qs);   
   TABMntuTrackTr* track = (TABMntuTrackTr*) lineTracks->GetId(idx);
   if (track == 0x0) return;
   fInfoView->AddLine( Form("BM Track with %3d wires\n", track->GetNwire()) );
}

//__________________________________________________________
void TAIReventDisplay::UpdateKeInfo(TEveDigitSet* qs, Int_t idx)
{
   TAGtrackDisplay* lineTracks = dynamic_cast<TAGtrackDisplay*> (qs);   
   TAKEtrack* track = (TAKEtrack*) lineTracks->GetId(idx);
   if (track == 0x0) return;
   fInfoView->AddLine( Form("Kentros Track # %2d:", track->id) );
   fInfoView->AddLine( " at exit point\n");
   
   TVector3 pos(track->x2, track->y2, track->z2);
   if (fpFirstGeo)
	  pos = fpFirstGeo->FromVTLocalToGlobal(pos);
   pos *= TAGgeoTrafo::CmToMu();
	  
   fInfoView->AddLine( Form(" (%6.1f %6.1f %6.1f)\n", pos[0], pos[1], pos[2]));
}

//__________________________________________________________
void TAIReventDisplay::UpdateElements()
{      
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
	  if (fBmFlag) {
		 fBmClusDisplay->ResetWires();
		 fBmTrackDisplay->ResetTracks();
	  }
	  if (fKeFlag) fKeTrackDisplay->ResetTracks();
   }
   
   if(!fpFirstGeo) return;
   
   TAVTeventDisplay::UpdateElements();
   
   //BM
   if (fBmFlag) {
	  Bool_t bmTrackOk = false;
	  
	  TABMntuTrack*   pBMtrack = (TABMntuTrack*) fpBmTrack->Object();
	  TABMntuTrackTr* bmTrack  = pBMtrack->Track(0);
	  if (!bmTrack) return;
	  
	  TABMntuRaw* pBMntu = (TABMntuRaw*) fpBmNtu->Object();
	  Int_t       nHits  = pBMntu->nhit;
	  double bm_h_side;
	  Float_t chi2 = bmTrack->GetChi2();
	  TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
	  
	  if (chi2 < pConfig->GetAnalysisPar().BmTrackChi2Limit && chi2 > 0) 
		 bmTrackOk = true;
	  
	  TABMparGeo* pbmGeo = (TABMparGeo*) fpBmGeo->Object();
	  
	  if (bmTrackOk) {
		 TVector3 bmPos = fpFirstGeo->GetBMCenter();
		 TVector3 vtPos = fpFirstGeo->GetVTCenter();
		 TVector3 scPos = fpFirstGeo->GetSCCenter();
		 
		 //hits
		 for (Int_t i = 0; i < nHits; i++) {
			TABMntuHit* hit = pBMntu->Hit(i);
			if(hit->TrkAss()) {
			   TVector3 posHit  = hit->Position();
			   TVector3 posHitG = fpFirstGeo->FromBMLocalToGlobal(posHit);
			   posHitG    *= TAGgeoTrafo::CmToMu();
			   posHitG[2] -= fTgZ;
			   
			   bm_h_side   = pbmGeo->GetWidth()*TAGgeoTrafo::CmToMu();
			   if(hit->View() < 0) {
				  //X,Z, top view
				  fBmClusDisplay->AddWire(posHitG(0), posHitG(1), posHitG(2), posHitG(0), posHitG(1)+bm_h_side, posHitG(2));
			   } else {
				  //Y,Z, side view
				  fBmClusDisplay->AddWire(posHitG(0), posHitG(1), posHitG(2), posHitG(0)+bm_h_side, posHitG(1), posHitG(2));
			   }
			}
		 }
		 
		 // tracks
		 TVector3 trackPos  = bmTrack->PointAtLocalZ(scPos.Z()-bmPos.Z());
		 TVector3 trackPosG = fpFirstGeo->FromBMLocalToGlobal(trackPos);
		 trackPosG    *= TAGgeoTrafo::CmToMu();
		 trackPosG[2] -= fTgZ;
		 
		 TVector3 trackPos1  = bmTrack->PointAtLocalZ(vtPos.Z()-bmPos.Z());
		 TVector3 trackPosG1 = fpFirstGeo->FromBMLocalToGlobal(trackPos1);
		 trackPosG1    *= TAGgeoTrafo::CmToMu();
		 trackPosG1[2] -= fTgZ;
		 
		 fBmTrackDisplay->AddTracklet(60, trackPosG(0), trackPosG(1), trackPosG(2), trackPosG1(0), trackPosG1(1), trackPosG1(2)); 
		 fBmTrackDisplay->TrackId(bmTrack);
	  }
	  fBmClusDisplay->RefitPlex(); 
	  fBmTrackDisplay->RefitPlex();
   }
   
   // Kentros
   if (fKeFlag) {
	  // reset previous fired modules 
	  for (Int_t i = 0; i < fKeFiredMod.GetSize(); ++i) {
		 TString name = TAKEparGeo::GetDefaultModName(fKeFiredMod[i]);
		 TGeoVolume* vol = gGeoManager->FindVolumeFast(name.Data());
		 if (vol) 
			vol->SetLineColor(TAKEparGeo::GetDefaultModCol());
	  }
	  
	  // fired modules
	  TAKEntuRaw* pKeNtu = (TAKEntuRaw*) fpKeNtu->Object();
	  Int_t nHit = pKeNtu->nHit;
	  fKeFiredMod.Set(nHit);
	  for (Int_t i = 0; i < nHit; ++i) {
		 TAKEntuHit* hit = pKeNtu->getHit(i);
		 Int_t region = hit->region;
		 fKeFiredMod.AddAt(region, i);
		 if (fDebugLevel)
			printf("reg: %d\n", region);
		 TString name = TAKEparGeo::GetDefaultModName(region);
		 TGeoVolume* vol = gGeoManager->FindVolumeFast(name.Data());
		 if (vol) 
			vol->SetLineColor(TAKEparGeo::GetDefaultModColOn());
	  }
		 
	  // extrapolated tracks of the VTX
	  TAKEntuTrack* pKeTrack = (TAKEntuTrack*) fpKeTrack->Object();
	  
	  for (Int_t i = 0; i < pKeTrack->nTrack; ++i) {
		 TAKEtrack* track = pKeTrack->getTrack(i);
		 Double_t x0 = track->x0;
		 Double_t y0 = track->y0;
		 Double_t z0 = track->z0;
		 Double_t x1 = track->x1;
		 Double_t y1 = track->y1;
		 Double_t z1 = track->z1;
		 TVector3 trackPos(x0 ,y0 ,0);
		 TVector3 trackPosG = fpFirstGeo->FromVTLocalToGlobal(trackPos);
		 trackPosG *= TAGgeoTrafo::CmToMu();
		 trackPosG[2] -= fTgZ;
		 TVector3 trackPos1(x1 ,y1 ,z1-z0);
		 TVector3 trackPosG1 = fpFirstGeo->FromVTLocalToGlobal(trackPos1);
		 trackPosG1 *= 1.2*TAGgeoTrafo::CmToMu();
		 trackPosG1[2] -= fTgZ;

		 fKeTrackDisplay->AddTracklet(60, trackPosG(0), trackPosG(1), trackPosG(2), trackPosG1(0), trackPosG1(1), trackPosG1(2)); 
		 fKeTrackDisplay->TrackId(track);
	  }
	  fKeTrackDisplay->RefitPlex(); 
   } 
   
   gEve->FullRedraw3D(kFALSE);
}

//__________________________________________________________
void TAIReventDisplay::AddRequiredItem()
{
   //BM
   fAGRoot->AddRequiredItem("trActRaw");
   fAGRoot->AddRequiredItem("irActRaw");
  
   fAGRoot->AddRequiredItem("bmActRaw");
   fAGRoot->AddRequiredItem("bmActNtu");
   fAGRoot->AddRequiredItem("bmActTrk");
   

   if (fKeFlag) {
	  fAGRoot->AddRequiredItem("keActRaw");
	  fAGRoot->AddRequiredItem("keActNtu");
	  fAGRoot->AddRequiredItem("keActTrk");
   }
   
   TAVTeventDisplay::AddRequiredItem();
}
