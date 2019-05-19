
#include "TAIRdatRaw.hxx"
#include "TAIRactDatRaw.hxx"

ClassImp(TAIRactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.
 
TAIRactDatRaw::TAIRactDatRaw(const char* name,
			 TAGdataDsc* p_datraw, 
       TAGparaDsc* p_parmap,
			 BM_struct* p_bmstruct)
  : TAGaction(name, "TAIRactDatRaw - Unpack Start Counter raw data"),
    fpNtuDatRaw(p_datraw),
    fpParMap(p_parmap),
    bmstruct(p_bmstruct)
{
  AddDataOut(p_datraw, "TAIRdatRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAIRactDatRaw::~TAIRactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAIRactDatRaw::Action()
{

  TAIRdatRaw* p_nturaw = (TAIRdatRaw*) fpNtuDatRaw->Object();
  TABMparMap*    p_parmap = (TABMparMap*)    fpParMap->Object();
  
  Int_t nhits(0);
  if (!p_nturaw->hir) p_nturaw->SetupClones();
  //~ Double_t meantime(0);
 
  //for the moment only one hit for each event... (det type,channel,charge,time)
  while(bmstruct->tdc_sync[nhits]!=-10000){
    new((*(p_nturaw->hir))[nhits]) TAIRrawHit(0,p_parmap->GetTrefCh(),0,bmstruct->tdc_sync[nhits]);
    //~ meantime+=bmstruct->tdc_sync[nhits];
    nhits++;
  }
  
  //~ Int_t i_nhit = p_nturaw->nirhit;
  //~ for (Int_t i = 0; i < nhits; i++) {
    //~ const TAIRrawHit* aHi = p_nturaw->Hit(i);
    //~ if(aHi->Type() == 0) {
      //~ if(aHi->ChID() == p_parmap->GetTrefCh()) 
        //~ firsttime = aHi->Time();
      //~ meantime += aHi->Time();
    //~ }
  //~ }
  //~ if(nhits) meantime /= nhits;
  
  //Set up of the Trigger Time
  if(bmstruct->tdc_sync[1]==-10000)
    p_nturaw->SetTrigTime((Double_t) bmstruct->tdc_sync[0]/10.);
  else
    p_nturaw->SetTrigTime((Double_t) bmstruct->tdc_sync[1]/10.);//due to a hardware prob., when there are two sync time, I have to take the second one
    

  p_nturaw->nirhit  = nhits;

  fpNtuDatRaw->SetBit(kValid);
  return kTRUE;
}

