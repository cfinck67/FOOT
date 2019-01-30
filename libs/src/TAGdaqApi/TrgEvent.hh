#ifndef TRGEVENT_HH
#define TRGEVENT_HH
#include "BaseFragment.hh"

class TrgEvent : public BaseFragment {

public:
  u_int time_sec;
  u_int time_usec;
  u_int eventNumber;
  // firmware data
  u_int liveTime;
  u_int timeSinceLastTrigger;
  u_int clockCounter;
  u_int triggerCounter;
  u_int BCOofTrigger;
  u_int spillNrAndTrgFineDelay;
  u_int PMTsAndBusy;

  TrgEvent(){};
  virtual ~TrgEvent();
  
  virtual void readData(unsigned int **p);
  virtual void printData()const;
  virtual bool check() const { return eventNumber==triggerCounter;};

  virtual std::string classType() const {return "TrgEvent";};
};

#endif
