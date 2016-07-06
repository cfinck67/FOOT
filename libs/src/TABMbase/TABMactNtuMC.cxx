/*!
  \file
  \version $Id: TABMactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TABMactNtuMC.
*/

#include "TAGroot.hxx"
#include "TABMntuRaw.hxx"
#include "TABMparCon.hxx"
#include "TABMactNtuMC.hxx"

//First
#include "TAGgeoTrafo.hxx"

/*!
  \class TABMactNtuMC TABMactNtuMC.hxx "TABMactNtuMC.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TABMactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactNtuMC::TABMactNtuMC(const char* name,
			   TAGdataDsc* p_nturaw, 
			   TAGparaDsc* p_parcon, 
			   EVENT_STRUCT* evStr)
  : TAGaction(name, "TABMactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_nturaw),
    fpParCon(p_parcon),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddPara(p_parcon, "TABMparCon");
  AddDataOut(p_nturaw, "TABMntuRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactNtuMC::~TABMactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TABMactNtuMC::Action()
{

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  TABMntuRaw* p_nturaw = (TABMntuRaw*) fpNtuMC->Object();
  TABMparCon* p_parcon = (TABMparCon*) fpParCon->Object();


  Int_t nhits(0);
  if (!p_nturaw->h) p_nturaw->SetupClones();
  double locx, locy, locz;
  Double_t resolution;
  //The number of hits inside the BM is nmon
  Info("Action()","Processing n :: %2d hits \n",fpEvtStr->nmon);
  for (Int_t i = 0; i < fpEvtStr->nmon; i++) {
    if(i<32) {
      
      //X,Y and Z needs to be placed in Local coordinates.
      TVector3 gloc(fpEvtStr->xcamon[i],fpEvtStr->ycamon[i],fpEvtStr->zcamon[i]);
      TVector3 loc = fpFirstGeo->FromGlobalToBMLocal(gloc);
      locx = loc.X();
      locy = loc.Y();
      locz = loc.Z();

      resolution = p_parcon->ResoEval(fpEvtStr->rdrift[i]);

      TABMntuHit *mytmp = new((*(p_nturaw->h))[i]) 
	TABMntuHit(fpEvtStr->idmon[i],		 fpEvtStr->iview[i],
		   fpEvtStr->ilayer[i],          fpEvtStr->icell[i],  
		   locx, locy,
		   locz, fpEvtStr->pxcamon[i],
		   fpEvtStr->pycamon[i],	 fpEvtStr->pzcamon[i],
		   fpEvtStr->rdrift[i],		 fpEvtStr->tdrift[i],
		   fpEvtStr->timmon[i] );
      mytmp->SetSigma(resolution);
      mytmp->SetTrkAss(0);
      nhits++;

    }
  }
  
  p_nturaw->nhit  = nhits;
  
  fpNtuMC->SetBit(kValid);
  return kTRUE;
}

