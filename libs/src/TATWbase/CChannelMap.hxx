#ifndef CCHANNELMAP_H
#define CCHANNELMAP_H

#include <string>
#include <map>
#include <TSystem.h>
#include "Parameters.h"

enum TALayer {NoLayer=-1,Front=0,Rear=1};

typedef std::map<TBarId,TALayer> TMapBarIdLayerId;
typedef std::tuple <TBoardId,TChannelId,TChannelId> TChannelBoardTuple;
typedef std::map<TBarId,TChannelBoardTuple> TChannelPairMapType;


class CChannelMap
{
	TMapBarIdLayerId _BarLayer;
    TChannelPairMapType _ChannelBarMap;
    bool _ChannelMapIsOk;
public:
    CChannelMap();
    bool Exists(TBarId BarId);
	void LoadChannelMap(std::string Filename,int verbose=0);
	TALayer GetBarLayer(TBarId BarId);
	TChannelBoardTuple GetChannelABar(TBarId BarId);
	TChannelPairMapType::iterator begin();
	TChannelPairMapType::iterator end();
	Int_t GetNumberOfBars();
	std::vector<TBarId> GetBarIds();
};


#endif
