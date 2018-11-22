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

// #include "TAMSDparMap.hxx"
#include "TAMSDparGeo.hxx"

// #include "foot_geo.h"
// #include "GlobalPar.hxx"




//_____________________________________________________________________________
TAMSDparGeo::TAMSDparGeo() {

	m_volumeCount = -1;
	m_passiveCount = -1;
	m_setW_0number = 2;

	m_debug = GlobalPar::GetPar()->Debug();

	// fill m_materialOrder, m_materialThick, m_materialType
	InitMaterial();

};



//_____________________________________________________________________________
TAMSDparGeo::~TAMSDparGeo() {
	// sensor matrix
	for ( SensorMatrix::iterator itX = m_sensorMatrix.begin(); itX != m_sensorMatrix.end(); itX++ ) {
		for ( SensorPlane::iterator itY = (*itX).begin(); itY != (*itX).end(); itY++ ) {
			for ( SensorLine::iterator itZ = (*itY).begin(); itZ != (*itY).end(); itZ++ ) {
				delete (*itZ);
			}
			(*itY).clear();
		}
		(*itX).clear();
	}
	m_sensorMatrix.clear();
}



//_____________________________________________________________________________
//  copy constructor
TAMSDparGeo::TAMSDparGeo( TAMSDparGeo* original ) :

	m_rotation(original->m_rotation), 
	m_origin(original->m_origin),  // current position
	m_center(original->m_center),  // current position
	m_dimension(original->m_dimension),

	m_nSensors (original->m_nSensors),

	m_materialOrder(original->m_materialOrder),

	m_materialThick(original->m_materialThick),
	m_materialType(original->m_materialType),

	m_layerDistance(original->m_layerDistance)

	    {

	SensorMatrix m_sensorMatrix = original->m_sensorMatrix;
}





//_____________________________________________________________________________
void TAMSDparGeo::InitMaterial() {

	m_materialOrder = {  "MSD_MEDIUM"
						 };

	
	for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
		if( m_materialOrder[i] == "MSD_MEDIUM" ){
			m_materialThick[ m_materialOrder[i] ] = MSD_THICK;
			m_materialType[ m_materialOrder[i] ] = MSD_MEDIUM;
		}
		
	}

}





//_____________________________________________________________________________
void TAMSDparGeo::InitGeo()  {

	if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTAMSDparGeo::InitGeo" << endl<< endl;

	m_origin = TVector3( 0, 0, 0 );
	m_center = TVector3( MSD_X, MSD_Y, MSD_Z );

	m_nSensors = TVector3( 1, 1, MSD_NLAY*2 );     // 1 layers includes x and y bars

	// init sensor matrix   (z, x, y) = (sensor in z = layerX2, sensor in x, sensor in y)
	m_sensorMatrix.resize( m_nSensors.Z() );
	for (int k=0; k<m_nSensors.Z(); k++) {
		m_sensorMatrix[k].resize( m_nSensors.X() );
		for (int i=0; i<m_nSensors.X(); i++) {
			m_sensorMatrix[k][i].resize( m_nSensors.Y() );
			for (int j=0; j<m_nSensors.Y(); j++) {
                // m_sensorMatrix[k][i][j] = new LightSabre();
				m_sensorMatrix[k][i][j] = new IronPlate();
			}
		}
	}
	
//---------------------------------------------------------------------
//     Find DETECTOR dimension
//---------------------------------------------------------------------

	m_layerDistance = MSD_LAYDIST;      // from center to center
	m_sensorDimension = TVector3( MSD_WIDTH, MSD_HEIGHT, MSD_THICK );       // layer dimaension 
    m_layer_EdgeToEdgeDistance = m_layerDistance - 2*m_sensorDimension.z(); 
	m_dimension = TVector3( MSD_WIDTH, MSD_HEIGHT, (2*m_sensorDimension.z() + (m_nSensors.Z()-1)*m_layerDistance) );   // from edge to edge              
	m_nView = MSD_NVIEW;


   
	m_nLayer = m_nSensors.Z();
	if ( MSD_XSTRIP != MSD_YSTRIP )   cout << "ERROR << number of strip in x "<<MSD_XSTRIP<<" and y "<<MSD_YSTRIP<<" are different form foot_geo.h. Shouldn't be!!!"<< endl, exit(0);
	m_nStrip = MSD_XSTRIP;

	// set detector dimension
	// double width_Lx = m_dimension.x();
	// double height_Ly = m_dimension.y();
	

//---------------------------------------------------------------------
//     Init SENSOR geometry
//---------------------------------------------------------------------

	// double pixelDistance = 0;
	double stripDistance = 0;

	if ( MSD_DX != MSD_DY )   cout << "ERROR << width of strip in x "<<MSD_DX<<" and y "<<MSD_DY<<" are different form foot_geo.h. Shouldn't be!!!"<< endl, exit(0);
	if ( MSD_WIDTH != MSD_HEIGHT )   cout << "ERROR << MSD_WIDTH "<<MSD_WIDTH<<" and MSD_HEIGHT "<<MSD_HEIGHT<<" are different form foot_geo.h. Shouldn't be!!!"<< endl, exit(0);
    m_dimensionStrip = TVector3( MSD_DX, MSD_WIDTH, MSD_THICK );	// later thickness

	// double pixelWidth_Lx = MSD_DX;
	// double pixelHeight_Ly = MSD_DY;

	// // total pixels
	// m_nPixel_X = width_Lx / (pixelWidth_Lx + pixelDistance);
	// m_nPixel_Y = height_Ly / (pixelHeight_Ly + pixelDistance);

	// pixels per sensors, same as above as far as we use 1 sensor
	// m_nPixel_X = m_sensorDimension.x() / (pixelWidth_Lx + pixelDistance);
	// m_nPixel_Y = m_sensorDimension.y() / (pixelHeight_Ly + pixelDistance);

	// fill sensor matrix
	for (int k=0; k<m_nSensors.Z(); k++) {
        // double sensor_newZ = m_origin.Z() - m_dimension.z()/2 + m_sensorDimension.z()/2 + k*m_layerDistance;
		double sensor_newZ = m_origin.Z() - m_dimension.z()/2 + (0.5+k)*m_sensorDimension.z() + trunc(k/2)*m_layer_EdgeToEdgeDistance; // layer distance is among xy plan, so only every 2 strip-layers

		for (int i=0; i<m_nSensors.X(); i++) {	// only 1
			double sensor_newX = m_origin.X() - m_dimension.x()/2 + (0.5+i)*(m_sensorDimension.x());		
			
			for (int j=0; j<m_nSensors.Y(); j++) {	// only 1
				double sensor_newY = m_origin.Y() - m_dimension.y()/2 + (0.5+j)*(m_sensorDimension.y());

				m_volumeCount++;
				stringstream ss_bodySensorName; ss_bodySensorName << "msds" << m_volumeCount;
				stringstream ss_regionSensorName; ss_regionSensorName << "MSDS" << m_volumeCount;

				m_sensorMatrix[k][i][j]->SetMaterial( m_materialType[ "MSD_MEDIUM" ], "MSD_MEDIUM", ss_bodySensorName.str(), ss_regionSensorName.str(), m_volumeCount );

				m_sensorMatrix[k][i][j]->SetSensor( 
						TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
						TVector3( m_sensorDimension.x(), m_sensorDimension.y(), m_sensorDimension.z() ),    // sensor dimension
						(k%2 == 0 ? m_nStrip : 1) , (k%2 == 0 ? 1 : m_nStrip),
						// m_dimensionStrip.x(), m_dimensionStrip.x(), m_dimensionStrip.z(),
						( k%2 == 0 ? m_sensorDimension.x() : m_dimensionStrip.x() ), ( k%2 == 0 ? m_dimensionStrip.y() : m_sensorDimension.y() ), m_dimensionStrip.z(),
						stripDistance, stripDistance, 0, //layerDistance,
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

	for ( unsigned int k=0; k<m_nSensors.Z(); k++ ) {
		for ( unsigned int j=0; j<m_nSensors.Y(); j++ ) {
			for ( unsigned int i=0; i<m_nSensors.X(); i++ ) {    

				
				//ROOT addNode
				if ( GlobalPar::GetPar()->geoROOT() )   {
					if ( !gGeoManager->GetVolume( m_sensorMatrix[k][i][j]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_sensorMatrix[k][i][j]->GetMaterialRegionName() << endl;

					TVector3 globalCoord = m_sensorMatrix[k][i][j]->GetCenter();
					// Local2Global(&globalCoord);
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





// //_____________________________________________________________________________
// TVector3 TAMSDparGeo::GetPosition( int layer, int view, int strip )  {
// 	// TVector3 sensorCoord = GetSensorCoortdinates( int layer, int col, int row );
// 	// TVector3 pos = m_sensorMatrix[sensorCoord.z()][sensorCoord.x()][sensorCoord.y()]->GetPosition();

//     // view = 0 -> column (x strip)        view = 1 -> row (y strip)

//     int xStrip, yStrip;
//     if ( view == 0 )
//         xStrip = strip, yStrip = 1;
//     else 
//         xStrip = 1, yStrip = strip;

// 	TVector3 pos = m_sensorMatrix[ SensorZ_From_LayerView(layer, view) ][0][0]->GetPosition( xStrip, yStrip );

// 	// set the z coordinate as one of the sensor surface, depending on the view
// 	// if ( view == 0 )           pos.SetZ( pos.z() - 0.5*m_siliconSensorThick_Lz );
// 	// else if ( view ==  1 )      pos.SetZ( pos.z() + 0.5*m_siliconSensorThick_Lz );
// 	// else                        cout << "ERROR :: TAMSDparGeo::GetPosition  -->  wrong value for strip view = " << view << endl, exit(0);

// 	Local2Global(&pos);
// 	return pos;
// }




// //_____________________________________________________________________________
// TVector3 TAMSDparGeo::GetSensorCenter( int layer, int view ) {

//     TVector3 pos = m_sensorMatrix[ SensorZ_From_LayerView(layer, view) ][0][0]->GetPosition( xStrip, yStrip );
    
//     Local2Global(&pos);
//     return pos;

// }




//_____________________________________________________________________________
TVector3 TAMSDparGeo::GetLayerCenter( int layer ) {

	TVector3 pos = m_sensorMatrix[ layer*2 ][0][0]->GetCenter();
	
	Local2Global(&pos);
	return pos;

}





//_____________________________________________________________________________
void TAMSDparGeo::Global2Local( TVector3* glob ) {
	glob->Transform( GetRotationToLocal() );
	*glob = *glob - m_center;
} 

//_____________________________________________________________________________
void TAMSDparGeo::Global2Local_RotationOnly( TVector3* glob ) {
	glob->Transform( GetRotationToLocal() );
} 

//_____________________________________________________________________________
void TAMSDparGeo::Global2Local_TranslationOnly( TVector3* glob ) {
  *glob = *glob - m_center;
}

//_____________________________________________________________________________
void TAMSDparGeo::Local2Global( TVector3* loc ) {
	loc->Transform( GetRotationToGlobal() );
	*loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAMSDparGeo::Local2Global_RotationOnly( TVector3* loc ) {
	loc->Transform( GetRotationToGlobal() );
} 

//_____________________________________________________________________________
void TAMSDparGeo::Local2Global_TranslationOnly( TVector3* loc ) {
  *loc = *loc + m_center;
}




//_____________________________________________________________________________
TGeoVolume* TAMSDparGeo::GetVolume() {

	if ( !GlobalPar::GetPar()->geoROOT() ) 
		cout << "ERROR << TAMSDparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

	return m_universe;

}





//_____________________________________________________________________________
string TAMSDparGeo::PrintBodies( ){

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAMSDparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
	

	stringstream outstr;
	outstr << "* ***Micro Strip Detector" << endl;

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
string TAMSDparGeo::PrintRegions(){

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAMSDparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

	stringstream outstr; 
	outstr << "* ***Micro Strip Detector" << endl;

	// loop in order of the material alfabeth
	for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
		// loop over all body of the same material
		for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
			outstr << (*itRegion);
			if (m_debug > 3)    cout << (*itRegion);
		}        
	}
	return outstr.str();
}





//_____________________________________________________________________________
string TAMSDparGeo::PrintSubtractBodiesFromAir() {

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAMSDparGeo::PrintSubtractMaterialFromAir()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


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
string TAMSDparGeo::PrintAssignMaterial() {

	if ( !GlobalPar::GetPar()->geoFLUKA() ) 
		cout << "ERROR << TAMSDparGeo::PrintAssignMaterial()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


	// loop in order of the material alfabeth
	stringstream outstr; 
	for ( map<string, vector<string> >::iterator itMat = m_regionName.begin(); itMat != m_regionName.end(); itMat++ ) {

		// check dimension greater than 0
		if ( (*itMat).second.size() == 0 ) {
			cout << "ERROR << TAMSDparGeo::PrintAssignMaterial  ::  "<<endl, exit(0);
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
string TAMSDparGeo::PrintParameters() {
  
  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);

  string precision = "D+00";

  outstr << "c     MICRO STRIP DETECTOR PARAMETERS " << endl;
  outstr << endl;    
  
  map<string, int> intp;
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







//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TAMSDparGeo::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAMSDparGeo::ToStream(ostream& os, Option_t*) const
{
//  os << "TAMSDparGeo " << GetName() << endl;
//  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
//     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}

