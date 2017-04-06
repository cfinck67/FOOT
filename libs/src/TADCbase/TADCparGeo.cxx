/*!
  \file
  \version $Id: TADCparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TADCparGeo.
*/

#include <string.h>
#include <math.h>

#include <fstream>
#include "TEveTrans.h"
#include "TGeoBBox.h"
#include "TSystem.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TList.h"
#include "TROOT.h"
#include "TFile.h"

#include "TCanvas.h"
#include "TGraph.h"

#include "foot_geo.h"
#include "GlobalPar.hxx"

#include "TAGgeoTrafo.hxx" 
#include "TADCparGeo.hxx"


//##############################################################################

/*!
  \class TADCparGeo TADCparGeo.hxx "TADCparGeo.hxx"
  \brief Map and Geometry parameters for the beam monitor. **
*/

ClassImp(TADCparGeo);

//------------------------------------------+-----------------------------------
//! Default constructor.
TADCparGeo::TADCparGeo() {

    /*  cordinates of the Beam Monitor center */ //middle of beam monitor
    m_origin.SetXYZ( 0,0,0 );
    m_center.SetXYZ( DCH_X, DCH_Y, DCH_Z );

    /* parametri geometrici per la nuova camera a drift di monitor del beam */
    m_width_x = DCH_WIDTH;              //width of beam monitor (gas)
    m_height_y = DCH_HEIGHT;            //height of beam monitor (gas) 
    m_length_z = DCH_LENGTH;            //length of beam monitor (gas)
    m_Dimension_root.SetXYZ( DCH_WIDTH/2, DCH_HEIGHT/2, DCH_LENGTH/2 );

    // shield
    m_shield_length = DCH_SHI_LENGTH;    //length (along z) of beam monitor shield
    m_shield_thick = DCH_SHI_THICK;      //thickness of beam monitor shield
    m_maylar_thick = DCH_MYL_THICK;      //mylar windows thickness


    m_nLayers = DCH_NLAY;                 //no. of layers
    m_nWire_x_Layer  = DCH_NWIRELAY;      //no. of wires (field or sense) per layer
    m_nSense_x_Layer = DCH_NSENSELAY;     //no. of sense wires (and also cells) per layers
    // m_sensePos = {14,16,18,20,22,24};

    m_wire_step_z = DCH_STEP;             //distance between wires (z distance)
    m_wire_passo_t = DCH_PASSO;           //distance between wires (x or y distance)
    m_layerDistance = DCH_LAYDIST;        //distance between layers U and V

    m_sense_radius = DCH_RSENSE;        //radius of sensitive wires//ATTENZIONE:in first erano 0.003
    m_field_radius = DCH_RFIELD;        //radius of field wires//ATTENZIONE:in first erano 0.008

    /*  shift dei fili rispetto ai lati della camera (-0.8 in x,y if you change senseid)*/
    m_delta_shift.SetXYZ( DCH_DELTAX, DCH_DELTAY, DCH_DELTAZ );


    /* Euler angles that defines the BEAM monitor orientation ( degrees)*/
    m_tilt_eulerAngle.SetXYZ(0,0,0);  

    m_material_shield = DCH_SHI_MEDIUM;
    m_material_maylar = DCH_MYL_MEDIUM;
    m_material_gas = DCH_GAS_MEDIUM;
    m_material_wire_S = DCH_SWIRE_MEDIUM;
    m_material_wire_F = DCH_FWIRE_MEDIUM;

    // FillMaterialCollection();
    m_rotation = new TRotation();
    m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );

}



// void TADCparGeo::FillMaterialCollection() {

//     TGeoMixture *_airMat = new TGeoMixture("Air",3);
//     _airMat->AddElement(14.01,7.,.78);
//     _airMat->AddElement(16.00,8.,.21);
//     _airMat->AddElement(39.95,18.,.01);
//     _airMat->SetDensity(1.2e-3);
//     TGeoMedium *_air = new TGeoMedium("air",medInd++,_airMat,mPar);

// }



void TADCparGeo::InitGeo(){
   /* 
      generazione parametri volumi di FLUKA :  ogni filo e' definito 
      da un cilindro: 2 basi nel mylar e direzione
      parallela all'asse x ( vista U) e y (vista V) definito da:
      
      1) vettore posizione del punto centrale della base    NO
      2) vettore che da una base arriva all'altra base
      3) raggio
   */

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTADCparGeo::InitGeo wire: " << m_nWire_x_Layer<< "  lay: "<<m_nLayers<< endl<< endl;
    
    InitMatrix_Can( m_wireMatrix_YZ, m_nWire_x_Layer, m_nLayers );
    InitMatrix_Can( m_wireMatrix_XZ, m_nWire_x_Layer, m_nLayers );

    for( int il=0; il<m_nLayers; il++ ) {

        double intraLayerStep = 0;
        int nWire_x_Column = m_nWire_x_Layer/3; 
        // Side and top layers are shifted of 1-layer width 
        double layerStep_side =  2 * il * ( 2 * m_wire_step_z + m_layerDistance );
        double layerStep_top =  (2 * il + 1) * ( 2 * m_wire_step_z + m_layerDistance );

        // sense wire position
        for (int k=nWire_x_Column+1; k<2*nWire_x_Column-1; k+=2){
            m_sensePos.push_back( k );
            // cout << k << endl;
        }

        // alternate layers are shifted in x(y), first shift in top view(along x), even layers.
        float top_WireAlternateShift = ( il % 2 == 0 ? 0 : m_wire_passo_t ); 
        float side_WireAlternateShift = ( il % 2 == 0 ? m_wire_passo_t : 0 ); 

        
        // if ( il % 2 == 0 )  {
        //     sfalsoXZ = 0;
        //     sfalsoYZ = m_wire_passo_t;
        // }
        // else {                
        //     sfalsoXZ = m_wire_passo_t;
        //     sfalsoYZ = 0;
        // }

        for ( int iw=0; iw<m_nWire_x_Layer; iw++ ) {       

            // every 13 (nWire_x_Column) wire shift to make che cells, same layer
            if ( iw != 0 && (iw % nWire_x_Column == 0) )   intraLayerStep += m_wire_step_z;

            // evaluate the wire increment in x(y)
            double passoT = ( iw - nWire_x_Column*trunc(iw/nWire_x_Column) ) * m_wire_passo_t;

            /*  U wires -> along x, SIDE view */
            m_wireMatrix_YZ[iw][il]->SetPosX( m_origin.X() );
            m_wireMatrix_YZ[iw][il]->SetPosY( - m_Dimension_root.Y() + m_delta_shift.Y() + passoT + side_WireAlternateShift );
            m_wireMatrix_YZ[iw][il]->SetPosZ( - m_Dimension_root.Z() + m_delta_shift.Z() + intraLayerStep + layerStep_side );

            m_wireMatrix_YZ[iw][il]->SetLengthX( m_width_x );
            m_wireMatrix_YZ[iw][il]->SetLengthY( 0 );
            m_wireMatrix_YZ[iw][il]->SetLengthZ( 0 );


            /* V wires -> along y, TOP view*/
            m_wireMatrix_XZ[iw][il]->SetPosX( - m_Dimension_root.X() + m_delta_shift.X() + passoT + top_WireAlternateShift );
            m_wireMatrix_XZ[iw][il]->SetPosY( m_origin.Y() );
            m_wireMatrix_XZ[iw][il]->SetPosZ( - m_Dimension_root.Z() + m_delta_shift.Z() + intraLayerStep + layerStep_top );

            m_wireMatrix_XZ[iw][il]->SetLengthX( 0 );
            m_wireMatrix_XZ[iw][il]->SetLengthY( m_height_y );
            m_wireMatrix_XZ[iw][il]->SetLengthZ( 0 );


            // radius V & U
            m_wireMatrix_XZ[iw][il]->SetRadius( m_field_radius );
            m_wireMatrix_YZ[iw][il]->SetRadius( m_field_radius );
            m_wireMatrix_XZ[iw][il]->SetSense( false );
            m_wireMatrix_YZ[iw][il]->SetSense( false );

            // m_wireMatrix_XZ[iw][il]->SetMaterial( m_MaterialCollection[m_material_wire_F] );
            // m_wireMatrix_YZ[iw][il]->SetMaterial( m_MaterialCollection[m_material_wire_F] );

            for ( unsigned int i=0; i<m_sensePos.size(); i++ ) {
                if ( iw == m_sensePos[i] ) {   
                    m_wireMatrix_XZ[iw][il]->SetRadius( m_sense_radius );
                    m_wireMatrix_YZ[iw][il]->SetRadius( m_sense_radius );
                    m_wireMatrix_XZ[iw][il]->SetSense( true );
                    m_wireMatrix_YZ[iw][il]->SetSense( true );

                    // m_wireMatrix_XZ[iw][il]->SetMaterial( m_MaterialCollection[m_material_wire_S] );
                    // m_wireMatrix_YZ[iw][il]->SetMaterial( m_MaterialCollection[m_material_wire_S] );

                    break;
                }                        
            }

        }// end wire loop
    }// end layer loop

}



void TADCparGeo::GetWireEndPoints( int cell, int layer, string view, TVector3 & end1, TVector3 & end2 ) {

    if ( cell < 0 || cell > m_nSense_x_Layer-1 ) {
        cout << "ERROR :: TADCparGeo.hxx::GetWireEndPoints >> wrong cell number input:  " << cell << endl;
        exit(0);
    }

    int wire = m_sensePos[cell];
    TVector3 center, halfLenght;

    if ( view == "x" || view == "U" || view == "SIDE" ) 
        m_wireMatrix_YZ[wire][layer]->GetCoord_ROOT( center, halfLenght );
    else if ( view == "y" || view == "V" || view == "TOP" ) 
        m_wireMatrix_XZ[wire][layer]->GetCoord_ROOT( center, halfLenght );
    else {
        cout << "ERROR :: TADCparGeo.hxx::GetWireEndPoints >> wrong wire view definition:  " << view << endl;
        exit(0);
    }

    end1 = center + halfLenght;
    end2 = center - halfLenght;
    Local2Global(&end1);
    Local2Global(&end2);

}




// bottom end coordinates of the wire
TVector3 TADCparGeo::GetWirePosition_FLUKA( int cell, int layer, int aview ) {
    vector<string> viewTranslator = { "SIDE", "TOP" };
    int view = ( aview == 1 ? 0 : 1 );
    return GetWirePosition_FLUKA( cell, layer, viewTranslator[view] );
}




// bottom end coordinates of the wire
TVector3 TADCparGeo::GetWirePosition_FLUKA( int cell, int layer, string view ) {

    if ( cell < 0 || cell > m_nSense_x_Layer-1 ) {
        cout << "ERROR :: TADCparGeo.hxx::GetWirePosition_FLUKA >> wrong cell number input:  " << cell << endl;
        exit(0);
    }

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "GetWirePosition_FLUKA "<< cell << " " << layer << " " <<view << endl;
    // if ( GlobalPar::GetPar()->Debug() > 1 )     cout << "GetWirePosition_FLUKA "<< m_wireMatrix_YZ.size() << endl;

    int wire = m_sensePos[cell];
    TVector3 bottomEnd;

    if ( view == "x" || view == "U" || view == "SIDE" ) 
        m_wireMatrix_YZ[wire][layer]->GetPosition_FLUKA( bottomEnd );
    else if ( view == "y" || view == "V" || view == "TOP" ) 
        m_wireMatrix_XZ[wire][layer]->GetPosition_FLUKA( bottomEnd );
    else {
        cout << "ERROR :: TADCparGeo.hxx::GetWirePosition_FLUKA >> wrong wire view definition:  " << view << endl;
        exit(0);
    }

    return bottomEnd;

}





TVector3 TADCparGeo::GetWireLenght_FLUKA( int cell, int layer, int aview ) {
    vector<string> viewTranslator = { "SIDE", "TOP" };
    int view = ( aview == 1 ? 0 : 1 );
    return GetWireLenght_FLUKA( cell, layer, viewTranslator[view] );
}




TVector3 TADCparGeo::GetWireLenght_FLUKA( int cell, int layer, string view ) {

    if ( cell < 0 || cell > m_nSense_x_Layer-1 ) {
        cout << "ERROR :: TADCparGeo.hxx::GetWireLenght_FLUKA >> wrong cell number input:  " << cell << endl;
        exit(0);
    }

    int wire = m_sensePos[cell];
    TVector3 lenght;

    if ( view == "x" || view == "U" || view == "SIDE" ) 
        m_wireMatrix_YZ[wire][layer]->GetLenght_FLUKA( lenght );
    else if ( view == "y" || view == "V" || view == "TOP" ) 
        m_wireMatrix_XZ[wire][layer]->GetLenght_FLUKA( lenght );
    else {
        cout << "ERROR :: TADCparGeo.hxx::GetWireLenght_FLUKA >> wrong wire view definition:  " << view << endl;
        exit(0);
    }

    return lenght;

}





//_____________________________________________________________________________
void TADCparGeo::Global2Local( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
    *glob = *glob - m_center;
} 

//_____________________________________________________________________________
void TADCparGeo::Global2Local_RotationOnly( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
} 

//_____________________________________________________________________________
void TADCparGeo::Local2Global( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TADCparGeo::Local2Global_RotationOnly( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
} 


// // create material & media(this last is to transport the volume info...bah)
    // TGeoMaterial* aluminumMaterial;
    // TGeoMedium*   aluminum;

    // if ( (aluminumMaterial = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("ALUMINUM")) == 0x0 )
    //     aluminumMaterial = new TGeoMaterial("ALUMINUM", 0., 0., 0.);
    // // TGeoMixture *_vacuumMat = new TGeoMixture("vacuumMat",3);
    // // _vacuumMat->AddElement(14.01,7.,.78);
    // // _vacuumMat->AddElement(16.00,8.,.21);
    // // _vacuumMat->AddElement(39.95,18.,.01);
    // // _vacuumMat->SetDensity(1.2e-15);

    // if ( (aluminum = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("ALUMINUM")) == 0x0 )
    // aluminum = new TGeoMedium("ALUMINUM", 1, vacuum);



//_____________________________________________________________________________
TGeoVolume* TADCparGeo::GetVolume() {


    // create material   
//    TGeoMaterial *matAr = new TGeoMaterial("Argon", 39.948, 18., 0.001662);//densità viene da flair, 
//    TGeoMaterial *matC = new TGeoMaterial("Carbon", 12.0107, 6., 2.26);
//    TGeoMaterial *matO = new TGeoMaterial("Oxygen", 16., 8., 0.0013315);
//    TGeoMaterial *matAl = new TGeoMaterial("Aluminium", 26.981539, 13., 2.6989);
//    TGeoMaterial *matW = new TGeoMaterial("Tungsten", 183.84, 74., 19.3);// poi magari mettere la copertura in oro
//    TGeoMaterial *vacuum = new TGeoMaterial("Vacuum",0,0,0);//a,z,rho
   
//    TGeoMixture *matMylar = new TGeoMixture("Mylar",3,   1.39000    );
//     // matMylar->SetUniqueID(  18);
//     matMylar->DefineElement(0,12.01,6,0.624935);
//     matMylar->DefineElement(1,1.01,1,0.4204392E-01);
//     matMylar->DefineElement(2,16,8,0.3330211);

//    TGeoMixture *ArCO2 = new TGeoMixture("ArCO2",3);
//    ArCO2->AddElement(matAr ,1./4.);
//    ArCO2->AddElement(matC ,1./4.);
//    ArCO2->AddElement(matO ,2./4.);
// //   ArCO2->AddElement(matCO2 ,20.);
 
//    ArCO2->SetDensity(0.001677136); //da flair


   //create medium
   // TGeoMedium *vacuum_med = new TGeoMedium("vacuum_med",0, vacuum);
   // TGeoMedium *gas_med = new TGeoMedium("gas_med",1, ArCO2);
   // TGeoMedium *c_wire_med = new TGeoMedium("catod wire",2, matAl);
   // TGeoMedium *a_wire_med = new TGeoMedium("anod wire",3, matW);
   // TGeoMedium *a_maylar_med = new TGeoMedium("maylar foil",4, matMylar);  // metti maylar
   
    if ( GlobalPar::GetPar()->Debug() > 3 ) {
        cout << endl << " DC List of Materil\n ";
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

   // create main box
   // TGeoVolume *top = gGeoManager->MakeBox("top",gGeoManager->GetMedium("Vacuum_med"),m_width_x/2,m_height_y/2,m_length_z/2); //top è scatola che conterrà tutto (dimensioni in cm)
   TGeoVolume *top = gGeoManager->MakeBox("top",gGeoManager->GetMedium("ArCO2_med"),m_width_x/2,m_height_y/2,m_length_z/2); //top è scatola che conterrà tutto (dimensioni in cm)
   // gGeoManager->SetTopVolume(top);
   gGeoManager->SetTopVisible(1);

   //create gas box
   // TGeoVolume* airBox = gGeoManager->MakeBox("airBox", gGeoManager->GetMedium("ArCO2_med"), m_width_x/2., m_height_y/2., m_length_z/2.); //scatola di gas poi si dovrà mettere le rotazioni e le traslazioni, per ora centrato nell'origine.
   top->SetLineColor(8);
   
   
//sense
   TGeoVolume *c_x_wire = gGeoManager->MakeTubs("c_x_wire", gGeoManager->GetMedium("Aluminium_med"), 0, m_sense_radius, m_width_x/2, 0, 0 ); //filo catodo lungo z 
   c_x_wire->SetLineColor(kBlue);
   TGeoVolume *c_y_wire = gGeoManager->MakeTubs("c_y_wire", gGeoManager->GetMedium("Aluminium_med"), 0, m_sense_radius, m_height_y/2, 0, 0 ); 
   c_y_wire->SetLineColor(kRed);
// field
   TGeoVolume *a_x_wire = gGeoManager->MakeTubs("a_x_wire", gGeoManager->GetMedium("Tungsten_med"), 0, m_field_radius, m_width_x/2, 0, 0 ); //BISOGNERÀ POI SETTARE MEGLIO IL TUTTO
   a_x_wire->SetLineColor(kYellow);
   TGeoVolume *a_y_wire = gGeoManager->MakeTubs("a_y_wire", gGeoManager->GetMedium("Tungsten_med"), 0, m_field_radius, m_height_y/2, 0, 0 );  
   a_y_wire->SetLineColor(kGreen);   

   //set wires
   int c=0;
// cout << "TADCparGeo::GetVolume cazzo" << endl;
// cout << "TADCparGeo::GetVolume " << m_wireMatrix_YZ.size() << endl;
// cout << m_wireMatrix_YZ.at(0).size() << endl;
   for(unsigned int wire=0; wire<m_wireMatrix_YZ.size(); wire++ ){
     for (unsigned int lay =0; lay<m_wireMatrix_YZ.at(wire).size(); lay++ ){  
        //se è filo di sense 
         if( m_wireMatrix_XZ[wire][lay]->IsSense() ){
            // cout << "\tTADCparGeo::GetVolume sense " << wire << "  " << lay << endl;
            top->AddNode(a_x_wire, c++ , new TGeoCombiTrans(0, m_wireMatrix_YZ[wire][lay]->GetCenter().y(),  m_wireMatrix_YZ[wire][lay]->GetCenter().z(), new TGeoRotation("a_x_wire,",90.,90.,0.)));    
            }
         else{// filo di catodo
            // cout << "\tTADCparGeo::GetVolume " << wire << "  " << lay << endl;
            top->AddNode(c_x_wire, c++ , new TGeoCombiTrans(0, m_wireMatrix_YZ[wire][lay]->GetCenter().y(), m_wireMatrix_YZ[wire][lay]->GetCenter().z(), new TGeoRotation("c_x_wire,",90.,90.,0.)));  
            }
         }
      }

  for(unsigned int wire=0; wire<m_wireMatrix_XZ.size(); wire++ ){
     for (unsigned int lay =0; lay<m_wireMatrix_XZ.at(wire).size(); lay++ ){  
        //se è filo di sense 
         if( m_wireMatrix_XZ[wire][lay]->IsSense() ){
            // cout << "\tTADCparGeo::GetVolume Ysense " << wire << "  " << lay << endl;
            top->AddNode(a_y_wire, c++ , new TGeoCombiTrans( m_wireMatrix_XZ[wire][lay]->GetCenter().x(), 0,  m_wireMatrix_XZ[wire][lay]->GetCenter().z(), new TGeoRotation("a_y_wire,",0.,90.,0.)));
            }
         else{// filo di catodo
            // cout << "\tTADCparGeo::GetVolume " << wire << "  " << lay << endl;
            top->AddNode(c_y_wire, c++ , new TGeoCombiTrans(m_wireMatrix_XZ[wire][lay]->GetCenter().x(), 0, m_wireMatrix_XZ[wire][lay]->GetCenter().z(), new TGeoRotation("c_y_wire,",0.,90.,0.)));
            }
         }
      }

    // maylar
    TGeoVolume *maylarFoil = gGeoManager->MakeBox("maylarFoil",gGeoManager->GetMedium("Mylar_med"),m_width_x/2,m_height_y/2,m_maylar_thick/2); //top è scatola che conterrà tutto (dimensioni in cm)
    maylarFoil->SetLineColor(kMagenta);
    top->AddNode(maylarFoil, c++ , new TGeoCombiTrans( 0, 0,  -m_maylar_thick/2-m_length_z/2, new TGeoRotation("null,",0,0,0)));
    top->AddNode(maylarFoil, c++ , new TGeoCombiTrans( 0, 0,  m_maylar_thick/2+m_length_z/2, new TGeoRotation("null,",0,0,0)));


    // shield


    // //--- close the geometry
    // gGeoManager->CloseGeometry();

    //--- draw the ROOT box
    // cout << "--- draw the ROOT box" << endl;


//     gGeoManager->CloseGeometry();
//    TFile *outfile = TFile::Open("DCgeo.root","RECREATE");
// //   bm_geo->Write();
//    gGeoManager->Write();
//    outfile->Close();

    
    // gGeoManager->SetVisLevel(10);


    // cout << "cazzo0 XXX" << endl;
    // // top->Draw("ogl");
    // TCanvas* cazzo = new TCanvas("c", "c",  700, 700); 
    // top->Draw("");
    // cout << "cazzo1 XXX" << endl;
    // gGeoManager->CloseGeometry();
    // cout << "cazzo2 XXX" << endl;
    // TFile *outfile = TFile::Open("DCgeo.root","RECREATE");
    // cout << "cazzo3 XXX" << endl;
    // gGeoManager->Write();
    // cout << "cazzo4 XXX" << endl;
    // outfile->Close();
    // cout << "cazzo5 XXX" << endl;
    // cazzo->SaveAs("cazzo.png");
    // cazzo->SaveAs("cazzo.root");

    return top;
}





//_____________________________________________________________________________
void TADCparGeo::Print() {
  
    // cout << "m_wireMatrix_XZ.size() "<< m_wireMatrix_XZ.size() << endl;
    // cout << "m_wireMatrix_YZ.size() "<< m_wireMatrix_YZ.size() << endl;
    // cout << "m_wireMatrix_XZ.size() "<< m_wireMatrix_XZ[0].size() << endl;
    // cout << "m_wireMatrix_YZ.size() "<< m_wireMatrix_YZ[0].size() << endl;


    TCanvas* sideviewCanvas = new TCanvas("sideviewCanvas", "sideviewCanvas", 700, 700);
    TCanvas* topviewCanvas = new TCanvas("topviewCanvas", "topviewCanvas", 700, 700);

    int nWireSense = m_sensePos.size() * m_nLayers;
    int nWireField = m_nLayers * m_nWire_x_Layer - nWireSense;
    
    TGraph* sideviewGraph_sense = new TGraph( nWireSense );
    TGraph* sideviewGraph_field = new TGraph( nWireField );

    TGraph* topviewGraph_sense = new TGraph( nWireSense );
    TGraph* topviewGraph_field = new TGraph( nWireField );

    int senseCounter = 0;
    int fieldCounter = 0;
    for( int il=0; il<m_nLayers; il++ ) {
        for ( int iw=0; iw<m_nWire_x_Layer; iw++ ) {

            //sense
            if ( find( m_sensePos.begin(), m_sensePos.end(), iw ) != m_sensePos.end() ) {
                sideviewGraph_sense->SetPoint( senseCounter, m_wireMatrix_YZ[iw][il]->GetCenter().y(), 
                                            m_wireMatrix_YZ[iw][il]->GetCenter().z() );
                topviewGraph_sense->SetPoint( senseCounter, m_wireMatrix_XZ[iw][il]->GetCenter().x(), 
                                            m_wireMatrix_XZ[iw][il]->GetCenter().z() );
                senseCounter++;
            }
            else {  // field
                sideviewGraph_field->SetPoint( fieldCounter, m_wireMatrix_YZ[iw][il]->GetCenter().y(), 
                                            m_wireMatrix_YZ[iw][il]->GetCenter().z() );
                topviewGraph_field->SetPoint( fieldCounter, m_wireMatrix_XZ[iw][il]->GetCenter().x(), 
                                            m_wireMatrix_XZ[iw][il]->GetCenter().z() );
                fieldCounter++;
            }
        }
    }

    sideviewGraph_sense->SetMarkerColor(kOrange+2);
    sideviewGraph_sense->SetMarkerStyle(20);
    sideviewGraph_field->SetMarkerColor(kBlack);
    sideviewGraph_field->SetMarkerStyle(5);

    topviewGraph_sense->SetMarkerColor(kBlue-2);
    topviewGraph_sense->SetMarkerStyle(20);
    topviewGraph_field->SetMarkerColor(kBlack);
    topviewGraph_field->SetMarkerStyle(5);

    sideviewCanvas->cd();
    sideviewGraph_sense->Draw("AP");
    sideviewGraph_field->Draw("P same");
    sideviewCanvas->SaveAs("sideviewCanvas.png");

    topviewCanvas->cd();
    topviewGraph_sense->Draw("AP");
    topviewGraph_field->Draw("P same");
    topviewCanvas->SaveAs("topviewGraph.png");

}






// int TADCparGeo::ShiftBmon(){

//   TVector3 cen = GetCenter();

//   for(int il=0; il<NLAYERNEW;il++){
//     for (int iw =0; iw<NWIRELAYERNEW;iw++){

//       x_pos[iw][il][0]  += cen.X();
//       y_pos[iw][il][0]  += cen.Y();
//       z_pos[iw][il][0]  += cen.Z();

//       x_pos[iw][il][1]  += cen.X();
//       y_pos[iw][il][1]  += cen.Y();
//       z_pos[iw][il][1]  += cen.Z();
//     }
//   }
//   return 0 ;
// }


//  /*-----------------------------------------------------------------*/
// int TADCparGeo::RotateBmon(){

//   /*
//     Rotate the chamber by means of the Euler angles (in degrees) 
//     WE FOLLOW THE GOLDSTEIN CONVENTION for the Euler angles
//   */

//   const int nrows = 3, ncols = 3;
//   const double epsilon = 1.0e-14;
//   const int DBG = 0;

//   TVectorD PosWireIn,PosWireOut,DirWireIn,DirWireOut;
//   TMatrixD EulerRot(nrows,ncols);
//   PosWireIn.ResizeTo(nrows); PosWireIn.Zero();
//   PosWireOut.ResizeTo(nrows); PosWireOut.Zero();
//   DirWireIn.ResizeTo(nrows); DirWireIn.Zero();
//   DirWireOut.ResizeTo(nrows); DirWireOut.Zero();
  
//   double phi = GetAngles().X()*acos(-1)/180;
//   double theta = GetAngles().Y()*acos(-1)/180;
//   double psi = GetAngles().Z()*acos(-1)/180;

//   /* definizione della matrice di rotazione tramite angoli di Eulero */

//   EulerRot[0][0] = cos(phi)*cos(psi) - sin(phi)*cos(theta)*sin(psi);
//   EulerRot[0][1] = sin(phi)*cos(psi) + cos(phi)*cos(theta)*sin(psi);
//   EulerRot[0][2] = sin(theta)*sin(psi) ;
//   EulerRot[1][0] = -cos(phi)*sin(psi) - sin(phi)*cos(theta)*cos(psi);
//   EulerRot[1][1] = -sin(phi)*sin(psi) + cos(phi)*cos(theta)*cos(psi);
//   EulerRot[1][2] = sin(theta)*cos(psi) ;

//   EulerRot[2][0] = sin(phi)*sin(theta);
//   EulerRot[2][1] = -cos(phi)*sin(theta);
//   EulerRot[2][2] = cos(theta);

//   /*  clean up very tiny matrix elements due to roundings */

//   for (int nc =0; nc<ncols;nc++){       
//     for(int nr=0; nr<nrows;nr++){
//       if(fabs(EulerRot(nr,nc)) < epsilon) EulerRot(nr,nc) = 0 ;
//     }
//   }
  
//   if(DBG>0) {
//     cout<<" Beam Monitor Rotation: phi = "<<phi<<"  theta= "<<
//       theta<<"  psi= "<<psi<<endl<<endl;
//     cout<<"  Rotation Matrix= "<<endl<<endl;
//     cout<<"  "<<EulerRot(0,0)<<"  "<<EulerRot(0,1)<<"  "<<EulerRot(0,2)<<endl;
//     cout<<"  "<<EulerRot(1,0)<<"  "<<EulerRot(1,1)<<"  "<<EulerRot(1,2)<<endl;
//     cout<<"  "<<EulerRot(2,0)<<"  "<<EulerRot(2,1)<<"  "<<EulerRot(2,2)<<endl;
//   }
//   /*   rotate the U, SIDE view */
//   for(int il=0; il<NLAYERNEW;il++){
//     for (int iw =0; iw<NWIRELAYERNEW;iw++){       
      
//       PosWireIn(0) = x_pos[iw][il][0];
//       PosWireIn(1) = y_pos[iw][il][0];
//       PosWireIn(2) = z_pos[iw][il][0];
//       DirWireIn(0) = cx_pos[iw][il][0];
//       DirWireIn(1) = cy_pos[iw][il][0];
//       DirWireIn(2) = cz_pos[iw][il][0];
  
//       PosWireOut = EulerRot*PosWireIn;
//       DirWireOut = EulerRot*DirWireIn;

//       x_pos[iw][il][0] = PosWireOut(0);
//       y_pos[iw][il][0] = PosWireOut(1);
//       z_pos[iw][il][0] = PosWireOut(2);
//       cx_pos[iw][il][0] = DirWireOut(0);
//       cy_pos[iw][il][0] = DirWireOut(1);
//       cz_pos[iw][il][0] = DirWireOut(2);
//      }
//    }

//   /*   rotate the V, TOP view */
//   for(int il=0; il<NLAYERNEW;il++){
//     for (int iw =0; iw<NWIRELAYERNEW;iw++){       
      
//       PosWireIn(0) = x_pos[iw][il][1];
//       PosWireIn(1) = y_pos[iw][il][1];
//       PosWireIn(2) = z_pos[iw][il][1];
//       DirWireIn(0) = cx_pos[iw][il][1];
//       DirWireIn(1) = cy_pos[iw][il][1];
//       DirWireIn(2) = cz_pos[iw][il][1];
  
//       PosWireOut = EulerRot*PosWireIn;
//       DirWireOut = EulerRot*DirWireIn;

//       x_pos[iw][il][1] = PosWireOut(0);
//       y_pos[iw][il][1] = PosWireOut(1);
//       z_pos[iw][il][1] = PosWireOut(2);
//       cx_pos[iw][il][1] = DirWireOut(0);
//       cy_pos[iw][il][1] = DirWireOut(1);
//       cz_pos[iw][il][1] = DirWireOut(2);

//      }
//    }

//   return 0;
// }








//_____________________________________________________________________________
TEveGeoShapeExtract* TADCparGeo::AddExtractBM(const char *bmName ) {

   TGeoBBox* tube = new TGeoBBox(bmName, TAGgeoTrafo::CmToMu()*GetWidth()/2., TAGgeoTrafo::CmToMu()*GetHeigth()/2., 
                                           TAGgeoTrafo::CmToMu()*GetLength()/2.);
   TColor* color = gROOT->GetColor(17);
   Float_t rgba[4];
   color->GetRGB(rgba[0], rgba[1], rgba[2]);
   rgba[3] = TAGgeoTrafo::GetDefaultTransp()/100.;
   
   TEveGeoShapeExtract* tubeExtract = new TEveGeoShapeExtract(bmName);
   tubeExtract->SetShape(tube);
   tubeExtract->SetRGBA(rgba);
   
   return tubeExtract;
}






//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TADCparGeo::Clear(Option_t*)
{
  for( unsigned int i=0; i<m_sensePos.size(); i++) {
    m_sensePos[i] = 0;
  }
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TADCparGeo::ToStream(ostream& os, Option_t*) const
{
  os << "TADCparGeo " << GetName() << endl;
  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}






























