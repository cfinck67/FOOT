#include "TWaveformContainer.hxx"
#include "TGraph.h"
#include "TAxis.h"





TWaveformContainer::TWaveformContainer()
{
	ChannelId=0;
	BoardId=0;
	ir_time=0;
	ir_chg=0;
	ir_pedestal=0;
	ir_amplitude=0;
	T=(Double_t *)malloc(sizeof(Double_t)*WAVEFORMBINS);
	W=(Double_t *)malloc(sizeof(Double_t)*WAVEFORMBINS);
}

void TWaveformContainer::Clear()
{
	ChannelId=0;
	BoardId=0;
	ir_time=0;
	ir_chg=0;
	ir_pedestal=0;
	ir_amplitude=0;
	memset((void *)T,0,sizeof(Double_t)* WAVEFORMBINS);
	memset((void *)W,0,sizeof(Double_t)* WAVEFORMBINS);

}

TWaveformContainer::TWaveformContainer(const TWaveformContainer &other)
{
    std::memcpy(T, other.T, sizeof(Double_t)*WAVEFORMBINS);
    std::memcpy(W, other.W, sizeof(Double_t)*WAVEFORMBINS);

	ChannelId=other.ChannelId;
	BoardId=other.BoardId;
	ir_time=other.ir_time;
	ir_chg=other.ir_chg;
	ir_pedestal=other.ir_pedestal;
	ir_amplitude=other.ir_amplitude;

}

Double_t TWaveformContainer::ComputeCharge()
{
	ir_chg=0;
	for(int bin=CHARGESTARTBIN;bin<CHARGESTOPBIN;bin++)
	{
		ir_chg+=W[bin]-ir_pedestal;
	}
	// the minus is to return a positive charge
	return -ir_chg;

}
Double_t TWaveformContainer::ComputePedestal()
{
	ir_pedestal=0;
	for (int bin=PEDESTALSTARTBIN;bin<PEDESTALSTOPBIN;++bin)
	{
		ir_pedestal+=W[bin];
	}
	return ir_pedestal/(PEDESTALSTOPBIN-PEDESTALSTARTBIN);
}
 Double_t TWaveformContainer::ComputeAmplitude()
 {
	 ir_amplitude=W[0];
	 for(int bin=AMPLITUDESTARTBIN;bin<AMPLITUDESTOPBIN;bin++)
	 {
		 if(ir_amplitude< W[bin])
		 {
			 ir_amplitude=W[bin];
		 }
	 }
	 return ir_amplitude-ir_pedestal;

 }
 Double_t TWaveformContainer::ComputeTimeStamp()
 {
	 TGraph gcfd = TGraph(5);
	 // evaluate the absolute threshold
	 Double_t AbsoluteThreshold=CFD_THREHSOLD*ir_amplitude+ir_pedestal;
	 for(int bin=TIMESTAMPSTARTBIN;bin<TIMESTAMPSTOPBIN;bin++)
	 {
		 if(W[bin]<AbsoluteThreshold)
		 {
			 //define  5 points to perform the linear fit
			 gcfd.SetPoint(0,T[bin-2],W[bin-2]);
			 gcfd.SetPoint(1,T[bin-1],W[bin-1]);
			 gcfd.SetPoint(2,T[bin]  ,W[bin]);
			 gcfd.SetPoint(3,T[bin+1],W[bin+1]);
			 gcfd.SetPoint(4,T[bin+2],W[bin+2]);
			 gcfd.Fit("pol1","Q","Q");
			 Double_t cfdp0=gcfd.GetFunction("pol1")->GetParameter(0);
			 Double_t cfdp1=gcfd.GetFunction("pol1")->GetParameter(1);
			 // extract the time from the parameters estimated by the fit
			 return (AbsoluteThreshold-cfdp0)/cfdp1;
		 }
	 }
	 return -1.;
 }

 void TWaveformContainer::PlotWaveForm()
 {
	 TGraph *g=new TGraph(WAVEFORMBINS,T,W);
	 g->GetXaxis()->SetTitle("t (s)");
	 g->GetYaxis()->SetTitle("Amplitude (V)");
	 g->SetTitle(TString::Format(" Board %d Channel %d",BoardId,ChannelId));
	 g->Draw("AC");
 }
