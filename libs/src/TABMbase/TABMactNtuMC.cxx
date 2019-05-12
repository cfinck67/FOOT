/*!
  \file
  \version $Id: TABMactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TABMactNtuMC.
*/

#include "TABMactNtuMC.hxx"

/*!
  \class TABMactNtuMC TABMactNtuMC.hxx "TABMactNtuMC.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TABMactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactNtuMC::TABMactNtuMC(const char* name,
			   TAGdataDsc* dscnturaw, 
			   TAGparaDsc* dscbmcon, 
			   TAGparaDsc* dscbmgeo, 
			   EVENT_STRUCT* evStr)
  : TAGaction(name, "TABMactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(dscnturaw),
    fpParCon(dscbmcon),
    fpParGeo(dscbmgeo),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddPara(fpParCon, "TABMparCon");
  AddPara(fpParGeo, "TABMparGeo");
  AddDataOut(fpNtuMC, "TABMntuRaw"); 

  p_bmcon = (TABMparCon*) (gTAGroot->FindParaDsc("myp_bmcon", "TABMparCon")->Object()); 
  p_bmgeo = (TABMparGeo*) (gTAGroot->FindParaDsc("myp_bmgeo", "TABMparGeo")->Object());

}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactNtuMC::~TABMactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TABMactNtuMC::Action()
{ 
  //~ TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  p_nturaw = (TABMntuRaw*) fpNtuMC->Object();
  
  //parameters:
  //~ TVector3 real_rotation(-3.,0.,0.);//proton_calib_x05_y01_theta3.root
  TVector3 real_rotation(-1.2,-3.2,0.);//proton_calib_xy_rot_tras.root

  Int_t cell, view, lay, ipoint, tmp_int, tmp2int;
  vector<Int_t> hitxcell(fpEvtStr->BMNn, 99); 
  vector<bool> tobecharged(fpEvtStr->BMNn, true);
  vector<Double_t> rdriftxcell(fpEvtStr->BMNn, 99.);
  Int_t nhits=0;
  TVector3 loc, gmom, A0, Wvers, dirfrompos;
  if (!p_nturaw->h) p_nturaw->SetupClones();//se non c'è l'array di h, lo crea
  //The number of hits inside the BM is nmon
  Info("Action()","Processing n :: %2d hits \n",fpEvtStr->BMNn);

  if(p_bmcon->GetCalibro()!=0){
    //~ cout<<"Before tilt and shift the wires:"<<endl;//PROVV
    //~ p_bmgeo->CoutWirePosDir();
    p_bmgeo->RotateNewBmon(p_bmcon->GetMeas_tilt().X(),p_bmcon->GetMeas_tilt().Y(),p_bmcon->GetMeas_tilt().Z(),false);//WARNING!!!! ROOT AND FLUKA ROTATE WITH DIFFERENT SIGN...
    p_bmgeo->ShiftBmon(p_bmcon->GetMeas_shift());
    //~ cout<<endl<<"DOPO AVER SPOSTATO I FILI"<<endl;
    //~ p_bmgeo->CoutWirePosDir();
    //~ cout<<endl<<"bm center position X="<<p_bmgeo->GetCenter().X()<<"  Y="<<p_bmgeo->GetCenter().Y()<<"  Z="<<p_bmgeo->GetCenter().Z()<<endl;
  }

  //loop for double hits and hits with energy less than enxcell_cut:
  for (Int_t i = 0; i < fpEvtStr->BMNn; i++) {
    if(fpEvtStr->BMNde[i] < p_bmcon->GetEnxcellcut() && fpEvtStr->TRpaid[fpEvtStr->BMNid[i]-1]!=0)
      tobecharged.at(i)=false;
    if(tobecharged.at(i)){
      cell = fpEvtStr->BMNicell[i];
      lay = fpEvtStr->BMNilay[i]; 
      view = fpEvtStr->BMNiview[i]==-1 ? 1:0;
      hitxcell.at(i)=p_bmgeo->GetBMNcell(lay, view, cell);
      loc.SetXYZ(fpEvtStr->BMNxin[i],fpEvtStr->BMNyin[i],fpEvtStr->BMNzin[i]);
      p_bmgeo->Global2Local(&loc);
      dirfrompos.SetXYZ(fpEvtStr->BMNxout[i]-fpEvtStr->BMNxin[i],fpEvtStr->BMNyout[i]-fpEvtStr->BMNyin[i],fpEvtStr->BMNzout[i]-fpEvtStr->BMNzin[i]);
      dirfrompos.SetMag(1.);
      //~ gmom.SetXYZ(fpEvtStr->BMNpxin[i],fpEvtStr->BMNpyin[i],fpEvtStr->BMNpzin[i]);
      
      A0.SetXYZ(p_bmgeo->GetX(p_bmgeo->GetID(cell),lay,view),      
                p_bmgeo->GetY(p_bmgeo->GetID(cell),lay,view),    
                p_bmgeo->GetZ(p_bmgeo->GetID(cell),lay,view));  
      Wvers.SetXYZ(p_bmgeo->GetCX(p_bmgeo->GetID(cell),lay,view), 
                   p_bmgeo->GetCY(p_bmgeo->GetID(cell),lay,view), 
                   p_bmgeo->GetCZ(p_bmgeo->GetID(cell),lay,view)); 
      Wvers.SetMag(1.);                      
      //~ rdriftxcell.at(i)=FindRdrift(loc, gmom, A0, Wvers);
      rdriftxcell.at(i)=FindRdrift(loc, dirfrompos, A0, Wvers);
      
      //~ rdriftxcell.at(i)-=0.05;//provv
      //~ if(rdriftxcell.at(i)<0) rdriftxcell.at(i)=0.;
      
      if(rdriftxcell.at(i)==99) //FindRdrift return 99 if a particle is born without energy, so it shouldn't release energy for a hit.
        tobecharged.at(i)=false;
      //if there is a double hit in the same cell, it charges the hits if they have rdrift difference more than p_bmcon->GetRdriftCut()
      for(Int_t j=0;j<i;j++){ 
        if((rdriftxcell.at(i)-rdriftxcell.at(j))<p_bmcon->GetRdriftCut() && rdriftxcell.at(i)>=rdriftxcell.at(j) && hitxcell.at(i)==hitxcell.at(j))
          tobecharged.at(i)=false;
        if((rdriftxcell.at(j)-rdriftxcell.at(i))<p_bmcon->GetRdriftCut() && rdriftxcell.at(j)>rdriftxcell.at(i) && hitxcell.at(i)==hitxcell.at(j))
          tobecharged.at(j)=false;
        }
      }
    }
    
  if(p_bmcon->GetCalibro()!=0){
    p_bmgeo->ShiftBmon(-p_bmcon->GetMeas_shift());
    p_bmgeo->RotateNewBmon(p_bmcon->GetMeas_tilt().X(),p_bmcon->GetMeas_tilt().Y(),p_bmcon->GetMeas_tilt().Z(),true);    
    //~ cout<<"end hits charging, I retilt and reshift the wires"<<endl;//PROVV
    //~ p_bmgeo->CoutWirePosDir();
    }
    
          
  //set the number of hits
  Int_t hitsrandtot;    
  Int_t remainhitsn, nrealhits;
  
  if(p_bmcon->GetSmearhits()){
    nrealhits=0;
    for(Int_t i=0;i<tobecharged.size();i++)
      if(tobecharged.at(i))
        nrealhits++;
        
    //prune the real hits
    Int_t tmp_int=p_bmcon->GetRand()->Uniform(0,10);
    if(tmp_int<p_bmcon->GetFakehitsMean()) 
      hitsrandtot = 12 - (Int_t) fabs(p_bmcon->GetRand()->Gaus(0, p_bmcon->GetFakehitsSigmaLeft()));  
    else
      hitsrandtot = 12 + (Int_t) fabs(p_bmcon->GetRand()->Gaus(0, p_bmcon->GetFakehitsSigmaRight()));  
    Int_t nprunehits=nrealhits*(1.-p_bmcon->GetRand()->Gaus(p_bmcon->GetMCEffMean(), p_bmcon->GetMCEffSigma()))+0.5;
    if(nprunehits<0)
      nprunehits=0;
    if(nprunehits>nrealhits)
      nprunehits=nrealhits;
    if((nrealhits-nprunehits)>hitsrandtot)
      nprunehits=nrealhits-hitsrandtot;
    remainhitsn=nrealhits-nprunehits;
    while(nprunehits>0){
      tmp_int=p_bmcon->GetRand()->Uniform(0.5,nprunehits+0.5);
        //~ if(tobecharged.at(tmp_int)==true){
          //~ tobecharged.at(tmp_int)=false;
          //~ nprunehits--;
        //~ }
      tmp2int=0;
      for(Int_t i=0;i<tobecharged.size();i++){
        if(tobecharged.at(i)){
          if(tmp2int==tmp_int){
            tobecharged.at(i)=false;
            nprunehits--;
            i=tobecharged.size()+1;
          }else
            tmp2int++;
        }
      }
    };
    
    //add fake hits
    if(hitsrandtot-remainhitsn>0)
      CreateFakeHits(hitsrandtot-remainhitsn, nhits);
  }
    
  Double_t realrdrift, hitsigma;    
  //charge the hits:
  for (Int_t i = 0; i < fpEvtStr->BMNn; i++) {

    if(tobecharged.at(i)){
      ipoint=fpEvtStr->BMNid[i]-1;
      cell = fpEvtStr->BMNicell[i];
      lay = fpEvtStr->BMNilay[i];
      view = fpEvtStr->BMNiview[i]==-1 ? 1:0;
      loc.SetXYZ(fpEvtStr->BMNxin[i],fpEvtStr->BMNyin[i],fpEvtStr->BMNzin[i]);
      //~ cout<<"da global:   loc.X="<<loc.X()<<"  loc.Y()="<<loc.Y()<<"  loc.Z()="<<loc.Z()<<endl;
      p_bmgeo->Global2Local(&loc);
      //~ cout<<"local:   loc.X="<<loc.X()<<"  loc.Y()="<<loc.Y()<<"  loc.Z()="<<loc.Z()<<endl;

      //shift the t0 and change the strelations:
      realrdrift=rdriftxcell.at(i);
      if(p_bmcon->GetCalibro()>0){
        if(p_bmcon->GetAutostrel()==1 || p_bmcon->GetAutostrel()==0)
          rdriftxcell.at(i)=p_bmcon->FirstSTrel(p_bmcon->InverseStrel(realrdrift), p_bmcon->GetCalibro());//this is useful if you want to change the strel    
        else
          rdriftxcell.at(i)=p_bmcon->FirstSTrel(p_bmcon->InverseStrel(realrdrift));      
      }
      //~ if(rdriftxcell.at(i)==0)
        //~ rdriftxcell.at(i)=0.001;

      if(p_bmcon->GetBMdebug()>=3)
        cout<<"In the charging hits loop: I'm going to charge hit number:"<<i<<"/"<<fpEvtStr->BMNn<<"  tobecharged="<<tobecharged[i]<<"  view="<<view<<"  lay="<<lay<<"  cell="<<cell<<"  rdriftxcell[i]="<<rdriftxcell[i]<<"  realrdrift="<<realrdrift<<"    p_bmcon->InverseStrel(realrdrift)="<<p_bmcon->InverseStrel(realrdrift)<<endl;      
      //create hit
      TABMntuHit *mytmp = new((*(p_nturaw->h))[nhits]) TABMntuHit(    
                          fpEvtStr->BMNid[i],	view, lay, cell,        
                          loc.X(), loc.Y(), loc.Z(),  
                          fpEvtStr->BMNpxin[i], fpEvtStr->BMNpyin[i], fpEvtStr->BMNpzin[i],  //mom @ entrance in cell
                          rdriftxcell.at(i), p_bmcon->InverseStrel(realrdrift), fpEvtStr->BMNtim[i], p_bmgeo);     
        
      //X,Y and Z needs to be placed in Local coordinates.
      //~ mytmp->SetAW(p_bmgeo);
      //hit resolution and smearing
      if(p_bmcon->ResoEvalTime(p_bmcon->InverseStrel(realrdrift))>0)
        hitsigma=p_bmcon->ResoEvalTime(p_bmcon->InverseStrel(realrdrift));
      else{  
        cout<<"WARNING: error from config ResoEvalTime(p_bmcon->InverseStrel(realrdrift))! sigma on rdrift is zero!!! going to set error=0.015; rdrift="<<p_bmcon->InverseStrel(realrdrift)<<"   realrdrift="<<realrdrift<<"    ResoEvaltime="<<p_bmcon->ResoEvalTime(p_bmcon->InverseStrel(realrdrift))<<"   Inversestrel="<<p_bmcon->InverseStrel(realrdrift)<<endl;
        hitsigma=p_bmcon->GetRdrift_err();
       }
      if(p_bmcon->GetAutostrel()>0)
        mytmp->SetSigma(p_bmcon->MCResoEvalTime(p_bmcon->InverseStrel(realrdrift)));
      else
        mytmp->SetSigma(hitsigma);
        
      if(p_bmcon->GetSmearrdrift()>0)
        mytmp->SmearRdrift(p_bmcon);   //smearing
      if(p_bmcon->GetAutostrel()>0)
        mytmp->SetSigma(hitsigma);
        
      if(p_bmcon->GetBMdebug()>=3)
        cout<<"TABMactNtuMC: postsmearing:  hitsigma="<<hitsigma<<"   getsigma="<<mytmp->GetSigma()<<"   MCresoEval="<<p_bmcon->MCResoEvalTime(p_bmcon->InverseStrel(realrdrift))<<"    hit_rdrift="<<mytmp->Dist()<<"   tdrift="<<mytmp->Tdrift()<<endl;
      
      mytmp->SetRealRdrift(realrdrift);  
      
      if(fpEvtStr->TRpaid[fpEvtStr->BMNid[i]-1]!=0)
        mytmp->SetIsFake(1);
      else
        mytmp->SetIsFake(0);
      nhits++;
    }//end of tobecharded if
  }
  
  if(hitsrandtot!= nhits && p_bmcon->GetSmearhits())
    cout<<"TABMactNtuMC::ERROR!!!!!!!!  nhits="<<nhits<<"  hitsrandtot="<<hitsrandtot<<"  remainhitsn"<<remainhitsn<<"  nrealhits"<<nrealhits<<endl;
  p_nturaw->nhit  = nhits;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}



void TABMactNtuMC::CreateFakeHits(Int_t nfake, Int_t &nhits){
  Int_t plane, view, cell;
  for(Int_t i=0;i<nfake;i++){
    do{plane=p_bmcon->GetRand()->Uniform(0,6);}while(plane<0 || plane>5);  
    view=(p_bmcon->GetRand()->Uniform(0,2)>1) ? 0: 1;  
    do{cell=p_bmcon->GetRand()->Uniform(0,3);}while(cell<0 || cell>2);  
    Double_t rdrift=p_bmcon->GetRand()->Uniform(0.,0.9);
    //~ cout<<"view="<<view<<" plane="<<plane<<"  cell="<<cell<<endl;
    
    //charge the fake hits
    TABMntuHit *mytmp = new((*(p_nturaw->h))[nhits]) TABMntuHit(    
                    -100,	view, plane, cell,        
                    -100., -100., -100.,  
                    -100.,-100.,-100.,  //mom @ entrance in cell
                    rdrift, p_bmcon->InverseStrel(rdrift), -1., p_bmgeo);     //tdrift has no meaning for MC (now)
    //~ mytmp->SetAW(p_bmgeo);
    if(p_bmcon->ResoEvalTime(p_bmcon->InverseStrel(rdrift))>0)
      mytmp->SetSigma(p_bmcon->ResoEvalTime(p_bmcon->InverseStrel(rdrift)));
    else
      mytmp->SetSigma(p_bmcon->GetRdrift_err());
    mytmp->SetRealRdrift(rdrift);  
    mytmp->SetIsFake(2);
    nhits++;  
  }
  return;
}


Double_t TABMactNtuMC::FindRdrift(TVector3 R0, TVector3 Pvers, TVector3 A0, TVector3 Wvers) {

  Double_t tp = 0., tf= 0., rdrift; 
  TVector3 D0;

  //~ if (dir.Mag()!=0.)
    //~ dir.SetMag(1.);
  //~ else{
  if(Pvers.Mag()==0.){
    //~ cout<<"WARNING: FindRdrift: momentum is 0 and the hit shouldn't be charged because this hit is from a fragmentated particle with zero momentum"<<endl;
    return 99;//fake value
  }
    
  Pvers.SetMag(1.);
  
  D0 = R0 - A0;//distance between position of reference point of current wire and current particle position

  Double_t prosca = Pvers*Wvers ;//scalar product of directions
  Double_t D0W = D0*Wvers;//distance projected on wire
  Double_t D0P = D0*Pvers;//distance projected on particle direction

  if(prosca!= 1.) {//if the don't fly parallel
    tp = (D0W*prosca - D0P)/(1.-prosca*prosca);
    tf = (-D0P*prosca + D0W)/(1.-prosca*prosca);
    rdrift = sqrt( abs(D0.Mag2() + tp*tp + tf*tf + 2.*tp*D0P -2.*tf*D0W -2.*prosca*tf*tp ));
    } 
  else  //if they go parallel
    rdrift = sqrt(abs( D0.Mag2() - D0W*D0W)); 

  if(rdrift<0 || rdrift>0.945){
    cout<<"ERROR! in TABMactNtuMC::FindRdrift: rdrift is >0.945 or <0: rdrift="<<rdrift<<endl;
    cout<<"rdrift="<<rdrift<<endl;
    cout<<"R0=("<<R0.X()<<","<<R0.Y()<<","<<R0.Z()<<")  Pvers=("<<Pvers.X()<<","<<Pvers.Y()<<","<<Pvers.Z()<<")"<<endl;
    cout<<"A0=("<<A0.X()<<","<<A0.Y()<<","<<A0.Z()<<")  Wvers=("<<Wvers.X()<<","<<Wvers.Y()<<","<<Wvers.Z()<<")"<<endl;
    cout<<"Now I'll adjust manually the rdrfit to 0 or 0.944"<<endl;
    rdrift= (rdrift<0) ? 0. : 0.944;
    }
    
    //~ cout<<"rdrift="<<rdrift<<endl;
    //~ cout<<"pos=("<<pos.X()<<","<<pos.Y()<<","<<pos.Z()<<")  dir=("<<dir.X()<<","<<dir.Y()<<","<<dir.Z()<<")"<<endl;
    //~ cout<<"A0=("<<A0.X()<<","<<A0.Y()<<","<<A0.Z()<<")  Wvers=("<<Wvers.X()<<","<<Wvers.Y()<<","<<Wvers.Z()<<")"<<endl;  
  
  //~ if(rdrift>=0.945)
    //~ cout<<"WARNING:: TABMactNtuMC::FindRdrift: rdrift="<<rdrift<<endl;
    
  return rdrift;
}


