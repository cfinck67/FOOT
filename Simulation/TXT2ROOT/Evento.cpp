#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Evento.h"

using namespace std;

ClassImp(Evento);

/*-----------------------------------------------------------------*/
  
Evento::Evento()
{

  ar3 = new TRandom3();
  ar3->SetSeed(12345);

  eve.trn    = 0;
  eve.stn     = 0;
  eve.nvtx    = 0;
  eve.nmon    = 0;
  eve.n2dc    = 0;
  eve.nscint  = 0;
  eve.ncry    = 0;
  eve.ncross  = 0;

  for(int kk=0;kk<MAXNUMP;kk++){
    eve.trpaid[kk]   = 0;
    eve.trgen[kk]   = 0;
    eve.trcha[kk]   = -100;
    eve.trreg[kk] = 0;
    eve.trbar[kk]    = 0;
    eve.trdead[kk]  = 0;
    eve.trfid[kk]    = -100;
    eve.trix[kk]    = 0.;
    eve.triy[kk]    = 0.;
    eve.triz[kk]    = 0.;
    eve.trfx[kk]    = 0.;
    eve.trfy[kk]    = 0.;
    eve.trfz[kk]    = 0.;
    eve.tripx[kk]     = 0.;
    eve.tripy[kk]     = 0.;
    eve.tripz[kk]     = 0.;
    eve.trfpx[kk]    = 0.;
    eve.trfpy[kk]    = 0.;
    eve.trfpz[kk]    = 0.;
    eve.trmass[kk]  = 0.;
    eve.trtime[kk]  = 0.;
    eve.tof[kk]    = 0.;
    eve.trlen[kk]  = 0.;
  }

  for(int kk=0;kk<MAXSC;kk++){
    eve.stid[kk]    = 0;
    eve.stinx[kk]   = 0.;
    eve.stiny[kk]   = 0.;
    eve.stinz[kk]   = 0.;
    eve.stoutx[kk]  = 0.;
    eve.stouty[kk]  = 0.;
    eve.stoutz[kk]  = 0.;
    eve.stinpx[kk]  = 0.;
    eve.stinpy[kk]  = 0.;
    eve.stinpz[kk]  = 0.;
    eve.stoutpx[kk] = 0.;
    eve.stoutpy[kk] = 0.;
    eve.stoutpz[kk] = 0.;
    eve.stde[kk]    = 0.;
    eve.stal[kk]    = 0.;
    eve.sttim[kk]   = 0.;
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
    eve.idmon[kk]    = 0;
    eve.ilayer[kk]  = 0;
    eve.iview[kk] = 0;
    eve.icell[kk] = 0;
    eve.xinmon[kk]   = 0.;
    eve.yinmon[kk]   = 0.;
    eve.zinmon[kk]   = 0.;
    eve.xoutmon[kk]  = 0.;
    eve.youtmon[kk]  = 0.;
    eve.zoutmon[kk]  = 0.;
    eve.pxinmon[kk]  = 0.;
    eve.pyinmon[kk]  = 0.;
    eve.pzinmon[kk]  = 0.;
    eve.pxoutmon[kk] = 0.;
    eve.pyoutmon[kk] = 0.;
    eve.pzoutmon[kk] = 0.;
    eve.demon[kk]    = 0.;
    eve.almon[kk]    = 0.;
    eve.timmon[kk]   = 0.;
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
  eve.EventNumber = fnumero_evento;
  return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::Clean(){
  //  cout <<" Entro in clean"<<endl;

  for(int kk=0;kk<eve.trn;kk++){
    eve.trpaid[kk]   = 0;
    eve.trgen[kk]   = 0;
    eve.trcha[kk]   = -100;
    eve.trreg[kk] = 0;
    eve.trbar[kk]    = 0;
    eve.trdead[kk]  = 0;
    eve.trfid[kk]    = -100;
    eve.trix[kk]    = 0.;
    eve.triy[kk]    = 0.;
    eve.triz[kk]    = 0.;
    eve.trfx[kk]    = 0.;
    eve.trfy[kk]    = 0.;
    eve.trfz[kk]    = 0.;
    eve.tripx[kk]     = 0.;
    eve.tripy[kk]     = 0.;
    eve.tripz[kk]     = 0.;
    eve.trfpx[kk]    = 0.;
    eve.trfpy[kk]    = 0.;
    eve.trfpz[kk]    = 0.;
    eve.trmass[kk]  = 0.;
    eve.trtime[kk]  = 0.;
    eve.tof[kk]    = 0.;
    eve.trlen[kk]  = 0.;
  }

  for(int kk=0;kk<eve.stn;kk++){
    eve.stid[kk]    = 0;
    eve.stinx[kk]   = 0.;
    eve.stiny[kk]   = 0.;
    eve.stinz[kk]   = 0.;
    eve.stoutx[kk]  = 0.;
    eve.stouty[kk]  = 0.;
    eve.stoutz[kk]  = 0.;
    eve.stinpx[kk]  = 0.;
    eve.stinpy[kk]  = 0.;
    eve.stinpz[kk]  = 0.;
    eve.stoutpx[kk] = 0.;
    eve.stoutpy[kk] = 0.;
    eve.stoutpz[kk] = 0.;
    eve.stde[kk]    = 0.;
    eve.stal[kk]    = 0.;
    eve.sttim[kk]   = 0.;
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

  for(int kk=0;kk<eve.nmon;kk++){
    eve.idmon[kk]    = 0;
    eve.ilayer[kk]  = 0;
    eve.iview[kk] = 0;
    eve.icell[kk] = 0;
    eve.xinmon[kk]   = 0.;
    eve.yinmon[kk]   = 0.;
    eve.zinmon[kk]   = 0.;
    eve.xoutmon[kk]  = 0.;
    eve.youtmon[kk]  = 0.;
    eve.zoutmon[kk]  = 0.;
    eve.pxinmon[kk]  = 0.;
    eve.pyinmon[kk]  = 0.;
    eve.pzinmon[kk]  = 0.;
    eve.pxoutmon[kk] = 0.;
    eve.pyoutmon[kk] = 0.;
    eve.pzoutmon[kk] = 0.;
    eve.demon[kk]    = 0.;
    eve.almon[kk]    = 0.;
    eve.timmon[kk]   = 0.;
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

  eve.EventNumber = 0;
  eve.trn      = 0;
  eve.stn       = 0;
  eve.nvtx      = 0;
  eve.nmon      = 0;
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

  if(eve.trn<MAXNUMP)
    {
      eve.trn ++;
      eve.trpaid[eve.trn-1] = fidpa;
      eve.trgen[eve.trn-1] = figen;
      eve.trcha[eve.trn-1] = ficha;
      eve.trreg[eve.trn-1] = fnumreg;
      eve.trbar[eve.trn-1] = fiba;
      eve.trdead[eve.trn-1] = fidead;
      eve.trfid[eve.trn-1] = fjpa;
      eve.trix[eve.trn-1] = fvxi;
      eve.triy[eve.trn-1] = fvyi;
      eve.triz[eve.trn-1] = fvzi;
      eve.trfx[eve.trn-1] = fvxf;
      eve.trfy[eve.trn-1] = fvyf;
      eve.trfz[eve.trn-1] = fvzf;
      eve.tripx[eve.trn-1] = fpx;
      eve.tripy[eve.trn-1] = fpy;
      eve.tripz[eve.trn-1] = fpz;
      eve.trfpx[eve.trn-1] = fpxf;
      eve.trfpy[eve.trn-1] = fpyf;
      eve.trfpz[eve.trn-1] = fpzf;
      eve.trmass[eve.trn-1] = famass;
      eve.trtime[eve.trn-1] = ftempo;
      eve.tof[eve.trn-1] = ftof;
      eve.trlen[eve.trn-1] = ftrlen;
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

  if(eve.stn<MAXSC) {
    eve.stn ++;
    eve.stid[eve.stn-1] = fidsc;
    eve.stinx[eve.stn-1] = fxinsc;
    eve.stiny[eve.stn-1] = fyinsc;
    eve.stinz[eve.stn-1] = fzinsc;
    eve.stinpx[eve.stn-1] = fpxinsc;
    eve.stinpy[eve.stn-1] = fpyinsc;
    eve.stinpz[eve.stn-1] = fpzinsc;
    eve.stoutx[eve.stn-1] = fxoutsc;
    eve.stouty[eve.stn-1] = fyoutsc;
    eve.stoutz[eve.stn-1] = fzoutsc;
    eve.stoutpx[eve.stn-1] = fpxoutsc;
    eve.stoutpy[eve.stn-1] = fpyoutsc;
    eve.stoutpz[eve.stn-1] = fpzoutsc;
    eve.stde[eve.stn-1] = fdesc;
    eve.stal[eve.stn-1] = falsc;
    eve.sttim[eve.stn-1] = ftimsc;
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

  if(eve.nmon<MAX1DC)
    {
      eve.nmon ++;
      eve.idmon[eve.nmon-1] = fid1dc;
      eve.ilayer[eve.nmon-1] = fipla1dc;
      eve.iview[eve.nmon-1] = fiview1dc;
      eve.icell[eve.nmon-1] = ficell1dc;
      eve.xinmon[eve.nmon-1] = fxin1dc;
      eve.yinmon[eve.nmon-1] = fyin1dc;
      eve.zinmon[eve.nmon-1] = fzin1dc;
      eve.pxinmon[eve.nmon-1] = fpxin1dc;
      eve.pyinmon[eve.nmon-1] = fpyin1dc;
      eve.pzinmon[eve.nmon-1] = fpzin1dc;
      eve.xoutmon[eve.nmon-1] = fxout1dc;
      eve.youtmon[eve.nmon-1] = fyout1dc;
      eve.zoutmon[eve.nmon-1] = fzout1dc;
      eve.pxoutmon[eve.nmon-1] = fpxout1dc;
      eve.pyoutmon[eve.nmon-1] = fpyout1dc;
      eve.pzoutmon[eve.nmon-1] = fpzout1dc;
      eve.demon[eve.nmon-1] = fde1dc;
      eve.almon[eve.nmon-1] = fal1dc;
      eve.timmon[eve.nmon-1] = ftim1dc;
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

int Evento::FindBranches(TTree *RootTree, EVENT_STRUCT *eve){

  RootTree->SetBranchAddress("EventNumber",&(eve->EventNumber));
  RootTree->SetBranchAddress("trn",&(eve->trn));
  RootTree->SetBranchAddress("trpaid",&(eve->trpaid));
  RootTree->SetBranchAddress("trgen",&(eve->trgen));
  RootTree->SetBranchAddress("trcha",&(eve->trcha));
  RootTree->SetBranchAddress("trreg",&(eve->trreg));
  RootTree->SetBranchAddress("trbar",&(eve->trbar));
  RootTree->SetBranchAddress("trdead",&(eve->trdead));
  RootTree->SetBranchAddress("trfid",&(eve->trfid));
  RootTree->SetBranchAddress("trix",&(eve->trix));
  RootTree->SetBranchAddress("triy",&(eve->triy));
  RootTree->SetBranchAddress("triz",&(eve->triz));
  RootTree->SetBranchAddress("trfx",&(eve->trfx));
  RootTree->SetBranchAddress("trfy",&(eve->trfy));
  RootTree->SetBranchAddress("trfz",&(eve->trfz));
  RootTree->SetBranchAddress("trip",&(eve->tripx));
  RootTree->SetBranchAddress("tripy",&(eve->tripy));
  RootTree->SetBranchAddress("tripz",&(eve->tripz));
  RootTree->SetBranchAddress("trfpx",&(eve->trfpx));
  RootTree->SetBranchAddress("trfpy",&(eve->trfpy));
  RootTree->SetBranchAddress("trfpz",&(eve->trfpz));
  RootTree->SetBranchAddress("trmass",&(eve->trmass));
  RootTree->SetBranchAddress("trtime",&(eve->trtime));
  RootTree->SetBranchAddress("tof",&(eve->tof));
  RootTree->SetBranchAddress("trlen",&(eve->trlen));

  RootTree->SetBranchAddress("stn",&(eve->stn));
  RootTree->SetBranchAddress("stid",&(eve->stid));
  RootTree->SetBranchAddress("stinx",&(eve->stinx));
  RootTree->SetBranchAddress("stiny",&(eve->stiny));
  RootTree->SetBranchAddress("stinz",&(eve->stinz));
  RootTree->SetBranchAddress("stinpx",&(eve->stinpx));
  RootTree->SetBranchAddress("stinpy",&(eve->stinpy));
  RootTree->SetBranchAddress("stinpz",&(eve->stinpz));
  RootTree->SetBranchAddress("stoutx",&(eve->stoutx));
  RootTree->SetBranchAddress("stouty",&(eve->stouty));
  RootTree->SetBranchAddress("stoutz",&(eve->stoutz));
  RootTree->SetBranchAddress("stoutpx",&(eve->stoutpx));
  RootTree->SetBranchAddress("stoutpy",&(eve->stoutpy));
  RootTree->SetBranchAddress("stoutpz",&(eve->stoutpz));
  RootTree->SetBranchAddress("stde",&(eve->stde));
  RootTree->SetBranchAddress("stal",&(eve->stal));
  RootTree->SetBranchAddress("sttim",&(eve->sttim));

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

  RootTree->SetBranchAddress("nmon",&(eve->nmon));
  RootTree->SetBranchAddress("idmon",&(eve->idmon));
  RootTree->SetBranchAddress("ilayer",&(eve->ilayer));
  RootTree->SetBranchAddress("iview",&(eve->iview));
  RootTree->SetBranchAddress("icell",&(eve->icell));
  RootTree->SetBranchAddress("xinmon",&(eve->xinmon));
  RootTree->SetBranchAddress("yinmon",&(eve->yinmon));
  RootTree->SetBranchAddress("zinmon",&(eve->zinmon));
  RootTree->SetBranchAddress("pxinmon",&(eve->pxinmon));
  RootTree->SetBranchAddress("pyinmon",&(eve->pyinmon));
  RootTree->SetBranchAddress("pzinmon",&(eve->pzinmon));
  RootTree->SetBranchAddress("xoutmon",&(eve->xoutmon));
  RootTree->SetBranchAddress("youtmon",&(eve->youtmon));
  RootTree->SetBranchAddress("zoutmon",&(eve->zoutmon));
  RootTree->SetBranchAddress("pxoutmon",&(eve->pxoutmon));
  RootTree->SetBranchAddress("pyoutmon",&(eve->pyoutmon));
  RootTree->SetBranchAddress("pzoutmon",&(eve->pzoutmon));
  RootTree->SetBranchAddress("demon",&(eve->demon));
  RootTree->SetBranchAddress("almon",&(eve->almon));
  RootTree->SetBranchAddress("timmon",&(eve->timmon));

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


Int_t Evento::EDump(){
  return 0;

}

/*-----------------------------------------------------------------*/


EVENT_STRUCT Evento::Output(){
  return eve;
}


/*-----------------------------------------------------------------*/

Evento::~Evento()
{
}


