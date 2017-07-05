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

#include "foot_geo.h"
#include "GlobalPar.hxx"

//_____________________________________________________________________________
//  copy constructor
TAVTparGeo::TAVTparGeo( TAVTparGeo* original ) :

    m_rotation(original->m_rotation), 
    m_origin(original->m_origin),  // current position
    m_center(original->m_center),  // current position
    m_dimension(original->m_dimension),

    m_nSensors_X(original->m_nSensors_X),
    m_nSensors_Y(original->m_nSensors_Y),
    m_nSensors_Z(original->m_nSensors_Z),
    m_NSensors (original->m_NSensors),

    m_materialOrder(original->m_materialOrder),

    m_materialThick(original->m_materialThick),
    m_materialType(original->m_materialType),

    m_siliconSensorThick_Lz(original->m_siliconSensorThick_Lz),
    m_layerDistance(original->m_layerDistance),

    m_nPixel_X(original->m_nPixel_X),
    m_nPixel_Y(original->m_nPixel_Y)
{

    SensorMatrix m_sensorMatrix = original->m_sensorMatrix;
}


//_____________________________________________________________________________
void TAVTparGeo::InitGeo()
{

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTAVTparGeo::InitGeo" << endl<< endl;

    m_origin = TVector3(0,0,0);
    m_center = TVector3(VTX_X, VTX_Y, VTX_Z);

    m_nSensors_X = 1;
    m_nSensors_Y = 1;
    m_nSensors_Z = VTX_NLAY;
    TVector3 m_NSensors = TVector3( m_nSensors_X, m_nSensors_Y, m_nSensors_Z );

    // init sensor matrix   (z, x, y) = (layer, col, row)
    m_sensorMatrix.resize( m_nSensors_Z );
    for (int k=0; k<m_nSensors_Z; k++) {
        m_sensorMatrix[k].resize( m_nSensors_X );
        for (int i=0; i<m_nSensors_X; i++) {
            m_sensorMatrix[k][i].resize( m_nSensors_Y );
            for (int j=0; j<m_nSensors_Y; j++) {
                m_sensorMatrix[k][i][j] = new IronPlate();
            }
        }
    }
    
    // fill m_materialOrder, m_materialThick, m_materialType 
    // InitMaterial();


    m_layerDistance = VTX_LAYDIST;          // from center to center
    m_siliconSensorThick_Lz = VTX_THICK;    // ONLY silicon

    // set detector dimension
    double length_Lz = m_siliconSensorThick_Lz + (m_nSensors_Z-1)*m_layerDistance; // from edge to edge
    m_dimension = TVector3( VTX_WIDTH, VTX_HEIGHT, length_Lz );
    double width_Lx = m_dimension.x();
    double height_Ly = m_dimension.y();
    
    double sensorDistance = 0;
    double pixelDistance = 0;

    double pixelWidth_Lx = 0.002;
    double pixelHeight_Ly = 0.002;

    if ( GlobalPar::GetPar()->Debug() > 2 )  {
        cout << "m_layerDistance " << m_layerDistance << endl;
        cout << "length_Lz " << length_Lz << endl;
    }

    double sensor_Width_Lx = width_Lx - (sensorDistance*(1+m_nSensors_X)) /m_nSensors_X;
    double sensor_Height_Ly = height_Ly - (sensorDistance*(1+m_nSensors_Y)) /m_nSensors_Y;
    double sensor_Length_Lz = m_siliconSensorThick_Lz;
    // double sensor_Length_Lz = m_length_Lz - ((sensorDistance+1)*m_nSensors_Z) /m_nSensors_Z;

    // pixels per sensors, same as above as far as we use 1 sensor
   m_nPixel_X = 960;//sensor_Width_Lx / (pixelWidth_Lx + pixelDistance);
   m_nPixel_Y = 928;//sensor_Height_Ly / (pixelHeight_Ly + pixelDistance);
   
    // fill sensor matrix
    for (int k=0; k<m_nSensors_Z; k++) {
        double sensor_newZ = m_origin.Z() - length_Lz/2 +0.5*m_siliconSensorThick_Lz + k*m_layerDistance;
        for (int i=0; i<m_nSensors_X; i++) {
            double sensor_newX = m_origin.X() - width_Lx/2 + (0.5+i)*(sensor_Width_Lx);
            for (int j=0; j<m_nSensors_Y; j++) {

                double sensor_newY = m_origin.Y() - height_Ly/2 + (1+2*j)*(sensor_Height_Ly/2);

                m_sensorMatrix[k][i][j]->SetMaterial( (string)VTX_MEDIUM );

                m_sensorMatrix[k][i][j]->SetSensor( 
                        TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
                        TVector3( sensor_Width_Lx, sensor_Height_Ly, sensor_Length_Lz ),    // sensor dimension
                        m_nPixel_X, m_nPixel_Y,
                        pixelWidth_Lx, pixelHeight_Ly, m_siliconSensorThick_Lz,
                        pixelDistance, pixelDistance, 0, //layerDistance,
                        TVector3(0,0,0)
                 );
                // if (m_debug) 
                if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "sensor center ",    TVector3( sensor_newX, sensor_newY, sensor_newZ ).Print();
            }
        }
    }

    // FillMaterialCollection();


    m_rotation = new TRotation();
    // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
    m_rotation->SetYEulerAngles( 0,0,0 );
   
   m_Pitch_X = m_Pitch_Y = 20.7;
}


//_____________________________________________________________________________
Float_t TAVTparGeo::GetPositionU(Int_t column) const
{
   return ((2*column - m_nPixel_X + 1 ) * m_Pitch_X)/2 ;
}

//_____________________________________________________________________________
Float_t TAVTparGeo::GetPositionV(Int_t line) const
{
   return -((2*line - m_nPixel_Y + 1 ) * m_Pitch_Y)/2 ;
   
}

//_____________________________________________________________________________
TVector3 TAVTparGeo::GetPosition( int layer, int col, int row )  {
    // TVector3 sensorCoord = GetSensorCoortdinates( int layer, int col, int row );
    // TVector3 pos = m_sensorMatrix[sensorCoord.z()][sensorCoord.x()][sensorCoord.y()]->GetPosition();
    TVector3 pos = m_sensorMatrix[layer][0][0]->GetPosition( col, row );
    Local2Global(&pos);
    return pos;
}

//_____________________________________________________________________________
void TAVTparGeo::Global2Local( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
    *glob = *glob - m_center;
} 

//_____________________________________________________________________________
void TAVTparGeo::Global2Local_RotationOnly( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
} 

//_____________________________________________________________________________
void TAVTparGeo::Local2Global( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAVTparGeo::Local2Global_RotationOnly( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
} 

//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::GetVolume()
{
    // if ( (medium = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
    // TGeoMedium* vacuum_med = new TGeoMedium("Vacuum",0, gGeoManager->GetMaterial("air"));
    // TGeoMedium* silicon = new TGeoMedium( "silicon_med", 1, gGeoManager->GetMaterial("air") );
    // TGeoMedium* kapton = new TGeoMedium( "kapton_med", 2, gGeoManager->GetMaterial("air") );
    // TGeoMedium* epoxy = new TGeoMedium( "epoxy_med", 3, gGeoManager->GetMaterial("air") );
    // TGeoMedium* aluminium = new TGeoMedium( "aluminium_med", 4, gGeoManager->GetMaterial("air") );
    // TGeoMedium* siCFoam = new TGeoMedium( "siCFoam_med", 5, gGeoManager->GetMaterial("air") );

    if ( GlobalPar::GetPar()->Debug() > 1 ) {
        cout << endl << "VT List of Materil\n ";
        TIter next( gGeoManager->GetListOfMaterials() );
        while ( TGeoMaterial *obj = (TGeoMaterial*) next() ) {
          cout << obj->GetName () << endl;
        }
        cout << endl << "List of Media\n ";
        TIter nnext( gGeoManager->GetListOfMedia() );
        while ( TGeoMedium *obj = (TGeoMedium *) nnext()  ) {
          cout << obj->GetName () << endl;
        }
    }

    double width_Lx = m_dimension.X();
    double height_Ly = m_dimension.Y();

    // create main box
   // TGeoVolume *box = gGeoManager->MakeBox("ITbox",gGeoManager->GetMedium("Vacuum_med"),width_Lx+1,height_Ly+1,m_dimension.z()+0.5); //top è scatola che conterrà tutto (dimensioni in cm)
   TGeoVolume *box = gGeoManager->MakeBox("ITbox",gGeoManager->GetMedium("Air_med"),width_Lx/2,height_Ly/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
   gGeoManager->SetTopVisible(1);

    TGeoVolume *siliconFoil = gGeoManager->MakeBox("siliconFoil",gGeoManager->GetMedium("Silicon_med"),width_Lx/2,height_Ly/2,m_siliconSensorThick_Lz/2); //top è scatola che conterrà tutto (dimensioni in cm)
    siliconFoil->SetLineColor(kOrange);
    siliconFoil->SetFillColor(kOrange);
   int c=0;

    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  (-0.5*m_dimension.z())+(0.5*m_siliconSensorThick_Lz), new TGeoRotation("null,",0,0,0)));
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0, 0, new TGeoRotation("null,",0,0,0)));
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  (0.5*m_dimension.z())-(0.5*m_siliconSensorThick_Lz), new TGeoRotation("null,",0,0,0)));
    

    return box;
}

//_____________________________________________________________________________
void TAVTparGeo::InitMaterial()
{
    m_materialOrder = {  "VTX_MEDIUM", 
                        // "ITR_EPO_MEDIUM",
                        // "ITR_COV_MEDIUM",
                        // "ITR_AL_MEDIUM",
                        // "ITR_KAP_MEDIUM",
                        // "ITR_AL_MEDIUM",
                        // "ITR_COV_MEDIUM",
                        // "ITR_FOAM_MEDIUM",
                        // "ITR_COV_MEDIUM",
                        // "ITR_AL_MEDIUM",
                        // "ITR_KAP_MEDIUM",
                        // "ITR_AL_MEDIUM",
                        // "ITR_COV_MEDIUM",
                        // "ITR_EPO_MEDIUM",
                        "VTX_MEDIUM"
                         };

    
    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        if( m_materialOrder[i] == "VTX_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = VTX_THICK;
            m_materialType[ m_materialOrder[i] ] = VTX_MEDIUM;
        }
   }

}




// **************** VERTEXING      *******************************************************


// //_____________________________________________________________________________
// TGeoVolume* TAVTparGeo::BuildVertex(const char* basemoduleName, const char *vertexName)
// {
//    TGeoVolume* vertex = 0x0; 
   
//    for(Int_t iSensor = 0; iSensor < GetSensorsN(); iSensor++) {   
//    TGeoHMatrix* hm = GetTransfo(iSensor);
//    vertex = TAVTparGeo::AddVertexModule(hm, basemoduleName, vertexName);
//    }
   
//    return vertex;
// }

// //_____________________________________________________________________________
// TGeoVolume* TAVTparGeo::AddVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
// {
//    if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
//    new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
//    }
   
//    TGeoVolume* vertex = gGeoManager->FindVolumeFast(vertexName);
//    if ( vertex == 0x0 ) {
//    Int_t nSensors = GetSensorsN();
      
//    Float_t posZ1 = (*GetPosition(0))(2)*0.9;
//    Float_t posZ2 = (*GetPosition(nSensors-1))(2)*1.1;

//    TGeoMedium   *med;
//    TGeoMaterial *mat;
//    if ( (mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
//       mat = new TGeoMaterial("Vacuum",0,0,0);            
//    if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
//       med = new TGeoMedium("Vacuum",1,mat);
//    vertex = gGeoManager->MakeBox(vertexName, med, fHeight/2., fHeight/2., (posZ2-posZ1)/2.); // volume corresponding to vertex
//    } 
   
//    // create module
//    TGeoMaterial* matMod;
//    TGeoMedium*   medMod;
   
//    if ( (matMod = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Si")) == 0x0 )
//    matMod = new TGeoMaterial("Si", 28.09, 14, 2.3);
//    if ( (medMod = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Si")) == 0x0 )
//    medMod = new TGeoMedium("Si",2,matMod);
   
//    TGeoBBox *box = new TGeoBBox(Form("%s_Box",basemoduleName), fWidth/2, fHeight/2, fThick/2.);
   
//    Int_t nbModule = 0;
   
//    TGeoVolume *vertexMod = new TGeoVolume(Form("%s_Vertex",basemoduleName),box, medMod);
//    vertexMod->SetLineColor(kAzure-5);
//    vertexMod->SetTransparency( TAGgeoTrafo::GetDefaultTransp());
   
//    TObjArray* list = vertex->GetNodes();
//    if (list) {
//    for (Int_t i = 0; i < list->GetEntries(); ++i) {
//       TGeoVolume* vol = (TGeoVolume*)list->At(i);
//       if (vol) {
//          TString name(vol->GetName());
//          if ( name.Contains(Form("%s_Vertex",basemoduleName)) )
//             nbModule++;
//       }
//    }
//    }

//    vertex->AddNode(vertexMod, nbModule, new TGeoHMatrix(*hm));
//    return vertex;
// }



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

