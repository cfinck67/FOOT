/*!
  \file
  \version $Id: TAVTbaseParGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAVTbaseParGeo.
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

#include "TAVTbaseParGeo.hxx"

#include "foot_geo.h"
#include "GlobalPar.hxx"


//_____________________________________________________________________________
TAVTbaseParGeo::TAVTbaseParGeo()
{
    m_nPassiveLayersPerBoard_z = 13;
    m_volumeCount = -1;
    m_passiveCount = -1;
    m_setW_0number = 2;

};

//_____________________________________________________________________________
//  copy constructor
TAVTbaseParGeo::TAVTbaseParGeo( TAVTbaseParGeo* original ) :

    m_rotation(original->m_rotation),
    m_origin(original->m_origin),  // current position
    m_center(original->m_center),  // current position
    m_dimension(original->m_dimension),

    m_nSensors (original->m_nSensors),

    m_materialOrder(original->m_materialOrder),

    m_materialThick(original->m_materialThick),
    m_materialType(original->m_materialType),

    m_siliconSensorThick_Lz(original->m_siliconSensorThick_Lz),
    m_layerDistance_samePair(original->m_layerDistance_samePair),
    m_layerDistance_interPair(original->m_layerDistance_interPair),

    m_nPixel_X(original->m_nPixel_X),
    m_nPixel_Y(original->m_nPixel_Y)         
{

   SensorMatrix m_sensorMatrix = original->m_sensorMatrix;
}


//_____________________________________________________________________________
TVector3 TAVTbaseParGeo::GetPixelPos_sensorFrame( int layer, int col, int row )
{
   return m_sensorMatrix[layer][0][0]->GetPosition_local( col, row );
}

//_____________________________________________________________________________
TVector3 TAVTbaseParGeo::GetPixelPos_detectorFrame( int layer, int col, int row )
{
   return m_sensorMatrix[layer][0][0]->GetPosition( col, row );
}

//_____________________________________________________________________________
TVector3 TAVTbaseParGeo::GetPixelPos_footFrame( int layer, int col, int row )  {
    TVector3 pos = GetPixelPos_detectorFrame( layer, col, row );
    Local2Global(&pos);
    return pos;
}

//_____________________________________________________________________________
float TAVTbaseParGeo::GetColumnCenter_sensorFrame( int col )  { return GetPixelPos_sensorFrame( 0, col, 0 ).x(); }
float TAVTbaseParGeo::GetColumnCenter_detectorFrame( int layer, int col )  { return GetPixelPos_detectorFrame( layer, col, 0 ).x(); }
float TAVTbaseParGeo::GetColumnCenter_footFrame( int layer, int col ) { return GetPixelPos_footFrame( layer, col, 0 ).x(); }
//_____________________________________________________________________________
float TAVTbaseParGeo::GetRowCenter_sensorFrame( int row )     { return GetPixelPos_sensorFrame( 0, 0, row ).y();   }
float TAVTbaseParGeo::GetRowCenter_detectorFrame( int layer, int row )     { return GetPixelPos_detectorFrame( layer, 0, row ).y();   }
float TAVTbaseParGeo::GetRowCenter_footFrame( int layer, int row )    { return GetPixelPos_footFrame( layer, 0, row ).y();     }


//_____________________________________________________________________________
Float_t TAVTbaseParGeo::GetPositionU(Int_t column) const
{      // GetColumnCenter_sensorFrame
   return ((2*column - m_nPixel_X + 1 ) * m_pitchX)/2 ;
 }

//_____________________________________________________________________________
Float_t TAVTbaseParGeo::GetPositionV(Int_t line) const
{         //TAVTbaseParGeo::GetRowCenter_sensorFrame(
   return -((2*line - m_nPixel_X + 1 ) * m_pitchY)/2 ;
}


//_____________________________________________________________________________
void TAVTbaseParGeo::Detector2Sensor_frame( int sensorID, TVector3* coord )
{
    m_sensorMatrix[sensorID][0][0]->Global2Local( coord );
}

//_____________________________________________________________________________
void TAVTbaseParGeo::Sensor2Detector_frame( int sensorID, TVector3* coord )
{
    m_sensorMatrix[sensorID][0][0]->Local2Global( coord );
}

//_____________________________________________________________________________
void TAVTbaseParGeo::Global2Local( TVector3* glob )
{
    glob->Transform( GetRotationToLocal() );
    *glob = *glob - m_center;
}

//_____________________________________________________________________________
void TAVTbaseParGeo::Global2Local_TranslationOnly( TVector3* glob )
{
    *glob = *glob - m_center;
}

//_____________________________________________________________________________
void TAVTbaseParGeo::Global2Local_RotationOnly( TVector3* glob )
{
    glob->Transform( GetRotationToLocal() );
}

//_____________________________________________________________________________
void TAVTbaseParGeo::Local2Global( TVector3* loc )
{
    loc->Transform( GetRotationToGlobal() );
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAVTbaseParGeo::Local2Global_TranslationOnly( TVector3* loc )
{
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAVTbaseParGeo::Local2Global_RotationOnly( TVector3* loc )
{
    loc->Transform( GetRotationToGlobal() );
}

//_____________________________________________________________________________
TGeoVolume* TAVTbaseParGeo::GetVolume()
{

    if ( !GlobalPar::GetPar()->geoROOT() ) 
        cout << "ERROR << TAVTbaseParGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

    return m_universe;
}


//_____________________________________________________________________________
string TAVTbaseParGeo::PrintBodies()
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
            if (m_debug > 3)    cout << (*itBody);
        }        
    }
    return outstr.str();
}

//_____________________________________________________________________________
string TAVTbaseParGeo::PrintRegions()
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
            if (m_debug > 3)    cout << (*itRegion);
        }        
    }
    return outstr.str();
}

//_____________________________________________________________________________
string TAVTbaseParGeo::PrintSubtractBodiesFromAir()
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
string TAVTbaseParGeo::PrintAssignMaterial()
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

//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TAVTbaseParGeo::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAVTbaseParGeo::ToStream(ostream& os, Option_t*) const
{
//  os << "TAVTbaseParGeo " << GetName() << endl;
//  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
//     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}
