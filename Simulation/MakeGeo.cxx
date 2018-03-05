
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
    stringstream before, after;
    bool readBefore = true;
    bool readAfter = false;
    // read the old file
    while( getline( file, line ) ) {  

        if ( readBefore )        before << line << endl;

        if ( readAfter )         after << line << endl;

        if ( line == "* @@@START GENERATED, DO NOT MODIFY:MATERIAL&MAGFIELD@@@ ***********************" && readBefore ) { 
            readBefore = false;
            for (int i=0; i<6; i++) {
                getline( file, line );
                before << line << endl;
            }
        }
        else if ( line == "*" && !readBefore ) {
            readAfter = true;
            after << "*" << endl;
        }
    }
    file.close();

    // rewrite the file in the correct way
    ofstream outfile;
    outfile.open( fileName.c_str(), fstream::trunc );

    outfile << before.str();

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

    outfile << "MGNFIELD    0.100000  0.000010            0.000000  0.000000  0.000000" << endl;

    outfile << after.str();

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
    geofile << "RPP tgt        -0.750000 0.750000 -0.750000 0.750000 -0.100000 0.100000\n";
    
    geofile << vtxGeo->PrintBodies(  );
    geofile << itrGeo->PrintBodies(  );
    
    geofile << "* ***Magnets\n";
    geofile << "RCC MagCvOu0   0.000000 0.000000 2.800000 0.000000 0.000000 10.400000 15.000000\n";
    geofile << "RCC MagCvOu1   0.000000 0.000000 14.800000 0.000000 0.000000 10.400000 15.000000\n";
    geofile << "RCC MagPMOu0   0.000000 0.000000 3.000000 0.000000 0.000000 10.000000 14.800000\n";
    geofile << "RCC MagPMOu1   0.000000 0.000000 15.000000 0.000000 0.000000 10.000000 14.800000\n";
    geofile << "RCC MagPMIn0   0.000000 0.000000 3.000000 0.000000 0.000000 10.000000 4.800000\n";
    geofile << "RCC MagPMIn1   0.000000 0.000000 15.000000 0.000000 0.000000 10.000000 4.800000\n";
    geofile << "* ***Gap for magnets\n";
    geofile << "ZCC Gap0       0.000000 0.000000 4.600000\n";
    geofile << "ZCC Gap1       0.000000 0.000000 4.600000\n";
    geofile << "* ***Magnetic field air region\n";
    geofile << "RPP MagAir     -5.000000 5.000000 -5.000000 5.000000 -16.000000 44.000000\n";
    
    geofile << msdGeo->PrintBodies(  );
    
    geofile << "* ***Air Box for Scintillator and Calorimeter\n";
    geofile << "RPP box     -23.000000 23.000000 -23.000000 23.000000 99.400000 121.000000\n";
    
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












