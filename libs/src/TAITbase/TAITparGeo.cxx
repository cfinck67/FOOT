/*!
  \file
  \version $Id: TAITparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAITparGeo.
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

#include "TAITparMap.hxx"
#include "TAITparGeo.hxx"

#include "foot_geo.h"
#include "GlobalPar.hxx"

//##############################################################################


//_____________________________________________________________________________
TAITparGeo::TAITparGeo() {

    m_nPassiveLayersPerBoard_z = 13;
    m_volumeCount = -1;
    m_passiveCount = -1;
    m_setW_0number = 2;

    m_debug = GlobalPar::GetPar()->Debug();

    // fill m_materialOrder, m_materialThick, m_materialType
    InitMaterial();

};



//_____________________________________________________________________________
void TAITparGeo::InitMaterial() {

    m_materialOrder = {  "ITR_MEDIUM",
                        "ITR_EPO_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_KAP_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_FOAM_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_KAP_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_EPO_MEDIUM",
                        "ITR_MEDIUM"
                         };

    m_passiveMaterial = {  "ITR_EPO_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_KAP_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_FOAM_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_KAP_MEDIUM",
                        "ITR_AL_MEDIUM",
                        "ITR_COV_MEDIUM",
                        "ITR_EPO_MEDIUM",
                         };

    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {

        if( m_materialOrder[i] == "ITR_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_MEDIUM;
        }
        if(  m_materialOrder[i] == "ITR_EPO_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_EPO_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_EPO_MEDIUM;
        }
        if( m_materialOrder[i] == "ITR_COV_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_COV_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_COV_MEDIUM;
        }
        if( m_materialOrder[i] == "ITR_AL_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_AL_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_AL_MEDIUM;
        }
        if( m_materialOrder[i] == "ITR_KAP_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_KAP_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_KAP_MEDIUM;
        }
        if( m_materialOrder[i] == "ITR_FOAM_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_FOAM_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_FOAM_MEDIUM;
        }

    }

}



//_____________________________________________________________________________
//  copy constructor
TAITparGeo::TAITparGeo( TAITparGeo* original ) :

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
    m_nPixel_Y(original->m_nPixel_Y)         {

    SensorMatrix m_sensorMatrix = original->m_sensorMatrix;
}




//_____________________________________________________________________________
void TAITparGeo::InitGeo()  {

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTAITparGeo::InitGeo" << endl<< endl;

    m_origin = TVector3(0,0,0);                 // center in local coord.
    m_center = TVector3(ITR_X, ITR_Y, ITR_Z);   // center in global coord.

    // set number of sensors
    m_nSensors_X = 1;
    m_nSensors_Y = 1;
    m_nSensors_Z = ITR_NLAY;
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

    // evaluate detector dimension and layer distance using materials
    // !!!!!!!!!!!!!!!!!!!!  better put a variable in the footgeo.h  !!!!!!!!!!!!!!!!!!!!!
    double length_Lz = 0;       // from edge to edge
    m_passiveMaterialThick = 0;
    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        length_Lz += m_materialThick[ m_materialOrder[i] ];

        if ( ( m_materialOrder[i] != "ITR_MEDIUM" ) )  // only passive material
            m_passiveMaterialThick += m_materialThick[ m_materialOrder[i] ];
            // m_layerDistance += m_materialThick[ m_materialOrder[i] ];
    }

    m_dimension = TVector3( ITR_WIDTH, ITR_HEIGHT, length_Lz );

    m_siliconSensorThick_Lz = ITR_THICK;     // ONLY silicon
    m_layerDistance = m_passiveMaterialThick; 
    m_layerDistance += m_siliconSensorThick_Lz;     // from center to center

    if ( GlobalPar::GetPar()->Debug() > 2 )  {
        cout << "m_layerDistance " << m_layerDistance << endl;
        cout << "length_Lz " << m_dimension.z() << endl;
    }

//---------------------------------------------------------------------
//     Init SENSOR geometry
//---------------------------------------------------------------------

    // fixed
    double sensorDistance = 0;
    double pixelDistance = 0;

    double pixelWidth_Lx = ITR_DX;
    double pixelHeight_Ly = ITR_DY;

    // evaluate sensor dimension 
    double sensor_Width_Lx = m_dimension.x() - (sensorDistance*(1+m_nSensors_X)) /m_nSensors_X;
    double sensor_Height_Ly = m_dimension.y() - (sensorDistance*(1+m_nSensors_Y)) /m_nSensors_Y;
    double sensor_Length_Lz = m_siliconSensorThick_Lz;

    // pixels per sensors, same as above as far as we use 1 sensor
    m_nPixel_X = sensor_Width_Lx / (pixelWidth_Lx + pixelDistance);
    m_nPixel_Y = sensor_Height_Ly / (pixelHeight_Ly + pixelDistance);


    // fill sensor matrix
    for (int k=0; k<m_nSensors_Z; k++) {
        double sensor_newZ = m_origin.Z() - length_Lz/2 + 0.5*sensor_Length_Lz + k*m_layerDistance;
        for (int i=0; i<m_nSensors_X; i++) {
            double sensor_newX = m_origin.X() - m_dimension.x()/2 + (0.5+i)*(sensor_Width_Lx);
            for (int j=0; j<m_nSensors_Y; j++) {

                double sensor_newY = m_origin.Y() - m_dimension.y()/2 + (1+2*j)*(sensor_Height_Ly/2);

                stringstream ss_bodySensorName; ss_bodySensorName << "itr" << setw(m_setW_0number) << setfill('0') << ++m_volumeCount;
                stringstream ss_regionSensorName; ss_regionSensorName << "ITRS" << j << k << i;
                m_sensorMatrix[k][i][j]->SetMaterial( m_materialType[ "ITR_MEDIUM" ], "ITR_MEDIUM", ss_bodySensorName.str(), ss_regionSensorName.str(), m_volumeCount );


                m_sensorMatrix[k][i][j]->SetSensor(
                        TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
                        TVector3( sensor_Width_Lx, sensor_Height_Ly, sensor_Length_Lz ),    // sensor dimension
                        m_nPixel_X, m_nPixel_Y,
                        pixelWidth_Lx, pixelHeight_Ly, sensor_Length_Lz,
                        pixelDistance, pixelDistance, 0, //m_layerDistance,
                        TVector3(0,0,0)
                 );

                if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "sensor center ",    TVector3( sensor_newX, sensor_newY, sensor_newZ ).Print();
            }
        }
    }

    m_rotation = new TRotation();
    // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
    m_rotation->SetYEulerAngles( 0,0,0 );



//---------------------------------------------------------------------
//     Init passive materials geometry
//---------------------------------------------------------------------
  
    // minimum z coordinate of the passive group
    float position = -m_dimension.z()/2 + m_siliconSensorThick_Lz;
    int i = 0;
    m_passiveMatrix.resize( 1 );

    // TODO     Loop over alle the board  Y
    int j = 0;
    m_passiveMatrix[0].resize( 1 );

    // check same material as passive layers
    if ( m_passiveMaterial.size() != m_nPassiveLayersPerBoard_z ) 
        cout << "ERROR << TAITparGeo::InitGeo()  -->  m_passiveMaterial.size() != m_nPassiveLayersPerBoard_z\n", exit(0);

    m_passiveMatrix[0][0].resize( m_passiveMaterial.size() );
    // loop over the board passive layers
    for ( unsigned int k=0; k<m_passiveMaterial.size(); k++ ) {

        string matID = m_passiveMaterial.at(k);
        position +=  m_materialThick[ matID ]/2;    // increase the z coordinate

        stringstream ss_bodyName;      ss_bodyName << "itr" << setw(m_setW_0number) << setfill('0') << ++m_volumeCount;
        stringstream ss_regionSensorName;      ss_regionSensorName << "ITRP" << setw(m_setW_0number) << setfill('0') << ++m_passiveCount;

        // init matrix of passive material for a board
        m_passiveMatrix[i][j][k] = new FootBox( TVector3( 0,0, position ),          // position
                                                TVector3( m_dimension.x(), m_dimension.y(), m_materialThick[matID] ),   // dimension
                                                m_materialType[ matID ],            // name of the material
                                                matID,                              // name of the material-region in the foot_geo.h
                                                ss_bodyName.str(),                  // FLUKA body name
                                                ss_regionSensorName.str(),          // FLUKA region name
                                                m_volumeCount                       // volume ID num
                                                );


        position +=  m_materialThick[ matID ]/2;

    }

    // create the universe volume
    if ( GlobalPar::GetPar()->geoROOT() ) {
        m_universe = gGeoManager->MakeBox("ITuniverse",gGeoManager->GetMedium("AIR"),m_dimension.x()/2,m_dimension.y()/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
        gGeoManager->SetTopVisible(1);
    }

//---------------------------------------------------------------------
//     Build passive materials in ROOT and FLUKA
//---------------------------------------------------------------------

    for ( PassiveMatrix::iterator itX = m_passiveMatrix.begin(); itX != m_passiveMatrix.end(); itX++ ) {
        for ( PassivePlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
            for ( PassiveLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

                //ROOT addNode
                if ( GlobalPar::GetPar()->geoROOT() )    
                    (*itZ)->AddNodeToUniverse( m_universe );
                // TVector3 trasl = (*itZ)->GetPosition();
                // m_universe->AddNode( gGeoManager->GetVolume( (*itZ)->GetMaterialRegionName().c_str() ), nVol++ , new TGeoCombiTrans( trasl.x(), trasl.y(), trasl.z(), new TGeoRotation("null,",0,0,0)) );
                // (*itZ)->SetNodeID(nVol);

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
                                            << "5 " << (*itZ)->GetBodyName() << endl;
                        
                    m_regionPrintOut[ (*itZ)->GetMaterialName() ].push_back( ssr.str() );
                }

            }
        }
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
TVector3 TAITparGeo::GetPosition( int layer, int col, int row )  {
    // TVector3 sensorCoord = GetSensorCoortdinates( int layer, int col, int row );
    // TVector3 pos = m_sensorMatrix[sensorCoord.z()][sensorCoord.x()][sensorCoord.y()]->GetPosition();
    TVector3 pos = m_sensorMatrix[layer][0][0]->GetPosition( col, row );
    Local2Global(&pos);
    return pos;
}





//_____________________________________________________________________________
void TAITparGeo::Global2Local( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
    *glob = *glob - m_center;
}

//_____________________________________________________________________________
void TAITparGeo::Global2Local_RotationOnly( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
}

//_____________________________________________________________________________
void TAITparGeo::Local2Global( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAITparGeo::Local2Global_RotationOnly( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
}




//_____________________________________________________________________________
TGeoVolume* TAITparGeo::GetVolume() {

    if ( !GlobalPar::GetPar()->geoROOT() ) 
        cout << "ERROR << TAITparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

    return m_universe;
}





//_____________________________________________________________________________
void TAITparGeo::PrintBodies( string geoFileName ){

    if ( !GlobalPar::GetPar()->geoFLUKA() ) 
        cout << "ERROR << TAITparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
    

    // ofstream geofile;
    // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );
    
    // geofile << "* ***Inner Tracker" << endl;
    cout << "* BODIES ***Inner Tracker" << endl;

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
void TAITparGeo::PrintRegions( string geoFileName ){

    if ( !GlobalPar::GetPar()->geoFLUKA() ) 
        cout << "ERROR << TAITparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

    // ofstream geofile;
    // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );
      
    // geofile << "* ***Inner Tracker" << endl;
    cout << "* REGION ***Inner Tracker" << endl;

    // loop in order of the material alfabeth
    for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
        // loop over all body of the same material
        for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
            // geofile << (*itRegion);
            cout << (*itRegion);
            if (m_debug > 0)    cout << (*itRegion);
        }        
    }





/*


  string itrID = "itr";
  unsigned int count = 0;

  

  string defineRegion; //this string contains 5 itrbox
  string addRegion;  //this string contains +itr
  string removeRegion; //this string contains -itr


  for (unsigned int i = 0; i < m_regionOrder.size() ; ++i) {
    defineRegion = "5 itrbox ";
    addRegion = "+itr";
    removeRegion = "-itr";
    if ( i == 0) {
      m_streamRegion << defineRegion << addRegion;
      m_streamRegion << setw(m_setW_0number) << setfill('0') << std::right << count;
      m_regionMap[m_regionOrder.at(i)] = m_streamRegion.str();
      m_streamRegion.str("");
      continue;
    }
    m_streamRegion.str("");
    m_streamRegion << defineRegion << removeRegion;
    m_streamRegion << setw(m_setW_0number) << setfill('0') << std::right << count << ' ';
    m_streamRegion << addRegion << setw(m_setW_0number) << setfill('0') << std::right << count+1;
    m_regionMap[m_regionOrder.at(i)] = m_streamRegion.str();
    m_streamRegion.str("");

    if ( i == m_regionOrder.size()-1 ){

      m_streamRegion << defineRegion << removeRegion;
      m_streamRegion << setw(m_setW_0number) << setfill('0') << std::right << count;
      m_regionMap[m_regionOrder.at(i)] = m_streamRegion.str();
      m_streamRegion.str("");
      break;
    }
    ++count;

  }

  // for ( std::map<string, string>::iterator it = m_regionMap.begin(); it != m_regionMap.end(); it++ ) {
  //
  //   cout << setw(13) << setfill(' ') << std::left << it->first << it->second << endl;
  // }

  for ( std::vector<string>::iterator it = m_regPrintOrder.begin(); it != m_regPrintOrder.end(); it++){

    geofile << setw(13) << setfill( ' ' ) << std::left << *it << m_regionMap[*it] << endl;

  }
  */

  // geofile.close();
}

// //_____________________________________________________________________________
// void TAITparGeo::AddTransMatrix(TGeoHMatrix* mat, Int_t idx)
// {
//   if (idx == -1)
//    fMatrixList->Add(mat);
//   else {
//    TGeoHMatrix* oldMat = GetTransfo(idx);
//    if (oldMat)
//     RemoveTransMatrix(oldMat);
//    fMatrixList->AddAt(mat, idx);
//   }
// }

// //_____________________________________________________________________________
// void TAITparGeo::RemoveTransMatrix(TGeoHMatrix* mat)
// {
//    if (!fMatrixList->Remove(mat))
//     printf("Cannot remove matrix");
// }

// //_____________________________________________________________________________
// TGeoHMatrix* TAITparGeo::GetTransfo(Int_t iSensor)
// {
//    if (iSensor < 0 || iSensor >= GetSensorsN()) {
//     cout << "Wrong detector id number" << endl;
//     return 0x0;
//    }

//    return (TGeoHMatrix*)fMatrixList->At(iSensor);
// }




// //_____________________________________________________________________________
// TGeoVolume* TAITparGeo::BuildVertex(const char* basemoduleName, const char *vertexName)
// {
//    TGeoVolume* vertex = 0x0;

//    for(Int_t iSensor = 0; iSensor < GetSensorsN(); iSensor++) {
// 	  TGeoHMatrix* hm = GetTransfo(iSensor);
// 	  vertex = TAITparGeo::AddVertexModule(hm, basemoduleName, vertexName);
//    }

//    return vertex;
// }

// //_____________________________________________________________________________
// TGeoVolume* TAITparGeo::AddVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
// {
//    if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
// 	  new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
//    }

//    TGeoVolume* vertex = gGeoManager->FindVolumeFast(vertexName);
//    if ( vertex == 0x0 ) {
// 	  Int_t nSensors = GetSensorsN();

// 	  Float_t posZ1 = (*GetPosition(0))(2)*0.9;
// 	  Float_t posZ2 = (*GetPosition(nSensors-1))(2)*1.1;

// 	  TGeoMedium   *med;
// 	  TGeoMaterial *mat;
// 	  if ( (mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
// 		 mat = new TGeoMaterial("Vacuum",0,0,0);
// 	  if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
// 		 med = new TGeoMedium("Vacuum",1,mat);
// 	  vertex = gGeoManager->MakeBox(vertexName, med, fHeight/2., fHeight/2., (posZ2-posZ1)/2.); // volume corresponding to vertex
//    }

//    // create module
//    TGeoMaterial* matMod;
//    TGeoMedium*   medMod;

//    if ( (matMod = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Si")) == 0x0 )
// 	  matMod = new TGeoMaterial("Si", 28.09, 14, 2.3);
//    if ( (medMod = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Si")) == 0x0 )
// 	  medMod = new TGeoMedium("Si",2,matMod);

//    TGeoBBox *box = new TGeoBBox(Form("%s_Box",basemoduleName), fWidth/2, fHeight/2, fThick/2.);

//    Int_t nbModule = 0;

//    TGeoVolume *vertexMod = new TGeoVolume(Form("%s_Vertex",basemoduleName),box, medMod);
//    vertexMod->SetLineColor(kAzure-5);
//    vertexMod->SetTransparency( TAGgeoTrafo::GetDefaultTransp());

//    TObjArray* list = vertex->GetNodes();
//    if (list) {
// 	  for (Int_t i = 0; i < list->GetEntries(); ++i) {
// 		 TGeoVolume* vol = (TGeoVolume*)list->At(i);
// 		 if (vol) {
// 			TString name(vol->GetName());
// 			if ( name.Contains(Form("%s_Vertex",basemoduleName)) )
// 			   nbModule++;
// 		 }
// 	  }
//    }

//    vertex->AddNode(vertexMod, nbModule, new TGeoHMatrix(*hm));
//    return vertex;
// }

// //_____________________________________________________________________________
// TGeoVolume* TAITparGeo::AddTarget(const Float_t dx, const Float_t dy, const Float_t dz, const char *targetName)
// {
//    if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
// 	  new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
//    }

//    // create module
//    TGeoMaterial* matTarget;
//    TGeoMedium*   medTarget;

//    if ( (matTarget = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
// 	  matTarget = new TGeoMaterial("Vacuum", 0., 0., 0.);
//    if ( (medTarget = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
// 	  medTarget = new TGeoMedium("Vacuum", 1, matTarget);

//    TGeoVolume* target = gGeoManager->MakeBox(targetName, medTarget, dx, dy, dz);
//    target->SetVisibility(true);
//    target->SetTransparency( TAGgeoTrafo::GetDefaultTransp());

//    return target;
// }


// //_____________________________________________________________________________
// TEveGeoShapeExtract* TAITparGeo::BuildExtractVertex(const char* basemoduleName, const char *vertexName)
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
// 	  TGeoHMatrix* hm = GetTransfo(iSensor);
// 	  TEveGeoShapeExtract* vertexMod = AddExtractVertexModule(hm, basemoduleName, vertexName);
// 	  vertexExtract->AddElement(vertexMod);
//    }

//    return vertexExtract;
// }

// //_____________________________________________________________________________
// TEveGeoShapeExtract* TAITparGeo::AddExtractVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
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
// TEveGeoShapeExtract* TAITparGeo::AddExtractTarget(const Float_t dx, const Float_t dy, const Float_t dz, const char *targetName)
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
void TAITparGeo::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAITparGeo::ToStream(ostream& os, Option_t*) const
{
//  os << "TAITparGeo " << GetName() << endl;
//  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
//     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}
