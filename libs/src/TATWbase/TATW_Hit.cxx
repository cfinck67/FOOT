

#include "TATW_Hit.hxx"


// layer 0 -->  column, asse X
// layer 1 -->  row, asse Y


ClassImp(TATW_Hit) // Description of Single Detector TATW_Hit 



//______________________________________________________________________________
//  build a hit from a rawHit
TATW_Hit::TATW_Hit( TATWrawHit* hit )
   : TObject(),
    m_layer(0),
    m_bar(0),
    m_de(0),
    m_time(0),
    m_coordinate(0),
    m_z(0)
{
   m_McTrackId[0] = -1;
}

//______________________________________________________________________________
// Build the hit from its sensor, line and column// constructor of a Pixel with column and line 
TATW_Hit::TATW_Hit ( int aView, int aBar, Double_t aDe, Double_t aTime)
: TObject(),
  m_layer(aView),
  m_bar(aBar),
  m_de(aDe),
  m_time(aTime)
{
   m_McTrackId[0] = -1;
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