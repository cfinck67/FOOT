
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

#include "GlobalPar.hxx"
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
   
   if (GlobalPar::GetPar()->geoFLUKA())
      PrintFluka();
   
   return vertexMod;
}

//_____________________________________________________________________________
void TAVTparGeo::PrintFluka()
{
   static Int_t count = 0;
   const Char_t* matName = fEpiMat.Data();

   TVector3 pos = GetSensorPosition(count);
   stringstream ss;
   ss << setiosflags(ios::fixed) << setprecision(6);
   ss <<  "RPP " << Form("VTXP%d", count) <<  "     "
   << pos.x() - fTotalSize.X()/2.  << " " << pos.x() +  fTotalSize.X()/2. << " "
   << pos.y() - fTotalSize.Y()/2.  << " " << pos.y() +  fTotalSize.Y()/2. << " "
   << pos.z() - fTotalSize.Z()/2.  << " " << pos.z() +  fTotalSize.Z()/2. << endl;
   
   m_bodyPrintOut[matName].push_back( ss.str() );
   
   m_regionName[matName].push_back(Form("vtxp%d", count));
   m_bodyName[matName].push_back(Form("VTXP%d", count));
   m_bodyPrintOut[matName].push_back(Form("VTXP%d", count));
   m_bodyName[matName].push_back(Form("VTXP%d", count));
}

//_____________________________________________________________________________
string TAVTparGeo::PrintParameters()
{   
   stringstream outstr;
   outstr << setiosflags(ios::fixed) << setprecision(5);
   
   string precision = "D+00";
   
   outstr << "c     VERTEX PARAMETERS " << endl;
   outstr << endl;
   
   map<string, int> intp;
   intp["xpixVTX"] = fPixelsNx;
   intp["ypixVTX"] = fPixelsNx;
   intp["nlayVTX"] = fSensorsN;
   for (auto i : intp){
      outstr << "      integer " << i.first << endl;
      outstr << "      parameter (" << i.first << " = " << i.second << ")" << endl;
      // outstr << endl;
   }
   
   map<string, double> doublep;
   doublep["dxVTX"] = fTotalSize[0];
   doublep["dyVTX"] = fTotalSize[1];
   doublep["xminVTX"] = fMinPosition[0];
   doublep["yminVTX"] = fMinPosition[1];
   for (auto i : doublep){
      outstr << "      double precision " << i.first << endl;
      outstr << "      parameter (" << i.first << " = " << i.second << precision << ")" << endl;
      // outstr << endl;
   }
   outstr << endl;
   
   return outstr.str();
}
//_____________________________________________________________________________
string TAVTparGeo::PrintBodies()
{
   if ( !GlobalPar::GetPar()->geoFLUKA() )
      cout << "ERROR << TAVTbaseParGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
   
   stringstream outstr;
   outstr << "* ***Vertex" << endl;
   
   // loop in order of the material alfabeth
   for ( map<string, vector<string> >::iterator itMat = m_bodyPrintOut.begin(); itMat != m_bodyPrintOut.end(); itMat++ ) {
      // loop over all body of the same material
      for ( vector<string>::iterator itBody = (*itMat).second.begin(); itBody != (*itMat).second.end(); itBody++ ) {
         outstr << (*itBody);
      }
   }
   return outstr.str();
}

//_____________________________________________________________________________
string TAVTparGeo::PrintRegions()
{
   if ( !GlobalPar::GetPar()->geoFLUKA() )
      cout << "ERROR << TAVTbaseParGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
   
   stringstream outstr;
   outstr << "* ***Vertex" << endl;
   
   // loop in order of the material alfabeth
   for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
      // loop over all body of the same material
      for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
         outstr << (*itRegion);
      }
   }
   return outstr.str();
}

//_____________________________________________________________________________
string TAVTparGeo::PrintSubtractBodiesFromAir()
{
   
   if ( !GlobalPar::GetPar()->geoFLUKA() )
      cout << "ERROR << TAVTbaseParGeo::PrintSubtractMaterialFromAir()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);
   
   
   stringstream outstr;
   // loop in order of the material alfabeth
   for ( map<string, vector<string> >::iterator itMat = m_bodyName.begin(); itMat != m_bodyName.end(); itMat++ ) {
      // loop over all region of the same material
      for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
         outstr << " -" << (*itRegion);
      }
   }
   return outstr.str();
   
}

//_____________________________________________________________________________
string TAVTparGeo::PrintAssignMaterial()
{
   if ( !GlobalPar::GetPar()->geoFLUKA() )
      cout << "ERROR << TAVTbaseParGeo::PrintAssignMaterial()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);
   
   
   // loop in order of the material alfabeth
   stringstream outstr;
   for ( map<string, vector<string> >::iterator itMat = m_regionName.begin(); itMat != m_regionName.end(); itMat++ ) {
      
      // check dimension greater than 0
      if ( (*itMat).second.size() == 0 ) {
         cout << "ERROR << TAVTbaseParGeo::PrintAssignMaterial  ::  "<<endl, exit(0);
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
      bool isMag = GlobalPar::GetPar()->IncludeDI();
      
      outstr << setw(10) << setfill( ' ' ) << std::right  << isMag ;
      
      outstr << endl;
   }
   
   return outstr.str();
}


