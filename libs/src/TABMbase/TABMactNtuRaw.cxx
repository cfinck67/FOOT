/*!
  \file
  \version $Id: TABMactNtuRaw.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TABMactNtuRaw.
*/

#include "math.h"

#include "TABMdatRaw.hxx"
#include "TAIRdatRaw.hxx"
#include "TATRdatRaw.hxx"
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"
#include "TABMntuRaw.hxx"
#include "TABMactNtuRaw.hxx"

/*!
  \class TABMactNtuRaw TABMactNtuRaw.hxx "TABMactNtuRaw.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TABMactNtuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactNtuRaw::TABMactNtuRaw(const char* name,
			   TAGdataDsc* p_nturaw, 
			   TAGdataDsc* p_datraw, 
			   TAGdataDsc* p_timraw, 
			   TAGdataDsc* p_triraw, 
			   TAGparaDsc* p_geomap, 
			   TAGparaDsc* p_parcon)
  : TAGaction(name, "TABMactNtuRaw - NTuplize BM raw data"),
    fpNtuRaw(p_nturaw),
    fpDatRaw(p_datraw),
    fpTimRaw(p_timraw),
    fpTriRaw(p_triraw),
    fpGeoMap(p_geomap),
    fpParCon(p_parcon)
{
  AddDataOut(p_nturaw, "TABMntuRaw");
  AddDataIn(p_datraw, "TABMdatRaw");
  AddDataIn(p_timraw, "TAIRdatRaw");
  AddDataIn(p_triraw, "TATRdatRaw");
  AddPara(p_geomap, "TABMparGeo");
  AddPara(p_parcon, "TABMparCon");

  m_fittedVO=0;
  m_fittedVE=0;
  m_fittedUO=0;
  m_fittedUE=0;

  eff_V.clear(); eff_V.resize(12);
  eff_U.clear(); eff_U.resize(12);

  for(int id=0; id<6; id++) m_missedX[id]=0;
  for(int id=0; id<6; id++) m_missedY[id]=0;
  for(int id=0; id<12; id++) eff_V.at(id)=0;
  for(int id=0; id<12; id++) eff_U.at(id)=0;

}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactNtuRaw::~TABMactNtuRaw()
{}

void TABMactNtuRaw::cleanup() {

  m_fittedVO=0;
  m_fittedVE=0;
  m_fittedUO=0;
  m_fittedUE=0;

  eff_V.clear(); eff_V.resize(12);
  eff_U.clear(); eff_U.resize(12);

  for(int id=0; id<6; id++) m_missedX[id]= 0;
  for(int id=0; id<6; id++) m_missedY[id]= 0;
  for(int id=0; id<12; id++) eff_V.at(id) = 0;
  for(int id=0; id<12; id++) eff_U.at(id) = 0;
}

vector<double> TABMactNtuRaw::retrieve_V() {

  double nume(0), deno(0), erro(0), eff(0);

  for (int i = 0; i<3; i++) {
    nume = m_fittedVE - m_missedX[2*i];
    deno = m_fittedVE;
    CalcEffi(nume, deno, eff, erro);
    if (m_fittedVE!=0) {
      eff_V.at(i*2) = eff;
      eff_V.at((i+3)*2) = erro;
    }
    nume = m_fittedVO - m_missedX[2*i+1];
    deno = m_fittedVO;
    CalcEffi(nume, deno, eff, erro);
    if (m_fittedVO!=0) {
      eff_V.at(2*i+1)=eff;
      eff_V.at(2*(i+3)+1)=erro;
    }
  }

  return eff_V;
}

vector<double> TABMactNtuRaw::retrieve_U() {
  
  double nume(0), deno(0), erro(0), eff(0);

  for (int i = 0; i<3; i++) {
    nume = m_fittedUE - m_missedY[2*i];
    deno = m_fittedUE;
    CalcEffi(nume, deno, eff, erro);
    if (m_fittedUE!=0) {
      eff_U.at(i*2) = eff;
      eff_U.at((i+3)*2) = erro;
    }
    nume = m_fittedUO - m_missedY[2*i+1];
    deno = m_fittedUO;
    CalcEffi(nume, deno, eff, erro);
    if (m_fittedUO!=0) {
      eff_U.at(2*i+1)=eff;
      eff_U.at(2*(i+3)+1)=erro;
    }
  }

  return eff_U;
}


//------------------------------------------+-----------------------------------
//! Action.

Bool_t TABMactNtuRaw::Action()
{
  TABMdatRaw* p_datraw = (TABMdatRaw*) fpDatRaw->Object();
  TAIRdatRaw* p_timraw = (TAIRdatRaw*) fpTimRaw->Object();
  TATRdatRaw* p_triraw = (TATRdatRaw*) fpTriRaw->Object();
  TABMntuRaw* p_nturaw = (TABMntuRaw*) fpNtuRaw->Object();
  TABMparGeo* p_geomap = (TABMparGeo*) fpGeoMap->Object();
  TABMparCon* p_parcon = (TABMparCon*) fpParCon->Object();

  Double_t h_x,h_y,h_z,h_cx,h_cy,h_cz;

  if (!p_nturaw->h) p_nturaw->SetupClones();

  Int_t i_nhit = p_datraw->NHit();
  double irtime(-10000);
  double trigtime(-10000);
  
  Int_t timhit = p_timraw->nirhit;
  for (Int_t i = 0; i < timhit; i++) {
    const TAIRrawHit* aHi = p_timraw->Hit(i);
    if(aHi->ChID()==0 && !aHi->Type()) {
      irtime = aHi->Time();
    }
  }

  Int_t trihit = p_triraw->ntrhit;
  if(trihit>1) 
    Error("Action()","Multiple Trig hits :: %d",trihit);
  for (Int_t i = 0; i < trihit; i++) {
    const TATRrawHit* aTrHi = p_triraw->Hit(i);
    //0 : trigger
    //1 : master trigger (should be the same as 0)
    trigtime = aTrHi->TrigTime(1);
  }

  if(trigtime == -10000) {
    Info("Action()","Trigger Time is Missing!!!");
    fpNtuRaw->SetBit(kValid);
    return kTRUE;
  }

  if(irtime == -10000) {
    Info("Action()","Trigger IR Time is Missing!!!");
    fpNtuRaw->SetBit(kValid);
    return kTRUE;
  }

  int exi[3][6][2];
  for(int ic=0; ic<3; ic++) {
    for(int ip=0; ip<6; ip++) {
      for(int iv=0; iv<2; iv++) {
        exi[ic][ip][iv] = -1;
      }
    }
  }
  int tmp_view; bool doAdd, doReplace;     
  p_nturaw->nhit = 0;
  int tmp_hi = 0;

  //Efficiency variables
  int v_occPla[6], u_occPla[6], v_cellPla[6], u_cellPla[6];
  
  //PLANE efficiency
  for(int iD = 0; iD<6; iD++) { 
    v_occPla[iD] = u_occPla[iD] = 0;
    v_cellPla[iD] = u_cellPla[iD] = -10;
  }

  for (Int_t i = 0; i < i_nhit; i++) {
    const TABMrawHit& hit = p_datraw->Hit(i);
    Int_t i_cell = hit.Cell();
    Int_t i_plane = hit.Plane();
    Int_t i_view = hit.View();
    tmp_view = i_view;
    if(i_view<0) tmp_view =  0; //TOP View, V view view == -1
    doAdd = kFALSE;
    doReplace = kFALSE;
    Int_t i_idmon = 0; //!dummy!

    //T0s from parameters.
    Double_t t0_corr = p_parcon->GetT0(i_view,i_plane,i_cell);
    Double_t i_time = hit.Time()-t0_corr-(irtime);
    if(i_time<0) i_time = 0.;
    Double_t stcorr = p_parcon->STrelCorr(i_time,i_cell,i_plane,tmp_view);

    //Vdrift from parameters.
    Double_t vdrift = p_parcon->GetVDrift();
    Double_t i_timmon = hit.Time()-(irtime);
    if(i_timmon<0) 
      Info("Action()","Time %lf, Tirgger %lf T0 %lf",hit.Time(),irtime,t0_corr);
    
    Double_t i_drift = i_time*vdrift + stcorr;

    //    cout<<"DBG:: "<<t0_corr<<" "<<i_time<<" "<<(hit.Time()-(irtime)-t0_corr)*vdrift<<" "<<stcorr<<" "<<i_drift<<" "<<endl;

    if(exi[i_cell][i_plane][tmp_view]==-1) {
      //First time that there's an hit here
      exi[i_cell][i_plane][tmp_view] = tmp_hi;
      doAdd = kTRUE;
    } else {
      //Hit already existing check time
      TABMntuHit *tm = p_nturaw->Hit(exi[i_cell][i_plane][tmp_view]);
      if(tm->Timmon()>i_timmon) {
        doReplace = kTRUE;
      }
    }

    
    if(i_drift<0) {
      Info("Action()","Negative Distances!!! %lf %lf %lf %lf",i_timmon,hit.Time()-(irtime)-t0_corr,(hit.Time()-(irtime)-t0_corr)*vdrift,stcorr);    
      //      i_drift = 0;
    }
    //V view, (X,Z)
    p_geomap->GetCellInfo(i_view, i_plane, i_cell, h_x, h_y, h_z, h_cx, h_cy, h_cz);
    
    Double_t resolution = p_parcon->ResoEval(i_drift);

    if(doAdd) {
      
      if (i_view == -1) {
	v_occPla[i_plane]++;
	v_cellPla[i_plane] = i_cell;
      }
      if (i_view == 1) {
	u_occPla[i_plane]++;
	u_cellPla[i_plane] = i_cell;
      }

      TABMntuHit *mytmp = new((*(p_nturaw->h))[tmp_hi]) 
	TABMntuHit(i_idmon,   i_view,
		   i_plane,   i_cell,  
		   h_x, h_y, h_z,
		   h_cx, h_cy, h_cz,
		   i_drift,  i_time,
		   i_timmon );

      mytmp->SetSigma(resolution);
      //~ mytmp->SetTrkAss(0);
      tmp_hi++;
      p_nturaw->nhit++;

    } 

    if(doReplace) {

      TABMntuHit *tm = p_nturaw->Hit(exi[i_cell][i_plane][tmp_view]);
      tm->SetData(i_idmon,   i_view,
		  i_plane,   i_cell,  
		  h_x, h_y, h_z,
		  h_cx, h_cy, h_cz,
		  i_drift,  i_time,
		  i_timmon );
      tm->SetSigma(resolution);
      //~ tm->SetTrkAss(0);
    }
  }
  
  
  bool chk_u_eve = (u_cellPla[0] == u_cellPla[2]) && (u_cellPla[0] == u_cellPla[4]);
  bool chk_u_odd = (u_cellPla[1] == u_cellPla[3]) && (u_cellPla[1] == u_cellPla[5]);
  bool chk_v_eve = (v_cellPla[0] == v_cellPla[2]) && (v_cellPla[0] == v_cellPla[4]);
  bool chk_v_odd = (v_cellPla[1] == v_cellPla[3]) && (v_cellPla[1] == v_cellPla[5]);
  
  if((v_occPla[0]+v_occPla[2]+v_occPla[4]) == 3 && chk_v_eve) {
    m_fittedVO++;
    if(v_occPla[1] == 0) m_missedX[1]++;
    if(v_occPla[3] == 0) m_missedX[3]++;
    if(v_occPla[5] == 0) m_missedX[5]++;
  }

  if((v_occPla[1]+v_occPla[3]+v_occPla[5]) == 3 && chk_v_odd) {
    m_fittedVE++;
    if(v_occPla[0] == 0) m_missedX[0]++;
    if(v_occPla[2] == 0) m_missedX[2]++;
    if(v_occPla[4] == 0) m_missedX[4]++;
  }

  if((u_occPla[0]+u_occPla[2]+u_occPla[4]) == 3 && chk_u_eve) {
   m_fittedUO++;
   if(u_occPla[1] == 0) m_missedY[1]++;
   if(u_occPla[3] == 0) m_missedY[3]++;
   if(u_occPla[5] == 0) m_missedY[5]++;
  }
  
  if((u_occPla[1]+u_occPla[3]+u_occPla[5]) == 3 && chk_u_odd) {
    m_fittedUE++;
    if(u_occPla[0] == 0) m_missedY[0]++;
    if(u_occPla[2] == 0) m_missedY[2]++;
    if(u_occPla[4] == 0) m_missedY[4]++;
  }
  
  fpNtuRaw->SetBit(kValid);
  return kTRUE;
  
}

//=============================================================================
// Calc the Error on the efficiency
//=============================================================================
void TABMactNtuRaw::CalcEffi(double nume, double deno, double &eff, double &erro) {

  double argone(0);
  if(deno<=0) {
    eff = 0;
    erro = 0;
    cout<<"deno<0 :: "<<deno<<endl;
    return;
  }
  eff = nume/deno;
  argone = 4.*eff*(1.-eff)/deno+1./pow(deno,2);
  if(argone>0.) {
    erro = sqrt(argone)/(2.*(1.+1./deno));
  } else {
    cout<<"arg(sqrt)<0 :: "<<argone<<endl;
    erro= 1./(2.*(deno+1.));
  }
  return;
}
