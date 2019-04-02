#include "TWaveformContainer.hxx"
#include "TGraph.h"


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
