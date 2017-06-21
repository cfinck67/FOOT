#ifndef _TADCparGeo_HXX
#define _TADCparGeo_HXX
/*!
  \file
  \version $Id: TADCparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TADCparGeo.
*/
/*------------------------------------------+---------------------------------*/

#include "TEveGeoShapeExtract.h"
#include "TString.h"
#include "TVector3.h"
#include "TRotation.h"

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"

#include "TAGpara.hxx"

// #include "foot_geo.h"
// #include "TAGtinCan.h"
#include "TinCan.hxx"

//##############################################################################
class TGeoVolume;
class TADCparGeo : public TAGpara {

typedef vector< vector< TinCan* > > CanMatrix;

  public:

    TADCparGeo();
    virtual ~TADCparGeo() {};


    void InitGeo();
    int Rotate( TVector3 angle ) {return 0;};
    int Shift( TVector3 dir ) {return 0;};


    TVector3 GetCenter()  {return m_center;};
    TVector3 GetAngles() {return m_tilt_eulerAngle;};

    double        GetWidth() { return m_width_x; };
    double        GetLength() { return m_length_z; };
    double        GetHeigth() { return m_height_y; };

    double  GetShieldLength()  {return m_shield_length;};
    double  GetShieldThick()   {return m_shield_thick;};
    double  GetMaylarThick()   {return m_maylar_thick;};

    int     GetLayersNumber()   {return m_nLayers;};     // outdated!!!
    int     GetNLayers()        { return m_nLayers; };
    int     NWire_x_Layer()     {return m_nWire_x_Layer;};
    int     NSense_x_Layer()    {return m_nSense_x_Layer;};

    // TVector3 GetWirePosition(int wire, int layer, string view ) {};
    // TVector3 GetWireLength(int wire, int layer, string view ) {};
    // get also double?

    
    void InitMatrix_Can( CanMatrix& neo, int row, int col ) {

        neo.resize( row, vector< TinCan* >(col) );
        
        for ( int i=0; i<row; i++ ) 
            for ( int j=0; j<col; j++ ) 
                neo[i][j] = new TinCan();

    }
    

    //X,Y,Z as a function of wire, plane, view
    // position vector of the bottom end of the wire, FLUKA convention
    /**************CANCEL!!!!!!!!!!!!!!!!!******************************************************/
    double        GetX(int w, int p, int v){return 0;};
    double        GetY(int w, int p, int v){return 0;};
    double        GetZ(int w, int p, int v){return 0;};

    // wire lenght vector
    double        GetCX(int w, int p, int v){return 0;};
    double        GetCY(int w, int p, int v){return 0;};
    double        GetCZ(int w, int p, int v){return 0;};
    void        GetCellInfo(Int_t view, Int_t plane, Int_t cellID, 
                            Double_t& h_x, Double_t& h_y, Double_t& h_z, 
                            Double_t& h_cx, Double_t& h_cy, Double_t& h_cz) {};
    int GetCellsNumber(){return 0;};
    /******************************************************************************************/
    void Print();

    // double GetCellLength() { return 2*m_wire_step_z;};  // along z
    int GetID(int cell){
        if ( cell >= (int)m_sensePos.size() )    return -1;
        return m_sensePos[ cell ];
    };
    double GetCellHeight() { return 2*m_wire_step_z;};  // along z
    double GetCellWidth() { return 2*m_wire_passo_t;};  // along x or y

    void GetWireEndPoints( int cell, int layer, string view, TVector3 & end1, TVector3 & end2 );
    
    TVector3 GetWirePosition_FLUKA( int cell, int layer, int aview );
    TVector3 GetWirePosition_FLUKA( int cell, int layer, string view );
    
    TVector3 GetWireLenght_FLUKA( int cell, int layer, int aview );
    TVector3 GetWireLenght_FLUKA( int cell, int layer, string view );
    
    TGeoVolume*     GetVolume();   

    TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };

    void Global2Local( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );
    void Local2Global( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );

    // Not used by Frank
    /**************CANCEL!!!!!!!!!!!!!!!!!*************************/
    // void        SetAngles(TVector3 angle);
    // void        SetShift(TVector3 dir);
    TVector3 GetSide() { 
        TVector3 fuffa;
        return fuffa; 
    };
    /**************************************************************/
    TEveGeoShapeExtract* AddExtractBM(const char *bmName = "BM");
    virtual void    Clear(Option_t* opt="");
    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
    ClassDef(TADCparGeo,1)


  private:

    CanMatrix m_wireMatrix_XZ;
    CanMatrix m_wireMatrix_YZ;
    // map<string, TGeoMaterial*> m_MaterialCollection;

    vector<int> m_sensePos;

    /*  cordinates of the Beam Monitor center */ //middle of beam monitor
    TVector3 m_origin;
    TVector3 m_center;

    /* parametri geometrici per la nuova camera a drift di monitor del beam */
    double m_width_x;              //width of beam monitor (gas)
    double m_height_y;            //height of beam monitor (gas) 
    double m_length_z;            //length of beam monitor (gas)
    TVector3 m_Dimension_root;

    // shield
    double m_shield_length;     //length (along z) of beam monitor shield
    double m_shield_thick;      //thickness of beam monitor shield
    double m_maylar_thick;      //mylar windows thickness


    int m_nLayers;                 //no. of layers
    int m_nWire_x_Layer;      //no. of wires (field or sense) per layer
    int m_nSense_x_Layer;     //no. of sense wires (and also cells) per layers

    double m_wire_step_z;             //distance between wires (z distance)
    double m_wire_passo_t;           //distance between wires (x or y distance)
    double m_layerDistance;        //distance between layers U and V

    double m_sense_radius;        //radius of sensitive wires//ATTENZIONE:in first erano 0.003
    double m_field_radius;        //radius of field wires//ATTENZIONE:in first erano 0.008

    string m_material_shield;
    string m_material_maylar;
    string m_material_gas;
    string m_material_wire_S;
    string m_material_wire_F;

    /*  shift dei fili rispetto ai lati della camera (-0.8 in x,y if you change senseid)*/
    TVector3 m_delta_shift;

    /* Euler angles that defines the BEAM monitor orientation ( degrees)*/
    TVector3 m_tilt_eulerAngle;  

    TRotation* m_rotation;

};

#include "TADCparGeo.icc"

#endif


















 
















