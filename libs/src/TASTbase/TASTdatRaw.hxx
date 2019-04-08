#ifndef _TASTdatRaw_HXX
#define _TASTdatRaw_HXX
/*!
  \file
  \version $Id: TASTdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TASTdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"
#include "TAGdata.hxx"
#include "TH1D.h"

#define WAVE_CFD_ID 999
#define WAVE_ID 998


class TASTrawHit : public TObject {

public:
  TASTrawHit();
  TASTrawHit(int ch_num , vector<double> time, vector<double> amplitude, int type);
  virtual         ~TASTrawHit();

  virtual void Clear(Option_t* /*option*/);

  
  inline int GetChannel(){return m_ch_num;}
  inline double GetCharge(){return m_charge;}
  inline double GetArrivalTime(){return m_tarr;}
  inline double GetAmplitude(){return m_max_amp;}
  inline int GetTrigType(){return m_trig_type;}
  inline vector<double> GetTimeArray(){return m_time;}
  inline vector<double> GetAmplitudeArray(){return m_amplitude;}

  inline void SetArrivalTime(double value){m_tarr = value;}
  inline void SetTrigType(int value){m_trig_type = value;}
  inline void SetAmplitude(double value){m_max_amp = value;}
  inline void SetCharge(double value){m_charge = value;}
  
    ClassDef(TASTrawHit,1)

  private:
    vector<double> m_time, m_amplitude;
    int m_ch_num, m_board_id, m_max_amp;
    double m_tarr;
    double m_charge;
    int m_trig_type;
   
  
};




//##############################################################################

class TASTdatRaw : public TAGdata {
  public:

 TASTdatRaw();
  virtual         ~TASTdatRaw();
  
  
 
  void AddWaveform(int ch_num, vector<double> time, vector<double> amplitude, int type);
  void SumWaveforms();
  inline TASTrawHit* GetWaveCFD(){return fSumWaves_cfd;}
  inline TASTrawHit* GetWaveSum(){return fSumWaves;}
  //bool IsSTChannel();


  inline void SetTriggerTime(double value){fdTrgTime = value;}
  inline void SetCharge(double value){fdCharge = value;}
  inline int GetTrigType(){return m_trig_type;}
  inline void SetTrigType(int value){m_trig_type = value;}
  
  inline double     TrigTime(){return fdTrgTime;}
  inline double     Charge(){return fdCharge;}
  inline vector<TASTrawHit*>GetHits(){return  fListOfWaveforms;}
  inline vector<TASTrawHit*>GetHitsCFD(){return  fListOfWaveforms_cfd;}  
  
  virtual void      Clear(Option_t* opt="");
  virtual void      ToStream(ostream& os=cout, Option_t* option="") const;
  
    static const Char_t* GetBranchName()   { return fgkBranchName.Data();   }
  
    ClassDef(TASTdatRaw,1)
      
  private:

     static TString fgkBranchName;    // Branch name in TTree

    vector<TASTrawHit*> fListOfWaveforms; // hits
    vector<TASTrawHit*> fListOfWaveforms_cfd; // hits
    TASTrawHit*     fSumWaves; // hits
    TASTrawHit*     fSumWaves_cfd; // hits
    double          fdTrgTime;   // SC trigger time
    double          fdCharge;   // SC total charge
    int m_trig_type;
 

  
};

#include "TASTdatRaw.icc"

#endif
