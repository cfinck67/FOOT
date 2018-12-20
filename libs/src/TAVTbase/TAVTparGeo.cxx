
////////////////////////////////////////////////////////////////////////////////
//                                   MI28
//
//                 0
//                ------------------------------------
//              0 |                 |                 |
//                |                 |                 |
//  Y(+/-9604.8)  |                 |                 |
//                |                 |                 |
//                |                 |                 |
//                |                 |                 |Nv = Nline = 928
//                |                 | (0,0)           |
//                -------------------------------------
//                |                 |                 |
//                |                 |                 |
//                |                 |                 |
//                |                 |                 |
//                |                 |                 |
//                |                 |                 |
//                |                 |                 |
//                -------------------------------------
//                              Nu = Ncol = 960
//                                X (+/-9936)
////////////////////////////////////////////////////////////////////////////////

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"

#include "TAGgeoTrafo.hxx" 
#include "TAGmaterials.hxx"

#include "TAVTparGeo.hxx"

//##############################################################################

/*!
  \class TAVTparGeo TAVTparGeo.hxx "TAVTparGeo.hxx"
  \brief Map and Geometry parameters for vertex. **
*/

ClassImp(TAVTparGeo);

const TString TAVTparGeo::fgkBaseName      = "VT";
const TString TAVTparGeo::fgkDefParaName   = "vtGeo";


//______________________________________________________________________________
TAVTparGeo::TAVTparGeo()
: TAVTbaseParGeo()
{
   // Standard constructor
   fgDefaultGeoName = "./geomaps/TAVTdetector.map";
}

//______________________________________________________________________________
TAVTparGeo::~TAVTparGeo()
{
   // Destructor
}



//_____________________________________________________________________________
void TAVTparGeo::DefineMaterial()
{
   // Silicon material
   TAVTbaseParGeo::DefineMaterial();
   
   // Pixel material
   TGeoMixture* mix = TAGmaterials::Instance()->CreateMixture(fPixMat, fPixMatDensities, fPixMatProp, fPixMatDensity);
   if (fDebugLevel) {
      printf("pixels material:\n");
      mix->Print();
   }
}

//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::BuildVertex(const char *vertexName, const char* basemoduleName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
	  new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
 
   // define box
   DefineMaxMinDimension();
   
   TGeoVolume* vertex = gGeoManager->FindVolumeFast(vertexName);
   if ( vertex == 0x0 ) {
      TGeoMedium*   med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("AIR");
      vertex = gGeoManager->MakeBox(vertexName,med,fSizeBox.X()/2.,fSizeBox.Y()/2.,fSizeBox.Z()/2.); // volume corresponding to vertex
   }
   
   TGeoVolume* vertexMod = 0x0; 
   
   for(Int_t iSensor = 0; iSensor < GetNSensors(); iSensor++) {
	  TGeoHMatrix* hm = GetTransfo(iSensor);
      vertexMod = AddModule(Form("%s%d",basemoduleName, iSensor), vertexName);
      
      TGeoHMatrix* transf = (TGeoHMatrix*)hm->Clone();
	  vertex->AddNode(vertexMod, iSensor, transf);
   }
   
   return vertex;
}

//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::AddModule(const char* basemoduleName, const char *vertexName)
{
    // create M28 module
   const Char_t* matName = fEpiMat.Data();
   TGeoMedium*   medMod = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);

   TGeoBBox *box = new TGeoBBox(Form("%s_Box",basemoduleName), fEpiSize.X()/2., fEpiSize.Y()/2.,
                                fTotalSize.Z()/2.);
      
   TGeoVolume *vertexMod = new TGeoVolume(Form("%s_Vertex",basemoduleName),box, medMod);
   vertexMod->SetLineColor(kAzure-5);
   vertexMod->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return vertexMod;
}
