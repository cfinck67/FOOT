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

    m_singleSensorThick_Lz(original->m_singleSensorThick_Lz),
    m_layerDistance(original->m_layerDistance),

    m_nPixel_X(original->m_nPixel_X),
    m_nPixel_Y(original->m_nPixel_Y)         {

    SensorMatrix m_sensorMatrix = original->m_sensorMatrix;
}


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

    // fill m_materialOrder, m_materialThick, m_materialType 
    InitMaterial();

    // evaluate detector dimension and layer distance using materials 
    double length_Lz = 0;
    m_layerDistance = 0;
    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        length_Lz += m_materialThick[ m_materialOrder[i] ];
        if ( m_materialOrder[i] != "ITR_MEDIUM" )
            m_layerDistance += m_materialThick[ m_materialOrder[i] ];
    }

    // set detector dimension
    m_dimension = TVector3( ITR_WIDTH, ITR_HEIGHT, length_Lz );
    double width_Lx = m_dimension.x();
    double height_Ly = m_dimension.y();
    
    m_singleSensorThick_Lz = ITR_THICK;

    // overload -> use a cross check later on
    // m_layerDistance = ITR_LAYDIST;   //wrong
    m_layerDistance += m_singleSensorThick_Lz;
    double sensorDistance = 0;
    double pixelDistance = 0;

    // take it somewhere            // wrong to have it hardcoded
    double pixelWidth_Lx = ITR_DX;
    double pixelHeight_Ly = ITR_DY;

    if ( GlobalPar::GetPar()->Debug() > 2 )  {
        cout << "m_layerDistance " << m_layerDistance << endl;
        cout << "length_Lz " << length_Lz << endl;
    }


    double sensor_Width_Lx = width_Lx - (sensorDistance*(1+m_nSensors_X)) /m_nSensors_X;
    double sensor_Height_Ly = height_Ly - (sensorDistance*(1+m_nSensors_Y)) /m_nSensors_Y;
    double sensor_Length_Lz = m_singleSensorThick_Lz;

    // // total pixels
    // m_nPixel_X = width_Lx / (pixelWidth_Lx + pixelDistance);
    // m_nPixel_Y = height_Ly / (pixelHeight_Ly + pixelDistance);

    // pixels per sensors, same as above as far as we use 1 sensor
    m_nPixel_X = sensor_Width_Lx / (pixelWidth_Lx + pixelDistance);
    m_nPixel_Y = sensor_Height_Ly / (pixelHeight_Ly + pixelDistance);

    
    // fill sensor matrix
    for (int k=0; k<m_nSensors_Z; k++) {
        double sensor_newZ = m_origin.Z() - length_Lz/2 + 0.5*m_singleSensorThick_Lz + k*m_layerDistance;
        for (int i=0; i<m_nSensors_X; i++) {
            double sensor_newX = m_origin.X() - width_Lx/2 + (0.5+i)*(sensor_Width_Lx);
            for (int j=0; j<m_nSensors_Y; j++) {

                double sensor_newY = m_origin.Y() - height_Ly/2 + (1+2*j)*(sensor_Height_Ly/2);

                m_sensorMatrix[k][i][j]->SetMaterial( "SILICON" );

                m_sensorMatrix[k][i][j]->SetSensor( 
                        TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
                        TVector3( sensor_Width_Lx, sensor_Height_Ly, sensor_Length_Lz ),    // sensor dimension
                        m_nPixel_X, m_nPixel_Y,
                        pixelWidth_Lx, pixelHeight_Ly, m_singleSensorThick_Lz,
                        pixelDistance, pixelDistance, 0, //m_layerDistance,
                        TVector3(0,0,0)
                 );
                // if (m_debug) 
                if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "sensor center ",    TVector3( sensor_newX, sensor_newY, sensor_newZ ).Print();
            }
        }
    }

    m_rotation = new TRotation();
    // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
    m_rotation->SetYEulerAngles( 0,0,0 );


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

    
    // if ( (medium = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
    // TGeoMedium* vacuum_med = new TGeoMedium("Vacuum",0, gGeoManager->GetMaterial("Vacuum"));
    // TGeoMedium* silicon = new TGeoMedium( "silicon_med", 1, gGeoManager->GetMaterial("Silicon") );
    // TGeoMedium* kapton = new TGeoMedium( "kapton_med", 2, gGeoManager->GetMaterial("Kapton") );
    // TGeoMedium* epoxy = new TGeoMedium( "epoxy_med", 3, gGeoManager->GetMaterial("Epoxy") );
    // TGeoMedium* aluminium = new TGeoMedium( "aluminium_med", 4, gGeoManager->GetMaterial("Aluminium") );
    // TGeoMedium* siCFoam = new TGeoMedium( "siCFoam_med", 5, gGeoManager->GetMaterial("SiCFoam") );


    if ( GlobalPar::GetPar()->Debug() > 3 )  {
        cout << endl << "IT List of Material\n ";
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

    double width_Lx = m_dimension.X();
    double height_Ly = m_dimension.Y();
    
   // create main box
    // TGeoVolume *box = gGeoManager->MakeBox("ITbox",gGeoManager->GetMedium("Vacuum_med"),width_Lx+1,height_Ly+1,m_dimension.z()+0.5); //top è scatola che conterrà tutto (dimensioni in cm)
    TGeoVolume *box = gGeoManager->MakeBox("ITbox",gGeoManager->GetMedium("Air_med"),width_Lx/2,height_Ly/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
    gGeoManager->SetTopVisible(1);

    TGeoVolume *siliconFoil = gGeoManager->MakeBox("siliconFoil",gGeoManager->GetMedium("Silicon_med"),width_Lx/2,height_Ly/2,m_singleSensorThick_Lz/2); //top è scatola che conterrà tutto (dimensioni in cm)
    siliconFoil->SetLineColor(kOrange);
    TGeoVolume *kaptonFoil = gGeoManager->MakeBox("kaptonFoil",gGeoManager->GetMedium("Kapton_med"),width_Lx/2,height_Ly/2,m_materialThick[ "ITR_KAP_MEDIUM" ]/2); //top è scatola che conterrà tutto (dimensioni in cm)
    kaptonFoil->SetLineColor(kOrange-7);
    TGeoVolume *coverKaptonFoil = gGeoManager->MakeBox("coverKaptonFoil",gGeoManager->GetMedium("Kapton_med"),width_Lx/2,height_Ly/2,m_materialThick[ "ITR_COV_MEDIUM" ]/2); //top è scatola che conterrà tutto (dimensioni in cm)
    kaptonFoil->SetLineColor(kOrange-7);
    TGeoVolume *alFoil = gGeoManager->MakeBox("alFoil",gGeoManager->GetMedium("Aluminium_med"),width_Lx/2,height_Ly/2,m_materialThick[ "ITR_AL_MEDIUM" ]/2); //top è scatola che conterrà tutto (dimensioni in cm)
    alFoil->SetLineColor(kGray);
    TGeoVolume *epoxyFoil = gGeoManager->MakeBox("epoxyFoil",gGeoManager->GetMedium("Epoxy_med"),width_Lx/2,height_Ly/2,m_materialThick[ "ITR_EPO_MEDIUM" ]/2); //top è scatola che conterrà tutto (dimensioni in cm)
    epoxyFoil->SetLineColor(kCyan-3);
    TGeoVolume *siCFoamFoil = gGeoManager->MakeBox("iCFoamFoil",gGeoManager->GetMedium("SiCFoam_med"),width_Lx/2,height_Ly/2,m_materialThick[ "ITR_FOAM_MEDIUM" ]/2); //top è scatola che conterrà tutto (dimensioni in cm)
    siCFoamFoil->SetLineColor(kViolet+6);

    int c=0;
    double position = -m_dimension.z()/2;
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(epoxyFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_MEDIUM" ]/2 + m_materialThick[ "ITR_EPO_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(coverKaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_EPO_MEDIUM" ]/2 + m_materialThick[ "ITR_COV_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(alFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_COV_MEDIUM" ]/2 + m_materialThick[ "ITR_AL_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(kaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_AL_MEDIUM" ]/2 + m_materialThick[ "ITR_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(alFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_KAP_MEDIUM" ]/2 + m_materialThick[ "ITR_AL_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(coverKaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_AL_MEDIUM" ]/2 + m_materialThick[ "ITR_COV_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(siCFoamFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_COV_MEDIUM" ]/2 + m_materialThick[ "ITR_FOAM_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(coverKaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_FOAM_MEDIUM" ]/2 + m_materialThick[ "ITR_COV_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(alFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_COV_MEDIUM" ]/2 + m_materialThick[ "ITR_AL_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(kaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_AL_MEDIUM" ]/2 + m_materialThick[ "ITR_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(alFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_KAP_MEDIUM" ]/2 + m_materialThick[ "ITR_AL_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(coverKaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_AL_MEDIUM" ]/2 + m_materialThick[ "ITR_COV_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(epoxyFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_COV_MEDIUM" ]/2 + m_materialThick[ "ITR_EPO_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position+=( m_materialThick[ "ITR_EPO_MEDIUM" ]/2+ m_materialThick[ "ITR_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    

    return box;
}



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

    
    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        if( m_materialOrder[i] == "ITR_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_MEDIUM;
        }
        else if( m_materialOrder[i] == "ITR_EPO_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_EPO_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_EPO_MEDIUM;
        }
        else if( m_materialOrder[i] == "ITR_COV_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_COV_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_COV_MEDIUM;
        }
        else if( m_materialOrder[i] == "ITR_AL_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_AL_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_AL_MEDIUM;
        }
        else if( m_materialOrder[i] == "ITR_KAP_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_KAP_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_KAP_MEDIUM;
        }
        else if( m_materialOrder[i] == "ITR_FOAM_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = ITR_FOAM_THICK;
            m_materialType[ m_materialOrder[i] ] = ITR_FOAM_MEDIUM;
        }
    }

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

