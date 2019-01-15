#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Getline.h"


void MagMapSum() {
  
  ifstream fin;
  int nx, ny, nz;
  double zMag1, zMag2;
  double x, y, z, bx, by, bz;
  vector<double> new_z1, new_z2;
  vector<double> tmpvx, tmpvy, tmpvz, tmpvbx, tmpvby, tmpvbz;
  vector<double> vx, vy, vz, vbx, vby, vbz;
  string mys;
  FILE *fout;

  zMag1 = MAG_Z-MAG_DIST/2.;
  zMag2 = MAG_Z+MAG_DIST/2.;
  nx = 21;
  ny = 21;
  nz = 61;

  cout << "Z coord center Mag1 = " << zMag1 << endl;
  cout << "Z coord center Mag2 = " << zMag2 << endl;
  
  fin.open("SingleMagLPM10cm/PMDIPFOOT_Single_8kGLPM10cm.table");

  while (!fin.eof()) {
  
    fin >> x >> y >> z >> bx >> by >> bz;

    tmpvx.push_back(x);
    tmpvy.push_back(y);
    tmpvz.push_back(z);
    tmpvbx.push_back(bx);
    tmpvby.push_back(by);
    tmpvbz.push_back(bz);

  }
  fin.close();
  // cout<<"ciao"<<endl;

  fout = fopen("SummedSingleMap.table","w");

  // for (int i=0; i<tmpvx.size(); i++){

  //   tmpnew_z1.push_back( (vz.at(i) + zMag1) );
  //   tmpnew_z2.push_back(vz.at(i) + zMag2);
  //   if (i<100)cout <<tmpnew_z1.at(i) << endl;

  // }

  // size = tmpvx.size();

  if (nx*ny*nz != tmpvz.size()) cout << "Error in number of bins" << endl;
  // cout << tmpvz.back()<<" "<<tmpvz.size()<< endl;
  cout << "\nInizio costruzione del campo completo" << endl;

  int count = -1;
  for (int i=0; i<nx; i++){
    for (int j=0; j<ny; j++){
      for (int k=0; k<nz; k++){
	count++;
	vx.push_back(tmpvx.at(count));
	vy.push_back(tmpvy.at(count));
	vz.push_back(tmpvz.at(count));
	new_z1.push_back(zMag1+tmpvz.at(count));
	new_z2.push_back(zMag2+tmpvz.at(count));
	vbx.push_back(tmpvbx.at(count));
	vby.push_back(tmpvby.at(count));
	vbz.push_back(tmpvbz.at(count));
	// fprintf(fout, "%f\t%f\t%f\t%f\t%f\t%f\n", vx.back(), vy.back(), new_z2.back(),
	// 	vbx.back(), vby.back(), vbz.back());
      }
      for (int m=1; m<nz; m++){
	vx.push_back(tmpvx.at(count-m));
	vy.push_back(tmpvy.at(count-m));
	new_z1.push_back(zMag1-tmpvz.at(count-m));
	new_z2.push_back(zMag2-tmpvz.at(count-m));
	vbx.push_back(tmpvbx.at(count-m));
	vby.push_back(tmpvby.at(count-m));
	vbz.push_back(tmpvbz.at(count-m));
	// fprintf(fout, "%f\t%f\t%f\t%f\t%f\t%f\n", vx.back(), vy.back(), new_z2.back(),
	// 	vbx.back(), vby.back(), vbz.back());
      }
    }
  }
  

  // TCanvas *cg = new TCanvas("c", "c");
  // TGraph *gr = new TGraph(vby.size(),&new_z1[0],&vby[0]);
  // TGraph *gr = new TGraph(tmpvby.size(),&tmpvz[0],&tmpvby[0]);
  // gr->Draw("APZ");
  // cout << new_z1.back()<<" "<<new_z2.back() << " "  <<new_z1.size()<< endl;
  
  cout << "\nFine costruzione dei campi" << endl;
  
  cout << "\nInizio somma dei campi" << endl;
  
  bool a = true; 
  for (int i=0; i<new_z1.size(); i++){
    if (i%1000==0) cout<<"Siamo a "<<i<<endl;
    
    if ( new_z1.at(i) < new_z2.front() )
      fprintf(fout, "%f\t%f\t%f\t%f\t%f\t%f\n", vx.at(i), vy.at(i), new_z1.at(i),
	      vbx.at(i), vby.at(i), vbz.at(i));

    else if ( new_z2.at(i) > new_z1.back() )
      fprintf(fout, "%f\t%f\t%f\t%f\t%f\t%f\n", vx.at(i), vy.at(i), new_z2.at(i),
	      vbx.at(i), vby.at(i), vbz.at(i));
    else {
      // if (a){
	for (int j=0; j<new_z2.size(); j++){
	  	  
	  if ( vx.at(i) == vx.at(j) &&
	       vy.at(i) == vy.at(j) &&
	       new_z1.at(i) == new_z2.at(j) ){

	    fprintf(fout, "%f\t%f\t%f\t%f\t%f\t%f\n", vx.at(i), vy.at(i), new_z1.at(i),
		    vbx.at(i)+vbx.at(j), vby.at(i)+vby.at(j), vbz.at(i)+vbz.at(j));

	  //   a = false;
	  
	  // }
	}
      }
    }
    
    a = true;
    }
  cout << "\nFine somma dei campi" << endl;
  cout << endl;
 
}
