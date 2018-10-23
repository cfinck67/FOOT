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

// #include "TATWparMap.hxx"
#include "TATWparGeo.hxx"

#include "foot_geo.h"
// #include "GlobalPar.hxx"


      TString TATWparGeo::fgkDefParaName = "twGeo";
      Int_t   TATWparGeo::fgkLayerOffset = 100;
const Color_t TATWparGeo::fgkDefaultModCol   = kGray+1;
const Color_t TATWparGeo::fgkDefaultModColOn = kRed-8;


//_____________________________________________________________________________
TATWparGeo::TATWparGeo()
{

  m_volumeCount = -1;
  InitMaterial();

};


//_____________________________________________________________________________
void TATWparGeo::InitMaterial() {

  if ( fDebugLevel> 1 ) {
	cout << endl << "SCN List of Materials\n ";
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

   DefineMaterial();
}

//_____________________________________________________________________________
void TATWparGeo::DefineMaterial()
{
   
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoElementTable* table = gGeoManager->GetElementTable();

   // create material
   TGeoMixture* mat = 0x0;;
   TGeoMedium*  med = 0x0;

   // EJ-212 Scintillator material from eljen technology
   const Char_t* matName = SCN_MEDIUM.Data();
   if ( (mat = (TGeoMixture *)gGeoManager->GetListOfMaterials()->FindObject(matName)) == 0x0 ) {
   
      TGeoElement* matC = table->GetElement(6);
      TGeoElement* matH = table->GetElement(1);
   
      mat =new TGeoMixture(matName,2, 1.023);
      mat->AddElement(matC, 9);
      mat->AddElement(matH, 10);
   }
   if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName)) == 0x0 )
      med = new TGeoMedium(matName,1,mat);
}

//_____________________________________________________________________________
TGeoVolume* TATWparGeo::BuildTofWall(const char *wallName)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager(TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoVolume* wall = gGeoManager->FindVolumeFast(wallName);
   if ( wall == 0x0 ) {
      const Char_t* matName = SCN_MEDIUM.Data();
      TGeoMixture* mat = (TGeoMixture *)gGeoManager->GetListOfMaterials()->FindObject(matName);
      TGeoMedium*  med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
      
      wall = gGeoManager->MakeBox(wallName, med,  m_dimension[1]/2.,  m_dimension[1]/2., GetSingleBarThickness()*2);
   }
   
   for (Int_t i = 0; i < SCN_NLAY; ++i) {
      
      TGeoVolume* wallXY = BuildTofWallXY(Form("%s%d", wallName, i+1), i);
      
      Float_t zPos = -GetSingleBarThickness()/2. + i*GetSingleBarThickness();
      TGeoTranslation trans(0, 0, zPos);
      TGeoRotation rot;
      rot.SetAngles(0, 0, 90*i);
      TGeoHMatrix  transfo = rot*trans;
      TGeoHMatrix* hm = new TGeoHMatrix(transfo);
      
      wall->AddNode(wallXY, i, hm);
   }
   
   return wall;
}


//_____________________________________________________________________________
TGeoVolume* TATWparGeo::BuildTofWallXY(const char *wallName, Int_t iLayer)
{
   
   TGeoVolume* wall = gGeoManager->FindVolumeFast(wallName);
   
   if ( wall == 0x0 ) {
      const Char_t* matName = SCN_MEDIUM.Data();
      TGeoMixture* mat = (TGeoMixture *)gGeoManager->GetListOfMaterials()->FindObject(matName);
      TGeoMedium*  med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
      wall = gGeoManager->MakeBox(wallName, med,  m_dimension[1]/2.,  m_dimension[1]/2., GetSingleBarThickness()/2.);
   }
   
   for (Int_t i = 0; i < SCN_NBAR; ++i) {
      Float_t xPos = m_dimensionBar[0]/2. + i*m_dimensionBar[0] - m_dimension[1]/2.;
      
      TGeoTranslation trans(xPos, 0, 0);
      TGeoHMatrix  transfo = trans;
      TGeoHMatrix* hm = new TGeoHMatrix(transfo);
      
      TGeoVolume* module = BuildModule(i, iLayer);
      
      module->SetLineColor(fgkDefaultModCol);
      module->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
      wall->AddNode(module, i, hm);
   }
   
   return wall;
}

/*------------------------------------------+---------------------------------*/
//! build module

TGeoVolume* TATWparGeo::BuildModule(Int_t iMod, Int_t iLayer)
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   const char* moduleName = Form("Module_%d_%d", iMod, iLayer);
   TGeoVolume* module     = gGeoManager->FindVolumeFast(moduleName);
   if ( module == 0x0 ) {
      const Char_t* matName = SCN_MEDIUM.Data();
      TGeoMixture* mat = (TGeoMixture *)gGeoManager->GetListOfMaterials()->FindObject(matName);
      TGeoMedium*  med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName);
      module = gGeoManager->MakeBox(moduleName, med,  m_dimensionBar[0]/2., m_dimensionBar[1]/2., GetSingleBarThickness()/2.);
   }
   
   module->SetLineColor(fgkDefaultModCol);
   module->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return module;
}

//_____________________________________________________________________________
void TATWparGeo::InitGeo()  {

	if ( fDebugLevel> 0 )     cout << "\n\nTATWparGeo::InitGeo" << endl<< endl;

	m_origin = TVector3(0,0,0);                         // center in local coord.
	m_center = TVector3(SCN_X, SCN_Y, SCN_Z);           // center in global coord.

	//prima lungo y poi lungo x
	m_nBar = SCN_BAR_HEIGHT/SCN_BAR_WIDTH;
	m_nLayer = SCN_NLAY;
	m_NBar = TVector2(m_nBar, m_nLayer);

	// init sensor matrix (layers, bars)
	m_barMatrix.resize( m_nLayer );
	for (int i=0; i<m_nLayer; i++) {
		m_barMatrix[i].resize( m_nBar );
		for (int j=0; j<m_nBar; j++) {
			m_barMatrix[i][j] = new LightSabre();
		}
	}

	//---------------------------------------------------------------------
	//     Find DETECTOR dimension
	//---------------------------------------------------------------------

	m_layerDistance = 0.;      // from center to center
	
	//V13 m_dimension = TVector3( SCN_WIDTH, SCN_HEIGHT, length_Lz );
	m_dimensionBar = TVector3( SCN_BAR_WIDTH, SCN_BAR_HEIGHT, SCN_BAR_THICK );;   // ( shorter dim (2 cm), longer dim (44 cm), z dim )
	
	// set detector dimension
	double length_Lz = m_dimensionBar.z()*2. + m_layerDistance; // from edge to edge
	m_dimension = TVector3( m_dimensionBar.y(), m_dimensionBar.y(), length_Lz );


	//---------------------------------------------------------------------
	//     Init BAR geometry
	//---------------------------------------------------------------------
	if ( fDebugLevel> 0 ) cout << " Init SCINT BAR geometry " << endl;
	double barDistance = 0;
	double bar_newX=-1000., bar_newY=-1000., bar_newZ=-1000.;

	// fill sensor matrix
	double coord_x = m_origin.X() - m_dimension.x()/2.; 	// x coordinate of the bar center in layer 0
	double coord_y = m_origin.y() - m_dimension.y()/2.;		// y coordinate of the bar center in layer 1
	for (int k=0; k<m_nLayer; k++) {
		bar_newZ = m_origin.Z() - m_dimension.z()/2. + (k+0.5)*m_dimensionBar.z() + k*m_layerDistance;
		
		//prima lungo y poi lungo x
		// for (int i=0; i<m_nBar; i++) {
		for (int j=0; j<m_nBar; j++) {
		  bar_newX = ( k == 0 ? ( coord_x + (0.5+j)*m_dimensionBar.x() ) : m_origin.x() );
		  
		  bar_newY = ( k == 1 ? ( coord_y + (0.5+j)*m_dimensionBar.x() ) : m_origin.y() );

		  stringstream ss_bodyBarName; ss_bodyBarName << "scn" << k << setw(2) << setfill('0') << j;
		  stringstream ss_regionBarName; ss_regionBarName << "SCN" << k << setw(2) << setfill('0') << j;

		  m_volumeCount++;

		  m_barMatrix[k][j]->SetMaterial( (string)SCN_MEDIUM, "SCN_MEDIUM", ss_bodyBarName.str(), ss_regionBarName.str(), m_volumeCount );

		  m_barMatrix[k][j]->SetBar( TVector3( bar_newX, bar_newY, bar_newZ),  // bar center
				 m_dimensionBar,    // bar dimension
				 barDistance, barDistance, m_layerDistance,
				 TVector3( (k*TMath::Pi()*0.5),0,0 )
				 );
  
	  if ( fDebugLevel> 0 ) cout << "bar center ",    TVector3( bar_newX, bar_newY, bar_newZ ).Print();
	  
	}
  }
  // }

  m_rotation = new TRotation();
  // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
  m_rotation->SetYEulerAngles( 0,0,0 );

  // create the detector universe volume
  if ( GlobalPar::GetPar()->geoROOT() ) {
	m_universe = gGeoManager->MakeBox("TWuniverse",gGeoManager->GetMedium("AIR"),m_dimension.x()/2.,m_dimension.y()/2.,m_dimension.z()/2.); 
	m_universe->SetLineColor(kRed);
	gGeoManager->SetTopVisible(1);
  }

  //---------------------------------------------------------------------
  //     Build bar materials in ROOT and FLUKA
  //---------------------------------------------------------------------

  for ( int k=0; k<m_nLayer; k++ ) {
	for ( int j=0; j<m_nBar; j++ ) {
	  
	  //ROOT addNode
	  if ( GlobalPar::GetPar()->geoROOT() )    {
		
			if ( !gGeoManager->GetVolume( m_barMatrix[k][j]->GetMaterialRegionName().c_str() ) )       cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_barMatrix[k][j]->GetMaterialRegionName() << endl;

			TVector3 globalCoord = m_barMatrix[k][j]->GetPosition();
			if ( fDebugLevel> 0 ) cout <<"\t"<<"prova   ", globalCoord.Print();
			Local2Global(&globalCoord);
			TVector3 barRotation = m_barMatrix[k][j]->GetEuler();
			barRotation = barRotation*180.*pow(TMath::Pi(),-1);//TGeoRotation wants the angles in DEG!!! (while our transformations, as Loc2Glob ecc, need rad angles)
			m_universe->AddNode( gGeoManager->GetVolume( m_barMatrix[k][j]->GetMaterialRegionName().c_str() ), 
						 m_barMatrix[k][j]->GetNodeID() , 
						 new TGeoCombiTrans( globalCoord.x(), globalCoord.y(), globalCoord.z(), 
								 new TGeoRotation("null,",
										  barRotation.X(), barRotation.Y(), barRotation.Z()
										  ) ) );
			if ( fDebugLevel> 0 ) cout <<"\t"<<m_barMatrix[k][j]->GetMaterialRegionName()<<"  ", globalCoord.Print();
		}

	  
		// bodies
		if ( GlobalPar::GetPar()->geoFLUKA() ) {
								
			TVector3 minCoord = TVector3( m_barMatrix[k][j]->GetMinCoord().x(), m_barMatrix[k][j]->GetMinCoord().y(), m_barMatrix[k][j]->GetMinCoord().z() );
			TVector3 maxCoord = TVector3( m_barMatrix[k][j]->GetMaxCoord().x(), m_barMatrix[k][j]->GetMaxCoord().y(), m_barMatrix[k][j]->GetMaxCoord().z() );
			Local2Global( &minCoord );
			Local2Global( &maxCoord );


			stringstream ss;
			ss << setiosflags(ios::fixed) << setprecision(6);
			ss << "RPP " << m_barMatrix[k][j]->GetBodyName() << "     " 
			   << minCoord.x() << " " << maxCoord.x() << " "
			   << minCoord.y() << " " << maxCoord.y() << " "
			   << minCoord.z() << " " << maxCoord.z() << endl;
									
			m_bodyPrintOut[ m_barMatrix[k][j]->GetMaterialName() ].push_back( ss.str() );
			m_bodyName    [ m_barMatrix[k][j]->GetMaterialName() ].push_back( m_barMatrix[k][j]->GetBodyName() );

			// regions
			stringstream ssr;
			ssr << setw(13) << setfill( ' ' ) << std::left << m_barMatrix[k][j]->GetRegionName()
				<< "5 " << m_barMatrix[k][j]->GetBodyName() << endl;
										
			m_regionPrintOut[ m_barMatrix[k][j]->GetMaterialName() ].push_back( ssr.str() );
			m_regionName    [ m_barMatrix[k][j]->GetMaterialName() ].push_back( m_barMatrix[k][j]->GetRegionName() );
			if ( genfit::FieldManager::getInstance()->getFieldVal( TVector3( minCoord ) ).Mag() == 0 && genfit::FieldManager::getInstance()->getFieldVal( TVector3( maxCoord ) ).Mag() == 0 )
			  m_magneticRegion[ m_barMatrix[k][j]->GetRegionName() ] = 0;
			else 
			  m_magneticRegion[ m_barMatrix[k][j]->GetRegionName() ] = 1;

		}
	}
  } 
}

//_____________________________________________________________________________
void TATWparGeo::Detector2Sensor_frame( int view, int bar, TVector3* coord ) {
	m_barMatrix[view][bar]->Global2Local( coord );
}

//_____________________________________________________________________________
void TATWparGeo::Sensor2Detector_frame( int bar, int view, TVector3* coord ) {
	m_barMatrix[view][bar]->Local2Global( coord );
}



//_____________________________________________________________________________
void TATWparGeo::Global2Local( TVector3* glob ) {
  glob->Transform( GetRotationToLocal() );
  *glob = *glob - m_center;
}



//_____________________________________________________________________________
void TATWparGeo::Global2Local_TranslationOnly( TVector3* glob ) {
  *glob = *glob - m_center;
}



//_____________________________________________________________________________
void TATWparGeo::Global2Local_RotationOnly( TVector3* glob ) {
  glob->Transform( GetRotationToLocal() );
}



//_____________________________________________________________________________
void TATWparGeo::Local2Global( TVector3* loc ) {
  loc->Transform( GetRotationToGlobal() );
  *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TATWparGeo::Local2Global_TranslationOnly( TVector3* loc ) {
  *loc = *loc + m_center;
}



//_____________________________________________________________________________
void TATWparGeo::Local2Global_RotationOnly( TVector3* loc ) {
  loc->Transform( GetRotationToGlobal() );
}





//_____________________________________________________________________________
float TATWparGeo::GetCoordiante_footFrame( int view, int bar )  {
	TVector3 pos = m_barMatrix[view][bar]->GetPosition();     // bar center
	Local2Global(&pos);
	return ( view == 0 ? pos.X() : pos.Y() );
}




//_____________________________________________________________________________
float TATWparGeo::GetZ_footFrame( int view, int bar )  {
	TVector3 pos = m_barMatrix[view][bar]->GetPosition();     // bar center
	Local2Global(&pos);
	return pos.Z();
}




//_____________________________________________________________________________
TGeoVolume* TATWparGeo::GetVolume() {

  if ( !GlobalPar::GetPar()->geoROOT() ) 
	cout << "ERROR << TATWparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

  return m_universe;

}



string TATWparGeo::PrintBodies() {

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
	cout << "ERROR << TATWparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  stringstream outstr;
  outstr << "* ***Scintillator" << endl;

  // loop in order of the material alfabeth
  for ( map<string, vector<string> >::iterator itMat = m_bodyPrintOut.begin(); itMat != m_bodyPrintOut.end(); itMat++ ) {
	// loop over all body of the same material
	for ( vector<string>::iterator itBody = (*itMat).second.begin(); itBody != (*itMat).second.end(); itBody++ ) {
	  outstr << (*itBody);
	  if (fDebugLevel > 3)    cout << (*itBody);
	}        
  }
  return outstr.str();
}



//_____________________________________________________________________________
string TATWparGeo::PrintRegions() {

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
	cout << "ERROR << TATWparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  stringstream outstr;
  outstr << "* ***Scintillator" << endl;

  // loop in order of the material alfabeth
  for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
	// loop over all region of the same material
	for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
	  outstr << (*itRegion);
	  if (fDebugLevel > 3)    cout << (*itRegion);
	}        
  }
  return outstr.str();
}




//_____________________________________________________________________________
string TATWparGeo::PrintSubtractBodiesFromAir() {

  stringstream outstr;
  int count=0;
  // loop in order of the material alfabeth
  for ( map<string, vector<string> >::iterator itMat = m_bodyName.begin(); itMat != m_bodyName.end(); itMat++ ) {
	// loop over all region of the same material
	for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
	  if ( count % 10 == 0 && count>0 )
	outstr << "\n              ";
	  count++;
	  outstr << " -" << (*itRegion);
	}        
  }
  return outstr.str();

}





//_____________________________________________________________________________
string TATWparGeo::PrintAssignMaterial() {

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
	cout << "ERROR << TATWparGeo::PrintAssignMaterial()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


  // loop in order of the material alfabeth
  stringstream outstr; 
  for ( map<string, vector<string> >::iterator itMat = m_regionName.begin(); itMat != m_regionName.end(); itMat++ ) {

	// check dimension greater than 0
	if ( (*itMat).second.size() == 0 ) {
	  cout << "ERROR << TATWparGeo::PrintAssignMaterial  ::  "<<endl, exit(0);
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
	// if (fDebugLevel > 0)    cout << outstr.str();

  }

  return outstr.str();

}





//_____________________________________________________________________________
string TATWparGeo::PrintParameters() {

  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);

  outstr << "c     SCINTILLATOR PARAMETERS " << endl;
  outstr << endl;
  
  string nstrip = "nstripSCN";
  outstr << "      integer " << nstrip << endl;
  outstr << "      parameter(" << nstrip << " = " << m_nBar << ")" << endl;
  // outstr << typeid(m_nBar).name()<< endl;
  outstr << endl;  

  return outstr.str();

}










//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TATWparGeo::Clear(Option_t*)
{
  return;
}



/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TATWparGeo::ToStream(ostream& os, Option_t*) const
{
  //  os << "TATWparGeo " << GetName() << endl;
  //  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
  //     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}
