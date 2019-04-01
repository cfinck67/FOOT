#include "Message.hxx"

void Message::DisplayWarning(TString Message, std::ostream &os)
{
  os << "WARNING: " << Message <<std::endl;
}
void Message::DisplayFatalError(TString Message,std::ostream &os)
{
  os << "FATAL ERROR: " << Message <<std::endl;
  throw -1;
}
void Message::DisplayMessage(TString Message,std::ostream &os)
{
  os << Message <<std::endl;
}

void Message::DisplayMessageWithEmphasys(TString Message,std::ostream &os)
{
  os << "=> " << Message <<std::endl;
}
