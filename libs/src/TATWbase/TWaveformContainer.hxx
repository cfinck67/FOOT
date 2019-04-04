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

#define VOLTAGE_TS 0.5
#define CFD_THREHSOLD 0.3
// this class holds the waveform, channel id of the WD and the BoardId
class TWaveformContainer
{


public:
  TWaveformContainer();
  ~TWaveformContainer();
  TWaveformContainer(const TWaveformContainer &other);
  Double_t ComputeCharge();
  Double_t ComputePedestal();
  Double_t ComputeAmplitude();
  Double_t ComputeTimeStamp();
  void Clear();
  void PlotWaveForm(int i);
  void SanitizeWaveform();
  Int_t ChannelId;
  Int_t BoardId;
  Double_t ir_time;    
  Double_t ir_chg;
  Double_t ir_pedestal;
  Double_t ir_amplitude;
  
  Double_t *T;
  Double_t *W;

};

#endif
