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

#include "TAMSDparMap.hxx"
#include "TAMSDparGeo.hxx"

#include "foot_geo.h"
#include "GlobalPar.hxx"




//  copy constructor
TAMSDparGeo::TAMSDparGeo( TAMSDparGeo* original ) :

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





void TAMSDparGeo::InitGeo()  {

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTAMSDparGeo::InitGeo" << endl<< endl;

    m_origin = TVector3( 0, 0, 0 );
    m_center = TVector3( MSD_X, MSD_Y, MSD_Z );

    m_nSensors_X = 1;
    m_nSensors_Y = 1;
    m_nSensors_Z = MSD_NLAY;
    TVector3 m_NSensors = TVector3( m_nSensors_X, m_nSensors_Y, m_nSensors_Z );

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
    
    // fill m_materialOrder, m_materialThick, m_materialType 
    InitMaterial();

    // evaluate layer dimension 
    m_layerThick = 0;
    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        m_layerThick += m_materialThick[ m_materialOrder[i] ];     
    }

    m_layerDistance = MSD_LAYDIST;            // from center to center
    m_siliconSensorThick_Lz = MSD_THICK;       // ONLY silicon
    m_nSensor_X_Layer = MSD_NVIEW;

    // set detector dimension
    double length_Lz = m_layerThick + (m_nSensors_Z-1)*m_layerDistance; // from edge to edge
    m_dimension = TVector3( MSD_WIDTH, MSD_HEIGHT, length_Lz );
    double width_Lx = m_dimension.x();
    double height_Ly = m_dimension.y();
    
    double sensorDistance = 0;
    double pixelDistance = 0;

    double pixelWidth_Lx = MSD_DX;
    double pixelHeight_Ly = MSD_DY;

    if ( GlobalPar::GetPar()->Debug() > 2 )  {
        cout << "m_layerDistance " << m_layerDistance << endl;
        cout << "length_Lz " << length_Lz << endl;
    }

    double sensor_Width_Lx = width_Lx - (sensorDistance*(1+m_nSensors_X)) /m_nSensors_X;
    double sensor_Height_Ly = height_Ly - (sensorDistance*(1+m_nSensors_Y)) /m_nSensors_Y;
    double sensor_Length_Lz = m_layerThick;
    // double sensor_Length_Lz = m_length_Lz - ((sensorDistance+1)*m_nSensors_Z) /m_nSensors_Z;

    // // total pixels
    // m_nPixel_X = width_Lx / (pixelWidth_Lx + pixelDistance);
    // m_nPixel_Y = height_Ly / (pixelHeight_Ly + pixelDistance);

    // pixels per sensors, same as above as far as we use 1 sensor
    m_nPixel_X = sensor_Width_Lx / (pixelWidth_Lx + pixelDistance);
    m_nPixel_Y = sensor_Height_Ly / (pixelHeight_Ly + pixelDistance);

    
    // fill sensor matrix
    for (int k=0; k<m_nSensors_Z; k++) {
        double sensor_newZ = m_origin.Z() - length_Lz/2 +0.5*m_layerThick + k*m_layerDistance;
        for (int i=0; i<m_nSensors_X; i++) {
            double sensor_newX = m_origin.X() - width_Lx/2 + (0.5+i)*(sensor_Width_Lx);
            for (int j=0; j<m_nSensors_Y; j++) {

                double sensor_newY = m_origin.Y() - height_Ly/2 + (1+2*j)*(sensor_Height_Ly/2);

                m_sensorMatrix[k][i][j]->SetMaterial( (string)MSD_MEDIUM );

                m_sensorMatrix[k][i][j]->SetSensor( 
                        TVector3( sensor_newX, sensor_newY, sensor_newZ ),  // sensor center
                        TVector3( sensor_Width_Lx, sensor_Height_Ly, sensor_Length_Lz ),    // sensor dimension
                        m_nPixel_X, m_nPixel_Y,
                        pixelWidth_Lx, pixelHeight_Ly, m_siliconSensorThick_Lz,
                        pixelDistance, pixelDistance, 0, //layerDistance,
                        TVector3(0,0,0)
                 );
                // if (m_debug) 
                if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "sensor center ",    TVector3( sensor_newX, sensor_newY, sensor_newZ ).Print();
            }
        }
    }

    // FillMaterialCollection();


    m_rotation = new TRotation();
    // m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );
    m_rotation->SetYEulerAngles( 0,0,0 );


}





//_____________________________________________________________________________
TVector3 TAMSDparGeo::GetPosition( int layer, int view, int strip )  {
    // TVector3 sensorCoord = GetSensorCoortdinates( int layer, int col, int row );
    // TVector3 pos = m_sensorMatrix[sensorCoord.z()][sensorCoord.x()][sensorCoord.y()]->GetPosition();
    TVector3 pos = m_sensorMatrix[layer][0][0]->GetPosition( view, strip );

    // set the z coordinate as one of the sensor surface, depending on the view
    // if ( view == 0 )           pos.SetZ( pos.z() - 0.5*m_siliconSensorThick_Lz );
    // else if ( view ==  1 )      pos.SetZ( pos.z() + 0.5*m_siliconSensorThick_Lz );
    // else                        cout << "ERROR :: TAMSDparGeo::GetPosition  -->  wrong value for strip view = " << view << endl, exit(0);

    Local2Global(&pos);
    return pos;
}


//_____________________________________________________________________________
TVector3 TAMSDparGeo::GetLayerCenter( int layer ) {

    TVector3 pos = m_sensorMatrix[layer][0][0]->GetCenter();
    
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
void TAMSDparGeo::Local2Global( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAMSDparGeo::Local2Global_RotationOnly( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
} 





//_____________________________________________________________________________
TGeoVolume* TAMSDparGeo::GetVolume() {

    
    // if ( (medium = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
    // TGeoMedium* vacuum_med = new TGeoMedium("Vacuum",0, gGeoManager->GetMaterial("air"));
    // TGeoMedium* silicon = new TGeoMedium( "silicon_med", 1, gGeoManager->GetMaterial("air") );
    // TGeoMedium* kapton = new TGeoMedium( "kapton_med", 2, gGeoManager->GetMaterial("air") );
    // TGeoMedium* epoxy = new TGeoMedium( "epoxy_med", 3, gGeoManager->GetMaterial("air") );
    // TGeoMedium* aluminium = new TGeoMedium( "aluminium_med", 4, gGeoManager->GetMaterial("air") );
    // TGeoMedium* siCFoam = new TGeoMedium( "siCFoam_med", 5, gGeoManager->GetMaterial("air") );

    // if ( GlobalPar::GetPar()->Debug() > 1 ) {
    //     cout << endl << "MSD List of Materil\n ";
    //     TIter next( gGeoManager->GetListOfMaterials() );
    //     while ( TGeoMaterial *obj = (TGeoMaterial*) next() ) {
    //       cout << obj->GetName () << endl;
    //     }
    //     cout << endl << "List of Media\n ";
    //     TIter nnext( gGeoManager->GetListOfMedia() );
    //     while ( TGeoMedium *obj = (TGeoMedium *) nnext()  ) {
    //       cout << obj->GetName () << endl;
    //     }
    // }

    double width_Lx = m_dimension.X();
    double height_Ly = m_dimension.Y();

    // create main box
   // TGeoVolume *box = gGeoManager->MakeBox("ITbox",gGeoManager->GetMedium("Vacuum_med"),width_Lx+1,height_Ly+1,m_dimension.z()+0.5); //top è scatola che conterrà tutto (dimensioni in cm)
   TGeoVolume *box = gGeoManager->MakeBox("MSDbox",gGeoManager->GetMedium("Air_med"),width_Lx/2,height_Ly/2,m_dimension.z()/2); //top è scatola che conterrà tutto (dimensioni in cm)
   gGeoManager->SetTopVisible(1);

    TGeoVolume *siliconFoil = gGeoManager->MakeBox("siliconFoil",gGeoManager->GetMedium("Silicon_med"),width_Lx/2,height_Ly/2, m_materialThick[ "MSD_MEDIUM" ]/2 ); //top è scatola che conterrà tutto (dimensioni in cm)
    siliconFoil->SetLineColor(kOrange);
    siliconFoil->SetFillColor(kOrange);
    // TGeoVolume *kaptonFoil = gGeoManager->MakeBox("kaptonFoil",gGeoManager->GetMedium("Kapton_med"), width_Lx/2, height_Ly/2, m_materialThick[ "MSD_KAP_MEDIUM" ]/2 ); //top è scatola che conterrà tutto (dimensioni in cm)
    // kaptonFoil->SetLineColor(kOrange-7);



    int c=0;
    
    double position1 = -m_dimension.z()/2;
    // layer 1
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position1+=( m_materialThick[ "m_siliconSensorThick_Lz_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    // box->AddNode(kaptonFoil, c++ , new TGeoCombiTrans( 0, 0,   position1+=( m_materialThick[ "MSD_MEDIUM" ]/2 + m_materialThick[ "MSD_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position1+=( m_materialThick[ "MSD_MEDIUM" ]/2 + m_materialThick[ "MSD_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    
    // layer 2
    double position2 = -(m_materialThick[ "MSD_MEDIUM" ] + m_materialThick[ "MSD_KAP_MEDIUM" ]/2);
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position2+=( m_materialThick[ "MSD_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    // box->AddNode(kaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position2+=( m_materialThick[ "MSD_MEDIUM" ]/2 + m_materialThick[ "MSD_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position2+=( m_materialThick[ "MSD_MEDIUM" ]/2 + m_materialThick[ "MSD_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    
    // layer 3
    double position3 = (m_dimension.z()/2) -( m_materialThick[ "MSD_MEDIUM" ]*2 + m_materialThick[ "MSD_KAP_MEDIUM" ] );
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position3+=( m_materialThick[ "MSD_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    // box->AddNode(kaptonFoil, c++ , new TGeoCombiTrans( 0, 0,  position3+=( m_materialThick[ "MSD_MEDIUM" ]/2 + m_materialThick[ "MSD_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    box->AddNode(siliconFoil, c++ , new TGeoCombiTrans( 0, 0,  position3+=( m_materialThick[ "MSD_MEDIUM" ]/2 + m_materialThick[ "MSD_KAP_MEDIUM" ]/2 ), new TGeoRotation("null,",0,0,0)));
    

    return box;
}




void TAMSDparGeo::InitMaterial() {

    m_materialOrder = {  "MSD_MEDIUM", 
                         // "MSD_KAP_MEDIUM",
                         "MSD_MEDIUM"
                         };

    
    for ( unsigned int i=0; i<m_materialOrder.size(); i++ ) {
        if( m_materialOrder[i] == "MSD_MEDIUM" ){
            m_materialThick[ m_materialOrder[i] ] = MSD_THICK;
            m_materialType[ m_materialOrder[i] ] = MSD_MEDIUM;
        }
        
        // else if( m_materialOrder[i] == "MSD_KAP_MEDIUM" ){
        //     m_materialThick[ m_materialOrder[i] ] = MSD_THICK;
        //     m_materialType[ m_materialOrder[i] ] = MSD_KAP_MEDIUM;
        // }
        
    }

}




// **************** VERTEXING      *******************************************************


// //_____________________________________________________________________________
// TGeoVolume* TAMSDparGeo::BuildVertex(const char* basemoduleName, const char *vertexName)
// {
//    TGeoVolume* vertex = 0x0; 
   
//    for(Int_t iSensor = 0; iSensor < GetSensorsN(); iSensor++) {	 
// 	  TGeoHMatrix* hm = GetTransfo(iSensor);
// 	  vertex = TAMSDparGeo::AddVertexModule(hm, basemoduleName, vertexName);
//    }
   
//    return vertex;
// }

// //_____________________________________________________________________________
// TGeoVolume* TAMSDparGeo::AddVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
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
// TEveGeoShapeExtract* TAMSDparGeo::BuildExtractVertex(const char* basemoduleName, const char *vertexName)
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
// TEveGeoShapeExtract* TAMSDparGeo::AddExtractVertexModule(TGeoHMatrix* hm, const char* basemoduleName, const char *vertexName)
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
// TEveGeoShapeExtract* TAMSDparGeo::AddExtractTarget(const Float_t dx, const Float_t dy, const Float_t dz, const char *targetName)
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

