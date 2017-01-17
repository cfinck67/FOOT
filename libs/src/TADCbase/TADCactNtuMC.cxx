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
  //The number of hits inside the DC is DCHn
  Info("Action()","Processing n :: %2d hits \n",fpEvtStr->DCHn);
  for (Int_t i = 0; i < fpEvtStr->DCHn; i++) {
    if(i<32) {
      
      //X,Y and Z needs to be placed in Local coordinates.
      TVector3 gloc(fpEvtStr->DCHxin[i],fpEvtStr->DCHyin[i],fpEvtStr->DCHzin[i]);
      TVector3 loc = fpFirstGeo->FromGlobalToDCLocal(gloc);
      locx = loc.X();
      locy = loc.Y();
      locz = loc.Z();

      //      resolution = p_parcon->ResoEval(fpEvtStr->rdrift[i]);
      resolution = p_parcon->ResoEval(0.1);
      //AS::: drift quantities have to be computed,
      TADCntuHit *mytmp = new((*(p_nturaw->h))[i]) 
	TADCntuHit(fpEvtStr->DCHid[i],		 fpEvtStr->DCHiview[i],
		   fpEvtStr->DCHilay[i],          fpEvtStr->DCHicell[i],  
		   locx, locy,
		   locz, fpEvtStr->DCHpxin[i],
		   fpEvtStr->DCHpyin[i],	 fpEvtStr->DCHpzin[i],
		   0,		 0,
		   fpEvtStr->DCHtim[i] );
      mytmp->SetSigma(resolution);
      mytmp->SetTrkAss(0);
      nhits++;

    }
  }
  
  p_nturaw->nhit  = nhits;
  
  fpNtuMC->SetBit(kValid);
  return kTRUE;
}

