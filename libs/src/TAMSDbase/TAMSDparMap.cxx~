/*!
 \file
 \version $Id: TAVTparMap.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
 \brief   Implementation of TAVTparMap.
 */

#include "TAVTparMap.hxx"

//##############################################################################

/*!
 \class TAVTparMap TAVTparMap.hxx "TAVTparMap.hxx"
 \brief Map and Geometry parameters for Tof wall. **
 */

ClassImp(TAVTparMap);

const Int_t TAVTparMap::fgkSensorsN = 8;
//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTparMap::TAVTparMap() 
: TAGpara(),
  fPixelsNu(1152),
  fPixelsNv(576),
  fPitchU(18.4),
  fPitchV(18.4),
  fPixelSize(18.4)
{  
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAVTparMap::~TAVTparMap()
{}

/*------------------------------------------+---------------------------------*/
//! Returns real id sensor respect to MC
Int_t TAVTparMap::GetRealId(Int_t mcSensorId)
{
   switch (mcSensorId) {
	  case 0:
		 return 4;
		 break;  
	  case 1:
		 return 0;
		 break;
	  case 2:
		 return 5;
		 break;
	  case 3:
		 return 1;
		 break;
	  case 4:
		 return 6;
		 break;
	  case 5:
		 return 2;
		 break;
	  case 6:
		 return 7;
		 break;
	  case 7:
		 return 3;
		 break;
	  default:
		 return -1;
   }
}

/*------------------------------------------+---------------------------------*/
//! Returns position from pixel line/column in U direction
Int_t TAVTparMap::GetIndex(Int_t line, Int_t column) const
{
   return line*fPixelsNu + column;
}

/*------------------------------------------+---------------------------------*/
//! Returns position from pixel line/column in U direction
Float_t TAVTparMap::GetPositionU(Int_t column) const
{
   return ((2*column - fPixelsNu + 1 ) * fPitchU)/2 ;
}

/*------------------------------------------+---------------------------------*/
//! Returns position from pixel line/column in V direction
Float_t TAVTparMap::GetPositionV(Int_t line) const
{
   return -((2*line - fPixelsNv + 1 ) * fPitchV)/2 ;

}
