#ifndef INFOEVENT_HH
#define INFOEVENT_HH

#include "BaseFragment.hh"

class InfoEvent : public BaseFragment {

public:
  u_int time_sec;
  u_int time_nsec;
  u_int runType;
  u_int runNumber;
  u_int eventNumber;

  InfoEvent(){};
  virtual ~InfoEvent();
  
  virtual void readData(unsigned int **p);
  virtual void printData() const;
  virtual bool check() const { return true;};

  virtual std::string classType() const {return "InfoEvent";};
};

#endif
