#ifndef WDEVENT_HH
#define WDCEVENT_HH
#include "BaseFragment.hh"

#include <vector>

#define NBOARDS 1

///////////////////////////////////////////////event structs definition //////////////////////////////

//gtraini:types have to be checked

typedef struct {
   char           tag[3];
   char           version;
} FHEADER;

typedef struct {
   char           time_header[4];
} THEADER;

typedef struct {
   char           bn[2];
   unsigned short board_serial_number;
} BHEADER;

typedef struct {
   char           event_header[4];
   unsigned int   event_serial_number;
   unsigned short year;
   unsigned short month;
   unsigned short day;
   unsigned short hour;
   unsigned short minute;
   unsigned short second;
   unsigned short millisecond;
   unsigned short range;
} EHEADER;

typedef struct {
   char           tc[2];
   unsigned short trigger_cell;
} TCHEADER;

typedef struct {
   char           c[1];
   char           cn[3];
} CHEADER;

/////////////////////////////////////////////////////////////////


class WDEvent : public BaseFragment {

public:

  
  FHEADER  m_fh;
  THEADER  m_th;
  BHEADER  m_bh;
  EHEADER  m_eh;
  TCHEADER m_tch;
  CHEADER  m_ch;

  u_int eventNumber;
  u_int glbHeader;

  std::map<int,std::vector<float>> m_time_calib;
  std::map<int,std::vector<double>> m_time;
  std::map<int,std::vector<double>> m_voltage;

 
  virtual ~WDEvent();

  virtual void readData(unsigned int **p);
  virtual void printData() const;
  virtual bool check() const{return true;};

  virtual std::string classType() const {return "WDEvent";};
};

#endif
