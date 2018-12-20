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

// #include "TATWparMap.hxx"
#include "TATWparGeo.hxx"


      TString TATWparGeo::fgkDefParaName     = "twGeo";
const TString TATWparGeo::fgkBaseName        = "TW";
      Int_t   TATWparGeo::fgkLayerOffset     = 100;
const Color_t TATWparGeo::fgkDefaultModCol   = kGray+1;
const Color_t TATWparGeo::fgkDefaultModColOn = kRed-8;
const TString TATWparGeo::fgkDefaultBarName = "twBar";


//_____________________________________________________________________________
TATWparGeo::TATWparGeo()
: TAGparTools(),
   fMatrixList(new TObjArray(22*22))
{
   fgDefaultGeoName = "./geomaps/TATWdetector.map";
   fMatrixList->SetOwner(true);
}

//______________________________________________________________________________
TATWparGeo::~TATWparGeo()
{
   fMatrixList->Delete();
}

//______________________________________________________________________________
Bool_t TATWparGeo::FromFile(const TString& name)
{
   TString nameExp;
   
   if (name.IsNull())
      nameExp = fgDefaultGeoName;
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
void TATWparGeo::AddTransMatrix(TGeoHMatrix* mat, Int_t idx)
{
   if (idx == -1)
      fMatrixList->Add(mat);
   else {
      TGeoHMatrix* oldMat = GetTransfo(idx);
      if (oldMat)
         RemoveTransMatrix(oldMat);
      fMatrixList->AddAt(mat, idx);
   }
}

//_____________________________________________________________________________
void TATWparGeo::RemoveTransMatrix(TGeoHMatrix* mat)
{
   if (!fMatrixList->Remove(mat))
      printf("Cannot remove matrix");
}

//_____________________________________________________________________________
TGeoHMatrix* TATWparGeo::GetTransfo(Int_t idx)
{
   if (idx < 0 || idx >= fBarsN*fLayersN) {
      Warning("GetTransfo()","Wrong detector id number: %d ", idx);
      return 0x0;
   }
   
   return (TGeoHMatrix*)fMatrixList->At(idx);
}

//_____________________________________________________________________________
TGeoHMatrix* TATWparGeo::GetTransfo(Int_t iLayer, Int_t iBar)
{
   Int_t idx = iLayer*fBarsN + iBar;

   return GetTransfo(idx);
}

//_____________________________________________________________________________
TVector3 TATWparGeo::GetBarPosition(Int_t iLayer, Int_t iBar)
{
   Int_t idx = iLayer*fBarsN + iBar;

   TGeoHMatrix* hm = GetTransfo(idx);
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
   
   TGeoHMatrix* hm = GetTransfo(idx);
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
   
   TGeoHMatrix* hm = GetTransfo(idx);
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
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(idx) );
   Double_t local[3]  = {loc.X(), loc.Y(), loc.Z()};
   Double_t global[3] = {0., 0., 0.};
   
   mat->LocalToMaster(local, global);
   TVector3 pos(global[0], global[1], global[2]);
   
   return pos;
}


//_____________________________________________________________________________
TVector3 TATWparGeo::Sensor2DetectorVect(Int_t iLayer, Int_t iBar, TVector3& loc) const
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   if (idx < 0 || idx > fBarsN*fLayersN) {
      Warning("Sensor2DetectorVect()","Wrong detector id number: %d ", idx);
      TVector3(0,0,0);
   }
   
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(idx) );
   
   Double_t local[3]  = {loc.X(), loc.Y(), loc.Z()};
   Double_t global[3] = {0., 0., 0.};
   
   mat->LocalToMasterVect(local, global);
   TVector3 pos(global[0], global[1], global[2]);
   
   return pos;
}

//_____________________________________________________________________________
TVector3 TATWparGeo::Detector2Sensor(Int_t iLayer, Int_t iBar, TVector3& glob) const
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   if (idx < 0 || idx > fBarsN*fLayersN) {
      Warning("Detector2Sensor()","Wrong detector id number: %d ", idx);
      return TVector3(0,0,0);
   }
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(idx) );
   Double_t local[3]  = {0., 0., 0.};
   Double_t global[3] = {glob.X(), glob.Y(), glob.Z()};
   
   mat->MasterToLocal(global, local);
   TVector3 pos(local[0], local[1], local[2]);
   
   return pos;
}

//_____________________________________________________________________________
TVector3 TATWparGeo::Detector2SensorVect(Int_t iLayer, Int_t iBar, TVector3& glob) const
{
   Int_t idx = iLayer*fBarsN + iBar;
   
   if (idx < 0 || idx > fBarsN*fLayersN) {
      Warning("Detector2SensorVect()","Wrong detector id number: %d ", idx);
      return TVector3(0,0,0);
   }
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(idx) );
   Double_t local[3]  = {0., 0., 0.};
   Double_t global[3] = {glob.X(), glob.Y(), glob.Z()};
   
   mat->MasterToLocalVect(global, local);
   TVector3 pos(local[0], local[1], local[2]);
   
   return pos;
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
string TATWparGeo::PrintBodies() {

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
	cout << "ERROR << TATWparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  stringstream outstr;
  outstr << "* ***Scintillator" << endl;

  // loop in order of the material alfabeth
  for ( map<string, vector<string> >::iterator itMat = m_bodyPrintOut.begin(); itMat != m_bodyPrintOut.end(); itMat++ ) {
	// loop over all body of the same material
	for ( vector<string>::iterator itBody = (*itMat).second.begin(); itBody != (*itMat).second.end(); itBody++ ) {
	  outstr << (*itBody);
	  if (fDebugLevel > 3)    cout << (*itBody);
	}        
  }
  return outstr.str();
}



//_____________________________________________________________________________
string TATWparGeo::PrintRegions() {

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
	cout << "ERROR << TATWparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  stringstream outstr;
  outstr << "* ***Scintillator" << endl;

  // loop in order of the material alfabeth
  for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
	// loop over all region of the same material
	for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
	  outstr << (*itRegion);
	  if (fDebugLevel > 3)    cout << (*itRegion);
	}        
  }
  return outstr.str();
}




//_____________________________________________________________________________
string TATWparGeo::PrintSubtractBodiesFromAir() {

  stringstream outstr;
  int count=0;
  // loop in order of the material alfabeth
  for ( map<string, vector<string> >::iterator itMat = m_bodyName.begin(); itMat != m_bodyName.end(); itMat++ ) {
	// loop over all region of the same material
	for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
	  if ( count % 10 == 0 && count>0 )
	outstr << "\n              ";
	  count++;
	  outstr << " -" << (*itRegion);
	}        
  }
  return outstr.str();

}





//_____________________________________________________________________________
string TATWparGeo::PrintAssignMaterial() {

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
	cout << "ERROR << TATWparGeo::PrintAssignMaterial()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


  // loop in order of the material alfabeth
  stringstream outstr; 
  for ( map<string, vector<string> >::iterator itMat = m_regionName.begin(); itMat != m_regionName.end(); itMat++ ) {

	// check dimension greater than 0
	if ( (*itMat).second.size() == 0 ) {
	  cout << "ERROR << TATWparGeo::PrintAssignMaterial  ::  "<<endl, exit(0);
	}

	// take the first region
	string firstReg = (*itMat).second.at(0);
	// take the last region
	string lastReg = "";
	if ( (*itMat).second.size() != 1 ) 
	  lastReg = (*itMat).second.at( (*itMat).second.size()-1 );

	// build output string 
	outstr  << setw(10) << setfill( ' ' ) << std::left << "ASSIGNMA" 
		<< setw(10) << setfill( ' ' ) << std::right << (*itMat).first 
		<< setw(10) << setfill( ' ' ) << std::right << firstReg 
		<< setw(10) << setfill( ' ' ) << std::right << lastReg;
					   
		
	// multiple region condition 
	if ( (*itMat).second.size() != 1 ) {
	  outstr << setw(10) << setfill( ' ' ) << std::right  << 1 ;
	}
	else {
	  outstr << setw(10) << setfill( ' ' ) << std::right  << " ";
	}


	// region in the magnetic filed condition
	bool isMag = true;
	for (int i=0; i<(*itMat).second.size(); i++) {
	  if ( m_magneticRegion[ (*itMat).second.at(i) ] == 0 ) {
	isMag = false;
	break;
	  }
	}
	if ( isMag )
	  outstr << setw(10) << setfill( ' ' ) << std::right  << 1 ;
	else 
	  outstr << setw(10) << setfill( ' ' ) << std::right  << " " ;
		
	outstr << endl;

	// DEBUG
	// if (fDebugLevel > 0)    cout << outstr.str();

  }

  return outstr.str();

}





//_____________________________________________________________________________
string TATWparGeo::PrintParameters() {

  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);

  outstr << "c     SCINTILLATOR PARAMETERS " << endl;
  outstr << endl;
  
  string nstrip = "nstripSCN";
  outstr << "      integer " << nstrip << endl;
  outstr << "      parameter(" << nstrip << " = " << fBarsN << ")" << endl;
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
