


#include "TATW_Point.hxx"


ClassImp(TATW_Point) // Description of Single Detector TATW_Point 


//______________________________________________________________________________
//  default constructor
TATW_Point::TATW_Point()
: TAGobject(),
   m_position(),
   m_column(0),
   m_row(0),
   m_columnHit(0x0),
   m_rowHit(0x0),
   m_de(0.),
   m_time(0.)
{
}

//______________________________________________________________________________
//  build a point
TATW_Point::TATW_Point( double x, TATW_Hit* colHit, double y, TATW_Hit* rowHit )
 : TAGobject(),
   m_position(x, y, 0),
   m_columnHit(new TATW_Hit(*colHit)),
   m_rowHit(new TATW_Hit(*rowHit))
{
   m_column = m_columnHit->GetBar();
   m_row    = m_rowHit->GetBar();
   
   m_de     = m_columnHit->GetEnergyLoss() + m_rowHit->GetEnergyLoss();
   m_time   = m_columnHit->GetTime();
}

//______________________________________________________________________________
// Clear 
void TATW_Point::Clear(Option_t*)
{
   delete m_columnHit;
   delete m_rowHit;
}
