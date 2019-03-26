/*!
  \file
  \version $Id: TAMSDparConf.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAMSDparConf.
*/


#include "TAMSDparConf.hxx"

//##############################################################################

/*!
  \class TAMSDparConf TAMSDparConf.hxx "TAMSDparConf.hxx"
  \brief Map and Geometry parameters for Inner Tracker **
*/

ClassImp(TAMSDparConf);


//______________________________________________________________________________
TAMSDparConf::TAMSDparConf()
: TAVTbaseParConf()
{
   // Standard constructor
   fkDefaultConfName = "./config/TAMSDdetector.cfg";
}

//______________________________________________________________________________
TAMSDparConf::~TAMSDparConf()
{
   // Destructor
}
