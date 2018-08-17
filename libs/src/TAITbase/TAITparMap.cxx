/*!
 \file
 \version $Id: TAITparMap.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
 \brief   Implementation of TAITparMap.
 */

#include "TAITparMap.hxx"

//##############################################################################

/*!
 \class TAITparMap TAITparMap.hxx "TAITparMap.hxx"
 \brief Map and Geometry parameters for Tof wall. **
 */

ClassImp(TAITparMap);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAITparMap::TAITparMap() 
: TAVTparMap()
{
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAITparMap::~TAITparMap()
{}

/*------------------------------------------+---------------------------------*/
//! Returns real id sensor respect to MC
Int_t TAITparMap::GetRealId(Int_t mcSensorId)
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
