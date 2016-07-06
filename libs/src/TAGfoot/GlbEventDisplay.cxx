
#include "Riostream.h"

#ifndef _GlbEventDisplay_HXX_
#include "GlbEventDisplay.hxx"
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
#include "TAGactTreeReader.hxx"

//vtx
#include "TAVTparMap.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTntuVertex.hxx"

//Beam Monitor
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMparMap.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuTrack.hxx"

//Kentros
#include "TAKEntuRaw.hxx"
#include "TAKEntuTrack.hxx"
#include "TAKEparGeo.hxx"
#include "TAKEparEd.hxx"
#include "TAKEparMap.hxx"

//TOF
#include "TATparGeo.hxx"
#include "TATparMap.hxx"
#include "TATparCal.hxx"
#include "TATdatRaw.hxx"
#include "TATntuRaw.hxx"

// Aladin
#include "TADIparGeo.hxx"

// GLB
#include "TAGntuGlbTracks.hxx"
#include "TAGactGlbTracking.hxx"
#include "TAGtrackDisplay.hxx"

ClassImp(GlbEventDisplay)

GlbEventDisplay* GlbEventDisplay::fgInstance = 0x0;

//__________________________________________________________
GlbEventDisplay* GlbEventDisplay::Instance(const TString name)
{
   if (fgInstance == 0x0) 
	  fgInstance = new GlbEventDisplay(name);

   return fgInstance;
}

//__________________________________________________________
GlbEventDisplay::GlbEventDisplay(const TString name)
: EventDisplay(name),
  fGlbTrackDisplay(new TAGtrackDisplay("Global Tracks")),
  fVtxFlag(true),
  fGlbFlag(false),
  fMagCurrent(730)
{ 
   // default constructor   
   
   fGlbTrackDisplay->SetMaxEnergy(fMaxEnergy);
   fGlbTrackDisplay->SetDefWidth(fQuadDefWidth*2);
   fGlbTrackDisplay->SetDefHeight(fQuadDefHeight*2);
   fGlbTrackDisplay->SetPickable(true);
}

//__________________________________________________________
GlbEventDisplay::~GlbEventDisplay()
{
   // default destructor
   delete fGlbTrackDisplay; 
}

//__________________________________________________________
void GlbEventDisplay::CreateRawAction()
{
   fActEvtReader = new TAGactTreeReader("vtActEvtReader");

  if (fVtxFlag) {
		
	 fpGeoMap    = new TAGparaDsc("vtGeo", new TAVTparGeo());
	 TAVTparGeo* geoMap   = (TAVTparGeo*) fpGeoMap->Object();
	 if (geoMap->FromFile("")) exit(0);
	 
	 fpConfig  = new TAGparaDsc("vtConf", new TAVTparConf());
	 TAVTparConf* parconf = (TAVTparConf*) fpConfig->Object();
	 if (parconf->FromFile("")) exit(0);
  }
   
   if (fBmFlag) {
	  Bool_t b_bad = kTRUE;
	  fpBmMap  = new TAGparaDsc("bmMap", new TABMparMap());
	  TABMparMap* pbmMap = (TABMparMap*) fpBmMap->Object();
	  pbmMap->Clear();
	  TString filename = "./geomaps/beammonitor_geoch.map";
	  b_bad    = pbmMap->FromFile(filename);
	  if (!b_bad) fpBmMap->SetBit(TAGparaDsc::kValid);   
	  
	  //BM con
	  b_bad = kTRUE;
	  fpBmConf = new TAGparaDsc("bmCon", new TABMparCon());
	  TABMparCon* pbmCon = (TABMparCon*) fpBmConf->Object();
	  pbmCon->Clear();
	  filename = "./config/beammonitor.cfg";
	  b_bad    = pbmCon->FromFile(filename);
	  filename = "./config/beammonitor_t0s.cfg";
	  pbmCon->loadT0s(filename);
	  filename = "./config/file_stlist_FIRST.txt";	  
	  pbmCon->LoadSTrel(filename);
	  filename = "config/bmreso_vs_r.root";
	  pbmCon->LoadReso(filename);
	  if (!b_bad) fpBmConf->SetBit(TAGparaDsc::kValid);   
	  
	  // BM geo
	  //Initialize the geometry of the beam monitor: this maps hold info on the
	  //TDC channels status: the mapping of TDC channels into Chamber wires happens 
	  fpBmGeo  = new TAGparaDsc("bmGeo", new TABMparGeo());
	  TABMparGeo* pbmGeo = (TABMparGeo*) fpBmGeo->Object();
	  pbmGeo->InitGeo();
	  fpBmGeo->SetBit(TAGparaDsc::kValid);
	  
	  fpBmNtu   = new TAGdataDsc("bmNtu", new TABMntuRaw());
   	  fActEvtReader->SetupBranch(fpBmNtu, "bmrh.");
   }
   
   if (fKeFlag) {
	  fpKeMap  = new TAGparaDsc("keMap", new TAKEparMap());
	  TAKEparMap* pKeMap = (TAKEparMap*) fpKeMap->Object();
	  pKeMap->Clear();
	  Bool_t b_bad = kTRUE;
	  TString filename = "./geomaps/kentros_ch.map";
	  b_bad = pKeMap->FromFile(filename);
	  if (!b_bad) pKeMap->SetBit(TAGparaDsc::kValid);
	  
	  fpKeGeo = new TAGparaDsc("keGeo", new TAKEparGeo());
	  TAKEparGeo* pKeGeo = (TAKEparGeo*) fpKeGeo->Object();
	  //Initialization of KE parameters
	  pKeGeo->InitGeo();
	  pKeGeo->SetBit(TAGparaDsc::kValid);
	  
	  fpKeNtu   = new TAGdataDsc("keNtu", new TAKEntuRaw());
	  fActEvtReader->SetupBranch(fpKeNtu, "kerh.");
   }
   
   if (fTofFlag) {
	  // map
	  fpTofMap  = new TAGparaDsc("bmMap", new TATparMap());
	  TATparMap* o_tofmap = (TATparMap*) fpTofMap->Object();
	  o_tofmap->Clear();
	  Bool_t b_bad = kTRUE;
	  
	  TString filename = "geomaps/s254_twal.map";
	  b_bad = o_tofmap->FromFile(filename);
	  if (!b_bad) fpTofMap->SetBit(TAGparaDsc::kValid);

	  //geo
	  fpTofGeo =   new TAGparaDsc("tofGeo", new TATparGeo());
	  TATparGeo* geoTofMap = (TATparGeo*) fpTofGeo->Object();
	  geoTofMap->InitGeo();
	  fpTofGeo->SetBit(TAGparaDsc::kValid);

	  // aladin
	  fpAlaGeo = new TAGparaDsc("alaGeo", new TADIparGeo());
	  fpAlaGeo->SetBit(TAGparaDsc::kValid);
	  
	  fpTofNtu   = new TAGdataDsc("tofNtu", new TATntuRaw());
	  fActEvtReader->SetupBranch(fpTofNtu, "trh.");
   }
}

//__________________________________________________________
void GlbEventDisplay::CreateRecAction()
{
   // Trafo
   fpFirstGeo = new TAGgeoTrafo();
   fpFirstGeo->InitGeo("./geomaps/FIRST_geo.map");
   fTgZ = fpFirstGeo->GetVTCenter()[2]*TAGgeoTrafo::CmToMu();
   
   if(fVtxFlag) {
	  fpNtuClus  = new TAGdataDsc("vtClus",  new TAVTntuCluster());
	  fpNtuTrack = new TAGdataDsc("vtTrack", new TAVTntuTrack());
	  fpNtuVtx   = new TAGdataDsc("vtVtx",   new TAVTntuVertex());
	  fActEvtReader->SetupBranch(fpNtuClus,  "vtclus.");
	  fActEvtReader->SetupBranch(fpNtuTrack, "vtTrack.");
	  fActEvtReader->SetupBranch(fpNtuVtx,   "vtVtx.");
   }
   
   if (fBmFlag) {
	  fpBmTrack = new TAGdataDsc("bmTrk", new TABMntuTrack());
	  fActEvtReader->SetupBranch(fpBmTrack, "bmtrk.");  
   }
   
   if (fKeFlag) {
	  fpKeTrack = new TAGdataDsc("keTrk", new TAKEntuTrack());
	  fActEvtReader->SetupBranch(fpKeTrack, "ketrk.");  
   }
   
   if (fGlbFlag) {
	  fpGlbTrack   = new TAGdataDsc("glbTrack", new TAGntuGlbTracks());
	  fActGlbTrack = new TAGactGlbTracking("glbActTrack",fpGlbTrack, fpNtuVtx, fpTofNtu, fpTofGeo, fMagCurrent, "./");
	  fActGlbTrack->CreateHistogram();
   }
   
   fActEvtReader->Open(fFileName.Data());
}

//__________________________________________________________
void GlbEventDisplay::UpdateElements()
{      
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
	  if (fGlbFlag) {
		 fGlbTrackDisplay->ResetTracks();
	  }
   }
      
   EventDisplay::UpdateElements();
   
   if (!fGlbFlag) return;
   if (!fpFirstGeo) return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   Float_t x1 = 0.,  y1 =0., z1 = 0.;

   TAGntuGlbTracks* pGlbtrk  =  (TAGntuGlbTracks*) fpGlbTrack->Object();
   
   vector<TVector3> trPath;

   for(int itr = 0; itr < pGlbtrk->nglbtrk; itr++) {
	  
	  TAGntuGlbTrk* track = pGlbtrk->Track(itr);
	  trPath     = track->Path();
	  Int_t size = trPath.size();
	  Int_t max_ite = size - 1;
	  	  
	  for(int ips = 0; ips < max_ite; ips++) {
		 
		 TVector3 pos = trPath.at(ips);
		 pos *=  TAGgeoTrafo::CmToMu();

		 x = pos(0); y = pos(1); z = pos(2) - fTgZ;
   
		 pos = trPath.at(ips+1);
		 pos *=  TAGgeoTrafo::CmToMu();
		 
		 x1 = pos(0); y1 = pos(1); z1 = pos(2) - fTgZ;
		 fGlbTrackDisplay->AddTracklet(100, x, y, z, x1, y1, z1);
		 fGlbTrackDisplay->TrackId(track);
	  }
   }
   
   gEve->FullRedraw3D(kFALSE);
}

//__________________________________________________________
void GlbEventDisplay::AddElements()
{
   EventDisplay::AddElements();
   
   if (fTofFlag) {
	  fGlbTrackDisplay->ResetTracks();
	  gEve->AddElement(fGlbTrackDisplay);
   }
}

//__________________________________________________________
void GlbEventDisplay::ConnectElements()
{  
   EventDisplay::ConnectElements();
   
   fGlbTrackDisplay->SetEmitSignals(true);
   fGlbTrackDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "GlbEventDisplay", this, "UpdateGlbInfo(TEveDigitSet*, Int_t)");   
}

//__________________________________________________________
void GlbEventDisplay::UpdateGlbInfo(TEveDigitSet* qs, Int_t idx)
{
   TAGtrackDisplay* lineTracks = dynamic_cast<TAGtrackDisplay*> (qs);      
   if (!lineTracks) return;
   
   TAGntuGlbTrk* track = (TAGntuGlbTrk*)lineTracks->GetId(idx);
    if (track == 0x0) return;
   
   fInfoView->AddLine( Form("Glb Track#: %d", track->GetId()) );
   fInfoView->AddLine( Form(" Mass:     %4.2f (GeV/c^2)", track->GetMass() ) );
   fInfoView->AddLine( Form(" Momentum: %4.2f (GeV/c)", track->GetMomentum() ) );
   fInfoView->AddLine( Form(" Charge:   %3.1f", track->GetCharge() ) );
   fInfoView->AddLine( Form(" TOF f/r:  %3.1f/%3.1f (ns)", track->GetToff()*1e9, track->GetTofr()*1e9 ) );
 }

//__________________________________________________________
void GlbEventDisplay::AddRequiredItem()
{
   fAGRoot->AddRequiredItem("vtActEvtReader");
   if (fGlbFlag) 
	  fAGRoot->AddRequiredItem("glbActTrack");
   fAGRoot->Print();
   fAGRoot->BeginEventLoop();
}
