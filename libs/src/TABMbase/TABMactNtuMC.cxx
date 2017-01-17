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
			   TAGparaDsc* p_pargeo, 
			   EVENT_STRUCT* evStr)
  : TAGaction(name, "TABMactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_nturaw),
    fpParCon(p_parcon),
    fpParGeo(p_pargeo),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddPara(p_parcon, "TABMparCon");
  AddPara(p_pargeo, "TABMparGeo");
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
  TABMparGeo* p_pargeo = (TABMparGeo*) fpParGeo->Object();

  int cell, view, lay;
  Int_t nhits(0);
  if (!p_nturaw->h) p_nturaw->SetupClones();
  Double_t resolution;
  //The number of hits inside the BM is nmon
  Info("Action()","Processing n :: %2d hits \n",fpEvtStr->BMNn);

  for (Int_t i = 0; i < fpEvtStr->BMNn; i++) {

    /*
      Pre processing of INFO to compute the PCA info + rDrift and tDrift
    */
    /*
      write(*,*)'PCA= ',xca(ii), yca(ii), zca(ii) 
      write(*,*)'p at PCA= ',pxca(ii), pyca(ii), pzca(ii) 
      write(*,*)'rdrift= ',rdrift(ii),' tdrift= ', tdrift(ii), 
    */

    //AS::: drift quantities have to be computed,
    cell = fpEvtStr->BMNicell[i];
    cell += -1;
    lay = fpEvtStr->BMNilay[i];
    lay += -1;
    view = fpEvtStr->BMNiview[i];
    if(view==2) view = -1;
    
    if(fpEvtStr->BMNid[i]-1 == 0) {
      TABMntuHit *mytmp = new((*(p_nturaw->h))[nhits]) 
	TABMntuHit(fpEvtStr->BMNid[i],		 view,
		   lay,          cell,  
		   0, 0, 0, //Will become PCA that is needed @ tracking level.
		   fpEvtStr->BMNpxin[i], fpEvtStr->BMNpyin[i], fpEvtStr->BMNpzin[i],  //mom @ entrance in cell
		   0, 0, //Rdrift is set later on (see FindRdrift) while tdrift has no meaning for now for MC
		   fpEvtStr->BMNtim[i] );
      
      
      //X,Y and Z needs to be placed in Local coordinates.
      TVector3 gloc(fpEvtStr->BMNxin[i],fpEvtStr->BMNyin[i],fpEvtStr->BMNzin[i]);
      TVector3 loc = fpFirstGeo->FromGlobalToBMLocal(gloc);
      
      TVector3 gmom(fpEvtStr->BMNpxin[i],fpEvtStr->BMNpyin[i],fpEvtStr->BMNpzin[i]);
      TVector3 mom = fpFirstGeo->VecFromGlobalToBMLocal(gmom);
      
      mytmp->SetAW(p_pargeo);
      
      //Finds and sets rdrift for a given hit
      mytmp->FindRdrift(loc,mom);
      
      resolution = p_parcon->ResoEval(mytmp->Dist());
      //      resolution = p_parcon->ResoEval(0.1);
      
      //      cout<<"TADAH "<<mytmp->Cell()<<" "<<mytmp->Plane()<<" "<<mytmp->View()<<" "<<mytmp->Dist()<<" "<<endl;
      
      
      mytmp->SetSigma(resolution);
      mytmp->SetTrkAss(0);
      nhits++;
    }
  }
  
  p_nturaw->nhit  = nhits;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}

