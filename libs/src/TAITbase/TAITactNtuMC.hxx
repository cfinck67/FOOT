#ifndef _TAITactNtuMC_HXX
#define _TAITactNtuMC_HXX
/*!
 \file
 \version $Id: TAITactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAITactNtuMC.
 */
/*------------------------------------------+---------------------------------*/

#include "Evento.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

// #include "TAVTbaseDigitizer.hxx"

#include "TAITntuHit.hxx"


class TH2F;
class TH1F;
class TAITactNtuMC : public TAGaction {
   
// class TAVTbaseDigitizerg;
// class TAVTntuHit;

   // struct RawMcHit_t : public  TObject {
   //    RawMcHit_t() {id = 0; de = x = y = 0.;}
   //    Int_t  id;  // sensor id
   //    Float_t de; // enerhy loss
   //    Float_t x;  // hit in X    
   //    Float_t y;  // hit in Y
   //    Float_t zi;  // hit in Zin
   //    Float_t zo;  // hit in Zout
   // };
   

public:
   explicit TAITactNtuMC(const char* name=0, TAGdataDsc* p_nturaw=0, TAGparaDsc* p_geomap = 0, TAGparaDsc* pParMap =0,EVENT_STRUCT* evtStr=0);
   virtual ~TAITactNtuMC() {};
   
   //! Base action 
   virtual bool  Action();

   //! Base creation of histogram
   virtual  void   CreateHistogram();



   // Fill noise over sensors
   void           FillNoise();
   
   //! Set refit flag
   static void   SetPileup(Bool_t flag)      { fgPileup = flag;         }
   
   //! Get refit flag
   static Bool_t GetPileup()                 { return fgPileup;         }
   
   //! Set number of pileup evt
   static void   SetPileupEventsN(Bool_t n)  { fgPileupEventsN = n;     }
   
   //! Get number of pileup evt
   static Int_t  GetPileupEventsN()          { return fgPileupEventsN;  }
   
   //! Set Poisson parameter
   static void   SetPoissonPar(Float_t par)  { fgPoissonPar = par;      }
   
   //! Get Poisson parameter
   static Float_t GetPoissonPar()            { return fgPoissonPar;     }


   static Float_t GetSigmaNoiseLevel()              { return fgSigmaNoiseLevel;  }
   static void    SetSigmaNoiseLevel(Float_t level) { fgSigmaNoiseLevel = level; }
   
   static Int_t   GetMcNoiseId()                    { return fgMcNoiseId;        }
   static void    SetMcNoiseId(Int_t id)            { fgMcNoiseId = id;          }
   

   
private:
   TAGdataDsc*     fpNtuRaw;		    // output data dsc
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc
   TAGparaDsc*     fpParMap;		    // map para dsc
   EVENT_STRUCT*   fpEvtStr;
   
   // TAITbaseDigitizer*  fDigitizer;       // cluster size digitizer
   // Int_t           fNoisyPixelsN;

   Int_t           fDebugLevel;         // debug level
   
   TH2F*           fpHisPixelMap[8];  // pixel map per sensor
   TH2F*           fpHisPosMap[8];    // pixel map per sensor   
   TH1F*           fpHisRateMap[8];   // rates per sensor
   TH1F*           fpHisRateMapQ[8];  // rates per quadrant

   // std::vector<std::vector<RawMcHit_t>> fStoredEvents;



   void            SetMCinfo(TAITntuHit* pixel, Int_t hitId);
   void            GeneratePileup();
   void            FillPixels( Int_t sensorId, Int_t mcId );  // for pileup
   void            FillPixels( TAITntuHit* originatingHit, Int_t sensorId, Int_t mcId );  // for mc_cluster
   void            ComputeNoiseLevel();
   void            FillNoise(Int_t sensorId);


   static Bool_t   fgPileup;           // flag to generated pileup events
   static Int_t    fgPileupEventsN;    // number of pileup events to be stored
   static Float_t  fgPoissonPar;       // Poisson parameter for pileup simulation
   static Float_t  fgSigmaNoiseLevel;
   static Int_t    fgMcNoiseId;
    
   
   ClassDef(TAITactNtuMC,0)
};

#endif



































