#ifndef DAQFILEHEADER_HH
#define DAQFILEHEADER_HH

#include "BaseFragment.hh"

class DAQFileHeader {

public:
  std::string writer;
  std::string filename;
  std::string guid;
  std::string stream;
  std::string project;
  std::string lumiblock;
  u_int fileNumber;

  void printData () const;

};

#endif
