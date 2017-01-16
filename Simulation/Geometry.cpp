#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <sstream>

#include "Geometry.h"

using namespace std;


ClassImp(Geometry);


/*-----------------------------------------------------------------*/

Geometry::Geometry() {
  
	fRegionFlag = 0;
	fRegionName.clear();
	fRegionNumber.clear();

	Geometry::InitBmon();
	Geometry::InitDch();

}


/*-----------------------------------------------------------------*/

Int_t Geometry::InitBmon() {

  //sposto di +/-0.00001 perchè le celle non tocchino i fili, se no mi dà problemi
  Double_t shift = 0.00001;
  
  Double_t aa[BMN_NWIRELAY], bb[BMN_NWIRELAY];
  
  geo.Bmon.idsense[0] = 8;
  geo.Bmon.idsense[1] = 10;
  geo.Bmon.idsense[2] = 12;

  for (int nn = 0; nn < 7; nn++) {
    
    bb[nn] = nn * BMN_PASSO;
    bb[nn + 7] = nn * BMN_PASSO;
    bb[nn + 14] = nn * BMN_PASSO;
    
    aa[nn] = 0.;
    aa[nn + 7] = BMN_STEP;
    aa[nn + 14] = 2 * BMN_STEP;
    
  }
  
  geo.Bmon.nlayer = BMN_NLAY;
  geo.Bmon.nwirelayer = BMN_NWIRELAY;
  geo.Bmon.nsense = BMN_NSENSELAY;
  
  geo.Bmon.CenterBmn[0] = BMN_X;
  geo.Bmon.CenterBmn[1] = BMN_Y;
  geo.Bmon.CenterBmn[2] = BMN_Z;
  
  geo.Bmon.DeltaBmn[0] = BMN_DELTAX;
  geo.Bmon.DeltaBmn[1] = BMN_DELTAY;
  geo.Bmon.DeltaBmn[2] = BMN_DELTAZ;
  
  geo.Bmon.SideBmn[0] = BMN_WIDTH;
  geo.Bmon.SideBmn[1] = BMN_HEIGHT;
  geo.Bmon.SideBmn[2] = BMN_LENGTH;
  
  /* generazione parametri volumi di FLUKA :  ogni filo e' definito
     da un cilindro: 2 basi nell'alluminio e direzione
     parallela all'asse x ( vista U) e y (vista V) definito da:
     
     1) vettore posizione del punto centrale della base
     2) vettore che da una base arriva all'altra base
     3) raggio
  */

  for (int il = 0; il < geo.Bmon.nlayer; il++) {
    
    //  U cells -> along x
    
    for (int ic = 0; ic < geo.Bmon.nsense; ic++){

      geo.Bmon.Uc_xm[ic][il] = geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0]/2 + shift;
      geo.Bmon.Uc_xM[ic][il] = geo.Bmon.CenterBmn[0] + geo.Bmon.SideBmn[0]/2 - shift;
      geo.Bmon.Uc_zm[ic][il] = geo.Bmon.CenterBmn[2] - geo.Bmon.SideBmn[2]/2
	+ geo.Bmon.DeltaBmn[2] + BMN_RFIELD + 4 * il * BMN_STEP + 2 * il
	* BMN_LAYDIST + shift;
      geo.Bmon.Uc_zM[ic][il] = geo.Bmon.CenterBmn[2] - geo.Bmon.SideBmn[2]/2
	+ geo.Bmon.DeltaBmn[2] - BMN_RFIELD + 4 * (il+1./2.) * BMN_STEP + 2 * il
	* BMN_LAYDIST - shift;

      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Bmon.Uc_ym[ic][il] =  geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1] / 2
	  + geo.Bmon.DeltaBmn[1] + (2.*ic+1) * BMN_PASSO + BMN_RFIELD + shift;
	geo.Bmon.Uc_yM[ic][il] =  geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1] / 2
	  + geo.Bmon.DeltaBmn[1] + (2.*ic+3) * BMN_PASSO - BMN_RFIELD - shift;
      } else {
	geo.Bmon.Uc_ym[ic][il] =  geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1] / 2
	  + geo.Bmon.DeltaBmn[1] + 2.*ic * BMN_PASSO + BMN_RFIELD + shift;
	geo.Bmon.Uc_yM[ic][il] =  geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1] / 2
	  + geo.Bmon.DeltaBmn[1] + (2.*ic+2) * BMN_PASSO - BMN_RFIELD - shift;
      }

      // V cells -> along y

      geo.Bmon.Vc_ym[ic][il] = geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1]/2 + shift;
      geo.Bmon.Vc_yM[ic][il] = geo.Bmon.CenterBmn[1] + geo.Bmon.SideBmn[1]/2 - shift;
      geo.Bmon.Vc_zm[ic][il] = geo.Bmon.CenterBmn[2] - geo.Bmon.SideBmn[2]/2
	+ geo.Bmon.DeltaBmn[2] + BMN_RFIELD + (4.*il+2) * BMN_STEP +
	(2.*il+1) * BMN_LAYDIST + shift;
      geo.Bmon.Vc_zM[ic][il] = geo.Bmon.CenterBmn[2] - geo.Bmon.SideBmn[2]/2
	+ geo.Bmon.DeltaBmn[2] - BMN_RFIELD + (4.*il+4)* BMN_STEP
	+ (2.*il+1) * BMN_LAYDIST - shift;

      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Bmon.Vc_xm[ic][il] =  geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0] / 2
	  + geo.Bmon.DeltaBmn[0] + 2.*ic * BMN_PASSO + BMN_RFIELD + shift;
	geo.Bmon.Vc_xM[ic][il] =  geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0] / 2
	  + geo.Bmon.DeltaBmn[0] + (2.*ic+2) * BMN_PASSO - BMN_RFIELD - shift;
      } else {
	geo.Bmon.Vc_xm[ic][il] =  geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0] / 2
	  + geo.Bmon.DeltaBmn[0] + (2.*ic+1.) * BMN_PASSO + BMN_RFIELD + shift;
	geo.Bmon.Vc_xM[ic][il] =  geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0] / 2
	  + geo.Bmon.DeltaBmn[0] + (2.*ic+3.) * BMN_PASSO - BMN_RFIELD - shift;
      }
      
    }
    
    for (int iw = 0; iw < geo.Bmon.nwirelayer; iw++) {
      
      //  U wires -> along x 

      geo.Bmon.U_x[iw][il] = geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0] / 2;
      geo.Bmon.U_z[iw][il] = geo.Bmon.CenterBmn[2] - geo.Bmon.SideBmn[2]/ 2
	+ geo.Bmon.DeltaBmn[2] 
	+aa[iw] + (2 * il) * (2 * BMN_STEP + BMN_LAYDIST);
      
      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Bmon.U_y[iw][il] =  geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1] / 2 
	  + geo.Bmon.DeltaBmn[1] + bb[iw] + BMN_PASSO;
      } else {
	geo.Bmon.U_y[iw][il] =  geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1] / 2 
	  + geo.Bmon.DeltaBmn[1] + bb[iw];
      }
      geo.Bmon.U_cx[iw][il] = geo.Bmon.SideBmn[0];
      geo.Bmon.U_cy[iw][il] = 0.;
      geo.Bmon.U_cz[iw][il] = 0.;

      // V wires -> along y
      
      geo.Bmon.V_y[iw][il] = geo.Bmon.CenterBmn[1] - geo.Bmon.SideBmn[1] / 2;
      geo.Bmon.V_z[iw][il] = geo.Bmon.CenterBmn[2] - geo.Bmon.SideBmn[2] / 2
	+ geo.Bmon.DeltaBmn[2] + aa[iw] + (2*il+1) * (2 * BMN_STEP + BMN_LAYDIST);
      
      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Bmon.V_x[iw][il] = geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0] / 2
	  + geo.Bmon.DeltaBmn[0] + bb[iw];
      } else {
	geo.Bmon.V_x[iw][il] = geo.Bmon.CenterBmn[0] - geo.Bmon.SideBmn[0] / 2
	  + geo.Bmon.DeltaBmn[0] + bb[iw] + BMN_PASSO;
      }
      
      geo.Bmon.V_cx[iw][il] = 0.;
      geo.Bmon.V_cy[iw][il] = geo.Bmon.SideBmn[1];
      geo.Bmon.V_cz[iw][il] = 0.;
      
      // filo di sense 
      if ((iw == geo.Bmon.idsense[0]) || (iw == geo.Bmon.idsense[1])
	  || (iw == geo.Bmon.idsense[2])) {
	geo.Bmon.U_rad[iw][il] = BMN_RSENSE;
	geo.Bmon.V_rad[iw][il] = BMN_RSENSE;
      }
      
      // filo di campo 
      else {
	geo.Bmon.U_rad[iw][il] = BMN_RFIELD;
	geo.Bmon.V_rad[iw][il] = BMN_RFIELD;
      }
    }
  }
  
  return 0;
  
}


/*-----------------------------------------------------------------*/

Int_t Geometry::InitDch() {

  //sposto di +/-0.00001 perchè le celle non tocchino i fili, se no mi dà problemi
  Double_t shift = 0.00001;
  
  Double_t aa[DCH_NWIRELAY], bb[DCH_NWIRELAY];

  geo.Dch.idsense[0] = 14;
  geo.Dch.idsense[1] = 16;
  geo.Dch.idsense[2] = 18;
  geo.Dch.idsense[3] = 20;
  geo.Dch.idsense[4] = 22;
  geo.Dch.idsense[5] = 24;

  for (int nn = 0; nn < 13; nn++) {
    bb[nn] = nn * DCH_PASSO;
    bb[nn + 13] = nn * DCH_PASSO;
    bb[nn + 26] = nn * DCH_PASSO;
    
    aa[nn] = 0.;
    aa[nn + 13] = DCH_STEP;
    aa[nn + 26] = 2 * DCH_STEP;
  }
  
  geo.Dch.nlayer = DCH_NLAY;
  geo.Dch.nwirelayer = DCH_NWIRELAY;
  geo.Dch.nsense = DCH_NSENSELAY;
  
  geo.Dch.CenterDch[0] = DCH_X;
  geo.Dch.CenterDch[1] = DCH_Y;
  geo.Dch.CenterDch[2] = DCH_Z;
  
  geo.Dch.DeltaDch[0] = DCH_DELTAX;
  geo.Dch.DeltaDch[1] = DCH_DELTAY;
  geo.Dch.DeltaDch[2] = DCH_DELTAZ;
  
  geo.Dch.SideDch[0] = DCH_WIDTH;
  geo.Dch.SideDch[1] = DCH_HEIGHT;
  geo.Dch.SideDch[2] = DCH_LENGTH;
  
  /* generazione parametri volumi di FLUKA :  ogni filo e' definito
     da un cilindro: 2 basi nell'alluminio e direzione
     parallela all'asse x ( vista U) e y (vista V) definito da:
     
     1) vettore posizione del punto centrale della base
     2) vettore che da una base arriva all'altra base
     3) raggio
  */
  
  for (int il = 0; il < geo.Dch.nlayer; il++) {
    
    //  U cells -> along x
    
    for (int ic = 0; ic < geo.Dch.nsense; ic++){

      geo.Dch.Uc_xm[ic][il] = geo.Dch.CenterDch[0] - geo.Dch.SideDch[0]/2 + shift;
      geo.Dch.Uc_xM[ic][il] = geo.Dch.CenterDch[0] + geo.Dch.SideDch[0]/2 - shift;
      geo.Dch.Uc_zm[ic][il] = geo.Dch.CenterDch[2] - geo.Dch.SideDch[2]/2
	+ geo.Dch.DeltaDch[2] + DCH_RFIELD + 4 * il * DCH_STEP
	+ 2 * il * DCH_LAYDIST + shift;
      geo.Dch.Uc_zM[ic][il] = geo.Dch.CenterDch[2] - geo.Dch.SideDch[2]/2
	+ geo.Dch.DeltaDch[2] - DCH_RFIELD + 4 * (il+1./2.) * DCH_STEP
	+ 2 * il * DCH_LAYDIST - .00001;

      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Dch.Uc_ym[ic][il] =  geo.Dch.CenterDch[1] - geo.Dch.SideDch[1] / 2
	  + geo.Dch.DeltaDch[1] + (2.*ic+1) * DCH_PASSO + DCH_RFIELD + shift;
	geo.Dch.Uc_yM[ic][il] =  geo.Dch.CenterDch[1] - geo.Dch.SideDch[1] / 2
	  + geo.Dch.DeltaDch[1] + (2.*ic+3) * DCH_PASSO - DCH_RFIELD - shift;
      } else {
	geo.Dch.Uc_ym[ic][il] =  geo.Dch.CenterDch[1] - geo.Dch.SideDch[1] / 2
	  + geo.Dch.DeltaDch[1] + 2.*ic * DCH_PASSO + DCH_RFIELD+shift;
	geo.Dch.Uc_yM[ic][il] =  geo.Dch.CenterDch[1] - geo.Dch.SideDch[1] / 2
	  + geo.Dch.DeltaDch[1] + (2.*ic+2) * DCH_PASSO - DCH_RFIELD - shift;
      }

      // V cells -> along y

      geo.Dch.Vc_ym[ic][il] = geo.Dch.CenterDch[1] - geo.Dch.SideDch[1]/2 + shift;
      geo.Dch.Vc_yM[ic][il] = geo.Dch.CenterDch[1] + geo.Dch.SideDch[1]/2 - shift;
      geo.Dch.Vc_zm[ic][il] = geo.Dch.CenterDch[2] - geo.Dch.SideDch[2]/2 
	+ geo.Dch.DeltaDch[2] + DCH_RFIELD + (4.*il+2) * DCH_STEP
	+ (2.*il+1) * DCH_LAYDIST + shift;
      geo.Dch.Vc_zM[ic][il] = geo.Dch.CenterDch[2] - geo.Dch.SideDch[2]/2
	+ geo.Dch.DeltaDch[2] - DCH_RFIELD + (4.*il+4)* DCH_STEP
	+ (2.*il+1) * DCH_LAYDIST - shift;

      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Dch.Vc_xm[ic][il] =  geo.Dch.CenterDch[0] - geo.Dch.SideDch[0] / 2
	  + geo.Dch.DeltaDch[0] + 2.*ic * DCH_PASSO + DCH_RFIELD + shift;
	geo.Dch.Vc_xM[ic][il] =  geo.Dch.CenterDch[0] - geo.Dch.SideDch[0] / 2
	  + geo.Dch.DeltaDch[0] + (2.*ic+2) * DCH_PASSO - DCH_RFIELD - shift;
      } else {
	geo.Dch.Vc_xm[ic][il] =  geo.Dch.CenterDch[0] - geo.Dch.SideDch[0] / 2
	  + geo.Dch.DeltaDch[0] + (2.*ic+1.) * DCH_PASSO + DCH_RFIELD + shift;
	geo.Dch.Vc_xM[ic][il] =  geo.Dch.CenterDch[0] - geo.Dch.SideDch[0] / 2
	  + geo.Dch.DeltaDch[0] + (2.*ic+3.) * DCH_PASSO - DCH_RFIELD;
      }
      
    }
    
    for (int iw = 0; iw < geo.Dch.nwirelayer; iw++) {
      
      //  U wires -> along x 

      geo.Dch.U_x[iw][il] = geo.Dch.CenterDch[0] - geo.Dch.SideDch[0] / 2;
      geo.Dch.U_z[iw][il] = geo.Dch.CenterDch[2] - geo.Dch.SideDch[2]/ 2 +
	geo.Dch.DeltaDch[2] + aa[iw] + (2 * il) * (2 * DCH_STEP + DCH_LAYDIST);
      
      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Dch.U_y[iw][il] =  geo.Dch.CenterDch[1] - geo.Dch.SideDch[1] / 2 
	  + geo.Dch.DeltaDch[1]
	  + bb[iw] + DCH_PASSO;
      } else {
	geo.Dch.U_y[iw][il] =  geo.Dch.CenterDch[1] - geo.Dch.SideDch[1] / 2 
	  + geo.Dch.DeltaDch[1] + bb[iw];
      }
      geo.Dch.U_cx[iw][il] = geo.Dch.SideDch[0];
      geo.Dch.U_cy[iw][il] = 0.;
      geo.Dch.U_cz[iw][il] = 0.;

      // V wires -> along y
      
      geo.Dch.V_y[iw][il] = geo.Dch.CenterDch[1] - geo.Dch.SideDch[1] / 2;
      geo.Dch.V_z[iw][il] = geo.Dch.CenterDch[2] - geo.Dch.SideDch[2] / 2
	+ geo.Dch.DeltaDch[2] + aa[iw] + (2*il+1) * (2 * DCH_STEP + DCH_LAYDIST);
      
      // FIRST:see above !!!!! upside down....
      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Dch.V_x[iw][il] = geo.Dch.CenterDch[0] - geo.Dch.SideDch[0] / 2
	  + geo.Dch.DeltaDch[0] + bb[iw];
      } else {
	geo.Dch.V_x[iw][il] = geo.Dch.CenterDch[0] - geo.Dch.SideDch[0] / 2
	  + geo.Dch.DeltaDch[0] + bb[iw] + DCH_PASSO;
      }
      geo.Dch.V_cx[iw][il] = 0.;
      geo.Dch.V_cy[iw][il] = geo.Dch.SideDch[1];
      geo.Dch.V_cz[iw][il] = 0.;
      
      // filo di sense 
      if ((iw == geo.Dch.idsense[0]) || (iw == geo.Dch.idsense[1])
	  || (iw == geo.Dch.idsense[2])) {
	geo.Dch.U_rad[iw][il] = DCH_RSENSE;
	geo.Dch.V_rad[iw][il] = DCH_RSENSE;
      }
      
      // filo di campo 
      else {
	geo.Dch.U_rad[iw][il] = DCH_RFIELD;
	geo.Dch.V_rad[iw][il] = DCH_RFIELD;
      }
    }
  }
  
  return 0;
  }


/*-----------------------------------------------------------------*/

GEOMETRY_STRUCT Geometry::GetStruct() {
	return geo;
}


/*-----------------------------------------------------------------*/
// assign a material
Int_t Geometry::AssignMaterial(TString Region, TString Material,
		Int_t Verbosity, TString* FLUKALine) {

	stringstream Line;

	Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << Region;

	*FLUKALine = Line.str();

	return 0;
}

/*-----------------------------------------------------------------*/
// assign a material from ... to ... steps of ...
Int_t Geometry::AssignMaterial(TString RegionS, TString RegionE, Int_t Step,
		TString Material, Int_t Verbosity, TString* FLUKALine) {

	stringstream Line;

	Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << RegionS
			<< setw(10) << RegionE << setw(10) << Step;

	*FLUKALine = Line.str();

	return 0;
}


/*-----------------------------------------------------------------*/
// assign a material, with option for magnetic field 
Int_t Geometry::AssignMaterial(TString Region,TString Material, Bool_t Magnetic,
			       Int_t Verbosity,TString* FLUKALine) {

	stringstream Line;

	if (Magnetic) {
		Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << Region
				<< setw(30) << 1.;
	} else {
		Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << Region;
	}

	*FLUKALine = Line.str();
	
	return 0;
}


/*-----------------------------------------------------------------*/
// assign a material, with option for magnetic field from ... to ... steps of ...
Int_t Geometry::AssignMaterial(TString RegionS, TString RegionE, Int_t Step,
			       TString Material, Bool_t Magnetic, Int_t Verbosity,
			       TString* FLUKALine) {

	stringstream Line;

	if (Magnetic) {
		Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << RegionS
		     << setw(10) << RegionE << setw(10) << Step << setw(10) << 1.;
	} else {
		Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << RegionS
		     << setw(10) << RegionE << setw(10) << Step;
	}

	*FLUKALine = Line.str();

	return 0;
}


/*-----------------------------------------------------------------*/
// create generic FLUKA card: TITLE WHAT(1-6) SDUM
Int_t Geometry::GenericCard(TString fTitle, Int_t fVerbosity,
		TString* fFLUKALine, TString fWHAT1, TString fWHAT2, TString fWHAT3,
		TString fWHAT4, TString fWHAT5, TString fWHAT6, TString fSDUM) {

	stringstream fLine;
	
	//max 10 letters
	if (fTitle.Sizeof() != 10) {
		fTitle.Resize(10);
	}
	//max 10 letters
	if (fSDUM.Sizeof() != 10) {
		fSDUM.Resize(10);
	}

	if (fWHAT1.Sizeof() > 10) {
		fWHAT1.Resize(10);
	}
	if (fWHAT2.Sizeof() > 10) {
		fWHAT2.Resize(10);
	}
	if (fWHAT3.Sizeof() > 10) {
		fWHAT3.Resize(10);
	}
	if (fWHAT4.Sizeof() > 10) {
		fWHAT4.Resize(10);
	}
	if (fWHAT5.Sizeof() > 10) {
		fWHAT5.Resize(10);
	}
	if (fWHAT6.Sizeof() > 10) {
		fWHAT6.Resize(10);
	}

	//       "* 23456789 *23456789 *23456789 *23456789 *23456789 *23456789 *23456789
	fLine << setw(10) << fTitle << setw(10) << fWHAT1 << setw(10) << fWHAT2
			<< setw(10) << fWHAT3 << setw(10) << fWHAT4 << setw(10) << fWHAT5
			<< setw(10) << fWHAT6 << setw(10) << fSDUM;
	
	*fFLUKALine = fLine.str();
	return 0;
}


/*-----------------------------------------------------------------*/
// take bodies and subtract them in groups of 8
Int_t Geometry::Reg8sub(TString fVolPreName, Int_t fVolNbStart,
		Int_t fVolNbEnd, Int_t fVerbosity, TString* fFLUKALine) {

	//check consistency and range of variables
	if (fVolNbStart > fVolNbEnd) {
		cerr << "ERROR: Geometry::Reg8sub no bodies!" << endl;
		return -1;
	}
	if (fVolNbEnd > 999) {
		cerr << "ERROR: Geometry::Reg8sub body number too high! " << fVolNbEnd
				<< endl;
		return -1;
	}

	stringstream fLine;

	//max 4 letters in vol name
	if (fVolPreName.Sizeof() > 5) {
		fVolPreName.Resize(5);
	}

	fLine << "             ";
	for (Int_t jj = fVolNbStart; jj <= fVolNbEnd; jj++) {
		if ((((jj - fVolNbStart) % 6) == 0) && (jj != fVolNbStart)) {
			fLine << "\n             ";
		}
		fLine << " -" << fVolPreName << jj;

	}
	fLine.flush();

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// take bodies and subtract them in groups of 8 (variable letter number)
Int_t Geometry::Reg8sub(TString fVolPreName, Int_t fVolNbStart,
		Int_t fVolNbEnd, Int_t fVerbosity, TString* fFLUKALine, Int_t fLetterNb) {

	//check consistency and range of variables
	if (fVolNbStart > fVolNbEnd) {
		cerr << "ERROR: Geometry::Reg8sub no bodies!" << endl;
		return -1;
	}
	if (fVolNbEnd > 9999) {
		cerr << "ERROR: Geometry::Reg8sub body number too high! " << fVolNbEnd
				<< endl;
		return -1;
	}

	stringstream fLine;

	//max fLetterNb letters in vol name
	if (fVolPreName.Sizeof() > fLetterNb) {
		fVolPreName.Resize(fLetterNb);
	}

	fLine << "             ";
	for (Int_t jj = fVolNbStart; jj <= fVolNbEnd; jj++) {
		if ((((jj - fVolNbStart) % 6) == 0) && (jj != fVolNbStart)) {
			fLine << "\n             ";
		}
		fLine << " -" << fVolPreName << jj;

	}
	fLine.flush();

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// take changing bodies and add them in groups of 8 (by union) with fExpression
Int_t Geometry::Reg8addExp(TString fVolPreName, Int_t fVolNbStart,
		Int_t fVolNbEnd, TString fExpression, Int_t fVerbosity,
		TString* fFLUKALine) {

	//check consistency and range of variables
	if (fVolNbStart > fVolNbEnd) {
		cerr << "ERROR: Geometry::Reg8add no bodies!" << endl;
		return -1;
	}
	if (fVolNbEnd > 999) {
		cerr << "ERROR: Geometry::Reg8add body number too high!" << endl;
		return -1;
	}

	stringstream fLine;

	//max 4 letters in vol name
	if (fVolPreName.Sizeof() > 5) {
		fVolPreName.Resize(5);
	}

	fLine << "             ";
	for (Int_t jj = fVolNbStart; jj <= fVolNbEnd; jj++) {
		if ((((jj - fVolNbStart) % 4) == 0) && (jj != fVolNbStart)) {
			fLine << "\n             ";
		}
		fLine << " |" << fExpression << " " << fVolPreName << jj;

	}

	fLine.flush();

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// take changing bodies and add them in groups of 8 (by union) with fExpression
Int_t Geometry::Reg8addVarExp(TString fVolPreName, Int_t fVolNbStart,
			      Int_t fVolNbEnd, TString fExpression,
			      Int_t fVerbosity,	TString* fFLUKALine) {

	//check consistency and range of variables
	if (fVolNbStart > fVolNbEnd) {
		cerr << "ERROR: Geometry::Reg8add no bodies!" << endl;
		return -1;
	}
	if (fVolNbEnd > 999) {
		cerr << "ERROR: Geometry::Reg8add body number too high!" << endl;
		return -1;
	}

	stringstream fLine;

	//max 4 letters in vol name
	if (fVolPreName.Sizeof() > 5) {
		fVolPreName.Resize(5);
	}

	fLine << "             ";
	for (Int_t jj = fVolNbStart; jj <= fVolNbEnd; jj++) {
		if ((((jj - fVolNbStart) % 4) == 0) && (jj != fVolNbStart)) {
			fLine << "\n             ";
		}
		fLine << " |" << fExpression << jj << "+ " << fVolPreName << jj;

	}

	fLine.flush();

	*fFLUKALine = fLine.str();
	return 0;
}


/*-----------------------------------------------------------------*/
// find first occurence of tag-pair in file, insert text, if fFileIn==fFileOut overwrite original file
Int_t Geometry::InsertInFile(TString fFileInS, TString fFileOutS,
		TString fTagStart, TString fTagStop, TString fInsertText) {

	ifstream fFileIn;
	ofstream fFileOut;
	TString fRead;

	Bool_t fTagFound = 0;

	cout << "READ DATA from " << fFileInS << " WRITE DATA to " << fFileOutS
			<< " ...";
	cout.flush();
	
	fFileIn.open(fFileInS);
	if (fFileInS == fFileOutS) {
		fFileOut.open(fFileOutS + ".tmp");
	} else {
		fFileOut.open(fFileOutS);
	}

	if (!fFileIn.good() && !fFileOut.good()) {
		cerr << "\nERROR: File Error!" << endl;
		fFileIn.close();
		fFileOut.close();
		return -1; 
	}

	while (!fFileIn.eof()) {
		fRead.ReadLine(fFileIn, kFALSE);
		if ((fTagFound == 0) && (strstr(fRead, fTagStart) != NULL)) {
			fFileOut << fRead << endl;
			fFileOut << fInsertText << endl;
			while (!fFileIn.eof()) {
				fRead.ReadLine(fFileIn, kFALSE);
				if (strstr(fRead, fTagStop) != NULL) {
					fTagFound = 1;
					break;
				}
			}

		}
		fFileOut << fRead << endl;
		if (strstr(fRead, "STOP") != NULL) break;
	}

	if (fTagFound == 0) {
		cerr << "\nERROR: File Error!!" << endl;
		fFileIn.close();
		fFileOut.close();
		return -1;
	}

	fFileIn.close();
	fFileOut.close();

	// overwrite original
	if (fFileInS == fFileOutS) {
		fFileIn.open(fFileOutS + ".tmp");
		fFileOut.open(fFileOutS);
		while (!fFileIn.eof()) {
			fRead.ReadLine(fFileIn, kFALSE);
			fFileOut << fRead << endl;
		}
		fFileIn.close();
		fFileOut.close();
	}

	cout << "DONE" << endl;
	return 0;

}


/*-----------------------------------------------------------------*/
// create box with position in centre, and half length given
Int_t Geometry::VolBox(TString fVolName, Int_t fVerbosity, TString* fFLUKALine,
		Double_t fPosX, Double_t fPosY, Double_t fPosZ, Double_t fWidX,
		Double_t fWidY, Double_t fWidZ) {

	stringstream fLine;
	//	fLine.precision();
	fLine << setiosflags(ios::fixed) << setprecision(6);
	
	Double_t fBox_X = fPosX - fWidX;
	Double_t fBox_Y = fPosY - fWidY;
	Double_t fBox_Z = fPosZ - fWidZ;

	if (fVerbosity > 1) {
		cout << "Box Volume: " << fVolName << endl;
	}

	fLine << "RPP " << fVolName << "     " << fBox_X << " " << fBox_X + 2. * fWidX
	      << " " << fBox_Y << " " << fBox_Y + 2. * fWidY << " " << fBox_Z << " "
	      << fBox_Z + 2. * fWidZ;

	*fFLUKALine = fLine.str();
	return 0;
}


/*-----------------------------------------------------------------*/
Geometry::~Geometry() {
}


///*********************************************************************************************
///*** EOF *************************************************************************************
///*********************************************************************************************
