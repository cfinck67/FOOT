#include "TWaveformContainer.hxx"
#include "TGraph.h"
#include "TAxis.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TMath.h"
#include "TLegend.h"
#include "Parameters.h"


TWaveformContainer::TWaveformContainer()
{
	T=(Double_t *)malloc(sizeof(Double_t)*WAVEFORMBINS);
	W=(Double_t *)malloc(sizeof(Double_t)*WAVEFORMBINS);
	Clear();
}

void TWaveformContainer::Clear()
{
	ChannelId=0;
	BoardId=0;
	ir_time=0;
	ir_chg=0;
	ir_pedestal=0;
	ir_amplitude=0;
	ir_pedestalstd=0;
	memset((void *)T,0,sizeof(Double_t)* WAVEFORMBINS);
	memset((void *)W,0,sizeof(Double_t)* WAVEFORMBINS);
}

TWaveformContainer::TWaveformContainer(const TWaveformContainer &other)
{
	ChannelId=other.ChannelId;
	BoardId=other.BoardId;
	ir_time=other.ir_time;
	ir_chg=other.ir_chg;
	ir_pedestal=other.ir_pedestal;
	ir_amplitude=other.ir_amplitude;
	memcpy(T, other.T, sizeof(Double_t)*WAVEFORMBINS);
	memcpy(W, other.W, sizeof(Double_t)*WAVEFORMBINS);
}

Double_t TWaveformContainer::ComputeCharge()
{
	ir_chg=0;
	for(int bin=CHARGESTARTBIN;bin<CHARGESTOPBIN;bin++)
	{
		ir_chg+=W[bin]-ir_pedestal;
	}
	// the minus is to return a positive charge
	ir_chg*=-1;
	return ir_chg;

}
Double_t TWaveformContainer::ComputePedestal()
{
	ir_pedestal=0;
	// this could be useful to flag pileup events
	ir_pedestalstd=TMath::RMS(PEDESTALSTOPBIN-PEDESTALSTARTBIN+1,&W[PEDESTALSTARTBIN]);
	ir_pedestal=TMath::Mean(PEDESTALSTOPBIN-PEDESTALSTARTBIN+1,&W[PEDESTALSTARTBIN]);
	return ir_pedestal;
}
 Double_t TWaveformContainer::ComputeAmplitude()
 {
	 ir_amplitude=W[AMPLITUDESTARTBIN];
	 // rember TW => negative signals
	 ir_amplitude=TMath::MinElement(AMPLITUDESTOPBIN-AMPLITUDESTARTBIN+1,&W[AMPLITUDESTARTBIN]);
	 ir_amplitude-=ir_pedestal;
	 return ir_amplitude;

 }
 Double_t TWaveformContainer::ComputeTimeStamp()
 {
	 TGraph gcfd = TGraph(5);
	 // evaluate the absolute threshold
	 Double_t AbsoluteThreshold=CFD_THREHSOLD*ir_amplitude+ir_pedestal;
	 for(int bin=TIMESTAMPSTARTBIN;bin<TIMESTAMPSTOPBIN;++bin)
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
			 ir_time=(AbsoluteThreshold-cfdp0)/cfdp1;
			 return ir_time;
		 }
	 }
	 return -1.;
 }

 // DEBUG ONLY
 void TWaveformContainer::PlotWaveForm(int i)
 {
	 Double_t max=W[0];
	 Double_t min=W[0];
	 for (int bin=0;bin<WAVEFORMBINS;++bin)
	 {
		 if (W[bin]>max)
		 {
			 max=W[bin];
		 }
		 if (W[bin]<min)
		 {
			 min=W[bin];
		 }
	 }
	 TCanvas *c = new TCanvas;
	 c->Range(T[0],min,T[WAVEFORMBINS-1],max);
	 TGraph *g=new TGraph(WAVEFORMBINS,T,W);
	 g->GetXaxis()->SetTitle("t (ns)");
	 g->GetYaxis()->SetTitle("Amplitude (V)");
	 g->SetTitle(TString::Format(" Board %d Channel %d",BoardId,ChannelId));
	 TLine *ped = new TLine(T[0],ir_pedestal,T[WAVEFORMBINS-1],ir_pedestal);
	 ped->SetLineWidth(2);
	 ped->SetLineColor(kGreen);
	 TLine *ampl = new TLine(T[0],ir_amplitude+ir_pedestal,T[WAVEFORMBINS-1],ir_pedestal+ir_amplitude);
	 ampl->SetLineColor(kRed);
	 ampl->SetLineWidth(2);
	 g->Draw("AC");
	 ped->Draw();
	 ampl->Draw();
	 c->SaveAs(TString::Format("Test%d.png",i));
	 c->Close();
	 gSystem->ProcessEvents();
 }

 void TWaveformContainer::SanitizeWaveform()
 {
	 Double_t old=W[WAVEFORMBINS-1];
	 if (old<-0.8)
	 {
		 old+=1;
		 W[WAVEFORMBINS-1]+=1;
	 }
	 for (int bin=WAVEFORMBINS-2;bin>0;--bin)
	 {
		 Double_t derivative=(W[bin]-old);
		 if (fabs(derivative)>VOLTAGE_TS)
		 {
			W[bin]-=TMath::Sign(1,derivative);
		 }
		 old=W[bin];
	 }
 }

 TWaveformContainer::~TWaveformContainer()
 {
	 if (T!=nullptr)
	 {
		 free(T);
	 }
	 if (W!=nullptr)
	 {
		 free(W);
	 }
 }
