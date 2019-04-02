#ifndef MESSAGE_H
#define MESSAGE_H
#include <iostream>
#include "TString.h"

namespace Message {
  void DisplayWarning(TString Message,std::ostream &os=std::cout);
  void DisplayFatalError(TString Message,std::ostream &os=std::cout);
  void DisplayMessage(TString Message,std::ostream &os=std::cout);
  void DisplayMessageWithEmphasys(TString Message,std::ostream &os=std::cout);
};
#endif
