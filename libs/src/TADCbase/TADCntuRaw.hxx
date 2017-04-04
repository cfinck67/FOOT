#ifndef _TADCntuRaw_HXX
#define _TADCntuRaw_HXX
/*!
  \file
  \version $Id: adapted from TABMntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TADCntuRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"
#include "TADCparGeo.hxx"


// GenFit
#include "FieldManager.h"
#include "MaterialEffects.h"
#include "TGeoMaterialInterface.h"
#include "ConstField.h"
#include "Track.h"
#include "WireMeasurementNew.h"

#include "TGeoManager.h"
#include "KalmanFitterRefTrack.h"
#include "KalmanFitter.h"

#include "AbsMeasurement.h"
#include "MeasurementCreator.h"

using namespace std;
using namespace genfit;


class TADCntuHit : public TObject {
  public:
    TADCntuHit();
    TADCntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm);
    virtual         ~TADCntuHit();

    void            SetData(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm);
    Int_t           Cell() const;
    Int_t           Plane() const;
    Int_t           View() const;
    Double_t X() const;
    Double_t Y() const;
    Double_t Z() const;
    TVector3 Position() const;
    TVector3 Momentum() { return TVector3( pxcadc, pycadc, pzcadc ); };
    Double_t Dist() const;
    Double_t Tdrift() const;
    Double_t Timdc() const;
    Bool_t HorView() const; //Horizontal, Top, XZ == -1
    Bool_t VertView() const; //Vertical, Side, YZ == 1

    double Px() { return pxcadc; };
    double Py() { return pycadc; };
    double Pz() { return pzcadc; };

    

    //Track Related Paramters
    int TrkAss() {return itrkass;};
    Int_t GetMCId() {return iddc;};
    Double_t GetRho() {return rho;};
    Double_t GetChi2() {return ichi2;};
    Double_t GetSigma() {return sigma;};
    TVector3 GetA0() {return A0;};
    TVector3 GetWvers() {return Wvers;};

    // Get generated particle quantities
    //  provvisorio
    int m_genPartID;
    int m_genPartFLUKAid;
    int m_genPartCharge;
    int m_genPartBarionNum;
    float m_genPartMass;
    TVector3 m_genPartPosition;
    TVector3 m_genPartMomentum;
  
    void SetAW(TADCparGeo *f_dcgeo);
    void SetTrkAss(Int_t in_ass) { itrkass = in_ass;};
    void SetMCId(Int_t in_id) { iddc = in_id;};
    void SetRho(Double_t in_rho) { rho = in_rho;};
    void SetRdrift(TVector3 pos, TVector3 dir);
    void SetSigma(Double_t sig) {sigma = sig;};
    ////////   Frank'd say useless with Kalman
    void SetChi2(Double_t in_chi2) { ichi2 = in_chi2;};
    void SetPca(TVector3 in_pca) { pca = in_pca;};
    void SetPcaWire(TVector3 in_pca) { pca_wire = in_pca;};
    ////////////////////////////////////////////


    // Frank
    void SetGeneratedParticleInfo ( int genPartID, int genPartFLUKAid, int genPartCharge,
                        int genPartBarionNum, float genPartMass,
                        TVector3 genPartPosition,
                        TVector3 genPartMomentum ) {
        m_genPartID = genPartID;
        m_genPartFLUKAid = genPartFLUKAid;
        m_genPartCharge = genPartCharge;
        m_genPartBarionNum = genPartBarionNum;
        m_genPartMass = genPartMass;
        m_genPartPosition = genPartPosition;
        m_genPartMomentum = genPartMomentum;
    };


  ClassDef(TADCntuHit,1)

  private:
    Int_t iddc;
    Int_t iview;    
    Int_t ilayer;    
    Int_t icell;
    Int_t itrkass;
    Double_t ichi2;
    Double_t xcadc;    
    Double_t ycadc;    
    Double_t zcadc;
    Double_t pxcadc;   
    Double_t pycadc;   
    Double_t pzcadc;
    Double_t rdrift;
    Double_t tdrift;
    Double_t timdc;
    Double_t sigma;

    //Track related params
    TVector3  A0;
    TVector3  Wvers;
    TVector3  m_trueP;

    Double_t  rho;
    TVector3  pca;
    TVector3  pca_wire;

    // // link generated particle quantities
    // int m_genPartID;
    // int m_genPartFLUKAid;
    // int m_genPartCharge;
    // int m_genPartBarionNum;
    // float m_genPartMass;
    // TVector3 m_genPartPosition;
    // TVector3 m_genPartMomentum;

};

//##############################################################################

class TADCntuRaw : public TAGdata {
  public:

                    TADCntuRaw();
    virtual         ~TADCntuRaw();

    TADCntuHit*       Hit(Int_t i_ind);
    const TADCntuHit* Hit(Int_t i_ind) const;

    void FillWithMeasurementPoints( vector<AbsMeasurement*> & hit_vect, TADCparGeo* dc_geo );
    void FillWithMeasurementPoints_Test( vector<AbsMeasurement*> & hit_vect, TADCparGeo* dc_geo);
    void FillWithMeasurementTypes( vector<eMeasurementType> & hit_vect );

    int NHit() { return nhit; };

    virtual void    SetupClones();

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TADCntuRaw,1)

  public:
    Int_t           nhit;		    // 
    TClonesArray*   h;			    // hits
};

#include "TADCntuRaw.icc"

#endif
