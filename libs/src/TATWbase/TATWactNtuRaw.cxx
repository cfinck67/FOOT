/*!
  \file
  \version $Id: TATWactNtuRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TATWactNtuRaw.
*/

#include "TATWparMap.hxx"
#include "TATWactNtuRaw.hxx"
#include "TMath.h"
#include <map>
#include "TATWparCal.hxx"
#include "CCalibrationMap.hxx"

/*!
  \class TATWactNtuRaw TATWactNtuRaw.hxx "TATWactNtuRaw.hxx"
  \brief Get Beam Monitor raw data from WD. **
*/

ClassImp(TATWactNtuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWactNtuRaw::TATWactNtuRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_nturaw,
			     TAGparaDsc* p_pargeom,
				 TAGparaDsc* p_parmap,
				 TAGparaDsc* p_calmap)
  : TAGaction(name, "TATWactNtuRaw - Unpack TW raw data"),
    fpDatRaw(p_datraw),
    fpNtuRaw(p_nturaw),
    fpParGeo(p_pargeom),
	fpParMap(p_parmap),
	fpCalPar(p_calmap)

{
  AddDataIn(p_datraw, "TATWdatRaw");
  AddDataOut(p_nturaw, "TATWntuRaw");
  AddPara(p_pargeom, "TATWparGeo");
  AddPara(p_parmap,"TATWparMap");
  AddPara(p_calmap,"TATWparCal");

}

//------------------------------------------+-----------------------------------
//! Destructor.

TATWactNtuRaw::~TATWactNtuRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATWactNtuRaw::Action() {

   TATWdatRaw*    p_datraw = (TATWdatRaw*) fpDatRaw->Object();
   TATWntuRaw*   p_nturaw = (TATWntuRaw*)  fpNtuRaw->Object();
   TATWparGeo*   p_pargeo = (TATWparGeo*)  fpParGeo->Object();
   TATWparMap*   p_parmap = (TATWparMap*)  fpParMap->Object();

   p_nturaw->SetupClones();
   CChannelMap *c=p_parmap->getChannelMap();
   int nhit = p_datraw->nirhit;
   //
   map<int, vector<TATWrawHit*> > PMap;
   //
   for(int ih = 0; ih< nhit; ih++)
   {
	   TATWrawHit *aHi = p_datraw->Hit(ih);
	   //
	   if (PMap.find(aHi->BoardId())==PMap.end())
	   {
		   PMap[aHi->BoardId()].resize(NUMBEROFCHANNELS);
		   for (int ch=0;ch<NUMBEROFCHANNELS;++ch)
		   {
			   PMap[aHi->BoardId()][ch]=nullptr;
		   }
	   }
	   PMap[aHi->BoardId()][aHi->ChID()]=aHi;
   }
   //
   for (auto it=c->begin();it!=c->end();++it)
   {
	   Int_t boardid=get<0>(it->second);
	   int channelA=get<1>(it->second);
	   int channelB=get<2>(it->second);
	   int BarId=it->first;
	   if (PMap.find(boardid)!=PMap.end())
	   {
		   //
		   TATWrawHit* hita=PMap[boardid][channelA];
		   TATWrawHit* hitb=PMap[boardid][channelB];
		   if (hita!=nullptr && hitb!=nullptr )
		   {
			   //
			   Double_t RawEnergy=GetRawEnergy(hita,hitb);
			   Double_t Energy=GetEnergy(RawEnergy,BarId);
			   //
			   Double_t RawTime=GetRawTime(hita,hitb);
			   Double_t Time=GetTime(RawTime,BarId);
			   p_nturaw->NewHit(c->GetBarLayer(BarId),BarId, Energy,Time,0);
		   }
	   }
   }
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}


Double_t TATWactNtuRaw::GetRawEnergy(TATWrawHit*a,TATWrawHit*b)
{
	if (a->Charge()<0|| b->Charge()<0)
	{
		std::cout <<std::dec<< a->ChID() << " " << b->ChID() << " " << a->BoardId() << " " << b->BoardId() <<std::endl;
		return -1;
	}
	return TMath::Sqrt(a->Charge()*b->Charge());
}
Double_t TATWactNtuRaw::GetEnergy(Double_t RawEnergy,Int_t BarId)
{
	if (RawEnergy<0)
	{
		return -1;
	}
	TATWparCal*   p_calmap = (TATWparCal*)    fpCalPar->Object();
	Double_t p0=p_calmap->getCalibrationMap()->GetBarParameter(BarId,0);
	Double_t p1=p_calmap->getCalibrationMap()->GetBarParameter(BarId,1);
	// correct using the Birk's Law
	return RawEnergy/(p0-RawEnergy*p1);
}

Double_t  TATWactNtuRaw::GetTime(Double_t RawTime, Int_t BarId)
{
	return RawTime;
}

Double_t TATWactNtuRaw::GetRawTime(TATWrawHit*a,TATWrawHit*b)
{
	return (a->Time()+b->Time())/2;

}

