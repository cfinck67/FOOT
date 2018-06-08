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

#include "GlobalPar.hxx"
#include "TAGpara.hxx"
#include "foot_geo.h"
#include "TABMparMap.hxx"


//##############################################################################
class TABMparCon : public TAGpara {
  public:

                    TABMparCon();
    virtual         ~TABMparCon();

    //setters
    void SetVDrift(Double_t v){vdrift=v; return;};
    void SetIsMC(bool ism){m_isMC=ism; return;};
    void SetRdriftCut(Double_t Rdcut){rdrift_cut=Rdcut; return;};
    void SetEnxcellcut(Double_t Encut){enxcell_cut=Encut; return;};
    
    //getters
    bool     IsMC(){return m_isMC;};
    Double_t GetVDrift(){return vdrift;};
    Double_t GetRdriftCut(){return rdrift_cut;};
    Double_t GetEnxcellcut(){return enxcell_cut;};
    Double_t GetChi2Redcut(){return chi2red_cut;};
    Double_t GetAngZCut(){return angz_cut;};
    Double_t GetAngZRescut(){return angzres_cut;};
    Int_t GetFitterIndex(){return fitter_index;};
    Int_t GetBMdebug(){return bm_debug;};
    Int_t GetBMvietrack(){return bm_vietrack;};
    //~ string GetBMdataFileName(){return datafile_name;};
    Int_t GetBMcharge(){return part_in_charge;};
    Double_t GetBMmom(){return part_in_mom;};
    Int_t GetTdcMaxcha(){return tdc_maxcha;};

    //acquisition stuff
    //~ void        CalculateT0(TABMparMap* o_bmmap, TString datafile_name);//calculate the T0
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
  
    void LoadReso(TString sF);
    double ResoEval(double dist);

    Bool_t FromFile(const TString& name);

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    ClassDef(TABMparCon,1)

  private:

    bool m_isMC;
    Double_t vdrift;
    Double_t rdrift_cut;
    Double_t enxcell_cut;
    Double_t chi2red_cut;
    Double_t angz_cut;
    Double_t angzres_cut;
    Int_t    fitter_index;
    Int_t    bm_debug;//index for the bm_debug, (it is set at least to the global debug level)
    Int_t    bm_vietrack;//every bm_vietrack events SHOE will plot the BM track
    //~ string   datafile_name;//name of the input data file 
    Int_t    part_in_charge;//for BM Genfit tracking
    Double_t part_in_mom;//for BM Genfit tracking
    Int_t    total_ev_num;//total number of events
    Int_t    tdc_maxcha;//tdc number of channel
    
    //old framework stuff...:
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
