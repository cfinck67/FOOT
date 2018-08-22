/*!
  \file
  \version $Id: TAMSDparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAMSDparGeo.
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

#include "TAMSDparGeo.hxx"


TString TAMSDparGeo::fgkDefParaName = "msdGeo";

//_____________________________________________________________________________
TAMSDparGeo::TAMSDparGeo()
: TAVTbaseParGeo()
{
}

//_____________________________________________________________________________
TAMSDparGeo::~TAMSDparGeo()
{
}

//_____________________________________________________________________________
//  copy constructor
TAMSDparGeo::TAMSDparGeo( TAMSDparGeo* original )
  : TAVTbaseParGeo(original),
    m_nSensors_X(original->m_nSensors_X),
    m_nSensors_Y(original->m_nSensors_Y),
    m_nSensors_Z(original->m_nSensors_Z)
{

}

//_____________________________________________________________________________
void TAMSDparGeo::InitMaterial()
{
    m_materialOrder = {  "MSD_MEDIUM" };

    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        if( m_materialOrder[i] == "MSD_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = MSD_THICK;
            m_materialType[ m_materialOrder[i] ] = MSD_MEDIUM;
        }
    }
}

//_____________________________________________________________________________
void TAMSDparGeo::InitGeo()
{
    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTAMSDparGeo::InitGeo" << endl<< endl;

   // fill m_materialOrder, m_materialThick, m_materialType
   InitMaterial();

    m_origin = TVector3( 0, 0, 0 );
    m_center = TVector3( MSD_X, MSD_Y, MSD_Z );

    m_nSensors_X = 1;
    m_nSensors_Y = 1;
    m_nSensors_Z = MSD_NLAY;
    m_nSensors = TVector3( m_nSensors_X, m_nSensors_Y, m_nSensors_Z );

    // init sensor matrix   (z, x, y) = (layer, view, strip)
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

    m_layerDistance = MSD_LAYDIST;      // from center to center
    TVector3 sensorDimension = TVector3( MSD_WIDTH, MSD_HEIGHT, MSD_THICK );
    m_dimension = TVector3( MSD_WIDTH, MSD_HEIGHT, sensorDimension.z() + (m_nSensors_Z-1)*m_layerDistance );  //!!!!!!!!!!!!!!!!!!!!!!!!!!!  // from edge to edge              
    m_nSensor_X_Layer = MSD_NVIEW;

    // set detector dimension
  //  double width_Lx = m_dimension.x();
    double height_Ly = m_dimension.y();
    

//---------------------------------------------------------------------
//     Init SENSOR geometry
//---------------------------------------------------------------------

    double pixelDistance = 0;
    double pixelWidth_Lx = MSD_DX;
    double pixelHeight_Ly = MSD_DY;

    // pixels per sensors
    m_nPixel_X = MSD_XSTRIP;
    m_nPixel_Y = MSD_YSTRIP;
    m_pitchX = MSD_DX;
    m_pitchY = MSD_DY;
   
    // // total pixels
    // m_nPixel_X = width_Lx / (pixelWidth_Lx + pixelDistance);
    // m_nPixel_Y = height_Ly / (pixelHeight_Ly + pixelDistance);

    // pixels per sensors, same as above as far as we use 1 sensor
    m_nPixel_X = sensorDimension.x() / (pixelWidth_Lx + pixelDistance);
    m_nPixel_Y = sensorDimension.y() / (pixelHeight_Ly + pixelDistance);

    // fill sensor matrix
    for (int k=0; k<m_nSensors_Z; k++) {
        double sensor_newZ = m_origin.Z() - m_dimension.z()/2 + sensorDimension.z()/2 + k*m_layerDistance;
        
        for (int i=0; i<m_nSensors_X; i++) {
            double sensor_newX = m_origin.X() - m_dimension.x()/2 + (0.5+i)*(sensorDimension.x());
            
            for (int j=0; j<m_nSensors_Y; j++) {
                double sensor_newY = m_origin.Y() - height_Ly/2 + (1+2*j)*(sensorDimension.y()/2);

                m_volumeCount++;
                stringstream ss_bodySensorName; ss_bodySensorName << "msds" << m_volumeCount;
                stringstream ss_regionSensorName; ss_regionSensorName << "MSDS" << m_volumeCount;

                m_sensorMatrix[k][i][j]->SetMaterial( m_materialType[ "MSD_MEDIUM" ], "MSD_MEDIUM", ss_bodySensorName.str(), ss_regionSensorName.str(), m_volumeCount );

                m_sensorMatrix[k][i][j]->SetSensor( 
                        TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
                        TVector3( sensorDimension.x(), sensorDimension.y(), sensorDimension.z() ),    // sensor dimension
                        m_nPixel_X, m_nPixel_Y,
                        pixelWidth_Lx, pixelHeight_Ly, sensorDimension.z(),
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


    // create the universe volume
    if ( GlobalPar::GetPar()->geoROOT() ) {
        m_universe = gGeoManager->MakeBox("MSDuniverse",gGeoManager->GetMedium("AIR"),m_dimension.x()/2,m_dimension.y()/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
        m_universe->SetLineColor(kRed);
        gGeoManager->SetTopVisible(1);
    }

//---------------------------------------------------------------------
//     Build passive materials in ROOT and FLUKA
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//     Build sensor materials in ROOT and FLUKA
//---------------------------------------------------------------------
if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "Build sensor materials in ROOT and FLUKA" << endl;

    // for ( SensorMatrix::iterator itX = m_sensorMatrix.begin(); itX != m_sensorMatrix.end(); itX++ ) {
    //     for ( SensorPlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
    //         for ( SensorLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

    for ( unsigned int k=0; k<m_nSensors_Z; k++ ) {
        for ( unsigned int j=0; j<m_nSensors_Y; j++ ) {
            for ( unsigned int i=0; i<m_nSensors_X; i++ ) {    

                
                //ROOT addNode
                if ( GlobalPar::GetPar()->geoROOT() )   {
                    if ( !gGeoManager->GetVolume( m_sensorMatrix[k][i][j]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_sensorMatrix[k][i][j]->GetMaterialRegionName() << endl;

                    TVector3 globalCoord = m_sensorMatrix[k][i][j]->GetCenter();
                    Local2Global(&globalCoord);
                    m_universe->AddNode( gGeoManager->GetVolume( m_sensorMatrix[k][i][j]->GetMaterialRegionName().c_str() ), 
                                        m_sensorMatrix[k][i][j]->GetNodeID() , 
                                        new TGeoCombiTrans( globalCoord.x(), globalCoord.y(), globalCoord.z(), 
                                        new TGeoRotation("null,",0,0,0) ) );
                    if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "\t"<<m_sensorMatrix[k][i][j]->GetMaterialRegionName()<<"  "<<m_sensorMatrix[k][i][j]->GetRegionName()<<" ", globalCoord.Print();

                }
                    // m_sensorMatrix[k][i][j]->AddNodeToUniverse( m_universe );

                    // boidies
                if ( GlobalPar::GetPar()->geoFLUKA() ) {
                    
                    TVector3 minCoord = TVector3( m_sensorMatrix[k][i][j]->GetMinCoord().x(), m_sensorMatrix[k][i][j]->GetMinCoord().y(), m_sensorMatrix[k][i][j]->GetMinCoord().z() );
                    TVector3 maxCoord = TVector3( m_sensorMatrix[k][i][j]->GetMaxCoord().x(), m_sensorMatrix[k][i][j]->GetMaxCoord().y(), m_sensorMatrix[k][i][j]->GetMaxCoord().z() );
                    Local2Global( &minCoord );
                    Local2Global( &maxCoord );

                    stringstream ss;
                    ss << setiosflags(ios::fixed) << setprecision(6);
                    ss <<  "RPP " << m_sensorMatrix[k][i][j]->GetBodyName() <<  "     " 
                                << minCoord.x() << " " << maxCoord.x() << " "
                                << minCoord.y() << " " << maxCoord.y() << " "
                                << minCoord.z() << " " << maxCoord.z() << endl;
                    
                    m_bodyPrintOut[ m_sensorMatrix[k][i][j]->GetMaterialName() ].push_back( ss.str() );
                    m_bodyName    [ m_sensorMatrix[k][i][j]->GetMaterialName() ].push_back( m_sensorMatrix[k][i][j]->GetBodyName() );

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
string TAMSDparGeo::PrintParameters() {
  
  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);

  string precision = "D+00";

  outstr << "c     MICRO STRIP DETECTOR PARAMETERS " << endl;
  outstr << endl;    
  
  map<string, int> intp;
  intp["xstripMSD"] = m_nPixel_X;
  intp["ystripMSD"] = m_nPixel_Y;
  intp["nlayMSD"] = m_nSensors_Z;
  for (auto i : intp){
    outstr << "      integer " << i.first << endl;
    outstr << "      parameter (" << i.first << " = " << i.second << ")" << endl;
    // outstr << "ciao"<< endl;
  }

  map<string, double> doublep;
  doublep["dxMSD"] = MSD_DX;
  doublep["dyMSD"] = MSD_DY;
  doublep["xminMSD"] = MSD_XMIN;
  doublep["yminMSD"] = MSD_YMIN;
  for (auto i : doublep){
    outstr << "      double precision " << i.first << endl;
    outstr << "      parameter (" << i.first << " = " << i.second << precision << ")" << endl;
    // outstr << endl;    
  }
  
  outstr << endl;
  // cout<<outstr.str().length()<<endl;
  
  return outstr.str();

}

