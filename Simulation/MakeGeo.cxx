
#include <TROOT.h>
#include <TMath.h>

#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "TAIRparGeo.hxx"
#include "TABMparGeo.hxx"
#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TAMSDparGeo.hxx"
#include "TATWparGeo.hxx"
#include "TACAparGeo.hxx"

#include "Materials.hxx"

#include "GlobalPar.hxx"
#include "FootField.hxx"
#include <FieldManager.h>


using namespace std;

string PrintCard(TString fTitle, TString fWHAT1, TString fWHAT2, TString fWHAT3,
		 TString fWHAT4, TString fWHAT5, TString fWHAT6, TString fSDUM);

int main (int argc, char *argv[]) {

    clock_t start_tot, end_tot;

    // start time
    start_tot = clock();
    time_t now = time(0);
    tm *ltm = localtime(&now);
    cout << "Date: "<<  ltm->tm_mday <<" / "<< 1 + ltm->tm_mon
        <<" / "<< 1900 + ltm->tm_year << "  Time: "<< 1 + ltm->tm_hour << ":";
    cout << 1 + ltm->tm_min << ":";
    cout << 1 + ltm->tm_sec << endl;


    cout << "Hello Footer!" << endl;


    // real coding starts here!
    GlobalPar::Instance("GeoConfig.par");
    GlobalPar::GetPar()->Print();

    // crea la lista dei materiali nel costruttore
    // leggi i materiali da file o definiti nella calsse stessa?
    // Materials listOfMaterials();
    // Materials* listOfMaterials = new Materials();
    // listOfMaterials->PrintMap();

    genfit::FieldManager::getInstance()->init(new FootField( "DoubleDipole.table" ) ); // variable field
    // genfit::FieldManager::getInstance()->init(new FootField("DoubleGaussMag.table")); // variable field

    Materials* listMaterials = new Materials() ;
    listMaterials->PrintCompMap();

    // GlobalFootGeo footGeo;
    TAIRparGeo* stcGeo = new TAIRparGeo();
    TABMparGeo* bmGeo = new TABMparGeo();
    TAVTparGeo* vtxGeo = new TAVTparGeo();
    TAITparGeo* itrGeo = new TAITparGeo();
    TAMSDparGeo* msdGeo = new TAMSDparGeo();
    TATWparGeo* twGeo = new TATWparGeo();
    TACAparGeo* caGeo = new TACAparGeo();

    //  si costruisce le coordinate di ogni oggetto geometrico e sensibile
    stcGeo->InitGeo();
    bmGeo->InitGeo();
    bmGeo->ShiftBmon();
    vtxGeo->InitGeo();
    itrGeo->InitGeo();
    msdGeo->InitGeo();
    twGeo->InitGeo();
    caGeo->InitGeo();


    // assegna ad ogni oggetto se sta nel campo magnetico
    ifstream file;
    string fileName = "foot.inp";
    file.open( fileName.c_str(), ios::in );
    if ( !file.is_open() )        cout<< "ERROR  -->  wrong input in GlobalPar::ReadParemFile file "<< endl, exit(0);
    
    string line = "";
    stringstream init, geomat, end;
    bool readInit = true;
    bool readGeoMat = false;
    bool readEnd = false;
    // read the old file
    while( getline( file, line ) ) {  

      if ( readInit )           init    << line << endl;

      if ( line == "* @@@START GENERATED, DO NOT MODIFY:GENERAL@@@ *********************************" ) 
	readInit = false;
      	  
      if ( line == "* @@@END GENERATED:GENERAL@@@ **************************************************" ) 
	readGeoMat = true;

      if ( readGeoMat )         geomat  << line << endl;
	  
      if ( line == "* @@@START GENERATED, DO NOT MODIFY:MATERIAL&MAGFIELD@@@ ***********************" ) 
	readGeoMat = false;
	  
      if ( line == "* @@@END GENERATED:MATERIAL&MAGFIELD@@@ ****************************************" ) 
	readEnd = true;

      if ( readEnd )            end     << line << endl;
      
    }

    
    file.close();

    // rewrite the file in the correct way
    ofstream outfile;
    outfile.open( fileName.c_str(), fstream::trunc );

    outfile << init.str();

    if ( GlobalPar::GetPar()->verFLUKA() )
       outfile << PrintCard("PHYSICS","1.","","","","","","COALESCE") << endl;
    else
      outfile << PrintCard("PHYSICS","12001.","1.","1.","","","","COALESCE") << endl;
    
    outfile << PrintCard("BEAM",TString::Format("%f",-PRIM_T),
			 TString::Format("%f",PRIM_dP),
			 TString::Format("%f",PRIM_DIV),
			 TString::Format("%f",-PRIM_RMAX),
			 TString::Format("%f",-PRIM_RMAX),
			 "1.0","HEAVYION") << endl;
    outfile << PrintCard("HI-PROPE",TString::Format("%f",(double)PRIM_Z),
			 TString::Format("%f",(double)PRIM_A),"","","","","")
	    << endl;
    outfile << PrintCard("BEAMPOS",TString::Format("%.3f",PRIM_Pos_X),
			 TString::Format("%.3f",PRIM_Pos_Y),
			 TString::Format("%.3f",PRIM_Pos_Z),
			 TString::Format("%.3f",PRIM_Pos_CX),
			 TString::Format("%.3f",PRIM_Pos_CY),"","") << endl;
    
    outfile << PrintCard("EMFCUT",TString::Format("%f",-TRANS_THRES_EM),
			 TString::Format("%f",TRANS_THRES_EM)
			 ,"","BLACK","@LASTREG","1.0","") << endl;
    outfile << PrintCard("EMFCUT",TString::Format("%f",-PROD_THRES_EM),
			 TString::Format("%f",PROD_THRES_EM),
			 "1.","BLCKHOLE","@LASTMAT","1.0","PROD-CUT") << endl;
    outfile << PrintCard("DELTARAY",TString::Format("%f",DELTA_THRES_EM),
			 "","","BLCKHOLE","@LASTMAT","1.0","") << endl;
    outfile << PrintCard("PAIRBREM","-3.","","","BLCKHOLE",
			 "@LASTMAT","","") << endl;

    outfile << geomat.str();

    outfile << "ASSIGNMA    BLCKHOLE     BLACK\n";
    outfile << "ASSIGNMA         AIR       AIR\n";

    outfile << stcGeo->PrintAssignMaterial();
    outfile << bmGeo->PrintAssignMaterial();
    outfile << "ASSIGNMA    Polyethy    TARGET                             1\n";
    outfile << vtxGeo->PrintAssignMaterial();
    outfile << itrGeo->PrintAssignMaterial();
    outfile << "ASSIGNMA        SmCo   MAG_PM0\n";
    outfile << "ASSIGNMA    ALUMINUM   MAG_CV0\n";
    outfile << "ASSIGNMA        SmCo   MAG_PM1\n";
    outfile << "ASSIGNMA    ALUMINUM   MAG_CV1\n";
    outfile << "ASSIGNMA         AIR   MAG_AIR                             1\n";
    outfile << msdGeo->PrintAssignMaterial();
    outfile << "ASSIGNMA         AIR       BOX\n";
    outfile << twGeo->PrintAssignMaterial();
    outfile << caGeo->PrintAssignMaterial();

    outfile << PrintCard("MGNFIELD",TString::Format("%f",MaxAng),
			 TString::Format("%f",BoundAcc),"",
			 TString::Format("%f",Bx),TString::Format("%f",By),
			 TString::Format("%f",Bz),"") << endl;
    // outfile << "MGNFIELD    0.100000  0.000010            0.000000  0.000000  0.000000" << endl;

    outfile << end.str();

    outfile.close();

    

    // PRINT OUT foot.geo
    // per ora chiamati da qui, si puo fare una classe gestore separata se serve
    string geofileName = "foot.geo";

    ofstream geofile;
    geofile.open( geofileName.c_str(), std::ofstream::out | std::ofstream::trunc );

    geofile << "    0    0          FOOT experiment geometry" << endl;
    geofile << "* ***Black Body" << endl;
    geofile << "RPP blk        -1000. 1000. -1000. 1000. -1000. 1000." << endl;
    geofile << "* ***Air -> no mag field" << endl;
    geofile << "RPP air        -900.0 900.0 -900.0 900.0 -900.0 900.0" << endl;

    // geofile.close();

    geofile << stcGeo->PrintBodies(  );
    geofile << bmGeo->PrintBodies(  );

    geofile << "* ***Target\n";
    geofile << "RPP tgt        " << ( TG_X - TG_WIDTH/2. ) << " "
	    << ( TG_X + TG_WIDTH/2. ) << " "
	    << ( TG_Y - TG_HEIGHT/2. ) << " "
	    << ( TG_Y + TG_HEIGHT/2. ) << " "
	    << ( TG_Z - TG_THICK/2. ) << " "
	    << ( TG_Z + TG_THICK/2. ) << endl;
    
    geofile << vtxGeo->PrintBodies(  );
    geofile << itrGeo->PrintBodies(  );
    
    geofile << "* ***Magnets\n";  /*  
    geofile << "RCC MagCvOu0   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_CV_LENGTH - MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_CV_LENGTH << " " << MAG_CV0_OUTRAD << endl;
    geofile << "RCC MagCvOu1   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z + MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_CV_LENGTH << " " << MAG_CV1_OUTRAD << endl;
    geofile << "RCC MagPMOu0   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_PM_LENGTH - MAG_CV_THICK - MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM0_OUTRAD << endl;
    geofile << "RCC MagPMOu1   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z + MAG_CV_THICK + MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM1_OUTRAD << endl;
    geofile << "RCC MagPMIn0   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_CV_THICK - MAG_PM_LENGTH - MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM0_INRAD << endl;
    geofile << "RCC MagPMIn1   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z + MAG_CV_THICK + MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM1_INRAD << endl;*/  
    geofile << "RCC MagCvOu0   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_CV_LENGTH/2. - MAG_DIST/2. << " 0.000000 0.000000 "
	    // << MAG_Z - MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_CV_LENGTH << " " << MAG_CV0_OUTRAD << endl;
    geofile << "RCC MagCvOu1   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_CV_LENGTH/2. + MAG_DIST/2. << " 0.000000 0.000000 "
	    // << MAG_Z + MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_CV_LENGTH << " " << MAG_CV1_OUTRAD << endl;
    geofile << "RCC MagPMOu0   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_PM_LENGTH/2. - MAG_DIST/2. << " 0.000000 0.000000 "
	    // << MAG_Z - MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM0_OUTRAD << endl;
    geofile << "RCC MagPMOu1   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_PM_LENGTH/2. + MAG_DIST/2. << " 0.000000 0.000000 "
	    // << MAG_Z + MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM1_OUTRAD << endl;
    geofile << "RCC MagPMIn0   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_PM_LENGTH/2. - MAG_DIST/2. << " 0.000000 0.000000 "
	    // << MAG_Z - MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM0_INRAD << endl;
    geofile << "RCC MagPMIn1   " << MAG_X << " " << MAG_Y << " "
	    << MAG_Z - MAG_PM_LENGTH/2. + MAG_DIST/2. << " 0.000000 0.000000 "
	    // << MAG_Z + MAG_DIST/2. << " 0.000000 0.000000 "
	    << MAG_PM_LENGTH << " " << MAG_PM1_INRAD << endl;
    geofile << "* ***Gap for magnets\n";
    geofile << "ZCC Gap0       0.000000 0.000000 " << MAG_CV0_INRAD << endl;
    geofile << "ZCC Gap1       0.000000 0.000000 " << MAG_CV1_INRAD << endl;
    geofile << "* ***Magnetic field air region\n";
    geofile << "RPP MagAir     " << MAG_X - MAG_AIR_WIDTH/2. << " " << MAG_X + MAG_AIR_WIDTH/2. << " "
	    << MAG_Y - MAG_AIR_HEIGHT/2. << " " << MAG_Y + MAG_AIR_HEIGHT/2. << " "
	    << MAG_Z - MAG_AIR_LENGTH/2. << " " << MAG_Z + MAG_AIR_LENGTH/2. << endl;
    
    geofile << msdGeo->PrintBodies(  );

    // cout << prova1 << endl;
    // cout << prova2 << endl;
    
    geofile << "* ***Air Box for Scintillator and Calorimeter\n";
    geofile << "RPP box     " << SCN_X - SCN_BAR_HEIGHT/2. - 1. << " "
	    << SCN_X + SCN_BAR_HEIGHT/2. + 1. << " "
	    << SCN_Y - SCN_BAR_HEIGHT/2. - 1. << " "
	    << SCN_Y + SCN_BAR_HEIGHT/2. + 1. << " "
	    << SCN_Z - SCN_BAR_THICK/2. - 1. << " "
	    << CAL_Z + CAL_CRY_THICK/2. +1. << endl;
    
    geofile << twGeo->PrintBodies(  );
    geofile << caGeo->PrintBodies(  );




    // print bodies
    // geofile.open( geofileName.c_str(), std::ofstream::out | std::ofstream::app );
    geofile << "END        " <<endl;


     // region print begins here
    geofile <<"* ******************************************************************************"<<endl;
    geofile <<"*                         REGIONS                                              *"<<endl;
    geofile <<"* ******************************************************************************"<<endl;

    //print  regioni

    geofile <<"BLACK        5 blk -air\n";
    geofile <<"* ***Air -> no mag field\n";
    geofile <<"AIR          5 air -stc -MagAir -(MagCvOu0 -Gap0) -(MagCvOu1 -Gap1) -box\n";
    geofile <<" -(BmnShiOu -BmnShiIn)\n";
    geofile <<" -(BmnShiIn -BmnMyl0 +BmnMyl3)\n";
    geofile << itrGeo->PrintSubtractBodiesFromAir();
    geofile << msdGeo->PrintSubtractBodiesFromAir();
    geofile <<"\n";

    geofile << stcGeo->PrintRegions(  );
    geofile << bmGeo->PrintRegions(  );
    
    geofile <<"* ***Target\n";
    geofile <<"TARGET       5 tgt\n";
    
    geofile << vtxGeo->PrintRegions(  );
    geofile << itrGeo->PrintRegions(  );
    
    geofile <<"* ***Magnets\n";
    geofile <<"MAG_PM0      5 MagPMOu0 -MagPMIn0\n";
    geofile <<"MAG_CV0      5 MagCvOu0 -(MagPMOu0 -MagPMIn0) -Gap0\n";
    geofile <<"MAG_PM1      5 MagPMOu1 -MagPMIn1\n";
    geofile <<"MAG_CV1      5 MagCvOu1 -(MagPMOu1 -MagPMIn1) -Gap1\n";
    geofile <<"* ***Magnetic field air region\n";
    geofile <<"MAG_AIR      5 MagAir -tgt -(BmnShiIn -BmnMyl0 +BmnMyl3) -(MagCvOu0 -Gap0) -(MagCvOu1 -Gap1) ";
    geofile << vtxGeo->PrintSubtractBodiesFromAir();
    geofile << itrGeo->PrintSubtractBodiesFromAir();
    geofile << msdGeo->PrintSubtractBodiesFromAir();
    geofile <<"\n";

    geofile << msdGeo->PrintRegions(  );

    geofile <<"* ***Air Box for Scintillator and Calorimeter\n";
    geofile <<"BOX          5 box ";
    geofile << twGeo->PrintSubtractBodiesFromAir();
    geofile << caGeo->PrintSubtractBodiesFromAir();
    geofile <<"\n";

    geofile << twGeo->PrintRegions(  );
    geofile << caGeo->PrintRegions(  );

    geofile << "END        " <<endl;
    geofile.close();


    ofstream paramfile;
    paramfile.open("ROUTINES/parameters.inc");
    
    paramfile << bmGeo->PrintParameters();
    paramfile << vtxGeo->PrintParameters();
    paramfile << itrGeo->PrintParameters();
    paramfile << msdGeo->PrintParameters();
    paramfile << twGeo->PrintParameters();
    paramfile << caGeo->PrintParameters();
    
    paramfile.close();
    

    // stop time
    end_tot = clock();
    double tempo=((double)(end_tot-start_tot ))/CLOCKS_PER_SEC;
    int t_h = trunc( tempo/3600 );
    int t_m = trunc( fmod(tempo, 3600)/60 );
    int t_s = trunc( fmod(fmod(tempo, 3600), 60) );
    cout<< "\n\nExecution Time: "<< tempo << " seconds" << endl;
    cout<< "Execution Time in human units: "<< t_h <<" : "<< t_m <<" : "<< t_s << endl;


    cout<<" Job done! "<<endl;
    return 0;
}




string PrintCard(TString fTitle, TString fWHAT1, TString fWHAT2, TString fWHAT3,
		 TString fWHAT4, TString fWHAT5, TString fWHAT6, TString fSDUM) {
  
  stringstream fLine;
	
  if (fTitle.Sizeof() != 10) fTitle.Resize(10);
  if (fSDUM.Sizeof() != 10) fSDUM.Resize(10);
  if (fWHAT1.Sizeof() > 10) fWHAT1.Resize(10);
  if (fWHAT2.Sizeof() > 10) fWHAT2.Resize(10);
  if (fWHAT3.Sizeof() > 10) fWHAT3.Resize(10);
  if (fWHAT4.Sizeof() > 10) fWHAT4.Resize(10);
  if (fWHAT5.Sizeof() > 10) fWHAT5.Resize(10);
  if (fWHAT6.Sizeof() > 10) fWHAT6.Resize(10);

  fLine << setw(10) << fTitle << setw(10) << fWHAT1 << setw(10) << fWHAT2
	<< setw(10) << fWHAT3 << setw(10) << fWHAT4 << setw(10) << fWHAT5
	<< setw(10) << fWHAT6 << setw(10) << fSDUM;
	
  return fLine.str();
  
}

