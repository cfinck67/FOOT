#ifndef _TABMntuRaw_HXX
#define _TABMntuRaw_HXX
/*!
  \file
  \version $Id: TABMntuRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TABMntuRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"
#include "TABMparGeo.hxx"

class TABMntuHit : public TObject {
  public:
    TABMntuHit();
    TABMntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm);
    virtual         ~TABMntuHit();

    void            SetData(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm);
    Int_t           Cell() const;
    Int_t           Plane() const;
    Int_t           View() const;
    Double_t X() const;
    Double_t Y() const;
    Double_t Z() const;
    TVector3 Position() const;
    Double_t Dist() const;
    Double_t Tdrift() const;
    Double_t Timmon() const;
    Bool_t HorView() const; //Horizontal, Top, XZ == -1
    Bool_t VertView() const; //Vertical, Side, YZ == 1

    //Track Related Paramters
    void SetAW(TABMparGeo *f_bmgeo);
    int TrkAss() {return itrkass;};
    Double_t GetRho() {return rho;};
    Double_t GetChi2() {return ichi2;};
    Double_t GetSigma() {return sigma;};
    TVector3 GetA0() {return A0;};
    TVector3 GetWvers() {return Wvers;};
  
    void SetTrkAss(Int_t in_ass) { itrkass = in_ass;};
    void SetRho(Double_t in_rho) { rho = in_rho;};
    void SetChi2(Double_t in_chi2) { ichi2 = in_chi2;};
    void SetPca(TVector3 in_pca) { pca = in_pca;};
    void SetPcaWire(TVector3 in_pca) { pca_wire = in_pca;};
    void SetSigma(Double_t sig) {sigma = sig;};

  ClassDef(TABMntuHit,1)

  private:
    Int_t idmon;
    Int_t iview;    
    Int_t ilayer;    
    Int_t icell;
    Int_t itrkass;
    Double_t ichi2;
    Double_t xcamon;    
    Double_t ycamon;    
    Double_t zcamon;
    Double_t pxcamon;   
    Double_t pycamon;   
    Double_t pzcamon;
    Double_t rdrift;
    Double_t tdrift;
    Double_t timmon;
    Double_t sigma;

    //Track related params
    TVector3  A0;
    TVector3  Wvers;

    Double_t  rho;
    TVector3  pca;
    TVector3  pca_wire;

};

//##############################################################################

class TABMntuRaw : public TAGdata {
  public:

                    TABMntuRaw();
    virtual         ~TABMntuRaw();

    TABMntuHit*       Hit(Int_t i_ind);
    const TABMntuHit* Hit(Int_t i_ind) const;

    virtual void    SetupClones();

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

    ClassDef(TABMntuRaw,1)

  public:
    Int_t           nhit;		    // 
    TClonesArray*   h;			    // hits
};

#include "TABMntuRaw.icc"

#endif
