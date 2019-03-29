#ifndef TWAVEFORM_CONTAINER
#define TWAVEFORM_CONTAINER

#define WAVEFORMBINS 1024
// this class holds the waveform, channel id of the WD and the BoardId
class TWaveformContainer
{
public:
	Int_t ChannelId;
	Int_t BoardId;
	Double_t T[WAVEFORMBINS];
	Double_t W[WAVEFORMBINS];
};

#endif
