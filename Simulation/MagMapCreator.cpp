#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TVector3.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Getline.h"
#include "foot_geo.h"


void MagMapCreator() {
  
  ifstream fin;
  int nx, ny, nz, nz_new;
  double zMag1, zMag2, passo;
  double x, y, z, bx, by, bz;
  double zmin, zmax;
  string junk, mys;
  TVector3 pos, mag;
  vector<TVector3> tmp_vpos, tmp_vmag;
  vector<TVector3> vpos, vmag;
  vector<TVector3> vmag1, vmag2;
  FILE *fout;
  TFile *f_out = new TFile("MagneticField.root","RECREATE");
  // zMag1 = MAG_Z-MAG_DIST/2.-MAG_CV_LENGTH/2.;
  // zMag2 = MAG_Z+MAG_DIST/2.+MAG_CV_LENGTH/2.;
  zMag1 = MAG_Z-MAG_DIST/2.;
  zMag2 = MAG_Z+MAG_DIST/2.;
  passo = 0.5;//cm
  
  // TCanvas *c  = new TCanvas("c", "c");    c->Divide(1,3);
  // TCanvas *c1 = new TCanvas("c1", "c1"); c1->Divide(1,3);
  // TCanvas *c2 = new TCanvas("c2", "c2"); c2->Divide(1,3);
  // TCanvas *c3 = new TCanvas("c3", "c3"); c3->Divide(1,3);
  // TH1D *h = new TH1D("h","h",160,-20,60);
  TH2D *h1 = new TH2D("h1","h1",300,-30,70,100,0,1.5);
  TH2D *h2 = new TH2D("h2","h2",300,-30,70,100,0,1.5);
  TH2D *h3 = new TH2D("h3","h3",60,-30,0,100,0,1.5);
  TH2D *h4 = new TH2D("h4","h4",300,-30,70,100,-1.5,1.5);
  
  f_out->cd();

  TH2D *hBx_xy = new TH2D("hBx_xy","hBx_xy",20,-5.,5.,20,5.,5.);
  TH2D *hBx_xz = new TH2D("hBx_xz","hBx_xz",20,-5.,5.,200,-30,70);
  TH2D *hBx_yz = new TH2D("hBx_yz","hBx_yz",20,-5.,5.,200,-30,70);
  TH2D *hBy_xy = new TH2D("hBy_xy","hBy_xy",20,-5.,5.,20,5.,5.);
  TH2D *hBy_xz = new TH2D("hBy_xz","hBy_xz",20,-5.,5.,200,-30,70);
  TH2D *hBy_yz = new TH2D("hBy_yz","hBy_yz",20,-5.,5.,200,-30,70);
  TH2D *hBz_xy = new TH2D("hBz_xy","hBz_xy",20,-5.,5.,20,5.,5.);
  TH2D *hBz_xz = new TH2D("hBz_xz","hBz_xz",20,-5.,5.,200,-30,70);
  TH2D *hBz_yz = new TH2D("hBz_yz","hBz_yz",20,-5.,5.,200,-30,70);
  TH2D *hB_xy  = new TH2D("hB_xy","hB_xy",  20,-5.,5.,20,5.,5.);
  TH2D *hB_xz  = new TH2D("hB_xz","hB_xz",  20,-5.,5.,200,-30,70);
  TH2D *hB_yz  = new TH2D("hB_yz","hB_yz",  20,-5.,5.,200,-30,70);
  
  f_out->cd("..");

  cout << endl;
  cout << "Z coord center Mag1 = " << zMag1 << endl;
  cout << "Z coord center Mag2 = " << zMag2 << endl;
  cout << endl;
  
  fin.open("SingleMagLPM10cm/Single.table");

  // cout << "Leggo la mappa di Sanelli\n" << endl;

  fin >> nz >> ny >> nx >> junk;
  cout << "nx = " << nx << ", ny = " << ny << ", nz = "<< nz << endl;
  
  istringstream iss(mys);
  for (int i=0;i<8;i++) getline(fin,mys,'\n'), cout << mys << endl;

  while (!fin.eof()) {
  
    fin >> x >> y >> z >> bx >> by >> bz;

    pos.SetX(x);
    pos.SetY(y);
    pos.SetZ(z);
    mag.SetX(bx);
    mag.SetY(by);
    mag.SetZ(bz);

    // if(pos.X()==0&&pos.Y()==0) h3->Fill(pos.Z(),mag.Y()*1e-4);
    
    //Rot original map --> Rotate both mag
    // mag.RotateX(TMath::Pi()/180 * 1);
    // mag.RotateY(TMath::Pi()/180 * 1);
    
    tmp_vpos.push_back(pos);
    tmp_vmag.push_back(mag);

  }

  fin.close();


  // cout << tmp_vpos.front().Z() << endl;
  // cout << tmp_vpos.back().Z() << endl;
  // cout << tmp_vpos.size() << endl;
  // cout << nx*ny*nz << endl;
  
  
  if (nx*ny*nz != tmp_vpos.size()) cout << "***************************\n**Error in number of bins**\n***************************\n" << endl;
  
  fout = fopen("MagFieldMap.table","w");

  // cout << "Calcolo i campi completi per i singoli magneti" << endl;

  zmin = zMag1+tmp_vpos.front().Z();
  zmax = zMag2-tmp_vpos.front().Z();
  nz_new = (zmax - zmin)/passo + 1;
	
  for (int i=0; i<nx; i++){
    for (int j=0; j<ny; j++){
      for (int k=0; k<nz_new; k++){

	pos.SetXYZ(tmp_vpos.front().X() + i*passo,
		   tmp_vpos.front().Y() + j*passo,
		   zmin + k*passo );
	vpos.push_back(pos);
	
      }
    }
  }
  
  int count = -1;
  for (int i=0; i<nx*ny; i++){
    //riempio di zeri la prima parte della mappa magnetica del secondo magnete
    for (int k=0; k<(nz_new-2*nz+1); k++){
      vmag2.push_back(TVector3(0,0,0));
    }
    //metà gaussiane dei campi dei due magneti
    for (int k=0; k<nz; k++){
      count++;
      vmag1.push_back(tmp_vmag.at(count));
      vmag2.push_back(tmp_vmag.at(count));
      // mag.SetXYZ(tmp_vmag.at(count).X(),
      // 		 tmp_vmag.at(count).Y(),
      // 		 tmp_vmag.at(count).Z());
      // vmag2.push_back(mag);
      // if(pos.X()==0&&pos.Y()==0) {
      //   h1->Fill(vpos1.back().Z(),vmag1.back().Y()*1e-4);
      //   h2->Fill(vpos2.back().Z(),vmag2.back().Y()*1e-4);
      // }
    }
    //seconda metà della mappa in z (speculare)
    for (int k=1; k<nz; k++){
      vmag1.push_back(tmp_vmag.at(count-k));
      // vmag2.push_back(tmp_vmag.at(count-k));
      mag.SetXYZ(tmp_vmag.at(count-k).X(),
		 tmp_vmag.at(count-k).Y(),
		 tmp_vmag.at(count-k).Z());
      //Rot second mag
      // mag.RotateX(TMath::Pi()/180 * 1);
      // mag.RotateY(TMath::Pi()/180 * 1);
      vmag2.push_back(mag);
      // if(pos.X()==0&&pos.Y()==0) {
      //   h1->Fill(vpos1.back().Z(),vmag1.back().Y()*1e-4);
      //   h2->Fill(vpos2.back().Z(),vmag2.back().Y()*1e-4);
      // }
      }
    //riempio di zeri la parte finale della mappa magnetica del primo magnete
    for (int k=0; k<(nz_new-2*nz+1); k++){
      vmag1.push_back(TVector3(0,0,0));
    }
   
  }
  
  // cout <<vpos.size()<<endl;
  // cout <<vmag1.size()<<endl;
  // cout <<vmag2.size()<<endl;

  int size = vpos.size();

  fprintf(fout, "%d\t%d\t%d\t%d\n", size, nx, ny, nz_new);
  
  for (int i=0; i<nx*ny*nz_new; i++){

    vmag.push_back(vmag1.at(i)+vmag2.at(i));

    fprintf(fout, "%f\t%f\t%f\t%f\t%f\t%f\n", vpos.at(i).X(), vpos.at(i).Y(), vpos.at(i).Z(),
      	      vmag.at(i).X(), vmag.at(i).Y(), vmag.at(i).Z() );

    if( vpos.back().Z() == 0 ) hBx_xy->Fill(vpos.back().X(),vpos.back().Y(),vmag.back().X()*1.e-4);
    if( vpos.back().Y() == 0 ) hBx_xz->Fill(vpos.back().X(),vpos.back().Z(),vmag.back().X()*1.e-4);
    if( vpos.back().X() == 0 ) hBx_yz->Fill(vpos.back().Y(),vpos.back().Z(),vmag.back().X()*1.e-4);

    if( vpos.back().Z() == 0 ) hBy_xy->Fill(vpos.back().X(),vpos.back().Y(),vmag.back().Y()*1.e-4);
    if( vpos.back().Y() == 0 ) hBy_xz->Fill(vpos.back().X(),vpos.back().Z(),vmag.back().Y()*1.e-4);
    if( vpos.back().X() == 0 ) hBy_yz->Fill(vpos.back().Y(),vpos.back().Z(),vmag.back().Y()*1.e-4);
  
    if( vpos.back().Z() == 0 ) hBz_xy->Fill(vpos.back().X(),vpos.back().Y(),vmag.back().Z()*1.e-4);
    if( vpos.back().Y() == 0 ) hBz_xz->Fill(vpos.back().X(),vpos.back().Z(),vmag.back().Z()*1.e-4);
    if( vpos.back().X() == 0 ) hBz_yz->Fill(vpos.back().Y(),vpos.back().Z(),vmag.back().Z()*1.e-4);
  
    if( vpos.back().Z() == 0 ) hB_xy->Fill(vpos.back().X(),vpos.back().Y(),vmag.back().Mag()*1.e-4);
    if( vpos.back().Y() == 0 ) hB_xz->Fill(vpos.back().X(),vpos.back().Z(),vmag.back().Mag()*1.e-4);
    if( vpos.back().X() == 0 ) hB_yz->Fill(vpos.back().Y(),vpos.back().Z(),vmag.back().Mag()*1.e-4);
        
    if( vpos.back().X() == 0 && vpos.back().Y() == 0 ) h4->Fill(vpos.back().Z(),vmag.back().Y()*1e-4);
    
  }
    
  // cout << "\nFine costruzione dei campi completi per i singoli magneti " << endl;
  
  cout << endl;
  cout << "NZ = " << nz_new+1 <<endl;
  cout << "NROWS = " << vpos.size() << endl;
  cout << "z_min = " << vpos.front().Z() << endl;
  cout << "z_max = " << vpos.back().Z() << endl;
  cout << "MAG_AIR_LENGHT = " << vpos.back().Z()-vpos.front().Z() << endl;
  cout << endl;
   
  f_out->Write();
  f_out->Close();
  
}
 
