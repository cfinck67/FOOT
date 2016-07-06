/*!
  \file
  \version $Id: TAVTparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAVTparGeo.
*/

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

#include "TAGgeoTrafo.hxx" 

#include "TAVTparMap.hxx"
#include "TAVTparGeo.hxx"

//##############################################################################

/*!
  \class TAVTparGeo TAVTparGeo.hxx "TAVTparGeo.hxx"
  \brief Map and Geometry parameters for Tof wall. **
*/

ClassImp(TAVTparGeo);

TString TAVTparGeo::fgkDefaultGeoName   = "./geomaps/TAVTdetector.map";
TString TAVTparGeo::fgkDefaultGeoNameMC = "./geomaps/TAVTdetectorMC.map";
Float_t TAVTparGeo::fgTargetWidth       =  8000.;
Float_t TAVTparGeo::fgTargetSize        =  20000.;

//______________________________________________________________________________
TAVTparGeo::TAVTparGeo()
: TAVTparTools(),
  fMatrixList(new TObjArray(TAVTparMap::GetSensorsN())),
  fCurrentPosition(new TVector3()),
  fSensorsN(0),
  fPixelsNu(0),
  fPixelsNv(0),
  fPitchU(0),
  fPitchV(0),
  fWidth(10600),
  fHeight(21200),
  fThick(25),
  fTotalSize(),
  fPixThickness(6.4),
  fTotalThickness(20.4)
{
   // Standard constructor
   fMatrixList->SetOwner(true);
   fTotalSize.SetXYZ(21200, 13600, 50);
}

//______________________________________________________________________________
TAVTparGeo::~TAVTparGeo()
{
   // Destructor
   delete fMatrixList;
   delete fCurrentPosition;
}

//______________________________________________________________________________
Bool_t TAVTparGeo::FromFile(const TString& name) 
{   
   // simple file reading, waiting for real config file
   TString nameExp;
   
   if (name.IsNull())
	  nameExp = fgkDefaultGeoName;
   else 
	  nameExp = name;

   if (Open(nameExp)) return kTRUE;

   ReadItem(fSensorsN);  
   if(fDebugLevel)
	  cout << endl << "Reading Parameters for "<< fSensorsN << " Sensors" << endl;
   
   ReadItem(fPixelsNu);  
   if(fDebugLevel)
	  cout << endl << "  Number of pixel in U "<< fPixelsNu << endl;
   
   ReadItem(fPixelsNv);  
   if(fDebugLevel)
	  cout << endl << "  Number of pixel in U "<< fPixelsNv << endl;
   
   ReadItem(fPitchU);  
   if(fDebugLevel)
	  cout << endl << "  Pitch for pixel in U "<< fPitchU << endl;
   
   ReadItem(fPitchV);  
   if(fDebugLevel)
	  cout << endl << "  Pitch for pixel in U "<< fPitchV << endl;

   for (Int_t p = 0; p < fSensorsN; p++) { // Loop on each plane
	  
	  // read sensor index
	  ReadItem(fSensorParameter[p].SensorIdx);
	  if(fDebugLevel)
		 cout << endl << " - Reading Parameters of Sensor " <<  fSensorParameter[p].SensorIdx << endl;

	  // read sensor position
	  ReadVector3(fSensorParameter[p].Position);
	  fSensorParameter[p].Position    *=  TAGgeoTrafo::CmToMu(); // convert to mu
	  if(fDebugLevel)
		 cout << endl << "   Position: " 
	          << Form("%f %f %f", fSensorParameter[p].Position[0], fSensorParameter[p].Position[1], fSensorParameter[p].Position[2]) << endl;
	  
	  // read sensor angles
	  ReadVector3(fSensorParameter[p].Tilt);
	  if(fDebugLevel)
		 cout << endl << "   Tilt: " 
		 << Form("%f %f %f", fSensorParameter[p].Tilt[0], fSensorParameter[p].Tilt[1], fSensorParameter[p].Tilt[2]) << endl;
	  
	  // read alignment
	  ReadItem(fSensorParameter[p].AlignmentU);
	  ReadItem(fSensorParameter[p].AlignmentV);
	  if(fDebugLevel)
		 cout << endl << "   Alignment: " <<  fSensorParameter[p].AlignmentU << " " << fSensorParameter[p].AlignmentV << endl;
	  
	  // read tiltW
	  ReadItem(fSensorParameter[p].TiltW);
	  if(fDebugLevel)
		 cout << endl << "   Rotation tiltW: " << fSensorParameter[p].TiltW << endl;
	  
	  Float_t thetaX = fSensorParameter[p].Tilt[0];
	  Float_t thetaY = fSensorParameter[p].Tilt[1];
	  Float_t thetaZ = fSensorParameter[p].Tilt[2] - fSensorParameter[p].TiltW;
	  TGeoRotation rot;
	  rot.RotateX(thetaX);   
	  rot.RotateY(thetaY);   
	  rot.RotateZ(thetaZ);   
	  TGeoTranslation trans((fSensorParameter[p].Position[0]-fSensorParameter[p].AlignmentU), 
							(fSensorParameter[p].Position[1]-fSensorParameter[p].AlignmentV), 
							(fSensorParameter[p].Position[2]));
	  
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  AddTransMatrix(new TGeoHMatrix(transfo));
	 
	 // change to rad
	 fSensorParameter[p].Tilt[0] = fSensorParameter[p].Tilt[0]*TMath::DegToRad();
	 fSensorParameter[p].Tilt[1] = fSensorParameter[p].Tilt[1]*TMath::DegToRad();
	 fSensorParameter[p].Tilt[2] = fSensorParameter[p].Tilt[2]*TMath::DegToRad();
	 fSensorParameter[p].TiltW   = fSensorParameter[p].TiltW*TMath::DegToRad();
   }	  

   return kFALSE;
}

//_____________________________________________________________________________
void TAVTparGeo::AddTransMatrix(TGeoHMatrix* mat, Int_t idx)
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
void TAVTparGeo::RemoveTransMatrix(TGeoHMatrix* mat)
{
	 if (!fMatrixList->Remove(mat))
		printf("Cannot remove matrix");
}

//_____________________________________________________________________________
TGeoHMatrix* TAVTparGeo::GetTransfo(Int_t iSensor)
{
   if (iSensor < 0 || iSensor >= GetSensorsN()) {
	  cout << "Wrong detector id number" << endl; 
	  return 0x0;
   }
   
   return (TGeoHMatrix*)fMatrixList->At(iSensor);
}

//_____________________________________________________________________________
TVector3* TAVTparGeo::GetPosition(Int_t iSensor)
{
   TGeoHMatrix* hm = GetTransfo(iSensor);
   if (hm) {
	  TVector3 local(0,0,0);
	  *fCurrentPosition =  Local2Global(iSensor,local);
   }
   return fCurrentPosition;
}

//_____________________________________________________________________________
void TAVTparGeo::Global2Local(Int_t detID,
									Double_t xg, Double_t yg, Double_t zg, 
									Double_t& xl, Double_t& yl, Double_t& zl) const
{  
   if (detID < 0 || detID > GetSensorsN()) {
	  cout << "Wrong detector id number" << endl; 
	  return ;
   }
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(detID) );
   Double_t local[3]  = {0., 0., 0.};
   Double_t global[3] = {xg, yg, zg};
   
   mat->MasterToLocal(global, local);
   xl = local[0];
   yl = local[1];
   zl = local[2];
}   

//_____________________________________________________________________________
TVector3 TAVTparGeo::Global2Local(Int_t detID, TVector3& glob) const
{
   if (detID < 0 || detID > GetSensorsN()) {
	  cout << "Wrong detector id number" << endl; 
	  return TVector3(0,0,0);
   }
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(detID) );
   Double_t local[3]  = {0., 0., 0.};
   Double_t global[3] = {glob.X(), glob.Y(), glob.Z()};
   
   mat->MasterToLocal(global, local);
   TVector3 pos(local[0], local[1], local[2]);
   
   return pos;
}   

//_____________________________________________________________________________
TVector3 TAVTparGeo::Global2LocalVect(Int_t detID, TVector3& glob) const
{
   if (detID < 0 || detID > GetSensorsN()) {
	  Warning("Global2LocalVect()","Wrong detector id number: %d ", detID); 
	  return TVector3(0,0,0);
   }
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(detID) );
   Double_t local[3]  = {0., 0., 0.};
   Double_t global[3] = {glob.X(), glob.Y(), glob.Z()};
   
   mat->MasterToLocalVect(global, local);
   TVector3 pos(local[0], local[1], local[2]);
   
   return pos;
}   

//_____________________________________________________________________________
void TAVTparGeo::Local2Global(Int_t detID,
									Double_t xl, Double_t yl, Double_t zl, 
									Double_t& xg, Double_t& yg, Double_t& zg) const
{
   if (detID < 0 || detID > GetSensorsN()) {
	  cout << "Wrong detector id number" << endl; 
	  return;
   }
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(detID) );
   Double_t local[3]  = {xl, yl, zl};
   Double_t global[3] = {0., 0., 0.};
   
   mat->LocalToMaster(local, global);
   xg = global[0];
   yg = global[1];
   zg = global[2];
}   

//_____________________________________________________________________________
TVector3 TAVTparGeo::Local2Global(Int_t detID, TVector3& loc) const
{
   if (detID < 0 || detID > GetSensorsN()) {
	  cout << "Wrong detector id number" << endl; 
	  TVector3(0,0,0);
   }
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(detID) );
   Double_t local[3]  = {loc.X(), loc.Y(), loc.Z()};
   Double_t global[3] = {0., 0., 0.};
   
   mat->LocalToMaster(local, global);
   TVector3 pos(global[0], global[1], global[2]);
   
   return pos;
}   

//_____________________________________________________________________________
TVector3 TAVTparGeo::Local2GlobalVect(Int_t detID, TVector3& loc) const
{
   if (detID < 0 || detID > GetSensorsN()) {
	  Warning("Local2GlobalVect()","Wrong detector id number: %d ", detID); 
	  TVector3(0,0,0);
   }
   
   
   TGeoHMatrix* mat = static_cast<TGeoHMatrix*> ( fMatrixList->At(detID) );
   
   Double_t local[3]  = {loc.X(), loc.Y(), loc.Z()};
   Double_t global[3] = {0., 0., 0.};
   
   mat->LocalToMasterVect(local, global);
   TVector3 pos(global[0], global[1], global[2]);
   
   return pos;
}   


//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::BuildVertex(const char* basemoduleName, const char *vertexName)
{
   TGeoVolume* vertex = 0x0; 
   
   for(Int_t iSensor = 0; iSensor < GetSensorsN(); iSensor++) {	 
	  TGeoHMatrix* hm = GetTransfo(iSensor);
	  vertex = TAVTparGeo::AddVertexModule(hm, basemoduleName, vertexName);
   }
   
   return vertex;
}

//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::AddVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
	  new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoVolume* vertex = gGeoManager->FindVolumeFast(vertexName);
   if ( vertex == 0x0 ) {
	  Int_t nSensors = GetSensorsN();
	  
	  Float_t posZ1 = (*GetPosition(0))(2)*0.9;
	  Float_t posZ2 = (*GetPosition(nSensors-1))(2)*1.1;

	  TGeoMedium   *med;
	  TGeoMaterial *mat;
	  if ( (mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
		 mat = new TGeoMaterial("Vacuum",0,0,0); 			
	  if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
		 med = new TGeoMedium("Vacuum",1,mat);
	  vertex = gGeoManager->MakeBox(vertexName, med, fHeight/2., fHeight/2., (posZ2-posZ1)/2.); // volume corresponding to vertex
   } 
   
   // create module
   TGeoMaterial* matMod;
   TGeoMedium*   medMod;
   
   if ( (matMod = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Si")) == 0x0 )
	  matMod = new TGeoMaterial("Si", 28.09, 14, 2.3);
   if ( (medMod = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Si")) == 0x0 )
	  medMod = new TGeoMedium("Si",2,matMod);
   
   TGeoBBox *box = new TGeoBBox(Form("%s_Box",basemoduleName), fWidth/2, fHeight/2, fThick/2.);
   
   Int_t nbModule = 0;
   
   TGeoVolume *vertexMod = new TGeoVolume(Form("%s_Vertex",basemoduleName),box, medMod);
   vertexMod->SetLineColor(kAzure-5);
   vertexMod->SetTransparency( TAGgeoTrafo::GetDefaultTransp());
   
   TObjArray* list = vertex->GetNodes();
   if (list) {
	  for (Int_t i = 0; i < list->GetEntries(); ++i) {
		 TGeoVolume* vol = (TGeoVolume*)list->At(i);
		 if (vol) {
			TString name(vol->GetName());
			if ( name.Contains(Form("%s_Vertex",basemoduleName)) )
			   nbModule++;
		 }
	  }
   }

   vertex->AddNode(vertexMod, nbModule, new TGeoHMatrix(*hm));
   return vertex;
}

//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::AddTarget(const Float_t dx, const Float_t dy, const Float_t dz, const char *targetName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
	  new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // create module
   TGeoMaterial* matTarget;
   TGeoMedium*   medTarget;
   
   if ( (matTarget = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
	  matTarget = new TGeoMaterial("Vacuum", 0., 0., 0.);
   if ( (medTarget = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
	  medTarget = new TGeoMedium("Vacuum", 1, matTarget);
   
   TGeoVolume* target = gGeoManager->MakeBox(targetName, medTarget, dx, dy, dz);
   target->SetVisibility(true);
   target->SetTransparency( TAGgeoTrafo::GetDefaultTransp());

   return target;
}


//_____________________________________________________________________________
TEveGeoShapeExtract* TAVTparGeo::BuildExtractVertex(const char* basemoduleName, const char *vertexName)
{
   Int_t nSensors = GetSensorsN();
   Float_t posZ1 = (*GetPosition(0))(2)*0.9;
   Float_t posZ2 = (*GetPosition(nSensors-1))(2)*1.1;
   TGeoBBox *box = new TGeoBBox(vertexName, 11000,11000,(posZ2-posZ1)/2.);
   
   TEveGeoShapeExtract* vertexExtract = new TEveGeoShapeExtract(vertexName);
   vertexExtract->SetShape(box);
   Float_t color[] = {0, 0, 0, 0};
   vertexExtract->SetRGBA(color);
   
   for(Int_t iSensor = 0; iSensor < nSensors; iSensor++) {	 
	  TGeoHMatrix* hm = GetTransfo(iSensor);
	  TEveGeoShapeExtract* vertexMod = AddExtractVertexModule(hm, basemoduleName, vertexName);
	  vertexExtract->AddElement(vertexMod);
   }
   
   return vertexExtract;
}

//_____________________________________________________________________________
TEveGeoShapeExtract* TAVTparGeo::AddExtractVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
{
   // create module
   static Int_t nbModule = 0;
   
   TGeoBBox *box = new TGeoBBox(Form("%s_%d",basemoduleName, nbModule), fWidth/2, fHeight/2, fThick/2.);
   
   TEveTrans eveTrans;
   eveTrans.SetFrom(*hm);
   TColor* color = gROOT->GetColor(kAzure-5);
   Float_t rgba[4];
   color->GetRGB(rgba[0], rgba[1], rgba[2]);
   rgba[3] = TAGgeoTrafo::GetDefaultTransp()/100.;
   
   TEveGeoShapeExtract* vertexModExtract = new TEveGeoShapeExtract(Form("%s_%d",basemoduleName, nbModule++));
   vertexModExtract->SetShape(box);
   vertexModExtract->SetTrans(eveTrans.Array());
   vertexModExtract->SetRGBA(rgba);
      
   return vertexModExtract;
}

//_____________________________________________________________________________
TEveGeoShapeExtract* TAVTparGeo::AddExtractTarget(const Float_t dx, const Float_t dy, const Float_t dz, const char *targetName)
{
   TGeoBBox* box = new TGeoBBox(targetName, dx, dy, dz);
   
   TColor* color = gROOT->GetColor(19);
   Float_t rgba[4];
   color->GetRGB(rgba[0], rgba[1], rgba[2]);
   rgba[3] = TAGgeoTrafo::GetDefaultTransp()/100.;
   
   TEveGeoShapeExtract* target = new TEveGeoShapeExtract(targetName);
   target->SetShape(box);
   target->SetRGBA(rgba);
   
   return target;
   
}

//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TAVTparGeo::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAVTparGeo::ToStream(ostream& os, Option_t*) const
{
//  os << "TAVTparGeo " << GetName() << endl;
//  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
//     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}

