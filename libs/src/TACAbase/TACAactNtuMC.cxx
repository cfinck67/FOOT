/*!
  \file
  \version $Id: TACAactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TACAactNtuMC.
*/

#include "TACAdatRaw.hxx"
#include "TACAactNtuMC.hxx"

/*!
  \class TACAactNtuMC TACAactNtuMC.hxx "TACAactNtuMC.hxx"
  \brief NTuplizer for Calo raw hits. **
*/

ClassImp(TACAactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TACAactNtuMC::TACAactNtuMC(const char* name,
			 TAGdataDsc* p_datraw, 
			 EVENT_STRUCT* evStr)
  : TAGaction(name, "TACAactNtuMC - NTuplize ToF raw data"),
    m_hitContainer(p_datraw),
    m_eventStruct(evStr)
    // fpNtuMC(p_datraw),
    // fpEvtStr(evStr)
{
  Info("Action()"," Creating the Calorimeter MC tuplizer action\n");
  AddDataOut(p_datraw, "TACAdatRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TACAactNtuMC::~TACAactNtuMC()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TACAactNtuMC::Action()
{

  if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "TACAactNtuMC::Action() start" << endl;

    // TACA_ContainerHit* containerHit = (TACA_ContainerHit*) m_hitContainer->Object();
    TACA_ContainerHit* containerHit = (TACA_ContainerHit*) gTAGroot->FindDataDsc("containerHit", "TACA_ContainerHit")->Object(); 
    TACAparGeo* geoMap = (TACAparGeo*) gTAGroot->FindParaDsc("caGeo", "TACAparGeo")->Object();     // *** Break *** segmentation violation

    // int nhits(0);
    // if (!containerHit->m_listOfHits) containerHit->SetupClones();

    //The number of hits inside the Calorimeter is CALn
    if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "Processing n BGO: " << m_eventStruct -> CALn << endl;

    // fill the container of hits
    for (int i = 0; i < m_eventStruct -> CALn; i++) { 

        // int BGOsensorId  = geoMap->GetSensorID( m_eventStruct->CALxin[i], m_eventStruct->CALyin[i] );
        
        TACA_Hit* hit = containerHit -> NewHit( m_eventStruct->CALicry[i], m_eventStruct->CALde[i],   
                                                m_eventStruct->CALtim[i] );

        cout << "So far, so good" << endl;
        
        TVector3 MCpos = TVector3(  (m_eventStruct->CALxin[i]  + m_eventStruct->CALxout[i])/2,  
                                    (m_eventStruct->CALyin[i]  + m_eventStruct->CALyout[i])/2,  
                                    (m_eventStruct->CALzin[i]  + m_eventStruct->CALzout[i])/2 );
        TVector3 MCmom = TVector3(  (m_eventStruct->CALpxin[i] + m_eventStruct->CALpxout[i])/2, 
                                    (m_eventStruct->CALpyin[i] + m_eventStruct->CALpyout[i])/2, 
                                    (m_eventStruct->CALpzin[i] + m_eventStruct->CALpzout[i])/2 );    
        
        geoMap->Global2Local( &MCpos );
        geoMap->Global2Local_RotationOnly( &MCmom );

        hit->SetMCPosition( MCpos );
        hit->SetMCMomentum( MCmom );

    }
    
    return true;
}



// Vecchia versione

/*
  TACAdatRaw* p_nturaw = (TACAdatRaw*) fpNtuMC->Object();

  int BGOsensorId  = pGeoMap->GetSensorID( fpEvtStr->CALxin[i], fpEvtStr->CALyin[i] );

  Int_t nhits(0);
  if (!p_nturaw->hir) p_nturaw->SetupClones();

  //The number of hits inside the Calo is CALn
  Info("Action()","Processing n Calo :: %2d hits \n",fpEvtStr->CALn);
  for (Int_t i = 0; i < fpEvtStr->CALn; i++) {                        // loop su tutti gli hit di ogni evento del Calo (CALn)
    //First two numbers make sense only for data (typ, channel)
    new((*(p_nturaw->hir))[i]) TACArawHit(0,0,fpEvtStr->CALde[i],fpEvtStr->CALtim[i]);    //devo cambiare TACArawHit con TACA_Hit  
    nhits++;
  }
  
  double meantime(0), firsttime(-10000);
  Int_t i_nhit = p_nturaw->nirhit;
  for (Int_t i = 0; i < i_nhit; i++) {
    const TACArawHit* aHi = p_nturaw->Hit(i);
    if(aHi->Type() == 0) {
      if(aHi->ChID() == 0) firsttime = aHi->Time();
      meantime += aHi->Time();
    }
  }
  if(i_nhit) meantime /= i_nhit;
  
  //Set up of the Trigger Time
  p_nturaw->SetTrigTime(firsttime);

  p_nturaw->nirhit  = nhits;

  fpNtuMC->SetBit(kValid);
  return kTRUE;
}

*/





