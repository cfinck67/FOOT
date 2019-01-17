/*!
  \file
  \version $Id: TABMactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TABMactDatRaw.
*/

#include "TABMparMap.hxx"
#include "TABMparCon.hxx"
//~ #include "TAGmbsEvent.hxx"
#include "TABMdatRaw.hxx"
#include "TABMrawHit.hxx"
#include "TATRdatRaw.hxx"

#include "TABMactDatRaw.hxx"
#include <iomanip>

/*!
  \class TABMactDatRaw TABMactDatRaw.hxx "TABMactDatRaw.hxx"
  \brief Get Beam Monitor raw data from MBS. **
*/

ClassImp(TABMactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactDatRaw::TABMactDatRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGparaDsc* p_parmap,
			     TAGparaDsc* p_parcon,
			     TAGparaDsc* p_pargeo,
			     TAGdataDsc* p_timraw, 
              BM_struct*  p_bmstruct)
  : TAGaction(name, "TABMactDatRaw - Unpack BM raw data"),
    fpDatRaw(p_datraw),
    fpParMap(p_parmap),
    fpParCon(p_parcon),
    fpParGeo(p_pargeo),
    fpTimRaw(p_timraw),
    fpEvtStruct(p_bmstruct)
{
  AddDataOut(p_datraw, "TABMdatRaw");
  AddPara(p_parmap, "TABMparMap");
  AddPara(p_parcon, "TABMparCon");
  AddPara(p_pargeo, "TABMparGeo");
  AddDataIn(p_timraw, "TAIRdatRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactDatRaw::~TABMactDatRaw()
{
  //~ datastream.close();    
}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TABMactDatRaw::Action() {
  //~ cout<<"sono in action di tabmactdatraw"<<endl;
  
  TABMdatRaw*    p_datraw = (TABMdatRaw*)    fpDatRaw->Object();
  
  //From there we get the Mapping of the wires into the Chamber to the TDC channels
  TABMparMap*    p_parmap = (TABMparMap*)    fpParMap->Object();
  TABMparCon*    p_parcon = (TABMparCon*)    fpParCon->Object();
  TABMparGeo*    p_pargeo = (TABMparGeo*)    fpParGeo->Object();
  TATRdatRaw*    p_timraw = (TATRdatRaw*)    fpTimRaw->Object();
  
  Int_t view,plane,cell;
    
  for(Int_t i=0;i<fpEvtStruct->tdc_hitnum[0];i++){
    if(p_parmap->tdc2cell(fpEvtStruct->tdc_id[i])>=0 && (((Double_t) (fpEvtStruct->tdc_meas[i])/10.) - p_parcon->GetT0(p_parmap->tdc2cell(fpEvtStruct->tdc_id[i]))-p_timraw->TrigTime())<300.){//-1000=syncTime, -1=not set
      p_pargeo->GetBMNlvc(p_parmap->tdc2cell(fpEvtStruct->tdc_id[i]),plane,view,cell);
      p_datraw->SetHitData(plane,view,cell,(Double_t) (fpEvtStruct->tdc_meas[i])/10.);
    }
  }
  
  
  fpDatRaw->SetBit(kValid);
  return kTRUE;
}

