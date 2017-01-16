#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <sstream>

#include "Geometry.h"

using namespace std;

//ofstream outfile;
static Geometry *TheGeometry = 0;
GEOMETRY_STRUCT geo;


int main(int argc, char *argv[]){ 

  char stringa[300], nomefile[50], regname[20];
  string line;
  size_t nfound;

  TString FLUKAcard;
  TString FLUKAname;

  stringstream genstg;

  Int_t iReg;
  Int_t iCellBMN, iSenseBMN, iFieldBMN;
  Int_t iCellDCH, iSenseDCH, iFieldDCH;
  Int_t nCry, nScnV, nScnU;

  Double_t x, y;
  
  genstg<<"* ******************************************************************************"<<endl;
  genstg<<"*                           GENERAL & PRIMARY                                  *"<<endl;
  genstg<<"* ******************************************************************************"<<endl;

  genstg<<"* -------1---------2---------3---------4---------5---------6---------7---------8"<<endl;
  genstg<<"* command| what(1) | what(2) | what(3) | what(4) | what(5) | what(6) | SDUM    |"<<endl;
  genstg<<"* -------1---------2---------3---------4---------5---------6---------7---------8"<<endl;
  
  // Primary properties
  
  TString BeamParticle;
  
  if (PRIM_Type==-2){
    BeamParticle="HEAVYION";
  } else if (PRIM_Type==1){
    BeamParticle="PROTON";
  } else {    
    cerr << "ERROR: Wrong particle type:" << PRIM_Type << endl;
    exit(1);
  }
  
  if (PRIM_DIST=="GAUSSIAN"){
    TheGeometry->GenericCard("BEAM",kVerbosity,&FLUKAcard
			     ,TString::Format("%f",-PRIM_T),TString::Format("%f",PRIM_dP)
			     ,TString::Format("%.3f",PRIM_DIV),TString::Format("%f",-PRIM_RMAX)
			     ,TString::Format("%f",-PRIM_RMAX),"1.0",BeamParticle);
  } else if (PRIM_DIST=="ANNULAR"){
    TheGeometry->GenericCard("BEAM",kVerbosity,&FLUKAcard
			     ,TString::Format("%f",-PRIM_T),TString::Format("%f",PRIM_dP)
			     ,TString::Format("%.3f",PRIM_DIV), TString::Format("%f",PRIM_RMAX)
			     , "0.0","-1.0",BeamParticle);
  }
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  if (PRIM_Type==-2) {
    TheGeometry->GenericCard("HI-PROPE",kVerbosity,&FLUKAcard
    			     ,TString::Format("%f",(double)PRIM_Z)
    			     ,TString::Format("%f",(double)PRIM_A),"","","","","");
    if (kVerbosity>0) cout << FLUKAcard << endl;
    genstg << FLUKAcard << endl;
  }
  
  TheGeometry->GenericCard("BEAMPOS",kVerbosity,&FLUKAcard
			   ,TString::Format("%.3f",PRIM_Pos_X),TString::Format("%.3f",PRIM_Pos_Y)
			   ,TString::Format("%.3f",PRIM_Pos_Z), TString::Format("%.3f",PRIM_Pos_CX)
			   ,TString::Format("%.3f",PRIM_Pos_CY),"","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  stringstream matstg;
  matstg<<"* ******************************************************************************"<<endl;
  matstg<<"*                         MEDIA & MAGFIELD                                     *"<<endl;
  matstg<<"* ******************************************************************************"<<endl;

  matstg<<"* -------1---------2---------3---------4---------5---------6---------7---------8"<<endl;
  matstg<<"* command| what(1) | what(2) | what(3) | what(4) | what(5) | what(6) | SDUM    |"<<endl;
  matstg<<"* -------1---------2---------3---------4---------5---------6---------7---------8"<<endl;

  ofstream geofile;
  ofstream regfile;
  ofstream paramfile;
  geofile.open("foot.geo");
  regfile.open("foot.reg");
  paramfile.open("parameters.inc");
  
  if (TheGeometry) {
    delete TheGeometry;
    TheGeometry = 0;
  }

  TheGeometry = new Geometry();

  geo = TheGeometry->GetStruct();

  //END OF INIT
  
  /*
    start writing geo file 
  */
  
  sprintf(stringa,"    0    0          FOOT experiment geometry");
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  geofile << "* ***Black Body" << endl;
  sprintf(stringa,"RPP blk        -10000.1 10000.1 -10000.1 10000.1 -50000.1 50000.1");
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  geofile << "* ***Air -> no mag field" << endl;
  sprintf(stringa,"RPP air        -1000.0 1000.0 -1000.0 1000.0 -5000.0 5000.0");
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  //Start counter

  geofile << "* ***Start Counter" << endl;
  sprintf(stringa,"RCC stc        %f %f %f %f %f %f %f",STC_X,STC_Y,(STC_Z-STC_THICK/2),
	  ZERO,ZERO,STC_THICK,STC_RAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  // Beam Monitor
  
  geofile << "* ***Beam Monitor" << endl;
  sprintf(stringa,"RPP BmnShiOu    %f %f %f %f %f %f",BMN_X-BMN_WIDTH/2.-BMN_SHI_THICK
	  ,BMN_X+BMN_WIDTH/2.+BMN_SHI_THICK,BMN_Y-BMN_HEIGHT/2.-BMN_SHI_THICK
	  ,BMN_Y+BMN_HEIGHT/2.+BMN_SHI_THICK,BMN_Z-BMN_SHI_LENGTH/2.,BMN_Z+BMN_SHI_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"RPP BmnShiIn    %f %f %f %f %f %f",BMN_X-BMN_WIDTH/2.,BMN_X+BMN_WIDTH/2.,
	  BMN_Y-BMN_HEIGHT/2.,BMN_Y+BMN_HEIGHT/2.,BMN_Z-BMN_SHI_LENGTH/2.,BMN_Z+BMN_SHI_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  sprintf(stringa,"XYP BmnMyl0        %f",BMN_Z-BMN_LENGTH/2.-BMN_MYL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP BmnMyl1        %f",BMN_Z-BMN_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP BmnMyl2        %f",BMN_Z+BMN_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP BmnMyl3        %f",BMN_Z+BMN_LENGTH/2.+BMN_MYL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  // Cells 
  iCellBMN=-1;
  for (int il=0;il<BMN_NLAY;il++){
    for (int ic =0; ic<BMN_NSENSELAY;ic++){      
      iCellBMN++;
      sprintf(stringa,"RPP BmnCU%d    %f %f %f %f %f %f",iCellBMN
	      ,geo.Bmon.Uc_xm[ic][il],geo.Bmon.Uc_xM[ic][il]
	      ,geo.Bmon.Uc_ym[ic][il],geo.Bmon.Uc_yM[ic][il]
	      ,geo.Bmon.Uc_zm[ic][il],geo.Bmon.Uc_zM[ic][il]);	
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl;
      
      sprintf(stringa,"RPP BmnCV%d    %f %f %f %f %f %f",iCellBMN
	      ,geo.Bmon.Vc_xm[ic][il],geo.Bmon.Vc_xM[ic][il]
	      ,geo.Bmon.Vc_ym[ic][il],geo.Bmon.Vc_yM[ic][il]
	      ,geo.Bmon.Vc_zm[ic][il],geo.Bmon.Vc_zM[ic][il]);	
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl;      
    }
  }

  // Wires U structure  
  iFieldBMN=-1;
  iSenseBMN=-1;
  for (int il=0;il<BMN_NLAY;il++){
    for (int iw =0; iw<BMN_NWIRELAY;iw++){      
      if ( (iw==geo.Bmon.idsense[0]) ||(iw==geo.Bmon.idsense[1]) ||
	   (iw==geo.Bmon.idsense[2]) ){	
	iSenseBMN++;	
	sprintf(stringa,"RCC BmnUS%d    %f %f %f %f %f %f %f",iSenseBMN
		,geo.Bmon.U_x[iw][il],geo.Bmon.U_y[iw][il],geo.Bmon.U_z[iw][il]
		,geo.Bmon.U_cx[iw][il],geo.Bmon.U_cy[iw][il],geo.Bmon.U_cz[iw][il]
		,geo.Bmon.U_rad[iw][il]);
      	if (kVerbosity>0) cout << stringa << endl;
      	geofile << stringa << endl;
      } else {	
	iFieldBMN++;	 
	sprintf(stringa,"RCC BmnUF%d   %f %f %f %f %f %f %f",iFieldBMN
		,geo.Bmon.U_x[iw][il],geo.Bmon.U_y[iw][il],geo.Bmon.U_z[iw][il]
		,geo.Bmon.U_cx[iw][il],geo.Bmon.U_cy[iw][il],geo.Bmon.U_cz[iw][il]
		,geo.Bmon.U_rad[iw][il]); 
	if (kVerbosity>0) cout << stringa << endl;
	geofile << stringa << endl;
      }
    }
  }
  
  //  Wires V structure
  iFieldBMN=-1;
  iSenseBMN=-1;
  for (int il=0;il<BMN_NLAY;il++){
    for (int iw =0; iw<BMN_NWIRELAY;iw++){
      if ( (iw==geo.Bmon.idsense[0]) ||(iw==geo.Bmon.idsense[1]) ||
	   (iw==geo.Bmon.idsense[2]) ){
	iSenseBMN++;	 
	sprintf(stringa,"RCC BmnVS%d    %f %f %f %f %f %f %f",iSenseBMN
		,geo.Bmon.V_x[iw][il],geo.Bmon.V_y[iw][il],geo.Bmon.V_z[iw][il]
		,geo.Bmon.V_cx[iw][il],geo.Bmon.V_cy[iw][il],geo.Bmon.V_cz[iw][il]
		,geo.Bmon.V_rad[iw][il]);
	if (kVerbosity>0) cout << stringa << endl;
	geofile << stringa << endl;
      } else {
	iFieldBMN++;	 
	sprintf(stringa,"RCC BmnVF%d    %f %f %f %f %f %f %f",iFieldBMN
		,geo.Bmon.V_x[iw][il],geo.Bmon.V_y[iw][il],geo.Bmon.V_z[iw][il]
		,geo.Bmon.V_cx[iw][il],geo.Bmon.V_cy[iw][il],geo.Bmon.V_cz[iw][il]
		,geo.Bmon.V_rad[iw][il]);
	if (kVerbosity>0) cout << stringa <<endl;
	geofile << stringa <<endl;
      }
    }
  }  

  // Target
  
  geofile << "* ***Target" << endl;
  sprintf(stringa,"RPP tgt        %f %f %f %f %f %f",TG_X-TG_WIDTH/2.,TG_X+TG_WIDTH/2.,
	   TG_Y-TG_HEIGHT/2.,TG_Y+TG_HEIGHT/2.,TG_Z-TG_THICK/2.,TG_Z+TG_THICK/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  // Vertex

  geofile << "* ***Vertex" << endl;
  for (int i=0; i<VTX_NLAY; i++){
    sprintf(regname,"vtx%d%",i);
    TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
			,VTX_X,VTX_Y,VTX_Z+VTX_LAYDIST*(i-1)
			,VTX_WIDTH/2.,VTX_HEIGHT/2.,VTX_THICK/2.);
    if (kVerbosity>0) cout << FLUKAcard << endl;
    geofile << FLUKAcard << endl;
  }
  
  // Inner Tracker

  geofile << "* ***Inner Tracker" << endl;
  for (int i=0; i<ITR_NLAY; i++){
    sprintf(regname,"itr%d%",i);
    TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
			,ITR_X,ITR_Y,ITR_Z+ITR_LAYDIST*(i-1./2.)
			,ITR_WIDTH/2.,ITR_HEIGHT/2.,ITR_THICK/2.);
    if (kVerbosity>0) cout << FLUKAcard << endl;
    geofile << FLUKAcard << endl;
  }

  // Magnets

  geofile << "* ***Magnets" << endl;
  for (int i=0; i<MAG_N; i++){
    sprintf(stringa,"RCC MagCvOu%d   %f %f %f %f %f %f %f",i,MAG_X,MAG_Y
	    ,(MAG_Z-MAG_CV_LENGTH/2+i*MAG_DIST),ZERO,ZERO,MAG_CV_LENGTH,MAG_CV_OUTRAD);
    if (kVerbosity>0) cout << stringa <<endl;
    geofile << stringa << endl;
    sprintf(stringa,"RCC MagPMOu%d   %f %f %f %f %f %f %f",i,MAG_X,MAG_Y
	    ,(MAG_Z-MAG_PM_LENGTH/2+i*MAG_DIST),ZERO,ZERO,MAG_PM_LENGTH,MAG_PM_OUTRAD);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
    sprintf(stringa,"RCC MagPMIn%d   %f %f %f %f %f %f %f",i,MAG_X,MAG_Y
	    ,(MAG_Z-MAG_PM_LENGTH/2+i*MAG_DIST),ZERO,ZERO,MAG_PM_LENGTH,MAG_PM_INRAD);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
  }
  
  geofile << "* ***Gap for both magnets" << endl;
  sprintf(stringa,"ZCC Gap        %f %f %f",ZERO,ZERO,MAG_CV_INRAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  
  geofile << "* ***Magnetic field air region" << endl;
  sprintf(stringa,"RPP MagAir     %f %f %f %f %f %f",MAG_AIR_X-MAG_AIR_WIDTH/2.
	  ,MAG_AIR_X+MAG_AIR_WIDTH/2.,MAG_AIR_Y-MAG_AIR_HEIGHT/2.,MAG_AIR_Y+MAG_AIR_HEIGHT/2.
	  ,MAG_AIR_Z-MAG_AIR_LENGTH/2.,MAG_AIR_Z+MAG_AIR_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  // Drift Chamber
  
  geofile << "* ***Drift Chamber" << endl;
  sprintf(stringa,"RPP DchShiOu    %f %f %f %f %f %f",DCH_X-DCH_WIDTH/2.-DCH_SHI_THICK
	  ,DCH_X+DCH_WIDTH/2.+DCH_SHI_THICK,DCH_Y-DCH_HEIGHT/2.-DCH_SHI_THICK
	  ,DCH_Y+DCH_HEIGHT/2.+DCH_SHI_THICK,DCH_Z-DCH_SHI_LENGTH/2.,DCH_Z+DCH_SHI_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"RPP DchShiIn    %f %f %f %f %f %f",DCH_X-DCH_WIDTH/2.,DCH_X+DCH_WIDTH/2.,
	  DCH_Y-DCH_HEIGHT/2.,DCH_Y+DCH_HEIGHT/2.,DCH_Z-DCH_SHI_LENGTH/2.,DCH_Z+DCH_SHI_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  sprintf(stringa,"XYP DchMyl0        %f",DCH_Z-DCH_LENGTH/2.-DCH_MYL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP DchMyl1        %f",DCH_Z-DCH_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP DchMyl2        %f",DCH_Z+DCH_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP DchMyl3        %f",DCH_Z+DCH_LENGTH/2.+DCH_MYL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  
  // Cells 
  iCellDCH=-1;
  for (int il=0;il<DCH_NLAY;il++){
    for (int ic =0; ic<DCH_NSENSELAY;ic++){      
      iCellDCH++;
      sprintf(stringa,"RPP DchCU%d    %f %f %f %f %f %f",iCellDCH
	      ,geo.Dch.Uc_xm[ic][il],geo.Dch.Uc_xM[ic][il]
	      ,geo.Dch.Uc_ym[ic][il],geo.Dch.Uc_yM[ic][il]
	      ,geo.Dch.Uc_zm[ic][il],geo.Dch.Uc_zM[ic][il]);	
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl;      
      sprintf(stringa,"RPP DchCV%d    %f %f %f %f %f %f",iCellDCH
	      ,geo.Dch.Vc_xm[ic][il],geo.Dch.Vc_xM[ic][il]
	      ,geo.Dch.Vc_ym[ic][il],geo.Dch.Vc_yM[ic][il]
	      ,geo.Dch.Vc_zm[ic][il],geo.Dch.Vc_zM[ic][il]);	
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl;      
    }
  }
  
  // Wires U structure  
  iFieldDCH=-1;
  iSenseDCH=-1;
  for (int il=0;il<DCH_NLAY;il++){
    for (int iw =0; iw<DCH_NWIRELAY;iw++){      
      if ( (iw==geo.Dch.idsense[0]) ||(iw==geo.Dch.idsense[1]) ||
	   (iw==geo.Dch.idsense[2]) ||(iw==geo.Dch.idsense[3]) ||
	   (iw==geo.Dch.idsense[4]) ||(iw==geo.Dch.idsense[5]) ){
	iSenseDCH++;
	sprintf(stringa,"RCC DchUS%d    %f %f %f %f %f %f %f",iSenseDCH
		,geo.Dch.U_x[iw][il],geo.Dch.U_y[iw][il],geo.Dch.U_z[iw][il]
		,geo.Dch.U_cx[iw][il],geo.Dch.U_cy[iw][il],geo.Dch.U_cz[iw][il]
		,geo.Dch.U_rad[iw][il]);
      	if (kVerbosity>0) cout << stringa << endl;
      	geofile << stringa << endl;
      } else {
	iFieldDCH++;	 
	sprintf(stringa,"RCC DchUF%d   %f %f %f %f %f %f %f",iFieldDCH
		,geo.Dch.U_x[iw][il],geo.Dch.U_y[iw][il],geo.Dch.U_z[iw][il]
		,geo.Dch.U_cx[iw][il],geo.Dch.U_cy[iw][il],geo.Dch.U_cz[iw][il]
		,geo.Dch.U_rad[iw][il]); 
	if (kVerbosity>0) cout << stringa << endl;
	geofile << stringa << endl;
      }
    }
  }
  
  // Wires V structure
  iFieldDCH=-1;
  iSenseDCH=-1;
  for (int il=0;il<DCH_NLAY;il++){
    for (int iw =0; iw<DCH_NWIRELAY;iw++){
      if ((iw==geo.Dch.idsense[0]) ||(iw==geo.Dch.idsense[1]) ||
	  (iw==geo.Dch.idsense[2]) ||(iw==geo.Dch.idsense[3]) ||
	  (iw==geo.Dch.idsense[4]) ||(iw==geo.Dch.idsense[5]) ){
	iSenseDCH++;	 
	sprintf(stringa,"RCC DchVS%d    %f %f %f %f %f %f %f",iSenseDCH
		,geo.Dch.V_x[iw][il],geo.Dch.V_y[iw][il],geo.Dch.V_z[iw][il]
		,geo.Dch.V_cx[iw][il],geo.Dch.V_cy[iw][il],geo.Dch.V_cz[iw][il],
		geo.Dch.V_rad[iw][il]);
	if (kVerbosity>0) cout << stringa << endl;
	geofile << stringa << endl;
      } else {
	iFieldDCH++;	 
	sprintf(stringa,"RCC DchVF%d    %f %f %f %f %f %f %f",iFieldDCH
		,geo.Dch.V_x[iw][il],geo.Dch.V_y[iw][il],geo.Dch.V_z[iw][il]
		,geo.Dch.V_cx[iw][il],geo.Dch.V_cy[iw][il],geo.Dch.V_cz[iw][il]
		,geo.Dch.V_rad[iw][il]);
	if (kVerbosity>0) cout << stringa << endl;
	geofile << stringa << endl;
      }
    }
  }  
  
  //Scintillator
  
  geofile << "* ***Air Box for Scintillator and Calorimeter" << endl;
  TheGeometry->VolBox(FLUKAname="box",kVerbosity,&FLUKAcard,SCN_X,SCN_Y,CAL_Z-SCN_STRIP_THICK
  		      ,SCN_STRIP_HEIGHT/2+1.,SCN_STRIP_HEIGHT/2+1.,SCN_STRIP_THICK+CAL_CRY_THICK/2)+1.;
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;

  geofile << "* ***Scintillator strips" <<endl;
  for (int i=0; i<SCN_NLAY; i++){
    for (int j=0; j<SCN_NSTRIP; j++){
      if (i==0)sprintf(regname,"scnV%d",j);
      else sprintf(regname,"scnU%d",j);
      if (i==0){
        TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
                            ,SCN_X+(j-SCN_NSTRIP/2)*SCN_STRIP_WIDTH+SCN_STRIP_WIDTH/2,SCN_Y
			    ,SCN_Z-SCN_STRIP_THICK/2,SCN_STRIP_WIDTH/2.,SCN_STRIP_HEIGHT/2.
			    ,SCN_STRIP_THICK/2.);
      } else {
        TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
                            ,SCN_X,SCN_Y+(j-SCN_NSTRIP/2)*SCN_STRIP_WIDTH+SCN_STRIP_WIDTH/2
			    ,SCN_Z+SCN_STRIP_THICK/2,SCN_STRIP_HEIGHT/2.,SCN_STRIP_WIDTH/2.
			    ,SCN_STRIP_THICK/2.);
      }
      if (kVerbosity>0) cout << FLUKAcard << endl;
      geofile << FLUKAcard << endl;
    }
  }

  // Calorimeter

  geofile << "* ***Calorimeter" << endl;
  nCry=-1;
  for(int i=0;i<CAL_NROW;i++){
    y = -CAL_WIDTH/2 + i * CAL_CRY_WIDTH + CAL_CRY_WIDTH/2;
    for (int j=0;j<CAL_NCOL;j++){
      x = -CAL_WIDTH/2 + j * CAL_CRY_WIDTH + CAL_CRY_WIDTH/2;
      if( sqrt(x*x+y*y) <= CAL_WIDTH/2 - 0.7){
	nCry++;
	sprintf(regname,"cal%d",nCry);
	TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
			    ,x,y,CAL_Z,CAL_CRY_WIDTH/2.,CAL_CRY_HEIGHT/2.,CAL_CRY_THICK/2.); 
	if (kVerbosity>0) cout << FLUKAcard << endl;
	geofile << FLUKAcard << endl;
      }
    }
  }

  //END
  
  sprintf(stringa,"END        ");
  if (kVerbosity>0) cout << stringa <<endl;
  geofile << stringa <<endl;

  /*
     inizio definizione regioni
  */
  
  cout    <<"* ******************************************************************************"<<endl;
  cout    <<"*                         REGIONS                                              *"<<endl;
  cout    <<"* ******************************************************************************"<<endl;

  geofile <<"* ******************************************************************************"<<endl;
  geofile <<"*                         REGIONS                                              *"<<endl;
  geofile <<"* ******************************************************************************"<<endl;

  iReg=1;

  // Black Body
  
  sprintf(stringa,"BLACK        5 blk -air");
  regfile << "black " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa <<endl;
  geofile << stringa << endl;  
  TheGeometry->AssignMaterial("BLACK",BLK_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Air -> no mag field
  
  geofile <<"* ***Air -> no mag field" << endl;
  sprintf(stringa,"AIR          5 air -stc -MagAir -box -(MagCvOu0 -Gap) -(MagCvOu1 -Gap)\n -(BmnShiOu -BmnShiIn) -(BmnShiIn -BmnMyl0 +BmnMyl3)\n -(DchShiOu -DchShiIn) -(DchShiIn -DchMyl0 +DchMyl3) ");
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  regfile << "air " << iReg << endl; iReg++;  
  TheGeometry->AssignMaterial("AIR",AIR_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl; 
  
  // Start Counter
  
  if (kVerbosity>0) cout << "* Start Counter" << endl;
  geofile <<"* ***Start Counter" << endl;
  sprintf(stringa,"STC          5 stc");
  regfile << "start counter " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;  
  TheGeometry->AssignMaterial("STC",STC_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  // Beam Monitor

  if (kVerbosity>0) cout << "* Beam Monitor" << endl;
  geofile <<"* ***Beam Monitor" << endl;
  
  //beam monitor shielding
  sprintf(stringa,"BMN_SHI      5 BmnShiOu -BmnShiIn");
  regfile << "bmn shield " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("BMN_SHI",BMN_SHI_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor mylar windows
  sprintf(stringa,"BMN_MYL0      5 BmnShiIn -BmnMyl0 +BmnMyl1");
  regfile << "bmn front mylar " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"BMN_MYL1      5 BmnShiIn -BmnMyl2 +BmnMyl3");
  regfile << "bmn back mylar " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("BMN_MYL0",BMN_MYL_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  TheGeometry->AssignMaterial("BMN_MYL1",BMN_MYL_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor gas region
  sprintf(stringa,"BMN_GAS       5 BmnShiIn -BmnMyl1 +BmnMyl2");
  regfile << "bmn gas " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("BMN_GAS",BMN_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;  
  TheGeometry->Reg8sub("BmnUF",0,iFieldBMN,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8sub("BmnVF",0,iFieldBMN,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8sub("BmnCU",0,BMN_NLAY*BMN_NSENSELAY-1,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8sub("BmnCV",0,BMN_NLAY*BMN_NSENSELAY-1,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;

  //beam monitor cells regions
  iCellBMN=-1;
  for (int il=0;il<BMN_NLAY;il++){
    for (int iw =0; iw<BMN_NSENSELAY;iw++){
      iCellBMN++;   
      if (iw==0 && il==0) regfile << "bmn cell first " << iReg << endl;
      sprintf(stringa,"BMN_CU%02d    5 BmnCU%d -BmnUS%d",iCellBMN,iCellBMN,iCellBMN);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl; iReg++;
      if (iw==BMN_NSENSELAY-1 && il==BMN_NLAY-1) regfile << "bmn cell last " << iReg << endl;   
      sprintf(stringa,"BMN_CV%02d    5 BmnCV%d -BmnVS%d",iCellBMN,iCellBMN,iCellBMN);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl; iReg++;
    }
  }
  sprintf(regname,"BMN_CU%d",BMN_NLAY*BMN_NSENSELAY-1);
  TheGeometry->AssignMaterial("BMN_CU00",regname,1,BMN_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;  
  sprintf(regname,"BMN_CV%d",BMN_NLAY*BMN_NSENSELAY-1);
  TheGeometry->AssignMaterial("BMN_CV00",regname,1,BMN_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor field wires regions
  sprintf(stringa,"BMN_FWI      5 ");
  regfile << "bmn field wires " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->Reg8addExp("BmnUF",0,iFieldBMN,"BmnShiIn+",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8addExp("BmnVF",0,iFieldBMN,"BmnShiIn+",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->AssignMaterial("BMN_FWI",BMN_FWIRE_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor sense wires regions
  sprintf(stringa,"BMN_SWI       5 ");
  regfile << "bmn sense wires " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->Reg8addVarExp("BmnUS",0,iSenseBMN,"BmnCU",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8addVarExp("BmnVS",0,iSenseBMN,"BmnCV",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->AssignMaterial("BMN_SWI",BMN_SWIRE_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  // Target

  if (kVerbosity>0) cout << "* Target" << endl;
  geofile <<"* ***Target" << endl;
  sprintf(stringa,"TARGET       5 tgt");
  regfile << "target " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("TARGET",TG_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Vertex 

  if (kVerbosity>0) cout << "* Vertex" <<endl;
  geofile <<"* ***Vertex" <<endl;
  for (int i=0; i<VTX_NLAY; i++){
    sprintf(stringa,"VTX%d      5 vtx%d%",i,i);
    if ( i == 0 ) regfile << "vertex first " << iReg << endl;
    if ( i == VTX_NLAY-1 ) regfile << "vertex last " << iReg << endl;
    iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
  }
  sprintf(regname,"VTX%d",VTX_NLAY-1);
  TheGeometry->AssignMaterial("VTX0",regname,1,VTX_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  // Inner Tracker 

  if (kVerbosity>0) cout << "* Inner Tracker" << endl;
  geofile <<"* ***Inner Tracker" << endl;
  for (int i=0; i<ITR_NLAY; i++){
    sprintf(stringa,"ITR%d      5 itr%d%",i,i);
    if ( i == 0 ) regfile << "inner tracker first " << iReg << endl;
    if ( i == ITR_NLAY-1 ) regfile << "inner tracker last " << iReg << endl;
    iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa <<endl;
  }
  sprintf(regname,"ITR%d",ITR_NLAY-1);
  TheGeometry->AssignMaterial("ITR0",regname,1,ITR_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Magnets & Magnetic Field Air Region
  
  if (kVerbosity>0) cout << "* Magnets" << endl;
  geofile <<"* ***Magnets" << endl;
  for (int i=0; i<MAG_N; i++){
    //permanent magnet
    sprintf(stringa,"MAG_PM%d      5 MagPMOu%d -MagPMIn%d",i,i,i);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl; iReg++;
    sprintf(stringa,"MAG_PM%d",i);
    TheGeometry->AssignMaterial(stringa,MAG_PM_MEDIUM,kVerbosity,&FLUKAcard);//non accendo mag field
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
    //aluminium cover
    sprintf(stringa,"MAG_CV%d      5 MagCvOu%d -(MagPMOu%d -MagPMIn%d) -Gap",i,i,i,i);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl; iReg++;
    sprintf(stringa,"MAG_CV%d",i);    
    TheGeometry->AssignMaterial(stringa,MAG_CV_MEDIUM,kVerbosity,&FLUKAcard);//non accendo mag field
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
  }
  
  if (kVerbosity>0) cout << "* Magnetic field air region" << endl;
  geofile <<"* ***Magnetic field air region" << endl;  
  sprintf(stringa,"MAG_AIR      5 MagAir -tgt -vtx0 -vtx1 -vtx2 -itr0 -itr1 -(MagCvOu0 -Gap) -(MagCvOu1 -Gap)\n -(BmnShiIn -BmnMyl0 +BmnMyl3)-(DchShiIn -DchMyl0 +DchMyl3) ");
  regfile << "magnetic field air region " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("MAG_AIR",MAG_AIR_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  // Drift Chamber

  if (kVerbosity>0) cout << "* Drift Chamber" << endl;
  geofile <<"* ***Drift Chamber" << endl;
  
  //drift chamber shielding
  sprintf(stringa,"DCH_SHI      5 DchShiOu -DchShiIn");
  regfile << "dch shield " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("DCH_SHI",DCH_SHI_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //drift chamber mylar windows
  sprintf(stringa,"DCH_MYL0      5 DchShiIn -DchMyl0 +DchMyl1");
  regfile << "dch front mylar " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"DCH_MYL1      5 DchShiIn -DchMyl2 +DchMyl3");
  regfile << "dch back mylar " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("DCH_MYL0",DCH_MYL_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  TheGeometry->AssignMaterial("DCH_MYL1",DCH_MYL_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //drift chamber gas region
  sprintf(stringa,"DCH_GAS       5 DchShiIn -DchMyl1 +DchMyl2");
  regfile << "dch gas " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("DCH_GAS",DCH_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;  
  TheGeometry->Reg8sub("DchUF",0,iFieldDCH,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard <<endl;
  TheGeometry->Reg8sub("DchVF",0,iFieldDCH,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8sub("DchCU",0,DCH_NLAY*DCH_NSENSELAY-1,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8sub("DchCV",0,DCH_NLAY*DCH_NSENSELAY-1,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  
  //drift chamber cells regions
  iCellDCH=-1;
  for (int il=0;il<DCH_NLAY;il++){
    for (int iw =0; iw<DCH_NSENSELAY;iw++){   
      iCellDCH++;   
      if (iw==0 && il==0) regfile << "dch cell first " << iReg << endl; 
      sprintf(stringa,"DCH_CU%02d    5 DchCU%d -DchUS%d",iCellDCH,iCellDCH,iCellDCH);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl; iReg++;
      if (iw==DCH_NSENSELAY-1 && il==DCH_NLAY-1) regfile << "dch cell last " << iReg << endl;
      sprintf(stringa,"DCH_CV%02d    5 DchCV%d -DchVS%d",iCellDCH,iCellDCH,iCellDCH);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl; iReg++;
    }
  }
  sprintf(regname,"DCH_CU%d",DCH_NLAY*DCH_NSENSELAY-1);
  TheGeometry->AssignMaterial("DCH_CU00",regname,1,DCH_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;  
  sprintf(regname,"DCH_CV%d",DCH_NLAY*DCH_NSENSELAY-1);
  TheGeometry->AssignMaterial("DCH_CV00",regname,1,DCH_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //drift chamber field wires regions
  sprintf(stringa,"DCH_FWI      5 ");
  regfile << "dch field wires " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->Reg8addExp("DchUF",0,iFieldDCH,"DchShiIn+",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8addExp("DchVF",0,iFieldDCH,"DchShiIn+",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->AssignMaterial("DCH_FWI",DCH_FWIRE_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //drift chamber sense wires regions
  sprintf(stringa,"DCH_SWI       5 ");
  regfile << "dch sense wires " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->Reg8addVarExp("DchUS",0,iSenseDCH,"DchCU",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8addVarExp("DchVS",0,iSenseDCH,"DchCV",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->AssignMaterial("DCH_SWI",DCH_SWIRE_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Box for Scintillator and Calorimeter

  if (kVerbosity>0) cout << "* Air Box for Scintillator and Calorimeter" << endl;
  geofile <<"* ***Air Box for Scintillator and Calorimeter" << endl;
  sprintf(stringa,"BOX          5 box");
  regfile << "air box for scint and calo " << iReg << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;  
  TheGeometry->Reg8sub("scnV",0,SCN_NSTRIP-1,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8sub("scnU",0,SCN_NSTRIP-1,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8sub("cal",0,nCry,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->AssignMaterial("BOX",AIR_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  
  // Scintillator  

  if (kVerbosity>0) cout << "* Scintillator" << endl;
  geofile <<"* ***Scintillator" << endl;
  for (int i=0; i<SCN_NLAY; i++){
    for (int j=0; j<SCN_NSTRIP; j++){
      if (i==0) sprintf(stringa,"SCNV%02d   5 scnV%d",j,j);
      else sprintf(stringa,"SCNU%02d   5 scnU%d",j,j);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl;
      if (i==0 && j==0) regfile << "scintillator first " << iReg << endl;
      if (i==SCN_NLAY-1 && j==SCN_NSTRIP-1) regfile << "scintillator last " << iReg << endl;
      iReg++;
    }
  }
  sprintf(regname,"SCNU%d",SCN_NSTRIP-1);
  TheGeometry->AssignMaterial("SCNV00",regname,1,SCN_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  // Calorimeter 
  
  if (kVerbosity>0) cout << "* Calorimeter" << endl;
  geofile <<"* ***Calorimeter" << endl;
  for (int i=0; i<=nCry; i++){
    sprintf(stringa,"CAL%03d  5 cal%d",i,i);
    if (i == 0) regfile << "calorimeter first " << iReg << endl;
    if (i == nCry) regfile << "calorimeter last " << iReg << endl;
    iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
  }  
  sprintf(regname,"CAL%d",nCry);
  TheGeometry->AssignMaterial("CAL000",regname,1,CAL_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //END
  if (kVerbosity>0) cout << "END" <<endl;
  geofile <<"END" <<endl;

  regfile.close();
  geofile.close();

  //************************************************
  //***** EO geofile *******************************
  //************************************************

  // insert general in inp-file
  
  cout    <<"* ******************************************************************************"<<endl;
  cout    <<"*                         GENERAL & PRIMARY                                    *"<<endl;
  cout    <<"* ******************************************************************************"<<endl;
  
  //set EM transport for all regions
  
  genstg << "* **************************************************" << endl;
  genstg << "* * EM TRANSPORT THRESHOLD FOR ALL REGIONS" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-TRANS_THRES_EM)
			   ,TString::Format("%f",TRANS_THRES_EM)
			   ,"","AIR","@LASTREG","1.0","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  //set EM transport for certain regions
  
  genstg << "* * Calorimeter" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-CALO_THRES_EM),TString::Format("%f",CALO_THRES_EM)
			   ,"","CAL000",regname,"1.0","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  genstg << "* * Magnets" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-MAG_THRES_EM),TString::Format("%f",MAG_THRES_EM)
			   ,"","MAG_PM0","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-MAG_THRES_EM),TString::Format("%f",MAG_THRES_EM)
			   ,"","MAG_PM1","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  genstg << "* * Aluminium shieldings of magnets and drift chambers" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-AL_THRES_EM),TString::Format("%f",AL_THRES_EM)
			   ,"","MAG_CV0","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-AL_THRES_EM),TString::Format("%f",AL_THRES_EM)
			   ,"","MAG_CV1","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-AL_THRES_EM),TString::Format("%f",AL_THRES_EM)
			   ,"","BMN_SHI","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-AL_THRES_EM),TString::Format("%f",AL_THRES_EM)
			   ,"","DCH_SHI","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  //set EM production for all materials
  
  genstg << "* ***************************************************" <<endl;
  genstg << "* * EM PRODUCTION THRESHOLD FOR ALL REGIONS" <<endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-PROD_THRES_EM),TString::Format("%f",PROD_THRES_EM)
			   ,"","AIR","@LASTMAT","1.0","PROD-CUT");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  //set EM production for certain materials
  
  genstg << "* * Calorimeter" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-CALO_THRES_EM),TString::Format("%f",CALO_THRES_EM)
			   ,"","BGO","","","PROD-CUT");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  genstg << "* * Magnets" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-MAG_THRES_EM),TString::Format("%f",MAG_THRES_EM)
			   ,"","SmCo","","","PROD-CUT");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  genstg << "* * Aluminium shieldings of magnets and drift chambers" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-AL_THRES_EM),TString::Format("%f",AL_THRES_EM)
			   ,"","ALUMINUM","","","PROD-CUT");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  //set delta-ray for all materials
  
  genstg << "* ***********************************************" << endl;
  genstg << "* * DELTA-RAY THRESHOLD FOR ALL REGIONS" << endl;
  TheGeometry->GenericCard("DELTARAY",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",DELTA_THRES_EM),"","","AIR","@LASTMAT","1.0","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  //set delta-ray to for certain materials
  
  genstg << "* * Calorimeter" << endl;
  TheGeometry->GenericCard("DELTARAY",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",CALO_THRES_EM),"","","BGO","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  genstg << "* * Magnets" << endl;
  TheGeometry->GenericCard("DELTARAY",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",MAG_THRES_EM),"","","SmCo","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  genstg << "* * Aluminium shieldings of magnets and drift chambers" << endl;
  TheGeometry->GenericCard("DELTARAY",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",AL_THRES_EM),"","","ALUMINUM","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  //set pair-brem for all materials
  
  genstg << "* ***********************************************" << endl;
  genstg << "* * PAIR-BREM THRESHOLD FOR ALL REGIONS" << endl;
  TheGeometry->GenericCard("PAIRBREM",kVerbosity,&FLUKAcard
			   ,"3.",TString::Format("%f",PAIRBREM_THRES_EM),
			   TString::Format("%f",PAIRBREM_THRES_EM),"AIR","@LASTMAT","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  //set pair-brem for certain materials
  
  genstg << "* * Calorimeter" << endl;
  TheGeometry->GenericCard("PAIRBREM",kVerbosity,&FLUKAcard
			   ,"3.",TString::Format("%f",CALO_THRES_EM),
			   TString::Format("%f",CALO_THRES_EM),"BGO","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  genstg << "* * Magnets" << endl;
  TheGeometry->GenericCard("PAIRBREM",kVerbosity,&FLUKAcard
			   ,"3.",TString::Format("%f",MAG_THRES_EM),
			   TString::Format("%f",MAG_THRES_EM),"SmCo","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  genstg << "* * Aluminium shieldings of magnets and drift chambers" << endl;
  TheGeometry->GenericCard("PAIRBREM",kVerbosity,&FLUKAcard
			   ,"3.",TString::Format("%f",AL_THRES_EM),
			   TString::Format("%f",AL_THRES_EM),"ALUMINUM","","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  genstg << "*";

  TheGeometry->InsertInFile("foot.inp","foot.inp","* @@@START GENERATED, DO NOT MODIFY:GENERAL@@@","* @@@END GENERATED:GENERAL@@@",genstg.str());

  // insert media in inp-file
  
  cout    <<"* ******************************************************************************"<<endl;
  cout    <<"*                         MEDIA & MAGFIELD                                     *"<<endl;
  cout    <<"* ******************************************************************************"<<endl;

  if (MagField){
    TheGeometry->GenericCard("MGNFIELD",kVerbosity,&FLUKAcard ,"","","","0.","0.","0.","");
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
  }
  
  matstg << "*";

  TheGeometry->InsertInFile("foot.inp","foot.inp","* @@@START GENERATED, DO NOT MODIFY:MATERIAL&MAGFIELD@@@","* @@@END GENERATED:MATERIAL&MAGFIELD@@@",matstg.str());



  /*
    start writing parameters file for FLUKA user routines
  */

  paramfile << "      integer nlayBMN, ncellBMN, nlayDCH, ncellDCH" << endl;
  paramfile << "      integer nlayVTX, nlayITR" << endl;
  paramfile << "      integer nstripSCN, ncryCAL\n" << endl;
  paramfile << "      parameter nlayBMN = " << BMN_NLAY  << endl;
  paramfile << "      parameter ncellBMN = " << BMN_NSENSELAY  << endl;
  paramfile << "      parameter nlayDCH = " << DCH_NLAY  << endl;
  paramfile << "      parameter ncellDCH = " << DCH_NSENSELAY  << endl;
  paramfile << "      parameter nlayVTX = " << VTX_NLAY  << endl;
  paramfile << "      parameter nlayITR = " << ITR_NLAY  << endl;
  paramfile << "      parameter nstripSCN = " << SCN_NSTRIP  << endl;
  paramfile << "      parameter ncryCAL = " << nCry+1  << endl;
  
  paramfile.close();
  
  return 0;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************
