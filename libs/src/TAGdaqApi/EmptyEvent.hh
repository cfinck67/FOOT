#ifndef EMPTYEVENT_HH
#define EMPTYEVENT_HH

#include "BaseFragment.hh"

class EmptyEvent : public BaseFragment {

  public:
    u_int time_sec;
    u_int time_usec;
    u_int lumiBlock;
    u_int eventNumber;

    virtual ~EmptyEvent();
  
    virtual void readData(unsigned int **p);
    virtual void printData() const;
    virtual bool check() const { return true;};

    virtual std::string classType() const {return "EmptyEvent";};

};

#endif
