#ifndef FADCEVENT_HH
#define FADCEVENT_HH

#include <vector>
#include "BaseFragment.hh"

class fADCEvent : public BaseFragment {

  public:
    u_int word;
    u_int data;
    u_int nchans;
    u_int eventNumber;
    std::vector<bool> active;
    std::vector<u_int> channel[8];
 
    virtual ~fADCEvent();
  
    virtual void readData(unsigned int **p);
    virtual void printData() const;
    virtual bool check() const { return true;};

    virtual std::string classType() const {return "fADCEvent";};
 
  private:
  
    void printV ();
    int getWordsChannel( int word, int nchans );
    int getnchans ( int data );
	
};

#endif
