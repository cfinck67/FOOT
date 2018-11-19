#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Evento.h"

using namespace std;

ClassImp(Evento);

/*-----------------------------------------------------------------*/
  
Evento::Evento()
{

  //  ar3 = new TRandom3();
  //  ar3->SetSeed(12345);
  
  eve.TRn    = 0;
  eve.STCn    = 0;
  eve.VTXn    = 0;
  eve.ITRn    = 0;
  eve.BMNn    = 0;
  eve.MSDn    = 0;
  eve.SCNn    = 0;
  eve.CALn    = 0;
  eve.CROSSn  = 0;

  for(int kk=0;kk<MAXTR;kk++){
    eve.TRpaid[kk]   = 0;
    eve.TRgen[kk]    = 0;
    eve.TRcha[kk]    = -100;
    eve.TRreg[kk]    = 0;
    eve.TRbar[kk]    = 0;
    eve.TRdead[kk]   = 0;
    eve.TRfid[kk]    = -100;
    eve.TRix[kk]     = 0.;
    eve.TRiy[kk]     = 0.;
    eve.TRiz[kk]     = 0.;
    eve.TRfx[kk]     = 0.;
    eve.TRfy[kk]     = 0.;
    eve.TRfz[kk]     = 0.;
    eve.TRipx[kk]    = 0.;
    eve.TRipy[kk]    = 0.;
    eve.TRipz[kk]    = 0.;
    eve.TRfpx[kk]    = 0.;
    eve.TRfpy[kk]    = 0.;
    eve.TRfpz[kk]    = 0.;
    eve.TRmass[kk]   = 0.;
    eve.TRtime[kk]   = 0.;
    eve.TRtof[kk]    = 0.;
    eve.TRtrlen[kk]  = 0.;
  }

  for(int kk=0;kk<MAXSTC;kk++){
    eve.STCid[kk]    = 0;
    eve.STCxin[kk]   = 0.;
    eve.STCyin[kk]   = 0.;
    eve.STCzin[kk]   = 0.;
    eve.STCxout[kk]  = 0.;
    eve.STCyout[kk]  = 0.;
    eve.STCzout[kk]  = 0.;
    eve.STCpxin[kk]  = 0.;
    eve.STCpyin[kk]  = 0.;
    eve.STCpzin[kk]  = 0.;
    eve.STCpxout[kk] = 0.;
    eve.STCpyout[kk] = 0.;
    eve.STCpzout[kk] = 0.;
    eve.STCde[kk]    = 0.;
    eve.STCal[kk]    = 0.;
    eve.STCtim[kk]   = 0.;
  }
  
  for(int kk=0;kk<MAXBMN;kk++){
    eve.BMNid[kk]    = 0;
    eve.BMNilay[kk]  = 0;
    eve.BMNiview[kk] = 0;
    eve.BMNicell[kk] = 0;
    eve.BMNxin[kk]   = 0.;
    eve.BMNyin[kk]   = 0.;
    eve.BMNzin[kk]   = 0.;
    eve.BMNxout[kk]  = 0.;
    eve.BMNyout[kk]  = 0.;
    eve.BMNzout[kk]  = 0.;
    eve.BMNpxin[kk]  = 0.;
    eve.BMNpyin[kk]  = 0.;
    eve.BMNpzin[kk]  = 0.;
    eve.BMNpxout[kk] = 0.;
    eve.BMNpyout[kk] = 0.;
    eve.BMNpzout[kk] = 0.;
    eve.BMNde[kk]    = 0.;
    eve.BMNal[kk]    = 0.;
    eve.BMNtim[kk]   = 0.;
  }
  
  for(int kk=0;kk<MAXVTX;kk++){
    eve.VTXid[kk]    = 0;
    eve.VTXilay[kk]  = 0;
    eve.VTXirow[kk]  = 0;
    eve.VTXicol[kk]  = 0;
    eve.VTXxin[kk]   = 0.;
    eve.VTXyin[kk]   = 0.;
    eve.VTXzin[kk]   = 0.;
    eve.VTXxout[kk]  = 0.;
    eve.VTXyout[kk]  = 0.;
    eve.VTXzout[kk]  = 0.;
    eve.VTXpxin[kk]  = 0.;
    eve.VTXpyin[kk]  = 0.;
    eve.VTXpzin[kk]  = 0.;
    eve.VTXpxout[kk] = 0.;
    eve.VTXpyout[kk] = 0.;
    eve.VTXpzout[kk] = 0.;
    eve.VTXde[kk]    = 0.;
    eve.VTXal[kk]    = 0.;
    eve.VTXtim[kk]   = 0.;
  }

  for(int kk=0;kk<MAXITR;kk++){
    eve.ITRid[kk]    = 0;
    eve.ITRiplume[kk]  = 0;
    eve.ITRimimo[kk]  = 0;
    eve.ITRilay[kk]  = 0;
    eve.ITRirow[kk]  = 0;
    eve.ITRicol[kk]  = 0;
    eve.ITRxin[kk]   = 0.;
    eve.ITRyin[kk]   = 0.;
    eve.ITRzin[kk]   = 0.;
    eve.ITRxout[kk]  = 0.;
    eve.ITRyout[kk]  = 0.;
    eve.ITRzout[kk]  = 0.;
    eve.ITRpxin[kk]  = 0.;
    eve.ITRpyin[kk]  = 0.;
    eve.ITRpzin[kk]  = 0.;
    eve.ITRpxout[kk] = 0.;
    eve.ITRpyout[kk] = 0.;
    eve.ITRpzout[kk] = 0.;
    eve.ITRde[kk]    = 0.;
    eve.ITRal[kk]    = 0.;
    eve.ITRtim[kk]   = 0.;
  }

  for(int kk=0;kk<MAXMSD;kk++){
    eve.MSDid[kk]    = 0;
    eve.MSDilay[kk]  = 0;
    eve.MSDistripx[kk]= 0;
    eve.MSDistripy[kk]= 0;
    eve.MSDxin[kk]   = 0.;
    eve.MSDyin[kk]   = 0.;
    eve.MSDzin[kk]   = 0.;
    eve.MSDxout[kk]  = 0.;
    eve.MSDyout[kk]  = 0.;
    eve.MSDzout[kk]  = 0.;
    eve.MSDpxin[kk]  = 0.;
    eve.MSDpyin[kk]  = 0.;
    eve.MSDpzin[kk]  = 0.;
    eve.MSDpxout[kk] = 0.;
    eve.MSDpyout[kk] = 0.;
    eve.MSDpzout[kk] = 0.;
    eve.MSDde[kk]    = 0.;
    eve.MSDal[kk]    = 0.;
    eve.MSDtim[kk]   = 0.;
  }
  
  for(int kk=0;kk<MAXSCN;kk++){
    eve.SCNid[kk]     = 0;
    eve.SCNibar[kk] = 0;
    eve.SCNiview[kk]  = 0;
    eve.SCNxin[kk]    = 0.;
    eve.SCNyin[kk]    = 0.;
    eve.SCNzin[kk]    = 0.;
    eve.SCNxout[kk]   = 0.;
    eve.SCNyout[kk]   = 0.;
    eve.SCNzout[kk]   = 0.;
    eve.SCNpxin[kk]   = 0.;
    eve.SCNpyin[kk]   = 0.;
    eve.SCNpzin[kk]   = 0.;
    eve.SCNpxout[kk]  = 0.;
    eve.SCNpyout[kk]  = 0.;
    eve.SCNpzout[kk]  = 0.;
    eve.SCNde[kk]     = 0.;
    eve.SCNal[kk]     = 0.;
    eve.SCNtim[kk]    = 0.;
  }

  for(int kk=0;kk<MAXCAL;kk++){
    eve.CALid[kk]    = 0;
    eve.CALicry[kk]  = 0;
    eve.CALxin[kk]   = 0.;
    eve.CALyin[kk]   = 0.;
    eve.CALzin[kk]   = 0.;
    eve.CALxout[kk]  = 0.;
    eve.CALyout[kk]  = 0.;
    eve.CALzout[kk]  = 0.;
    eve.CALpxin[kk]  = 0.;
    eve.CALpyin[kk]  = 0.;
    eve.CALpzin[kk]  = 0.;
    eve.CALpxout[kk] = 0.;
    eve.CALpyout[kk] = 0.;
    eve.CALpzout[kk] = 0.;
    eve.CALde[kk]    = 0.;
    eve.CALal[kk]    = 0.;
    eve.CALtim[kk]   = 0.;    
  }

  for(int kk=0;kk<MAXCROSS;kk++){
    eve.CROSSid[kk]      = 0;
    eve.CROSSnreg[kk]    = 0;
    eve.CROSSnregold[kk] = 0;
    eve.CROSSx[kk]       = 0.;
    eve.CROSSy[kk]       = 0.;
    eve.CROSSz[kk]       = 0.;
    eve.CROSSpx[kk]      = 0.;
    eve.CROSSpy[kk]      = 0.;
    eve.CROSSpz[kk]      = 0.;
    eve.CROSSm[kk]       = 0.;
    eve.CROSSch[kk]      = 0.;
    eve.CROSSt[kk]       = 0.;
  }

}

/*-----------------------------------------------------------------*/

Int_t Evento::SetEvent(Int_t fEventNumber){
  //  cout <<" Entro in SetEvent"<<endl;
  eve.EventNumber = fEventNumber;
  return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::Clean(){
  //  cout <<" Entro in clean"<<endl;

  for(int kk=0;kk<eve.TRn;kk++){
    eve.TRpaid[kk]  = 0;
    eve.TRgen[kk]   = 0;
    eve.TRcha[kk]   = -100;
    eve.TRreg[kk]   = 0;
    eve.TRbar[kk]   = 0;
    eve.TRdead[kk]  = 0;
    eve.TRfid[kk]   = -100;
    eve.TRix[kk]    = 0.;
    eve.TRiy[kk]    = 0.;
    eve.TRiz[kk]    = 0.;
    eve.TRfx[kk]    = 0.;
    eve.TRfy[kk]    = 0.;
    eve.TRfz[kk]    = 0.;
    eve.TRipx[kk]   = 0.;
    eve.TRipy[kk]   = 0.;
    eve.TRipz[kk]   = 0.;
    eve.TRfpx[kk]   = 0.;
    eve.TRfpy[kk]   = 0.;
    eve.TRfpz[kk]   = 0.;
    eve.TRmass[kk]  = 0.;
    eve.TRtime[kk]  = 0.;
    eve.TRtof[kk]   = 0.;
    eve.TRtrlen[kk] = 0.;
  }

  for(int kk=0;kk<eve.STCn;kk++){
    eve.STCid[kk]    = 0;
    eve.STCxin[kk]   = 0.;
    eve.STCyin[kk]   = 0.;
    eve.STCzin[kk]   = 0.;
    eve.STCxout[kk]  = 0.;
    eve.STCyout[kk]  = 0.;
    eve.STCzout[kk]  = 0.;
    eve.STCpxin[kk]  = 0.;
    eve.STCpyin[kk]  = 0.;
    eve.STCpzin[kk]  = 0.;
    eve.STCpxout[kk] = 0.;
    eve.STCpyout[kk] = 0.;
    eve.STCpzout[kk] = 0.;
    eve.STCde[kk]    = 0.;
    eve.STCal[kk]    = 0.;
    eve.STCtim[kk]   = 0.;
  }
  
  for(int kk=0;kk<eve.BMNn;kk++){
    eve.BMNid[kk]    = 0;
    eve.BMNilay[kk]  = 0;
    eve.BMNiview[kk] = 0;
    eve.BMNicell[kk] = 0;
    eve.BMNxin[kk]   = 0.;
    eve.BMNyin[kk]   = 0.;
    eve.BMNzin[kk]   = 0.;
    eve.BMNxout[kk]  = 0.;
    eve.BMNyout[kk]  = 0.;
    eve.BMNzout[kk]  = 0.;
    eve.BMNpxin[kk]  = 0.;
    eve.BMNpyin[kk]  = 0.;
    eve.BMNpzin[kk]  = 0.;
    eve.BMNpxout[kk] = 0.;
    eve.BMNpyout[kk] = 0.;
    eve.BMNpzout[kk] = 0.;
    eve.BMNde[kk]    = 0.;
    eve.BMNal[kk]    = 0.;
    eve.BMNtim[kk]   = 0.;
  }

  for(int kk=0;kk<eve.VTXn;kk++){
    eve.VTXid[kk]    = 0;
    eve.VTXilay[kk]  = 0;
    eve.VTXirow[kk]  = 0;
    eve.VTXicol[kk]  = 0;
    eve.VTXxin[kk]   = 0.;
    eve.VTXyin[kk]   = 0.;
    eve.VTXzin[kk]   = 0.;
    eve.VTXxout[kk]  = 0.;
    eve.VTXyout[kk]  = 0.;
    eve.VTXzout[kk]  = 0.;
    eve.VTXpxin[kk]  = 0.;
    eve.VTXpyin[kk]  = 0.;
    eve.VTXpzin[kk]  = 0.;
    eve.VTXpxout[kk] = 0.;
    eve.VTXpyout[kk] = 0.;
    eve.VTXpzout[kk] = 0.;
    eve.VTXde[kk]    = 0.;
    eve.VTXal[kk]    = 0.;
    eve.VTXtim[kk]   = 0.;
  }

  for(int kk=0;kk<eve.ITRn;kk++){
    eve.ITRid[kk]    = 0;
    eve.ITRiplume[kk]  = 0;
    eve.ITRimimo[kk]  = 0;
    eve.ITRilay[kk]  = 0;
    eve.ITRirow[kk]  = 0;
    eve.ITRicol[kk]  = 0;
    eve.ITRxin[kk]   = 0.;
    eve.ITRyin[kk]   = 0.;
    eve.ITRzin[kk]   = 0.;
    eve.ITRxout[kk]  = 0.;
    eve.ITRyout[kk]  = 0.;
    eve.ITRzout[kk]  = 0.;
    eve.ITRpxin[kk]  = 0.;
    eve.ITRpyin[kk]  = 0.;
    eve.ITRpzin[kk]  = 0.;
    eve.ITRpxout[kk] = 0.;
    eve.ITRpyout[kk] = 0.;
    eve.ITRpzout[kk] = 0.;
    eve.ITRde[kk]    = 0.;
    eve.ITRal[kk]    = 0.;
    eve.ITRtim[kk]   = 0.;
  }

  for(int kk=0;kk<eve.MSDn;kk++){
    eve.MSDid[kk]    = 0;
    eve.MSDilay[kk]  = 0;
    eve.MSDistripx[kk]= 0;
    eve.MSDistripy[kk]= 0;
    eve.MSDxin[kk]   = 0.;
    eve.MSDyin[kk]   = 0.;
    eve.MSDzin[kk]   = 0.;
    eve.MSDxout[kk]  = 0.;
    eve.MSDyout[kk]  = 0.;
    eve.MSDzout[kk]  = 0.;
    eve.MSDpxin[kk]  = 0.;
    eve.MSDpyin[kk]  = 0.;
    eve.MSDpzin[kk]  = 0.;
    eve.MSDpxout[kk] = 0.;
    eve.MSDpyout[kk] = 0.;
    eve.MSDpzout[kk] = 0.;
    eve.MSDde[kk]    = 0.;
    eve.MSDal[kk]    = 0.;
    eve.MSDtim[kk]   = 0.;
  }

  for(int kk=0;kk<eve.SCNn;kk++){
    eve.SCNid[kk]     = 0;
    eve.SCNibar[kk] = 0;
    eve.SCNiview[kk]  = 0;
    eve.SCNxin[kk]    = 0.;
    eve.SCNyin[kk]    = 0.;
    eve.SCNzin[kk]    = 0.;
    eve.SCNxout[kk]   = 0.;
    eve.SCNyout[kk]   = 0.;
    eve.SCNzout[kk]   = 0.;
    eve.SCNpxin[kk]   = 0.;
    eve.SCNpyin[kk]   = 0.;
    eve.SCNpzin[kk]   = 0.;
    eve.SCNpxout[kk]  = 0.;
    eve.SCNpyout[kk]  = 0.;
    eve.SCNpzout[kk]  = 0.;
    eve.SCNde[kk]     = 0.;
    eve.SCNal[kk]     = 0.;
    eve.SCNtim[kk]    = 0.;
  }
  
  for(int kk=0;kk<eve.CALn;kk++){
    eve.CALid[kk]    = 0;
    eve.CALicry[kk]  = 0;
    eve.CALxin[kk]   = 0.;
    eve.CALyin[kk]   = 0.;
    eve.CALzin[kk]   = 0.;
    eve.CALxout[kk]  = 0.;
    eve.CALyout[kk]  = 0.;
    eve.CALzout[kk]  = 0.;
    eve.CALpxin[kk]  = 0.;
    eve.CALpyin[kk]  = 0.;
    eve.CALpzin[kk]  = 0.;
    eve.CALpxout[kk] = 0.;
    eve.CALpyout[kk] = 0.;
    eve.CALpzout[kk] = 0.;
    eve.CALde[kk]    = 0.;
    eve.CALal[kk]    = 0.;
    eve.CALtim[kk]   = 0.;    
  }
  
  for(int kk=0;kk<eve.CROSSn;kk++){
    eve.CROSSid[kk]      = 0;
    eve.CROSSnreg[kk]    = 0;
    eve.CROSSnregold[kk] = 0;
    eve.CROSSx[kk]       = 0.;
    eve.CROSSy[kk]       = 0.;
    eve.CROSSz[kk]       = 0.;
    eve.CROSSpx[kk]      = 0.;
    eve.CROSSpy[kk]      = 0.;
    eve.CROSSpz[kk]      = 0.;
    eve.CROSSm[kk]       = 0.;
    eve.CROSSch[kk]      = 0.;
    eve.CROSSt[kk]       = 0.;
  }

  eve.EventNumber = 0;
  eve.TRn       = 0;
  eve.STCn      = 0;
  eve.VTXn      = 0;
  eve.BMNn      = 0;
  eve.MSDn      = 0;
  eve.SCNn      = 0;
  eve.CALn      = 0;
  eve.CROSSn    = 0;
  
  return 0;
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddPart(Int_t fTRpaid, Int_t fTRgen, 
		      Int_t fTRcha, Int_t fTRreg, Int_t fTRbar,
		      Int_t fTRdead, Int_t fTRfid,
		      Double_t fTRix, Double_t fTRiy, Double_t fTRiz,
		      Double_t fTRfx, Double_t fTRfy, Double_t fTRfz,
		      Double_t fTRipx, Double_t fTRipy, Double_t fTRipz,
		      Double_t fTRfpx, Double_t fTRfpy, Double_t fTRfpz,
		      Double_t fTRmass, Double_t fTRtime,
		      Double_t fTRtof, Double_t fTRtrlen){

  if(eve.TRn<MAXTR)
    {
      eve.TRn ++;
      eve.TRpaid[eve.TRn-1] = fTRpaid;
      eve.TRgen[eve.TRn-1] = fTRgen;
      eve.TRcha[eve.TRn-1] = fTRcha;
      eve.TRreg[eve.TRn-1] = fTRreg;
      eve.TRbar[eve.TRn-1] = fTRbar;
      eve.TRdead[eve.TRn-1] = fTRdead;
      eve.TRfid[eve.TRn-1] = fTRfid;
      eve.TRix[eve.TRn-1] = fTRix;
      eve.TRiy[eve.TRn-1] = fTRiy;
      eve.TRiz[eve.TRn-1] = fTRiz;
      eve.TRfx[eve.TRn-1] = fTRfx;
      eve.TRfy[eve.TRn-1] = fTRfy;
      eve.TRfz[eve.TRn-1] = fTRfz;
      eve.TRipx[eve.TRn-1] = fTRipx;
      eve.TRipy[eve.TRn-1] = fTRipy;
      eve.TRipz[eve.TRn-1] = fTRipz;
      eve.TRfpx[eve.TRn-1] = fTRfpx;
      eve.TRfpy[eve.TRn-1] = fTRfpy;
      eve.TRfpz[eve.TRn-1] = fTRfpz;
      eve.TRmass[eve.TRn-1] = fTRmass;
      eve.TRtime[eve.TRn-1] = fTRtime;
      eve.TRtof[eve.TRn-1] = fTRtof;
      eve.TRtrlen[eve.TRn-1] = fTRtrlen;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddSTC(Int_t fSTCid,
		    Double_t fSTCxin, Double_t fSTCyin, Double_t fSTCzin,
		    Double_t fSTCxout, Double_t fSTCyout, Double_t fSTCzout, 
		    Double_t fSTCpxin, Double_t fSTCpyin, Double_t fSTCpzin, 
		    Double_t fSTCpxout, Double_t fSTCpyout, Double_t fSTCpzout, 
		    Double_t fSTCde, Double_t fSTCal, Double_t fSTCtim){

  if(eve.STCn<MAXSTC) {
    eve.STCn ++;
    eve.STCid[eve.STCn-1] = fSTCid;
    eve.STCxin[eve.STCn-1] = fSTCxin;
    eve.STCyin[eve.STCn-1] = fSTCyin;
    eve.STCzin[eve.STCn-1] = fSTCzin;
    eve.STCpxin[eve.STCn-1] = fSTCpxin;
    eve.STCpyin[eve.STCn-1] = fSTCpyin;
    eve.STCpzin[eve.STCn-1] = fSTCpzin;
    eve.STCxout[eve.STCn-1] = fSTCxout;
    eve.STCyout[eve.STCn-1] = fSTCyout;
    eve.STCzout[eve.STCn-1] = fSTCzout;
    eve.STCpxout[eve.STCn-1] = fSTCpxout;
    eve.STCpyout[eve.STCn-1] = fSTCpyout;
    eve.STCpzout[eve.STCn-1] = fSTCpzout;
    eve.STCde[eve.STCn-1] = fSTCde;
    eve.STCal[eve.STCn-1] = fSTCal;
    eve.STCtim[eve.STCn-1] = fSTCtim;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddBMN(Int_t fBMNid, Int_t fBMNilay, Int_t fBMNiview,
		     Int_t fBMNicell,
		     Double_t fBMNxin, Double_t fBMNyin, Double_t fBMNzin,
		     Double_t fBMNxout, Double_t fBMNyout, Double_t fBMNzout,
		     Double_t fBMNpxin, Double_t fBMNpyin, Double_t fBMNpzin,
		     Double_t fBMNpxout, Double_t fBMNpyout, Double_t fBMNpzout,
		     Double_t fBMNde, Double_t fBMNal, Double_t fBMNtim){

  if(eve.BMNn<MAXBMN){
      eve.BMNn ++;
      eve.BMNid[eve.BMNn-1] = fBMNid;
      eve.BMNilay[eve.BMNn-1] = fBMNilay;
      eve.BMNiview[eve.BMNn-1] = fBMNiview;
      eve.BMNicell[eve.BMNn-1] = fBMNicell;
      eve.BMNxin[eve.BMNn-1] = fBMNxin;
      eve.BMNyin[eve.BMNn-1] = fBMNyin;
      eve.BMNzin[eve.BMNn-1] = fBMNzin;
      eve.BMNpxin[eve.BMNn-1] = fBMNpxin;
      eve.BMNpyin[eve.BMNn-1] = fBMNpyin;
      eve.BMNpzin[eve.BMNn-1] = fBMNpzin;
      eve.BMNxout[eve.BMNn-1] = fBMNxout;
      eve.BMNyout[eve.BMNn-1] = fBMNyout;
      eve.BMNzout[eve.BMNn-1] = fBMNzout;
      eve.BMNpxout[eve.BMNn-1] = fBMNpxout;
      eve.BMNpyout[eve.BMNn-1] = fBMNpyout;
      eve.BMNpzout[eve.BMNn-1] = fBMNpzout;
      eve.BMNde[eve.BMNn-1] = fBMNde;
      eve.BMNal[eve.BMNn-1] = fBMNal;
      eve.BMNtim[eve.BMNn-1] = fBMNtim;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddVTX(Int_t fVTXid, Int_t fVTXilay,
		     Int_t fVTXirow, Int_t fVTXicol,
		     Double_t fVTXxin, Double_t fVTXyin, Double_t fVTXzin,
		     Double_t fVTXxout, Double_t fVTXyout, Double_t fVTXzout,
		     Double_t fVTXpxin, Double_t fVTXpyin, Double_t fVTXpzin,
		     Double_t fVTXpxout, Double_t fVTXpyout, Double_t fVTXpzout,
		     Double_t fVTXde, Double_t fVTXal, Double_t fVTXtim){

  if(eve.VTXn<MAXVTX){
      eve.VTXn ++;
      eve.VTXilay[eve.VTXn-1] = fVTXilay;
      eve.VTXid[eve.VTXn-1] = fVTXid;
      eve.VTXirow[eve.VTXn-1] = fVTXirow;
      eve.VTXicol[eve.VTXn-1] = fVTXicol;
      eve.VTXxin[eve.VTXn-1] = fVTXxin;
      eve.VTXyin[eve.VTXn-1] = fVTXyin;
      eve.VTXzin[eve.VTXn-1] = fVTXzin;
      eve.VTXpxin[eve.VTXn-1] = fVTXpxin;
      eve.VTXpyin[eve.VTXn-1] = fVTXpyin;
      eve.VTXpzin[eve.VTXn-1] = fVTXpzin;
      eve.VTXxout[eve.VTXn-1] = fVTXxout;
      eve.VTXyout[eve.VTXn-1] = fVTXyout;
      eve.VTXzout[eve.VTXn-1] = fVTXzout;
      eve.VTXpxout[eve.VTXn-1] = fVTXpxout;
      eve.VTXpyout[eve.VTXn-1] = fVTXpyout;
      eve.VTXpzout[eve.VTXn-1] = fVTXpzout;
      eve.VTXde[eve.VTXn-1] = fVTXde;
      eve.VTXal[eve.VTXn-1] = fVTXal;
      eve.VTXtim[eve.VTXn-1] = fVTXtim;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddITR(Int_t fITRid, Int_t fITRilay,Int_t fITRirow,
		     Int_t fITRicol,Int_t fITRiplume, Int_t fITRimimo,
		     Double_t fITRxin, Double_t fITRyin, Double_t fITRzin,
		     Double_t fITRxout, Double_t fITRyout, Double_t fITRzout,
		     Double_t fITRpxin, Double_t fITRpyin, Double_t fITRpzin,
		     Double_t fITRpxout, Double_t fITRpyout, Double_t fITRpzout,
		     Double_t fITRde, Double_t fITRal, Double_t fITRtim){

  if(eve.ITRn<MAXITR){
      eve.ITRn ++;
      eve.ITRilay[eve.ITRn-1] = fITRilay;
      eve.ITRid[eve.ITRn-1] = fITRid;
      eve.ITRirow[eve.ITRn-1] = fITRirow;
      eve.ITRicol[eve.ITRn-1] = fITRicol;
      eve.ITRiplume[eve.ITRn-1] = fITRiplume;
      eve.ITRimimo[eve.ITRn-1] = fITRimimo;
      eve.ITRxin[eve.ITRn-1] = fITRxin;
      eve.ITRyin[eve.ITRn-1] = fITRyin;
      eve.ITRzin[eve.ITRn-1] = fITRzin;
      eve.ITRpxin[eve.ITRn-1] = fITRpxin;
      eve.ITRpyin[eve.ITRn-1] = fITRpyin;
      eve.ITRpzin[eve.ITRn-1] = fITRpzin;
      eve.ITRxout[eve.ITRn-1] = fITRxout;
      eve.ITRyout[eve.ITRn-1] = fITRyout;
      eve.ITRzout[eve.ITRn-1] = fITRzout;
      eve.ITRpxout[eve.ITRn-1] = fITRpxout;
      eve.ITRpyout[eve.ITRn-1] = fITRpyout;
      eve.ITRpzout[eve.ITRn-1] = fITRpzout;
      eve.ITRde[eve.ITRn-1] = fITRde;
      eve.ITRal[eve.ITRn-1] = fITRal;
      eve.ITRtim[eve.ITRn-1] = fITRtim;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddMSD(Int_t fMSDid, Int_t fMSDilay,
		     Int_t fMSDistripx, Int_t fMSDistripy,
		     Double_t fMSDxin, Double_t fMSDyin, Double_t fMSDzin,
		     Double_t fMSDxout, Double_t fMSDyout, Double_t fMSDzout,
		     Double_t fMSDpxin, Double_t fMSDpyin, Double_t fMSDpzin,
		     Double_t fMSDpxout, Double_t fMSDpyout, Double_t fMSDpzout,
		     Double_t fMSDde, Double_t fMSDal, Double_t fMSDtim){
  
  if(eve.MSDn<MAXMSD){
      eve.MSDn ++;
      eve.MSDid[eve.MSDn-1] = fMSDid;
      eve.MSDilay[eve.MSDn-1] = fMSDilay;
      eve.MSDistripx[eve.MSDn-1] = fMSDistripx;
      eve.MSDistripy[eve.MSDn-1] = fMSDistripy;
      eve.MSDxin[eve.MSDn-1] = fMSDxin;
      eve.MSDyin[eve.MSDn-1] = fMSDyin;
      eve.MSDzin[eve.MSDn-1] = fMSDzin;
      eve.MSDpxin[eve.MSDn-1] = fMSDpxin;
      eve.MSDpyin[eve.MSDn-1] = fMSDpyin;
      eve.MSDpzin[eve.MSDn-1] = fMSDpzin;
      eve.MSDxout[eve.MSDn-1] = fMSDxout;
      eve.MSDyout[eve.MSDn-1] = fMSDyout;
      eve.MSDzout[eve.MSDn-1] = fMSDzout;
      eve.MSDpxout[eve.MSDn-1] = fMSDpxout;
      eve.MSDpyout[eve.MSDn-1] = fMSDpyout;
      eve.MSDpzout[eve.MSDn-1] = fMSDpzout;
      eve.MSDde[eve.MSDn-1] = fMSDde;
      eve.MSDal[eve.MSDn-1] = fMSDal;
      eve.MSDtim[eve.MSDn-1] = fMSDtim;
      return 0;
    }
  else
    {
      return -1;
    }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddSCN(Int_t fSCNid,
		       Int_t fSCNibar, Int_t fSCNiview,
		       Double_t fSCNxin, Double_t fSCNyin, 
		       Double_t fSCNzin,
		       Double_t fSCNxout, Double_t fSCNyout, 
		       Double_t fSCNzout, Double_t fSCNpxin, 
		       Double_t fSCNpyin, Double_t fSCNpzin, 
		       Double_t fSCNpxout, Double_t fSCNpyout, 
		       Double_t fSCNpzout, Double_t fSCNde, 
		       Double_t fSCNal, Double_t fSCNtim){

  if(eve.SCNn<MAXSCN){
    eve.SCNn ++;
    eve.SCNid[eve.SCNn-1] = fSCNid;
    eve.SCNibar[eve.SCNn-1] = fSCNibar;
    eve.SCNiview[eve.SCNn-1] = fSCNiview;
    eve.SCNxin[eve.SCNn-1] = fSCNxin;
    eve.SCNyin[eve.SCNn-1] = fSCNyin;
    eve.SCNzin[eve.SCNn-1] = fSCNzin;
    eve.SCNpxin[eve.SCNn-1] = fSCNpxin;
    eve.SCNpyin[eve.SCNn-1] = fSCNpyin;
    eve.SCNpzin[eve.SCNn-1] = fSCNpzin;
    eve.SCNxout[eve.SCNn-1] = fSCNxout;
    eve.SCNyout[eve.SCNn-1] = fSCNyout;
    eve.SCNzout[eve.SCNn-1] = fSCNzout;
    eve.SCNpxout[eve.SCNn-1] = fSCNpxout;
    eve.SCNpyout[eve.SCNn-1] = fSCNpyout;
    eve.SCNpzout[eve.SCNn-1] = fSCNpzout;
    eve.SCNde[eve.SCNn-1] = fSCNde;
    eve.SCNal[eve.SCNn-1] = fSCNal;
    eve.SCNtim[eve.SCNn-1] = fSCNtim;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddCAL(Int_t fCALid, Int_t fCALicry, 
		     Double_t fCALxin, Double_t fCALyin,  Double_t fCALzin,
		     Double_t fCALxout, Double_t fCALyout, Double_t fCALzout,
		     Double_t fCALpxin, Double_t fCALpyin, Double_t fCALpzin,
		     Double_t fCALpxout, Double_t fCALpyout, Double_t fCALpzout,
		     Double_t fCALde, Double_t fCALal, Double_t fCALtim){

  if(eve.CALn<MAXCAL){
    eve.CALn ++;
    eve.CALid[eve.CALn-1] = fCALid;
    eve.CALicry[eve.CALn-1] = fCALicry;
    eve.CALxin[eve.CALn-1] = fCALxin;
    eve.CALyin[eve.CALn-1] = fCALyin;
    eve.CALzin[eve.CALn-1] = fCALzin;
    eve.CALpxin[eve.CALn-1] = fCALpxin;
    eve.CALpyin[eve.CALn-1] = fCALpyin;
    eve.CALpzin[eve.CALn-1] = fCALpzin;
    eve.CALxout[eve.CALn-1] = fCALxout;
    eve.CALyout[eve.CALn-1] = fCALyout;
    eve.CALzout[eve.CALn-1] = fCALzout;
    eve.CALpxout[eve.CALn-1] = fCALpxout;
    eve.CALpyout[eve.CALn-1] = fCALpyout;
    eve.CALpzout[eve.CALn-1] = fCALpzout;
    eve.CALde[eve.CALn-1] = fCALde;
    eve.CALal[eve.CALn-1] = fCALal;
    eve.CALtim[eve.CALn-1] = fCALtim;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

Int_t Evento::AddCROSS(Int_t fCROSSid, Int_t fCROSSnreg, Int_t fCROSSnregold,
		       Double_t fCROSSx, Double_t fCROSSy, Double_t fCROSSz,
		       Double_t fCROSSpx, Double_t fCROSSpy, Double_t fCROSSpz, 
		       Double_t fCROSSm, Double_t fCROSSch, Double_t fCROSSt){

  if(eve.CROSSn<MAXCROSS) {
    eve.CROSSn ++;
    eve.CROSSid[eve.CROSSn-1] = fCROSSid;
    eve.CROSSnreg[eve.CROSSn-1] = fCROSSnreg;
    eve.CROSSnregold[eve.CROSSn-1] = fCROSSnregold;
    eve.CROSSx[eve.CROSSn-1] = fCROSSx;
    eve.CROSSy[eve.CROSSn-1] = fCROSSy;
    eve.CROSSz[eve.CROSSn-1] = fCROSSz;
    eve.CROSSpx[eve.CROSSn-1] = fCROSSpx;
    eve.CROSSpy[eve.CROSSn-1] = fCROSSpy;
    eve.CROSSpz[eve.CROSSn-1] = fCROSSpz;
    eve.CROSSm[eve.CROSSn-1] = fCROSSm;
    eve.CROSSch[eve.CROSSn-1] = fCROSSch;
    eve.CROSSt[eve.CROSSn-1] = fCROSSt;
    return 0;
  }
  else{
    return -1;
  }
}

/*-----------------------------------------------------------------*/

int Evento::FindBranches(TTree *RootTree, EVENT_STRUCT *eve){

  RootTree->SetBranchAddress("TRn",&(eve->TRn));
  RootTree->SetBranchAddress("TRpaid",&(eve->TRpaid));
  RootTree->SetBranchAddress("TRgen",&(eve->TRgen));
  RootTree->SetBranchAddress("TRcha",&(eve->TRcha));
  RootTree->SetBranchAddress("TRreg",&(eve->TRreg));
  RootTree->SetBranchAddress("TRbar",&(eve->TRbar));
  RootTree->SetBranchAddress("TRdead",&(eve->TRdead));
  RootTree->SetBranchAddress("TRfid",&(eve->TRfid));
  RootTree->SetBranchAddress("TRix",&(eve->TRix));
  RootTree->SetBranchAddress("TRiy",&(eve->TRiy));
  RootTree->SetBranchAddress("TRiz",&(eve->TRiz));
  RootTree->SetBranchAddress("TRfx",&(eve->TRfx));
  RootTree->SetBranchAddress("TRfy",&(eve->TRfy));
  RootTree->SetBranchAddress("TRfz",&(eve->TRfz));
  RootTree->SetBranchAddress("TRipx",&(eve->TRipx));
  RootTree->SetBranchAddress("TRipy",&(eve->TRipy));
  RootTree->SetBranchAddress("TRipz",&(eve->TRipz));
  RootTree->SetBranchAddress("TRfpx",&(eve->TRfpx));
  RootTree->SetBranchAddress("TRfpy",&(eve->TRfpy));
  RootTree->SetBranchAddress("TRfpz",&(eve->TRfpz));
  RootTree->SetBranchAddress("TRmass",&(eve->TRmass));
  RootTree->SetBranchAddress("TRtime",&(eve->TRtime));
  RootTree->SetBranchAddress("TRtof",&(eve->TRtof));
  RootTree->SetBranchAddress("TRtrlen",&(eve->TRtrlen));

  RootTree->SetBranchAddress("STCn",&(eve->STCn));
  RootTree->SetBranchAddress("STCid",&(eve->STCid));
  RootTree->SetBranchAddress("STCxin",&(eve->STCxin));
  RootTree->SetBranchAddress("STCyin",&(eve->STCyin));
  RootTree->SetBranchAddress("STCzin",&(eve->STCzin));
  RootTree->SetBranchAddress("STCpxin",&(eve->STCpxin));
  RootTree->SetBranchAddress("STCpyin",&(eve->STCpyin));
  RootTree->SetBranchAddress("STCpzin",&(eve->STCpzin));
  RootTree->SetBranchAddress("STCxout",&(eve->STCxout));
  RootTree->SetBranchAddress("STCyout",&(eve->STCyout));
  RootTree->SetBranchAddress("STCzout",&(eve->STCzout));
  RootTree->SetBranchAddress("STCpxout",&(eve->STCpxout));
  RootTree->SetBranchAddress("STCpyout",&(eve->STCpyout));
  RootTree->SetBranchAddress("STCpzout",&(eve->STCpzout));
  RootTree->SetBranchAddress("STCde",&(eve->STCde));
  RootTree->SetBranchAddress("STCal",&(eve->STCal));
  RootTree->SetBranchAddress("STCtim",&(eve->STCtim));

  RootTree->SetBranchAddress("BMNn",&(eve->BMNn));
  RootTree->SetBranchAddress("BMNid",&(eve->BMNid));
  RootTree->SetBranchAddress("BMNilay",&(eve->BMNilay));
  RootTree->SetBranchAddress("BMNiview",&(eve->BMNiview));
  RootTree->SetBranchAddress("BMNicell",&(eve->BMNicell));
  RootTree->SetBranchAddress("BMNxin",&(eve->BMNxin));
  RootTree->SetBranchAddress("BMNyin",&(eve->BMNyin));
  RootTree->SetBranchAddress("BMNzin",&(eve->BMNzin));
  RootTree->SetBranchAddress("BMNpxin",&(eve->BMNpxin));
  RootTree->SetBranchAddress("BMNpyin",&(eve->BMNpyin));
  RootTree->SetBranchAddress("BMNpzin",&(eve->BMNpzin));
  RootTree->SetBranchAddress("BMNxout",&(eve->BMNxout));
  RootTree->SetBranchAddress("BMNyout",&(eve->BMNyout));
  RootTree->SetBranchAddress("BMNzout",&(eve->BMNzout));
  RootTree->SetBranchAddress("BMNpxout",&(eve->BMNpxout));
  RootTree->SetBranchAddress("BMNpyout",&(eve->BMNpyout));
  RootTree->SetBranchAddress("BMNpzout",&(eve->BMNpzout));
  RootTree->SetBranchAddress("BMNde",&(eve->BMNde));
  RootTree->SetBranchAddress("BMNal",&(eve->BMNal));
  RootTree->SetBranchAddress("BMNtim",&(eve->BMNtim));
  
  RootTree->SetBranchAddress("VTXn",&(eve->VTXn));
  RootTree->SetBranchAddress("VTXid",&(eve->VTXid));
  RootTree->SetBranchAddress("VTXilay",&(eve->VTXilay));
  RootTree->SetBranchAddress("VTXirow",&(eve->VTXirow));
  RootTree->SetBranchAddress("VTXicol",&(eve->VTXicol));
  RootTree->SetBranchAddress("VTXxin",&(eve->VTXxin));
  RootTree->SetBranchAddress("VTXyin",&(eve->VTXyin));
  RootTree->SetBranchAddress("VTXzin",&(eve->VTXzin));
  RootTree->SetBranchAddress("VTXpxin",&(eve->VTXpxin));
  RootTree->SetBranchAddress("VTXpyin",&(eve->VTXpyin));
  RootTree->SetBranchAddress("VTXpzin",&(eve->VTXpzin));
  RootTree->SetBranchAddress("VTXxout",&(eve->VTXxout));
  RootTree->SetBranchAddress("VTXyout",&(eve->VTXyout));
  RootTree->SetBranchAddress("VTXzout",&(eve->VTXzout));
  RootTree->SetBranchAddress("VTXpxout",&(eve->VTXpxout));
  RootTree->SetBranchAddress("VTXpyout",&(eve->VTXpyout));
  RootTree->SetBranchAddress("VTXpzout",&(eve->VTXpzout));
  RootTree->SetBranchAddress("VTXde",&(eve->VTXde));
  RootTree->SetBranchAddress("VTXal",&(eve->VTXal));
  RootTree->SetBranchAddress("VTXtim",&(eve->VTXtim));
  
  RootTree->SetBranchAddress("ITRn",&(eve->ITRn));
  RootTree->SetBranchAddress("ITRid",&(eve->ITRid));
  RootTree->SetBranchAddress("ITRiplume",&(eve->ITRiplume));
  RootTree->SetBranchAddress("ITRimimo",&(eve->ITRimimo));
  RootTree->SetBranchAddress("ITRilay",&(eve->ITRilay));
  RootTree->SetBranchAddress("ITRirow",&(eve->ITRirow));
  RootTree->SetBranchAddress("ITRicol",&(eve->ITRicol));
  RootTree->SetBranchAddress("ITRxin",&(eve->ITRxin));
  RootTree->SetBranchAddress("ITRyin",&(eve->ITRyin));
  RootTree->SetBranchAddress("ITRzin",&(eve->ITRzin));
  RootTree->SetBranchAddress("ITRpxin",&(eve->ITRpxin));
  RootTree->SetBranchAddress("ITRpyin",&(eve->ITRpyin));
  RootTree->SetBranchAddress("ITRpzin",&(eve->ITRpzin));
  RootTree->SetBranchAddress("ITRxout",&(eve->ITRxout));
  RootTree->SetBranchAddress("ITRyout",&(eve->ITRyout));
  RootTree->SetBranchAddress("ITRzout",&(eve->ITRzout));
  RootTree->SetBranchAddress("ITRpxout",&(eve->ITRpxout));
  RootTree->SetBranchAddress("ITRpyout",&(eve->ITRpyout));
  RootTree->SetBranchAddress("ITRpzout",&(eve->ITRpzout));
  RootTree->SetBranchAddress("ITRde",&(eve->ITRde));
  RootTree->SetBranchAddress("ITRal",&(eve->ITRal));
  RootTree->SetBranchAddress("ITRtim",&(eve->ITRtim));

  RootTree->SetBranchAddress("MSDn",&(eve->MSDn));
  RootTree->SetBranchAddress("MSDid",&(eve->MSDid));
  RootTree->SetBranchAddress("MSDilay",&(eve->MSDilay));
  RootTree->SetBranchAddress("MSDistripx",&(eve->MSDistripx));
  RootTree->SetBranchAddress("MSDistripy",&(eve->MSDistripy));
  RootTree->SetBranchAddress("MSDxin",&(eve->MSDxin));
  RootTree->SetBranchAddress("MSDyin",&(eve->MSDyin));
  RootTree->SetBranchAddress("MSDzin",&(eve->MSDzin));
  RootTree->SetBranchAddress("MSDpxin",&(eve->MSDpxin));
  RootTree->SetBranchAddress("MSDpyin",&(eve->MSDpyin));
  RootTree->SetBranchAddress("MSDpzin",&(eve->MSDpzin));
  RootTree->SetBranchAddress("MSDxout",&(eve->MSDxout));
  RootTree->SetBranchAddress("MSDyout",&(eve->MSDyout));
  RootTree->SetBranchAddress("MSDzout",&(eve->MSDzout));
  RootTree->SetBranchAddress("MSDpxout",&(eve->MSDpxout));
  RootTree->SetBranchAddress("MSDpyout",&(eve->MSDpyout));
  RootTree->SetBranchAddress("MSDpzout",&(eve->MSDpzout));
  RootTree->SetBranchAddress("MSDde",&(eve->MSDde));
  RootTree->SetBranchAddress("MSDal",&(eve->MSDal));
  RootTree->SetBranchAddress("MSDtim",&(eve->MSDtim));
  
  RootTree->SetBranchAddress("SCNn",&(eve->SCNn));
  RootTree->SetBranchAddress("SCNid",&(eve->SCNid));
  RootTree->SetBranchAddress("SCNibar",&(eve->SCNibar));
  RootTree->SetBranchAddress("SCNiview",&(eve->SCNiview));
  RootTree->SetBranchAddress("SCNxin",&(eve->SCNxin));
  RootTree->SetBranchAddress("SCNyin",&(eve->SCNyin));
  RootTree->SetBranchAddress("SCNzin",&(eve->SCNzin));
  RootTree->SetBranchAddress("SCNpxin",&(eve->SCNpxin));
  RootTree->SetBranchAddress("SCNpyin",&(eve->SCNpyin));
  RootTree->SetBranchAddress("SCNpzin",&(eve->SCNpzin));
  RootTree->SetBranchAddress("SCNxout",&(eve->SCNxout));
  RootTree->SetBranchAddress("SCNyout",&(eve->SCNyout));
  RootTree->SetBranchAddress("SCNzout",&(eve->SCNzout));
  RootTree->SetBranchAddress("SCNpxout",&(eve->SCNpxout));
  RootTree->SetBranchAddress("SCNpyout",&(eve->SCNpyout));
  RootTree->SetBranchAddress("SCNpzout",&(eve->SCNpzout));
  RootTree->SetBranchAddress("SCNde",&(eve->SCNde));
  RootTree->SetBranchAddress("SCNal",&(eve->SCNal));
  RootTree->SetBranchAddress("SCNtim",&(eve->SCNtim));

  RootTree->SetBranchAddress("CALn",&(eve->CALn));
  RootTree->SetBranchAddress("CALid",&(eve->CALid));
  RootTree->SetBranchAddress("CALicry",&(eve->CALicry));
  RootTree->SetBranchAddress("CALxin",&(eve->CALxin));
  RootTree->SetBranchAddress("CALyin",&(eve->CALyin));
  RootTree->SetBranchAddress("CALzin",&(eve->CALzin));
  RootTree->SetBranchAddress("CALpxin",&(eve->CALpxin));
  RootTree->SetBranchAddress("CALpyin",&(eve->CALpyin));
  RootTree->SetBranchAddress("CALpzin",&(eve->CALpzin));
  RootTree->SetBranchAddress("CALxout",&(eve->CALxout));
  RootTree->SetBranchAddress("CALyout",&(eve->CALyout));
  RootTree->SetBranchAddress("CALzout",&(eve->CALzout));
  RootTree->SetBranchAddress("CALpxout",&(eve->CALpxout));
  RootTree->SetBranchAddress("CALpyout",&(eve->CALpyout));
  RootTree->SetBranchAddress("CALpzout",&(eve->CALpzout));
  RootTree->SetBranchAddress("CALde",&(eve->CALde));
  RootTree->SetBranchAddress("CALal",&(eve->CALal));
  RootTree->SetBranchAddress("CALtim",&(eve->CALtim));

  RootTree->SetBranchAddress("CROSSn",&(eve->CROSSn));
  RootTree->SetBranchAddress("CROSSid",&(eve->CROSSid));
  RootTree->SetBranchAddress("CROSSnreg",&(eve->CROSSnreg));
  RootTree->SetBranchAddress("CROSSnregold",&(eve->CROSSnreg));
  RootTree->SetBranchAddress("CROSSx",&(eve->CROSSx));
  RootTree->SetBranchAddress("CROSSy",&(eve->CROSSy));
  RootTree->SetBranchAddress("CROSSz",&(eve->CROSSz));
  RootTree->SetBranchAddress("CROSSpx",&(eve->CROSSpx));
  RootTree->SetBranchAddress("CROSSpy",&(eve->CROSSpy));
  RootTree->SetBranchAddress("CROSSpz",&(eve->CROSSpz));
  RootTree->SetBranchAddress("CROSSm",&(eve->CROSSm));
  RootTree->SetBranchAddress("CROSSch",&(eve->CROSSch));
  RootTree->SetBranchAddress("CROSSt",&(eve->CROSSt));
  return 0;

}


/*-----------------------------------------------------------------*/


Int_t Evento::Dump(){
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


