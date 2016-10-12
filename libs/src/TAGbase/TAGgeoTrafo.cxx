#include "TAGgeoTrafo.hxx"
#include <fstream>

#include "TObjArray.h"
#include "TSystem.h"

//Class that handles the Geometrical transformations in FIRST RS

ClassImp(TAGgeoTrafo);

      TString TAGgeoTrafo::fgDefaultActName = "actGeoTrafo";
const Char_t* TAGgeoTrafo::fgkGeomName      = "FIRST";
const Char_t* TAGgeoTrafo::fgkGeomTitle     = "FIRST Geometry";
      Char_t  TAGgeoTrafo::fgDefaultTransp  = 60;
const Float_t TAGgeoTrafo::fgkCmToMu        = 10000;
const Float_t TAGgeoTrafo::fgkMuToCm        = 1e-4;
const Float_t TAGgeoTrafo::fgkMmToMu        = 1000;
const Float_t TAGgeoTrafo::fgkMuToMm        = 1e-3;
const Float_t TAGgeoTrafo::fgkCmToMm        = 10;
const Float_t TAGgeoTrafo::fgkMmToCm        = 1e-1;
const Float_t TAGgeoTrafo::fgkMToCm         = 100;
const Float_t TAGgeoTrafo::fgkCmToM         = 1e-2;
const Float_t TAGgeoTrafo::fgkNsToSec         = 1e-9;
const Float_t TAGgeoTrafo::fgkSecToNs         = 1e+9;


//_____________________________________________________________________________
TAGgeoTrafo::TAGgeoTrafo() 
: TAGaction(fgDefaultActName.Data(), "TAGgeoTrafo - Geometry Transformations"),
  fMatrixList(new TObjArray())
{
   fMatrixList->SetOwner(true);
   TGeoIdentity* id =  new TGeoIdentity();
   fIdentity = new TGeoHMatrix(*id);
}

//_____________________________________________________________________________
TAGgeoTrafo::~TAGgeoTrafo() 
{
   delete fMatrixList;
   delete fIdentity;
}

/*
  Transformations of points in space
*/
//_____________________________________________________________________________
const TGeoHMatrix* TAGgeoTrafo::GetTrafo(const char* name) const
{
   const TGeoHMatrix* matrix = (const TGeoHMatrix*)fMatrixList->FindObject(name);
   if (matrix)
	  return matrix;
   else {
	  Error("GetTrafo", "No matrix with name %s found, reset to ID", name);
	  return fIdentity;
   }
}

//_____________________________________________________________________________
void TAGgeoTrafo::AddTrafo(TGeoHMatrix* mat)
{
	  fMatrixList->Add(mat);  
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::Global2Local(const char* name, TVector3& glob) const
{
   const TGeoHMatrix* mat = GetTrafo(name);
   
   Double_t local[3]  = {0., 0., 0.};
   Double_t global[3] = {glob.X(), glob.Y(), glob.Z()};
   
   mat->MasterToLocal(global, local);
   TVector3 pos(local[0], local[1], local[2]);
   
   return pos;
}   

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::Global2LocalVect(const char* name, TVector3& glob) const
{
   const TGeoHMatrix* mat = GetTrafo(name);
   
   Double_t local[3]  = {0., 0., 0.};
   Double_t global[3] = {glob.X(), glob.Y(), glob.Z()};
   
   mat->MasterToLocalVect(global, local);
   TVector3 pos(local[0], local[1], local[2]);
   
   return pos;
}   

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::Local2Global(const char* name, TVector3& loc) const
{
   const TGeoHMatrix* mat = GetTrafo(name);
   
   Double_t local[3]  = {loc.X(), loc.Y(), loc.Z()};
   Double_t global[3] = {0., 0., 0.};
   
   mat->LocalToMaster(local, global);
   TVector3 pos(global[0], global[1], global[2]);

   return pos;
}   

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::Local2GlobalVect(const char* name, TVector3& loc) const
{
   const TGeoHMatrix* mat = GetTrafo(name);
   
   Double_t local[3]  = {loc.X(), loc.Y(), loc.Z()};
   Double_t global[3] = {0., 0., 0.};
   
   mat->LocalToMasterVect(local, global);
   TVector3 pos(global[0], global[1], global[2]);
   
   return pos;
}   

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromBMLocalToGlobal(TVector3 apoi)
{
  TVector3 glb_poi = Local2Global("trafoBM", apoi);
  return glb_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromVTLocalToGlobal(TVector3 apoi)
{
  TVector3 glb_poi = Local2Global("trafoVT", apoi);
  return glb_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromCALocalToGlobal(TVector3 apoi)
{
  TVector3 glb_poi = Local2Global("trafoCA", apoi);
  return glb_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromITLocalToGlobal(TVector3 apoi)
{
  TVector3 glb_poi = Local2Global("trafoIT", apoi);
  return glb_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromDCLocalToGlobal(TVector3 apoi)
{
  TVector3 glb_poi = Local2Global("trafoDC", apoi);
  return glb_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromTWLocalToGlobal(TVector3 apoi)
{
   TVector3 glb_poi = Local2Global("trafoTW", apoi);
   return glb_poi;
}

/*
  Transformations of vectors in space:: local --> global
*/
//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromBMLocalToGlobal(TVector3 avec)
{
  TVector3 glb_vec = Local2GlobalVect("trafoBM", avec);
  return glb_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromITLocalToGlobal(TVector3 avec)
{
  TVector3 glb_vec = Local2GlobalVect("trafoIT", avec);
  return glb_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromCALocalToGlobal(TVector3 avec)
{
  TVector3 glb_vec = Local2GlobalVect("trafoCA", avec);
  return glb_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromDCLocalToGlobal(TVector3 avec)
{
  TVector3 glb_vec = Local2GlobalVect("trafoDC", avec);
  return glb_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromVTLocalToGlobal(TVector3 avec)
{
  TVector3 glb_vec = Local2GlobalVect("trafoVT", avec);
  return glb_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromTWLocalToGlobal(TVector3 avec)
{
  TVector3 glb_vec = Local2GlobalVect("trafoTW", avec);
  return glb_vec;
}

/*
  Transformations of point in space:: global --> Local
*/
//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromGlobalToBMLocal(TVector3 apoi)
{
  TVector3 lcl_poi = Global2Local("trafoBM", apoi);
  return lcl_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromGlobalToDCLocal(TVector3 apoi)
{
  TVector3 lcl_poi = Global2Local("trafoDC", apoi);
  return lcl_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromGlobalToITLocal(TVector3 apoi)
{
  TVector3 lcl_poi = Global2Local("trafoIT", apoi);
  return lcl_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromGlobalToCALocal(TVector3 apoi)
{
  TVector3 lcl_poi = Global2Local("trafoCA", apoi);
  return lcl_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromGlobalToVTLocal(TVector3 apoi)
{
  TVector3 lcl_poi = Global2Local("trafoVT", apoi);
  return lcl_poi;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::FromGlobalToTWLocal(TVector3 apoi)
{
  TVector3 lcl_poi = Global2Local("trafoTW", apoi);
  return lcl_poi;
}

/*
  Transformations of vectors in space:: global --> Local
*/
//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromGlobalToBMLocal(TVector3 avec)
{
  TVector3 lcl_vec = Global2LocalVect("trafoBM", avec);
  return lcl_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromGlobalToDCLocal(TVector3 avec)
{
  TVector3 lcl_vec = Global2LocalVect("trafoDC", avec);
  return lcl_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromGlobalToCALocal(TVector3 avec)
{
  TVector3 lcl_vec = Global2LocalVect("trafoCA", avec);
  return lcl_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromGlobalToITLocal(TVector3 avec)
{
  TVector3 lcl_vec = Global2LocalVect("trafoIT", avec);
  return lcl_vec;
}

//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromGlobalToVTLocal(TVector3 avec)
{
  TVector3 lcl_vec = Global2LocalVect("trafoVT", avec);
  return lcl_vec;
}


//_____________________________________________________________________________
TVector3 TAGgeoTrafo::VecFromGlobalToTWLocal(TVector3 avec)
{
  TVector3 lcl_vec = Global2LocalVect("trafoTW", avec);
  return lcl_vec;
}


//_____________________________________________________________________________
bool TAGgeoTrafo::InitGeo(TString ifile) 
{
  //Initialization of Geom Parameters
  Info("InitGeo()"," Init Geo ");

  gSystem->ExpandPathName(ifile);

  LoadIRGeo(ifile);
  LoadBMGeo(ifile);
  LoadDCGeo(ifile);
  LoadInTrGeo(ifile);
  LoadCalGeo(ifile);
  LoadVtxGeo(ifile);
  LoadTofGeo(ifile);

  return kTRUE;
}

//_____________________________________________________________________________
bool TAGgeoTrafo::LoadIRGeo(TString ifile) 
{
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 

  ifstream incF;
  incF.open(ifile.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", ifile.Data());
    return kTRUE;
  }

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'S')) {
      //Start counter
      sscanf(bufConf, "S%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadIRGeo()","Load Start Counter geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetSCCenter(my1,my2,my3);
      SetSCAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoSC");
	  AddTrafo(trafo);    
	} else if(strchr(bufConf,'D')) {
      //Veto
      sscanf(bufConf, "D%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadIRGeo()","Load Veto geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetVECenter(my1,my2,my3);
      SetVEAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoVE");
	  AddTrafo(trafo);    
    }
  }

  return kTRUE;
}

//_____________________________________________________________________________
bool TAGgeoTrafo::LoadBMGeo(TString ifile) 
{
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 
  
  ifstream incF;
  incF.open(ifile.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", ifile.Data());
    return kTRUE;
  }
  
  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'B')) {
      //Start counter
      sscanf(bufConf, "B%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadBMGeo()","Load Beam Monitor geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetBMCenter(my1,my2,my3);
      SetBMAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoBM");
	  AddTrafo(trafo);    
    }
  }
  return kTRUE;
}

//_____________________________________________________________________________
bool TAGgeoTrafo::LoadInTrGeo(TString ifile) 
{
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 
  
  ifstream incF;
  incF.open(ifile.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", ifile.Data());
    return kTRUE;
  }
  
  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'B')) {
      //Start counter
      sscanf(bufConf, "B%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadITGeo()","Load Beam Monitor geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetITCenter(my1,my2,my3);
      SetITAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoIT");
	  AddTrafo(trafo);    
    }
  }
  return kTRUE;
}
//_____________________________________________________________________________
bool TAGgeoTrafo::LoadCalGeo(TString ifile) 
{
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 
  
  ifstream incF;
  incF.open(ifile.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", ifile.Data());
    return kTRUE;
  }
  
  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'B')) {
      //Start counter
      sscanf(bufConf, "B%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadCAGeo()","Load Beam Monitor geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetCACenter(my1,my2,my3);
      SetCAAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoCA");
	  AddTrafo(trafo);    
    }
  }
  return kTRUE;
}
//_____________________________________________________________________________
bool TAGgeoTrafo::LoadDCGeo(TString ifile) 
{
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 
  
  ifstream incF;
  incF.open(ifile.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", ifile.Data());
    return kTRUE;
  }
  
  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'B')) {
      //Start counter
      sscanf(bufConf, "B%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadDCGeo()","Load Beam Monitor geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetDCCenter(my1,my2,my3);
      SetDCAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoDC");
	  AddTrafo(trafo);    
    }
  }
  return kTRUE;
}

//_____________________________________________________________________________
bool TAGgeoTrafo::LoadVtxGeo(TString ifile) 
{
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 
  
  ifstream incF;
  incF.open(ifile.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", ifile.Data());
    return kTRUE;
  }
  
  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'V')) {
      //Start counter
      sscanf(bufConf, "V%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadVtxGeo()","Load Vertex geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetVTCenter(my1,my2,my3);
      SetVTAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoVT");
	  AddTrafo(trafo);
    }
  }

  return kTRUE;
}

//_____________________________________________________________________________
bool TAGgeoTrafo::LoadTofGeo(TString ifile) 
{
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 
  
  ifstream incF;
  incF.open(ifile.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", ifile.Data());
    return kTRUE;
  }
  
  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'W')) {
      //Start counter
      sscanf(bufConf, "W%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      Info("LoadTofGeo()","Load TofWall Front geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetTWCenter(my1,my2,my3);
      SetTWAngles(my4,my5,my6);
	  TGeoRotation rot;
	  rot.RotateX(my5); rot.RotateY(my4); rot.RotateZ(my6);   
	  TGeoTranslation trans(my1, my2, my3);
	  TGeoHMatrix  transfo;
	  transfo  = trans;
	  transfo *= rot;
	  TGeoHMatrix* trafo = new TGeoHMatrix(transfo);
	  trafo->SetName("trafoTW");
	  AddTrafo(trafo);    
    }
  }
  return kTRUE;
}
