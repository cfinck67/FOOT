#ifndef _TABMntuHit_HXX
#define _TABMntuHit_HXX

#include "TAGdata.hxx"
#include "TABMparGeo.hxx"

#include "TObject.h"
//~ #include "TClonesArray.h"
#include "TRandom3.h"
#include "TString.h"
#include "TAGdata.hxx"

#include <vector>
#include <string.h>
#include <fstream>
#include <bitset>
#include <algorithm>

using namespace std;

class TABMntuHit : public TAGdata {
  public:
    TABMntuHit();
    TABMntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t s);
    virtual         ~TABMntuHit();

    //~ void            SetData(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t,Double_t s);
    Int_t           Cell() const;
    Int_t           Plane() const;
    Int_t           View() const;
    Double_t X() const;//return xcamoon
    Double_t Y() const;
    Double_t Z() const;
    TVector3 Position() const; //return (xcamon, ycamon, zcamon)
    TVector3 Momentum() const;
    Double_t Dist() const; //return rdrift
    Double_t Tdrift() const;
    //~ Double_t Timmon() const;
    

    //Getters
    void SetAW(TABMparGeo *f_bmgeo);
    Double_t GetRho() {return rho;};
    Double_t GetChi2() {return ichi2;};
    Double_t GetSigma() {return sigma;};
    Double_t GetIdmon() {return idmon;};
    TVector3 GetA0() {return A0;};
    TVector3 GetWvers() {return Wvers;};
    TVector3 GetPca() {return pca;};
    Double_t GetResidual(){return residual;};
    bool GetIsSelected() {return isSelected;};
    bool GetIsCrossTalk() {return isCrossTalk;};
    Double_t GetRealRdrift(){return realRdrift;};
    Double_t GetRdriftSmear(){return fabs(realRdrift-rdrift);};

    void SmearRdrift(Int_t smear_type); //to smear rdrift with resolution, use it ONLY for MC events!

    //setters
    void SetRho(Double_t in_rho) { rho = in_rho;};
    void SetChi2(Double_t in_chi2) { ichi2 = in_chi2;};
    void SetPca(TVector3 in_pca) { pca = in_pca;};
    void SetSigma(Double_t in_sigma) {sigma = in_sigma;};
    void SetRdrift(Double_t in_rdrift){rdrift=in_rdrift;};
    void SetRealRdrift(Double_t in_rdrift){realRdrift=in_rdrift;};
    void SetIsSelected(bool in_is){isSelected=in_is;};
    void SetResidual(Double_t res_in){residual=res_in*sigma;};
    
    //old software
    //~ void SetTrkAss(Int_t in_ass) { itrkass = in_ass;};
    //~ Int_t TrkAss() {return itrkass;};
    //~ Bool_t HorView() const; //Horizontal, Top, XZ == -1
    //~ Bool_t VertView() const; //Vertical, Side, YZ == 1

  ClassDef(TABMntuHit,1)

  private:
    Int_t idmon; //idmon==1 -> hit from primary particle, only for MC
    Int_t iview;    
    Int_t ilayer;    
    Int_t icell;
    Double_t ichi2;
    Double_t xcamon;    
    Double_t ycamon;    
    Double_t zcamon;
    Double_t pxcamon;   
    Double_t pycamon;   
    Double_t pzcamon;
    Double_t rdrift;
    Double_t tdrift;//hit.time - T0 - irtime (irtime is the trigger time/start counter time) 
    //~ Double_t timmon;//hit.time - irtime
    Double_t sigma;//rdrift resolution
    Double_t residual;//difference between rdrift and the fitted rdrift
 
    //Track related params
    TVector3  A0;    //posizione filo di anodo che dovrebbe essere interessato
    TVector3  Wvers;

    Double_t  rho; //old stuff
    TVector3  pca; //old stuff
    
    bool      isSelected;   //isSelected==true means that this hit is in the best track 
    
    //MC parameters
    Double_t  realRdrift;   //real rdrift, not smeared, only for MC 
    bool      isCrossTalk;   //true =  the hit is generated by CreateCrossTalk (not implemented yet)
    
    //old tracking software
    //~ Int_t itrkass;   //equivalente di isSelected
};


#include "TABMntuHit.icc"

#endif


