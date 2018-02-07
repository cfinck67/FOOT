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
TAVTparGeo::TAVTparGeo() {

    m_nPassiveLayersPerBoard_z = 13;
    m_volumeCount = -1;
    m_passiveCount = -1;
    m_setW_0number = 2;

    m_debug = GlobalPar::GetPar()->Debug();

    // fill m_materialOrder, m_materialThick, m_materialType
    InitMaterial();

};



//_____________________________________________________________________________
void TAVTparGeo::InitMaterial() {

    m_materialOrder = {     "VTX_MEDIUM",
                            "VTX_MEDIUM"
                             };

    m_passiveMaterial = {};


    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        if( m_materialOrder[i] == "VTX_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = VTX_THICK;
            m_materialType[ m_materialOrder[i] ] = VTX_MEDIUM;
        }
    }
}




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
    m_layerDistance_samePair(original->m_layerDistance_samePair),
    m_layerDistance_interPair(original->m_layerDistance_interPair),

    m_nPixel_X(original->m_nPixel_X),
    m_nPixel_Y(original->m_nPixel_Y)         {

    SensorMatrix m_sensorMatrix = original->m_sensorMatrix;
}





//_____________________________________________________________________________
void TAVTparGeo::InitGeo()  {

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTAVTparGeo::InitGeo" << endl<< endl;

    m_origin = TVector3(0,0,0);                         // center in local coord.
    m_center = TVector3(VTX_X, VTX_Y, VTX_Z);           // center in global coord.

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

//---------------------------------------------------------------------
//     Find DETECTOR dimension
//---------------------------------------------------------------------

    m_layerDistance_samePair  = VTX_LAYDIST1;          // from center to center
    m_layerDistance_interPair = VTX_LAYDIST2;          // from center to center
    m_siliconSensorThick_Lz = VTX_THICK;    // ONLY silicon

    // set detector dimension
    double length_Lz = m_siliconSensorThick_Lz + (VTX_NLAY/2)*m_layerDistance_samePair + (-1+VTX_NLAY/2)*m_layerDistance_interPair; // from edge to edge
    m_dimension = TVector3( VTX_WIDTH + 2*VTX_XDEAD , 2*VTX_HEIGHT - VTX_SENSE_HEIGHT, length_Lz );


//---------------------------------------------------------------------
//     Init SENSOR geometry
//---------------------------------------------------------------------
    double pixelDistance = 0;

    double pixelWidth_Lx = VTX_DX;
    double pixelHeight_Ly = VTX_DY;    

    // evaluate sensor dimension 
    // double sensor_Width_Lx = VTX_SENSE_WIDTH;
    // double sensor_Height_Ly = VTX_SENSE_HEIGHT;
    // double sensor_Length_Lz = m_siliconSensorThick_Lz;
    TVector3 sensorDimension = TVector3( VTX_SENSE_WIDTH, VTX_SENSE_HEIGHT, m_siliconSensorThick_Lz );
    TVector3 passiveSiDimension = TVector3( VTX_WIDTH, VTX_HEIGHT, m_siliconSensorThick_Lz );

    // pixels per sensors, same as above as far as we use 1 sensor
    m_nPixel_X = VTX_XPIX;
    m_nPixel_Y = VTX_YPIX;


    // fill sensor matrix
    double sensor_newZ = m_origin.Z() - m_dimension.z()/2 +0.5*m_siliconSensorThick_Lz;
    for (int k=0; k<m_nSensors_Z; k++) {
        if ( k!=0 )     // increment the layer distance Z, distance not uniform
            sensor_newZ += ( k%2 != 0 ? m_layerDistance_samePair : m_layerDistance_interPair );
        for (int i=0; i<m_nSensors_X; i++) {
            double sensor_newX = m_origin.X();  
            for (int j=0; j<m_nSensors_Y; j++) {
                double sensor_newY = m_origin.Y();

                m_volumeCount++;
                stringstream ss_bodySensorName; ss_bodySensorName << "vtxs" << m_volumeCount;
                stringstream ss_regionSensorName; ss_regionSensorName << "VTXS" << m_volumeCount;
                m_sensorMatrix[k][i][j]->SetMaterial( m_materialType[ "VTX_MEDIUM" ], "VTX_MEDIUM", ss_bodySensorName.str(), ss_regionSensorName.str(), m_volumeCount );

                m_sensorMatrix[k][i][j]->SetSensor(
                        TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
                        TVector3( sensorDimension.x(), sensorDimension.y(), sensorDimension.z() ),    // sensor dimension
                        m_nPixel_X, m_nPixel_Y,
                        pixelWidth_Lx, pixelHeight_Ly, m_siliconSensorThick_Lz,
                        pixelDistance, pixelDistance, 0, //layerDistance,
                        TVector3(0,0,0)
                 );

                if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "sensor center ",    TVector3( sensor_newX, sensor_newY, sensor_newZ ).Print();
            }
        }
    }

    m_rotation = new TRotation();
    // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
    m_rotation->SetYEulerAngles( 0,0,0 );

//---------------------------------------------------------------------
//     Init passive materials geometry
//---------------------------------------------------------------------

    double passiveSi_Z = m_origin.Z() - m_dimension.z()/2 +0.5*m_siliconSensorThick_Lz;
    for (int k=0; k<m_nSensors_Z; k++) {
        if ( k!=0 )     // increment the layer distance Z, distance not uniform
            passiveSi_Z += ( k%2 != 0 ? m_layerDistance_samePair : m_layerDistance_interPair );
        for (int i=0; i<m_nSensors_X; i++) {
            double passiveSi_X = m_origin.X() + sensorDimension.x()/2 + VTX_XDEAD - passiveSiDimension.x()/2;
            passiveSi_X *= ( i>1 ? -1 : 1 );   // commenta!
            for (int j=0; j<m_nSensors_Y; j++) {
                double passiveSi_Y = m_origin.Y() + passiveSiDimension.y()/2 - sensorDimension.y()/2;
                passiveSi_Y *= ( j%2 == 0 ? -1 : 1 );   // commenta!

                m_passiveCount++;
                stringstream ss_bodyPassiveName;      ss_bodyPassiveName << "vtxp" << m_passiveCount;
                stringstream ss_regionPassiveName;      ss_regionPassiveName << "VTXP" << m_passiveCount;


                m_passiveMatrix[i][j][k] = new FootBox( TVector3( passiveSi_X, passiveSi_Y, passiveSi_Z ),          // position passive
                                                TVector3( passiveSiDimension.x(), passiveSiDimension.y(), passiveSiDimension.z() ),   // dimension passive
                                                (string)VTX_MEDIUM,                         // name of the material
                                                "VTX_MEDIUM",                       // name of the material-region in the foot_geo.h
                                                ss_bodyPassiveName.str(),                  // FLUKA body name
                                                ss_regionPassiveName.str(),          // FLUKA region name
                                                m_volumeCount                       // volume ID num
                                                );


            }
        }
    }


    // create the universe volume
    if ( GlobalPar::GetPar()->geoROOT() ) {
        m_universe = gGeoManager->MakeBox("ITuniverse",gGeoManager->GetMedium("AIR"),m_dimension.x()/2,m_dimension.y()/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
        gGeoManager->SetTopVisible(1);
    }

//---------------------------------------------------------------------
//     Build passive materials in ROOT and FLUKA
//---------------------------------------------------------------------

    int sensor_i = 0, sensor_j = 0, sensor_k = 0;
    for ( PassiveMatrix::iterator itX = m_passiveMatrix.begin(); itX != m_passiveMatrix.end(); itX++ ) {
        for ( PassivePlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
            for ( PassiveLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

                //ROOT addNode
                if ( GlobalPar::GetPar()->geoROOT() )    
                    (*itZ)->AddNodeToUniverse( m_universe );

                // boidies
                if ( GlobalPar::GetPar()->geoFLUKA() ) {

                    TVector3 minCoord = TVector3( (*itZ)->GetMinCoord().x(), (*itZ)->GetMinCoord().y(), (*itZ)->GetMinCoord().z() );
                    TVector3 maxCoord = TVector3( (*itZ)->GetMaxCoord().x(), (*itZ)->GetMaxCoord().y(), (*itZ)->GetMaxCoord().z() );
                    Local2Global( &minCoord );
                    Local2Global( &maxCoord );

                    stringstream ss;    
                    ss << setiosflags(ios::fixed) << setprecision(6);
                    ss <<  "RPP " << (*itZ)->GetBodyName() <<  "     " 
                                << minCoord.x() << " " << maxCoord.x() << " "
                                << minCoord.y() << " " << maxCoord.y() << " "
                                << minCoord.z() << " " << maxCoord.z() << endl;
                    
                    m_bodyPrintOut[ (*itZ)->GetMaterialName() ].push_back( ss.str() );

                    // regions
                    stringstream ssr;    ssr << setw(13) << setfill( ' ' ) << std::left << (*itZ)->GetRegionName()
                                            << "5 " << (*itZ)->GetBodyName() << " - " << 
                                            m_sensorMatrix[sensor_i][sensor_j][sensor_k]->GetBodyName() << endl;
                        
                    m_regionPrintOut[ (*itZ)->GetMaterialName() ].push_back( ssr.str() );

                }

                sensor_k++;
            }
            sensor_j++;
        }
        sensor_i++;
    }


//---------------------------------------------------------------------
//     Build sensor materials in ROOT and FLUKA
//---------------------------------------------------------------------

    for ( SensorMatrix::iterator itX = m_sensorMatrix.begin(); itX != m_sensorMatrix.end(); itX++ ) {
        for ( SensorPlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
            for ( SensorLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

                //ROOT addNode
                if ( GlobalPar::GetPar()->geoROOT() )   
                    (*itZ)->AddNodeToUniverse( m_universe );

                    // boidies
                if ( GlobalPar::GetPar()->geoROOT() ) {
                    
                    TVector3 minCoord = TVector3( (*itZ)->GetMinCoord().x(), (*itZ)->GetMinCoord().y(), (*itZ)->GetMinCoord().z() );
                    TVector3 maxCoord = TVector3( (*itZ)->GetMaxCoord().x(), (*itZ)->GetMaxCoord().y(), (*itZ)->GetMaxCoord().z() );
                    Local2Global( &minCoord );
                    Local2Global( &maxCoord );

                    stringstream ss;
                    ss << setiosflags(ios::fixed) << setprecision(6);
                    ss <<  "RPP " << (*itZ)->GetBodyName() <<  "     " 
                                << minCoord.x() << " " << maxCoord.x() << " "
                                << minCoord.y() << " " << maxCoord.y() << " "
                                << minCoord.z() << " " << maxCoord.z() << endl;
                    
                    m_bodyPrintOut[ (*itZ)->GetMaterialName() ].push_back( ss.str() );

                    // regions
                    stringstream ssr;
                    ssr << setw(13) << setfill( ' ' ) << std::left << (*itZ)->GetRegionName()
                        << "5 " << (*itZ)->GetBodyName() << endl;
                        
                    m_regionPrintOut[ (*itZ)->GetMaterialName() ].push_back( ssr.str() );
                }


            }
        }
    } 

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
void TAVTparGeo::Global2Local_TranslationOnly( TVector3* glob ) {
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
void TAVTparGeo::Local2Global_TranslationOnly( TVector3* loc ) {
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAVTparGeo::Local2Global_RotationOnly( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
}





//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::GetVolume() {

    if ( !GlobalPar::GetPar()->geoROOT() ) 
        cout << "ERROR << TAVTparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

    return m_universe;
}





//_____________________________________________________________________________
void TAVTparGeo::PrintBodies( string geoFileName ) {

    if ( !GlobalPar::GetPar()->geoFLUKA() ) 
        cout << "ERROR << TAITparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

    // ofstream geofile;
    // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );

    // geofile << "* ***Vertex" << endl;
    cout << "* ***Vertex" << endl;

    // loop in order of the material alfabeth
    for ( map<string, vector<string> >::iterator itMat = m_bodyPrintOut.begin(); itMat != m_bodyPrintOut.end(); itMat++ ) {
        // loop over all body of the same material
        for ( vector<string>::iterator itBody = (*itMat).second.begin(); itBody != (*itMat).second.end(); itBody++ ) {
            // geofile << (*itBody);
            cout << (*itBody);
            if (m_debug > 0)    cout << (*itBody);
        }        
    }

    // geofile.close();
}





//_____________________________________________________________________________
void TAVTparGeo::PrintRegions( string geoFileName ) {

    if ( !GlobalPar::GetPar()->geoFLUKA() ) 
        cout << "ERROR << TAITparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

    // ofstream geofile;
    // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );

    // geofile << "* ***Vertex" << endl;
    cout << "* ***Vertex" << endl;



  // loop in order of the material alfabeth
    for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
        // loop over all body of the same material
        for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
            // geofile << (*itRegion);
            cout << (*itRegion);
            if (m_debug > 0)    cout << (*itRegion);
        }        
    }


    // geofile.close();
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


// //_____________________________________________________________________________
// TEveGeoShapeExtract* TAVTparGeo::BuildExtractVertex(const char* basemoduleName, const char *vertexName)
// {
//    Int_t nSensors = GetSensorsN();
//    Float_t posZ1 = (*GetPosition(0))(2)*0.9;
//    Float_t posZ2 = (*GetPosition(nSensors-1))(2)*1.1;
//    TGeoBBox *box = new TGeoBBox(vertexName, 11000,11000,(posZ2-posZ1)/2.);

//    TEveGeoShapeExtract* vertexExtract = new TEveGeoShapeExtract(vertexName);
//    vertexExtract->SetShape(box);
//    Float_t color[] = {0, 0, 0, 0};
//    vertexExtract->SetRGBA(color);

//    for(Int_t iSensor = 0; iSensor < nSensors; iSensor++) {
//    TGeoHMatrix* hm = GetTransfo(iSensor);
//    TEveGeoShapeExtract* vertexMod = AddExtractVertexModule(hm, basemoduleName, vertexName);
//    vertexExtract->AddElement(vertexMod);
//    }

//    return vertexExtract;
// }

// //_____________________________________________________________________________
// TEveGeoShapeExtract* TAVTparGeo::AddExtractVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
// {
//    // create module
//    static Int_t nbModule = 0;

//    TGeoBBox *box = new TGeoBBox(Form("%s_%d",basemoduleName, nbModule), fWidth/2, fHeight/2, fThick/2.);

//    TEveTrans eveTrans;
//    eveTrans.SetFrom(*hm);
//    TColor* color = gROOT->GetColor(kAzure-5);
//    Float_t rgba[4];
//    color->GetRGB(rgba[0], rgba[1], rgba[2]);
//    rgba[3] = TAGgeoTrafo::GetDefaultTransp()/100.;

//    TEveGeoShapeExtract* vertexModExtract = new TEveGeoShapeExtract(Form("%s_%d",basemoduleName, nbModule++));
//    vertexModExtract->SetShape(box);
//    vertexModExtract->SetTrans(eveTrans.Array());
//    vertexModExtract->SetRGBA(rgba);

//    return vertexModExtract;
// }

// //_____________________________________________________________________________
// TEveGeoShapeExtract* TAVTparGeo::AddExtractTarget(const Float_t dx, const Float_t dy, const Float_t dz, const char *targetName)
// {
//    TGeoBBox* box = new TGeoBBox(targetName, dx, dy, dz);

//    TColor* color = gROOT->GetColor(19);
//    Float_t rgba[4];
//    color->GetRGB(rgba[0], rgba[1], rgba[2]);
//    rgba[3] = TAGgeoTrafo::GetDefaultTransp()/100.;

//    TEveGeoShapeExtract* target = new TEveGeoShapeExtract(targetName);
//    target->SetShape(box);
//    target->SetRGBA(rgba);

//    return target;

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
