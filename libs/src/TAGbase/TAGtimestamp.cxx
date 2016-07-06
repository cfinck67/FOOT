/*!
  \file
  \version $Id: TAGtimestamp.cxx,v 1.2 2003/05/25 12:33:11 mueller Exp $
  \brief   Implementation of TAGtimestamp.
*/

#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#include "TBuffer.h"

#include "TAGtimestamp.hxx"

/*!
  \class TAGtimestamp TAGtimestamp.hxx "TAGtimestamp.hxx"
  \brief Time representation. **
*/

ClassImp(TAGtimestamp);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGtimestamp::TAGtimestamp()
  : fiSec(-1),
    fiUSec(0)
{}

//------------------------------------------+-----------------------------------
//! Construct from Unix time \a i_time .

TAGtimestamp::TAGtimestamp(Int_t i_time)
  : fiSec(i_time),
    fiUSec(0)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGtimestamp::~TAGtimestamp()
{}

//------------------------------------------+-----------------------------------
//! Convert a \c double to a timestamp.

void TAGtimestamp::FromDouble(Double_t d_time)
{
  Double_t d_sec   = floor(d_time);
  Double_t d_usec  = floor(1.e6*(d_time - d_sec));
  if (d_usec == 1.e6) {
    d_sec  += 1.;
    d_usec  = 0.;
  }
  fiSec  = (Int_t) d_sec;
  fiUSec = (Int_t) d_usec;
  return;
}

//------------------------------------------+-----------------------------------
//! Set to current system time.

void TAGtimestamp::SetCurrent()
{
  struct timeval  tv;
  struct timezone tz;
  
  gettimeofday(&tv, &tz);
  fiSec  = (Int_t) tv.tv_sec;
  fiUSec = (Int_t) tv.tv_usec;
  return;
}

//------------------------------------------+-----------------------------------
//! Returns time difference between current time and timestamp

Double_t TAGtimestamp::Age() const
{
  if (!Valid()) return 0.;
  TAGtimestamp current;
  current.SetCurrent();
  return current - *this;
}

//------------------------------------------+-----------------------------------
//! Add \a d_deltatime seconds to the timestamp.

TAGtimestamp& TAGtimestamp::operator+(Double_t d_deltatime)
{
  FromDouble(ToDouble() + d_deltatime);
  return *this;
}

//------------------------------------------+-----------------------------------
//! Subtract \a d_deltatime seconds from the timestamp.

TAGtimestamp& TAGtimestamp::operator-(Double_t d_deltatime)
{
  FromDouble(ToDouble() - d_deltatime);
  return *this;
}

//------------------------------------------+-----------------------------------
//! Custom streamer.

void TAGtimestamp::Streamer(TBuffer& R__b)
{
  UInt_t R__s, R__c;
  
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
    if (R__v) { }
    R__b >> fiSec;
    R__b >> fiUSec;

  } else {
    R__c = R__b.WriteVersion(TAGtimestamp::IsA(), kFALSE);
    R__b << fiSec;
    R__b << fiUSec;
  }

  return;
}

//------------------------------------------+-----------------------------------
/*!
  \relates TAGtimestamp
  Returns the time elapsed from timestamp \rhs to timestamp \lhs in seconds.
*/

Double_t operator-(const TAGtimestamp& lhs, const TAGtimestamp& rhs)
{
  return lhs.ToDouble() - rhs.ToDouble();
}
