#ifndef _TASTparTime_HXX
#define _TASTparTime_HXX
/*!
  \file
  \version $Id: TASTparTime.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
  \brief   Declaration of TASTparTime.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
#include <map>
using namespace std;

#include "TString.h"
#include "TAGpara.hxx"
#include "TASTparMap.hxx"


#define NMAX_BO_ID 100
#define NMAX_CH_ID 18

//##############################################################################

class TASTparTime : public TAGpara {
  public:

  
  TASTparTime();
  virtual         ~TASTparTime();

  virtual void    Clear(Option_t* opt="");
  virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
  
  bool GetTimeArray(int iBo, int iCha, int TrigCell,  vector<double> *time);
  void SetTimeCal(int iBo, int iCha, vector<float> tvec);
  ClassDef(TASTparTime,1)

  const double sec2Nano = 1E9;
  
  private:

    map<int, vector<double>> time_parcal;
    map<int, bool> m_GotCalib;
   
};

#endif
