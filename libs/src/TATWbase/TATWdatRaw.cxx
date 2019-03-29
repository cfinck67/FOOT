/*!
  \file
  \version $Id: TATWdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TATWdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TATWdatRaw.hxx"
#include "TGraph.h"
#include "TF1.h"
/*!
  \class TATWdatRaw TATWdatRaw.hxx "TATWdatRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TATWrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TATWrawHit::~TATWrawHit()
{}


TATWrawHit::TATWrawHit(TWaveformContainer &W)
{
	// set channel/board id
	ir_chid=W.ChannelId;
	ir_boardid=W.BoardId;
	// do not change the order of these methods
	ir_pedestal=ComputePedestal(W);
	ir_amplitude=ComputeAmplitude(W);
	ir_chg= ComputeCharge(W);
	ir_time= ComputeTimeStamp(W);
}


Double_t TATWrawHit::ComputeCharge(TWaveformContainer &W)
{
	ir_chg=0;
	for(int bin=CHARGESTARTBIN;bin<CHARGESTOPBIN;bin++)
	{
		ir_chg+=W.W[bin]-ir_pedestal;
	}
	// the minus is to return a positive charge
	return -ir_chg;

}
Double_t TATWrawHit::ComputePedestal(TWaveformContainer &W)
{
	ir_pedestal=0;
	for (int bin=PEDESTALSTARTBIN;bin<PEDESTALSTOPBIN;++bin)
	{
		ir_pedestal+=W.W[bin];
	}
	return ir_pedestal/(PEDESTALSTOPBIN-PEDESTALSTARTBIN);
}
 Double_t TATWrawHit::ComputeAmplitude(TWaveformContainer &W)
 {
	 ir_amplitude=W.W[0];
	 for(int bin=AMPLITUDESTARTBIN;bin<AMPLITUDESTOPBIN;bin++)
	 {
		 if(ir_amplitude< W.W[bin])
		 {
			 ir_amplitude=W.W[bin];
		 }
	 }
	 return ir_amplitude-ir_pedestal;

 }
 Double_t TATWrawHit::ComputeTimeStamp(TWaveformContainer &W)
 {
	 TGraph gcfd = TGraph(5);
	 // evaluate the absolute threshold
	 Double_t AbsoluteThreshold=CFD_THREHSOLD*ir_amplitude+ir_pedestal;
	 for(int bin=TIMESTAMPSTARTBIN;bin<TIMESTAMPSTOPBIN;bin++)
	 {
		 if(W.W[bin]<AbsoluteThreshold)
		 {
			 //define  5 points to perform the linear fit
			 gcfd.SetPoint(0,W.T[bin-2],W.W[bin-2]);
			 gcfd.SetPoint(1,W.T[bin-1],W.W[bin-1]);
			 gcfd.SetPoint(2,W.T[bin]  ,W.W[bin]);
			 gcfd.SetPoint(3,W.T[bin+1],W.W[bin+1]);
			 gcfd.SetPoint(4,W.T[bin+2],W.W[bin+2]);
			 gcfd.Fit("pol1","Q","Q");
			 Double_t cfdp0=gcfd.GetFunction("pol1")->GetParameter(0);
			 Double_t cfdp1=gcfd.GetFunction("pol1")->GetParameter(1);
			 // extract the time from the parameters estimated by the fit
			 return (AbsoluteThreshold-cfdp0)/cfdp1;
		 }
	 }
 }

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWrawHit::TATWrawHit()
  : ir_time(999999.), ir_chg(0.), ir_typ(0), ir_chid(0),ir_pedestal(0),ir_amplitude(0),ir_boardid(0)
{
}

TATWrawHit::TATWrawHit(Int_t typ, Int_t cha ,Int_t board, Double_t charge, Double_t amplitude, Double_t pedestal, Double_t time)
{
  ir_time = time;
  ir_chg  = charge;
  ir_typ  = typ;
  ir_pedestal=pedestal;
  ir_amplitude=amplitude;
  ir_chid   = cha;
  ir_boardid=board;
}

void TATWrawHit::SetData(Int_t typ, Int_t cha ,Int_t board, Double_t charge, Double_t amplitude, Double_t pedestal, Double_t time) {

	ir_time = time;
	ir_chg  = charge;
	ir_typ  = typ;
	ir_pedestal=pedestal;
	ir_amplitude=amplitude;
	ir_chid   = cha;
	ir_boardid=board;
	return;
}
//##############################################################################

ClassImp(TATWdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWdatRaw::TATWdatRaw() :
  nirhit(0), hir(0) {
}


//------------------------------------------+-----------------------------------
//! Destructor.

TATWdatRaw::~TATWdatRaw() {
  delete hir;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TATWdatRaw::SetupClones()
{
  if (!hir) hir = new TClonesArray("TATWrawHit");
  return;
}


/*------------------------------------------+---------------------------------*/
//! Set statistics counters.

void TATWdatRaw::SetCounter(Int_t i_ntdc, Int_t i_nadc, Int_t i_ndrop)
{
  fiNTdc  = i_ntdc;
  fiNAdc  = i_nadc;
  fiNDrop = i_ndrop;
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TATWdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();
  nirhit = 0;
  if (hir) hir->Clear();
  return;
}

TATWrawHit*  TATWdatRaw::NewHit(Int_t typ, Int_t cha ,Int_t board, Double_t charge, Double_t amplitude, Double_t pedestal, Double_t time)
{
	hir->AddLast(new TATWrawHit(typ, cha ,board, charge, amplitude, pedestal, time));
	nirhit++;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TATWdatRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TATWdatRaw " << GetName()
	 << " nirhit"    << nirhit
     << endl;
  return;
}
