#include <Riostream.h>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TEveGeoShapeExtract.h"
#include "TEveTrans.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TList.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TGeoMatrix.h"

#include "TAGmaterials.hxx"
#include "TAGgeoTrafo.hxx"

#include "TATWparGeo.hxx"
#include "TAGroot.hxx"


      TString TATWparGeo::fgkDefParaName     = "twGeo";
const TString TATWparGeo::fgkBaseName        = "TW";
      Int_t   TATWparGeo::fgkLayerOffset     = 100;
const Color_t TATWparGeo::fgkDefaultModCol   = kGray+1;
const Color_t TATWparGeo::fgkDefaultModColOn = kRed-8;
const TString TATWparGeo::fgkDefaultBarName = "twBar";


//_____________________________________________________________________________
TATWparGeo::TATWparGeo()
: TAGparTools()
{
   fkDefaultGeoName = "./geomaps/TATWdetector.map";
}

//______________________________________________________________________________
TATWparGeo::~TATWparGeo()
{
}

//______________________________________________________________________________
Bool_t TATWparGeo::FromFile(const TString& name)
{
   TString nameExp;
   
   if (name.IsNull())
      nameExp = fkDefaultGeoName;
   else
      nameExp = name;
   
   if (!Open(nameExp)) return false;
   
   ReadItem(fLayersN);
   if(fDebugLevel)
      cout << endl << "Number of layers "<< fLayersN << endl;
   
   ReadItem(fBarsN);
   if(fDebugLevel)
      cout  << "   Number of bars: " <<  fBarsN << endl;
   
   ReadStrings(fBarMat);
   if(fDebugLevel)
      cout  << "   Bars material : " <<  fBarMat << endl;
   
   ReadItem(fBarDensity);
   if(fDebugLevel)
      cout  << "   Bars density : " <<  fBarDensity << endl;
   
   ReadVector3(fBarSize);
   if(fDebugLevel)
      cout << "   Bar size: "
      << Form("%f %f %f", fBarSize[0], fBarSize[1], fBarSize[2]) << endl;
   
   // define material
   DefineMaterial();
   
   fSize.SetXYZ(fBarSize[1], fBarSize[1], fBarSize[2]*2);
   
   TVector3 position;
   TVector3 tilt;
   Int_t barId;
   Int_t layerId;
   
   SetupMatrices(fLayersN*fBarsN);
   
   // Read transformtion info
   for (Int_t iLayer = 0; iLayer < fLayersN; ++iLayer) {
      for (Int_t iBar = 0; iBar < fBarsN; ++iBar) {

         ReadItem(layerId);
         if(fDebugLevel)
            cout  << "Layer id "<< layerId << endl;
         
         ReadItem(barId);
         if(fDebugLevel)
            cout  << "Bar id "<< barId << endl;
         
         // read  position
         ReadVector3(position);
         if(fDebugLevel)
            cout << "   Position: "
            << Form("%f %f %f", position[0], position[1], position[2]) << endl;
         
         ReadVector3(tilt);
         if(fDebugLevel)
            cout  << "   Tilt: "
            << Form("%f %f %f", tilt[0], tilt[1], tilt[2]) << endl;

	 vTilt.push_back(tilt);
         
         TGeoRotation rot;
         rot.RotateX(tilt[0]);
         rot.RotateY(tilt[1]);
         rot.RotateZ(tilt[2]);
         
         TGeoTranslation trans(position[0], position[1], position[2]);
         
         TGeoHMatrix  transfo;
         transfo  = trans;
         transfo *= rot;
         Int_t idx = iLayer*fBarsN + iBar;
         AddTransMatrix(new TGeoHMatrix(transfo), idx);
      }
   }
   
   Close();
   return true;
}

//_____________________________________________________________________________
TGeoHMatrix* TATWparGeo::GetTransfo(Int_t iLayer, Int_t iBar)
{
   Int_t idx = iLayer*fBarsN + iBar;

   return TAGparTools::GetTransfo(idx);
}

//_____________________________________________________________________________
TVector3 TATWparGeo::GetBarPosition(Int_t iLayer, Int_t iBar)
{
   Int_t idx = iLayer*fBarsN + iBar;

   TGeoHMatrix* hm = TAGparTools::GetTransfo(idx);
   if (hm) {
      TVector3 local(0,0,0);
      fCurrentPosition =  Sensor2Detector(iLayer, iBar,local);
   }
   return fCurrentPosition;
}

//_____________________________________________________________________________
Float_t TATWparGeo::GetCoordiante_sensorFrame(Int_t iLayer, Int_t iBar)
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   TGeoHMatrix* hm = TAGparTools::GetTransfo(idx);
   if (hm) {
      TVector3 local(0,0,0);
      fCurrentPosition =  Sensor2Detector(iLayer, iBar,local);
   }
   if (iLayer == 0)
      return fCurrentPosition.X();
   else
      return fCurrentPosition.Y();
}

//_____________________________________________________________________________
Float_t TATWparGeo::GetZ_sensorFrame(Int_t iLayer, Int_t iBar)
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   TGeoHMatrix* hm = TAGparTools::GetTransfo(idx);
   if (hm) {
      TVector3 local(0,0,0);
      fCurrentPosition =  Sensor2Detector(iLayer, iBar,local);
   }
      return fCurrentPosition.Z();
}



//_____________________________________________________________________________
TVector3 TATWparGeo::Sensor2Detector(Int_t iLayer, Int_t iBar, TVector3& loc) const
{
   Int_t idx = iLayer*fBarsN + iBar;

   if (idx < 0 || idx > fBarsN*fLayersN) {
      Warning("Sensor2Detector()","Wrong detector id number: %d ", idx);
      TVector3(0,0,0);
   }
   
   return LocalToMaster(idx, loc);
}


//_____________________________________________________________________________
TVector3 TATWparGeo::Sensor2DetectorVect(Int_t iLayer, Int_t iBar, TVector3& loc) const
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   if (idx < 0 || idx > fBarsN*fLayersN) {
      Warning("Sensor2DetectorVect()","Wrong detector id number: %d ", idx);
      TVector3(0,0,0);
   }
   
   return LocalToMasterVect(idx, loc);
}

//_____________________________________________________________________________
TVector3 TATWparGeo::Detector2Sensor(Int_t iLayer, Int_t iBar, TVector3& glob) const
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   if (idx < 0 || idx > fBarsN*fLayersN) {
      Warning("Detector2Sensor()","Wrong detector id number: %d ", idx);
      return TVector3(0,0,0);
   }
   
   return MasterToLocal(idx, glob);
}

//_____________________________________________________________________________
TVector3 TATWparGeo::Detector2SensorVect(Int_t iLayer, Int_t iBar, TVector3& glob) const
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   if (idx < 0 || idx > fBarsN*fLayersN) {
      Warning("Detector2SensorVect()","Wrong detector id number: %d ", idx);
      return TVector3(0,0,0);
   }
   
   return MasterToLocalVect(idx, glob);
}


//_____________________________________________________________________________
void TATWparGeo::DefineMaterial()
{
   
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // TW material
   TGeoMaterial* mat = TAGmaterials::Instance()->CreateMaterial(fBarMat, fBarDensity);
   if (fDebugLevel) {
      printf("ToF Wall material:\n");
      mat->Print();
   }
}

//_____________________________________________________________________________
TGeoVolume* TATWparGeo::BuildTofWall(const char *wallName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager(TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoVolume* wall = gGeoManager->FindVolumeFast(wallName);
   if ( wall == 0x0 ) {
      TGeoMedium*  med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("AIR");
      wall = gGeoManager->MakeBox(wallName, med,  fBarSize[1]/2.,  fBarSize[1]/2., GetBarThick()*2);
   }
   
   for (Int_t i = 0; i < 2; ++i) {
      
      TGeoVolume* wallXY = BuildTofWallXY(Form("%s%d", wallName, i+1), i);
      wall->AddNode(wallXY, i);
   }
   
   return wall;
}


//_____________________________________________________________________________
TGeoVolume* TATWparGeo::BuildTofWallXY(const char *wallName, Int_t iLayer)
{
   
   TGeoVolume* wall = gGeoManager->FindVolumeFast(wallName);
   
   if ( wall == 0x0 ) {
      TGeoMedium*  med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("AIR");
      wall = gGeoManager->MakeBox(wallName, med,  fBarSize[1]/2.,  fBarSize[1]/2., GetBarThick()/2.);
   }
   
   for (Int_t i = 0; i < fBarsN; ++i) {

      TGeoHMatrix* hm = GetTransfo(iLayer, i);
      
      TGeoVolume* module = BuildModule(i, iLayer);
      
      module->SetLineColor(fgkDefaultModCol);
      module->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
      wall->AddNode(module, i, hm);
   }
   
   return wall;
}

/*------------------------------------------+---------------------------------*/
//! build module

TGeoVolume* TATWparGeo::BuildModule(Int_t iMod, Int_t iLayer)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   const char* moduleName = GetDefaultBarName(iMod, iLayer);
   TGeoVolume* module     = gGeoManager->FindVolumeFast(moduleName);
   if ( module == 0x0 ) {
      const Char_t* matName = fBarMat.Data();
      TGeoMedium*  med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
      module = gGeoManager->MakeBox(moduleName, med,  fBarSize[0]/2., fBarSize[1]/2., GetBarThick()/2.);
   }
   
   module->SetLineColor(fgkDefaultModCol);
   module->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return module;
}

//_____________________________________________________________________________
//! set color on for fired bars
void TATWparGeo::SetBarColorOn(Int_t bar, Int_t view)
{
   if (!gGeoManager) {
      Error("SetBarcolorOn()", "No Geo manager defined");
      return;
   }
   
   TString name = GetDefaultBarName(bar, view);
   
   TGeoVolume* vol = gGeoManager->FindVolumeFast(name.Data());
   if (vol)
      vol->SetLineColor(GetDefaultModColOn());
}

//_____________________________________________________________________________
//! reset color for unfired bars
void TATWparGeo::SetBarColorOff(Int_t bar, Int_t view)
{
   if (!gGeoManager) {
      Error("SetBarcolorOn()", "No Geo manager defined");
      return;
   }
   
   TString name = GetDefaultBarName(bar, view);
   
   TGeoVolume* vol = gGeoManager->FindVolumeFast(name.Data());
   if (vol)
      vol->SetLineColor(GetDefaultModCol());
}


//_____________________________________________________________________________
Int_t TATWparGeo::GetBarId(Int_t layer, Float_t xGlob, Float_t yGlob)
{
   Int_t barId = -99;
   
   if (layer == 0)
      barId =  xGlob/GetBarWidth() + fBarsN/2;
   else if (layer == 1)
      barId =  yGlob/GetBarWidth() + fBarsN/2;
   
   if (barId < 0 || barId >= fBarsN)
      Error("GetBarId()", " Wrong bar Id (%d) for global position (%.1f,%.1f)", barId, xGlob, yGlob);
   
   barId += fgkLayerOffset*layer;
   
   return barId;
}

//_____________________________________________________________________________
string TATWparGeo::PrintRotations()
{
  stringstream ss;

  if(GlobalPar::GetPar()->IncludeTW()){

    TAGgeoTrafo* fpFootGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  
    TVector3 fCenter = fpFootGeo->GetTWCenter();
    TVector3  fAngle = fpFootGeo->GetTWAngles();
    TVector3 pos;
    
    int c=0;
    
    for (int i=0; i<GetNLayers(); i++){
      for (int j=0; j<GetNBars(); j++){      
  
	pos.SetXYZ( fCenter.X() + GetBarPosition(i,j).X(),
		    fCenter.Y() + GetBarPosition(i,j).Y(),
		    fCenter.Z() + GetBarPosition(i,j).Z());

	if(vTilt.at(c).Z()!=0){  
	  ss << setw(10) << setfill(' ') << std::left << "ROT-DEFI"
	     << setw(10) << setfill(' ') << std::right << "300."
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << fCenter.Y()-pos.Y()
	     << setw(10) << setfill(' ') << std::right << " "
	     << setfill(' ') << std::left << Form("twZ_%d",c) 
	     << endl;
	  ss << setw(10) << setfill(' ') << std::left << "ROT-DEFI"
	     << setw(10) << setfill(' ') << std::right << "300."
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << vTilt.at(c).Z()
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << " "
	     << setfill(' ') << std::left << Form("twZ_%d",c) 
	     << endl;
	  ss << setw(10) << setfill(' ') << std::left << "ROT-DEFI"
	     << setw(10) << setfill(' ') << std::right << "300."
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << " "
	     << setw(10) << setfill(' ') << std::right << fCenter.Y()+pos.Y()
	     << setw(10) << setfill(' ') << std::right << " "
	     << setfill(' ') << std::left << Form("twZ_%d",c) 
	     << endl;

	}
	
	c++;
      }
    }
  }

  return ss.str();

}

//_____________________________________________________________________________
string TATWparGeo::PrintBodies()
{
  
  stringstream ss;

  if(GlobalPar::GetPar()->IncludeTW()){

    TAGgeoTrafo* fpFootGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  
    TVector3  fCenter = fpFootGeo->GetTWCenter();
    TVector3  fAngle = fpFootGeo->GetTWAngles();
    TVector3 pos;

    string bodyname, regionname;
  
    ss << "* ***Scintillator bodies" << endl;
    
    if(fAngle.X()!=0)
      cout << "TW rotation around x found: ATTENTION not implemented in fluka" << endl;
    if(fAngle.Y()!=0)
      cout << "TW rotation around y found: ATTENTION not implemented in fluka" << endl;
    if(fAngle.Z()!=0)
      cout << "TW rotation around z found: ATTENTION not implemented in fluka" << endl;
    //   ss << "$start_transform " << "twZ" << endl;
    
    int c=0;
    
    for (int i=0; i<GetNLayers(); i++){
      for (int j=0; j<GetNBars(); j++){
    
	// Double_t *rot = TATWparGeo::GetTransfo(i,j)->GetRotationMatrix();

 	if (vTilt.at(c).X()!=0)
	  cout << "TW bar rotation around x found: ATTENTION not implemented in fluka" << endl;
 	if (vTilt.at(c).Y()!=0)
	  cout << "TW bar rotation around y found: ATTENTION not implemented in fluka" << endl;
 	if (vTilt.at(c).Z()!=0)
	  ss << "$start_transform " << Form("twZ_%d",c) << endl;
      	
	bodyname = Form("scn%02d",c);
	regionname = Form("SCN%02d",c);
	pos.SetXYZ( fCenter.X() + GetBarPosition(i,j).X(),
		    fCenter.Y() + GetBarPosition(i,j).Y(),
		    fCenter.Z() + GetBarPosition(i,j).Z());
	ss <<  "RPP " << bodyname <<  "     "
	   << pos.x() - fBarSize.X()/2. << " "
	   << pos.x() + fBarSize.X()/2. << " "
	   << pos.y() - fBarSize.Y()/2. << " "
	   << pos.y() + fBarSize.Y()/2. << " "
	   << pos.z() - fBarSize.Z()/2. << " "
	   << pos.z() + fBarSize.Z()/2. << endl;
	vBody.push_back(bodyname);
	vRegion.push_back(regionname);

	// if (vTilt.at(c).X()!=0)
	//   ss << "$end_transform " << endl;
	// if (vTilt.at(c).Y()!=0)
	//   ss << "$end_transform " << endl;
	if (vTilt.at(c).Z()!=0)
	  ss << "$end_transform " << endl;

	c++;
      }
    }
    
    // if(fAngle.Z()!=0)
    //   ss << "$end_transform " << endl;
    
  }

  return ss.str();
}


//_____________________________________________________________________________
string TATWparGeo::PrintRegions()
{

  stringstream ss;

  if(GlobalPar::GetPar()->IncludeTW()){

    string name;

    ss << "* ***Scintillator regions" << endl;

    for(int i=0; i<vRegion.size(); i++) {
      ss << setw(13) << setfill( ' ' ) << std::left << vRegion.at(i)
    	 << "5 " << vBody.at(i) <<endl;
    }    
    
  }
  return ss.str();
}


//_____________________________________________________________________________
string TATWparGeo::PrintSubtractBodiesFromAir() {

  stringstream ss;

  if(GlobalPar::GetPar()->IncludeTW()){

    for (int i=0; i<vBody.size(); i++) {
      ss << " -" << vBody.at(i);
      if ((i+1)%10==0) ss << endl;
    }
    ss << endl;

  }
  
  return ss.str();

}


//_____________________________________________________________________________
string TATWparGeo::PrintAssignMaterial()
{

  stringstream ss; 
  
  if(GlobalPar::GetPar()->IncludeTW()){

    const Char_t* mat = fBarMat.Data();
      
    bool magnetic = false;
    if(GlobalPar::GetPar()->IncludeDI())
      magnetic = true;

    if (vRegion.size()==0 )
      cout << "Error: TW regions vector not correctly filled!"<<endl;
    
    ss << setw(10) << setfill(' ') << std::left << "ASSIGNMA"
       << setw(10) << setfill(' ') << std::right << mat
       << setw(10) << setfill(' ') << std::right << vRegion.at(0)
       << setw(10) << setfill(' ') << std::right << vRegion.back()
       << setw(10) << setfill(' ') << std::right << "1."
       << setw(10) << setfill(' ') << std::right << magnetic
       << endl;
  }

  return ss.str();

}





//_____________________________________________________________________________
string TATWparGeo::PrintParameters() {

  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);

  outstr << "c     SCINTILLATOR PARAMETERS " << endl;
  outstr << endl;
  
  string nstrip = "nstripSCN";
  outstr << "      integer " << nstrip << endl;
  outstr << "      parameter(" << nstrip << " = " << GetNBars() << ")" << endl;
  // outstr << typeid(m_nBar).name()<< endl;
  outstr << endl;  

  return outstr.str();

}



//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TATWparGeo::Clear(Option_t*)
{
  return;
}



/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TATWparGeo::ToStream(ostream& os, Option_t*) const
{
  //  os << "TATWparGeo " << GetName() << endl;
  //  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
  //     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}
