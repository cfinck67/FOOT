#ifndef _TATRparGeo_HXX
#define _TATRparGeo_HXX
/*!
  \file
  \version $Id: TATRparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TATRparGeo.
*/
/*------------------------------------------+---------------------------------*/

#include "TString.h"
#include "TVector3.h"

#include "TAGpara.hxx"

//##############################################################################

class TATRparGeo : public TAGpara {
  public:

                    TATRparGeo();
    virtual         ~TATRparGeo();

    Double_t        GetWidth();
    Double_t        GetLength();
    Double_t        GetHeigth();

    //Id sense as function of cell
    Int_t           GetID(int cell);

    //X,Y,Z as a function of wire, plane, view
    Double_t        GetX(int w, int p, int v);
    Double_t        GetY(int w, int p, int v);
    Double_t        GetZ(int w, int p, int v);

    Double_t        GetCX(int w, int p, int v);
    Double_t        GetCY(int w, int p, int v);
    Double_t        GetCZ(int w, int p, int v);

    Int_t GetLayersNumber();
    Int_t GetCellsNumber();

    Double_t GetCellHeight();
    Double_t GetCellWidth();

    void InitGeo();

    void        SetWidth(double wid);
    void        SetLength(double len);
    void        SetHeigth(double hei);

    void        SetCenter(TVector3 h_vec);
    TVector3    GetCenter();

    void        SetSide(TVector3 h_vec);
    TVector3    GetSide();

    void        SetDelta(TVector3 h_vec);
    TVector3    GetDelta();

    void        SetAngles(TVector3 h_vec);
    TVector3    GetAngles();

    void        SetDirection(TVector3 dir);
    TVector3    GetDirection();


    void        GetCellInfo(Int_t view, Int_t plane, Int_t cellID, 
			    Double_t& h_x, Double_t& h_y, Double_t& h_z, 
			    Double_t& h_cx, Double_t& h_cy, Double_t& h_cz);

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    ClassDef(TATRparGeo,1)

  private:

    Int_t NWIRELAYERNEW;
    Int_t NLAYERNEW;
    Int_t NSENSENEW;

    Double_t  RSENSE;
    Double_t  RFIELD;

    Double_t bm_step;
    Double_t bm_cellwide;
    Double_t bm_dplane;

    Double_t  BMHEIGHTNEW;
    Double_t  BMWIDTHNEW ;
    Double_t  BMLENGHTNEW;

    /*  cordinates of the Beam Monitor center */
    Double_t  XMONNEW; 
    Double_t  YMONNEW; 
    Double_t  ZMONNEW; // 51 cm + half leght away from origin 
    
    /* Euler angles that defines the BEAM monitor orientation ( degrees)*/
    Double_t  EULER1MONNEW;
    Double_t  EULER2MONNEW;
    Double_t  EULER3MONNEW;
    
    /*  shift dei fili rispetto ai lati della camera */
    Double_t  DELTAZNEW; 
    Double_t  DELTAYNEW;
    Double_t  DELTAXNEW;

    Int_t bm_idsense[3];

    Int_t bm_nlayer;
    Int_t bm_nwirelayer;
    Double_t bm_senserad;
    Double_t bm_fieldrad;

    TVector3 bm_CenterDch;    /* Chamber center positioning */
    TVector3 bm_SideDch;      /* Chamber side dimensions */
    TVector3 bm_DeltaDch;     /* displacement of 1st wire wrt chmb side */
    TVector3 bm_AnglesDch;    /* Euler chamb. rotations angles */  
    TVector3 bm_Direction;    /* Chamber direction (wrt z) */  

    //x,y,z center positions of the wires and dimensions
    Double_t x_pos[50][6][2];
    Double_t y_pos[50][6][2];
    Double_t z_pos[50][6][2];

    Double_t cx_pos[50][6][2];
    Double_t cy_pos[50][6][2];
    Double_t cz_pos[50][6][2];

};

#include "TATRparGeo.icc"

#endif
