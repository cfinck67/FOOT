
#include "Riostream.h"

#ifndef _EventDisplay_HXX_
#include "EventDisplay.hxx"
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
#include "TAGmbsEvent.hxx"
#include "TAGmbsEventFilterType.hxx"

// TOF
#include "TATactNtuRaw.hxx"
#include "TATactDatRaw.hxx"
#include "TATdatRaw.hxx"
#include "TATntuRaw.hxx"
#include "TATparGeo.hxx"
#include "TATparMap.hxx"
#include "TATparCal.hxx"

// Aladin
#include "TADIparGeo.hxx"

ClassImp(EventDisplay)

EventDisplay* EventDisplay::fgInstance = 0x0;

//__________________________________________________________
EventDisplay* EventDisplay::Instance(const TString name)
{
   if (fgInstance == 0x0) 
	  fgInstance = new EventDisplay(name);

   return fgInstance;
}

//__________________________________________________________
EventDisplay::EventDisplay(const TString name)
: TAIReventDisplay(name),
   fTofClusDisplay(new TAGclusterDisplay("TOF Hit")),
   fTofFlag(true)
{ 
   // default constructor   

   if (fTofFlag) {   
	  fTofClusDisplay->SetMaxEnergy(fMaxEnergy);
	  fTofClusDisplay->SetDefWidth(2.5*TAGgeoTrafo::CmToMu());
	  fTofClusDisplay->SetDefHeight(2.5*TAGgeoTrafo::CmToMu());
	  fTofClusDisplay->SetPickable(true);
   }    
}

//__________________________________________________________
EventDisplay::~EventDisplay()
{
   // default destructor
   delete fTofClusDisplay; 
}

//__________________________________________________________
void EventDisplay::CreateRawAction()
{
   // IR
   TAIReventDisplay::CreateRawAction();
   
   if (fTofFlag) {
	  // map
	  fpTofMap  = new TAGparaDsc("bmMap", new TATparMap());
	  TATparMap* o_tofmap = (TATparMap*) fpTofMap->Object();
	  o_tofmap->Clear();
	  Bool_t b_bad = kTRUE;
	  
	  TString filename = "geomaps/s254_twal.map";
	  b_bad = o_tofmap->FromFile(filename);
	  if (!b_bad) fpTofMap->SetBit(TAGparaDsc::kValid);
	  
	  // cal
	  fpTofCal = new TAGparaDsc("bmCon", new TATparCal());
	  TATparCal* o_tofcal = (TATparCal*) fpTofCal->Object();
	  o_tofcal->Clear();
	  b_bad = kTRUE;
	  
	  filename = "config/TOF_calib_postSweep.txt";
	  b_bad = o_tofcal->LoadFromFile(filename,"./");
	  if (!b_bad) fpTofCal->SetBit(TAGparaDsc::kValid);
	  
	  // cal zid
	  b_bad = kTRUE;
	  filename = "config/zid_calib.txt";	  
	  b_bad = o_tofcal->ZIDFromFile(filename);
	  if (!b_bad) fpTofCal->SetBit(TAGparaDsc::kValid);
	  
	  //geo
	  fpTofGeo = new TAGparaDsc("tofGeo", new TATparGeo());
	  TATparGeo* geoTofMap = (TATparGeo*) fpTofGeo->Object();
	  geoTofMap->InitGeo();
	  fpTofGeo->SetBit(TAGparaDsc::kValid);

	  // aladin
	  fpAlaGeo = new TAGparaDsc("alaGeo", new TADIparGeo());
	  fpAlaGeo->SetBit(TAGparaDsc::kValid);

	  // mbs
	  fpTofEvent = new TAGdataDsc("tofEvent", new TAGmbsEvent());
	  TAGmbsEventFilterType* tofFilter = new TAGmbsEventFilterType();
	  tofFilter->AddType(32,1000,1999); //Allowed sub event types!
	  fActEvtReader->SetupChannel(fpTofEvent, tofFilter);
	  
	  // raw & ntu
	  fpTofRaw  = new TAGdataDsc("tofRaw", new TATdatRaw());
	  fpTofNtu   = new TAGdataDsc("tofNtu", new TATntuRaw());

	  fActTofRaw = new TATactDatRaw("tofActRaw", fpTofRaw, fpTofEvent, fpTofMap);
	  fActTofNtu = new TATactNtuRaw("tofActNtu", fpTofNtu, fpTofRaw, fpTrRaw, fpTofCal);
   }
}

//__________________________________________________________
void EventDisplay::CreateRecAction()
{
   // IR
   TAIReventDisplay::CreateRecAction();
}

//__________________________________________________________
void EventDisplay::BuildDefaultGeometry()
{
   TAIReventDisplay::BuildDefaultGeometry();
   
   TATparGeo*  geoTofMap = (TATparGeo*)  fpTofGeo->Object();
   TADIparGeo* geoAlaMap = (TADIparGeo*) fpAlaGeo->Object();

   if(!fpFirstGeo) return;

   //TOF
   if (fTofFlag) {
	  // wall F
	  TVector3 wallFPos = fpFirstGeo->GetTWFCenter();
	  wallFPos *=  TAGgeoTrafo::CmToMu();
	  TGeoTranslation transF(wallFPos[0], wallFPos[1], wallFPos[2] - fTgZ);
	  TVector3 wallFAng = fpFirstGeo->GetTWFAngles();
	  TGeoRotation rotF;
	  rotF.RotateX(wallFAng.Y()); rotF.RotateY(wallFAng.X()); rotF.RotateZ(wallFAng.Z());
	  
	  TGeoHMatrix transfoF = transF*rotF;   
	  TGeoVolume* wallF    = geoTofMap->BuildWallF();
	  AddGeometry(wallF, new TGeoHMatrix(transfoF));
	  
	  // Wall R
	  TVector3 wallRPos = fpFirstGeo->GetTWRCenter();
	  wallRPos *=  TAGgeoTrafo::CmToMu();
	  TGeoTranslation transR(wallRPos[0], wallRPos[1], wallRPos[2] - fTgZ);
	  TVector3 wallRAng = fpFirstGeo->GetTWRAngles();
	  TGeoRotation rotR;
	  rotR.RotateX(wallRAng.Y()); rotR.RotateY(wallRAng.X()); rotR.RotateZ(wallRAng.Z());
	  
	  TGeoHMatrix transfoR = transR*rotR;  		 
	  TGeoVolume* wallR    = geoTofMap->BuildWallR();	
	  AddGeometry(wallR, new TGeoHMatrix(transfoR));
	  
	  // Aladin
	  TVector3 alaFPos = fpFirstGeo->GetALCenter();
	  alaFPos *=  TAGgeoTrafo::CmToMu();
	  TGeoTranslation transA(alaFPos[0], alaFPos[1], alaFPos[2] - fTgZ);
	  TVector3 alaAng  = fpFirstGeo->GetALAngles();
	  TGeoRotation rotA;
	  rotA.RotateX(alaAng.Y()); rotA.RotateY(alaAng.X()); rotA.RotateZ(alaAng.Z());
	  
	  TGeoHMatrix transfoA = transA*rotA;   
	  TGeoVolume* dipole    = geoAlaMap->AddDipole();
	  AddGeometry(dipole, new TGeoHMatrix(transfoA));
   }
}

//__________________________________________________________
void EventDisplay::CreateCanvases()
{
   TAIReventDisplay::CreateCanvases();   
}

//__________________________________________________________
void EventDisplay::UpdateDefCanvases()
{
   TAIReventDisplay::UpdateDefCanvases();
}

//______________________________________________________________________________
void EventDisplay::DrawReco()
{
   TAIReventDisplay::DrawReco();
}   

//__________________________________________________________
void EventDisplay::AddElements()
{
   TAIReventDisplay::AddElements();
   
   if (fTofFlag) {
	  fTofClusDisplay->ResetHits();
	  gEve->AddElement(fTofClusDisplay);
   }
}
  
//__________________________________________________________
void EventDisplay::ConnectElements()
{  
   TAIReventDisplay::ConnectElements();

   fTofClusDisplay->SetEmitSignals(true);
   fTofClusDisplay->Connect("SecSelected(TEveDigitSet*, Int_t )", "EventDisplay", this, "UpdateTofInfo(TEveDigitSet*, Int_t)");   
}

//__________________________________________________________
void EventDisplay::UpdateTofInfo(TEveDigitSet* qs, Int_t idx)
{
   TATparGeo* geoTofMap = (TATparGeo*) fpTofGeo->Object();
   TAGclusterDisplay* quadHits = dynamic_cast<TAGclusterDisplay*> (qs);   
   TATntuRawHit* hit = (TATntuRawHit*) quadHits->GetId(idx);
   if (hit == 0x0) return;
   
   Int_t iSlat = hit->slat;
   TVector3 el, er;	 
   geoTofMap->GeoSlatFace(el, er, iSlat);
   TVector3 pos = (er+el)*0.5*TAGgeoTrafo::MmToCm();
   pos[1] = hit->Y()*TAGgeoTrafo::MmToCm();
   TVector3 posG;
   
   if (geoTofMap->IsInFrontWall(iSlat)) 
	  posG = fpFirstGeo->FromTWFLocalToGlobal(pos);
   else 
	  posG = fpFirstGeo->FromTWRLocalToGlobal(pos);
   
   Float_t tof = hit->ToF();
   Float_t chg = hit->Chg();
   
   fInfoView->AddLine( Form("ToF: hit in slat %d \n", iSlat) );
   fInfoView->AddLine( Form(" Pos: (%4.1f %4.1f %6.1f) cm \n", posG[0], posG[1], posG[2]) );
   fInfoView->AddLine( Form(" Tof: %4.1f ns, Chg: %4.0f\n", tof, chg) );
}

//__________________________________________________________
void EventDisplay::UpdateElements()
{      
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
	  if (fTofFlag) {
		 fTofClusDisplay->ResetHits();
	  }
   }
   
   if(!fpFirstGeo) return;
   
   TAIReventDisplay::UpdateElements();
   
   TATparGeo* geoTofMap = (TATparGeo*) fpTofGeo->Object();
   TATntuRaw* pNtuHit   = (TATntuRaw*) fpTofNtu->Object();

   //Tof
   if (fTofFlag) {
	  // reset previous fired slat
	  if ( fRefreshButton->IsOn()) {
		 for (Int_t i = 0; i < fFiredSlat.GetSize(); ++i) 
			geoTofMap->SetSlatColorOff(fFiredSlat[i]);		 
	  }
		  
	  fFiredSlat.Set(pNtuHit->nhit);
	  for( Int_t iHit = 0; iHit < pNtuHit->nhit; iHit++) {
		 
		 TATntuRawHit* hit = pNtuHit->Hit(iHit);
		 
		 Int_t iSlat = hit->slat;
		 if (!TATparGeo::IsValidSlat(iSlat))
			continue;
		 
		 if (!hit->tdct || !hit->tdcb)
			continue;//only hits in coincidence with tdc
		 
		  if (!hit->adct || !hit->adcb)
			  continue;
			  
		 // Position
		 fFiredSlat.AddAt(iSlat, iHit);
		 geoTofMap->SetSlatColorOn(iSlat);		 
				 
		 TVector3 pos = geoTofMap->GetSlatPos(iSlat)*TAGgeoTrafo::MmToCm();
		 pos[1] = hit->Y()*TAGgeoTrafo::MmToCm();
		 TVector3 posG;
		
		 if (geoTofMap->IsInFrontWall(iSlat)) 
			posG = fpFirstGeo->FromTWFLocalToGlobal(pos);
		  else 
			posG = fpFirstGeo->FromTWRLocalToGlobal(pos);
		 posG *=  TAGgeoTrafo::CmToMu();

		 Float_t x = posG(0);
		 Float_t y = posG(1);
		 Float_t z = posG(2) - fTgZ;
		 
		 fTofClusDisplay->AddHit(500, x, y, z);
		 fTofClusDisplay->QuadId(hit);
	  } //end loop on hits		 
   }
   gEve->FullRedraw3D(kFALSE);
}

//__________________________________________________________
void EventDisplay::AddRequiredItem()
{
     //Tof
   if(fTofFlag) {
	  fAGRoot->AddRequiredItem("tofActRaw");
	  fAGRoot->AddRequiredItem("tofActNtu");
   }

   // IR
   TAIReventDisplay::AddRequiredItem();
   
}
