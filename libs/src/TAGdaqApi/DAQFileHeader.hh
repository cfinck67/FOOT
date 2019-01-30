#ifndef DAQFILEHEADER_HH
#define DAQFILEHEADER_HH

class DAQFileHeader {

public:
  std::string writer;
  std::string filename;
  std::string guid;
  std::string stream;
  std::string project;
  std::string lumiblock;

  void printData () const;

};

#endif
