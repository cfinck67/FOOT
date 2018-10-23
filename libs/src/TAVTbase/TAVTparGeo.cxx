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


TString TAVTparGeo::fgkDefParaName = "vtGeo";

//_____________________________________________________________________________
TAVTparGeo::TAVTparGeo()
: TAVTbaseParGeo()
{
}

//_____________________________________________________________________________
void TAVTparGeo::InitMaterial()
{
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
   
   DefineMaterial();
}

//_____________________________________________________________________________
void TAVTparGeo::DefineMaterial()
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // create material
   TGeoMaterial* mat = 0x0;;
   TGeoMedium*   med = 0x0;
   
   const Char_t* matName = VTX_MEDIUM.Data();

   if ( (mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject(matName)) == 0x0 )
      mat = new TGeoMaterial(matName, 28.09, 14, 2.3);
   if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName)) == 0x0 )
      med = new TGeoMedium(matName,1,mat);
}

//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::BuildVertex(const char *vertexName, const char* basemoduleName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // define box
   Float_t sizeZ = (VTX_LAYDIST2+VTX_THICK/2.)*VTX_NLAY;
   
   TGeoVolume* vertex = gGeoManager->FindVolumeFast(vertexName);
   if ( vertex == 0x0 ) {
      const Char_t* matName = VTX_MEDIUM.Data();
      TGeoMedium*   med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
      TGeoMaterial* mat =  (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject(matName);
      vertex = gGeoManager->MakeBox(vertexName,med,VTX_WIDTH/2., VTX_HEIGHT/2., sizeZ); // volume corresponding to vertex
   }
   
   TGeoVolume* vertexMod = 0x0;
   
   for(Int_t iSensor = 0; iSensor < VTX_NLAY; iSensor++) {
      
      vertexMod = AddVertexModule(Form("%s%d",basemoduleName, iSensor), vertexName);
      
      TGeoHMatrix* transf = new TGeoHMatrix();
      double vec[3] = {0, 0, -VTX_LAYDIST2+iSensor*VTX_LAYDIST2}; // completly hard coded, should be avoid
      transf->SetTranslation(vec);
      vertex->AddNode(vertexMod, iSensor, transf);
   }
   
   return vertex;
}

//_____________________________________________________________________________
TGeoVolume* TAVTparGeo::AddVertexModule(const char* basemoduleName, const char *vertexName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // create module
   const Char_t* matName = VTX_MEDIUM.Data();
   TGeoMedium*   med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
   TGeoMaterial* mat =  (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject(matName);
   
   TGeoBBox *box = new TGeoBBox(Form("%s_Box",basemoduleName), VTX_WIDTH/2., VTX_HEIGHT/2., VTX_THICK/2.);
   
   TGeoVolume *vertexMod = new TGeoVolume(Form("%s_Vertex",basemoduleName),box, med);
   vertexMod->SetLineColor(kAzure-5);
   vertexMod->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return vertexMod;
}

//_____________________________________________________________________________
void TAVTparGeo::InitGeo()  {

    if ( fDebugLevel> 0 )     cout << "\n\nTAVTparGeo::InitGeo" << endl<< endl;

   // fill m_materialOrder, m_materialThick, m_materialType
   InitMaterial();

    m_origin = TVector3(0,0,0);                         // center in local coord.
    m_center = TVector3(VTX_X, VTX_Y, VTX_Z);           // center in global coord.
    
    m_nSensors = TVector3( 1, 1, VTX_NLAY );            // set number of sensors


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
    if ( fDebugLevel> 0 ) cout << " Init SENSOR geometry " << endl;

    double pixelDistance = 0;  

    // evaluate sensor dimension 
    // double sensor_Width_Lx = VTX_SENSE_WIDTH;
    // double sensor_Height_Ly = VTX_SENSE_HEIGHT;
    // double sensor_Length_Lz = m_siliconSensorThick_Lz;
    TVector3 sensorDimension = TVector3( VTX_SENSE_WIDTH, VTX_SENSE_HEIGHT, m_siliconSensorThick_Lz );
    TVector3 passiveSiDimension = TVector3( VTX_WIDTH, VTX_HEIGHT, m_siliconSensorThick_Lz );

    // pixels per sensors
    m_nPixel_X = VTX_XPIX;
    m_nPixel_Y = VTX_YPIX;
    m_pitchX = VTX_DX;
    m_pitchY = VTX_DY;  


    // fill sensor matrix
    double sensor_newZ = m_origin.Z() - m_dimension.z()/2 +0.5*m_siliconSensorThick_Lz;
    m_sensorMatrix.resize( m_nSensors.Z() );
    for (int k=0; k<m_nSensors.Z(); k++) {
        if ( k!=0 )     // increment the layer distance Z, distance not uniform
            sensor_newZ += ( k%2 != 0 ? m_layerDistance_samePair : m_layerDistance_interPair );
        m_sensorMatrix[k].resize( m_nSensors.X() );
        for (int i=0; i<m_nSensors.X(); i++) {
            double sensor_newX = m_origin.X();  
            m_sensorMatrix[k][i].resize( m_nSensors.Y() );
            for (int j=0; j<m_nSensors.Y(); j++) {
                double sensor_newY = m_origin.Y();

                m_sensorMatrix[k][i][j] = new IronPlate();

                m_volumeCount++;
                stringstream ss_bodySensorName; ss_bodySensorName << "vtxs" << m_volumeCount;
                stringstream ss_regionSensorName; ss_regionSensorName << "VTXS" << m_volumeCount;
                m_sensorMatrix[k][i][j]->SetMaterial( m_materialType[ "VTX_MEDIUM" ], "VTX_MEDIUM", ss_bodySensorName.str(), ss_regionSensorName.str(), m_volumeCount );

                m_sensorMatrix[k][i][j]->SetSensor(
                        TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
                        TVector3( sensorDimension.x(), sensorDimension.y(), sensorDimension.z() ),    // sensor dimension
                        m_nPixel_X, m_nPixel_Y,
                        m_pitchX, m_pitchY, m_siliconSensorThick_Lz,
                        pixelDistance, pixelDistance, 0, //layerDistance,
                        TVector3(0,0,0)
                 );

                if ( fDebugLevel> 0 ) cout << "sensor center ",    TVector3( sensor_newX, sensor_newY, sensor_newZ ).Print();
            }
        }
    }

    m_rotation = new TRotation();
    // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
    m_rotation->SetYEulerAngles( 0,0,0 );

//---------------------------------------------------------------------
//     Init passive materials geometry
//---------------------------------------------------------------------
    if ( fDebugLevel> 0 ) cout << "Init passive materials geometry" << endl;

    double passiveSi_Z = m_origin.Z() - m_dimension.z()/2 +0.5*m_siliconSensorThick_Lz;
    m_passiveMatrix.resize( m_nSensors.Z() );
    for (int k=0; k<m_nSensors.Z(); k++) {
        if ( k!=0 )     // increment the layer distance Z, distance not uniform
            passiveSi_Z += ( k%2 != 0 ? m_layerDistance_samePair : m_layerDistance_interPair );
        m_passiveMatrix[k].resize( m_nSensors.X() );
        for (int i=0; i<m_nSensors.X(); i++) {
            double passiveSi_X = m_origin.X() + sensorDimension.x()/2 + VTX_XDEAD - passiveSiDimension.x()/2;
            passiveSi_X *= ( i>1 ? -1 : 1 );   // commenta!
            m_passiveMatrix[k][i].resize( m_nSensors.Y() );
            for (int j=0; j<m_nSensors.Y(); j++) {
                double passiveSi_Y = m_origin.Y() + passiveSiDimension.y()/2 - sensorDimension.y()/2;
                passiveSi_Y *= ( j%2 == 0 ? -1 : 1 );   // commenta!

                m_passiveCount++;
                stringstream ss_bodyPassiveName;      ss_bodyPassiveName << "vtxp" << m_passiveCount;
                stringstream ss_regionPassiveName;      ss_regionPassiveName << "VTXP" << m_passiveCount;


                m_passiveMatrix[k][i][j] = new FootBox( TVector3( passiveSi_X, passiveSi_Y, passiveSi_Z ),          // position passive
                                                TVector3( passiveSiDimension.x(), passiveSiDimension.y(), passiveSiDimension.z() ),   // dimension passive
                                                (string)VTX_MEDIUM,                         // name of the material
                                                "VTX_MEDIUM",                       // name of the material-region in the foot_geo.h
                                                ss_bodyPassiveName.str(),                  // FLUKA body name
                                                ss_regionPassiveName.str(),          // FLUKA region name
                                                m_volumeCount                       // volume ID num
                                                );


                if ( fDebugLevel> 0 ) cout << "passive center ",    TVector3( passiveSi_X, passiveSi_Y, passiveSi_Z ).Print();

            }
        }
    }


    // create the universe volume
    if ( GlobalPar::GetPar()->geoROOT() ) {
        m_universe = gGeoManager->MakeBox("VTXuniverse",gGeoManager->GetMedium("AIR"),m_dimension.x()/2,m_dimension.y()/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
        m_universe->SetLineColor(kGreen);
        m_universe->SetVisibility(kTRUE);
        gGeoManager->SetTopVisible(10);
    }

//---------------------------------------------------------------------
//     Build passive materials in ROOT and FLUKA
//---------------------------------------------------------------------
    if ( fDebugLevel> 0 ) cout << "Build passive materials in ROOT and FLUKA" << endl;

    // int sensor_k = 0;
    // cout << m_passiveMatrix.size()<<endl;
    // for ( PassiveMatrix::iterator itX = m_passiveMatrix.begin(); itX != m_passiveMatrix.end(); itX++ ) {
    //     cout << (*itX).size()<<endl;
    //     int sensor_i = 0;
    //     for ( PassivePlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
    //         cout << (*itY).size()<<endl;
    //         int sensor_j = 0;
    //         for ( PassiveLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

    for ( unsigned int k=0; k<m_nSensors.Z(); k++ ) {
        for ( unsigned int j=0; j<m_nSensors.Y(); j++ ) {
            for ( unsigned int i=0; i<m_nSensors.X(); i++ ) {    
                
                //ROOT addNode
                if ( GlobalPar::GetPar()->geoROOT() )    {
                    if ( !gGeoManager->GetVolume( m_passiveMatrix[k][i][j]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_passiveMatrix[k][i][j]->GetMaterialRegionName() << endl;

                    TVector3 globalCoord = m_passiveMatrix[k][i][j]->GetPosition();
                    Local2Global(&globalCoord);
                    m_universe->AddNode( gGeoManager->GetVolume( m_passiveMatrix[k][i][j]->GetMaterialRegionName().c_str() ), 
                                        m_passiveMatrix[k][i][j]->GetNodeID() , 
                                        new TGeoCombiTrans( globalCoord.x(), globalCoord.y(), globalCoord.z(), 
                                        new TGeoRotation("null,",0,0,0) ) );
                    if ( fDebugLevel > 0 ) cout <<"\t"<<m_passiveMatrix[k][i][j]->GetMaterialRegionName()<<"  ", globalCoord.Print();
                }

                // boidies
                if ( GlobalPar::GetPar()->geoFLUKA() ) {

                    TVector3 minCoord = TVector3( m_passiveMatrix[k][i][j]->GetMinCoord().x(), m_passiveMatrix[k][i][j]->GetMinCoord().y(), m_passiveMatrix[k][i][j]->GetMinCoord().z() );
                    TVector3 maxCoord = TVector3( m_passiveMatrix[k][i][j]->GetMaxCoord().x(), m_passiveMatrix[k][i][j]->GetMaxCoord().y(), m_passiveMatrix[k][i][j]->GetMaxCoord().z() );
                    // minCoord.Print();
                    // maxCoord.Print();
                    Local2Global( &minCoord );
                    Local2Global( &maxCoord );
                    // minCoord.Print();
                    // maxCoord.Print();
		    
                    stringstream ss;    
                    ss << setiosflags(ios::fixed) << setprecision(6);
                    ss <<  "RPP " << m_passiveMatrix[k][i][j]->GetBodyName() <<  "     " 
                                << minCoord.x() << " " << maxCoord.x() << " "
                                << minCoord.y() << " " << maxCoord.y() << " "
                                << minCoord.z() << " " << maxCoord.z() << endl;
                    // cout << "Build passive materials in ROOT and FLUKA " << ss.str() <<  endl;
                    m_bodyPrintOut  [ m_passiveMatrix[k][i][j]->GetMaterialName() ].push_back( ss.str() );
                    m_bodyName      [ m_passiveMatrix[k][i][j]->GetMaterialName() ].push_back( m_passiveMatrix[k][i][j]->GetBodyName() );

                    // regions
                    stringstream ssr;    ssr << setw(13) << setfill( ' ' ) << std::left << m_passiveMatrix[k][i][j]->GetRegionName()
                                            << "5 " << m_passiveMatrix[k][i][j]->GetBodyName() << " - " << 
                                            m_sensorMatrix[k][i][j]->GetBodyName() << endl;
                    // cout << "Build passive materials in ROOT and FLUKA " << ssr.str() <<  endl;
                    m_regionPrintOut[ m_passiveMatrix[k][i][j]->GetMaterialName() ].push_back( ssr.str() );
                    m_regionName    [ m_passiveMatrix[k][i][j]->GetMaterialName() ].push_back( m_passiveMatrix[k][i][j]->GetRegionName() );
                    if ( genfit::FieldManager::getInstance()->getFieldVal( TVector3( minCoord ) ).Mag() == 0 && genfit::FieldManager::getInstance()->getFieldVal( TVector3( maxCoord ) ).Mag() == 0 )
                        m_magneticRegion[ m_passiveMatrix[k][i][j]->GetRegionName() ] = 0;
                    else 
                        m_magneticRegion[ m_passiveMatrix[k][i][j]->GetRegionName() ] = 1;

                }

                // sensor_j++;
            }
            // sensor_i++;
        }
        // sensor_k++;
    }


//---------------------------------------------------------------------
//     Build sensor materials in ROOT and FLUKA
//---------------------------------------------------------------------
    if ( fDebugLevel > 0 ) cout << "Build sensor materials in ROOT and FLUKA" << endl;

    // for ( SensorMatrix::iterator itX = m_sensorMatrix.begin(); itX != m_sensorMatrix.end(); itX++ ) {
    //     for ( SensorPlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
    //         for ( SensorLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

    for ( unsigned int k=0; k<m_nSensors.Z(); k++ ) {
        for ( unsigned int j=0; j<m_nSensors.Y(); j++ ) {
            for ( unsigned int i=0; i<m_nSensors.X(); i++ ) {    

                //ROOT addNode
                if ( GlobalPar::GetPar()->geoROOT() )   {
                    if ( !gGeoManager->GetVolume( m_sensorMatrix[k][i][j]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_sensorMatrix[k][i][j]->GetMaterialRegionName() << endl;

                    TVector3 globalCoord = m_sensorMatrix[k][i][j]->GetCenter();
                    Local2Global(&globalCoord);
                    m_universe->AddNode( gGeoManager->GetVolume( m_sensorMatrix[k][i][j]->GetMaterialRegionName().c_str() ), 
                                        m_sensorMatrix[k][i][j]->GetNodeID() , 
                                        new TGeoCombiTrans( globalCoord.x(), globalCoord.y(), globalCoord.z(), 
                                        new TGeoRotation("null,",0,0,0) ) );
                    if ( fDebugLevel > 0 ) cout <<"\t"<<m_sensorMatrix[k][i][j]->GetMaterialRegionName()<<"  ", globalCoord.Print();
                }

                    // boidies
                if ( GlobalPar::GetPar()->geoFLUKA() ) {
                    
                    TVector3 minCoord = TVector3( m_sensorMatrix[k][i][j]->GetMinCoord().x(), m_sensorMatrix[k][i][j]->GetMinCoord().y(), m_sensorMatrix[k][i][j]->GetMinCoord().z() );
                    TVector3 maxCoord = TVector3( m_sensorMatrix[k][i][j]->GetMaxCoord().x(), m_sensorMatrix[k][i][j]->GetMaxCoord().y(), m_sensorMatrix[k][i][j]->GetMaxCoord().z() );
                    Local2Global( &minCoord );
                    Local2Global( &maxCoord );

		    if ( k==0 && j==0 && i==0 ) m_xmin = minCoord.x();
		    else{
		      if ( m_xmin != minCoord.x()){
    			cout << "Error in VTX xmin coord " << m_xmin
    			     << "  " << minCoord.x() << endl;
		      }
		    }
							    
		    if ( k==0 && j==0 && i==0 ) m_ymin = minCoord.y();
		    else{
                if ( m_ymin != minCoord.y()){
                    cout << "Error in VTX ymin coord" << m_ymin
                         << "  " << minCoord.y() << endl;
                }
		    }

                    stringstream ss;
                    ss << setiosflags(ios::fixed) << setprecision(6);
                    ss <<  "RPP " << m_sensorMatrix[k][i][j]->GetBodyName() <<  "     " 
                                << minCoord.x() << " " << maxCoord.x() << " "
                                << minCoord.y() << " " << maxCoord.y() << " "
                                << minCoord.z() << " " << maxCoord.z() << endl;
                    
                    m_bodyPrintOut  [ m_sensorMatrix[k][i][j]->GetMaterialName() ].push_back( ss.str() );
                    // m_bodyName      [ m_sensorMatrix[k][i][j]->GetMaterialName() ].push_back( m_sensorMatrix[k][i][j]->GetBodyName() );

                    // regions
                    stringstream ssr;
                    ssr << setw(13) << setfill( ' ' ) << std::left << m_sensorMatrix[k][i][j]->GetRegionName()
                        << "5 " << m_sensorMatrix[k][i][j]->GetBodyName() << endl;
                        
                    m_regionPrintOut[ m_sensorMatrix[k][i][j]->GetMaterialName() ].push_back( ssr.str() );
                    m_regionName    [ m_sensorMatrix[k][i][j]->GetMaterialName() ].push_back( m_sensorMatrix[k][i][j]->GetRegionName() );
                    if ( genfit::FieldManager::getInstance()->getFieldVal( TVector3( minCoord ) ).Mag() == 0 && genfit::FieldManager::getInstance()->getFieldVal( TVector3( maxCoord ) ).Mag() == 0 )
                        m_magneticRegion[ m_sensorMatrix[k][i][j]->GetRegionName() ] = 0;
                    else 
                        m_magneticRegion[ m_sensorMatrix[k][i][j]->GetRegionName() ] = 1;
                }
            }
        }
    } 

}


//_____________________________________________________________________________
string TAVTparGeo::PrintParameters() {
  
  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);

  string precision = "D+00";

  outstr << "c     VERTEX PARAMETERS " << endl;
  outstr << endl;    
  
  map<string, int> intp;
  intp["xpixVTX"] = m_nPixel_X;
  intp["ypixVTX"] = m_nPixel_Y;
  intp["nlayVTX"] = m_nSensors.Z();
  for (auto i : intp){
    outstr << "      integer " << i.first << endl;
    outstr << "      parameter (" << i.first << " = " << i.second << ")" << endl;
    // outstr << endl;    
  }
  
  map<string, double> doublep;
  doublep["dxVTX"] = VTX_DX;
  doublep["dyVTX"] = VTX_DY;
  doublep["xminVTX"] = m_xmin;
  doublep["yminVTX"] = m_ymin;
  for (auto i : doublep){
    outstr << "      double precision " << i.first << endl;
    outstr << "      parameter (" << i.first << " = " << i.second << precision << ")" << endl;
    // outstr << endl;    
  }
  outstr << endl;    
  
  return outstr.str();

}
