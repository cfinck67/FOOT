
#include <TROOT.h>
#include <TMath.h>

#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "TAVTparGeo.hxx"
#include "TAITparGeo.hxx"
#include "Materials.hxx"

#include "GlobalPar.hxx"



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

  // real coding !

  GlobalPar::Instance();
  GlobalPar::GetPar()->Print();

  // crea la lista dei materiali nel costruttore
  // leggi i materiali da file o definiti nella calsse stessa?
  // Materials listOfMaterials();
  // Materials* listOfMaterials = new Materials();
  // listOfMaterials->PrintMap();


  // GlobalFootGeo footGeo;
  TAVTparGeo vtxGeo;
  TAITparGeo itrGeo;

  //  si costruisce le coordinate di ogni oggetto geometrico e sensibile
  vtxGeo.InitGeo();
  itrGeo.InitGeo();

  // assegna ad ogni oggetto il materiale
  vtxGeo.AssignMaterial();
  // itrGeo.AssignMaterial();

  // assegna ad ogni oggetto se sta nel campo magnetico
  vtxGeo.AssignMagnetField();
  //itrGeo.AssignMaterial();


  // PRINT OUT foot.geo
  // per ora chiamati da qui, si puo fare una classe gestore separata se serve
  string geofileName = "foot.geonew";
  // string geofileName = "foot.geo";
  ofstream geofile;
  geofile.open( geofileName.c_str(), std::ofstream::out | std::ofstream::trunc );

  geofile << "    0    0          FOOT experiment geometry" << endl;
  geofile << "* ***Black Body" << endl;
  geofile << "RPP blk        -1000. 1000. -1000. 1000. -1000. 1000." << endl;
  geofile << "* ***Air -> no mag field" << endl;
  geofile << "RPP air        -900.0 900.0 -900.0 900.0 -900.0 900.0" << endl;

  geofile.close();

  vtxGeo.PrintBodies( geofileName );
  itrGeo.PrintBodies( geofileName );

  //prova geometria
 // itrGeo.GetVolume();
  geofile.open( geofileName.c_str(), std::ofstream::out | std::ofstream::app );
  geofile << "END        " <<endl;

  /*
     inizio definizione regioni
  */

  geofile <<"* ******************************************************************************"<<endl;
  geofile <<"*                         REGIONS                                              *"<<endl;
  geofile <<"* ******************************************************************************"<<endl;


  //prova regioni

  vtxGeo.PrintRegions( geofileName );
  itrGeo.PrintRegions( geofileName );

  geofile.close();
 // PRINT OUT foot.geo



  //  Print out of foot.inp


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
