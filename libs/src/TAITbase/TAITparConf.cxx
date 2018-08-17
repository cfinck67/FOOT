/*!
  \file
  \version $Id: TAITparConf.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAITparConf.
*/

#include <Riostream.h>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TList.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TSystem.h"

#include "TAITparConf.hxx"

//##############################################################################

/*!
  \class TAITparConf TAITparConf.hxx "TAITparConf.hxx"
  \brief Map and Geometry parameters for Inner Tracker **
*/

ClassImp(TAITparConf);

TString TAITparConf::fgkDefItConfName = "./config/TAITdetector.cfg";

//______________________________________________________________________________
TAITparConf::TAITparConf()
: TAVTparConf()
{
   // Standard constructor
}

//______________________________________________________________________________
TAITparConf::~TAITparConf()
{
   // Destructor
}
