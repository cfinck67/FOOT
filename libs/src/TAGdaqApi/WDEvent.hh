#ifndef WDEVENT_HH
#define WDEVENT_HH

#include "RemoteEvent.hh"

class WDEvent : public RemoteEvent {

  public:
    //firmware data
    u_int detectorHeader;
    u_int boardHeader;
    u_int numberWords;

    virtual ~WDEvent();
  
    virtual void readData(unsigned int **p);
    virtual void printData() const;
    virtual bool check() const{return true;};

    virtual std::string classType() const {return "WDEvent";};
  
};

#endif
