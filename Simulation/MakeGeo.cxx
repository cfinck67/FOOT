
#include <TROOT.h>
#include <TMath.h>

#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "TAMSDparGeo.hxx"
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
    TAVTparGeo* vtxGeo = new TAVTparGeo();
    TAITparGeo* itrGeo = new TAITparGeo();
    TAMSDparGeo* msdGeo = new TAMSDparGeo();

    //  si costruisce le coordinate di ogni oggetto geometrico e sensibile
    vtxGeo->InitGeo();
    itrGeo->InitGeo();
    msdGeo->InitGeo();


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

    outfile << vtxGeo->PrintAssignMaterial();
    outfile << itrGeo->PrintAssignMaterial();
    outfile << msdGeo->PrintAssignMaterial();

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

    geofile.close();

    vtxGeo->PrintBodies( geofileName );
    itrGeo->PrintBodies( geofileName );
    msdGeo->PrintBodies( geofileName );

    // print bodies
    geofile.open( geofileName.c_str(), std::ofstream::out | std::ofstream::app );
    geofile << "END        " <<endl;


     // region print begins here
    geofile <<"* ******************************************************************************"<<endl;
    geofile <<"*                         REGIONS                                              *"<<endl;
    geofile <<"* ******************************************************************************"<<endl;

    //print  regioni
    vtxGeo->PrintRegions( geofileName );
    itrGeo->PrintRegions( geofileName );
    msdGeo->PrintRegions( geofileName );

    geofile.close();
    

    // stop time
    end_tot = clock();
    double tempo=((double)(end_tot-start_tot ))/CLOCKS_PER_SEC;
    int t_h = trunc( tempo/3600 );
    int t_m = trunc( fmod(tempo, 3600)/60 );
    int t_s = trunc( fmod(fmod(tempo, 3600), 60) );
    cout<< "Execution Time: "<< tempo << " seconds" << endl;
    cout<< "Execution Time in human units: "<< t_h <<" : "<< t_m <<" : "<< t_s << endl;


    cout<<" Job done! "<<endl;
    return 0;
}
