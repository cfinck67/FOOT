#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Evento.h"

using namespace std;

ClassImp(Evento);

/*-----------------------------------------------------------------*/
  
Evento::Evento()
{
  eve.nump    = 0;
  eve.nsc     = 0;
  eve.nvtx    = 0;
  eve.n1dc    = 0;
  eve.n2dc    = 0;
  eve.nscint  = 0;
  eve.ncry    = 0;
  eve.ncross  = 0;

  for(int kk=0;kk<MAXNUMP;kk++){
    eve.idpa[kk]   = 0;
    eve.igen[kk]   = 0;
    eve.icha[kk]   = -100;
    eve.numreg[kk] = 0;
    eve.iba[kk]    = 0;
    eve.idead[kk]  = 0;
    eve.jpa[kk]    = -100;
    eve.vxi[kk]    = 0.;
    eve.vyi[kk]    = 0.;
    eve.vzi[kk]    = 0.;
    eve.vxf[kk]    = 0.;
    eve.vyf[kk]    = 0.;
    eve.vzf[kk]    = 0.;
    eve.px[kk]     = 0.;
    eve.py[kk]     = 0.;
    eve.pz[kk]     = 0.;
    eve.pxf[kk]    = 0.;
    eve.pyf[kk]    = 0.;
    eve.pzf[kk]    = 0.;
    eve.amass[kk]  = 0.;
    eve.tempo[kk]  = 0.;
    eve.tof[kk]    = 0.;
    eve.trlen[kk]  = 0.;
  }

  for(int kk=0;kk<MAXSC;kk++){
    eve.idsc[kk]    = 0;
    eve.xinsc[kk]   = 0.;
    eve.yinsc[kk]   = 0.;
    eve.zinsc[kk]   = 0.;
    eve.xoutsc[kk]  = 0.;
    eve.youtsc[kk]  = 0.;
    eve.zoutsc[kk]  = 0.;
    eve.pxinsc[kk]  = 0.;
    eve.pyinsc[kk]  = 0.;
    eve.pzinsc[kk]  = 0.;
    eve.pxoutsc[kk] = 0.;
    eve.pyoutsc[kk] = 0.;
    eve.pzoutsc[kk] = 0.;
    eve.desc[kk]    = 0.;
    eve.alsc[kk]    = 0.;
    eve.timsc[kk]   = 0.;
  }
  
  for(int kk=0;kk<MAXVTX;kk++){
    eve.idvtx[kk]    = 0;
    eve.iplavtx[kk] = 0;
    eve.irowvtx[kk]  = 0;
    eve.icolvtx[kk]  = 0;
    eve.xinvtx[kk]   = 0.;
    eve.yinvtx[kk]   = 0.;
    eve.zinvtx[kk]   = 0.;
    eve.xoutvtx[kk]  = 0.;
    eve.youtvtx[kk]  = 0.;
    eve.zoutvtx[kk]  = 0.;
    eve.pxinvtx[kk]  = 0.;
    eve.pyinvtx[kk]  = 0.;
    eve.pzinvtx[kk]  = 0.;
    eve.pxoutvtx[kk] = 0.;
    eve.pyoutvtx[kk] = 0.;
    eve.pzoutvtx[kk] = 0.;
    eve.devtx[kk]    = 0.;
    eve.alvtx[kk]    = 0.;
    eve.timvtx[kk]   = 0.;
  }

  for(int kk=0;kk<MAX1DC;kk++){
    eve.id1dc[kk]    = 0;
    eve.ipla1dc[kk]  = 0;
    eve.iview1dc[kk] = 0;
    eve.icell1dc[kk] = 0;
    eve.xin1dc[kk]   = 0.;
    eve.yin1dc[kk]   = 0.;
    eve.zin1dc[kk]   = 0.;
    eve.xout1dc[kk]  = 0.;
    eve.yout1dc[kk]  = 0.;
    eve.zout1dc[kk]  = 0.;
    eve.pxin1dc[kk]  = 0.;
    eve.pyin1dc[kk]  = 0.;
    eve.pzin1dc[kk]  = 0.;
    eve.pxout1dc[kk] = 0.;
    eve.pyout1dc[kk] = 0.;
    eve.pzout1dc[kk] = 0.;
    eve.de1dc[kk]    = 0.;
    eve.al1dc[kk]    = 0.;
    eve.tim1dc[kk]   = 0.;
  }

  for(int kk=0;kk<MAX2DC;kk++){
    eve.id2dc[kk]    = 0;
    eve.ipla2dc[kk]  = 0;
    eve.iview2dc[kk] = 0;
    eve.icell2dc[kk] = 0;
    eve.xin2dc[kk]   = 0.;
    eve.yin2dc[kk]   = 0.;
    eve.zin2dc[kk]   = 0.;
    eve.xout2dc[kk]  = 0.;
    eve.yout2dc[kk]  = 0.;
    eve.zout2dc[kk]  = 0.;
    eve.pxin2dc[kk]  = 0.;
    eve.pyin2dc[kk]  = 0.;
    eve.pzin2dc[kk]  = 0.;
    eve.pxout2dc[kk] = 0.;
    eve.pyout2dc[kk] = 0.;
    eve.pzout2dc[kk] = 0.;
    eve.de2dc[kk]    = 0.;
    eve.al2dc[kk]    = 0.;
    eve.tim2dc[kk]   = 0.;
  }
  
  for(int kk=0;kk<MAXSCINT;kk++){
    eve.idscint[kk]    = 0;
    eve.irowscint[kk]  = 0;
    eve.icolscint[kk]  = 0;
    eve.xinscint[kk]   = 0.;
    eve.yinscint[kk]   = 0.;
    eve.zinscint[kk]   = 0.;
    eve.xoutscint[kk]  = 0.;
    eve.youtscint[kk]  = 0.;
    eve.zoutscint[kk]  = 0.;
    eve.pxinscint[kk]  = 0.;
    eve.pyinscint[kk]  = 0.;
    eve.pzinscint[kk]  = 0.;
    eve.pxoutscint[kk] = 0.;
    eve.pyoutscint[kk] = 0.;
    eve.pzoutscint[kk] = 0.;
    eve.descint[kk]    = 0.;
    eve.alscint[kk]    = 0.;
    eve.timscint[kk]   = 0.;
  }

  for(int kk=0;kk<MAXCRY;kk++){
    eve.idcry[kk]    = 0;
    eve.irowcry[kk]  = 0;
    eve.icolcry[kk]  = 0;
    eve.xincry[kk]   = 0.;
    eve.yincry[kk]   = 0.;
    eve.zincry[kk]   = 0.;
    eve.xoutcry[kk]  = 0.;
    eve.youtcry[kk]  = 0.;
    eve.zoutcry[kk]  = 0.;
    eve.pxincry[kk]  = 0.;
    eve.pyincry[kk]  = 0.;
    eve.pzincry[kk]  = 0.;
    eve.pxoutcry[kk] = 0.;
    eve.pyoutcry[kk] = 0.;
    eve.pzoutcry[kk] = 0.;
    eve.decry[kk]    = 0.;
    eve.alcry[kk]    = 0.;
    eve.timcry[kk]   = 0.;    
  }

  for(int kk=0;kk<MAXCROSS;kk++){
    eve.idcross[kk]      = 0;
    eve.nregcross[kk]    = 0;
    eve.nregoldcross[kk] = 0;
    eve.xcross[kk]       = 0.;
    eve.ycross[kk]       = 0.;
    eve.zcross[kk]       = 0.;
    eve.pxcross[kk]      = 0.;
    eve.pycross[kk]      = 0.;
    eve.pzcross[kk]      = 0.;
    eve.mcross[kk]       = 0.;
    eve.chcross[kk]      = 0.;
    eve.tcross[kk]       = 0.;
  }

}

/*-----------------------------------------------------------------*/

Int_t Evento::SetEvent(Int_t fnumero_evento){
  //  cout <<" Entro in SetEvent"<<endl;
  eve.num_event = fnumero_evento;
  return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::Clean(){
  //  cout <<" Entro in clean"<<endl;

  for(int kk=0;kk<eve.nump;kk++){
    eve.idpa[kk]   = 0;
    eve.igen[kk]   = 0;
    eve.icha[kk]   = -100;
    eve.numreg[kk] = 0;
    eve.iba[kk]    = 0;
    eve.idead[kk]  = 0;
    eve.jpa[kk]    = -100;
    eve.vxi[kk]    = 0.;
    eve.vyi[kk]    = 0.;
    eve.vzi[kk]    = 0.;
    eve.vxf[kk]    = 0.;
    eve.vyf[kk]    = 0.;
    eve.vzf[kk]    = 0.;
    eve.px[kk]     = 0.;
    eve.py[kk]     = 0.;
    eve.pz[kk]     = 0.;
    eve.pxf[kk]    = 0.;
    eve.pyf[kk]    = 0.;
    eve.pzf[kk]    = 0.;
    eve.amass[kk]  = 0.;
    eve.tempo[kk]  = 0.;
    eve.tof[kk]    = 0.;
    eve.trlen[kk]  = 0.;
  }

  for(int kk=0;kk<eve.nsc;kk++){
    eve.idsc[kk]    = 0;
    eve.xinsc[kk]   = 0.;
    eve.yinsc[kk]   = 0.;
    eve.zinsc[kk]   = 0.;
    eve.xoutsc[kk]  = 0.;
    eve.youtsc[kk]  = 0.;
    eve.zoutsc[kk]  = 0.;
    eve.pxinsc[kk]  = 0.;
    eve.pyinsc[kk]  = 0.;
    eve.pzinsc[kk]  = 0.;
    eve.pxoutsc[kk] = 0.;
    eve.pyoutsc[kk] = 0.;
    eve.pzoutsc[kk] = 0.;
    eve.desc[kk]    = 0.;
    eve.alsc[kk]    = 0.;
    eve.timsc[kk]   = 0.;
  }

  for(int kk=0;kk<eve.nvtx;kk++){
    eve.idvtx[kk]    = 0;
    eve.iplavtx[kk] = 0;
    eve.irowvtx[kk]  = 0;
    eve.icolvtx[kk]  = 0;
    eve.xinvtx[kk]   = 0.;
    eve.yinvtx[kk]   = 0.;
    eve.zinvtx[kk]   = 0.;
    eve.xoutvtx[kk]  = 0.;
    eve.youtvtx[kk]  = 0.;
    eve.zoutvtx[kk]  = 0.;
    eve.pxinvtx[kk]  = 0.;
    eve.pyinvtx[kk]  = 0.;
    eve.pzinvtx[kk]  = 0.;
    eve.pxoutvtx[kk] = 0.;
    eve.pyoutvtx[kk] = 0.;
    eve.pzoutvtx[kk] = 0.;
    eve.devtx[kk]    = 0.;
    eve.alvtx[kk]    = 0.;
    eve.timvtx[kk]   = 0.;
  }

  for(int kk=0;kk<eve.n1dc;kk++){
    eve.id1dc[kk]    = 0;
    eve.ipla1dc[kk]  = 0;
    eve.iview1dc[kk] = 0;
    eve.icell1dc[kk] = 0;
    eve.xin1dc[kk]   = 0.;
    eve.yin1dc[kk]   = 0.;
    eve.zin1dc[kk]   = 0.;
    eve.xout1dc[kk]  = 0.;
    eve.yout1dc[kk]  = 0.;
    eve.zout1dc[kk]  = 0.;
    eve.pxin1dc[kk]  = 0.;
    eve.pyin1dc[kk]  = 0.;
    eve.pzin1dc[kk]  = 0.;
    eve.pxout1dc[kk] = 0.;
    eve.pyout1dc[kk] = 0.;
    eve.pzout1dc[kk] = 0.;
    eve.de1dc[kk]    = 0.;
    eve.al1dc[kk]    = 0.;
    eve.tim1dc[kk]   = 0.;
  }

  for(int kk=0;kk<eve.n2dc;kk++){
    eve.id2dc[kk]    = 0;
    eve.ipla2dc[kk]  = 0;
    eve.iview2dc[kk] = 0;
    eve.icell2dc[kk] = 0;
    eve.xin2dc[kk]   = 0.;
    eve.yin2dc[kk]   = 0.;
    eve.zin2dc[kk]   = 0.;
    eve.xout2dc[kk]  = 0.;
    eve.yout2dc[kk]  = 0.;
    eve.zout2dc[kk]  = 0.;
    eve.pxin2dc[kk]  = 0.;
    eve.pyin2dc[kk]  = 0.;
    eve.pzin2dc[kk]  = 0.;
    eve.pxout2dc[kk] = 0.;
    eve.pyout2dc[kk] = 0.;
    eve.pzout2dc[kk] = 0.;
    eve.de2dc[kk]    = 0.;
    eve.al2dc[kk]    = 0.;
    eve.tim2dc[kk]   = 0.;
  }

  for(int kk=0;kk<eve.nscint;kk++){
    eve.idscint[kk]    = 0;
    eve.irowscint[kk]  = 0;
    eve.icolscint[kk]  = 0;
    eve.xinscint[kk]   = 0.;
    eve.yinscint[kk]   = 0.;
    eve.zinscint[kk]   = 0.;
    eve.xoutscint[kk]  = 0.;
    eve.youtscint[kk]  = 0.;
    eve.zoutscint[kk]  = 0.;
    eve.pxinscint[kk]  = 0.;
    eve.pyinscint[kk]  = 0.;
    eve.pzinscint[kk]  = 0.;
    eve.pxoutscint[kk] = 0.;
    eve.pyoutscint[kk] = 0.;
    eve.pzoutscint[kk] = 0.;
    eve.descint[kk]    = 0.;
    eve.alscint[kk]    = 0.;
    eve.timscint[kk]   = 0.;
  }
  
  for(int kk=0;kk<eve.ncry;kk++){
    eve.idcry[kk]    = 0;
    eve.irowcry[kk]  = 0;
    eve.icolcry[kk]  = 0;
    eve.xincry[kk]   = 0.;
    eve.yincry[kk]   = 0.;
    eve.zincry[kk]   = 0.;
    eve.xoutcry[kk]  = 0.;
    eve.youtcry[kk]  = 0.;
    eve.zoutcry[kk]  = 0.;
    eve.pxincry[kk]  = 0.;
    eve.pyincry[kk]  = 0.;
    eve.pzincry[kk]  = 0.;
    eve.pxoutcry[kk] = 0.;
    eve.pyoutcry[kk] = 0.;
    eve.pzoutcry[kk] = 0.;
    eve.decry[kk]    = 0.;
    eve.alcry[kk]    = 0.;
    eve.timcry[kk]   = 0.;    
  }
  
  for(int kk=0;kk<eve.ncross;kk++){
    eve.idcross[kk]      = 0;
    eve.nregcross[kk]    = 0;
    eve.nregoldcross[kk] = 0;
    eve.xcross[kk]       = 0.;
    eve.ycross[kk]       = 0.;
    eve.zcross[kk]       = 0.;
    eve.pxcross[kk]      = 0.;
    eve.pycross[kk]      = 0.;
    eve.pzcross[kk]      = 0.;
    eve.mcross[kk]       = 0.;
    eve.chcross[kk]      = 0.;
    eve.tcross[kk]       = 0.;
  }

  eve.num_event = 0;
  eve.nump      = 0;
  eve.nsc       = 0;
  eve.nvtx      = 0;
  eve.n1dc      = 0;
  eve.n2dc      = 0;
  eve.nscint    = 0;
  eve.ncry      = 0;
  eve.ncross    = 0;
  
  return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddPart(Int_t fidpa, Int_t figen, 
		      Int_t ficha, Int_t fnumreg, Int_t fiba,
		      Int_t fidead, Int_t fjpa,
		      Double_t fvxi, Double_t fvyi, Double_t fvzi,
		      Double_t fvxf, Double_t fvyf, Double_t fvzf,
		      Double_t fpx, Double_t fpy, Double_t fpz,
		      Double_t fpxf, Double_t fpyf, Double_t fpzf,
		      Double_t famass, Double_t ftempo,
		      Double_t ftof, Double_t ftrlen){

  if(eve.nump<MAXNUMP)
    {
      eve.nump ++;
      eve.idpa[eve.nump-1] = fidpa;
      eve.igen[eve.nump-1] = figen;
      eve.icha[eve.nump-1] = ficha;
      eve.numreg[eve.nump-1] = fnumreg;
      eve.iba[eve.nump-1] = fiba;
      eve.idead[eve.nump-1] = fidead;
      eve.jpa[eve.nump-1] = fjpa;
      eve.vxi[eve.nump-1] = fvxi;
      eve.vyi[eve.nump-1] = fvyi;
      eve.vzi[eve.nump-1] = fvzi;
      eve.vxf[eve.nump-1] = fvxf;
      eve.vyf[eve.nump-1] = fvyf;
      eve.vzf[eve.nump-1] = fvzf;
      eve.px[eve.nump-1] = fpx;
      eve.py[eve.nump-1] = fpy;
      eve.pz[eve.nump-1] = fpz;
      eve.pxf[eve.nump-1] = fpxf;
      eve.pyf[eve.nump-1] = fpyf;
      eve.pzf[eve.nump-1] = fpzf;
      eve.amass[eve.nump-1] = famass;
      eve.tempo[eve.nump-1] = ftempo;
      eve.tof[eve.nump-1] = ftof;
      eve.trlen[eve.nump-1] = ftrlen;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddSc(Int_t fidsc,
		    Double_t fxinsc, Double_t fyinsc, Double_t fzinsc,
		    Double_t fxoutsc, Double_t fyoutsc, Double_t fzoutsc, 
		    Double_t fpxinsc, Double_t fpyinsc, Double_t fpzinsc, 
		    Double_t fpxoutsc, Double_t fpyoutsc, Double_t fpzoutsc, 
		    Double_t fdesc, Double_t falsc, Double_t ftimsc){

  if(eve.nsc<MAXSC) {
    eve.nsc ++;
    eve.idsc[eve.nsc-1] = fidsc;
    eve.xinsc[eve.nsc-1] = fxinsc;
    eve.yinsc[eve.nsc-1] = fyinsc;
    eve.zinsc[eve.nsc-1] = fzinsc;
    eve.pxinsc[eve.nsc-1] = fpxinsc;
    eve.pyinsc[eve.nsc-1] = fpyinsc;
    eve.pzinsc[eve.nsc-1] = fpzinsc;
    eve.xoutsc[eve.nsc-1] = fxoutsc;
    eve.youtsc[eve.nsc-1] = fyoutsc;
    eve.zoutsc[eve.nsc-1] = fzoutsc;
    eve.pxoutsc[eve.nsc-1] = fpxoutsc;
    eve.pyoutsc[eve.nsc-1] = fpyoutsc;
    eve.pzoutsc[eve.nsc-1] = fpzoutsc;
    eve.desc[eve.nsc-1] = fdesc;
    eve.alsc[eve.nsc-1] = falsc;
    eve.timsc[eve.nsc-1] = ftimsc;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddVtx(Int_t fidvtx, Int_t fiplavtx,
		     Int_t firowvtx, Int_t ficolvtx,
		     Double_t fxinvtx, Double_t fyinvtx, Double_t fzinvtx,
		     Double_t fxoutvtx, Double_t fyoutvtx, Double_t fzoutvtx,
		     Double_t fpxinvtx, Double_t fpyinvtx, Double_t fpzinvtx,
		     Double_t fpxoutvtx, Double_t fpyoutvtx, Double_t fpzoutvtx,
		     Double_t fdevtx, Double_t falvtx, Double_t ftimvtx){

  if(eve.nvtx<MAXVTX)
    {
      eve.nvtx ++;
      eve.iplavtx[eve.nvtx-1] = fiplavtx;
      eve.idvtx[eve.nvtx-1] = fidvtx;
      eve.irowvtx[eve.nvtx-1] = firowvtx;
      eve.icolvtx[eve.nvtx-1] = ficolvtx;
      eve.xinvtx[eve.nvtx-1] = fxinvtx;
      eve.yinvtx[eve.nvtx-1] = fyinvtx;
      eve.zinvtx[eve.nvtx-1] = fzinvtx;
      eve.pxinvtx[eve.nvtx-1] = fpxinvtx;
      eve.pyinvtx[eve.nvtx-1] = fpyinvtx;
      eve.pzinvtx[eve.nvtx-1] = fpzinvtx;
      eve.xoutvtx[eve.nvtx-1] = fxoutvtx;
      eve.youtvtx[eve.nvtx-1] = fyoutvtx;
      eve.zoutvtx[eve.nvtx-1] = fzoutvtx;
      eve.pxoutvtx[eve.nvtx-1] = fpxoutvtx;
      eve.pyoutvtx[eve.nvtx-1] = fpyoutvtx;
      eve.pzoutvtx[eve.nvtx-1] = fpzoutvtx;
      eve.devtx[eve.nvtx-1] = fdevtx;
      eve.alvtx[eve.nvtx-1] = falvtx;
      eve.timvtx[eve.nvtx-1] = ftimvtx;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::Add1DC(Int_t fid1dc, Int_t fipla1dc, Int_t fiview1dc,
		     Int_t ficell1dc,
		     Double_t fxin1dc, Double_t fyin1dc, Double_t fzin1dc,
		     Double_t fxout1dc, Double_t fyout1dc, Double_t fzout1dc,
		     Double_t fpxin1dc, Double_t fpyin1dc, Double_t fpzin1dc,
		     Double_t fpxout1dc, Double_t fpyout1dc, Double_t fpzout1dc,
		     Double_t fde1dc, Double_t fal1dc, Double_t ftim1dc){

  if(eve.n1dc<MAX1DC)
    {
      eve.n1dc ++;
      eve.id1dc[eve.n1dc-1] = fid1dc;
      eve.ipla1dc[eve.n1dc-1] = fipla1dc;
      eve.iview1dc[eve.n1dc-1] = fiview1dc;
      eve.icell1dc[eve.n1dc-1] = ficell1dc;
      eve.xin1dc[eve.n1dc-1] = fxin1dc;
      eve.yin1dc[eve.n1dc-1] = fyin1dc;
      eve.zin1dc[eve.n1dc-1] = fzin1dc;
      eve.pxin1dc[eve.n1dc-1] = fpxin1dc;
      eve.pyin1dc[eve.n1dc-1] = fpyin1dc;
      eve.pzin1dc[eve.n1dc-1] = fpzin1dc;
      eve.xout1dc[eve.n1dc-1] = fxout1dc;
      eve.yout1dc[eve.n1dc-1] = fyout1dc;
      eve.zout1dc[eve.n1dc-1] = fzout1dc;
      eve.pxout1dc[eve.n1dc-1] = fpxout1dc;
      eve.pyout1dc[eve.n1dc-1] = fpyout1dc;
      eve.pzout1dc[eve.n1dc-1] = fpzout1dc;
      eve.de1dc[eve.n1dc-1] = fde1dc;
      eve.al1dc[eve.n1dc-1] = fal1dc;
      eve.tim1dc[eve.n1dc-1] = ftim1dc;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::Add2DC(Int_t fid2dc, Int_t fipla2dc, Int_t fiview2dc,
		     Int_t ficell2dc,
		     Double_t fxin2dc, Double_t fyin2dc, Double_t fzin2dc,
		     Double_t fxout2dc, Double_t fyout2dc, Double_t fzout2dc,
		     Double_t fpxin2dc, Double_t fpyin2dc, Double_t fpzin2dc,
		     Double_t fpxout2dc, Double_t fpyout2dc, Double_t fpzout2dc,
		     Double_t fde2dc, Double_t fal2dc, Double_t ftim2dc){
  
  if(eve.n2dc<MAX2DC)
    {
      eve.n2dc ++;
      eve.id2dc[eve.n2dc-1] = fid2dc;
      eve.ipla2dc[eve.n2dc-1] = fipla2dc;
      eve.iview2dc[eve.n2dc-1] = fiview2dc;
      eve.icell2dc[eve.n2dc-1] = ficell2dc;
      eve.xin2dc[eve.n2dc-1] = fxin2dc;
      eve.yin2dc[eve.n2dc-1] = fyin2dc;
      eve.zin2dc[eve.n2dc-1] = fzin2dc;
      eve.pxin2dc[eve.n2dc-1] = fpxin2dc;
      eve.pyin2dc[eve.n2dc-1] = fpyin2dc;
      eve.pzin2dc[eve.n2dc-1] = fpzin2dc;
      eve.xout2dc[eve.n2dc-1] = fxout2dc;
      eve.yout2dc[eve.n2dc-1] = fyout2dc;
      eve.zout2dc[eve.n2dc-1] = fzout2dc;
      eve.pxout2dc[eve.n2dc-1] = fpxout2dc;
      eve.pyout2dc[eve.n2dc-1] = fpyout2dc;
      eve.pzout2dc[eve.n2dc-1] = fpzout2dc;
      eve.de2dc[eve.n2dc-1] = fde2dc;
      eve.al2dc[eve.n2dc-1] = fal2dc;
      eve.tim2dc[eve.n2dc-1] = ftim2dc;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddScint(Int_t fidscint,
		       Int_t firowscint, Int_t ficolscint,
		       Double_t fxinscint, Double_t fyinscint, 
		       Double_t fzinscint,
		       Double_t fxoutscint, Double_t fyoutscint, 
		       Double_t fzoutscint, Double_t fpxinscint, 
		       Double_t fpyinscint, Double_t fpzinscint, 
		       Double_t fpxoutscint, Double_t fpyoutscint, 
		       Double_t fpzoutscint, Double_t fdescint, 
		       Double_t falscint, Double_t ftimscint){

  if(eve.nscint<MAXSCINT) {
    eve.nscint ++;
    eve.idscint[eve.nscint-1] = fidscint;
    eve.irowscint[eve.nscint-1] = firowscint;
    eve.icolscint[eve.nscint-1] = ficolscint;
    eve.xinscint[eve.nscint-1] = fxinscint;
    eve.yinscint[eve.nscint-1] = fyinscint;
    eve.zinscint[eve.nscint-1] = fzinscint;
    eve.pxinscint[eve.nscint-1] = fpxinscint;
    eve.pyinscint[eve.nscint-1] = fpyinscint;
    eve.pzinscint[eve.nscint-1] = fpzinscint;
    eve.xoutscint[eve.nscint-1] = fxoutscint;
    eve.youtscint[eve.nscint-1] = fyoutscint;
    eve.zoutscint[eve.nscint-1] = fzoutscint;
    eve.pxoutscint[eve.nscint-1] = fpxoutscint;
    eve.pyoutscint[eve.nscint-1] = fpyoutscint;
    eve.pzoutscint[eve.nscint-1] = fpzoutscint;
    eve.descint[eve.nscint-1] = fdescint;
    eve.alscint[eve.nscint-1] = falscint;
    eve.timscint[eve.nscint-1] = ftimscint;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddCry(Int_t fidcry, Int_t firowcry, Int_t ficolcry,
		     Double_t fxincry, Double_t fyincry,  Double_t fzincry,
		     Double_t fxoutcry, Double_t fyoutcry, Double_t fzoutcry,
		     Double_t fpxincry, Double_t fpyincry, Double_t fpzincry, 
		     Double_t fpxoutcry, Double_t fpyoutcry, Double_t fpzoutcry,
		     Double_t fdecry, Double_t falcry, Double_t ftimcry){

  if(eve.ncry<MAXCRY) {
    eve.ncry ++;
    eve.idcry[eve.ncry-1] = fidcry;
    eve.irowcry[eve.ncry-1] = firowcry;
    eve.icolcry[eve.ncry-1] = ficolcry;
    eve.xincry[eve.ncry-1] = fxincry;
    eve.yincry[eve.ncry-1] = fyincry;
    eve.zincry[eve.ncry-1] = fzincry;
    eve.pxincry[eve.ncry-1] = fpxincry;
    eve.pyincry[eve.ncry-1] = fpyincry;
    eve.pzincry[eve.ncry-1] = fpzincry;
    eve.xoutcry[eve.ncry-1] = fxoutcry;
    eve.youtcry[eve.ncry-1] = fyoutcry;
    eve.zoutcry[eve.ncry-1] = fzoutcry;
    eve.pxoutcry[eve.ncry-1] = fpxoutcry;
    eve.pyoutcry[eve.ncry-1] = fpyoutcry;
    eve.pzoutcry[eve.ncry-1] = fpzoutcry;
    eve.decry[eve.ncry-1] = fdecry;
    eve.alcry[eve.ncry-1] = falcry;
    eve.timcry[eve.ncry-1] = ftimcry;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddCross(Int_t fidcross, Int_t fnregcross, Int_t fnregoldcross,
		       Double_t fxcross, Double_t fycross, Double_t fzcross,
		       Double_t fpxcross, Double_t fpycross, Double_t fpzcross, 
		       Double_t fmcross, Double_t fchcross, Double_t ftcross){

  if(eve.ncross<MAXCROSS) {
    eve.ncross ++;
    eve.idcross[eve.ncross-1] = fidcross;
    eve.nregcross[eve.ncross-1] = fnregcross;
    eve.nregoldcross[eve.ncross-1] = fnregoldcross;
    eve.xcross[eve.ncross-1] = fxcross;
    eve.ycross[eve.ncross-1] = fycross;
    eve.zcross[eve.ncross-1] = fzcross;
    eve.pxcross[eve.ncross-1] = fpxcross;
    eve.pycross[eve.ncross-1] = fpycross;
    eve.pzcross[eve.ncross-1] = fpzcross;
    eve.mcross[eve.ncross-1] = fmcross;
    eve.chcross[eve.ncross-1] = fchcross;
    eve.tcross[eve.ncross-1] = ftcross;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

int Evento::FindBranches(TTree *RootTree, EVENTO_STRUCT *eve){

  RootTree->SetBranchAddress("nump",&(eve->nump));
  RootTree->SetBranchAddress("idpa",&(eve->idpa));
  RootTree->SetBranchAddress("igen",&(eve->igen));
  RootTree->SetBranchAddress("icha",&(eve->icha));
  RootTree->SetBranchAddress("numreg",&(eve->numreg));
  RootTree->SetBranchAddress("iba",&(eve->iba));
  RootTree->SetBranchAddress("idead",&(eve->idead));
  RootTree->SetBranchAddress("jpa",&(eve->jpa));
  RootTree->SetBranchAddress("vxi",&(eve->vxi));
  RootTree->SetBranchAddress("vyi",&(eve->vyi));
  RootTree->SetBranchAddress("vzi",&(eve->vzi));
  RootTree->SetBranchAddress("vxf",&(eve->vxf));
  RootTree->SetBranchAddress("vyf",&(eve->vyf));
  RootTree->SetBranchAddress("vzf",&(eve->vzf));
  RootTree->SetBranchAddress("px",&(eve->px));
  RootTree->SetBranchAddress("py",&(eve->py));
  RootTree->SetBranchAddress("pz",&(eve->pz));
  RootTree->SetBranchAddress("pxf",&(eve->pxf));
  RootTree->SetBranchAddress("pyf",&(eve->pyf));
  RootTree->SetBranchAddress("pzf",&(eve->pzf));
  RootTree->SetBranchAddress("amass",&(eve->amass));
  RootTree->SetBranchAddress("tempo",&(eve->tempo));
  RootTree->SetBranchAddress("tof",&(eve->tof));
  RootTree->SetBranchAddress("trlen",&(eve->trlen));

  RootTree->SetBranchAddress("nsc",&(eve->nsc));
  RootTree->SetBranchAddress("idsc",&(eve->idsc));
  RootTree->SetBranchAddress("xinsc",&(eve->xinsc));
  RootTree->SetBranchAddress("yinsc",&(eve->yinsc));
  RootTree->SetBranchAddress("zinsc",&(eve->zinsc));
  RootTree->SetBranchAddress("pxinsc",&(eve->pxinsc));
  RootTree->SetBranchAddress("pyinsc",&(eve->pyinsc));
  RootTree->SetBranchAddress("pzinsc",&(eve->pzinsc));
  RootTree->SetBranchAddress("xoutsc",&(eve->xoutsc));
  RootTree->SetBranchAddress("youtsc",&(eve->youtsc));
  RootTree->SetBranchAddress("zoutsc",&(eve->zoutsc));
  RootTree->SetBranchAddress("pxoutsc",&(eve->pxoutsc));
  RootTree->SetBranchAddress("pyoutsc",&(eve->pyoutsc));
  RootTree->SetBranchAddress("pzoutsc",&(eve->pzoutsc));
  RootTree->SetBranchAddress("desc",&(eve->desc));
  RootTree->SetBranchAddress("alsc",&(eve->alsc));
  RootTree->SetBranchAddress("timsc",&(eve->timsc));

  RootTree->SetBranchAddress("nvtx",&(eve->nvtx));
  RootTree->SetBranchAddress("idvtx",&(eve->idvtx));
  RootTree->SetBranchAddress("iplavtx",&(eve->iplavtx));
  RootTree->SetBranchAddress("irowvtx",&(eve->irowvtx));
  RootTree->SetBranchAddress("icolvtx",&(eve->icolvtx));
  RootTree->SetBranchAddress("xinvtx",&(eve->xinvtx));
  RootTree->SetBranchAddress("yinvtx",&(eve->yinvtx));
  RootTree->SetBranchAddress("zinvtx",&(eve->zinvtx));
  RootTree->SetBranchAddress("pxinvtx",&(eve->pxinvtx));
  RootTree->SetBranchAddress("pyinvtx",&(eve->pyinvtx));
  RootTree->SetBranchAddress("pzinvtx",&(eve->pzinvtx));
  RootTree->SetBranchAddress("xoutvtx",&(eve->xoutvtx));
  RootTree->SetBranchAddress("youtvtx",&(eve->youtvtx));
  RootTree->SetBranchAddress("zoutvtx",&(eve->zoutvtx));
  RootTree->SetBranchAddress("pxoutvtx",&(eve->pxoutvtx));
  RootTree->SetBranchAddress("pyoutvtx",&(eve->pyoutvtx));
  RootTree->SetBranchAddress("pzoutvtx",&(eve->pzoutvtx));
  RootTree->SetBranchAddress("devtx",&(eve->devtx));
  RootTree->SetBranchAddress("alvtx",&(eve->alvtx));
  RootTree->SetBranchAddress("timvtx",&(eve->timvtx));

  RootTree->SetBranchAddress("n1dc",&(eve->n1dc));
  RootTree->SetBranchAddress("id1dc",&(eve->id1dc));
  RootTree->SetBranchAddress("ipla1dc",&(eve->ipla1dc));
  RootTree->SetBranchAddress("iview1dc",&(eve->iview1dc));
  RootTree->SetBranchAddress("icell1dc",&(eve->icell1dc));
  RootTree->SetBranchAddress("xin1dc",&(eve->xin1dc));
  RootTree->SetBranchAddress("yin1dc",&(eve->yin1dc));
  RootTree->SetBranchAddress("zin1dc",&(eve->zin1dc));
  RootTree->SetBranchAddress("pxin1dc",&(eve->pxin1dc));
  RootTree->SetBranchAddress("pyin1dc",&(eve->pyin1dc));
  RootTree->SetBranchAddress("pzin1dc",&(eve->pzin1dc));
  RootTree->SetBranchAddress("xout1dc",&(eve->xout1dc));
  RootTree->SetBranchAddress("yout1dc",&(eve->yout1dc));
  RootTree->SetBranchAddress("zout1dc",&(eve->zout1dc));
  RootTree->SetBranchAddress("pxout1dc",&(eve->pxout1dc));
  RootTree->SetBranchAddress("pyout1dc",&(eve->pyout1dc));
  RootTree->SetBranchAddress("pzout1dc",&(eve->pzout1dc));
  RootTree->SetBranchAddress("de1dc",&(eve->de1dc));
  RootTree->SetBranchAddress("al1dc",&(eve->al1dc));
  RootTree->SetBranchAddress("tim1dc",&(eve->tim1dc));

  RootTree->SetBranchAddress("n2dc",&(eve->n2dc));
  RootTree->SetBranchAddress("id2dc",&(eve->id2dc));
  RootTree->SetBranchAddress("ipla2dc",&(eve->ipla2dc));
  RootTree->SetBranchAddress("iview2dc",&(eve->iview2dc));
  RootTree->SetBranchAddress("icell2dc",&(eve->icell2dc));
  RootTree->SetBranchAddress("xin2dc",&(eve->xin2dc));
  RootTree->SetBranchAddress("yin2dc",&(eve->yin2dc));
  RootTree->SetBranchAddress("zin2dc",&(eve->zin2dc));
  RootTree->SetBranchAddress("pxin2dc",&(eve->pxin2dc));
  RootTree->SetBranchAddress("pyin2dc",&(eve->pyin2dc));
  RootTree->SetBranchAddress("pzin2dc",&(eve->pzin2dc));
  RootTree->SetBranchAddress("xout2dc",&(eve->xout2dc));
  RootTree->SetBranchAddress("yout2dc",&(eve->yout2dc));
  RootTree->SetBranchAddress("zout2dc",&(eve->zout2dc));
  RootTree->SetBranchAddress("pxout2dc",&(eve->pxout2dc));
  RootTree->SetBranchAddress("pyout2dc",&(eve->pyout2dc));
  RootTree->SetBranchAddress("pzout2dc",&(eve->pzout2dc));
  RootTree->SetBranchAddress("de2dc",&(eve->de2dc));
  RootTree->SetBranchAddress("al2dc",&(eve->al2dc));
  RootTree->SetBranchAddress("tim2dc",&(eve->tim2dc));
  
  RootTree->SetBranchAddress("nscint",&(eve->nscint));
  RootTree->SetBranchAddress("idscint",&(eve->idscint));
  RootTree->SetBranchAddress("irowscint",&(eve->irowscint));
  RootTree->SetBranchAddress("icolscint",&(eve->icolscint));
  RootTree->SetBranchAddress("xinscint",&(eve->xinscint));
  RootTree->SetBranchAddress("yinscint",&(eve->yinscint));
  RootTree->SetBranchAddress("zinscint",&(eve->zinscint));
  RootTree->SetBranchAddress("pxinscint",&(eve->pxinscint));
  RootTree->SetBranchAddress("pyinscint",&(eve->pyinscint));
  RootTree->SetBranchAddress("pzinscint",&(eve->pzinscint));
  RootTree->SetBranchAddress("xoutscint",&(eve->xoutscint));
  RootTree->SetBranchAddress("youtscint",&(eve->youtscint));
  RootTree->SetBranchAddress("zoutscint",&(eve->zoutscint));
  RootTree->SetBranchAddress("pxoutscint",&(eve->pxoutscint));
  RootTree->SetBranchAddress("pyoutscint",&(eve->pyoutscint));
  RootTree->SetBranchAddress("pzoutscint",&(eve->pzoutscint));
  RootTree->SetBranchAddress("descint",&(eve->descint));
  RootTree->SetBranchAddress("alscint",&(eve->alscint));
  RootTree->SetBranchAddress("timscint",&(eve->timscint));

  RootTree->SetBranchAddress("ncry",&(eve->ncry));
  RootTree->SetBranchAddress("idcry",&(eve->idcry));
  RootTree->SetBranchAddress("irowcry",&(eve->irowcry));
  RootTree->SetBranchAddress("icolcry",&(eve->icolcry));
  RootTree->SetBranchAddress("xincry",&(eve->xincry));
  RootTree->SetBranchAddress("yincry",&(eve->yincry));
  RootTree->SetBranchAddress("zincry",&(eve->zincry));
  RootTree->SetBranchAddress("pxincry",&(eve->pxincry));
  RootTree->SetBranchAddress("pyincry",&(eve->pyincry));
  RootTree->SetBranchAddress("pzincry",&(eve->pzincry));
  RootTree->SetBranchAddress("xoutcry",&(eve->xoutcry));
  RootTree->SetBranchAddress("youtcry",&(eve->youtcry));
  RootTree->SetBranchAddress("zoutcry",&(eve->zoutcry));
  RootTree->SetBranchAddress("pxoutcry",&(eve->pxoutcry));
  RootTree->SetBranchAddress("pyoutcry",&(eve->pyoutcry));
  RootTree->SetBranchAddress("pzoutcry",&(eve->pzoutcry));
  RootTree->SetBranchAddress("decry",&(eve->decry));
  RootTree->SetBranchAddress("alcry",&(eve->alcry));
  RootTree->SetBranchAddress("timcry",&(eve->timcry));

  RootTree->SetBranchAddress("ncross",&(eve->ncross));
  RootTree->SetBranchAddress("idcross",&(eve->idcross));
  RootTree->SetBranchAddress("nregcross",&(eve->nregcross));
  RootTree->SetBranchAddress("xcross",&(eve->xcross));
  RootTree->SetBranchAddress("ycross",&(eve->ycross));
  RootTree->SetBranchAddress("zcross",&(eve->zcross));
  RootTree->SetBranchAddress("pxcross",&(eve->pxcross));
  RootTree->SetBranchAddress("pycross",&(eve->pycross));
  RootTree->SetBranchAddress("pzcross",&(eve->pzcross));
  RootTree->SetBranchAddress("mcross",&(eve->mcross));
  RootTree->SetBranchAddress("chcross",&(eve->chcross));
  RootTree->SetBranchAddress("tcross",&(eve->tcross));
  return 0;

}


/*-----------------------------------------------------------------*/


Int_t Evento::Dump(){
  return 0;

}

/*-----------------------------------------------------------------*/


EVENTO_STRUCT Evento::Output(){
  return eve;
}


/*-----------------------------------------------------------------*/

Evento::~Evento()
{
}


