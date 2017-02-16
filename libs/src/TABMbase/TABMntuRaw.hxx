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
    TVector3 Momentum() const;
    Double_t Dist() const;
    Double_t Tdrift() const;
    Double_t Timmon() const;
    Bool_t HorView() const; //Horizontal, Top, XZ == -1
    Bool_t VertView() const; //Vertical, Side, YZ == 1

    //Getters
    void SetAW(TABMparGeo *f_bmgeo);
    Int_t TrkAss() {return itrkass;};
    Double_t GetRho() {return rho;};
    Double_t GetChi2() {return ichi2;};
    Double_t GetSigma() {return sigma;};
    Double_t GetIdmon() {return idmon;};
    TVector3 GetA0() {return A0;};
    TVector3 GetWvers() {return Wvers;};
    TVector3 GetPca() {return pca;};
    TVector3 GetPosm2() {return posm2;};

    void SmearRdrift(Int_t smear_type); //to smear rdrift with resolution, use it ONLY for MC events!

    //setters
    void SetTrkAss(Int_t in_ass) { itrkass = in_ass;};
    void SetRho(Double_t in_rho) { rho = in_rho;};
    void SetChi2(Double_t in_chi2) { ichi2 = in_chi2;};
    void SetPca(TVector3 in_pca) { pca = in_pca;};
    void SetSigma(Double_t in_sigma) {sigma = in_sigma;};
    void SetRdrift(Double_t in_rdrift){rdrift=in_rdrift;};
    void SetPosm2(TVector3 in_posm2){posm2=in_posm2;};

  ClassDef(TABMntuHit,1)

  private:
    Int_t idmon; //idmon==1 -> hit from primary particle
    Int_t iview;    
    Int_t ilayer;    
    Int_t icell;
    Double_t ichi2;
    Int_t itrkass;   
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
    TVector3  A0;    //posizione filo di anodo che dovrebbe essere interessato
    TVector3  Wvers;

    Double_t  rho;
    TVector3  pca;
    
    //MC parameters
    TVector3 posm2;  //ci salvo la posizione di uscita reale del primario nel suo ultimo hit,\
                       se il primario non esce, posm2=(0,0,99) \
                       lo uso per checkMC(), almeno all'inizio che non ho un event display.. \
                       potrei farne a meno visto che è solo controllo in più... \
                       fatto in coordinate locali!
                       

    
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
