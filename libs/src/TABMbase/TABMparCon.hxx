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
    void SetBMdebug(Int_t deb_in){bm_debug=deb_in; return;};
    void SetIsMC(Bool_t ism){m_isMC=ism; return;};
    void SetRdriftCut(Double_t Rdcut){rdrift_cut=Rdcut; return;};
    void SetEnxcellcut(Double_t Encut){enxcell_cut=Encut; return;};
    
    //getters
    Bool_t   IsMC(){return m_isMC;};
    Double_t GetRdriftCut(){return rdrift_cut;};
    Double_t GetEnxcellcut(){return enxcell_cut;};
    Double_t GetChi2Redcut(){return chi2red_cut;};
    Int_t GetPlanehitcut(){return planehit_cut;};
    Int_t GetMinnhit_cut(){return minnhit_cut;};
    Int_t GetMaxnhit_cut(){return maxnhit_cut;};
    Double_t GetAngZCut(){return angz_cut;};
    Double_t GetAngZRescut(){return angzres_cut;};
    Int_t GetRejmaxcut(){return rejmax_cut;};
    Int_t GetFitterIndex(){return fitter_index;};
    Int_t GetPrefitEnable(){return prefit_enable;};
    Int_t GetBMdebug(){return bm_debug;};
    Int_t GetBMvietrack(){return bm_vietrack;};
    //~ string GetBMdataFileName(){return datafile_name;};
    Int_t GetBMcharge(){return part_in_charge;};
    Double_t GetBMmom(){return part_in_mom;};
    Int_t GetmanageT0BM(){return manageT0BM;};
    Int_t GetT0switch(){return t0_switch;};
    Int_t GetmanageADCped(){return manageADCped;};
    string GetParmapfile(){return parmapfile;};
    Int_t GetCalibro(){return calibro;};
    Int_t GetNumIte(){return num_ite;};
    Double_t GetParMove(){return par_move;};
    Int_t GetSmearhits(){return smearhits;};
    Double_t GetFakehitsMean(){return fakehits_mean;};
    Double_t GetFakehitsSigma(){return fakehits_sigma;};
    Double_t GetMCEffMean(){return mceff_mean;};
    Double_t GetMCEffSigma(){return mceff_sigma;};
    Int_t GetSmearrdrift(){return smearrdrift;};
    //~ Double_t GetXShift(){return meas_shift.X();};
    //~ Double_t GetYShift(){return meas_shift.Y();};
    //~ Double_t GetZShift(){return meas_shift.Z();};
    //~ Double_t GetXrot(){return meas_tilt.X();};
    //~ Double_t GetYrot(){return meas_tilt.Y();};
    //~ Double_t GetZrot(){return meas_tilt.Z();};


    //T0 stuff
    void        PrintT0s(TString &input_file_name, Long64_t);
    Bool_t      loadT0s(Long64_t); 
    void        SetT0s(vector<Double_t> t0s);
    void        SetT0(Int_t cha, Double_t t0in);   
    const Double_t    GetT0(Int_t view, Int_t plane, Int_t cell){return GetT0(cell+((view==-1) ? 1:0)*3+plane*6);};
    const Double_t    GetT0(Int_t index_in){return (index_in<36 && index_in>-1) ? v_t0s[index_in]:-1000;};
    void        CoutT0();
    
    //ADC stuff
    void        PrintADCped(TString &input_file_name, Long64_t tot_num_ev);    
    void        loadADCped(Int_t mapcha);
    void        SetADCchanum(Int_t cha);
    void        SetADCped(Int_t cha, Double_t pedin, Double_t rmsin);
    void        CoutADCped();
    Double_t    GetADCped(Int_t cha){return (cha<adc_ped_mean.size()) ? adc_ped_mean[cha]:10000.;};
    Double_t    GetADCrms(Int_t cha){return (cha<adc_ped_rms.size()) ? adc_ped_rms[cha]:10000.;};
    
    //strel stuff
    void LoadSTrel(TString sF);
    Double_t FirstSTrel(Double_t tdrift);
    Double_t InverseStrel(Double_t rdrift);
    Double_t FirstSTrelMC(Double_t tdrift, Int_t mc_switch);    
    //~ Double_t STrelCorr(double time, int ic, int ip, int iv);//old, included in FirstSTrel
    //~ double STrel_Delta1(double time);//old, included in FirstSTrel
    //~ double STrel_Delta2(double time);//old, included in FirstSTrel
    //~ double STrel_Delta3(double time);//old, included in FirstSTrel

    //old calibration methods
    //~ TF1* GetCalibY();
    //~ TF1* GetCalibX();
    //~ void ConfigureTrkCalib();
  
    void LoadReso(TString sF);
    Double_t ResoEval(Double_t dist);

    Bool_t FromFile(const TString& name);

    virtual void    Clear(Option_t* opt="");

    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    ClassDef(TABMparCon,1)

  private:

    Bool_t m_isMC;
    Double_t rdrift_cut;
    Double_t enxcell_cut;
    Double_t chi2red_cut;
    Int_t    planehit_cut;
    Int_t    minnhit_cut;
    Int_t    maxnhit_cut;
    Double_t angz_cut;
    Double_t angzres_cut;
    Int_t    rejmax_cut;
    Int_t    fitter_index;
    Int_t    bm_debug;//index for the bm_debug, (it is set at least to the global debug level)
    Int_t    bm_vietrack;//every bm_vietrack events SHOE will plot the BM track
    //~ string   datafile_name;//name of the input data file 
    Int_t    part_in_charge;//for BM Genfit tracking
    Double_t part_in_mom;//for BM Genfit tracking
    Int_t    calibro;//flag for the calibration
    Int_t    strel_switch;//flag to choose the st relations (1=garfield, 0=FIRST embedded)
    Int_t    prefit_enable;//flag to enable or disable the prefit
    //~ TVector3 meas_shift;//shift for the calibration
    //~ TVector3 meas_tilt;//tilt for the calibration
    Int_t    manageT0BM; //0=calculate T0 and save v_t0s in bmt0file, 1=loadT0 from bmt0file
    Int_t    t0_switch;//0=t0 from the beginning of the tdc signal, 1=from the peak, 2=negative T0 enabled
    Int_t    manageADCped; //0=calculate and save ADCped in bmpedfile, 1=loadadcped from bmpedfile
    string   bmt0file; //name of the T0 value file to be charged or to be written 
    string   bmpedfile; //name of the ped value file to be charged or to be written 
    string   parmapfile; //name of the file with the tdc channel map for TABMparMap 
    vector<Double_t> v_t0s;//T0 in ns
    vector<Double_t> adc_ped_mean;//pedestals mean 
    vector<Double_t> adc_ped_rms;//pedestals rms
    Int_t    num_ite;//number of iteration for the fit (only for FIRST fit)
    Double_t par_move;//change of the parameters for the FIRST fit
    Int_t    smearhits;//0=no smearhits on MC, 1=smear the number of hits
    Int_t    smearrdrift;//0=no smear rdrift, 1=gauss truncated 1sigma, 2=gaus 2sigma, 3=gaus 3sigma, 4=gaus no truncated, 5=uniform  
    Double_t fakehits_mean;//mean for the total number of hits (only MC)
    Double_t fakehits_sigma;//sigma for the total number of hits (only MC)
    Double_t mceff_mean;//mean for the number of primary hits (only MC)
    Double_t mceff_sigma;//sigma for the number of primary hits (only MC)

    //~ TF1* f_mypol;
    //~ TF1* f_mypol2;
    TSpline3 *m_mySpl;
    vector <TSpline3*> m_myVSpl;
    TF1 *m_myFunSpl;
    vector <TF1*> m_myVFunSpl;
    TH1D *my_hreso;
  
};

#endif
