#include <stdio.h>
#include <iostream>
#include <fstream>

#include <cmath>
//#include <math.h>

#include <iomanip>

#include <sstream>

#include "Geometry.h"
//#include <string>
//#include <stdlib.h>
#include "TSystem.h"
//#include <Riostream.h>
//#include "TFile.h"


using namespace std;

ClassImp(Geometry)
;

/*-----------------------------------------------------------------*/

Geometry::Geometry() {

  fRegionFlag = 0;
  fRegionName.clear();
  fRegionNumber.clear();
  
  /*old BMON
    for(int kk=0;kk<NWIRELAYER;kk++){
    for(int ii=0;ii<NLAYER;ii++){
    geo.V_x[ii][kk] = 0. ;
    geo.V_y[ii][kk]= 0. ;
    geo.V_z[ii][kk] = 0. ;
    geo.V_cx[ii][kk] = 0. ;
    geo.V_cy[ii][kk] = 0. ;
    geo.V_cz[ii][kk] = 0. ;
    geo.V_rad[ii][kk] = 0.;
    
    geo.U_x[ii][kk] = 0. ;
    geo.U_y[ii][kk]= 0. ;
    geo.U_z[ii][kk] = 0. ;
    geo.U_cx[ii][kk] = 0. ;
    geo.U_cy[ii][kk] = 0. ;
    geo.U_cz[ii][kk] = 0. ;
    geo.U_rad[ii][kk] = 0.;
    }
    }
  */
  //  Geometry::InitBmon();
  string c_dir = gSystem->Getenv("FLUKAGSI");
  c_dir = c_dir + "/config/FIRST_geo.map";
  InitGeoFromFile(c_dir.data());

  InitBmonNew();
  InitTarg();
  InitMimo();
  
}

void Geometry::InitGeoFromFile(string cfgname) {

  //Initialization of Geom Parameters
  //  Info("InitGeo()"," Init Geo ");
  
  char bufConf[1024];
  double my1(0), my2(0), my3(0), my4(0), my5(0), my6(0); 
  
  ifstream incF;
  incF.open(cfgname.data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", cfgname.data());
    return;
  }
  
  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'B')) {
      //Start counter
      sscanf(bufConf, "B%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadBMGeo()","Load Beam Monitor geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetBMCenter(my1,my2,my3);
      SetBMAngles(my4,my5,my6);
    } else if(strchr(bufConf,'S')) {
      //Start counter
      sscanf(bufConf, "S%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadIRGeo()","Load Start Counter geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetSCCenter(my1,my2,my3);
      SetSCAngles(my4,my5,my6);
    } else if(strchr(bufConf,'D')) {
      //Veto
      sscanf(bufConf, "D%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadIRGeo()","Load Veto geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetVECenter(my1,my2,my3);
      SetVEAngles(my4,my5,my6);
    } else if(strchr(bufConf,'V')) {
      //Vertex
      sscanf(bufConf, "V%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadVtxGeo()","Load Vertex geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      cout<<"The Vertex Planes are displaced 0.65 cm from the Target in z!! "<<endl;
      SetVTCenter(my1,my2,my3);
      SetVTAngles(my4,my5,my6);
    } else if(strchr(bufConf,'K')) {
      //Kentros
      sscanf(bufConf, "K%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadKeGeo()","Load Kentros geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetKECenter(my1,my2,my3);
      SetKEAngles(my4,my5,my6);
    } else if(strchr(bufConf,'A')) {
      //Aladin
      sscanf(bufConf, "A%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadAlaGeo()","Load Aladin geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetALCenter(my1,my2,my3);
      SetALAngles(my4,my5,my6);
    } else if(strchr(bufConf,'M')) {
      //Music
      sscanf(bufConf, "M%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadMusGeo()","Load Music geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetMUCenter(my1,my2,my3);
      SetMUAngles(my4,my5,my6);
    }else if(strchr(bufConf,'N')) {
      //New Reference point for tofwall
      sscanf(bufConf, "N%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadNewRefGeo()","Load New Ref geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetNUCenter(my1,my2,my3);
      SetNUAngles(my4,my5,my6);
    } else if(strchr(bufConf,'W')) {
      //Front Tof Wall
      sscanf(bufConf, "W%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadTofGeo()","Load TofWall Front geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetTWFCenter(my1,my2,my3);
      SetTWFAngles(my4,my5,my6);
    } else if(strchr(bufConf,'R')) {
      //Rear Tof Wall
      sscanf(bufConf, "R%lf %lf %lf %lf %lf %lf",&my1,&my2,&my3,&my4,&my5,&my6);
      //  Info("LoadTofGeo()","Load TofWall Rear geo:: %lf %lf %lf %lf %lf %lf",my1,my2,my3,my4,my5,my6);
      SetTWRCenter(my1,my2,my3);
      SetTWRAngles(my4,my5,my6);
    }


  }

  return;

}

/*-----------------------------------------------------------------*/

Int_t Geometry::InitBeam() {
  
  geo.Beam.A = PRIM_A;
  geo.Beam.Z = PRIM_Z;
  geo.Beam.Type = PRIM_Type;
  geo.Beam.T = PRIM_T;
  
  if (PRIM_DIST == "GAUSSIAN") {
    geo.Beam.Shape = 0;
  } else if (PRIM_DIST == "ANNULAR") {
		geo.Beam.Shape = 1;
  } else {
    geo.Beam.Shape = -1;
  }
  geo.Beam.Avg = TVector3(PRIM_Pos_X, PRIM_Pos_Y, PRIM_Pos_Z);
  geo.Beam.Sig = TVector3(PRIM_RMAX * FWHM2Sig, PRIM_RMAX * FWHM2Sig, 0.0);
  
  geo.Beam.MomSpr = PRIM_dP * FWHM2Sig;
  
  return 0;
}

Int_t Geometry::InitBmonNew() {//new BMON

  //  cout <<" Entro in Geometry::InitBmonNew() "<<endl;
  Double_t aa[BMN_WIRELAYER_N], bb[BMN_WIRELAYER_N];
  //array 7x3
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
  
  geo.Bmon.nlayer = BMN_LAYER_N;
  geo.Bmon.nwirelayer = BMN_WIRELAYER_N;
  geo.Bmon.nsense = BMN_SENSE_N;
  
  geo.Bmon.CenterDch[0] = m_BM_center.X();
  geo.Bmon.CenterDch[1] = m_BM_center.Y();
  geo.Bmon.CenterDch[2] = m_BM_center.Z();
  
  geo.Bmon.DeltaDch[0] = BMN_DELTA_X;
  geo.Bmon.DeltaDch[1] = BMN_DELTA_Y;
  geo.Bmon.DeltaDch[2] = BMN_DELTA_Z;
  
  geo.Bmon.SideDch[0] = BMN_WIDTH;
  geo.Bmon.SideDch[1] = BMN_HEIGHT;
  geo.Bmon.SideDch[2] = BMN_LENGTH;
  
  /* generazione parametri volumi di FLUKA :  ogni filo e' definito
     da un cilindro: 2 basi nel mylar e direzione
     parallela all'asse x ( vista U) e y (vista V) definito da:
     
     1) vettore posizione del punto centrale della base
     2) vettore che da una base arriva all'altra base
     3) raggio
  */
  for (int il = 0; il < geo.Bmon.nlayer; il++) {
    for (int kk = 0; kk < geo.Bmon.nwirelayer; kk++) {
      
      /*  U wires -> along x */
      geo.Bmon.U_x[kk][il] = geo.Bmon.CenterDch[0] - geo.Bmon.SideDch[0] / 2;
      geo.Bmon.U_z[kk][il] = geo.Bmon.CenterDch[2] - geo.Bmon.SideDch[2]/ 2 + geo.Bmon.DeltaDch[2] 
	+aa[kk] + (2 * il) * (2 * BMN_STEP + BMN_PLANE_D);

      /* in principles should be like that but we mounted the chamber upside down*/
      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Bmon.U_y[kk][il] =  geo.Bmon.CenterDch[1] - geo.Bmon.SideDch[1] / 2 
	  + geo.Bmon.DeltaDch[1]
	  + bb[kk] + BMN_PASSO;
      } else {
	geo.Bmon.U_y[kk][il] =  geo.Bmon.CenterDch[1] - geo.Bmon.SideDch[1] / 2 
	  + geo.Bmon.DeltaDch[1] + bb[kk];
      }
      geo.Bmon.U_cx[kk][il] = geo.Bmon.SideDch[0];
      geo.Bmon.U_cy[kk][il] = 0.;
      geo.Bmon.U_cz[kk][il] = 0.;
      
      /* V wires -> along y*/
      geo.Bmon.V_y[kk][il] = geo.Bmon.CenterDch[1] - geo.Bmon.SideDch[1] / 2;
      geo.Bmon.V_z[kk][il] = geo.Bmon.CenterDch[2] - geo.Bmon.SideDch[2] / 2 + geo.Bmon.DeltaDch[2] 
	+ aa[kk] + (2*il+1) * (2 * BMN_STEP + BMN_PLANE_D);
      
      /* see above !!!!! upside down....*/
      if ((il == 0) || (il == 2) || (il == 4)) {
	geo.Bmon.V_x[kk][il] = geo.Bmon.CenterDch[0] - geo.Bmon.SideDch[0] / 2 + geo.Bmon.DeltaDch[0]
	  + bb[kk];
      } else {
	geo.Bmon.V_x[kk][il] = geo.Bmon.CenterDch[0] - geo.Bmon.SideDch[0] / 2 + geo.Bmon.DeltaDch[0]
	  + bb[kk] + BMN_PASSO;
      }
      geo.Bmon.V_cx[kk][il] = 0.;
      geo.Bmon.V_cy[kk][il] = geo.Bmon.SideDch[1];
      geo.Bmon.V_cz[kk][il] = 0.;
      
      /* filo di sense */
      if ((kk == geo.Bmon.idsense[0]) || (kk == geo.Bmon.idsense[1])
	  || (kk == geo.Bmon.idsense[2])) {
	geo.Bmon.U_rad[kk][il] = BMN_RSENSE;
	geo.Bmon.V_rad[kk][il] = BMN_RSENSE;
      }
      
      /* filo di campo */
      else {
	geo.Bmon.U_rad[kk][il] = BMN_RFIELD;
	geo.Bmon.V_rad[kk][il] = BMN_RFIELD;
      }
    }
  }
  
  return 0;
}

/*-----------------------------------------------------------------*/
//Int_t Geometry::InitBmon(){//old BMON
//  cout <<" Entro in InitBmonGeometry "<<endl;
/*   Double_t aa[NWIRELAYER], bb[NWIRELAYER];

 if ((elle==0.0)||(elle<acca)) cout<<"ERROR: values of acca,elle wrong:"<<acca<<" "<<elle<<endl;
 geo.UWC[0] = 0.;
 geo.UWC[1] = 1.;
 geo.UWC[2] = 0.;

 geo.UWS[0] = 0.;
 geo.UWS[1] = sqrt(1-acca*acca/elle/elle);
 geo.UWS[2] = -acca/elle;

 geo.UWD[0] = 0.;
 geo.UWD[1] = sqrt(1-acca*acca/elle/elle);
 geo.UWD[2] = acca/elle;

 geo.VWC[0] = 1.;
 geo.VWC[1] = 0.;
 geo.VWC[2] = 0.;

 geo.VWS[0] = sqrt(1-acca*acca/elle/elle);
 geo.VWS[1] = 0.;
 geo.VWS[2] = -acca/elle;

 geo.VWD[0] = sqrt(1-acca*acca/elle/elle);
 geo.VWD[1] = 0.;
 geo.VWD[2] = acca/elle;


 geo.idsense[0]= 4;
 geo.idsense[1]= 9;
 geo.idsense[2]= 14;
 geo.idsense[3]= 19;


 aa[0] = 0.5*elle ;
 aa[1] = 1.5*elle ;
 aa[2] = 0. ;
 aa[3] = elle ;
 aa[4] = 2*elle ;
 aa[5] = 0.5*elle ;
 aa[6] = 1.5*elle;
 aa[7] = 0. ;
 aa[8] = elle ;
 aa[9] = 2*elle ;
 aa[10] = 0.5*elle ;
 aa[11] = 1.5*elle ;
 aa[12] = 0. ;
 aa[13] = elle ;
 aa[14] = 2*elle ;
 aa[15] = 0.5*elle ;
 aa[16] = 1.5*elle ;
 aa[17] = 0. ;
 aa[18] = elle ;
 aa[19] = 2*elle ;
 aa[20] = 0.5*elle;
 aa[21] = 1.5*elle;

 bb[0] = 0. ;
 bb[1] = 0. ;
 bb[2] = acca ;
 bb[3] = acca ;
 bb[4] = acca ;
 bb[5] = 2*acca ;
 bb[6] = 2*acca ;
 bb[7] = 3*acca ;
 bb[8] = 3*acca ;
 bb[9] = 3*acca ;
 bb[10] = 4*acca ;
 bb[11] = 4*acca ;
 bb[12] = 5*acca ;
 bb[13] = 5*acca ;
 bb[14] = 5*acca ;
 bb[15] = 6*acca ;
 bb[16] = 6*acca ;
 bb[17] = 7*acca ;
 bb[18] = 7*acca ;
 bb[19] = 7*acca ;
 bb[20] = 8*acca;
 bb[21] = 8*acca;

 //    generazione parametri volumi di FLUKA :  ogni filo e' definito
 //    da un cilindro: 2 basi nel mylar e direzione
 //    parallela all'asse x ( vista U) e y (vista V) definito da:

 //    1) vettore posizione del punto centrale della base
 //    2) vettore che da una base arriva all'altra base
 //    3) raggio

 for(int il=0; il<NLAYER;il++){
 for (int kk =0; kk<NWIRELAYER;kk++){
 //  U wires -> along x
 geo.U_x[kk][il] = (-BM_WIDTH+MYLARTHICK)/2;
 geo.U_z[kk][il] = aa[kk] + BM_Z + DELTAZ +
 2*(il)*(2*elle + delta_chamber) ;
 if( (il==0) || (il==2)){
 geo.U_y[kk][il] = -BM_HEIGHT/2 + bb[kk] + DELTAY ;
 }
 else{
 geo.U_y[kk][il] = -BM_HEIGHT/2 + bb[kk] + DELTAY + acca ;
 }
 geo.U_cx[kk][il] = BM_WIDTH + MYLARTHICK;
 geo.U_cy[kk][il] = 0.;
 geo.U_cz[kk][il] = 0.;
 // V wires -> along y
 if( (il==0) || (il==2)){
 geo.V_x[kk][il] = -BM_WIDTH/2 + bb[kk] + DELTAX ;
 }
 else{
 geo.V_x[kk][il] = -BM_WIDTH/2 + bb[kk] + DELTAX + acca ;
 }
 geo.V_y[kk][il] = (-BM_HEIGHT+MYLARTHICK)/2;
 geo.V_z[kk][il] = aa[kk] + BM_Z + DELTAZ +
 (2*il+1)*(2*elle +delta_chamber) ;
 geo.V_cx[kk][il] = 0.;
 geo.V_cy[kk][il] = BM_HEIGHT + MYLARTHICK;;
 geo.V_cz[kk][il] = 0.;
 // filo di sense
 if((kk+1==geo.idsense[0]) ||(kk+1==geo.idsense[1])
 ||(kk+1==geo.idsense[2]) ||(kk+1==geo.idsense[3]) ) {
 geo.U_rad[kk][il] = RSENSE;
 geo.V_rad[kk][il] = RSENSE;
 }
 // filo di campo
 else{
 geo.U_rad[kk][il] = RFIELD;
 geo.V_rad[kk][il] = RFIELD;
 }
 }
 }
 */
//  return 0;
//}


/*-----------------------------------------------------------------*/

Int_t Geometry::InitTarg() {

  //  cout <<" Entro in Geometry::InitTARG "<<endl;
  
  geo.Targ.POS = m_TG_center;
  
  geo.Targ.DIM = TVector3(TG_WIDTH, TG_HEIGHT, TG_THICK);
  
  //  cout <<" Esco da Geometry::InitTARG "<<endl;
  return 0;
}

/*-----------------------------------------------------------------*/

Int_t Geometry::InitMimo() {
  //  cout <<" Entro in Geometry::InitMIMO "<<endl;

  geo.Mimo.DIST_TGT_MI = m_VT_center.Z() - m_TG_center.Z() - TG_THICK; //MI_DIST_TGT_MI;//distance target (end) and mimosa (beginning of first plane)
  geo.Mimo.DIST_PLANES = MI_DIST_PLANES;

  geo.Mimo.POS = TVector3(m_VT_center.X(), m_VT_center.Y(), m_VT_center.Z());//set z-position to start of first plane (without MIMOSA chip)

  geo.Mimo.MIMO_N = MI_MIMO_N; //number of MIMOSA chips

  geo.Mimo.MIMO_DIM = TVector3(MI_MIMO_HEIGHT, MI_MIMO_WIDTH, MI_MIMO_THICK);

  geo.Mimo.SENS_DIM = TVector3(MI_SENS_WIDTH, MI_SENS_HEIGHT, MI_SENS_THICK);

  geo.Mimo.SENS_BIN[0] = MI_SENS_BIN_HEIGHT;
  geo.Mimo.SENS_BIN[1] = MI_SENS_BIN_WIDTH;
  geo.Mimo.SENS_BIN[2] = MI_SENS_BIN_THICK;

  geo.Mimo.PAD_DIM = TVector3(MI_PAD_WIDTH, MI_PAD_HEIGHT, MI_PAD_THICK);

  for (Int_t i0 = 0; i0 < MI_MIMO_N; i0++) {
    //Used only on MI_Coord, not yet verified against rotations 
    geo.Mimo.MIMO_SHIFT[i0] = TVector3(0., MI_MIMO_Y[i0], 0.);
  }

  //init
  geo.Mimo.DIST_TGT_MIMO[0] = TG_THICK + geo.Mimo.DIST_TGT_MI + 0
    * geo.Mimo.DIST_PLANES - geo.Mimo.MIMO_DIM.Z();
  geo.Mimo.DIST_TGT_MIMO[1] = TG_THICK + geo.Mimo.DIST_TGT_MI + 0
    * geo.Mimo.DIST_PLANES + MI_GLAFI_THICK;
  geo.Mimo.DIST_TGT_MIMO[2] = TG_THICK + geo.Mimo.DIST_TGT_MI + 1
    * geo.Mimo.DIST_PLANES - geo.Mimo.MIMO_DIM.Z();
  geo.Mimo.DIST_TGT_MIMO[3] = TG_THICK + geo.Mimo.DIST_TGT_MI + 1
    * geo.Mimo.DIST_PLANES + MI_GLAFI_THICK;
  geo.Mimo.DIST_TGT_MIMO[4] = TG_THICK + geo.Mimo.DIST_TGT_MI + 2
    * geo.Mimo.DIST_PLANES - geo.Mimo.MIMO_DIM.Z();
  geo.Mimo.DIST_TGT_MIMO[5] = TG_THICK + geo.Mimo.DIST_TGT_MI + 2
    * geo.Mimo.DIST_PLANES + MI_GLAFI_THICK;
  geo.Mimo.DIST_TGT_MIMO[6] = TG_THICK + geo.Mimo.DIST_TGT_MI + 3
    * geo.Mimo.DIST_PLANES - geo.Mimo.MIMO_DIM.Z();
  geo.Mimo.DIST_TGT_MIMO[7] = TG_THICK + geo.Mimo.DIST_TGT_MI + 3
    * geo.Mimo.DIST_PLANES + MI_GLAFI_THICK;

  for (Int_t i0 = 0; i0 < MI_MIMO_N; i0++) {
    geo.Mimo.MIMO_POS_Z[i0] = m_TG_center.Z() + geo.Mimo.DIST_TGT_MIMO[i0];
  }

  //  cout <<" Esco da Geometry::InitMIMO "<<endl;
  return 0;
}

/*-----------------------------------------------------------------*/

GEOMETRY_STRUCT Geometry::GetStruct() {
	return geo;
}

/*-----------------------------------------------------------------*/
//read association of FLUKA region names and correspoding region numbers
Int_t Geometry::InitRegions(TString FileName) {

	ifstream FileIn;
	TString Read;
	TString RegionName;
	Int_t RegionNumber;

	if (fRegionFlag == 1) {//if already initialized
		cout << "Geometry: Reinitialize regions!" << endl;
		fRegionName.clear();
		fRegionNumber.clear();
	}

	cout << "READ DATA from: <" << FileName << "> ...";
	cout.flush();
	FileIn.open(FileName);

	if (!FileIn.good()) {
		cerr << "\nERROR: File Error!" << endl;
		FileIn.close();
		return -1;// exit(-1);
	}

	while (FileIn.good()) {
		Read.ReadToken(FileIn);
		RegionName = Read;
		fRegionName.push_back(RegionName);

		Read.ReadLine(FileIn);
		sscanf(Read, "\n%d\n", &RegionNumber);
		fRegionNumber.push_back(RegionNumber);

		//cout<<RegionName<<" "<<RegionNumber<<" "<<endl;
	}

	FileIn.close();

	cout << "DONE" << endl;

	fRegionFlag = 1;
	return 0;
}

/*-----------------------------------------------------------------*/

// This routine reads in the magnetic field maps and interpolates them for a given current 
// (interpolation not implemented yet due to missing maps, just linear scaling is applied, shifting and rotation not used)
// and generates a new file which is read by magfld.f. Also position and rotation of the magnet is saved in this file.
// WATCH OUT: coordinate system of magfield map is NOT the same as for geometry
Int_t Geometry::InitMagneticField(Double_t shiftX, Double_t shiftY,
		Double_t shiftZ, Double_t angleY, Double_t current) {

	//cout <<" Entro in InitMagneticField "<<endl;

	TString fFileInS[] = { "INPUT/aladin25.map" };
	Double_t fFileInCurrent[] = { 25.0 };
	//norm from SPALADiN: -0.760 (-.53 default, -0.760 for Si, -0.960 for Xe)
	TString fFileOutS = "INPUT/aladinInt.map";

	ifstream fFileIn;
	ofstream fFileOut;
	TString fRead;

	Int_t ix, iy, iz;
	Int_t fNX[3], fNY[3], fNZ[3];
	Double_t fMinX[3], fMinY[3], fMinZ[3];
	Double_t fMaxX[3], fMaxY[3], fMaxZ[3];
	Double_t fShiftX[3], fShiftY[3], fShiftZ[3];
	Double_t fRangeX[3], fRangeY[3], fRangeZ[3];

	Double_t fGetX = -1., fGetY = -1., fGetZ = -1.;
	Double_t fGetBX = -1., fGetBY = -1., fGetBZ = -1.;

	Double_t fNormalize;

	if (fFileInCurrent[0] == 0.0) {
		cout << "ERROR fFileInCurrent[0]=" << fFileInCurrent[0] << endl;
	} else {
		fNormalize = current / fFileInCurrent[0];
	}
	cout << "READ DATA from: " << fFileInS[0] << "  WRITE DATA to: "
			<< fFileOutS << " ...";
	cout.flush();
	fFileIn.open(fFileInS[0]);
	fFileOut.open(fFileOutS);
	fFileOut << setiosflags(ios::fixed) << setprecision(6);

	if (!fFileIn.good() && !fFileOut.good()) {
		cerr << "\nERROR: File Error!" << endl;
		fFileIn.close();
		fFileOut.close();
		return -1;// exit(-1);
	}

	//read & write header
	//fFileOut<<"# Interpolated field map for ALADiN magnet"<<endl;
	//fFileOut<<"# for a current of "<<current<<" A"<<endl;
	//fFileOut<<" "<<shiftX<<" "<<shiftY<<" "<<shiftZ<<endl;
	//fFileOut<<" "<<angleY<<endl;

	fRead.ReadLine(fFileIn);
	if (strstr(fRead, "5") == NULL) {
		cerr << "\nERROR: False File content!\n" << endl;
		return -1;// exit(-1);
	}
	fFileOut << fRead << endl;
	fRead.ReadLine(fFileIn);
	if (strstr(fRead, "nnn") == NULL) {
		cerr << "\nERROR: False File content!\n" << endl;
		return -1;// exit(-1);
	}
	fFileOut << fRead << endl;

	fRead.ReadLine(fFileIn);
	sscanf(fRead, "\n %d %lf %lf \n", &fNX[0], &fMinX[0], &fMaxX[0]);
	if (fNX[0] < 1) {
		fprintf(stderr, "\n!!!\n!!! ERROR: fNX = %d < 1 \n!!!", fNX[0]);
	}
	if (fMinX[0] >= fMaxX[0]) {
		fprintf(stderr,
				"\n!!!\n!!! ERROR: Field Min = %0.1f > Max = %0.1f \n!!!",
				fMinX[0], fMaxX[0]);
	}

	fRead.ReadLine(fFileIn);
	sscanf(fRead, "\n %d %lf %lf \n", &fNY[0], &fMinY[0], &fMaxY[0]);
	if (fNY[0] < 1) {
		fprintf(stderr, "\n!!!\n!!! ERROR: fNY = %d < 1 \n!!!", fNY[0]);
	}
	if (fMinY[0] >= fMaxY[0]) {
		fprintf(stderr,
				"\n!!!\n!!! ERROR: Field Min = %0.1f > Max = %0.1f \n!!!",
				fMinY[0], fMaxY[0]);
	}

	fRead.ReadLine(fFileIn);
	sscanf(fRead, "\n %d %lf %lf \n", &fNZ[0], &fMinZ[0], &fMaxZ[0]);
	if (fNZ[0] < 1) {
		fprintf(stderr, "\n!!!\n!!! ERROR: fNX = %d < 1 \n!!!", fNZ[0]);
	}
	if (fMinZ[0] >= fMaxZ[0]) {
		fprintf(stderr,
				"\n!!!\n!!! ERROR: Field Min = %0.1f > Max = %0.1f \n!!!",
				fMinZ[0], fMaxZ[0]);
	}

	fRead.ReadLine(fFileIn);

	fShiftX[0] = fMaxX[0] + fMinX[0];
	fShiftY[0] = fMaxY[0] + fMinY[0];
	fShiftZ[0] = fMaxZ[0] + fMinZ[0];
	fRangeX[0] = fMaxX[0] - fMinX[0];
	fRangeY[0] = fMaxY[0] - fMinY[0];
	fRangeZ[0] = fMaxZ[0] - fMinZ[0];

	//origin centred
	//scale y since the field does not cover our aperture (y corresponds to z , scale 1.39)
	//scale also x (x corresponds to x , scale 1.17)
	fFileOut << " " << fNX[0] << " " << 1.17 * (fMinX[0] - fShiftX[0] / 2.0)
			<< " " << 1.17 * (fMaxX[0] - fShiftX[0] / 2.0) << endl;
	fFileOut << " " << fNY[0] << " " << 1. * (fMinY[0] - fShiftY[0] / 2.0)
			<< " " << 1. * (fMaxY[0] - fShiftY[0] / 2.0) << endl;
	fFileOut << " " << fNZ[0] << " " << 1.39 * (fMinZ[0] - fShiftZ[0] / 2.0)
			<< " " << 1.39 * (fMaxZ[0] - fShiftZ[0] / 2.0) << endl;

	//origin not centred
	//   fFileOut<<" "<<fNX[0]<<" "<<fMinX[0]<<" "<<fMaxX[0]<<endl;
	//   fFileOut<<" "<<fNY[0]<<" "<<fMinY[0]<<" "<<fMaxY[0]<<endl;
	//   fFileOut<<" "<<fNZ[0]<<" "<<fMinZ[0]<<" "<<fMaxZ[0]<<endl;
	fFileOut << fRead << endl;

	//read & write body
	for (ix = 0; ix < fNX[0]; ix++) {
		for (iy = 0; iy < fNY[0]; iy++) {
			for (iz = 0; iz < fNZ[0]; iz++) {
				fRead.ReadLine(fFileIn);
				sscanf(fRead, "%lf %lf %lf %lf %lf %lf \n", &fGetX, &fGetY,
						&fGetZ, &fGetBX, &fGetBY, &fGetBZ);
				//origin centred
				//scale y since the field does not cover our aperture (y corresponds to z , scale 1.39)
				//scale also x (x corresponds to x , scale 1.17)
				fFileOut << "\t" << 1.17 * (fGetX - fShiftX[0] / 2.0) << "\t"
						<< 1. * (fGetY - fShiftY[0] / 2.0) << "\t" << 1.39
						* (fGetZ - fShiftZ[0] / 2.0) << "\t" << fNormalize
						* fGetBX << "\t" << fNormalize * fGetBY << "\t"
						<< fNormalize * fGetBZ << endl;
				//origin not centred
				//         fFileOut<<"\t"<<fGetX<<"\t"<<fGetY<<"\t"<<fGetZ<<"\t"<<fNormalize*fGetBX<<"\t"<<fNormalize*fGetBY<<"\t"<<fNormalize*fGetBZ<<endl;
			}
		}
	}

	fFileIn.close();
	fFileOut.close();
	cout << "DONE" << endl;
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
		return -1; //exit(-1);
	}

	while (!fFileIn.eof()) {
		fRead.ReadLine(fFileIn, kFALSE);
		if ((fTagFound == 0) && (strstr(fRead, fTagStart) != NULL)) {
			fFileOut << fRead << endl;
			fFileOut << fInsertText << endl;
			//      fRead="";
			while (!fFileIn.eof()) {
				fRead.ReadLine(fFileIn, kFALSE);
				if (strstr(fRead, fTagStop) != NULL) {
					fTagFound = 1;
					break;
				}
			}

		}
		fFileOut << fRead << endl;
	}

	if (fTagFound == 0) {
		cerr << "\nERROR: File Error!" << endl;
		fFileIn.close();
		fFileOut.close();
		return -1;// exit(-1);
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

	//  Int_t fEntryNb=fVolNbEnd-fVolNbStart;
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
	if (fVolNbEnd > 999) {
		cerr << "ERROR: Geometry::Reg8sub body number too high! " << fVolNbEnd
				<< endl;
		return -1;
	}

	//  Int_t fEntryNb=fVolNbEnd-fVolNbStart;
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
// take bodies and add them in groups of 8 (by union)
Int_t Geometry::Reg8add(TString fVolPreName, Int_t fVolNbStart,
		Int_t fVolNbEnd, Int_t fVerbosity, TString* fFLUKALine) {

	//check consistency and range of variables
	if (fVolNbStart > fVolNbEnd) {
		cerr << "ERROR: Geometry::Reg8add no bodies!" << endl;
		return -1;
	}
	if (fVolNbEnd > 999) {
		cerr << "ERROR: Geometry::Reg8add body number too high!" << endl;
		return -1;
	}

	//  Int_t fEntryNb=fVolNbEnd-fVolNbStart;
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
		fLine << " |" << fVolPreName << jj;

	}
	// fLine<<")";
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

	//  Int_t fEntryNb=fVolNbEnd-fVolNbStart;
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
	// fLine<<")";
	fLine.flush();

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// assign a material
Int_t Geometry::AssignMaterial(TString Region, TString Material,
		Int_t Verbosity, TString* FLUKALine) {

	stringstream Line;

	//max 8 letters in region name
	if (Region.Sizeof() > 8) {
		Region.Resize(8);
	}

	Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << Region;
	//Line.flush();

	*FLUKALine = Line.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// assign a material from ... to ... steps of ...
Int_t Geometry::AssignMaterial(TString RegionS, TString RegionE, Int_t Step,
		TString Material, Int_t Verbosity, TString* FLUKALine) {

	stringstream Line;

	//max 8 letters in region name
	if (RegionS.Sizeof() > 8) {
		RegionS.Resize(8);
	}
	if (RegionE.Sizeof() > 8) {
		RegionE.Resize(8);
	}

	Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << RegionS
			<< setw(10) << RegionE << setw(10) << Step;
	//fLine.flush();

	*FLUKALine = Line.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// assign a material, with option for magnetic field 
Int_t Geometry::AssignMaterial(TString Region, TString Material,
		Bool_t Magnetic, Int_t Verbosity, TString* FLUKALine) {

	stringstream Line;

	//max 8 letters in region name
	if (Region.Sizeof() > 8) {
		Region.Resize(8);
	}

	if (Magnetic) {
		Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << Region
				<< setw(30) << 1.;
	} else {
		Line << "ASSIGNMA  " << setw(10) << Material << setw(10) << Region;
	}

	//Line.flush();

	*FLUKALine = Line.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create box with position in centre, and half length given
Int_t Geometry::VolBox(TString fVolName, Int_t fVerbosity, TString* fFLUKALine,
		Double_t fPosX, Double_t fPosY, Double_t fPosZ, Double_t fWidX,
		Double_t fWidY, Double_t fWidZ) {

	stringstream fLine;
	fLine.precision(16);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}
	Double_t fBox_X = fPosX - fWidX;
	Double_t fBox_Y = fPosY - fWidY;
	Double_t fBox_Z = fPosZ - fWidZ;

	if (fVerbosity > 1) {
		cout << "Box Volume: " << fVolName << endl;
	}

	if (kFormat == 0) {
		fLine << "RPP " << fVolName << "     " << fBox_X << " " << fBox_X + 2.
				* fWidX << endl;
		fLine << "               " << fBox_Y << " " << fBox_Y + 2. * fWidY
				<< endl;
		fLine << "               " << fBox_Z << " " << fBox_Z + 2. * fWidZ;
	} else {
		fLine << "RPP " << fVolName << "     " << fBox_X << " " << fBox_X + 2.
				* fWidX << " " << fBox_Y << " " << fBox_Y + 2. * fWidY << " "
				<< fBox_Z << " " << fBox_Z + 2. * fWidZ;
	}

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create box with position in centre, and half length given (limited precision: 0.1um)
Int_t Geometry::VolBoxLP(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fWidX, Double_t fWidY, Double_t fWidZ) {

	stringstream fLine;
	//  fLine.precision(16);
	fLine << setiosflags(ios::fixed) << setprecision(5);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}
	Double_t fBox_X = fPosX - fWidX;
	Double_t fBox_Y = fPosY - fWidY;
	Double_t fBox_Z = fPosZ - fWidZ;

	if (fVerbosity > 1) {
		cout << "Box Volume: " << fVolName << endl;
	}

	if (kFormat == 0) {
		fLine << "RPP " << fVolName << "     " << fBox_X << " " << fBox_X + 2.
				* fWidX << endl;
		fLine << "               " << fBox_Y << " " << fBox_Y + 2. * fWidY
				<< endl;
		fLine << "               " << fBox_Z << " " << fBox_Z + 2. * fWidZ;
	} else {
		fLine << "RPP " << fVolName << "     " << fBox_X << " " << fBox_X + 2.
				* fWidX << " " << fBox_Y << " " << fBox_Y + 2. * fWidY << " "
				<< fBox_Z << " " << fBox_Z + 2. * fWidZ;
	}

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create box with angle Y, position in centre in X,Y and Z plus Rad in z-direction (when AngleZ=0), and half length given, 
Int_t Geometry::VolBox_Y(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fWidX, Double_t fWidY, Double_t fWidZ, Double_t fAngY,
		Double_t fRad) {

	stringstream fLine;
	fLine.precision(16);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}

	Double_t fBox_V1_X = std::cos(fAngY * PI / 180.) * fWidX * 2.0;
	Double_t fBox_V1_Y = 0.0;
	Double_t fBox_V1_Z = -std::sin(fAngY * PI / 180.) * fWidX * 2.0;
	Double_t fBox_V2_X = 0.0;
	Double_t fBox_V2_Y = fWidY * 2.0;
	Double_t fBox_V2_Z = 0.0;
	Double_t fBox_V3_X = std::sin(fAngY * PI / 180.) * fWidZ * 2.0;
	Double_t fBox_V3_Y = 0.0;
	Double_t fBox_V3_Z = std::cos(fAngY * PI / 180.) * fWidZ * 2.0;

	Double_t fDir_Z_X = std::sin(fAngY * PI / 180.);
	Double_t fDir_Z_Y = 0.0;
	Double_t fDir_Z_Z = std::cos(fAngY * PI / 180.);

	Double_t fBox_X = fPosX - std::cos(fAngY * PI / 180.) * fWidX + fDir_Z_X
			* (fRad - fWidZ);
	Double_t fBox_Y = fPosY - fWidY + fDir_Z_Y * (fRad - fWidZ);
	Double_t fBox_Z = fPosZ + std::sin(fAngY * PI / 180.) * fWidX + fDir_Z_Z
			* (fRad - fWidZ);

	if (fVerbosity > 1) {
		cout << "Box Volume: " << fVolName << endl;
		cout << "Angle Y:  " << fAngY << endl;
		cout << "Vectors12:" << fBox_V1_X * fBox_V2_X + fBox_V1_Y * fBox_V2_Y
				+ fBox_V1_Z * fBox_V2_Z << endl;
		cout << "Vectors13:" << fBox_V1_X * fBox_V3_X + fBox_V1_Y * fBox_V3_Y
				+ fBox_V1_Z * fBox_V3_Z << endl;
		cout << "Vectors23:" << fBox_V2_X * fBox_V3_X + fBox_V2_Y * fBox_V3_Y
				+ fBox_V2_Z * fBox_V3_Z << endl;
		cout << "sqrtVec11:" << std::sqrt(
				fBox_V1_X * fBox_V1_X + fBox_V1_Y * fBox_V1_Y + fBox_V1_Z
						* fBox_V1_Z) << endl;
		cout << "sqrtVec22:" << std::sqrt(
				fBox_V2_X * fBox_V2_X + fBox_V2_Y * fBox_V2_Y + fBox_V2_Z
						* fBox_V2_Z) << endl;
		cout << "sqrtVec33:" << std::sqrt(
				fBox_V3_X * fBox_V3_X + fBox_V3_Y * fBox_V3_Y + fBox_V3_Z
						* fBox_V3_Z) << endl;
	}

	if (kFormat == 0) {
		fLine << "BOX " << fVolName << "     " << fBox_X << " " << fBox_Y
				<< " " << fBox_Z << endl;
		fLine << "               " << fBox_V1_X << " " << fBox_V1_Y << " "
				<< fBox_V1_Z << endl;
		fLine << "               " << fBox_V2_X << " " << fBox_V2_Y << " "
				<< fBox_V2_Z << endl;
		fLine << "               " << fBox_V3_X << " " << fBox_V3_Y << " "
				<< fBox_V3_Z;
	} else {
		fLine << "BOX " << fVolName << "     " << fBox_X << " " << fBox_Y
		      << " " << fBox_V1_X << " " << fBox_V1_Y << " " << fBox_V1_Z<<" "
		      << fBox_V2_X << " " << fBox_V2_Y << " " << fBox_V2_Z<<" "
		      << fBox_V3_X << " " << fBox_V3_Y << " " << fBox_V3_Z;
	}

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create box with angle Y, position in centre in X,Y and at beginning of Z plus Rad in z-direction (when AngleZ=0), and half length given, 
Int_t Geometry::VolBox_Y_bZ(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fWidX, Double_t fWidY, Double_t fWidZ, Double_t fAngY,
		Double_t fRad) {

	stringstream fLine;
	fLine.precision(16);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}

	Double_t fBox_V1_X = std::cos(fAngY * PI / 180.) * fWidX * 2.0;
	Double_t fBox_V1_Y = 0.0;
	Double_t fBox_V1_Z = -std::sin(fAngY * PI / 180.) * fWidX * 2.0;
	Double_t fBox_V2_X = 0.0;
	Double_t fBox_V2_Y = fWidY * 2.0;
	Double_t fBox_V2_Z = 0.0;
	Double_t fBox_V3_X = std::sin(fAngY * PI / 180.) * fWidZ * 2.0;
	Double_t fBox_V3_Y = 0.0;
	Double_t fBox_V3_Z = std::cos(fAngY * PI / 180.) * fWidZ * 2.0;

	Double_t fDir_Z_X = std::sin(fAngY * PI / 180.);
	Double_t fDir_Z_Y = 0.0;
	Double_t fDir_Z_Z = std::cos(fAngY * PI / 180.);

	Double_t fBox_X = fPosX - std::cos(fAngY * PI / 180.) * fWidX + fDir_Z_X
			* fRad;
	Double_t fBox_Y = fPosY - fWidY + fDir_Z_Y * fRad;
	Double_t fBox_Z = fPosZ + std::sin(fAngY * PI / 180.) * fWidX + fDir_Z_Z
			* fRad;

	if (fVerbosity > 1) {
		cout << "Box Volume: " << fVolName << endl;
		cout << "Angle Y:  " << fAngY << endl;
		cout << "Vectors12:" << fBox_V1_X * fBox_V2_X + fBox_V1_Y * fBox_V2_Y
				+ fBox_V1_Z * fBox_V2_Z << endl;
		cout << "Vectors13:" << fBox_V1_X * fBox_V3_X + fBox_V1_Y * fBox_V3_Y
				+ fBox_V1_Z * fBox_V3_Z << endl;
		cout << "Vectors23:" << fBox_V2_X * fBox_V3_X + fBox_V2_Y * fBox_V3_Y
				+ fBox_V2_Z * fBox_V3_Z << endl;
		cout << "sqrtVec11:" << std::sqrt(
				fBox_V1_X * fBox_V1_X + fBox_V1_Y * fBox_V1_Y + fBox_V1_Z
						* fBox_V1_Z) << endl;
		cout << "sqrtVec22:" << std::sqrt(
				fBox_V2_X * fBox_V2_X + fBox_V2_Y * fBox_V2_Y + fBox_V2_Z
						* fBox_V2_Z) << endl;
		cout << "sqrtVec33:" << std::sqrt(
				fBox_V3_X * fBox_V3_X + fBox_V3_Y * fBox_V3_Y + fBox_V3_Z
						* fBox_V3_Z) << endl;
	}

	if (kFormat == 0) {
		fLine << "BOX " << fVolName << "     " << fBox_X << " " << fBox_Y
				<< " " << fBox_Z << endl;
		fLine << "               " << fBox_V1_X << " " << fBox_V1_Y << " "
				<< fBox_V1_Z << endl;
		fLine << "               " << fBox_V2_X << " " << fBox_V2_Y << " "
				<< fBox_V2_Z << endl;
		fLine << "               " << fBox_V3_X << " " << fBox_V3_Y << " "
				<< fBox_V3_Z;
	} else {
		fLine << "BOX " << fVolName << "     " << fBox_X << " " << fBox_Y
				<< " " << fBox_V1_X << " " << fBox_V1_Y << " " << fBox_V1_Z
				<< fBox_V2_X << " " << fBox_V2_Y << " " << fBox_V2_Z
				<< fBox_V3_X << " " << fBox_V3_Y << " " << fBox_V3_Z;
	}

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create cylinder with angle Y and Theta, position in centre, and radius and height given
Int_t Geometry::VolCyl_YTheta(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fRadius, Double_t fHeight, Double_t fAngY, Double_t fAngTheta) {

	stringstream fLine;
	fLine.precision(16);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}
	Double_t fCyl_X = fPosX - std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.) * fHeight / 2.;
	Double_t fCyl_Y = fPosY - std::sin(fAngTheta * PI / 180.) * fHeight / 2.;
	Double_t fCyl_Z = fPosZ - std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.) * fHeight / 2.;

	Double_t fCyl_Hei_X = std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.) * fHeight;
	Double_t fCyl_Hei_Y = std::sin(fAngTheta * PI / 180.) * fHeight;
	Double_t fCyl_Hei_Z = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.) * fHeight;

	if (fVerbosity > 1) {
		cout << "Cyl Volume: " << fVolName << endl;
		cout << "Angle Y:    " << fAngY << endl;
		cout << "Angle Theta:" << fAngTheta << endl;
	}

	if (kFormat == 0) {
		fLine << "RCC " << fVolName << "     " << fCyl_X << " " << fCyl_Y
				<< " " << fCyl_Z << endl;
		fLine << "               " << fCyl_Hei_X << " " << fCyl_Hei_Y << " "
				<< fCyl_Hei_Z << endl;
		fLine << "               " << fRadius;
	} else {
		fLine << "RCC " << fVolName << "     " << fCyl_X << " " << fCyl_Y
				<< " " << fCyl_Z << " " << fCyl_Hei_X << " " << fCyl_Hei_Y
				<< " " << fCyl_Hei_Z << " " << fRadius;
	}

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create cylinder with angle Y and Theta, position in the beginning of Z, and radius and height given
Int_t Geometry::VolCyl_YTheta_bZ(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fRadius, Double_t fHeight, Double_t fAngY, Double_t fAngTheta) {

	stringstream fLine;
	fLine.precision(16);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}
	Double_t fCyl_X = fPosX;
	Double_t fCyl_Y = fPosY;
	Double_t fCyl_Z = fPosZ;
	Double_t fCyl_Hei_X = std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.) * fHeight;
	Double_t fCyl_Hei_Y = std::sin(fAngTheta * PI / 180.) * fHeight;
	Double_t fCyl_Hei_Z = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.) * fHeight;

	if (fVerbosity > 1) {
		cout << "Cyl Volume: " << fVolName << endl;
		cout << "Angle Y:    " << fAngY << endl;
		cout << "Angle Theta:" << fAngTheta << endl;
	}

	if (kFormat == 0) {
		fLine << "RCC " << fVolName << "     " << fCyl_X << " " << fCyl_Y
				<< " " << fCyl_Z << endl;
		fLine << "               " << fCyl_Hei_X << " " << fCyl_Hei_Y << " "
				<< fCyl_Hei_Z << endl;
		fLine << "               " << fRadius;
	} else {
		fLine << "RCC " << fVolName << "     " << fCyl_X << " " << fCyl_Y
				<< " " << fCyl_Z << " " << fCyl_Hei_X << " " << fCyl_Hei_Y
				<< " " << fCyl_Hei_Z << " " << fRadius;
	}

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create plane with angle Y (direction x-z plane) and theta (azimutal from Y (angle measured: from z to x)), angles in degree
//for Y and theta=0 the plane is in x-y and the vector is pointing to z positive
Int_t Geometry::VolPla(TString fVolName, Int_t fVerbosity, TString* fFLUKALine,
		Double_t fPosX, Double_t fPosY, Double_t fPosZ, Double_t fAngY,
		Double_t fAngTheta) {

	stringstream fLine;
	fLine.precision(16);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}

	Double_t fPla_V_X = std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);
	Double_t fPla_V_Y = std::sin(fAngTheta * PI / 180.);
	Double_t fPla_V_Z = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);

	if (fVerbosity > 1) {
		cout << "Plane Volume:" << fVolName << endl;
		cout << "Angle Y:     " << fAngY << endl;
		cout << "Angle theta: " << fAngTheta << endl;
		cout << "sqrtVec11:"
				<< std::sqrt(
						fPla_V_X * fPla_V_X + fPla_V_Y * fPla_V_Y + fPla_V_Z
								* fPla_V_Z) << endl;
		if (fVerbosity > 2) {
			cout << "fPla_V_X:     " << fPla_V_X << endl;
			cout << "fPla_V_Y:     " << fPla_V_Y << endl;
			cout << "fPla_V_Z:     " << fPla_V_Z << endl;
			cout << "fPosX:        " << fPosX << endl;
			cout << "fPosY:        " << fPosY << endl;
			cout << "fPosZ:        " << fPosZ << endl;
		}
	}

	fLine << "PLA " << fVolName << "     " << fPla_V_X << " " << fPla_V_Y
			<< " " << fPla_V_Z << endl;
	fLine << "                 " << fPosX << " " << fPosY << " " << fPosZ;

	*fFLUKALine = fLine.str();
	return 0;
}

// create plane with angle Y (direction x-z plane) and theta (azimutal from Y (angle measured: from z to x)), angles in degree
//for Y and theta=0 the plane is in y-z and the vector is pointing to x positive
Int_t Geometry::VolPla_X(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fAngY, Double_t fAngTheta) {

	stringstream fLine;
	fLine.precision(16);

	//max 6 letters in vol name
	if (fVolName.Sizeof() > 6) {
		fVolName.Resize(6);
	}

	Double_t fPla_V_X = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);
	Double_t fPla_V_Y = std::sin(fAngTheta * PI / 180.);
	Double_t fPla_V_Z = -std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);

	if (fVerbosity > 1) {
		cout << "Plane Volume:" << fVolName << endl;
		cout << "Angle Y:     " << fAngY << endl;
		cout << "Angle theta: " << fAngTheta << endl;
		cout << "sqrtVec11:"
				<< std::sqrt(
						fPla_V_X * fPla_V_X + fPla_V_Y * fPla_V_Y + fPla_V_Z
								* fPla_V_Z) << endl;
		if (fVerbosity > 2) {
			cout << "fPla_V_X:     " << fPla_V_X << endl;
			cout << "fPla_V_Y:     " << fPla_V_Y << endl;
			cout << "fPla_V_Z:     " << fPla_V_Z << endl;
			cout << "fPosX:        " << fPosX << endl;
			cout << "fPosY:        " << fPosY << endl;
			cout << "fPosZ:        " << fPosZ << endl;
		}
	}

	fLine << "PLA " << fVolName << "     " << fPla_V_X << " " << fPla_V_Y
			<< " " << fPla_V_Z << endl;
	fLine << "                 " << fPosX << " " << fPosY << " " << fPosZ;

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create trapezoid with angle Y (direction in x-z plane) and theta (azimutal from Y), angles in degree
// x1,x2 and y1,y2 are the minimum and maximum half lengths, z1 is the half length in z, z0 is the distance to the rotation origin, position in centre in X,Y and Z
//for Y and theta=0 the the direction vector is pointing to z positive
//shift by a distance fRad, in direction defined by the angles
//half length: fLenX1,fLenX2, fLenY1,fLenY2,fLenZ1
Int_t Geometry::VolTrap_YTheta(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fAngY, Double_t fAngTheta, Double_t fRad, Double_t fLenX1,
		Double_t fLenX2, Double_t fLenY1, Double_t fLenY2, Double_t fLenZ1) {

	//transformed system
	Double_t fDir_X_X = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);
	Double_t fDir_X_Y = std::sin(fAngTheta * PI / 180.);
	Double_t fDir_X_Z = -std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);

	//TODO:check if no minus missing!
	Double_t fDir_Y_X = std::sin(fAngY * PI / 180.) * std::sin(
			fAngTheta * PI / 180.);
	Double_t fDir_Y_Y = std::cos(fAngTheta * PI / 180.);
	Double_t fDir_Y_Z = std::cos(fAngY * PI / 180.) * std::sin(
			fAngTheta * PI / 180.);

	Double_t fDir_Z_X = std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);
	Double_t fDir_Z_Y = std::sin(fAngTheta * PI / 180.);
	Double_t fDir_Z_Z = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);

	//shift so that the position is at the beginning:
	Double_t fPosXNew = fPosX - fLenZ1 * fDir_Z_X;
	Double_t fPosYNew = fPosY - fLenZ1 * fDir_Z_Y;
	Double_t fPosZNew = fPosZ - fLenZ1 * fDir_Z_Z;

	VolTrap_YTheta_bZ(fVolName, fVerbosity, fFLUKALine, fPosXNew, fPosYNew,
			fPosZNew, fAngY, fAngTheta, fRad, fLenX1, fLenX2, fLenY1, fLenY2,
			fLenZ1);

	return 0;
}

/*-----------------------------------------------------------------*/
// create trapezoid with angle Y (direction in x-z plane) and theta (azimutal from Y), angles in degree
// x1,x2 and y1,y2 are the minimum and maximum half lengths, z1 is the half length in z, z0 is the distance to the rotation origin, position in centre in X,Y and at beginning of Z
//for Y and theta=0 the the direction vector is pointing to z positive
//shift by a distance fRad, in direction defined by the angles
//half length: fLenX1,fLenX2, fLenY1,fLenY2,fLenZ1
Int_t Geometry::VolTrap_YTheta_bZ(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fAngY, Double_t fAngTheta, Double_t fRad, Double_t fLenX1,
		Double_t fLenX2, Double_t fLenY1, Double_t fLenY2, Double_t fLenZ1) {

	stringstream fLine;
	fLine.precision(16);

	//max 4 letters in vol name
	if (fVolName.Sizeof() > 4) {
		fVolName.Resize(4);
	}

	TString fVolNameSub;
	TString fFLUKACard;

	//transformed system
	Double_t fDir_X_X = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);
	Double_t fDir_X_Y = std::sin(fAngTheta * PI / 180.);
	Double_t fDir_X_Z = -std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);

	//TODO:check if no minus missing!
	Double_t fDir_Y_X = std::sin(fAngY * PI / 180.) * std::sin(
			fAngTheta * PI / 180.);
	Double_t fDir_Y_Y = std::cos(fAngTheta * PI / 180.);
	Double_t fDir_Y_Z = std::cos(fAngY * PI / 180.) * std::sin(
			fAngTheta * PI / 180.);

	Double_t fDir_Z_X = std::sin(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);
	Double_t fDir_Z_Y = std::sin(fAngTheta * PI / 180.);
	Double_t fDir_Z_Z = std::cos(fAngY * PI / 180.) * std::cos(
			fAngTheta * PI / 180.);

	if (fVerbosity > 1) {
		cout << "Trapezoid Volume:" << fVolName << "_<S,E,L,R,T,B>" << endl;
		cout << "Angle Y:     " << fAngY << endl;
		cout << "Angle theta: " << fAngTheta << endl;
		cout << "Radius:      " << fRad << endl;
		cout << "Vectors12:" << fDir_X_X * fDir_Y_X + fDir_X_Y * fDir_Y_Y
				+ fDir_X_Z * fDir_Y_Z << endl;
		cout << "Vectors13:" << fDir_X_X * fDir_Z_X + fDir_X_Y * fDir_Z_Y
				+ fDir_X_Z * fDir_Z_Z << endl;
		cout << "Vectors23:" << fDir_Y_X * fDir_Z_X + fDir_Y_Y * fDir_Z_Y
				+ fDir_Y_Z * fDir_Z_Z << endl;
		cout << "sqrtVec11:"
				<< std::sqrt(
						fDir_X_X * fDir_X_X + fDir_X_Y * fDir_X_Y + fDir_X_Z
								* fDir_X_Z) << endl;
		cout << "sqrtVec22:"
				<< std::sqrt(
						fDir_Y_X * fDir_Y_X + fDir_Y_Y * fDir_Y_Y + fDir_Y_Z
								* fDir_Y_Z) << endl;
		cout << "sqrtVec33:"
				<< std::sqrt(
						fDir_Z_X * fDir_Z_X + fDir_Z_Y * fDir_Z_Y + fDir_Z_Z
								* fDir_Z_Z) << endl;
		if (fVerbosity > 1) {
			cout << "Radius:      " << fRad << endl;
			cout << "fDir_Z_X*fRad" << fDir_Z_X * fRad << endl;
			cout << "fDir_Z_Y*fRad" << fDir_Z_Y * fRad << endl;
			cout << "fDir_Z_Z*fRad" << fDir_Z_Z * fRad << endl;
			cout << "fPosX" << fPosX << endl;
			cout << "fPosY" << fPosY << endl;
			cout << "fPosZ" << fPosZ << endl;
		}
	}

	Geometry::VolPla(fVolNameSub = fVolName + "_S", fVerbosity, &fFLUKACard,
			fPosX + fDir_Z_X * fRad, fPosY + fDir_Z_Y * fRad,
			fPosZ + fDir_Z_Z * fRad, fAngY + 180.0, fAngTheta);
	fLine << fFLUKACard << endl;

	Geometry::VolPla(fVolNameSub = fVolName + "_E", fVerbosity, &fFLUKACard,
			fPosX + fDir_Z_X * (fRad + fLenZ1 * 2.0),
			fPosY + fDir_Z_Y * (fRad + fLenZ1 * 2.0),
			fPosZ + fDir_Z_Z * (fRad + fLenZ1 * 2.0), fAngY, fAngTheta);
	fLine << fFLUKACard << endl;

	Geometry::VolPla(
			fVolNameSub = fVolName + "_L",
			fVerbosity,
			&fFLUKACard,
			fPosX + fDir_X_X * fLenX1 + fDir_Z_X * fRad,
			fPosY + fDir_X_Y * fLenX1 + fDir_Z_Y * fRad,
			fPosZ + fDir_X_Z * fLenX1 + fDir_Z_Z * fRad,
			fAngY + 90.0 + std::atan((fLenX2 - fLenX1) / (2.0 * fLenZ1)) / PI
					* 180., fAngTheta);
	fLine << fFLUKACard << endl;

	Geometry::VolPla(
			fVolNameSub = fVolName + "_R",
			fVerbosity,
			&fFLUKACard,
			fPosX - fDir_X_X * fLenX1 + fDir_Z_X * fRad,
			fPosY - fDir_X_Y * fLenX1 + fDir_Z_Y * fRad,
			fPosZ - fDir_X_Z * fLenX1 + fDir_Z_Z * fRad,
			fAngY - 90.0 - std::atan((fLenX2 - fLenX1) / (2.0 * fLenZ1)) / PI
					* 180., fAngTheta);
	fLine << fFLUKACard << endl;

	Geometry::VolPla(
			fVolNameSub = fVolName + "_T",
			fVerbosity,
			&fFLUKACard,
			fPosX + fDir_Y_X * fLenY1 + fDir_Z_X * fRad,
			fPosY + fDir_Y_Y * fLenY1 + fDir_Z_Y * fRad,
			fPosZ + fDir_Y_Z * fLenY1 + fDir_Z_Z * fRad,
			fAngY,
			fAngTheta + 90.0 + std::atan((fLenY2 - fLenY1) / (2.0 * fLenZ1))
					/ PI * 180.);
	fLine << fFLUKACard << endl;

	Geometry::VolPla(
			fVolNameSub = fVolName + "_B",
			fVerbosity,
			&fFLUKACard,
			fPosX - fDir_Y_X * fLenY1 + fDir_Z_X * fRad,
			fPosY - fDir_Y_Y * fLenY1 + fDir_Z_Y * fRad,
			fPosZ - fDir_Y_Z * fLenY1 + fDir_Z_Z * fRad,
			fAngY,
			fAngTheta - 90.0 - std::atan((fLenY2 - fLenY1) / (2.0 * fLenZ1))
					/ PI * 180.);
	fLine << fFLUKACard;

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create EVENTBIN with position in centre, and half length given
//WATCHOUT: number precision goes to 0.1um
Int_t Geometry::ScoEVENTBIN(TString fScoName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fWidX, Double_t fWidY, Double_t fWidZ, Int_t fNbX, Int_t fNbY,
		Int_t fNbZ, Int_t fType, TString fScoring, Int_t fUnit) {

	stringstream fLine;
	//  fLine.precision(16);
	fLine << setiosflags(ios::fixed) << setprecision(5);

	//max 10 letters in scoring name
	if (fScoName.Sizeof() > 10) {
		fScoName.Resize(10);
	}
	Double_t fBox_X = fPosX - fWidX;
	Double_t fBox_Y = fPosY - fWidY;
	Double_t fBox_Z = fPosZ - fWidZ;

	if (fVerbosity > 1) {
		cout << "Box Volume: " << fScoName << endl;
	}

	//   EVENTBIN       -10.0    ENERGY     -38.0      1.06    -0.175    16.702eventbin1
	//   EVENTBIN       -1.06    -1.235      16.7    1152.0     576.0       1.0&
	//       "* 23456789 *23456789 *23456789 *23456789 *23456789 *23456789 *23456789
	fLine << "EVENTBIN  " << setw(10) << fType << setw(10) << fScoring << setw(
			10) << fUnit << setw(10) << fBox_X + 2. * fWidX << setw(10)
			<< fBox_Y + 2. * fWidY << setw(10) << fBox_Z + 2. * fWidZ
			<< fScoName << endl;
	fLine << "EVENTBIN  " << setw(10) << fBox_X << setw(10) << fBox_Y << setw(
			10) << fBox_Z << setw(10) << fNbX << setw(10) << fNbY << setw(10)
			<< fNbZ << "&";

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
// create genertic FLUKA card: TITLE WHAT(1-6) SDUM
Int_t Geometry::GenericCard(TString fTitle, Int_t fVerbosity,
		TString* fFLUKALine, TString fWHAT1, TString fWHAT2, TString fWHAT3,
		TString fWHAT4, TString fWHAT5, TString fWHAT6, TString fSDUM) {

	stringstream fLine;
	//fLine.precision(16)
	//fLine << setiosflags(ios::fixed) << setprecision(3);

	//max 10 letters
	if (fTitle.Sizeof() != 10) {
		fTitle.Resize(10);
	}
	//max 10 letters
	if (fSDUM.Sizeof() != 10) {
		fSDUM.Resize(10);
	}

	if (fWHAT1.Sizeof() != 10) {
		fWHAT1.Resize(10);
	}
	if (fWHAT2.Sizeof() != 10) {
		fWHAT2.Resize(10);
	}
	if (fWHAT3.Sizeof() != 10) {
		fWHAT3.Resize(10);
	}
	if (fWHAT4.Sizeof() != 10) {
		fWHAT4.Resize(10);
	}
	if (fWHAT5.Sizeof() != 10) {
		fWHAT5.Resize(10);
	}
	if (fWHAT6.Sizeof() != 10) {
		fWHAT6.Resize(10);
	}

	if (fVerbosity > 1) {
		//    cout<<"Box Volume: "<<fScoName<<endl;
	}

	//       "* 23456789 *23456789 *23456789 *23456789 *23456789 *23456789 *23456789
	fLine << setw(10) << fTitle << setw(10) << fWHAT1 << setw(10) << fWHAT2
			<< setw(10) << fWHAT3 << setw(10) << fWHAT4 << setw(10) << fWHAT5
			<< setw(10) << fWHAT6 << setw(10) << fSDUM;

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
//TOF Wall parameterization for slabs, fNbOfSlats= number of slabs in each down and up part 
Int_t Geometry::TW_SlatParam(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fAngY, Double_t fXprime, Double_t fYprime, Double_t fZprime,
		Double_t fStartX, Double_t fSlatWidth, Int_t fNbOfSlats, Int_t fBS[100]) {

	stringstream fLine;
	TString fFLUKACard;

	TString fFLUKAName;
	Int_t fBadFlag;
	Double_t fXprimeS;
	Double_t fYprimeS;
	Double_t fZprimeS;

	Double_t fXPosition;

	//max 4 letters in vol name
	if (fVolName.Sizeof() > 4) {
		fVolName.Resize(4);
	}

	if (fVerbosity > 1) {
		cout << "fNbOfSlats " << fNbOfSlats << endl;
	}

	for (Int_t ii = 0; ii < fNbOfSlats; ii++) {

		//  FLUKAname=TString::Format("TWFL%d",ii);
		fFLUKAName = fVolName + TString::Format("%d", ii);

		fBadFlag = 0;
		for (Int_t jj = 0; jj < 100; jj++) {
			if (ii == fBS[jj])
				fBadFlag = 1;
		}

		if (fBadFlag == 1) {
			continue;
		} else if (ii < (fNbOfSlats)) {
			fXPosition = +fStartX - (ii) * (fSlatWidth) - fSlatWidth / 2.;
		}

		if (ii != 0) {
			fLine << fFLUKACard << endl;
		}
		fXprimeS = fXprime + fXPosition + fSlatWidth / 2.;
		fYprimeS = fYprime;
		fZprimeS = fZprime;

		Geometry::VolPla(
				fFLUKAName,
				kVerbosity,
				&fFLUKACard,
				0.5*(m_TWR_center.X()+m_TWF_center.X()) + std::cos(m_TWR_angles.X() * PI / 180.) * (fXprimeS)
						+ std::sin(m_TWR_angles.X() * PI / 180.) * (fZprimeS),
				0.5*(m_TWR_center.Y()+m_TWF_center.Y()) + fYprimeS,
				0.5*(m_TWR_center.Z()+m_TWF_center.Z()) - std::sin(m_TWR_angles.X() * PI / 180.) * (fXprimeS)
						+ std::cos(m_TWR_angles.X() * PI / 180.) * (fZprimeS),
				m_TWR_angles.X() + 90., 0.);
	}
	fLine << fFLUKACard;

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
//TOF Wall parameterization for slabs, fNbOfSlats= number of slabs in each down and up part, as BOX
Int_t Geometry::TW_SlatParamBOX(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY, Double_t fPosZ,
		Double_t fAngY, Double_t fXprime, Double_t fYprime, Double_t fZprime,
		Double_t fStartX, Double_t fSlatX, Double_t fSlatY, Double_t fSlatZ,
		Double_t fSlatSpaceX, Double_t fModuleSpaceX, Int_t fNbOfSlats,
		Int_t fBS[100]) {

	stringstream fLine;
	TString fFLUKACard;

	TString fFLUKAName;
	Int_t fBadFlag;
	Double_t fXprimeS;
	Double_t fYprimeS;
	Double_t fZprimeS;

	Double_t fXPosition;

	//max 4 letters in vol name
	if (fVolName.Sizeof() > 4) {
		fVolName.Resize(4);
	}

	if (fVerbosity > 1) {
		cout << "fNbOfSlats " << fNbOfSlats << endl;
	}

	for (Int_t ii = 0; ii < fNbOfSlats; ii++) {

		fBadFlag = 0;
		for (Int_t jj = 0; jj < 100; jj++) {
			if (ii == fBS[jj])
				fBadFlag = 1;
		}

		if (fBadFlag == 1) {
			continue;
		}
		fXPosition = +fStartX - (float) (ii) * (fSlatX + fSlatSpaceX)
				- (float) (ii / 8) * (fModuleSpaceX - fSlatSpaceX) - fSlatX
				/ 2.;

		if (ii != 0) {
			fLine << fFLUKACard << endl;
		}

		fXprimeS = fXprime + fXPosition;
		fYprimeS = fYprime;
		fZprimeS = fZprime;

		fFLUKAName = fVolName + TString::Format("%d", ii);

		Geometry::VolBox_Y(
				fFLUKAName,
				kVerbosity,
				&fFLUKACard,
				fPosX + std::cos(fAngY * PI / 180.) * (fXprimeS) + std::sin(
						fAngY * PI / 180.) * (fZprimeS),
				fPosY + fYprimeS,
				fPosZ - std::sin(fAngY * PI / 180.) * (fXprimeS) + std::cos(
						fAngY * PI / 180.) * (fZprimeS),
				fSlatX / 2. - 2. * kAvoidOverlap1, fSlatY / 2., fSlatZ / 2.,
				fAngY, 0.0);

		/*
		 fLine<<fFLUKACard<<endl;
		 //down
		 fFLUKAName=fVolName+TString::Format("%d",(fNbOfSlats+ii));
		 fYprimeS=fYprime-27.5;
		 Geometry::VolBox_Y(fFLUKAName,kVerbosity,&fFLUKACard
		 ,TW_X+std::cos(m_TWR_angles.X()*PI/180.)*(fXprimeS)+std::sin(m_TWR_angles.X()*PI/180.)*(fZprimeS)
		 ,TW_Y+fYprimeS
		 ,TW_Z-std::sin(m_TWR_angles.X()*PI/180.)*(fXprimeS)+std::cos(m_TWR_angles.X()*PI/180.)*(fZprimeS)
		 ,fSlatX/2.
		 ,fSlatY/2.
		 ,fSlatZ/2.
		 ,m_TWR_angles.X());*/
	}
	fLine << fFLUKACard;

	*fFLUKALine = fLine.str();
	return 0;
}

/*-----------------------------------------------------------------*/
//Onion ring parameterization
Int_t Geometry::TW_RingParam(TString fVolName, Int_t fVerbosity,
		TString* fFLUKALine, Double_t fPosX, Double_t fPosY,
		Double_t fPosZ,
		Double_t fAngY
		//	,Double_t fAngTheta
		, Double_t fXprime, Double_t fYprime, Double_t fZprime, Double_t fdR,
		Double_t fDepth
		//	,Double_t fThicknessSpace // distance between rings
		, Int_t fNbOfRings) {

	stringstream fLine;
	TString fFLUKACard;

	TString fFLUKAName;
	Double_t fXprimeS;
	Double_t fYprimeS;
	Double_t fZprimeS;

	Double_t fRadius;

	//max 4 letters in vol name
	if (fVolName.Sizeof() > 4) {
		fVolName.Resize(4);
	}

	if (fVerbosity > 1) {
		cout << "fNbOfRings " << fNbOfRings << endl;
	}

	for (Int_t ii = 0; ii < fNbOfRings; ii++) {

		fRadius = (float) (ii + 1) * (fdR);

		if (ii != 0) {
			fLine << fFLUKACard << endl;
		}

		fXprimeS = fXprime;
		fYprimeS = fYprime;
		fZprimeS = fZprime;

		fFLUKAName = fVolName + TString::Format("%d", ii);

		Geometry::VolCyl_YTheta(
				fFLUKAName,
				kVerbosity,
				&fFLUKACard,
				fPosX + std::cos(fAngY * PI / 180.) * (fXprimeS) + std::sin(
						fAngY * PI / 180.) * (fZprimeS),
				fPosY + fYprimeS,
				fPosZ - std::sin(fAngY * PI / 180.) * (fXprimeS) + std::cos(
						fAngY * PI / 180.) * (fZprimeS), fRadius,
				fNbOfRings == ii + 1 ? fDepth : fDepth + 1.0 //make inner cyls thicker (to avoid empty spaces in FLUKA)
				, fAngY, 0.);

	}
	fLine << fFLUKACard;

	*fFLUKALine = fLine.str();
	return 0;
}

/*--------------------------------------------------------*/
//given a pad in a chip return the position in FLUKA coordinates WARNING: chip is counting from 0:
TVector3 Geometry::MI_Coord(Double_t Column, Double_t Row, Int_t Chip) {

  TVector3 Pos;
  //AS::Used for internal use only....
  //Has to be Xchecked against preprocessing rotations.
  double x_pos = 
    -(geo.Mimo.MIMO_SHIFT[Chip].X() - geo.Mimo.SENS_DIM.X() / 2.
      + geo.Mimo.PAD_DIM.X() * (geo.Mimo.SENS_BIN[0] - Column
				+ 0.5));
  double y_pos = 
	  geo.Mimo.MIMO_SHIFT[Chip].Y() - geo.Mimo.SENS_DIM.Y() / 2.
    + geo.Mimo.PAD_DIM.Y() * (geo.Mimo.SENS_BIN[1] - Row - 0.5);
  
  Pos.SetXYZ(x_pos,y_pos,geo.Mimo.MIMO_POS_Z[Chip]);
  
  return Pos;
}

/*--------------------------------------------------------*/
//given a pad in a chip return the index for reconstruction (C. Finck)
Int_t Geometry::MI_IndexRec(Double_t Column, Double_t Row) {
  
  //  aPixel->SetPixelIndex(aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn());
  //fStripsNu=1152
  return Row * MI_SENS_BIN_WIDTH + Column;
}

/*--------------------------------------------------------*/
//given a pad in a chip return the position in coordinates for reconstruction (C. Finck)
TVector3 Geometry::MI_CoordRec(Double_t Column, Double_t Row, Int_t Chip) {
  TVector3 Pos;

  //AS: modified it in order to have matching with data convention
  //Before use check the  x<->y prescriptio, row, column and their numbering
  //Not used in decoding, only for fluka internal checking
  double x_pos, y_pos;
  x_pos = (2. * Column - MI_SENS_BIN_WIDTH + 1.) * MI_PAD_WIDTH / 2.; //UNITS: cm (x)
  y_pos = (2. * Row - MI_SENS_BIN_HEIGHT + 1.) * MI_PAD_HEIGHT / 2.; //UNITS: xm (y)
  double z_pos = 0. * Chip; //UNITS: mm (z)
  Pos.SetXYZ(x_pos, y_pos, z_pos);
  
  return Pos;
}

//for (Int_t tci = 0; tci < fPixelsN; tci++) { // loop over hit pixels
//aPixel = (DPixel*)fListOfPixels->At(tci);
//aPixel->SetPixelIndex(aPixel->GetPixelLine()*fStripsNu + aPixel->GetPixelColumn());
//u = ((2*aPixel->GetPixelColumn() - fStripsNu + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(0))/2 ;
//v = ((2*aPixel->GetPixelLine() - fStripsNv + 1 ) * fc->GetPlanePar(fPlaneNumber).Pitch(1))/2 ;
//w = fc->GetPlanePar(fPlaneNumber).Pitch(2);
//} // end loop over hit pixels
//Column==Column: StripsU: 1152 Row==Line: StripsV: 576 StripsW: 1
//PitchU: 0000.0184 PitchV: 0000.0184 PitchW: 0000.000


/*--------------------------------------------------------*/
//convert Double_t to Int_t without crashing - hell knows why (deadly FORTRAN-C++ mix?).
Int_t Geometry::castd2i(Double_t number) {
	Double_t top = 2147483647., bottom = -2147483648.;

	//check out-of-bounds
	if (number >= top) {
		return 2147483647;
	} else if (number <= bottom) {
		return -2147483648;
	} else {
		return (int) number;
	}
}

/*-----------------------------------------------------------------*/
Geometry::~Geometry() {
}


void Geometry::InitVtxPlanes() {

  VtPlanesZ[0] = 0.7+m_VT_center.Z()+MI_GLAFI_THICK/2.;      //middle of plane
  VtPlanesZ[1] = 0.7+m_VT_center.Z()+MI_GLAFI_THICK/2.+1.*MI_DIST_PLANES; 
  VtPlanesZ[2] = 0.7+m_VT_center.Z()+MI_GLAFI_THICK/2.+2.*MI_DIST_PLANES; 
  VtPlanesZ[3] = 0.7+m_VT_center.Z()+MI_GLAFI_THICK/2.+3.*MI_DIST_PLANES; 
  MI_THICK = (VtPlanesZ[3]-VtPlanesZ[0])+MI_GLAFI_THICK+0.1;
  return;
}

TVector3 Geometry::GetSCCenter()
{
  return m_SC_center;
}

void Geometry::SetSCCenter(TVector3 cen)
{
  m_SC_center = cen;
  return;
}

void Geometry::SetSCCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_SC_center = cen;
  return;
}

TVector3 Geometry::GetSCAngles()
{
  return m_SC_angles;
}

void Geometry::SetSCAngles(TVector3 ang)
{
  m_SC_angles = ang;
  return;
}

void Geometry::SetSCAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_SC_angles = ang;
  return;
}

TVector3 Geometry::GetBMCenter()
{
  return m_BM_center;
}

void Geometry::SetBMCenter(TVector3 cen)
{
  m_BM_center = cen;
  return;
}

void Geometry::SetBMCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_BM_center = cen;
  return;
}

TVector3 Geometry::GetBMAngles()
{
  return m_BM_angles;
}

void Geometry::SetBMAngles(TVector3 ang)
{
  m_BM_angles = ang;
  return;
}

void Geometry::SetBMAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_BM_angles = ang;
  return;
}

TVector3 Geometry::GetVTCenter()
{
  return m_VT_center;
}

void Geometry::SetVTCenter(TVector3 cen)
{
  m_VT_center = cen;
  return;
}

void Geometry::SetVTCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_VT_center = cen;
  return;
}

TVector3 Geometry::GetVTAngles()
{
  return m_VT_angles;
}

void Geometry::SetVTAngles(TVector3 ang)
{
  m_VT_angles = ang;
  return;
}

void Geometry::SetVTAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_VT_angles = ang;
  return;
}

TVector3 Geometry::GetKECenter()
{
  return m_KE_center;
}

void Geometry::SetKECenter(TVector3 cen)
{
  m_KE_center = cen;
  return;
}

void Geometry::SetKECenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_KE_center = cen;
  return;
}

TVector3 Geometry::GetKEAngles()
{
  return m_KE_angles;
}

void Geometry::SetKEAngles(TVector3 ang)
{
  m_KE_angles = ang;
  return;
}

void Geometry::SetKEAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_KE_angles = ang;
  return;
}

TVector3 Geometry::GetALCenter()
{
  return m_AL_center;
}

void Geometry::SetALCenter(TVector3 cen)
{
  m_AL_center = cen;
  return;
}

void Geometry::SetALCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_AL_center = cen;
  return;
}

TVector3 Geometry::GetALAngles()
{
  return m_AL_angles;
}

void Geometry::SetALAngles(TVector3 ang)
{
  m_AL_angles = ang;
  return;
}

void Geometry::SetALAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_AL_angles = ang;
  return;
}

TVector3 Geometry::GetMUCenter()
{
  return m_MU_center;
}

void Geometry::SetMUCenter(TVector3 cen)
{
  m_MU_center = cen;
  return;
}

void Geometry::SetMUCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_MU_center = cen;
  return;
}

TVector3 Geometry::GetMUAngles()
{
  return m_MU_angles;
}

void Geometry::SetMUAngles(TVector3 ang)
{
  m_MU_angles = ang;
  return;
}

void Geometry::SetMUAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_MU_angles = ang;
  return;
}

// declared and calling new reference point


TVector3 Geometry::GetNUCenter()
{
  return m_NU_center;
}

void Geometry::SetNUCenter(TVector3 cen)
{
  m_NU_center = cen;
  return;
}

void Geometry::SetNUCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_NU_center = cen;
  return;
}

TVector3 Geometry::GetNUAngles()
{
  return m_NU_angles;
}

void Geometry::SetNUAngles(TVector3 ang)
{
  m_NU_angles = ang;
  return;
}

void Geometry::SetNUAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_NU_angles = ang;
  return;
}

// end of new reference point

TVector3 Geometry::GetTWRCenter()
{
  return m_TWR_center;
}

void Geometry::SetTWRCenter(TVector3 cen)
{
  m_TWR_center = cen;
  return;
}

void Geometry::SetTWRCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_TWR_center = cen;
  return;
}

TVector3 Geometry::GetTWRAngles()
{
  return m_TWR_angles;
}

void Geometry::SetTWRAngles(TVector3 ang)
{
  m_TWR_angles = ang;
  return;
}

void Geometry::SetTWRAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_TWR_angles = ang;
  return;
}


TVector3 Geometry::GetTWFCenter()
{
  return m_TWF_center;
}

void Geometry::SetTWFCenter(TVector3 cen)
{
  m_TWF_center = cen;
  return;
}

void Geometry::SetTWFCenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_TWF_center = cen;
  return;
}

TVector3 Geometry::GetTWFAngles()
{
  return m_TWF_angles;
}

void Geometry::SetTWFAngles(TVector3 ang)
{
  m_TWF_angles = ang;
  return;
}

void Geometry::SetTWFAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_TWF_angles = ang;
  return;
}

TVector3 Geometry::GetVECenter()
{
  return m_VE_center;
}

void Geometry::SetVECenter(TVector3 cen)
{
  m_VE_center = cen;
  return;
}

void Geometry::SetVECenter(double x_c, double y_c, double z_c)
{
  TVector3 cen(x_c,y_c,z_c);
  m_VE_center = cen;
  return;
}

TVector3 Geometry::GetVEAngles()
{
  return m_VE_angles;
}

void Geometry::SetVEAngles(TVector3 ang)
{
  m_VE_angles = ang;
  return;
}

void Geometry::SetVEAngles(double x_d, double y_d, double z_d)
{
  TVector3 ang(x_d,y_d,z_d);
  m_VE_angles = ang;
  return;
}

TVector3 Geometry::GetIP()
{
  return m_ip;
}

void Geometry::SetIP(TVector3 ip)
{
  m_ip = ip;
  return;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************
