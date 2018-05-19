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
			m_materialThick[ m_materialOrder[i] ] = ITR_M28_THICK;
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

	m_nSensors (original->m_nSensors),

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
void TAITparGeo::InitGeo()  {

	if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTAITparGeo::InitGeo" << endl<< endl;

	m_origin = TVector3(0,0,0);                 // center in local coord.
	m_center = TVector3(ITR_X, ITR_Y, ITR_Z);   // center in global coord.

	// set number of sensors          TVector3( chip, plume, layer );
	m_nSensors = TVector3( ITR_NM28, ITR_NPLM, ITR_NLAY );

	// init sensor matrix   (z, y, x) = (layer, plume, chip)
	m_sensorMatrix.resize( m_nSensors.Z() );
	for (int k=0; k<m_nSensors.Z(); k++) {
		m_sensorMatrix[k].resize( m_nSensors.Y() );
		for (int j=0; j<m_nSensors.Y(); j++) {
			m_sensorMatrix[k][j].resize( m_nSensors.X() );
			for (int i=0; i<m_nSensors.X(); i++) {
				m_sensorMatrix[k][j][i] = new IronPlate();
			}
		}
	}

	// init passive matrix   (x, y, z) = (null, plume, layer)
	m_passiveMatrix.resize( 1 );
	for (int i=0; i<1; i++) {
		m_passiveMatrix[i].resize( m_nSensors.Y() );
		for (int j=0; j<m_nSensors.Y(); j++) {
			m_passiveMatrix[i][j].resize( m_passiveMaterial.size() );
			for (unsigned int k=0; k<m_passiveMaterial.size(); k++) {
				m_passiveMatrix[i][j][k] = new FootBox();    // wrong!!!!!!!!!!!!!!!!!!!!!
			}
		}
	}

	// init chip matrix   (x, y, z) = (chip, plume, layer)
	m_chipMatrix.resize( m_nSensors.X() );
	for (int i=0; i<m_nSensors.X(); i++) {
		m_chipMatrix[i].resize( m_nSensors.Y() );
		for (int j=0; j<m_nSensors.Y(); j++) {
			m_chipMatrix[i][j].resize( m_nSensors.Z() );
			for (int k=0; k<m_nSensors.Z(); k++) {
				m_chipMatrix[i][j][k] = new FootBox();   // wrong!!!!!!!!!!!!!!!!!!!!!
			}
		}
	}

//---------------------------------------------------------------------
//     Find DETECTOR dimension
//---------------------------------------------------------------------

	// evaluate detector dimension and layer distance using materials
	// !!!!!!!!!!!!!!!!!!!!  better put a variable in the footgeo.h  !!!!!!!!!!!!!!!!!!!!!

	TVector3 senseDimension = TVector3( ITR_SENSE_WIDTH, ITR_SENSE_HEIGHT, ITR_M28_THICK );

	TVector3 chipDimension = TVector3( ITR_M28_WIDTH, ITR_M28_HEIGHT, ITR_M28_THICK );
	double chipDead_X = chipDimension.x() - senseDimension.x();
	double chipDead_Ymax = ITR_M28_YDEAD;
	double chipDead_Ymin = chipDimension.y() - senseDimension.y() - chipDead_Ymax;

	m_siliconSensorThick_Lz = ITR_M28_THICK;     // ONLY silicon
	m_layerDistance = m_passiveMaterialThick + m_siliconSensorThick_Lz;   // from center to center
	m_plumeDistace_Z = ITR_PLMZDIST;                // from center to center
	m_plumeDistace_Y = ITR_PLMYDIST;                // from border to border

	double plume_Lz = 0;       // from edge to edge
	m_passiveMaterialThick = 0;
	for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
		plume_Lz += m_materialThick[ m_materialOrder[i] ];

		if ( ( m_materialOrder[i] != "ITR_MEDIUM" ) )  // only passive material
			m_passiveMaterialThick += m_materialThick[ m_materialOrder[i] ];
	}
	
	double board_z = m_passiveMaterialThick;

	TVector3 boardDimension = TVector3( ITR_BOARD_WIDTH, ITR_BOARD_HEIGHT, board_z );
	m_boardXDeadMin = ITR_BOARD_XDEAD1;
	m_boardXDeadMax = ITR_BOARD_XDEAD2;
	m_boardYDeadMin = ITR_BOARD_YDEAD1;
	m_boardYDeadMax = ITR_BOARD_YDEAD2;
	
	// double boardStagger_x = 1.5;  // distance from center
	double boardStagger_x = m_origin.x() +chipDimension.x()*m_nSensors.X()/2.+ITR_M28_DIST*(m_nSensors.X()-1)/2.+m_boardXDeadMin-boardDimension.x()/2.; // distance from center
	m_dimension = TVector3( ITR_BOARD_WIDTH + 2*boardStagger_x,  4*ITR_SENSE_HEIGHT + 2*ITR_BOARD_YDEAD2 , ITR_PLMZDIST + board_z );     // detector dim

	
	
//---------------------------------------------------------------------
//     Init SENSOR geometry
//---------------------------------------------------------------------
	if ( GlobalPar::GetPar()->Debug() > 0 ) cout << " Init SENSOR geometry " << endl;

	// fixed
	double pixelDistance = 0;

	double pixelWidth_Lx = ITR_DX;
	double pixelHeight_Ly = ITR_DY;

	// pixels per sensors, same as above as far as we use 1 sensor
	m_nPixel_X = ITR_XPIX;
	m_nPixel_Y = ITR_YPIX;

	double smalloffset_x;
	//sovrapposizione in y dei sensori:
	double displacement_y = senseDimension.y() - 0.3;

	// fill sensor matrix
	for (int k=0; k<m_nSensors.Z(); k++) {
		double sensor_newZ = ( (k%2 == 0 ? -1 : 1) * ( board_z/2 + m_siliconSensorThick_Lz/2 )  );      // distance of the center of sensors from the board center

		double offset_y = m_origin.Y() - ((m_nSensors.Y()-1)/2.)*displacement_y;    // center of the bottom sensor in Y
		for (int j=0; j<m_nSensors.Y(); j++) {
			// double sensor_newY = offset_y + j * chipDimension.y();
			double sensor_newY = offset_y + j * displacement_y;

			double offset_z = m_origin.z() + ( (j%2 == 0 ? -1 : 1) * m_plumeDistace_Z/2 ); // board center

		//offset of the sensor wrt the chip -> sensore allineato alla dx o alla sx del chip
		if ((j < 2 && k < 1) || (j >= 2 && k >= 1) ) smalloffset_x = 0.;
		else smalloffset_x = chipDead_X;

			double offset_x = m_origin.X() - ((m_nSensors.X()-1)/2.)*ITR_M28_DIST - (m_nSensors.X()/2.)*chipDimension.x() +    // end chip part of the chip line on the board 
		  senseDimension.x()/2 + smalloffset_x;     // now = first sensor center

			for (int i=0; i<m_nSensors.X(); i++) {
				double sensor_newX = offset_x + i * (chipDimension.x() + ITR_M28_DIST );

				stringstream ss_bodySensorName; ss_bodySensorName << "itrs" << j << k << i;
				stringstream ss_regionSensorName; ss_regionSensorName << "ITRS" << j << k << i;
				m_sensorMatrix[k][j][i]->SetMaterial( m_materialType[ "ITR_MEDIUM" ], "ITR_MEDIUM", ss_bodySensorName.str(), ss_regionSensorName.str(), ++m_volumeCount );


				m_sensorMatrix[k][j][i]->SetSensor(
						TVector3( sensor_newX, sensor_newY, offset_z + sensor_newZ ),              // sensor center
						TVector3( senseDimension.x(), senseDimension.y(), senseDimension.z() ),    // sensor dimension
						m_nPixel_X, m_nPixel_Y,
						pixelWidth_Lx, pixelHeight_Ly, senseDimension.z(),
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
if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "Init passive materials geometry" << endl;

	// check same material as passive layers
	if ( m_passiveMaterial.size() != m_nPassiveLayersPerBoard_z ) 
		cout << "ERROR << TAITparGeo::InitGeo()  -->  m_passiveMaterial.size() != m_nPassiveLayersPerBoard_z\n", exit(0);
  
	// minimum z coordinate of the passive group
	
	int i = 0;
	// m_passiveMatrix.resize( 1 );
	// m_chipMatrix.resize( m_nSensors.X() );

	// TODO     Loop over alle the board  Y
	// int j = 0;
	// m_passiveMatrix[i].resize( m_nSensors.Y() );
	// m_chipMatrix[i].resize( m_nSensors.Y() );
	
	// double sensor_newY = m_origin.Y() - 1.5*boardDimension.y();
	double sensor_offset_y = m_origin.Y() - ((m_nSensors.Y()-1)/2.)* displacement_y;    // center of the bottom sensor in Y
	for (int j=0; j<m_nSensors.Y(); j++) {
		// sensor_newY += j * boardDimension.y();

		double offset_x = m_origin.x() + (j < 2 ? boardStagger_x : -boardStagger_x);
		
		double sensor_newY = sensor_offset_y + j * displacement_y;
		double chipOffset_y = sensor_newY + 0.5*senseDimension.y() +
											(j < 2 ? chipDead_Ymin : chipDead_Ymax ) -
											chipDimension.y()/2;  // distance of the chip center from the sensor center
		double offset_y = sensor_newY + 0.5*senseDimension.y() + 
										(j < 2 ? chipDead_Ymin : chipDead_Ymax ) +
										(j < 2 ? m_boardYDeadMin : m_boardYDeadMax) -	  
										boardDimension.y()/2 ;  // board center y

	

		// double offset_y = m_origin.y() + (j < 2 ? -1 : 1)*senseDimension.y() + (j%2 == 0 ? -1 : 1)*boardDimension.y()/2 ;  // board center y
		// double chipOffset_y = offset_y - boardDimension.y()/2 +    // lower part of the board
		//                 (j < 2 ? m_boardDeadMin : m_boardDeadMax) + chipDimension.y()/2;  // distance of the chip center from the board border

		double offset_z = m_origin.z() + ( (j%2 == 0 ? -1 : 1) * ( m_plumeDistace_Z/2 + board_z/2 ) );    // board begin
		// double offset_z = m_origin.z() + ( (j%2 == 0 ? -1 : 1) * ( m_plumeDistace_Z/2 + m_siliconSensorThick_Lz + board_z/2 ) );    // board begin 
		double board_center = m_origin.z() + ( (j%2 == 0 ? -1 : 1) * ( m_plumeDistace_Z/2 ) ); // board center in z

		// m_passiveMatrix[i][j].resize( m_passiveMaterial.size() );
		// m_chipMatrix[i][j].resize( m_nSensors.Z() );

		// loop over the board passive layers
		float position = 0;

		for ( unsigned int k=0; k<m_passiveMaterial.size(); k++ ) {

			string matID = m_passiveMaterial.at(k);
			position += (-1) * (j%2 == 0 ? -1 : 1) * m_materialThick[ matID ]/2;    // increase or decrease the z coordinate

			++m_passiveCount;
			stringstream ss_bodyPassiveName;      ss_bodyPassiveName << "itrp" << m_passiveCount;
			stringstream ss_regionPassiveName;      ss_regionPassiveName << "ITRP" << m_passiveCount;

			// init matrix of passive material for a board
			m_passiveMatrix[i][j][k]->SetBox( TVector3( offset_x, offset_y , offset_z + position ),          // position in the detector frame
													TVector3( boardDimension.x(), boardDimension.y(), m_materialThick[matID] ),   // dimension
													m_materialType[ matID ],            // name of the material
													matID,                              // name of the material-region in the foot_geo.h
													ss_bodyPassiveName.str(),                  // FLUKA body name
													ss_regionPassiveName.str(),          // FLUKA region name
													++m_volumeCount                       // volume ID num
													);
			if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "passive center ",    TVector3( offset_x, offset_y, offset_z + position ).Print();

			position += (-1) * (j%2 == 0 ? -1 : 1) * m_materialThick[ matID ]/2;

		}



//---------------------------------------------------------------------
//     Init passive CHIP materials 
//---------------------------------------------------------------------
		if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "Init CHIP passive materials geometry" << endl;

		double chipOffset_x = m_origin.X() - 1.5*ITR_M28_DIST - 1.5*chipDimension.x();   // first chip center
		for ( int f=0; f<m_nSensors.X(); f++ ) {

			double chip_x = chipOffset_x + f * ( ITR_M28_DIST + chipDimension.x() );

			for ( int k=0; k<m_nSensors.Z(); k++ ) {

				string matID = "ITR_MEDIUM";
				double boardSide = (k%2 == 0 ? -1 : 1) * (board_z/2 + m_siliconSensorThick_Lz/2);

				++m_passiveCount;
				stringstream ss_bodyPassiveName;      ss_bodyPassiveName << "itrp" << m_passiveCount;
				stringstream ss_regionPassiveName;      ss_regionPassiveName << "ITRP" << m_passiveCount;

				// init matrix of passive material for a board
				m_chipMatrix[f][j][k]->SetBox( TVector3( chip_x, chipOffset_y , board_center +  boardSide ),          // position
														TVector3( chipDimension.x(), chipDimension.y(), chipDimension.z() ),   // dimension
														m_materialType[ matID ],            // name of the material
														matID,                              // name of the material-region in the foot_geo.h
														ss_bodyPassiveName.str(),           // FLUKA body name
														ss_regionPassiveName.str(),         // FLUKA region name
														++m_volumeCount                       // volume ID num
														);
				if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "chip center ",    TVector3( chip_x, chipOffset_y , board_center +  boardSide ).Print();

			}
		}


	}

	// create the universe volume
	if ( GlobalPar::GetPar()->geoROOT() ) {
		m_universe = gGeoManager->MakeBox("ITuniverse",gGeoManager->GetMedium("AIR"),m_dimension.x()/2,m_dimension.y()/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
		gGeoManager->SetTopVisible(1);
	}

// //---------------------------------------------------------------------
// //     Build passive materials in ROOT and FLUKA
// //---------------------------------------------------------------------
if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "Build passive materials in ROOT and FLUKA" << endl;

	// for ( PassiveMatrix::iterator itX = m_passiveMatrix.begin(); itX != m_passiveMatrix.end(); itX++ ) {
	//     for ( PassivePlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
	//         for ( PassiveLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

	for ( unsigned int k=0; k<m_passiveMaterial.size(); k++ ) {
		for ( unsigned int j=0; j<m_nSensors.Y(); j++ ) {
			for ( unsigned int i=0; i<1; i++ ) {    

				//ROOT addNode
				if ( GlobalPar::GetPar()->geoROOT() ) {
					if ( !gGeoManager->GetVolume( m_passiveMatrix[i][j][k]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_passiveMatrix[i][j][k]->GetMaterialRegionName() << endl;

					TVector3 globalCoord = m_passiveMatrix[i][j][k]->GetPosition();
					Local2Global(&globalCoord);
					m_universe->AddNode( gGeoManager->GetVolume( m_passiveMatrix[i][j][k]->GetMaterialRegionName().c_str() ), 
										m_passiveMatrix[i][j][k]->GetNodeID() , 
										new TGeoCombiTrans( globalCoord.x(), globalCoord.y(), globalCoord.z(), 
										new TGeoRotation("null,",0,0,0) ) );

					if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "\t"<<m_passiveMatrix[i][j][k]->GetMaterialRegionName()<<"  "<<m_passiveMatrix[i][j][k]->GetRegionName()<< "  ", globalCoord.Print();
				}
				
				// boidies
				if ( GlobalPar::GetPar()->geoFLUKA() ) {

					TVector3 minCoord = TVector3( m_passiveMatrix[i][j][k]->GetMinCoord().x(), m_passiveMatrix[i][j][k]->GetMinCoord().y(), m_passiveMatrix[i][j][k]->GetMinCoord().z() );
					TVector3 maxCoord = TVector3( m_passiveMatrix[i][j][k]->GetMaxCoord().x(), m_passiveMatrix[i][j][k]->GetMaxCoord().y(), m_passiveMatrix[i][j][k]->GetMaxCoord().z() );
					Local2Global( &minCoord );
					Local2Global( &maxCoord );

					stringstream ss;    
					ss << setiosflags(ios::fixed) << setprecision(6);
					ss <<  "RPP " << m_passiveMatrix[i][j][k]->GetBodyName() <<  "     " 
								<< minCoord.x() << " " << maxCoord.x() << " "
								<< minCoord.y() << " " << maxCoord.y() << " "
								<< minCoord.z() << " " << maxCoord.z() << endl;
					
					m_bodyPrintOut  [ m_passiveMatrix[i][j][k]->GetMaterialName() ].push_back( ss.str() );
					m_bodyName      [ m_passiveMatrix[i][j][k]->GetMaterialName() ].push_back( m_passiveMatrix[i][j][k]->GetBodyName() );

					// regions
					stringstream ssr;    ssr << setw(13) << setfill( ' ' ) << std::left << m_passiveMatrix[i][j][k]->GetRegionName()
											<< "5 " << m_passiveMatrix[i][j][k]->GetBodyName() << endl;
					
					m_regionPrintOut[ m_passiveMatrix[i][j][k]->GetMaterialName() ].push_back( ssr.str() );
					m_regionName    [ m_passiveMatrix[i][j][k]->GetMaterialName() ].push_back( m_passiveMatrix[i][j][k]->GetRegionName() );
					if ( genfit::FieldManager::getInstance()->getFieldVal( TVector3( minCoord ) ).Mag() == 0 && genfit::FieldManager::getInstance()->getFieldVal( TVector3( maxCoord ) ).Mag() == 0 )
						m_magneticRegion[ m_passiveMatrix[i][j][k]->GetRegionName() ] = 0;
					else 
						m_magneticRegion[ m_passiveMatrix[i][j][k]->GetRegionName() ] = 1;
					
				}

			}
		}
	} 

	if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "Build chip materials in ROOT and FLUKA" << endl;
	// passive chip material
	// int sensor_i = 0;
	// for ( PassiveMatrix::iterator itX = m_chipMatrix.begin(); itX != m_chipMatrix.end(); itX++ ) {
	//     int sensor_j = 0;
	//     for ( PassivePlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
	//         int sensor_k = 0;
	//         for ( PassiveLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {

	for ( unsigned int k=0; k<m_nSensors.Z(); k++ ) {
		for ( unsigned int j=0; j<m_nSensors.Y(); j++ ) {
			for ( unsigned int i=0; i<m_nSensors.X(); i++ ) {   

				//ROOT addNode
				if ( GlobalPar::GetPar()->geoROOT() )    {
					if ( !gGeoManager->GetVolume( m_chipMatrix[i][j][k]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_chipMatrix[i][j][k]->GetMaterialRegionName() << endl;

					TVector3 globalCoord = m_chipMatrix[i][j][k]->GetPosition();
					Local2Global(&globalCoord);
					m_universe->AddNode( gGeoManager->GetVolume( m_chipMatrix[i][j][k]->GetMaterialRegionName().c_str() ), 
										m_chipMatrix[i][j][k]->GetNodeID() , 
										new TGeoCombiTrans( globalCoord.x(), globalCoord.y(), globalCoord.z(), 
										new TGeoRotation("null,",0,0,0) ) );

					if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "\t"<<m_chipMatrix[i][j][k]->GetMaterialRegionName()<<"  "<<m_chipMatrix[i][j][k]->GetRegionName() <<"  ", globalCoord.Print();
				}
				
				// boidies
				if ( GlobalPar::GetPar()->geoFLUKA() ) {

					TVector3 minCoord = TVector3( m_chipMatrix[i][j][k]->GetMinCoord().x(), m_chipMatrix[i][j][k]->GetMinCoord().y(), m_chipMatrix[i][j][k]->GetMinCoord().z() );
					TVector3 maxCoord = TVector3( m_chipMatrix[i][j][k]->GetMaxCoord().x(), m_chipMatrix[i][j][k]->GetMaxCoord().y(), m_chipMatrix[i][j][k]->GetMaxCoord().z() );
					Local2Global( &minCoord );
					Local2Global( &maxCoord );

					stringstream ss;    
					ss << setiosflags(ios::fixed) << setprecision(6);
					ss <<  "RPP " << m_chipMatrix[i][j][k]->GetBodyName() <<  "     " 
								<< minCoord.x() << " " << maxCoord.x() << " "
								<< minCoord.y() << " " << maxCoord.y() << " "
								<< minCoord.z() << " " << maxCoord.z() << endl;
					
					m_bodyPrintOut[ m_chipMatrix[i][j][k]->GetMaterialName() ].push_back( ss.str() );
					m_bodyName    [ m_chipMatrix[i][j][k]->GetMaterialName() ].push_back( m_chipMatrix[i][j][k]->GetBodyName() );

					// regions
					stringstream ssr;    ssr << setw(13) << setfill( ' ' ) << std::left << m_chipMatrix[i][j][k]->GetRegionName()
											<< "5 " << m_chipMatrix[i][j][k]->GetBodyName() << " - " << 
											m_sensorMatrix[k][j][i]->GetBodyName() << endl;   
					
					m_regionPrintOut[ m_chipMatrix[i][j][k]->GetMaterialName() ].push_back( ssr.str() );
					m_regionName    [ m_chipMatrix[i][j][k]->GetMaterialName() ].push_back( m_chipMatrix[i][j][k]->GetRegionName() );
					
					if ( genfit::FieldManager::getInstance()->getFieldVal( TVector3( minCoord ) ).Mag() == 0 && genfit::FieldManager::getInstance()->getFieldVal( TVector3( maxCoord ) ).Mag() == 0 )
						m_magneticRegion[ m_chipMatrix[i][j][k]->GetRegionName() ] = 0;
					else 
						m_magneticRegion[ m_chipMatrix[i][j][k]->GetRegionName() ] = 1;

				}
				// sensor_k++;
			}
			// sensor_j++;
		}
		// sensor_i++;
	} 


//---------------------------------------------------------------------
//     Build sensor materials in ROOT and FLUKA
//---------------------------------------------------------------------
if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "Build sensor materials in ROOT and FLUKA" << endl;

	for ( unsigned int k=0; k<m_nSensors.Z(); k++ ) {
		vector<double> xmin, ymin;
		for ( unsigned int j=0; j<m_nSensors.Y(); j++ ) {
			for ( unsigned int i=0; i<m_nSensors.X(); i++ ) {    

				//ROOT addNode
				if ( GlobalPar::GetPar()->geoROOT() )   {
					// m_sensorMatrix[k][j][i]->AddNodeToUniverse( m_universe );
					if ( !gGeoManager->GetVolume( m_sensorMatrix[k][j][i]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_sensorMatrix[k][j][i]->GetMaterialRegionName() << endl;

					TVector3 globalCoord = m_sensorMatrix[k][j][i]->GetCenter();
					Local2Global(&globalCoord);
					m_universe->AddNode( gGeoManager->GetVolume( m_sensorMatrix[k][j][i]->GetMaterialRegionName().c_str() ), 
										m_sensorMatrix[k][j][i]->GetNodeID() , 
										new TGeoCombiTrans( globalCoord.x(), globalCoord.y(), globalCoord.z(), 
										new TGeoRotation("null,",0,0,0) ) );
					if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "\t"<<m_sensorMatrix[k][j][i]->GetMaterialRegionName()<<"  "<<m_sensorMatrix[k][j][i]->GetRegionName()<<" ", globalCoord.Print();
				}

				// boidies
				if ( GlobalPar::GetPar()->geoFLUKA() ) {

					TVector3 minCoord = TVector3( m_sensorMatrix[k][j][i]->GetMinCoord().x(), m_sensorMatrix[k][j][i]->GetMinCoord().y(), m_sensorMatrix[k][j][i]->GetMinCoord().z() );
					TVector3 maxCoord = TVector3( m_sensorMatrix[k][j][i]->GetMaxCoord().x(), m_sensorMatrix[k][j][i]->GetMaxCoord().y(), m_sensorMatrix[k][j][i]->GetMaxCoord().z() );
					Local2Global( &minCoord );
					Local2Global( &maxCoord );

					if ( i==0 ) {
					  xmin.push_back(minCoord.X());
			  if (fabs(minCoord.Y())<1.e-15)  {
			ymin.push_back(0.);// invece he ymin=0 mi viene =1.11022e-16D+00 -> nel parameters.in mi allunga la riga e va oltre il limite di caratteri in fortran
			  }
				  else ymin.push_back(minCoord.Y());
				}

					stringstream ss;
					ss << setiosflags(ios::fixed) << setprecision(6);
					ss <<  "RPP " << m_sensorMatrix[k][j][i]->GetBodyName() <<  "     " 
								<< minCoord.x() << " " << maxCoord.x() << " "
								<< minCoord.y() << " " << maxCoord.y() << " "
								<< minCoord.z() << " " << maxCoord.z() << endl;

					m_bodyPrintOut  [ m_sensorMatrix[k][j][i]->GetMaterialName() ].push_back( ss.str() );
					// m_bodyName      [ m_sensorMatrix[k][j][i]->GetMaterialName() ].push_back( m_sensorMatrix[k][j][i]->GetBodyName() );

					// regions
					stringstream ssr;
					ssr << setw(13) << setfill( ' ' ) << std::left << m_sensorMatrix[k][j][i]->GetRegionName()
						<< "5 " << m_sensorMatrix[k][j][i]->GetBodyName() << endl;

					m_regionPrintOut[ m_sensorMatrix[k][j][i]->GetMaterialName() ].push_back( ssr.str() );
					m_regionName    [ m_sensorMatrix[k][j][i]->GetMaterialName() ].push_back( m_sensorMatrix[k][j][i]->GetRegionName() );
					
					if ( genfit::FieldManager::getInstance()->getFieldVal( TVector3( minCoord ) ).Mag() == 0 && genfit::FieldManager::getInstance()->getFieldVal( TVector3( maxCoord ) ).Mag() == 0 )
						m_magneticRegion[ m_sensorMatrix[k][j][i]->GetRegionName() ] = 0;
					else 
						m_magneticRegion[ m_sensorMatrix[k][j][i]->GetRegionName() ] = 1;
					
				}

			}
	   }
	
	m_xmin.push_back(xmin);
	m_ymin.push_back(ymin);
	
	} 

}






//______________________________________________________________________________________________________________
TVector3 TAITparGeo::GetPixelPos_sensorFrame( int layer, int plume, int chip, int col, int row )  { 
	return m_sensorMatrix[layer][plume][chip]->GetPosition_local( col, row );  
}
//______________________________________________________________________________________________________________
TVector3 TAITparGeo::GetPixelPos_sensorFrame( int sensorID, int col, int row ) { 
    return GetPixelPos_sensorFrame( GetLayerFromSensorID(sensorID), 
                                    GetPlumeFromSensorID(sensorID),
                                    GetChipFromSensorID(sensorID), 
                                    col, row );
}


//______________________________________________________________________________________________________________
TVector3 TAITparGeo::GetPixelPos_detectorFrame( int layer, int plume, int chip, int col, int row )  { 
	return m_sensorMatrix[layer][plume][chip]->GetPosition( col, row );  
}
//______________________________________________________________________________________________________________
TVector3 TAITparGeo::GetPixelPos_detectorFrame( int sensorID, int col, int row ){ 
    return GetPixelPos_detectorFrame(   GetLayerFromSensorID(sensorID), 
                                        GetPlumeFromSensorID(sensorID),
                                        GetChipFromSensorID(sensorID), 
                                        col, row );
}


//______________________________________________________________________________________________________________
TVector3 TAITparGeo::GetPixelPos_footFrame( int layer, int plume, int chip, int col, int row )  {

	TVector3 pos = GetPixelPos_detectorFrame( layer, plume, chip, col, row );
	Local2Global(&pos);
	return pos;
}
//______________________________________________________________________________________________________________
TVector3 TAITparGeo::GetPixelPos_footFrame( int sensorID, int col, int row ){ 
    return GetPixelPos_footFrame(   GetLayerFromSensorID(sensorID), 
                                    GetPlumeFromSensorID(sensorID),
                                    GetChipFromSensorID(sensorID), 
                                    col, row );
}



//______________________________________________________________________________________________________________
TVector3 TAITparGeo::GetSensorPosition( int sensorID ){ 
    return m_sensorMatrix[GetLayerFromSensorID(sensorID)][GetPlumeFromSensorID(sensorID)][GetChipFromSensorID(sensorID)]->GetCenter();                                   
}



//_____________________________________________________________________________
float TAITparGeo::GetColumnCenter_sensorFrame( int col )  { return GetPixelPos_sensorFrame( 0, col, 0 ).x(); }
float TAITparGeo::GetColumnCenter_detectorFrame( int layer, int plume, int chip, int col )  { return GetPixelPos_detectorFrame( layer, plume, chip, col, 0 ).x(); }
float TAITparGeo::GetColumnCenter_footFrame( int layer, int plume, int chip, int col ) { return GetPixelPos_footFrame( layer, plume, chip, col, 0 ).x(); }
//_____________________________________________________________________________
float TAITparGeo::GetRowCenter_sensorFrame( int row )     { return GetPixelPos_sensorFrame( 0, 0, row ).y();   }
float TAITparGeo::GetRowCenter_detectorFrame( int layer, int plume, int chip, int row )     { return GetPixelPos_detectorFrame( layer, plume, chip, 0, row ).y();   }
float TAITparGeo::GetRowCenter_footFrame( int layer, int plume, int chip, int row )    { return GetPixelPos_footFrame( layer, plume, chip, 0, row ).y();     }


//_____________________________________________________________________________
float TAITparGeo::GetColumnCenter_detectorFrame( int sensorID, int col )  { 
	return GetColumnCenter_detectorFrame( 	GetLayerFromSensorID(sensorID), 
											GetPlumeFromSensorID(sensorID),
											GetChipFromSensorID(sensorID), col ); 
}
//_____________________________________________________________________________
float TAITparGeo::GetColumnCenter_footFrame( int sensorID, int col ) { 
	return GetColumnCenter_footFrame( 	GetLayerFromSensorID(sensorID), 
										GetPlumeFromSensorID(sensorID),
										GetChipFromSensorID(sensorID), col ); 
}
//_____________________________________________________________________________
float TAITparGeo::GetRowCenter_detectorFrame( int sensorID, int row )     { 
	return GetRowCenter_detectorFrame(  GetLayerFromSensorID(sensorID), 
										GetPlumeFromSensorID(sensorID),
										GetChipFromSensorID(sensorID), row );   
}
//_____________________________________________________________________________
float TAITparGeo::GetRowCenter_footFrame( int sensorID, int row )    { 
	return GetRowCenter_footFrame(  GetLayerFromSensorID(sensorID), 
									GetPlumeFromSensorID(sensorID),
									GetChipFromSensorID(sensorID), row );     
}


//_____________________________________________________________________________
void TAITparGeo::Detector2Sensor_frame( int sensorID, TVector3* coord ) {
	m_sensorMatrix[GetLayerFromSensorID(sensorID)][GetPlumeFromSensorID(sensorID)][GetChipFromSensorID(sensorID)]->Global2Local( coord );
}

//_____________________________________________________________________________
void TAITparGeo::Sensor2Detector_frame( int sensorID, TVector3* coord ) {
	m_sensorMatrix[GetLayerFromSensorID(sensorID)][GetPlumeFromSensorID(sensorID)][GetChipFromSensorID(sensorID)]->Local2Global( coord );
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
string TAITparGeo::PrintBodies(){

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAITparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
	
	stringstream outstr;
	outstr << "* ***Inner Tracker" << endl;

	// loop in order of the material alfabeth
	for ( map<string, vector<string> >::iterator itMat = m_bodyPrintOut.begin(); itMat != m_bodyPrintOut.end(); itMat++ ) {
		// loop over all body of the same material
		for ( vector<string>::iterator itBody = (*itMat).second.begin(); itBody != (*itMat).second.end(); itBody++ ) {
			outstr << (*itBody);
			if (m_debug > 3)    cout << (*itBody);
		}        
	}

	return outstr.str();
}





//_____________________________________________________________________________
string TAITparGeo::PrintRegions( ){

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAITparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

	stringstream outstr;
	outstr << "* ***Inner Tracker" << endl;
 
	// loop in order of the material alfabeth
	for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
		// loop over all region of the same material
		for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
			outstr << (*itRegion);
			if (m_debug > 3)    cout << (*itRegion);
		}        
	}
	return outstr.str();
}




//_____________________________________________________________________________
string TAITparGeo::PrintSubtractBodiesFromAir() {

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAITparGeo::PrintSubtractMaterialFromAir()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

	stringstream outstr;
	int count=0;
	// loop in order of the material alfabeth
	for ( map<string, vector<string> >::iterator itMat = m_bodyName.begin(); itMat != m_bodyName.end(); itMat++ ) {
		// loop over all bodies of the same material
		for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
		if ( count % 10 == 0  )
		  outstr << "\n              ";
		count++;
			outstr << " -" << (*itRegion);
		}        
	}
	return outstr.str();

}





//_____________________________________________________________________________
string TAITparGeo::PrintAssignMaterial() {

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAITparGeo::PrintAssignMaterial()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


	// loop in order of the material alfabeth
	stringstream outstr; 
	for ( map<string, vector<string> >::iterator itMat = m_regionName.begin(); itMat != m_regionName.end(); itMat++ ) {

		// check dimension greater than 0
		if ( (*itMat).second.size() == 0 ) {
			cout << "ERROR << TAITparGeo::PrintAssignMaterial  ::  "<<endl, exit(0);
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
		if (m_debug > 0)    cout << outstr.str();

	}

	return outstr.str();

}




//_____________________________________________________________________________
string TAITparGeo::PrintParameters() {
  
  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);
  string precision = "D+00";

  outstr << "c     INTERMEDIATE TRACKER PARAMETERS " << endl;
  outstr << endl;    
  
  map<string, int> intp;
  intp["xpixITR"] = m_nPixel_X;
  intp["ypixITR"] = m_nPixel_Y;
  intp["nlayITR"] = m_nSensors.Z();
  intp["nplumeITR"] = m_nSensors.Y();
  intp["nmimoITR"] = m_nSensors.X();
  for (auto i : intp){
	outstr << "      integer " << i.first << endl;
	outstr << "      parameter (" << i.first << " = " << i.second << ")" << endl;
  }
  
  map<string, double> doublep;
  doublep["dxITR"] = ITR_DX;
  doublep["dyITR"] = ITR_DY;
  doublep["widthITR"] =  ITR_SENSE_WIDTH;
  doublep["heightITR"] =  ITR_SENSE_HEIGHT;
  doublep["deadITR"] = ITR_M28_WIDTH - ITR_SENSE_WIDTH + ITR_M28_DIST;
  for (auto i : doublep){
	outstr << "      double precision " << i.first << endl;
	outstr << "      parameter (" << i.first << " = " << i.second << precision << ")" << endl;
  }
  
  map<string, vector<double>> dvectorp;
  string name;
  for ( int j = 0; j<m_xmin.size(); j++ ){
	name = "xminITR" + to_string(j);
	dvectorp[name] = m_xmin[j];
	name = "yminITR" + to_string(j);
	dvectorp[name] = m_ymin[j];
  }
  for (auto i : dvectorp){
	outstr << "      double precision " << i.first << "(" << dvectorp.size() << ")" << endl;
	outstr << "      data " << i.first << "/";
	for ( double d : i.second ){
	  outstr << d << precision << ",";
	}
	outstr.seekp(-1, std::ios_base::end);
	outstr << "/" << endl;
 
  }
  
  outstr << endl;
  // cout<<outstr.str().length()<<endl;
  
  return outstr.str();

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
