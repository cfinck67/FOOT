#ifndef TDCEVENT_HH
#define TDCEVENT_HH
#include "BaseFragment.hh"

#include <vector>

class TDCEvent : public BaseFragment {

public:
  u_int tdc_sec;
  u_int tdc_usec;
  u_int eventNumber;
  u_int globalHeader;
  std::vector<u_int> tdcHeader;
  std::vector<u_int> measurement;//measurement read directly from the TDC 7 bits (19-25) for the channel and 19 bits for the meas (0-18) 
  std::vector<u_int> tdcTrailer;
  u_int globalTrailer;

  virtual ~TDCEvent();

  virtual void readData(unsigned int **p);
  virtual void printData() const;
  virtual bool check() const{return true;};

  virtual std::string classType() const {return "TDCEvent";};
};

#endif
