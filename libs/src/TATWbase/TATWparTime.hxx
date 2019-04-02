#ifndef _TATWparTime_HXX
#define _TATWparTime_HXX
/*!
  \file
  \version $Id: TATWparTime.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
  \brief   Declaration of TATWparTime.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
#include <map>
using namespace std;

#include "TString.h"

#include "TAGpara.hxx"

//##############################################################################

class TATWparTime : public TAGpara {
  public:

  
  TATWparTime();
  virtual         ~TATWparTime();

  virtual void    Clear(Option_t* opt="");
  virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
  
  void GetTimeArray(int iBo, int iCha, int TrigCell,  vector<double> *time);
  void SetTimeCal(int iBo, int iCha, int iSa, double tbin);
  ClassDef(TATWparTime,1)

  const double sec2Nano = 1E9;
  
  private:

    map<int, vector<double>> time_parcal;
    map<int, bool> m_GotCalib;
};

#endif
