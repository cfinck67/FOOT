/*!
  \file
  \version $Id: TATWactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TATWactDatRaw.
 */

#include "TATWparMap.hxx"
#include "CChannelMap.hxx"
#include "WDEvent.hh"
#include "TWaveformContainer.hxx"
#include "TAGdaqEvent.hxx"
#include "TATWactDatRaw.hxx"

/*!
  \class TATWactDatRaw TATWactDatRaw.hxx "TATWactDatRaw.hxx"
  \brief Get Beam Monitor raw data from WD. **
 */

ClassImp(TATWactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWactDatRaw::TATWactDatRaw(const char* name,
		TAGdataDsc* p_datraw,
		TAGdataDsc* p_datdaq,
		TAGparaDsc* p_parmap,
		TAGparaDsc* p_partime)
: TAGaction(name, "TATWactDatRaw - Unpack TW raw data"),
  fpDatRaw(p_datraw),
  fpDatDaq(p_datdaq),
  fpParMap(p_parmap),
  fpParTime(p_partime)
{
	AddDataOut(p_datraw, "TATWdatRaw");
	AddDataIn(p_datdaq, "TAGdaqEvent");
	AddPara(p_parmap, "TATWparMap");
	AddPara(p_partime, "TATWparTime");
	m_debug = kFALSE;
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATWactDatRaw::~TATWactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATWactDatRaw::Action() {

	if(GetDebugLevel()) { cout<<" Entering the TATWactDatRaw action "<<endl; }

	TATWdatRaw*    p_datraw = (TATWdatRaw*)   fpDatRaw->Object();
	TAGdaqEvent*   p_datdaq = (TAGdaqEvent*)  fpDatDaq->Object();
	TATWparMap*    p_parmap = (TATWparMap*)   fpParMap->Object();
	TATWparTime*   p_partime = (TATWparTime*)  fpParTime->Object();
	p_datraw->SetupClones();
	CChannelMap *cMap = p_parmap->getChannelMap();
	Int_t nFragments = p_datdaq->GetFragmentsN();
	for (Int_t i = 0; i < nFragments; ++i) {
		TString type = p_datdaq->GetClassType(i);
		if (type.Contains("WDEvent")) {
			const WDEvent* evt = static_cast<const WDEvent*> (p_datdaq->GetFragment(i));
			DecodeHits(evt, p_partime, p_datraw);
		}
	}
	fpDatRaw->SetBit(kValid);
	return kTRUE;

}

//------------------------------------------+-----------------------------------
//! Decoding

Bool_t TATWactDatRaw::DecodeHits(const WDEvent* evt, TATWparTime *p_parTime, TATWdatRaw *p_datraw)
{


	u_int word;

	int iW=0;
	int board_id=0, ch_num=0;
	float time_bin=0;
	float temperature=0, range=0, sampling_freq;
	int flags=0;
	int trig_cell=0;
	int fe_settings;
	int adc_sa=0;
	double v_sa=0;
	double tmp_tcal;
	double tmo_signal;
	int bco_counter, trig_type, ser_evt_number;
	vector<double> w_time;
	vector<float> w_tcal;
	vector<double> w_amp;
	TWaveformContainer w;
	m_debug=false;
	iW=0;
	int nhitsA = 0;
	bool foundFooter = false;
	while(iW < evt->evtSize && !foundFooter){

		if(evt->values.at(iW) == GLB_EVT_HEADER){
			if(m_debug)printf("found glb header::%08x %08x\n", evt->values.at(iW), evt->values.at(iW+1));
			iW+=6;

			//found evt_header
			if(evt->values.at(iW) == EVT_HEADER){
				if(m_debug)printf("found evt header::%08x   %08x   %08x\n", evt->values.at(iW),evt->values.at(iW+1),evt->values.at(iW+2));

				iW++;
				trig_type = evt->values.at(iW) & 0xffff;
				ser_evt_number = (evt->values.at(iW) >> 16) & 0xffff;

				iW++;
				bco_counter = (int)evt->values.at(iW);

				iW++;
				while((evt->values.at(iW) & 0xffff)== BOARD_HEADER){
					board_id = (evt->values.at(iW)>>16)  & 0xffff;
					if(m_debug)printf("found board header::%08x num%d\n", evt->values.at(iW), board_id);
					iW++;
					temperature = *((float*)&evt->values.at(iW));
					if(m_debug)printf("temperatrue::%08x num%d\n", evt->values.at(iW), board_id);


					iW++;
					range = *((float*)&evt->values.at(iW));

					if(m_debug)printf("range::%08x num%d\n", evt->values.at(iW), board_id);

					iW++;
					sampling_freq = (float)(( evt->values.at(iW)>> 16)&0xffff);
					flags = evt->values.at(iW) & 0xffff;
					if(m_debug)printf("sampling::%08x    %08x   %08x    num%d\n", evt->values.at(iW),evt->values.at(iW+1),evt->values.at(iW+2), board_id);


					iW++;

					while((evt->values.at(iW) & 0xffff)== CH_HEADER){

						char tmp_chstr[3]={'0','0','\0'};
						tmp_chstr[1] = (evt->values.at(iW)>>24)  & 0xff;
						tmp_chstr[0] = (evt->values.at(iW)>>16)  & 0xff;
						ch_num = atoi(tmp_chstr);
						if(m_debug)printf("found channel header::%08x num%d\n", evt->values.at(iW), ch_num);

						iW++;
						trig_cell = (evt->values.at(iW)>>16) &0xffff;

						fe_settings = ((evt->values.at(iW))&0xffff);
						iW++;

						w_amp.clear();
						for(int iSa=0;iSa<512;iSa++){
							if(m_debug)printf("found sample isa::%d    ::%08x\n", iSa, evt->values.at(iW));
							adc_sa = evt->values.at(iW);
							v_sa = ((adc_sa >> 16) & 0xffff)/65536.-0.5+range;
							w_amp.push_back(v_sa);
							v_sa = (adc_sa & 0xffff)/65536.-0.5+range;
							w_amp.push_back(v_sa);
							iW++;
						}

						p_parTime->GetTimeArray(board_id, ch_num, trig_cell, &w_time);
						w.ChannelId = ch_num;
						w.BoardId = board_id;
						for(int iw = 0; iw<w_amp.size(); iw++) {
							w.T[iw] = w_time.at(iw);
							w.W[iw] = w_amp.at(iw);
						}
						p_datraw->NewHit(w);

						if(ValidHistogram()) {
							if (nhitsA<wv0.size())
								w.GraphWaveForm(wv0[nhitsA]);
						}
						nhitsA++;
						w.Clear();
						w_amp.clear();
						w_time.clear();

					}
				}
			}

			if(evt->values.at(iW) == EVT_FOOTER){
				//      printf("found footer\n");
				iW++;
				foundFooter = true;
			}else{
				printf("warining:: footer not found, event corrupted, iW::%d   last word::%08x!!\n",iW, evt->values.at(iW));
				break;
			}
		}
	}


	return true;
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TATWactDatRaw::CreateHistogram()
{
	// max number of histo
	Int_t MaxHist0=20;
	DeleteHistogram();
	wv0.resize(MaxHist0);
	//
	for (int i=0;i<MaxHist0;++i)
	{
		wv0[i] = new TH1F();
		wv0[i]->SetName("WD"+TString::Format("%d",i));
		wv0[i]->SetTitle("WD graph"+TString::Format("%d",i));
		AddHistogram(wv0[i]);
	}
	SetValidHistogram(kTRUE);
}

