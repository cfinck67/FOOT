#ifndef _TADCactNtuRaw_HXX
#define _TADCactNtuRaw_HXX
/*!
  \file
  \version $Id: adapted from TABMactNtuRaw.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
  \brief   Declaration of TADCactNtuRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

class TADCactNtuRaw : public TAGaction {
  public:
    explicit        TADCactNtuRaw(const char* name=0,
				 TAGdataDsc* p_nturaw=0, 
				 TAGdataDsc* p_datraw=0, 
				 TAGdataDsc* p_timraw=0, 
				 TAGdataDsc* p_triraw=0, 
				 TAGparaDsc* p_geomap=0, 
				 TAGparaDsc* p_parcon=0);
    virtual         ~TADCactNtuRaw();
    void     cleanup();
    
    vector<double> retrieve_V();
    vector<double> retrieve_U();
    void CalcEffi(double nume, double deno, double &eff, double &erro);
    virtual Bool_t  Action();

    ClassDef(TADCactNtuRaw,0)

  private:
    TAGdataDsc*     fpNtuRaw;		    // output data dsc
    TAGdataDsc*     fpDatRaw;		    // input data dsc
    TAGdataDsc*     fpTimRaw;		    // input time dsc
    TAGdataDsc*     fpTriRaw;		    // input time dsc
    TAGparaDsc*     fpGeoMap;		    // geometry para dsc
    TAGparaDsc*     fpParCon;		    // DC config params.

    double m_fittedVO;
    double m_fittedVE;
    double m_fittedUO, m_fittedUE;
    double m_missedX[6], m_missedY[6];
     vector<double> eff_V, eff_U;
};

#endif
