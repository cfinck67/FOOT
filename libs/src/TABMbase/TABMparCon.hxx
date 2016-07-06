#ifndef _TABMparCon_HXX
#define _TABMparCon_HXX
/*!
  \file
  \version $Id: TABMparCon.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TABMparCon.
*/
/*------------------------------------------+---------------------------------*/

#include "TString.h"
#include "TVector3.h"
#include "TSpline.h"
#include "TF1.h"
#include "TH1.h"

#include <vector>

#include "TAGpara.hxx"


//##############################################################################
class TABMparCon : public TAGpara {
  public:

                    TABMparCon();
    virtual         ~TABMparCon();


    void        SetVDrift(Double_t v);
    Double_t    GetVDrift();

    void        loadT0s(const TString& name);
    void        SetT0s(vector<double> t0s);
    Double_t    GetT0(int view, int plane, int cell);

    void LoadSTrel(TString sF);
    double STrelCorr(double time, int ic, int ip, int iv);
    double STrel_Delta1(double time);
    double STrel_Delta2(double time);
    double STrel_Delta3(double time);

    TF1* GetCalibY();
    TF1* GetCalibX();
    void ConfigureTrkCalib();
    void SetIsMC(bool ism);
    bool IsMC();

    void LoadReso(TString sF);
    double ResoEval(double dist);

    Bool_t FromFile(const TString& name);

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    ClassDef(TABMparCon,1)

  private:

    bool m_isMC;
    Double_t vdrift;
    vector<double> v_t0s;

    TF1* f_mypol;
    TF1* f_mypol2;
    TSpline3 *m_mySpl;
    vector <TSpline3*> m_myVSpl;
    TF1 *m_myFunSpl;
    vector <TF1*> m_myVFunSpl;
    TH1D *my_hreso;
  
};

#endif
