#ifndef _TATRdatRaw_HXX
#define _TATRdatRaw_HXX
/*!
  \file
  \version $Id: TATRdatRaw.hxx,v 1.0 2011/04/01 18:11:59 asarti Exp $
  \brief   Declaration of TATRdatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TObject.h"
#include "TClonesArray.h"

#include "TAGdata.hxx"


class TATRrawHit : public TObject {
public:
  
  TATRrawHit();

  TATRrawHit(int sta, int pat, int cnt);

  virtual         ~TATRrawHit();

  void            SetData(int sta, int pat, int cnt);

  double          TrigTime(int ch) const;
  double          TrigMult(int ch) const;
  Int_t           Count() const;
  Int_t           Pattern() const;
  Int_t           Status() const;
  Int_t           VulomTime() const; 
  Int_t           ScaCount(int ch) const;
  
  void           SetTrigBefLMU(int id, int val);
  int            GetTrigBefLMU(int id);
  
  void           SetTrigBefDT(int id, int val); //
  int            GetTrigBefDT(int id);    //
  void           SetTrigAftDT(int id, int val);
  int            GetTrigAftDT(int id);    
  void           SetTrigAftRed(int id, int val);
  int            GetTrigAftRed(int id);    

  void           SetTrigTime(int ch, double time);
  void           SetTrigMult(int ch, int mult);
  void           SetCount(int count);
  void           SetPattern(int patt);
  void           SetStatus(int stat);
  void           SetScaCount(int ch, int count);

  void           SetVulomTime(int tim);


  ClassDef(TATRrawHit,1)

  private:

  Int_t vu_stat;    
  Int_t trg_pat;    
  Int_t trg_count;
  Int_t trg_vul_time;
  vector<double> trg_tim;
  vector<int>    trg_mul;
  Int_t trg_bf_lmu[16];
  Int_t trg_bf_dti[16];
  Int_t trg_af_dti[16];
  Int_t trg_af_red[16];
  Int_t sca_cnt[16];

};

//##############################################################################

class TATRdatRaw : public TAGdata {
  public:

                    TATRdatRaw();
    virtual         ~TATRdatRaw();

    TATRrawHit*       Hit(Int_t i_ind);
    const TATRrawHit* Hit(Int_t i_ind) const;

    virtual void    Clear(Option_t* opt="");

    void SetupClones();

    ClassDef(TATRdatRaw,1)

  public:
    Int_t           ntrhit;		    // 
    TClonesArray*   htr;			    // hits

};

#include "TATRdatRaw.icc"

#endif
