
#include <Riostream.h>

#ifndef _TAVTbaseEventDisplay_HXX_
#include "TAVTbaseEventDisplay.hxx"
#endif

// root include 
#ifndef ROOT_TEveGeoNode
#include "TEveGeoNode.h"
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

#ifndef ROOT_TGeoManager
#include "TGeoManager.h"
#endif

#ifndef ROOT_TGeoNode
#include "TGeoNode.h"
#endif

#ifndef ROOT_TGeoVolume
#include "TGeoVolume.h"
#endif

#ifndef ROOT_TGButton
#include "TGButton.h"
#endif

#ifndef ROOT_TGLViewer
#include "TGLViewer.h"
#endif

#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif

#ifndef ROOT_TList
#include "TList.h"
#endif

#ifndef ROOT_TMath
#include "TMath.h"
#endif

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

#ifndef ROOT_TPad
#include "TPad.h"
#endif

#ifndef ROOT_TPaveLabel
#include "TPaveLabel.h"
#endif

#ifndef ROOT_TSystem
#include "TSystem.h"
#endif

#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif

#ifndef ROOT_TVector3
#include "TVector3.h"
#endif

#ifndef ROOT_TVirtualPad
#include "TVirtualPad.h"
#endif

#include "TAGeventDisplay.hxx" 

#include "TAVTparMap.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTactNtuVertexPD.hxx"

#include "TAVTactNtuVertex.hxx"

#include "TAVTntuVertex.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuCluster.hxx"

//
ClassImp(TAVTbaseEventDisplay)

Bool_t TAVTbaseEventDisplay::fgPostTracking = false;
//__________________________________________________________
TAVTbaseEventDisplay::TAVTbaseEventDisplay(const TString name, Int_t type) 
  : TAGeventDisplay(type),
    fVtxClusDisplay(new TAGclusterDisplay("Vertex Cluster")),
    fVtxTrackDisplay(new TAGtrackDisplay("Vertex Tracks"))
{ 
  // default constructor
   
   fType = type;
   fFileName = name;
   
   fVtxClusDisplay->SetMaxEnergy(fMaxEnergy);
   fVtxClusDisplay->SetDefWidth(fQuadDefWidth);
   fVtxClusDisplay->SetDefHeight(fQuadDefHeight);
   fVtxClusDisplay->SetPickable(true);
   
   fVtxTrackDisplay->SetMaxEnergy(fMaxEnergy);
   fVtxTrackDisplay->SetDefWidth(fBoxDefWidth);
   fVtxTrackDisplay->SetDefHeight(fBoxDefHeight);
   fVtxTrackDisplay->SetPickable(true);
}

//__________________________________________________________
TAVTbaseEventDisplay::~TAVTbaseEventDisplay()
{
  // default destructor
   delete fVtxClusDisplay;
   delete fVtxTrackDisplay;
   delete fpFirstGeo;
   delete fListOfCanvases;
   delete fListOfPads;
   
   fAGRoot->EndEventLoop();
   delete fAGRoot;
}

//__________________________________________________________
void TAVTbaseEventDisplay::CreateRecAction()
{
   fpNtuClus   = new TAGdataDsc("vtClus",  new TAVTntuCluster());
   fpNtuTrack  = new TAGdataDsc("vtTrack", new TAVTntuTrack());
   fpNtuVtx    = new TAGdataDsc("vtVtx",   new TAVTntuVertex()  );//NEW
    
   // Trafo
   fpFirstGeo =  new TAGgeoTrafo();
   fpFirstGeo->InitGeo("./geomaps/FIRST_geo.map");
   fTgZ = fpFirstGeo->GetVTCenter()[2]*TAGgeoTrafo::CmToMu();

   fActClus   = new TAVTactNtuClusterF("vtActClus", fpNtuRaw, fpNtuClus, fpConfig, fpGeoMap);
   fActClus->CreateHistogram();
   fActTrack  = new TAVTactNtuTrack("vtActTrack", fpNtuClus, fpNtuTrack, fpConfig, fpGeoMap);
   fActTrack->CreateHistogram();
   fActVtx    = new TAVTactNtuVertexPD("vtActVtx", fpNtuTrack, fpNtuVtx, fpConfig, fpGeoMap);
   fActVtx->CreateHistogram();
   
   if (fgPostTracking) {
	  fpNtuPostTrack = new TAGdataDsc("vtPostTrack", new TAVTntuTrack());
	  fActPostTrack  = new TAVTactNtuPostTrack("vtActPostTrack", fpNtuClus, fpNtuPostTrack, fpConfig, fpGeoMap, fpNtuVtx);
   }
}

//__________________________________________________________
void TAVTbaseEventDisplay::BuildDefaultGeometry()
{   
   TAVTparGeo* geoMap = (TAVTparGeo*) fpGeoMap->Object();

   TGeoVolume* vtVol = geoMap->BuildVertex();
   TGeoVolume* tgVol = geoMap->AddTarget();

   if (fpFirstGeo) {	  
	  TVector3 vtPos = fpFirstGeo->GetVTCenter();
	  vtPos *=  TAGgeoTrafo::CmToMu();
	  TGeoTranslation trans(vtPos[0], vtPos[1], vtPos[2]-fTgZ);
	  TVector3 vtAng = fpFirstGeo->GetVTAngles();
	  TGeoRotation rot;
	  rot.RotateX(vtAng.Y()); rot.RotateY(vtAng.X()); rot.RotateZ(vtAng.Z());
	  
	  TGeoHMatrix  transfo;
	  transfo  = trans*rot;   
	  AddGeometry(vtVol, new TGeoHMatrix(transfo));
	  
	  
	  TVector3 tgPos = fpFirstGeo->GetVTCenter();
	  tgPos *=  TAGgeoTrafo::CmToMu();
	  TGeoTranslation tgTrans(tgPos[0], tgPos[1],tgPos[2]-fTgZ);
	  //Target angles are the same as the vtx ones (same box)
	  TVector3 tgAng = fpFirstGeo->GetVTAngles();
	  TGeoRotation tgRot;
	  tgRot.RotateX(tgAng.Y()); tgRot.RotateY(tgAng.X()); tgRot.RotateZ(tgAng.Z());
	  
	  TGeoHMatrix  tgTransfo;
	  tgTransfo  = tgTrans*tgRot;   
	  AddGeometry(tgVol, new TGeoHMatrix(tgTransfo));
   } else {
	  AddGeometry(vtVol);
	  AddGeometry(tgVol);
   }
}

//__________________________________________________________
void TAVTbaseEventDisplay::AddElements()
{
   fVtxClusDisplay->ResetHits();
   gEve->AddElement(fVtxClusDisplay);
	  
   fVtxTrackDisplay->ResetTracks();
   gEve->AddElement(fVtxTrackDisplay);
}

//__________________________________________________________
void TAVTbaseEventDisplay::ConnectElements()
{  
   fVtxClusDisplay->SetEmitSignals(true);
   fVtxClusDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAVTbaseEventDisplay", this, "UpdateHitInfo(TEveDigitSet*, Int_t)");
   
   fVtxTrackDisplay->SetEmitSignals(true);
   fVtxTrackDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "TAVTbaseEventDisplay", this, "UpdateTrackInfo(TEveDigitSet*, Int_t)");
}

//__________________________________________________________
void TAVTbaseEventDisplay::UpdateHitInfo(TEveDigitSet* qs, Int_t idx)
{
   TAGclusterDisplay* quadHits = dynamic_cast<TAGclusterDisplay*> (qs);
   TObject* obj = quadHits->GetId(idx);
   
   if(obj == 0x0) return;

   if (obj->InheritsFrom("TAVTcluster")) {
	  TAVTcluster* clus = (TAVTcluster*)obj;
	  if (clus == 0x0) return;
	  TVector3 pos = clus->GetPositionG();
	  fInfoView->AddLine( Form("Cluster # %3d\n", clus->GetNumber()) );
	  fInfoView->AddLine( Form("with %3d pixels in sensor %d\n", clus->GetPixelsN(), clus->GetPlaneNumber()) );
	  fInfoView->AddLine( Form("at position: (%7.1f %7.1f)\n", pos.X(), pos.Y()) );
   } else if (obj->InheritsFrom("TAVTvertex")) {
	  TAVTvertex* vtx = (TAVTvertex*)obj;
	  if (vtx == 0x0) return;
	  TVector3 pos = vtx->GetVertexPosition();
	  fInfoView->AddLine( Form("Vertex # %d at position:\n", idx) );
	  fInfoView->AddLine( Form(" (%7.1f %7.1f %7.1f)\n", pos.X(), pos.Y(), pos.Z()) );
   } else {
	  return;
   }

}

//__________________________________________________________
void TAVTbaseEventDisplay::UpdateTrackInfo(TEveDigitSet* qs, Int_t idx)
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
	  fInfoView->AddLine( Form(" at position: (%7.1f %7.1f)\n", posG.X(), posG.Y()) );

   }
}

//__________________________________________________________
void TAVTbaseEventDisplay::UpdateElements()
{
   if (fgGUIFlag) 
	  fEventEntry->SetText(Form("Run %d Event %d", fAGRoot->CurrentRunInfo().RunNumber(), fAGRoot->CurrentEventId().EventNumber()));  
   
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
	  fVtxClusDisplay->ResetHits();
	  fVtxTrackDisplay->ResetTracks();
   }

   if (!fgDisplayFlag) // do not update event display
	  return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   Float_t x1 = 0.,  y1 =0., z1 = 0.;
      
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetNSensors();
   Float_t posLastPlane  = geoMap->GetSensorPar(nPlanes-1).Position[2];
   
   if (posLastPlane > 0) 
	  posLastPlane *= 1.1;
   else 
	  posLastPlane *= 0.9;

   // clusters
   TAVTntuCluster* pNtuClus  = (TAVTntuCluster*) fpNtuClus->Object();

   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  
	  Int_t nclus = pNtuClus->GetClustersN(iPlane);
	  
	  if (nclus == 0) continue;
	  for (Int_t iClus = 0; iClus < nclus; ++iClus) {
		 TAVTcluster *clus = pNtuClus->GetCluster(iPlane, iClus);
		 TVector3 pos = clus->GetPositionG();
		 TVector3 posG = pos;
		 Int_t nPix = clus->GetListOfPixels()->GetEntries();
		 if (fpFirstGeo) {
			posG  = fpFirstGeo->FromVTLocalToGlobal(pos*TAGgeoTrafo::MuToCm());
			posG *=  TAGgeoTrafo::CmToMu();
		 }
		 x = posG(0);
		 y = posG(1);
		 z = posG(2) - fTgZ;
		 fVtxClusDisplay->AddHit(nPix, x, y, z);
		 fVtxClusDisplay->QuadId(clus);
	  } //end loop on hits
	  
   } //end loop on planes

   // vertex
   TAVTntuVertex* pNtuVtxPD = (TAVTntuVertex*) fpNtuVtx->Object();
   TAVTvertex*    vtxPD     = 0x0;//NEW
   TVector3 vtxPositionPD;
   
   if (fpNtuVtx->Valid()) { 
	  for (Int_t iVtx = 0; iVtx < pNtuVtxPD->GetVertexN(); ++iVtx) {
		 vtxPD = pNtuVtxPD->GetVertex(iVtx);
		 if (vtxPD == 0x0) continue;
		 vtxPositionPD = vtxPD->GetVertexPosition();
		 z = vtxPositionPD[2];
		 if (fpFirstGeo) {
			vtxPositionPD  = fpFirstGeo->FromVTLocalToGlobal(vtxPositionPD*TAGgeoTrafo::MuToCm());
			vtxPositionPD *= TAGgeoTrafo::CmToMu();
		 }			
		 
		 fVtxClusDisplay->AddHit(50, vtxPositionPD.X(), vtxPositionPD.Y(), z);
		 fVtxClusDisplay->QuadId(vtxPD);
		 
		 Int_t nTracks = vtxPD->GetTracksN();
		 for( Int_t iTrack = 0; iTrack < nTracks; ++iTrack ) {
			TAVTtrack* track = vtxPD->GetTrack(iTrack);
			fVtxTrackDisplay->AddNewTrack();
			
			TVector3 pos  = track->Intersection(track->GetVertexZ());
			TVector3 posG = pos;
			if (fpFirstGeo) {
			   posG  = fpFirstGeo->FromVTLocalToGlobal(pos*TAGgeoTrafo::MuToCm());
			   posG *=  TAGgeoTrafo::CmToMu();
			}
			x = posG(0); y = posG(1); z = posG(2) - fTgZ;
			
			pos = track->Intersection(posLastPlane);
			if (fpFirstGeo) {
			   posG  = fpFirstGeo->FromVTLocalToGlobal(pos*TAGgeoTrafo::MuToCm());
			   posG *=  TAGgeoTrafo::CmToMu();
			}
			x1 = posG(0); y1 = posG(1); z1 = posG(2) - fTgZ;
			
			Int_t nClus = track->GetClustersN();
			fVtxTrackDisplay->AddTracklet(nClus*100, x, y, z, x1, y1, z1);
			fVtxTrackDisplay->TrackId(track);
		 } // end loop on tracks
	  }
   }   

   fVtxClusDisplay->RefitPlex();
   fVtxTrackDisplay->RefitPlex(); 
   gEve->FullRedraw3D(kFALSE);
}

//__________________________________________________________
void TAVTbaseEventDisplay::CreateCanvases()
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
 
   // histo 2D
   TAVTparGeo* geoMap   = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes = geoMap->GetSensorsN();      
   
   TCanvas* canvas = 0x0;
   TVirtualPad* pad    = 0x0;
   TEveWindowSlot* slot0 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab0 = slot0->MakeTab();
   tab0->SetElementName("Histo2D");
   tab0->SetShowTitleBar(kFALSE);
   
   // canvas tab 
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas00 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame00 = slot0->MakeFrame(eCanvas00);
   frame00->SetElementName("Clusters 2D");
   canvas = eCanvas00->GetCanvas();
   canvas->SetName("VtxClus2D");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxClus2D","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas01 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame01 = slot0->MakeFrame(eCanvas01);
   frame01->SetElementName("Pixel Pos 2D");
   canvas = eCanvas01->GetCanvas();
   canvas->SetName("VtxPixelPos2D");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxPixelPos2D","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas02 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame02 = slot0->MakeFrame(eCanvas02);
   frame02->SetElementName("Pixel Raw 2D");
   canvas = eCanvas02->GetCanvas();
   canvas->SetName("VtxPixelRaw2D");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxPixelRaw2D","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot0 = tab0->NewSlot();
   TRootEmbeddedCanvas* eCanvas03 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame03 = slot0->MakeFrame(eCanvas03);
   frame03->SetElementName("Beam Profile");
   canvas = eCanvas03->GetCanvas();
   canvas->SetName("VtxBeam2D");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxBeam2D","",0.,0.,1.,1);
   pad->Draw();
   fListOfPads->Add(pad);
	
	slot0 = tab0->NewSlot();
	TRootEmbeddedCanvas* eCanvas04 = new TRootEmbeddedCanvas();
	TEveWindowFrame* frame04 = slot0->MakeFrame(eCanvas04);
	frame04->SetElementName("Hough Transform XZ space");
	canvas = eCanvas04->GetCanvas();
	canvas->SetName("MatrixTrackingXZ");
	fListOfCanvases->Add(canvas);
	pad = new TPad("MatrixTrackingXZ","",0.,0.,1.,1);
	pad->Draw();
	fListOfPads->Add(pad);
	
	slot0 = tab0->NewSlot();
	TRootEmbeddedCanvas* eCanvas05 = new TRootEmbeddedCanvas();
	TEveWindowFrame* frame05 = slot0->MakeFrame(eCanvas05);
	frame05->SetElementName("Hough Transform YZ space");
	canvas = eCanvas05->GetCanvas();
	canvas->SetName("MatrixTrackingYZ");
	fListOfCanvases->Add(canvas);
	pad = new TPad("MatrixTrackingYZ","",0.,0.,1.,1);
	pad->Draw();
	fListOfPads->Add(pad);
   
   // other histo tab
   TEveWindowSlot* slot1 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab1 = slot1->MakeTab();
   tab1->SetElementName("Histos");
   tab1->SetShowTitleBar(kFALSE);
   
   // Residuals
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas10 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame10 = slot1->MakeFrame(eCanvas10);
   frame10->SetElementName("Total Residual");
   canvas = eCanvas10->GetCanvas();
   canvas->SetName("VtxTotRes");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxTotRes","",0.,0.,1.,1);
   pad->Divide(2, 1);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas11 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame11 = slot1->MakeFrame(eCanvas11);
   frame11->SetElementName("Individual Residual X");
   canvas = eCanvas11->GetCanvas();
   canvas->SetName("VtxIndResX");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxIndResX","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas12 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame12 = slot1->MakeFrame(eCanvas12);
   frame12->SetElementName("Individual Residual Y");
   canvas = eCanvas12->GetCanvas();
   canvas->SetName("VtxIndResY");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxIndResY","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   // rates tab
   TEveWindowSlot* slot10 = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
   TEveWindowTab*  tab10 = slot10->MakeTab();
   tab10->SetElementName("Rates");
   tab10->SetShowTitleBar(kFALSE);
   
   slot10 = tab10->NewSlot();
   TRootEmbeddedCanvas* eCanvas13 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame13 = slot10->MakeFrame(eCanvas13);
   frame13->SetElementName("Individual Rates");
   canvas = eCanvas13->GetCanvas();
   canvas->SetName("VtxIndRates");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxIndRates","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas14 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame14 = slot1->MakeFrame(eCanvas14);
   frame14->SetElementName("Individual # pixels/Cluster");
   canvas = eCanvas14->GetCanvas();
   canvas->SetName("VtxIndPixClus");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxIndPixClus","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   // canvas Chi2
   slot1 = tab1->NewSlot();
   TRootEmbeddedCanvas* eCanvas20 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame20 = slot1->MakeFrame(eCanvas20);
   frame20->SetElementName("Global Histo");
   canvas = eCanvas20->GetCanvas();
   canvas->SetName("VtxChi2");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxChi2","",0.,0.,1.,1);
   pad->Divide(2, 2);
   pad->Draw();
   fListOfPads->Add(pad);
   
   slot10 = tab10->NewSlot();
   TRootEmbeddedCanvas* eCanvas15 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame15 = slot10->MakeFrame(eCanvas15);
   frame15->SetElementName("Individual length/evt");
   canvas = eCanvas15->GetCanvas();
   canvas->SetName("VtxIndLengthEvt");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxIndLengthEvt","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();   
   fListOfPads->Add(pad);
   
   slot10 = tab10->NewSlot();
   TRootEmbeddedCanvas* eCanvas16 = new TRootEmbeddedCanvas();
   TEveWindowFrame* frame16 = slot10->MakeFrame(eCanvas16);
   frame16->SetElementName("Individual QRates");
   canvas = eCanvas16->GetCanvas();
   canvas->SetName("VtxIndQRates");
   fListOfCanvases->Add(canvas);
   pad = new TPad("VtxIndQRates","",0.,0.,1.,1);
   pad->Divide( (Int_t)ceil(nPlanes/2.), 2);
   pad->Draw();
   fListOfPads->Add(pad);
}

//______________________________________________________________________________
void TAVTbaseEventDisplay::DrawReco()
{
   TAVTparGeo* geoMap    = (TAVTparGeo*) fpGeoMap->Object();
   Int_t nPlanes         = geoMap->GetNSensors();
   
   TList* histoClus  = fActClus->GetHistogrammList();
   TList* histoTrack = fActTrack->GetHistogrammList();
   TVirtualPad* pad = 0x0;
   
//   pad = (TVirtualPad*)fListOfPads->FindObject("VtxChi2");
//   TH1F* pHisPixelTot = (TH1F*)histoClus->FindObject("vtClusPixelTot");
//   pad->cd(4);
//   pHisPixelTot->Draw();
	pad = (TVirtualPad*)fListOfPads->FindObject("VtxChi2");
	TH1F* pHisTrackClus = (TH1F*)histoTrack->FindObject("vtTrackClus");
	pad->cd(4);
	pHisTrackClus->Draw();
   
   TH1F* pHisChi2TotX = (TH1F*)histoTrack->FindObject("vtChi2TotX");
   pad->cd(1);
   pHisChi2TotX->Draw();
   
   TH1F* pHisChi2TotY = (TH1F*)histoTrack->FindObject("vtChi2TotY");
   pad->cd(2);
   pHisChi2TotY->Draw();
   
   TH1F* pHisTrackEvt = (TH1F*)histoTrack->FindObject("vtTrackEvt");
   pad->cd(3);
   pHisTrackEvt->Draw();
   
   pad = (TVirtualPad*)fListOfPads->FindObject("VtxTotRes");
   TH1F* pHisResTotX = (TH1F*)histoTrack->FindObject("vtResTotX");
   pad->cd(1);
   pHisResTotX->Draw();
   
   TH1F* pHisResTotY = (TH1F*)histoTrack->FindObject("vtResTotY");
   pad->cd(2);
   pHisResTotY->Draw();
      
   pad = (TVirtualPad*)fListOfPads->FindObject("VtxBeam2D");
   TH2F* pHisBeamProf = (TH2F*)histoTrack->FindObject("vtBeamProf");
   pad->cd();
   pHisBeamProf->Draw("colz");
	   
   TVirtualPad* pad0 = (TVirtualPad*)fListOfPads->FindObject("VtxClus2D");
   TVirtualPad* pad4 = (TVirtualPad*)fListOfPads->FindObject("VtxIndResX");
   TVirtualPad* pad5 = (TVirtualPad*)fListOfPads->FindObject("VtxIndResY");
   TVirtualPad* pad7 = (TVirtualPad*)fListOfPads->FindObject("VtxIndPixClus");

   for( Int_t iPlane = 0; iPlane < nPlanes; iPlane++) {
	  
	  TH2F* pHisClusMap = (TH2F*)histoClus->FindObject(Form("vtClusMap%d", iPlane+1));
	  pad0->cd(iPlane+1);
	  pHisClusMap->Draw("P"); 
	  
	  TH2F* pHisTrackMap = (TH2F*)histoTrack->FindObject(Form("vtTrackMap%d", iPlane+1));
	  pad0->cd(iPlane+1);
	  pHisTrackMap->Draw("SAME"); 
	  
	  TH1F* pHisResX = (TH1F*)histoTrack->FindObject(Form("vtResX%d", iPlane+1));
	  pad4->cd(iPlane+1);
	  pHisResX->Draw(""); 
	  
	  TH1F* pHisResY = (TH1F*)histoTrack->FindObject(Form("vtResY%d", iPlane+1));
	  pad5->cd(iPlane+1);
	  pHisResY->Draw(""); 
	  
	  TH1F* pHisPixel = (TH1F*)histoClus->FindObject(Form("vtClusPixel%d", iPlane+1));
	  pad7->cd(iPlane+1);
	  pHisPixel->Draw();
   }
}

//__________________________________________________________
void TAVTbaseEventDisplay::UpdateDefCanvases()
{
   DrawReco();
   DrawRawdata();
   
   TCanvas* canvas = 0x0;
   if (fClusterButton->IsOn()) {
	  canvas = (TCanvas*)fListOfCanvases->FindObject("VtxClus2D");
	  canvas->Modified();
	  canvas->Update();
   }
   
   if (fRawDataButton->IsOn()) {
	  
	  canvas = (TCanvas*)fListOfCanvases->FindObject("VtxPixelPos2D");
	  canvas->Modified();
	  canvas->Update();
	  
	  canvas = (TCanvas*)fListOfCanvases->FindObject("VtxPixelRaw2D");
	  canvas->Modified();
	  canvas->Update();
   }
   
   canvas = (TCanvas*)fListOfCanvases->FindObject("VtxTotRes");
   canvas->Modified();
   canvas->Update();
   
   canvas = (TCanvas*)fListOfCanvases->FindObject("VtxIndResX");
   canvas->Modified();
   canvas->Update();
   
   canvas = (TCanvas*)fListOfCanvases->FindObject("VtxIndResY");
   canvas->Modified();
   canvas->Update();
    
   canvas = (TCanvas*)fListOfCanvases->FindObject("VtxIndPixClus");
   canvas->Modified();
   canvas->Update();
   
   canvas = (TCanvas*)fListOfCanvases->FindObject("VtxChi2");
   canvas->Modified();
   canvas->Update(); 
   
   canvas = (TCanvas*)fListOfCanvases->FindObject("VtxBeam2D");
   canvas->Modified();
   canvas->Update(); 
}

//__________________________________________________________
void TAVTbaseEventDisplay::UpdateFreqCanvases()
{
   TCanvas* canvas = 0x0;
   DrawRate();

   if (fRateButton->IsOn()) {
	  canvas = (TCanvas*)fListOfCanvases->FindObject("VtxIndRates");
	  canvas->Modified();
	  canvas->Update();
	  
	  canvas = (TCanvas*)fListOfCanvases->FindObject("VtxIndQRates");
	  canvas->Modified();
	  canvas->Update();
	  
	  canvas = (TCanvas*)fListOfCanvases->FindObject("VtxIndLengthEvt");
	  canvas->Modified();
	  canvas->Update();
   }
   
   ResetRate();
}

