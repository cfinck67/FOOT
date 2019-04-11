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

TString TATWdatRaw::fgkBranchName   = "twdat.";

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
	ir_pedestal=W.ComputePedestal();
	ir_amplitude=W.ComputeAmplitude();
	ir_chg= W.ComputeCharge();
	ir_time= W.ComputeTimeStamp();
	ir_triggertype=W.TrigType;
}

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWrawHit::TATWrawHit()
  : ir_time(999999.), ir_chg(0.), ir_chid(0),ir_pedestal(0),
	ir_amplitude(0),ir_boardid(0), ir_isclock(0),ir_clock_time(0),ir_triggertype(0)
{
}

TATWrawHit::TATWrawHit(Int_t cha ,Int_t board, Double_t charge,
					   Double_t amplitude, Double_t pedestal,
					   Double_t time,Int_t isclock,Double_t clock_time,
					   Int_t TriggerType )
{
  ir_time = time;
  ir_chg  = charge;
  ir_pedestal=pedestal;
  ir_amplitude=amplitude;
  ir_chid   = cha;
  ir_boardid=board;
  ir_isclock=isclock;
  ir_clock_time=clock_time;
  ir_triggertype=TriggerType;
}


  void TATWrawHit::SetData(Int_t cha ,Int_t board, Double_t charge,
		  	  	  	  	   Double_t amplitude, Double_t pedestal, Double_t time,
						   Int_t isclock,Double_t clock_time,
						   Int_t TriggerType) {

	ir_time = time;
	ir_chg  = charge;
	ir_pedestal=pedestal;
	ir_amplitude=amplitude;
	ir_chid   = cha;
	ir_boardid=board;
	ir_isclock=isclock;
	ir_clock_time=clock_time;
	ir_triggertype=TriggerType;
	return;
}

  Int_t  TATWrawHit::IsClock()
  {
	  return ir_isclock;
  }
  Double_t TATWrawHit::Clocktime()
  {
	  if (IsClock())
	  {
		  return ir_clock_time;
	  }
	  return -1;
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


void TATWdatRaw::NewHit(TWaveformContainer &W)
{
  // get channel/board id
  Int_t cha =W.ChannelId;
  Int_t board =W.BoardId;
  W.SanitizeWaveform();
  // do not change the order of these methods
  Double_t pedestal=W.ComputePedestal();
  Double_t amplitude=W.ComputeAmplitude();
  Double_t charge= W.ComputeCharge();
  Double_t time= W.ComputeTimeStamp();
  Double_t ClockRaisingTime=-1;
  Int_t TriggerType= W.TrigType;
  if (W.IsAClock())
  {
	  ClockRaisingTime=W.FindFirstRaisingEdgeTime();
  }
  TClonesArray &pixelArray = *hir;
  TATWrawHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TATWrawHit(cha ,board, charge, amplitude, pedestal, time,W.IsAClock(),ClockRaisingTime,TriggerType);
  nirhit++;
  return;
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
