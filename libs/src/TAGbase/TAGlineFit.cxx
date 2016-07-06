/*!
  \file
  \version $Id: TAGlineFit.cxx,v 1.1 2003/06/15 18:22:01 mueller Exp $
  \brief   Implementation of TAGlineFit.
*/

#include "TString.h"

#include "TAGlineFit.hxx"

/*!
  \class TAGlineFit TAGlineFit.hxx "TAGlineFit.hxx"
  \brief Perform line fits. **
*/

ClassImp(TAGlineFit);

//------------------------------------------+-----------------------------------
//! Constructor.

TAGlineFit::TAGlineFit()
  : fdSn(0.),
    fdSx(0.), fdSxx(0.),
    fdSy(0.), fdSxy(0.)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGlineFit::~TAGlineFit()
{}

//------------------------------------------+-----------------------------------
//! Add a point to the fit

void TAGlineFit::AddPoint(Float_t f_x, Float_t f_y, Float_t f_w)
{
  fdSn  += 1.;
  fdSx  += f_x;
  fdSxx += f_x * f_x;
  fdSy  += f_y;
  fdSxy += f_x * f_y;
  return;
}

//------------------------------------------+-----------------------------------
//! Remove a point from the fit

void TAGlineFit::SubPoint(Float_t f_x, Float_t f_y, Float_t f_w)
{
  fdSn  -= 1.;
  fdSx  -= f_x;
  fdSxx -= f_x * f_x;
  fdSy  -= f_y;
  fdSxy -= f_x * f_y;
  return;
}

//------------------------------------------+-----------------------------------
//! Determine fit

Bool_t TAGlineFit::Fit(Float_t& f_p0, Float_t& f_p1,
		       Float_t& f_errp0, Float_t& f_errp1) const
{
  Double_t f_sd = fdSn * fdSxx - fdSx * fdSx;

  if (fdSn < 2. || f_sd == 0.) {
    f_p0    = 0.;
    f_p1    = 0.;
    f_errp0 = 0.;
    f_errp1 = 0.;
    return kTRUE;
  }

  f_p0 = (fdSy * fdSxx - fdSx * fdSxy) / f_sd;
  f_p1 = (fdSn * fdSxy - fdSx * fdSy)  / f_sd;

  f_errp0 = 0.;
  f_errp1 = 0.;

  return kFALSE;
}

//------------------------------------------+-----------------------------------
//! Clear fit context

void TAGlineFit::Clear(Option_t*)
{
  fdSn  = 0.;
  fdSx  = 0.;
  fdSxx = 0.;
  fdSy  = 0.;
  fdSxy = 0.;
  return;
}

//------------------------------------------+-----------------------------------
//! ostream insertion.


void TAGlineFit::ToStream(ostream& os, Option_t* option) const
{
  os << "TAGlineFit " << Form("npoint=%3d",NPoint()) << endl;
  if (NPoint() == 0) return;
  os << Form("  Sx  : %20.7g   Sxx : %20.7g",  fdSx, fdSxx) << endl;
  os << Form("  Sy  : %20.7g   Sxy : %20.7g",  fdSy, fdSxy) << endl;
  Float_t f_p0, f_p1, f_errp0, f_errp1;
  if (Fit(f_p0, f_p1, f_errp0, f_errp1))return;
  os << Form("  p0  : %20.7g   errp0:%20.7g",  f_p0, f_errp0) << endl;
  os << Form("  p1  : %20.7g   errp1:%20.7g",  f_p1, f_errp1) << endl;
  return;
}
