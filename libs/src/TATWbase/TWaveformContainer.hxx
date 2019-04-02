#include "TF1.h"

#ifndef TWAVEFORM_CONTAINER
#define TWAVEFORM_CONTAINER

#define WAVEFORMBINS 1024
// define  some parameters
#define PEDESTALSTARTBIN 10
#define PEDESTALSTOPBIN 60

#define AMPLITUDESTARTBIN 61
#define AMPLITUDESTOPBIN  800

#define CHARGESTARTBIN 0
#define CHARGESTOPBIN  1023

#define TIMESTAMPSTARTBIN 61
#define TIMESTAMPSTOPBIN  800

#define CFD_THREHSOLD 0.3
// this class holds the waveform, channel id of the WD and the BoardId
class TWaveformContainer
{
public:
  Double_t ComputeCharge();
  Double_t ComputePedestal();
  Double_t ComputeAmplitude();
  Double_t ComputeTimeStamp();

  Int_t ChannelId;
  Int_t BoardId;
  Double_t T[WAVEFORMBINS];
  Double_t W[WAVEFORMBINS];
  
  Double_t ir_time;    
  Double_t ir_chg;
  Double_t ir_pedestal;
  Double_t ir_amplitude;
  
  
};

#endif
