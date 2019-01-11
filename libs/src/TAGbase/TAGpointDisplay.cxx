
#include "TAGpointDisplay.hxx"

//
ClassImp(TAGpointDisplay)

//__________________________________________________________
TAGpointDisplay::TAGpointDisplay(const Text_t* name) 
  : TEvePointSet(name)
{
  // default constructor
}

//__________________________________________________________
TAGpointDisplay::~TAGpointDisplay()
{
  // default destructor
}

//__________________________________________________________
void TAGpointDisplay::AddPoint(Float_t x, Float_t y, Float_t z)
{
   SetPoint(Size(), x, y, z);
}

//__________________________________________________________
void TAGpointDisplay::ResetPoints()
{
   Reset();
}
