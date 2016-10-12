/*!
  \file
  \version $Id: adapted from TABMactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TADCactNtuMC.
*/

#include "TAGroot.hxx"
#include "TADCntuRaw.hxx"
#include "TADCparCon.hxx"
#include "TADCactNtuMC.hxx"

//First
#include "TAGgeoTrafo.hxx"

/*!
  \class TADCactNtuMC TADCactNtuMC.hxx "TADCactNtuMC.hxx"
  \brief NTuplizer for DC raw hits. **
*/

ClassImp(TADCactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCactNtuMC::TADCactNtuMC(const char* name,
			   TAGdataDsc* p_nturaw, 
			   TAGparaDsc* p_parcon, 
			   EVENT_STRUCT* evStr)
  : TAGaction(name, "TADCactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_nturaw),
    fpParCon(p_parcon),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Drif Chamber MC tuplizer action\n");
  AddPara(p_parcon, "TADCparCon");
  AddDataOut(p_nturaw, "TADCntuRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCactNtuMC::~TADCactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TADCactNtuMC::Action()
{

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  TADCntuRaw* p_nturaw = (TADCntuRaw*) fpNtuMC->Object();
  TADCparCon* p_parcon = (TADCparCon*) fpParCon->Object();


  Int_t nhits(0);
  if (!p_nturaw->h) p_nturaw->SetupClones();
  double locx, locy, locz;
  Double_t resolution;
  //The number of hits inside the DC is n2dc
  Info("Action()","Processing n :: %2d hits \n",fpEvtStr->n2dc);
  for (Int_t i = 0; i < fpEvtStr->n2dc; i++) {
    if(i<32) {
      
      //X,Y and Z needs to be placed in Local coordinates.
      TVector3 gloc(fpEvtStr->xin2dc[i],fpEvtStr->yin2dc[i],fpEvtStr->zin2dc[i]);
      TVector3 loc = fpFirstGeo->FromGlobalToDCLocal(gloc);
      locx = loc.X();
      locy = loc.Y();
      locz = loc.Z();

      //      resolution = p_parcon->ResoEval(fpEvtStr->rdrift[i]);
      resolution = p_parcon->ResoEval(0.1);
      //AS::: drift quantities have to be computed,
      TADCntuHit *mytmp = new((*(p_nturaw->h))[i]) 
	TADCntuHit(fpEvtStr->id2dc[i],		 fpEvtStr->iview2dc[i],
		   fpEvtStr->ipla2dc[i],          fpEvtStr->icell2dc[i],  
		   locx, locy,
		   locz, fpEvtStr->pxin2dc[i],
		   fpEvtStr->pyin2dc[i],	 fpEvtStr->pzin2dc[i],
		   0,		 0,
		   fpEvtStr->tim2dc[i] );
      mytmp->SetSigma(resolution);
      mytmp->SetTrkAss(0);
      nhits++;

    }
  }
  
  p_nturaw->nhit  = nhits;
  
  fpNtuMC->SetBit(kValid);
  return kTRUE;
}

