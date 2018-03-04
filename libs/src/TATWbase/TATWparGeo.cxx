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
#include "GlobalPar.hxx"



TATWparGeo::TATWparGeo() {

  m_volumeCount = -1;

  m_debug = GlobalPar::GetPar()->Debug(); 

  InitMaterial();

};



void TATWparGeo::InitMaterial() {

  if ( GlobalPar::GetPar()->Debug() > 1 ) {
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

}


//  copy constructor
TATWparGeo::TATWparGeo( TATWparGeo* original ) :

  m_rotation(original->m_rotation),
  m_origin(original->m_origin),  // current position
  m_center(original->m_center),  // current position
  m_dimension(original->m_dimension),

  m_nBar (original->m_nBar),
  m_nLayer (original->m_nLayer),
  m_NBar (original->m_NBar),

  m_barThick_Lz(original->m_barThick_Lz),
  m_layerDistance(original->m_layerDistance){	

  // m_materialThick(original->m_materialThick),
  // m_materialType(original->m_materialType)

  
  BarMatrix m_barMatrix = original->m_barMatrix;
}



void TATWparGeo::InitGeo()  {

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTATWparGeo::InitGeo" << endl<< endl;

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
    m_barThick_Lz =SCN_BAR_THICK;        // ONLY plastic scint

    // set detector dimension
    double length_Lz = m_barThick_Lz*2. + m_layerDistance; // from edge to edge
    //V13 m_dimension = TVector3( SCN_WIDTH, SCN_HEIGHT, length_Lz );
    m_dimension = TVector3( SCN_BAR_WIDTH*m_nBar, SCN_BAR_WIDTH*m_nBar, length_Lz );

    //---------------------------------------------------------------------
    //     Init BAR geometry
    //---------------------------------------------------------------------
    double barDistance = 0;

    // evaluate sensor dimension 
    // double barXdim;// = SCN_BAR_WIDTH;	// 2 cm
    // double barYdim;// = SCN_BAR_HEIGHT;	// 44 cm

    // fill sensor matrix
    double coord_x = m_origin.X() - m_dimension.x()/2.; 	// x coordinate of the bar center in layer 0
    double coord_y = m_origin.y() - m_dimension.y()/2.;	// t coordinate of the bar center in layer 1
    for (int k=0; k<m_nLayer; k++) {
    double bar_newZ = m_origin.Z() - m_dimension.z()/2 + (k+0.5)*m_barThick_Lz + k*m_layerDistance;
    double barXdim = ( k%2 == 0 ? SCN_BAR_WIDTH : SCN_BAR_HEIGHT );
    double barYdim = ( k%2 == 0 ? SCN_BAR_HEIGHT : SCN_BAR_WIDTH );

    //prima lungo y poi lungo x
    for (int i=0; i<m_nBar; i++) {
      double bar_newX = ( k%2 == 0 ? ( coord_x + (0.5+i)*(barXdim) ) : m_center.x() );
      
      for (int j=0; j<m_nBar; j++) {
    double bar_newY = ( k%2 == 1 ? ( coord_y + (0.5+i)*barYdim ) : m_center.y() );

    stringstream ss_bodyBarName; ss_bodyBarName << "scn" << k << setw(2) << setfill('0') << j;
    stringstream ss_regionBarName; ss_regionBarName << "SCN" << k << setw(2) << setfill('0') << j;

    m_volumeCount++;

    m_barMatrix[k][j]->SetMaterial( (string)SCN_MEDIUM, "SCN_MEDIUM", ss_bodyBarName.str(), ss_regionBarName.str(), m_volumeCount );

    m_barMatrix[k][i]->SetBar( bar_newX, bar_newY, bar_newZ,  // bar center
    			   barXdim, barYdim, m_barThick_Lz,    // bar dimension
    			   m_nLayer, m_nBar,
    			   barDistance, barDistance, m_layerDistance,
    			   0,0,0
    			   );

    if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "bar center ",    TVector3( bar_newX, bar_newY, bar_newZ ).Print();
      }
    }
    }

    m_rotation = new TRotation();
    // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
    m_rotation->SetYEulerAngles( 0,0,0 );

    // create the universe volume
    if ( GlobalPar::GetPar()->geoROOT() ) {
    m_universe = gGeoManager->MakeBox("ITuniverse",gGeoManager->GetMedium("AIR"),m_dimension.x()/2,m_dimension.y()/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
    gGeoManager->SetTopVisible(1);
    }

    //---------------------------------------------------------------------
    //     Build bar materials in ROOT and FLUKA
    //---------------------------------------------------------------------

    for ( BarMatrix::iterator itLayer = m_barMatrix.begin(); itLayer != m_barMatrix.end(); itLayer++ ) {
        for ( BarPlane::iterator itBar = (*itLayer).begin(); itBar != (*itLayer).end(); itBar++ ) {

            //ROOT addNode
            if ( GlobalPar::GetPar()->geoROOT() )   
                (*itBar)->AddNodeToUniverse( m_universe );

            // bodies
            if ( GlobalPar::GetPar()->geoFLUKA() ) {
                        
                TVector3 minCoord = TVector3( (*itBar)->GetMinCoord().x(), (*itBar)->GetMinCoord().y(), (*itBar)->GetMinCoord().z() );
                TVector3 maxCoord = TVector3( (*itBar)->GetMaxCoord().x(), (*itBar)->GetMaxCoord().y(), (*itBar)->GetMaxCoord().z() );
                Local2Global( &minCoord );
                Local2Global( &maxCoord );

                stringstream ss;
                ss << setiosflags(ios::fixed) << setprecision(6);
                ss <<  "RPP " << (*itBar)->GetBodyName() <<  "     " 
                << minCoord.x() << " " << maxCoord.x() << " "
                << minCoord.y() << " " << maxCoord.y() << " "
                << minCoord.z() << " " << maxCoord.z() << endl;
                            
                m_bodyPrintOut[ (*itBar)->GetMaterialName() ].push_back( ss.str() );
                m_bodyName    [ (*itBar)->GetMaterialName() ].push_back( (*itBar)->GetBodyName() );

                // regions
                stringstream ssr;
                ssr << setw(13) << setfill( ' ' ) << std::left << (*itBar)->GetRegionName()
                << "5 " << (*itBar)->GetBodyName() << endl;
                                
                m_regionPrintOut[ (*itBar)->GetMaterialName() ].push_back( ssr.str() );
                m_regionName    [ (*itBar)->GetMaterialName() ].push_back( (*itBar)->GetRegionName() );
                if ( genfit::FieldManager::getInstance()->getFieldVal( TVector3( minCoord ) ).Mag() == 0 && genfit::FieldManager::getInstance()->getFieldVal( TVector3( maxCoord ) ).Mag() == 0 )
                    m_magneticRegion[ (*itBar)->GetRegionName() ] = 0;
                else 
                    m_magneticRegion[ (*itBar)->GetRegionName() ] = 1;

            }
        }
    } 
}



//_____________________________________________________________________________
TVector3 TATWparGeo::GetBarPosition( int layer, int bar )  {
  TVector3 pos = m_barMatrix[layer][bar]->GetPosition( layer, bar );
  Local2Global(&pos);
  return pos;
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
      if (m_debug > 3)    cout << (*itBody);
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
      if (m_debug > 3)    cout << (*itRegion);
    }        
  }
  return outstr.str();
}




//_____________________________________________________________________________
string TATWparGeo::PrintSubtractBodiesFromAir() {

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
        // if (m_debug > 0)    cout << outstr.str();

    }

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
