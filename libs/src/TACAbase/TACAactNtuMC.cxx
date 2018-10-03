/*!
  \file
  \version $Id: TACAactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TACAactNtuMC.
*/
#include "TAGgeoTrafo.hxx"
#include "TACAntuRaw.hxx"
#include "TACAactNtuMC.hxx"
#include "TACAdigitizer.hxx"

/*!
  \class TACAactNtuMC TACAactNtuMC.hxx "TACAactNtuMC.hxx"
  \brief NTuplizer for Calo raw hits. **
*/

ClassImp(TACAactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TACAactNtuMC::TACAactNtuMC(const char* name, TAGdataDsc* p_datraw, TAGparaDsc* pGeoMap, EVENT_STRUCT* evStr)
  : TAGaction(name, "TACAactNtuMC - NTuplize ToF raw data"),
    fpGeoMap(pGeoMap),
    fpNtuMC(p_datraw),
    fpEvtStr(evStr)
{
  AddDataOut(p_datraw, "TACAntuRaw");
   
   CreateDigitizer();
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TACAactNtuMC::CreateHistogram()
{
   
   DeleteHistogram();
   
   fpHisHitMap = new TH1F("caHitMap", "Calorimeter - hits", 22*22, 0., 22*22);
   AddHistogram(fpHisHitMap);
   
   fpHisDeTot = new TH1F("caDeTot", "Calorimeter - Total energy loss", 2000, 0., 1000);
   AddHistogram(fpHisDeTot);
   
   fpHisTimeTot = new TH1F("caTimeTot", "Calorimeter - Total time", 1000, 0., 20);
   AddHistogram(fpHisTimeTot);
   
   fpHisDeTotMc = new TH1F("caMcDeTot", "Calorimeter - MC Total energy loss", 4000, 0., 400);
   AddHistogram(fpHisDeTotMc);
   
   fpHisTimeTotMc = new TH1F("caMcTimeTot", "Calorimeter - MC Total time", 1000, 0., 20);
   AddHistogram(fpHisTimeTotMc);


   SetValidHistogram(kTRUE);
}

//------------------------------------------+-----------------------------------
//! Create digitizer
void TACAactNtuMC::CreateDigitizer()
{
   TACAntuRaw* pNtuRaw = (TACAntuRaw*) fpNtuMC->Object();

   fDigitizer = new TACAdigitizer(pNtuRaw);
}

//------------------------------------------+-----------------------------------
//! Destructor.

TACAactNtuMC::~TACAactNtuMC()
{
   delete fDigitizer;
}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TACAactNtuMC::Action()
{
   for (Int_t i = 0; i < fpEvtStr->CALn; i++) {
      
      Int_t id      = fpEvtStr->CALicry[i];
      Int_t trackId = fpEvtStr->VTXid[i] - 1;
      Float_t x0    = fpEvtStr->CALxin[i]*TAGgeoTrafo::CmToMm();
      Float_t y0    = fpEvtStr->CALyin[i]*TAGgeoTrafo::CmToMm();
      Float_t z0    = fpEvtStr->CALzin[i]*TAGgeoTrafo::CmToMm();
      Float_t z1    = fpEvtStr->CALzout[i]*TAGgeoTrafo::CmToMm();
      Float_t edep  = fpEvtStr->CALde[i]*TAGgeoTrafo::GevToMev();
      Float_t time  = fpEvtStr->CALtim[i]*TAGgeoTrafo::SecToNs();
      
      //  veci = fpParGeo->Global2Local(sensorId, veci); // inverse x-y
      fDigitizer->Process(edep, x0, y0, z0, z1, time, id);
      TACAntuHit* hit = fDigitizer->GetCurrentHit();
      hit->AddMcTrackId(trackId);

      if (ValidHistogram()) {
         fpHisHitMap->Fill(hit->GetChannelId());
         fpHisDeTot->Fill(hit->GetCharge());
         fpHisTimeTot->Fill(hit->GetTime());
         
         fpHisDeTotMc->Fill(edep);
         fpHisTimeTotMc->Fill(time);
      }

   }
 
   fpNtuMC->SetBit(kValid);
   
  return kTRUE;
}

