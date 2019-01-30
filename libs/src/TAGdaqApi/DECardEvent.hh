#ifndef DECARDEVENT_HH
#define DECARDEVENT_HH

#include "RemoteEvent.hh"

class DECardEvent : public RemoteEvent {

  public:
    // firmware data
    u_int detectorHeader;
    u_int boardHeader;
    u_int triggerCounter;
    u_int BCOofTrigger;
    u_int clockCounter;

    virtual ~DECardEvent();
  
    virtual void readData(unsigned int **p);
    virtual void printData() const;
    virtual bool check() const;

    virtual std::string classType() const {return "DECardEvent";};
  
};

#endif
