

#include "TATW_Hit.hxx"


// layer 0 -->  column, asse X
// layer 1 -->  row, asse Y


ClassImp(TATW_Hit) // Description of Single Detector TATW_Hit 



//______________________________________________________________________________
//  build a hit from a rawHit
TATW_Hit::TATW_Hit( TATWrawHit* hit )
   : TAGobject(),
    m_layer(0),
    m_bar(0),
    m_de(0),
    m_time(0),
    m_coordinate(0),
    m_z(0)
{
}

//______________________________________________________________________________
//
TATW_Hit::TATW_Hit(const TATW_Hit& aHit)
:  TAGobject(aHit),
   m_layer(aHit.m_layer),
   m_bar(aHit.m_bar),
   m_de(aHit.m_de),
   m_time(aHit.m_time),
   m_coordinate(aHit.m_coordinate),
   m_z(aHit.m_z)
{
   
}

//______________________________________________________________________________
// Build the hit from its sensor, line and column// constructor of a Pixel with column and line 
TATW_Hit::TATW_Hit ( int aView, int aBar, Double_t aDe, Double_t aTime, Double_t pos)
: TAGobject(),
  m_layer(aView),
  m_bar(aBar),
  m_de(aDe),
  m_time(aTime),
  m_coordinate(pos),
  m_z(0)
{
}

//______________________________________________________________________________
//
void TATW_Hit::AddMcTrackId(Int_t trackId, Int_t mcId)
{
   m_MCindex.Set(m_MCindex.GetSize()+1);
   m_MCindex[m_MCindex.GetSize()-1]   = mcId;
   
   m_McTrackId.Set(m_McTrackId.GetSize()+1);
   m_McTrackId[m_McTrackId.GetSize()-1] = trackId;
}

//______________________________________________________________________________
//
void TATW_Hit::Clear(Option_t* /*option*/)
{
   m_MCindex.Set(0);
   m_McTrackId.Set(0);
}
