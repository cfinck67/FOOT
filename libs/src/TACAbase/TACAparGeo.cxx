#include <iostream>
#include <fstream>
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

#include "TACAparMap.hxx"
#include "TACAparGeo.hxx"

#include "foot_geo.h"
#include "GlobalPar.hxx"

//##############################################################################


//_____________________________________________________________________________
TACAparGeo::TACAparGeo() {

    m_nCry=-1;
    m_debug = GlobalPar::GetPar()->Debug();

    // fill m_materialOrder, m_materialThick, m_materialType
    InitMaterial();

};



//_____________________________________________________________________________
void TACAparGeo::InitMaterial() {

}



//_____________________________________________________________________________
void TACAparGeo::InitGeo()  {
  
  if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTACAparGeo::InitGeo" << endl<< endl;
 
}


/*
//_____________________________________________________________________________
TVector3 TACAparGeo::GetPosition( int col, int row )  {
}



//_____________________________________________________________________________
void TACAparGeo::Global2Local( TVector3* glob ) {
glob->Transform( GetRotationToLocal() );
*glob = *glob - m_center;
}



//_____________________________________________________________________________
void TACAparGeo::Global2Local_RotationOnly( TVector3* glob ) {
glob->Transform( GetRotationToLocal() );
}



//_____________________________________________________________________________
void TACAparGeo::Local2Global( TVector3* loc ) {
loc->Transform( GetRotationToGlobal() );
*loc = *loc + m_center;
}



//_____________________________________________________________________________
void TACAparGeo::Local2Global_RotationOnly( TVector3* loc ) {
loc->Transform( GetRotationToGlobal() );
}



//_____________________________________________________________________________
TGeoVolume* TACAparGeo::GetVolume() {

if ( !GlobalPar::GetPar()->geoROOT() ) 
cout << "ERROR << TACAparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

return m_universe;
}

*/

//_____________________________________________________________________________
string TACAparGeo::PrintBodies(){

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TACAparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
    

  stringstream outstr;
  outstr << "* ***Calorimeter" << endl;

  stringstream ss;
  char bodyname[20];

  double z = CAL_Z;
  for(int i=0;i<CAL_NROW;i++){
    double y = CAL_Y - CAL_HEIGHT/2 + i * CAL_CRY_HEIGHT+ CAL_CRY_HEIGHT/2;
    for (int j=0;j<CAL_NCOL;j++){
      double x = CAL_X - CAL_WIDTH/2 + j * CAL_CRY_WIDTH + CAL_CRY_WIDTH/2;
      if( sqrt(x*x+y*y) <= CAL_WIDTH/2 - 0.7){
	m_nCry++;
	sprintf(bodyname,"cal%d     ",m_nCry);
	double xmin = x - CAL_CRY_WIDTH/2.;
	double xmax = x + CAL_CRY_WIDTH/2.;
	double ymin = y - CAL_CRY_HEIGHT/2.;
	double ymax = y + CAL_CRY_HEIGHT/2.;
	double zmin = z - CAL_CRY_THICK/2.;
	double zmax = z + CAL_CRY_THICK/2.;

	ss << setiosflags(ios::fixed) << setprecision(6);
	ss << "RPP " << bodyname << xmin << " " << xmax << " "
	   << ymin << " " << ymax << " " 
	   << zmin << " " << zmax << endl;

      }
    }
  }
  outstr << ss.str() << endl;

  return outstr.str();
}



//_____________________________________________________________________________
string TACAparGeo::PrintRegions(){

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TACAparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  stringstream outstr;  
  outstr << "* ***Calorimeter" << endl;

  char stringa[100];
  for (int i=0; i<=m_nCry; i++){
    sprintf(stringa,"CAL%03d       5 cal%d",i,i);
    outstr << stringa << endl;
  }  
  return outstr.str();

}








//_____________________________________________________________________________
string TACAparGeo::PrintAssignMaterial() {

    if ( !GlobalPar::GetPar()->geoFLUKA() ) 
        cout << "ERROR << TACAparGeo::PrintAssignMaterial()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


    // loop in order of the material alfabeth
    stringstream outstr; 
    // for ( map<string, vector<string> >::iterator itMat = m_regionName.begin(); itMat != m_regionName.end(); itMat++ ) {

    //     // check dimension greater than 0
    //     if ( (*itMat).second.size() == 0 ) {
    //         cout << "ERROR << TACAparGeo::PrintAssignMaterial  ::  "<<endl, exit(0);
    //     }

    //     // take the first region
    //     string firstReg = (*itMat).second.at(0);
    //     // take the last region
    //     string lastReg = "";
    //     if ( (*itMat).second.size() != 1 ) 
    //         lastReg = (*itMat).second.at( (*itMat).second.size()-1 );

    //     // build output string 
    //     outstr  << setw(10) << setfill( ' ' ) << std::left << "ASSIGNMA" 
    //             << setw(10) << setfill( ' ' ) << std::right << (*itMat).first 
    //             << setw(10) << setfill( ' ' ) << std::right << firstReg 
    //             << setw(10) << setfill( ' ' ) << std::right << lastReg;
                       
        
    //     // multiple region condition 
    //     if ( (*itMat).second.size() != 1 ) {
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << 1 ;
    //     }
    //     else {
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << " ";
    //     }


    //     // region in the magnetic filed condition
    //     bool isMag = true;
    //     for (int i=0; i<(*itMat).second.size(); i++) {
    //         if ( m_magneticRegion[ (*itMat).second.at(i) ] == 0 ) {
    //             isMag = false;
    //             break;
    //         }
    //     }
    //     if ( isMag )
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << 1 ;
    //     else 
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << " " ;
        
    //     outstr << endl;

    //     // DEBUG
    //     if (m_debug > 0)    cout << outstr.str();

    // }

    // return outstr.str();

    return "ASSIGNMA         BGO    CAL000    CAL359         1\n";

}



//_____________________________________________________________________________
string TACAparGeo::PrintSubtractBodiesFromAir() {

    if ( !GlobalPar::GetPar()->geoFLUKA() ) 
        cout << "ERROR << TACAparGeo::PrintSubtractMaterialFromAir()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


    // loop in order of the material alfabeth
    stringstream outstr; 

     outstr << " -cal0 -cal1 -cal2 -cal3 -cal4 -cal5 ";
     outstr << " -cal6 -cal7 -cal8 -cal9 -cal10 -cal11 ";
     outstr << " -cal12 -cal13 -cal14 -cal15 -cal16 -cal17 ";
     outstr << " -cal18 -cal19 -cal20 -cal21 -cal22 -cal23 ";
     outstr << " -cal24 -cal25 -cal26 -cal27 -cal28 -cal29 ";
     outstr << " -cal30 -cal31 -cal32 -cal33 -cal34 -cal35 ";
     outstr << " -cal36 -cal37 -cal38 -cal39 -cal40 -cal41 ";
     outstr << " -cal42 -cal43 -cal44 -cal45 -cal46 -cal47 ";
     outstr << " -cal48 -cal49 -cal50 -cal51 -cal52 -cal53 ";
     outstr << " -cal54 -cal55 -cal56 -cal57 -cal58 -cal59 ";
     outstr << " -cal60 -cal61 -cal62 -cal63 -cal64 -cal65 ";
     outstr << " -cal66 -cal67 -cal68 -cal69 -cal70 -cal71 ";
     outstr << " -cal72 -cal73 -cal74 -cal75 -cal76 -cal77 ";
     outstr << " -cal78 -cal79 -cal80 -cal81 -cal82 -cal83 ";
     outstr << " -cal84 -cal85 -cal86 -cal87 -cal88 -cal89 ";
     outstr << " -cal90 -cal91 -cal92 -cal93 -cal94 -cal95 ";
     outstr << " -cal96 -cal97 -cal98 -cal99 -cal100 -cal101 ";
     outstr << " -cal102 -cal103 -cal104 -cal105 -cal106 -cal107 ";
     outstr << " -cal108 -cal109 -cal110 -cal111 -cal112 -cal113 ";
     outstr << " -cal114 -cal115 -cal116 -cal117 -cal118 -cal119 ";
     outstr << " -cal120 -cal121 -cal122 -cal123 -cal124 -cal125 ";
     outstr << " -cal126 -cal127 -cal128 -cal129 -cal130 -cal131 ";
     outstr << " -cal132 -cal133 -cal134 -cal135 -cal136 -cal137 ";
     outstr << " -cal138 -cal139 -cal140 -cal141 -cal142 -cal143 ";
     outstr << " -cal144 -cal145 -cal146 -cal147 -cal148 -cal149 ";
     outstr << " -cal150 -cal151 -cal152 -cal153 -cal154 -cal155 ";
     outstr << " -cal156 -cal157 -cal158 -cal159 -cal160 -cal161 ";
     outstr << " -cal162 -cal163 -cal164 -cal165 -cal166 -cal167 ";
     outstr << " -cal168 -cal169 -cal170 -cal171 -cal172 -cal173 ";
     outstr << " -cal174 -cal175 -cal176 -cal177 -cal178 -cal179 ";
     outstr << " -cal180 -cal181 -cal182 -cal183 -cal184 -cal185 ";
     outstr << " -cal186 -cal187 -cal188 -cal189 -cal190 -cal191 ";
     outstr << " -cal192 -cal193 -cal194 -cal195 -cal196 -cal197 ";
     outstr << " -cal198 -cal199 -cal200 -cal201 -cal202 -cal203 ";
     outstr << " -cal204 -cal205 -cal206 -cal207 -cal208 -cal209 ";
     outstr << " -cal210 -cal211 -cal212 -cal213 -cal214 -cal215 ";
     outstr << " -cal216 -cal217 -cal218 -cal219 -cal220 -cal221 ";
     outstr << " -cal222 -cal223 -cal224 -cal225 -cal226 -cal227 ";
     outstr << " -cal228 -cal229 -cal230 -cal231 -cal232 -cal233 ";
     outstr << " -cal234 -cal235 -cal236 -cal237 -cal238 -cal239 ";
     outstr << " -cal240 -cal241 -cal242 -cal243 -cal244 -cal245 ";
     outstr << " -cal246 -cal247 -cal248 -cal249 -cal250 -cal251 ";
     outstr << " -cal252 -cal253 -cal254 -cal255 -cal256 -cal257 ";
     outstr << " -cal258 -cal259 -cal260 -cal261 -cal262 -cal263 ";
     outstr << " -cal264 -cal265 -cal266 -cal267 -cal268 -cal269 ";
     outstr << " -cal270 -cal271 -cal272 -cal273 -cal274 -cal275 ";
     outstr << " -cal276 -cal277 -cal278 -cal279 -cal280 -cal281 ";
     outstr << " -cal282 -cal283 -cal284 -cal285 -cal286 -cal287 ";
     outstr << " -cal288 -cal289 -cal290 -cal291 -cal292 -cal293 ";
     outstr << " -cal294 -cal295 -cal296 -cal297 -cal298 -cal299 ";
     outstr << " -cal300 -cal301 -cal302 -cal303 -cal304 -cal305 ";
     outstr << " -cal306 -cal307 -cal308 -cal309 -cal310 -cal311 ";
     outstr << " -cal312 -cal313 -cal314 -cal315 -cal316 -cal317 ";
     outstr << " -cal318 -cal319 -cal320 -cal321 -cal322 -cal323 ";
     outstr << " -cal324 -cal325 -cal326 -cal327 -cal328 -cal329 ";
     outstr << " -cal330 -cal331 -cal332 -cal333 -cal334 -cal335 ";
     outstr << " -cal336 -cal337 -cal338 -cal339 -cal340 -cal341 ";
     outstr << " -cal342 -cal343 -cal344 -cal345 -cal346 -cal347 ";
     outstr << " -cal348 -cal349 -cal350 -cal351 -cal352 -cal353 ";
     outstr << " -cal354 -cal355 -cal356 -cal357 -cal358 -cal359 ";

     return outstr.str();
 }












//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TACAparGeo::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TACAparGeo::ToStream(ostream& os, Option_t*) const
{
  //  os << "TACAparGeo " << GetName() << endl;
  //  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
  //     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}

