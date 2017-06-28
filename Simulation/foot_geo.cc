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
// GEOMETRY_STRUCT geo;


int main(int argc, char *argv[]){ 

  char stringa[300], nomefile[50], regname[20];
  string line;

  TString FLUKAcard;
  TString FLUKAname;

  stringstream genstg;

  Int_t iReg;
  Int_t iCellBMN, iSenseBMN, iFieldBMN;
  //  Int_t iCellDCH, iSenseDCH, iFieldDCH;
  Int_t nCry, nScnV, nScnU;

  Double_t x, y;

  bool sense;
  
  genstg<<"* ******************************************************************************"<<endl;
  genstg<<"*                           GENERAL & PRIMARY                                  *"<<endl;
  genstg<<"* ******************************************************************************"<<endl;

  genstg<<"* -------1---------2---------3---------4---------5---------6---------7---------8"<<endl;
  genstg<<"* command| what(1) | what(2) | what(3) | what(4) | what(5) | what(6) | SDUM    |"<<endl;
  genstg<<"* -------1---------2---------3---------4---------5---------6---------7---------8"<<endl;
 
  if(FLK_VERS==0){
    TheGeometry->GenericCard("PHYSICS",kVerbosity,&FLUKAcard,"1.","","","","","","COALESCE");
  }else{
    TheGeometry->GenericCard("PHYSICS",kVerbosity,&FLUKAcard,"12001.","1.","1.","","","","COALESCE");
  }
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
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
			     ,TString::Format("%f",PRIM_DIV),TString::Format("%f",-PRIM_RMAX)
			     ,TString::Format("%f",-PRIM_RMAX),"1.0",BeamParticle);
  } else if (PRIM_DIST=="ANNULAR"){
    TheGeometry->GenericCard("BEAM",kVerbosity,&FLUKAcard
			     ,TString::Format("%f",-PRIM_T),TString::Format("%f",PRIM_dP)
			     ,TString::Format("%f",PRIM_DIV), TString::Format("%f",PRIM_RMAX)
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
  paramfile.open("ROUTINES/parameters.inc");
  
  if (TheGeometry) {
    delete TheGeometry;
    TheGeometry = 0;
  }

  TheGeometry = new Geometry();

  auto& geo = TheGeometry->GetStruct();

  //END OF INIT
  
  /*
    start writing geo file 
  */
  
  sprintf(stringa,"    0    0          FOOT experiment geometry");
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  geofile << "* ***Black Body" << endl;
  sprintf(stringa,"RPP blk        -1000. 1000. -1000. 1000. -1000. 1000.");
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  geofile << "* ***Air -> no mag field" << endl;
  sprintf(stringa,"RPP air        -900.0 900.0 -900.0 900.0 -900.0 900.0");
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

  // Vacuum chamber
  geofile << "* ***Vacuum chamber" << endl;
  TheGeometry->VolBox(FLUKAname="vacOut",kVerbosity,&FLUKAcard
			,ITR_X,ITR_Y,ITR_Z,10.+0.01,10.+0.01,12.+0.01);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->VolBox(FLUKAname="vacIn",kVerbosity,&FLUKAcard
			,ITR_X,ITR_Y,ITR_Z,10.,10,12.);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;

  // Target
  
  geofile << "* ***Target" << endl;
  sprintf(stringa,"RPP tgt        %f %f %f %f %f %f",TG_X-TG_WIDTH/2.,TG_X+TG_WIDTH/2.,
	   TG_Y-TG_HEIGHT/2.,TG_Y+TG_HEIGHT/2.,TG_Z-TG_THICK/2.,TG_Z+TG_THICK/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  // Vertex

  geofile << "* ***Vertex" << endl;
  for (int i=0; i<VTX_NLAY; i++){
    sprintf(regname,"vtx%d",i);
    TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
			,VTX_X,VTX_Y,VTX_Z+VTX_LAYDIST*(i-1.5)
			,VTX_WIDTH/2.,VTX_HEIGHT/2.,VTX_THICK/2.);
    if (kVerbosity>0) cout << FLUKAcard << endl;
    geofile << FLUKAcard << endl;
  }
  
  // Inner Tracker

  geofile << "* ***Inner Tracker" << endl;
  double itr_tot_thick=2*ITR_THICK+2*ITR_EPO_THICK+4*ITR_COV_THICK+4*ITR_AL_THICK
    +2*ITR_KAP_THICK+ITR_FOAM_THICK;
  sprintf(regname,"itrbox");
  TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
			,ITR_X,ITR_Y,ITR_Z
			,ITR_WIDTH/2.,ITR_HEIGHT/2.,itr_tot_thick/2);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  sprintf(stringa,"XYP itr00     %f",ITR_Z-itr_tot_thick/2+ITR_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr01     %f",ITR_Z-itr_tot_thick/2+ITR_THICK+ITR_EPO_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr02     %f",ITR_Z-itr_tot_thick/2+ITR_THICK+ITR_EPO_THICK+ITR_COV_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr03     %f",ITR_Z-itr_tot_thick/2+ITR_THICK+ITR_EPO_THICK+ITR_COV_THICK+ITR_AL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr04     %f",ITR_Z-ITR_FOAM_THICK/2-ITR_COV_THICK-ITR_AL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr05     %f",ITR_Z-ITR_FOAM_THICK/2-ITR_COV_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr06     %f",ITR_Z-ITR_FOAM_THICK/2);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr07     %f",ITR_Z+ITR_FOAM_THICK/2);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr08     %f",ITR_Z+ITR_FOAM_THICK/2+ITR_COV_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr09     %f",ITR_Z+ITR_FOAM_THICK/2+ITR_COV_THICK+ITR_AL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr10     %f",ITR_Z+itr_tot_thick/2-ITR_THICK-ITR_EPO_THICK-ITR_COV_THICK-ITR_AL_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr11     %f",ITR_Z+itr_tot_thick/2-ITR_THICK-ITR_EPO_THICK-ITR_COV_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr12     %f",ITR_Z+itr_tot_thick/2-ITR_THICK-ITR_EPO_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"XYP itr13     %f",ITR_Z+itr_tot_thick/2-ITR_THICK);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  

  // Magnets
  
  geofile << "* ***Magnets" << endl;
  sprintf(stringa,"RCC MagCvOu0   %f %f %f %f %f %f %f",MAG_X,MAG_Y
	  ,(MAG_Z-MAG_CV_LENGTH/2),ZERO,ZERO,MAG_CV_LENGTH,MAG_CV0_OUTRAD);
  if (kVerbosity>0) cout << stringa <<endl;
  geofile << stringa << endl;
  sprintf(stringa,"RCC MagCvOu1   %f %f %f %f %f %f %f",MAG_X,MAG_Y
	  ,(MAG_Z-MAG_CV_LENGTH/2+MAG_DIST),ZERO,ZERO,MAG_CV_LENGTH,MAG_CV1_OUTRAD);
  if (kVerbosity>0) cout << stringa <<endl;
  geofile << stringa << endl;
  sprintf(stringa,"RCC MagPMOu0   %f %f %f %f %f %f %f",MAG_X,MAG_Y
	  ,(MAG_Z-MAG_PM_LENGTH/2),ZERO,ZERO,MAG_PM_LENGTH,MAG_PM0_OUTRAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"RCC MagPMOu1   %f %f %f %f %f %f %f",MAG_X,MAG_Y
	  ,(MAG_Z-MAG_PM_LENGTH/2+MAG_DIST),ZERO,ZERO,MAG_PM_LENGTH,MAG_PM1_OUTRAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"RCC MagPMIn0   %f %f %f %f %f %f %f",MAG_X,MAG_Y
	  ,(MAG_Z-MAG_PM_LENGTH/2),ZERO,ZERO,MAG_PM_LENGTH,MAG_PM0_INRAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"RCC MagPMIn1   %f %f %f %f %f %f %f",MAG_X,MAG_Y
	  ,(MAG_Z-MAG_PM_LENGTH/2+MAG_DIST),ZERO,ZERO,MAG_PM_LENGTH,MAG_PM1_INRAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  
  geofile << "* ***Gap for magnets" << endl;
  sprintf(stringa,"ZCC Gap0       %f %f %f",ZERO,ZERO,MAG_CV0_INRAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"ZCC Gap1       %f %f %f",ZERO,ZERO,MAG_CV1_INRAD);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  
  geofile << "* ***Magnetic field air region" << endl;
  sprintf(stringa,"RPP MagAir     %f %f %f %f %f %f",MAG_AIR_X-MAG_AIR_WIDTH/2.
	  ,MAG_AIR_X+MAG_AIR_WIDTH/2.,MAG_AIR_Y-MAG_AIR_HEIGHT/2.,MAG_AIR_Y+MAG_AIR_HEIGHT/2.
	  ,MAG_AIR_Z-MAG_AIR_LENGTH/2.,MAG_AIR_Z+MAG_AIR_LENGTH/2.);
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;

  //Silicon MicroStrip Detector
  
  geofile << "* ***Silicon MicroStrip Detector" << endl;
  for (int i=0; i<MSD_NLAY; i++){
    sprintf(regname,"msd%d",i);
    TheGeometry->VolBox(FLUKAname=regname,kVerbosity,&FLUKAcard
			,MSD_X,MSD_Y,MSD_Z+MSD_LAYDIST*(i-1.5)
			,MSD_WIDTH/2.,MSD_HEIGHT/2.,MSD_THICK+MSD_KAP_THICK/2.);
    if (kVerbosity>0) cout << FLUKAcard << endl;
    geofile << FLUKAcard << endl;
    sprintf(stringa,"XYP msdpla0%d     %f",i,MSD_Z+MSD_LAYDIST*(i-1.5)-MSD_KAP_THICK/2.);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
    sprintf(stringa,"XYP msdpla1%d     %f",i,MSD_Z+MSD_LAYDIST*(i-1.5)+MSD_KAP_THICK/2.);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
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
  regfile << "const int nregBlk = " << iReg << "; //blackbody" << endl; iReg++;
  if (kVerbosity>0) cout << stringa <<endl;
  geofile << stringa << endl;  
  TheGeometry->AssignMaterial("BLACK",BLK_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Air -> no mag field
  
  geofile <<"* ***Air -> no mag field" << endl;
  if (VacChamber){
    sprintf(stringa,"AIR          5 air -stc -MagAir -vacOut -box -(BmnShiOu -BmnShiIn) -(BmnShiIn -BmnMyl0 +BmnMyl3)\n -msd1 -msd2 -msd0");
  }else{
    sprintf(stringa,"AIR          5 air -stc -MagAir -(MagCvOu0 -Gap0) -(MagCvOu1 -Gap1) -box\n -(BmnShiOu -BmnShiIn)\n -(BmnShiIn -BmnMyl0 +BmnMyl3) -msd1 -msd2 -msd0");
  }
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  regfile << "const int nregAir = " << iReg << "; //Air no mag field" << endl; iReg++;  
  TheGeometry->AssignMaterial("AIR",AIR_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl; 
  
  // Start Counter
  
  if (kVerbosity>0) cout << "* Start Counter" << endl;
  geofile <<"* ***Start Counter" << endl;
  sprintf(stringa,"STC          5 stc");
  regfile << "const int nregSTC = " << iReg << "; //start counter" << endl; iReg++;
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
  regfile << "const int nregShiBMN = " << iReg << "; //BMN shield" << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("BMN_SHI",BMN_SHI_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor mylar windows
  sprintf(stringa,"BMN_MYL0     5 BmnShiIn -BmnMyl0 +BmnMyl1");
  regfile << "const int nregMyl1BMN = " << iReg << "; //BMN front mylar window" << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  sprintf(stringa,"BMN_MYL1     5 BmnShiIn -BmnMyl2 +BmnMyl3");
  regfile << "const int nregMyl2BMN = " << iReg << "; //BMN back mylar window" << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->AssignMaterial("BMN_MYL0",BMN_MYL_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  if(BMN_MAG) TheGeometry->AssignMaterial("BMN_MYL1",BMN_MYL_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("BMN_MYL1",BMN_MYL_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor cells regions
  iCellBMN=-1;
  for (int il=0;il<BMN_NLAY;il++){
    for (int iw =0; iw<BMN_NSENSELAY;iw++){
      iCellBMN++;   
      if (iw==0 && il==0) regfile << "const int nregFirstBMN = " << iReg << "; //first BMN cell" << endl;
      sprintf(stringa,"BMN_CU%02d     5 BmnCU%d -BmnUS%d",iCellBMN,iCellBMN,iCellBMN);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl; iReg++;
      if (iw==BMN_NSENSELAY-1 && il==BMN_NLAY-1) regfile << "const int nregLastBMN = " << iReg << "; //last BMN cell" << endl;   
      sprintf(stringa,"BMN_CV%02d     5 BmnCV%d -BmnVS%d",iCellBMN,iCellBMN,iCellBMN);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl; iReg++;
    }
  }
  sprintf(regname,"BMN_CU%d",BMN_NLAY*BMN_NSENSELAY-1);
  if(BMN_MAG) TheGeometry->AssignMaterial("BMN_CU00",regname,1,BMN_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("BMN_CU00",regname,1,BMN_GAS_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;  
  sprintf(regname,"BMN_CV%d",BMN_NLAY*BMN_NSENSELAY-1);
  if(BMN_MAG) TheGeometry->AssignMaterial("BMN_CV00",regname,1,BMN_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("BMN_CV00",regname,1,BMN_GAS_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor gas region
  sprintf(stringa,"BMN_GAS      5 BmnShiIn -BmnMyl1 +BmnMyl2");
  regfile << "const int nregGasBMN = " << iReg << "; //gas inside bmn (no cells)" << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  if(BMN_MAG) TheGeometry->AssignMaterial("BMN_GAS",BMN_GAS_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("BMN_GAS",BMN_GAS_MEDIUM,kVerbosity,&FLUKAcard);
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
  
  //beam monitor field wires regions
  sprintf(stringa,"BMN_FWI      5 ");
  regfile << "const int nregFWBMN = " << iReg << "; //BMN field wires" << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->Reg8addExp("BmnUF",0,iFieldBMN,"BmnShiIn+",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8addExp("BmnVF",0,iFieldBMN,"BmnShiIn+",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  if(BMN_MAG) TheGeometry->AssignMaterial("BMN_FWI",BMN_FWIRE_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("BMN_FWI",BMN_FWIRE_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  //beam monitor sense wires regions
  sprintf(stringa,"BMN_SWI      5 ");
  regfile << "const int nregSWBMN = " << iReg << "; //BMN sense wires" << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  TheGeometry->Reg8addVarExp("BmnUS",0,iSenseBMN,"BmnCU",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  TheGeometry->Reg8addVarExp("BmnVS",0,iSenseBMN,"BmnCV",kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  geofile << FLUKAcard << endl;
  if(BMN_MAG) TheGeometry->AssignMaterial("BMN_SWI",BMN_SWIRE_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("BMN_SWI",BMN_SWIRE_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Vacuum chamber

  if (VacChamber){
    if (kVerbosity>0) cout << "* Vacuum chamber" << endl;
    geofile <<"* ***Vacuum chamber" << endl;
    sprintf(stringa,"VACCH        5 vacOut -vacIn");
    regfile << "const int nregWallVCH = " << iReg << "; //vacuum chamber walls" << endl; iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
    if(VAC_MAG) TheGeometry->AssignMaterial("VACCH","KAPTON",MagField,kVerbosity,&FLUKAcard);
    else TheGeometry->AssignMaterial("VACCH","KAPTON",kVerbosity,&FLUKAcard);
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
    if (kVerbosity>0) cout << "* Magnetic field vacuum region" << endl;
    geofile <<"* ***Magnetic field vacuum region" << endl;  
    sprintf(stringa,"MAG_VAC      5 vacIn -itrbox -(MagCvOu0 -Gap0) -(MagCvOu1 -Gap1)");
    regfile << "const int nregMagVac = " << iReg << "; //magnetic vacuum region" << endl; iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
    if(VAC_MAG) TheGeometry->AssignMaterial("MAG_VAC","VACUUM",MagField,kVerbosity,&FLUKAcard);
    else TheGeometry->AssignMaterial("MAG_VAC","VACUUM",kVerbosity,&FLUKAcard);
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
  }
  
  // Target

  if (kVerbosity>0) cout << "* Target" << endl;
  geofile <<"* ***Target" << endl;
  sprintf(stringa,"TARGET       5 tgt");
  regfile << "const int nregTarg = " << iReg << "; //target"  << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  if(TG_MAG) TheGeometry->AssignMaterial("TARGET",TG_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("TARGET",TG_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Vertex 

  if (kVerbosity>0) cout << "* Vertex" <<endl;
  geofile <<"* ***Vertex" <<endl;
  for (int i=0; i<VTX_NLAY; i++){
    sprintf(stringa,"VTX%d         5 vtx%d",i,i);
    if ( i == 0 ) regfile << "const int nregFirstVTX = " << iReg << "; //first vertex" << endl;
    if ( i == VTX_NLAY-1 ) regfile << "const int nregLastVTX = " << iReg << "; //last vertex" << endl;
    iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
  }
  sprintf(regname,"VTX%d",VTX_NLAY-1);
  if(VTX_MAG) TheGeometry->AssignMaterial("VTX0",regname,1,VTX_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("VTX0",regname,1,VTX_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  // Inner Tracker 
  
  if (kVerbosity>0) cout << "* Inner Tracker" << endl;
  geofile <<"* ***Inner Tracker" << endl;
  sprintf(stringa,"ITR0         5 itrbox +itr00");
  regfile << "const int nregFirstITR = " << iReg << "; //first inner tracker" << endl;iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITR1         5 itrbox -itr13");
  regfile << "const int nregLastITR = " << iReg << "; //last inner tracker" << endl;iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITREPO0      5 itrbox -itr00 +itr01");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITREPO1      5 itrbox -itr12 +itr13");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRAL0       5 itrbox -itr02 +itr03");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRAL1       5 itrbox -itr04 +itr05");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRAL2       5 itrbox -itr08 +itr09");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRAL3       5 itrbox -itr10 +itr11");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRCOV0      5 itrbox -itr01 +itr02");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRCOV1      5 itrbox -itr05 +itr06");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRCOV2      5 itrbox -itr07 +itr08");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRCOV3      5 itrbox -itr11 +itr12");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRKAP0      5 itrbox -itr03 +itr04");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRKAP1      5 itrbox -itr09 +itr10");iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(stringa,"ITRFOAM      5 itrbox -itr06 +itr07");
  regfile << "const int nregLastRegITR = " << iReg << "; //last region of inner tracker" << endl;iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa <<endl;
  sprintf(regname,"ITR%d",ITR_NLAY-1);
  if(ITR_MAG) TheGeometry->AssignMaterial("ITR0",regname,1,ITR_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("ITR0",regname,1,ITR_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  sprintf(regname,"ITREPO%d",ITR_NLAY-1);
  if(ITR_MAG) TheGeometry->AssignMaterial("ITREPO0",regname,1,ITR_EPO_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("ITREPO0",regname,1,ITR_EPO_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  sprintf(regname,"ITRAL%d",2*ITR_NLAY-1);
  if(ITR_MAG) TheGeometry->AssignMaterial("ITRAL0",regname,1,ITR_AL_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("ITRAL0",regname,1,ITR_AL_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  sprintf(regname,"ITRCOV%d",2*ITR_NLAY-1);
  if(ITR_MAG) TheGeometry->AssignMaterial("ITRCOV0",regname,1,ITR_COV_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("ITRCOV0",regname,1,ITR_COV_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  sprintf(regname,"ITRKAP%d",ITR_NLAY-1);
  if(ITR_MAG) TheGeometry->AssignMaterial("ITRKAP0",regname,1,ITR_KAP_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("ITRKAP0",regname,1,ITR_KAP_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  if(ITR_MAG) TheGeometry->AssignMaterial("ITRFOAM",ITR_FOAM_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("ITRFOAM",ITR_FOAM_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  
  // Magnets & Magnetic Field Air Region
  
  if (kVerbosity>0) cout << "* Magnets" << endl;
  geofile <<"* ***Magnets" << endl;
  for (int i=0; i<MAG_N; i++){
    //permanent magnet
    sprintf(stringa,"MAG_PM%d      5 MagPMOu%d -MagPMIn%d",i,i,i);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl; 
    sprintf(stringa,"const int nregPmMAG%d = %d; //permanent mag region of magnet %d",i,iReg,i);
    regfile << stringa  << endl;
    iReg++;
    sprintf(stringa,"MAG_PM%d",i);
    TheGeometry->AssignMaterial(stringa,MAG_PM_MEDIUM,kVerbosity,&FLUKAcard);//non accendo mag field
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
    //aluminium cover
    sprintf(stringa,"MAG_CV%d      5 MagCvOu%d -(MagPMOu%d -MagPMIn%d) -Gap%d",i,i,i,i,i);
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;  
    sprintf(stringa,"const int nregCvMAG%d = %d; //cover region of magnet %d",i,iReg,i);
    regfile << stringa  << endl;
    iReg++;
    sprintf(stringa,"MAG_CV%d",i);    
    TheGeometry->AssignMaterial(stringa,MAG_CV_MEDIUM,kVerbosity,&FLUKAcard);//non accendo mag field
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
  }
  
  if (kVerbosity>0) cout << "* Magnetic field air region" << endl;
  geofile <<"* ***Magnetic field air region" << endl;
  if(!VacChamber){
    sprintf(stringa,"MAG_AIR      5 MagAir -tgt -vtx0 -vtx1 -vtx2 -vtx3 -itrbox -(MagCvOu0 -Gap0) -(MagCvOu1 -Gap1)\n -(BmnShiIn -BmnMyl0 +BmnMyl3) -msd1 -msd2 -msd0");
  }else{
    sprintf(stringa,"MAG_AIR      5 MagAir -tgt -vtx0 -vtx1 -vtx2 -vacOut -(BmnShiIn -BmnMyl0 +BmnMyl3)\n  -msd1 -msd2 -msd0");
  }  
  regfile << "const int nregMagAir = " << iReg << "; //magnetic field air region" << endl; iReg++;
  if (kVerbosity>0) cout << stringa << endl;
  geofile << stringa << endl;
  if(MAG_AIR_MAG) TheGeometry->AssignMaterial("MAG_AIR",MAG_AIR_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("MAG_AIR",MAG_AIR_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;

  // Silicon MicroStrip Detector
  
  if (kVerbosity>0) cout << "* Silicon MicroStrip Detector" <<endl;
  geofile <<"* ***Silicon MicroStrip Detector" <<endl;
  for (int i=0; i<MSD_NLAY; i++){
    sprintf(stringa,"MSDV%d         5 msd%d -msdpla1%d",i,i,i);
    if ( i == 0 ) regfile << "const int nregFirstMSD = " << iReg << "; //first silicon microstrip detector" << endl;
    iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
    sprintf(stringa,"MSDU%d         5 msd%d +msdpla0%d",i,i,i);
    if ( i == MSD_NLAY-1 ) regfile << "const int nregLastMSD = " << iReg << "; //last silicon microstrip detector" << endl;
    iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
  }
  sprintf(regname,"MSDU%d",MSD_NLAY-1);
  if(MSD_MAG) TheGeometry->AssignMaterial("MSDV0",regname,1,MSD_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("MSDV0",regname,1,MSD_MEDIUM,kVerbosity,&FLUKAcard);

  //kapton layers
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  for (int i=0; i<MSD_NLAY; i++){
    sprintf(stringa,"MSDKAP%d       5 msd%d +msdpla1%d -msdpla0%d",i,i,i,i);
    iReg++;
    if (kVerbosity>0) cout << stringa << endl;
    geofile << stringa << endl;
    if ( i == 0 ) regfile << "const int nregFirstKapMSD = " << iReg << "; //first MSD kapton layer" << endl;
    if ( i == MSD_NLAY-1 ) regfile << "const int nregLastMSD = " << iReg << "; //last MSD kapton layer" << endl;
  }
  sprintf(regname,"MSDKAP%d",MSD_NLAY-1);
  if(MSD_MAG) TheGeometry->AssignMaterial("MSDKAP0",regname,1,MSD_KAP_MEDIUM,MagField,kVerbosity,&FLUKAcard);
  else TheGeometry->AssignMaterial("MSDKAP0",regname,1,MSD_MEDIUM,kVerbosity,&FLUKAcard);
  if (kVerbosity>0) cout << FLUKAcard << endl;
  matstg << FLUKAcard << endl;
  
  
  // Box for Scintillator and Calorimeter

  if (kVerbosity>0) cout << "* Air Box for Scintillator and Calorimeter" << endl;
  geofile <<"* ***Air Box for Scintillator and Calorimeter" << endl;
  sprintf(stringa,"BOX          5 box");
  regfile << "const int nregAirCAL = " << iReg << "; //air box for scint and calo" << endl; iReg++;
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
      if (i==0) sprintf(stringa,"SCNV%02d       5 scnV%d",j,j);
      else sprintf(stringa,"SCNU%02d       5 scnU%d",j,j);
      if (kVerbosity>0) cout << stringa << endl;
      geofile << stringa << endl;
      if (i==0 && j==0) regfile << "const int nregFirstSCN = " << iReg << "; //SCN first strip" << endl;
      if (i==SCN_NLAY-1 && j==SCN_NSTRIP-1) regfile << "const int nregLastSCN = " << iReg << "; //SCN fistr strip" << endl;
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
    sprintf(stringa,"CAL%03d       5 cal%d",i,i);
    if (i == 0) regfile << "const int nregFirstCAL = " << iReg << "; //first CAL crystal" << endl;
    if (i == nCry) regfile << "const int nregLastCAL = " << iReg << "; //last CAL crystal" << endl;
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
  genstg << "* * EM TRANSPORT THRESHOLD FOR EVERY REGION" << endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-TRANS_THRES_EM)
			   ,TString::Format("%f",TRANS_THRES_EM)
			   ,"","BLACK","@LASTREG","1.0","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  //set EM transport for gas regions
  
  if(lowth){
    genstg << "* * Drift chambers gas" << endl;
    TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			     ,TString::Format("%f",-GAS_THRES),TString::Format("%f",GAS_THRES)
			     ,"","BMN_CU00","BMN_GAS","1.0","");
    if (kVerbosity>0) cout << FLUKAcard << endl;
    genstg << FLUKAcard << endl;/*
    TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			     ,TString::Format("%f",-GAS_THRES),TString::Format("%f",GAS_THRES)
			     ,"","DCH_CU00","DCH_GAS","1.0","");
    if (kVerbosity>0) cout << FLUKAcard << endl;
    genstg << FLUKAcard << endl;*/
  }
  
  //set EM production for all materials
  
  genstg << "* ***************************************************" <<endl;
  genstg << "* * EM PRODUCTION THRESHOLD FOR EVERY MATERIAL" <<endl;
  TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",-PROD_THRES_EM),TString::Format("%f",PROD_THRES_EM)
			   ,"1.","BLCKHOLE","@LASTMAT","1.0","PROD-CUT");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  //set EM production for gas materials
  
  if(lowth){
    genstg << "* * Drift Chambers Gas" << endl;
    TheGeometry->GenericCard("EMFCUT",kVerbosity,&FLUKAcard
			     ,TString::Format("%f",-GAS_THRES),TString::Format("%f",GAS_THRES)
			     ,"1.","Ar-CO2","","","PROD-CUT");
    if (kVerbosity>0) cout << FLUKAcard << endl;
    genstg << FLUKAcard << endl;
  }
  
  //set delta-ray for all materials
  
  genstg << "* ***********************************************" << endl;
  genstg << "* * DELTA-RAY THRESHOLD FOR ALL REGIONS" << endl;
  TheGeometry->GenericCard("DELTARAY",kVerbosity,&FLUKAcard
			   ,TString::Format("%f",DELTA_THRES_EM),"","","BLCKHOLE","@LASTMAT","1.0","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;
  
  //set delta-ray to for certain materials
  if(lowth){
    genstg << "* * Drift Chambers Gas" << endl;
    TheGeometry->GenericCard("DELTARAY",kVerbosity,&FLUKAcard
			     ,TString::Format("%f",GAS_THRES),"","","Ar-CO2","","","");
    if (kVerbosity>0) cout << FLUKAcard << endl;
    genstg << FLUKAcard << endl;
  }
  
  //set pair-brem for all materials
  
  genstg << "* ***********************************************" << endl;
  genstg << "* * PAIR-BREM THRESHOLD FOR ALL REGIONS" << endl;
  TheGeometry->GenericCard("PAIRBREM",kVerbosity,&FLUKAcard
			   ,"-3.","","","BLCKHOLE","@LASTMAT","","");
  if (kVerbosity>0) cout << FLUKAcard << endl;
  genstg << FLUKAcard << endl;

  genstg << "*";

  TheGeometry->InsertInFile("foot.inp","foot.inp","* @@@START GENERATED, DO NOT MODIFY:GENERAL@@@","* @@@END GENERATED:GENERAL@@@",genstg.str());

  // insert media in inp-file
  
  cout    <<"* ******************************************************************************"<<endl;
  cout    <<"*                         MEDIA & MAGFIELD                                     *"<<endl;
  cout    <<"* ******************************************************************************"<<endl;

  if (MagField){
    TheGeometry->GenericCard("MGNFIELD",kVerbosity,&FLUKAcard,
			     TString::Format("%f",MaxAng),TString::Format("%f",BoundAcc),"",
			     TString::Format("%f",Bx),TString::Format("%f",By),
			     TString::Format("%f",Bz),"");
    if (kVerbosity>0) cout << FLUKAcard << endl;
    matstg << FLUKAcard << endl;
  }
  
  matstg << "*";

  TheGeometry->InsertInFile("foot.inp","foot.inp","* @@@START GENERATED, DO NOT MODIFY:MATERIAL&MAGFIELD@@@","* @@@END GENERATED:MATERIAL&MAGFIELD@@@",matstg.str());



  /*
    start writing parameters file for FLUKA user routines
  */

  paramfile << "      double precision xminVTX, yminVTX, dxVTX, dyVTX" << endl;
  paramfile << "      double precision xminITR, yminITR, dxITR, dyITR" << endl;
  paramfile << "      double precision xminMSD, yminMSD, dxMSD, dyMSD" << endl;
  paramfile << "      integer xpixVTX, ypixVTX" << endl;
  paramfile << "      integer xpixITR, ypixITR" << endl;
  paramfile << "      integer xstripMSD, ystripMSD" << endl;
  paramfile << "      integer nlayBMN, ncellBMN" << endl;
  paramfile << "      integer nlayVTX, nlayITR" << endl;
  paramfile << "      integer nlayMSD, nviewMSD" << endl;
  paramfile << "      integer nstripSCN, ncryCAL\n" << endl;
  paramfile << "      parameter (nlayBMN = "   << BMN_NLAY      << ")" << endl;
  paramfile << "      parameter (ncellBMN = "  << BMN_NSENSELAY << ")" << endl;
  paramfile << "      parameter (nlayVTX = "   << VTX_NLAY      << ")" << endl;
  paramfile << "      parameter (xminVTX = "   << VTX_XMIN      << ")" << endl;
  paramfile << "      parameter (yminVTX = "   << VTX_YMIN      << ")" << endl;
  paramfile << "      parameter (dxVTX = "     << VTX_DX        << ")" << endl;
  paramfile << "      parameter (dyVTX = "     << VTX_DY        << ")" << endl;
  paramfile << "      parameter (xpixVTX = "   << VTX_XPIX      << ")" << endl;
  paramfile << "      parameter (ypixVTX = "   << VTX_YPIX      << ")" << endl;
  paramfile << "      parameter (nlayITR = "   << ITR_NLAY      << ")" << endl;
  paramfile << "      parameter (xminITR = "   << ITR_XMIN      << ")" << endl;
  paramfile << "      parameter (yminITR = "   << ITR_YMIN      << ")" << endl;
  paramfile << "      parameter (dxITR = "     << ITR_DX        << ")" << endl;
  paramfile << "      parameter (dyITR = "     << ITR_DY        << ")" << endl;
  paramfile << "      parameter (xpixITR = "   << ITR_XPIX      << ")" << endl;
  paramfile << "      parameter (ypixITR = "   << ITR_YPIX      << ")" << endl;
  paramfile << "      parameter (nviewMSD = "  << MSD_NVIEW     << ")" << endl;
  paramfile << "      parameter (nlayMSD = "   << MSD_NLAY      << ")" << endl;
  paramfile << "      parameter (xminMSD = "   << MSD_XMIN      << ")" << endl;
  paramfile << "      parameter (yminMSD = "   << MSD_YMIN      << ")" << endl;
  paramfile << "      parameter (dxMSD = "     << MSD_DX        << ")" << endl;
  paramfile << "      parameter (dyMSD = "     << MSD_DY        << ")" << endl;
  paramfile << "      parameter (xstripMSD = "   << MSD_XSTRIP    << ")" << endl;
  paramfile << "      parameter (ystripMSD = "   << MSD_YSTRIP    << ")" << endl;
  paramfile << "      parameter (nstripSCN = " << SCN_NSTRIP    << ")" << endl;
  paramfile << "      parameter (ncryCAL = "   << nCry+1        << ")" << endl;
  
  paramfile.close();
  
  return 0;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************
