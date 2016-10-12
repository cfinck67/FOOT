#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>

#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "Evento.h"

using namespace std;

int main(int argc, char *argv[])
{

  int status = 0, iL=0, NumProcessed=0, numfiles = 0, nread=0;
  TString outname("Out.root"), inname("In.txt");
  vector<TString> infiles; TString tmpSin;

  ifstream lista_file;
  char fname[200],linea[200];
  FILE *pfile;
  bool ReadError = false;
  int kSelWord=0, kSelDc = 0, kSelLyso = 0, maxevpro = 1000000000;
  int fragtrig=0;
  double Ethreshold = 0;

  static TTree *RootTree = 0;
  static Evento *pEv = 0;

  EVENT_STRUCT eve;

  ifstream infile;


  for (int i = 0; i < argc; i++){
    if(strcmp(argv[i],"-in") == 0) {
      inname = TString(argv[++i]);
    }
    if(strcmp(argv[i],"-out") == 0) {
      outname = TString(argv[++i]);
    }
    if(strcmp(argv[i],"-nev") == 0) {
      maxevpro = atoi(argv[++i]);
    }
    if(strcmp(argv[i],"-sel") == 0) {
      kSelWord = atoi(argv[++i]);
      kSelLyso = (int)(((float)kSelWord)/10);
      kSelDc = (int)(kSelWord - 10*kSelLyso);
      cout<<endl<<"Txt2Root: Selection word = "<<kSelWord<<" SelLyso= "<<kSelLyso<<" SelDc= "<<kSelDc<<endl<<endl;
    }
    if(strcmp(argv[i],"-iL") == 0) { iL = 1; }
    if(strcmp(argv[i],"-help") == 0) {
      cout<<"Conversion of fluka TXT file : usage -> Txt2Root [opts] "<<endl;
      cout<<" possible opts are:"<<endl;
      cout<<"   -in  file    : [def=In.txt] Root input file"<<endl;
      cout<<"   -out  file   : [def=Out.root] Root output file"<<endl;
      cout<<"   -sel selw    : [def=0] select ev: 1*dc + 10*lyso "<<endl;
      cout<<"   -iL        : [def=none] input file is a list of files"<<endl;
      cout<<"   -nev        : [def=Inf] Max no. of events to process"<<endl;
      return 1;
    }
  }

  if(iL==0) {
    infiles.push_back(inname);
  } else {
    lista_file.open(inname.Data());
    cout<<"Processing data from "<<inname.Data()<<" LIST file!"<<endl;
    while (lista_file.getline(linea, 200, '\n')) {
      sscanf(linea,"%s",fname);
      cout<<"Adding "<<fname<<" to the list of files to be processed!"<<endl;
      tmpSin= fname;
      infiles.push_back(tmpSin);
    }
    lista_file.close();
  }
  numfiles = infiles.size();

  TFile *f_out = new TFile(outname,"RECREATE");
  f_out->cd();


  RootTree = new TTree("EventTree","gsimay");

  RootTree->Branch("EventNumber",&eve.EventNumber,"EventNumber/I");
  RootTree->Branch("trn",&eve.trn,"trn/I");
  RootTree->Branch("trid",&eve.trid,"trid[trn]/I");
  RootTree->Branch("trgen",&eve.trgen,"trgen[trn]/I");
  RootTree->Branch("trcha",&eve.trcha,"trcha[trn]/I");
  RootTree->Branch("trreg",&eve.trreg,"trreg[trn]/I");
  RootTree->Branch("trbar",&eve.trbar,"trbar[trn]/I");
  RootTree->Branch("trdead",&eve.trdead,"trdead[trn]/I");
  RootTree->Branch("trpaid",&eve.trpaid,"trpaid[trn]/I");
  RootTree->Branch("trix",&eve.trix,"trix[trn]/D");
  RootTree->Branch("triy",&eve.triy,"triy[trn]/D");
  RootTree->Branch("triz",&eve.triz,"triz[trn]/D");
  RootTree->Branch("trfx",&eve.trfx,"trfx[trn]/D");
  RootTree->Branch("trfy",&eve.trfy,"trfy[trn]/D");
  RootTree->Branch("trfz",&eve.trfz,"trfz[trn]/D");
  RootTree->Branch("tripx",&eve.tripx,"tripx[trn]/D");
  RootTree->Branch("tripy",&eve.tripy,"tripy[trn]/D");
  RootTree->Branch("tripz",&eve.tripz,"tripz[trn]/D");
  RootTree->Branch("trfpx",&eve.trfpx,"trfpx[trn]/D");
  RootTree->Branch("trfpy",&eve.trfpy,"trfpy[trn]/D");
  RootTree->Branch("trfpz",&eve.trfpz,"trfpz[trn]/D");
  RootTree->Branch("trmass",&eve.trmass,"trmass[trn]/D");
  RootTree->Branch("trtime",&eve.trtime,"trtime[trn]/D");
  RootTree->Branch("tof",&eve.tof,"tof[trn]/D");
  RootTree->Branch("trlen",&eve.trlen,"trlen[trn]/D");
  
  RootTree->Branch("stn",&eve.stn,"stn/I");
  RootTree->Branch("stid",&eve.stid,"stid[stn]/I");
  RootTree->Branch("stinx",&eve.stinx,"stinx[stn]/D");
  RootTree->Branch("stiny",&eve.stiny,"stiny[stn]/D");
  RootTree->Branch("stinz",&eve.stinz,"stinz[stn]/D");
  RootTree->Branch("stinpx",&eve.stinpx,"stinpx[stn]/D");
  RootTree->Branch("stinpy",&eve.stinpy,"stinpy[stn]/D");
  RootTree->Branch("stinpz",&eve.stinpz,"stinpz[stn]/D");
  RootTree->Branch("stoutx",&eve.stoutx,"stoutx[stn]/D");
  RootTree->Branch("stouty",&eve.stouty,"stouty[stn]/D");
  RootTree->Branch("stoutz",&eve.stoutz,"stoutz[stn]/D");
  RootTree->Branch("stoutpx",&eve.stoutpx,"stoutpx[stn]/D");
  RootTree->Branch("stoutpy",&eve.stoutpy,"stoutpy[stn]/D");
  RootTree->Branch("stoutpz",&eve.stoutpz,"stoutpz[stn]/D");
  RootTree->Branch("stde",&eve.stde,"stde[stn]/D");
  RootTree->Branch("stal",&eve.stal,"stal[stn]/D");
  RootTree->Branch("sttim",&eve.sttim,"sttim[stn]/D");
  
  RootTree->Branch("nvtx",&eve.nvtx,"nvtx/I");
  RootTree->Branch("idvtx",&eve.idvtx,"idvtx[nvtx]/I");
  RootTree->Branch("iplavtx",&eve.iplavtx,"iplavtx[nvtx]/I");
  RootTree->Branch("irowvtx",&eve.irowvtx,"irowvtx[nvtx]/I");
  RootTree->Branch("icolvtx",&eve.icolvtx,"icolvtx[nvtx]/I");
  RootTree->Branch("xinvtx",&eve.xinvtx,"xinvtx[nvtx]/D");
  RootTree->Branch("yinvtx",&eve.yinvtx,"yinvtx[nvtx]/D");
  RootTree->Branch("zinvtx",&eve.zinvtx,"zinvtx[nvtx]/D");
  RootTree->Branch("pxinvtx",&eve.pxinvtx,"pxinvtx[nvtx]/D");
  RootTree->Branch("pyinvtx",&eve.pyinvtx,"pyinvtx[nvtx]/D");
  RootTree->Branch("pzinvtx",&eve.pzinvtx,"pzinvtx[nvtx]/D");
  RootTree->Branch("xoutvtx",&eve.xoutvtx,"xoutvtx[nvtx]/D");
  RootTree->Branch("youtvtx",&eve.youtvtx,"youtvtx[nvtx]/D");
  RootTree->Branch("zoutvtx",&eve.zoutvtx,"zoutvtx[nvtx]/D");
  RootTree->Branch("pxoutvtx",&eve.pxoutvtx,"pxoutvtx[nvtx]/D");
  RootTree->Branch("pyoutvtx",&eve.pyoutvtx,"pyoutvtx[nvtx]/D");
  RootTree->Branch("pzoutvtx",&eve.pzoutvtx,"pzoutvtx[nvtx]/D");
  RootTree->Branch("devtx",&eve.devtx,"devtx[nvtx]/D");
  RootTree->Branch("alvtx",&eve.alvtx,"alvtx[nvtx]/D");
  RootTree->Branch("timvtx",&eve.timvtx,"timvtx[nvtx]/D");

  RootTree->Branch("nmon",&eve.nmon,"nmon/I");
  RootTree->Branch("idmon",&eve.idmon,"idmon[nmon]/I");
  RootTree->Branch("ilayer",&eve.ilayer,"ilayer[nmon]/I");
  RootTree->Branch("iview",&eve.iview,"iview[nmon]/I");
  RootTree->Branch("icell",&eve.icell,"icell[nmon]/I");
  RootTree->Branch("xinmon",&eve.xinmon,"xinmon[nmon]/D");
  RootTree->Branch("yinmon",&eve.yinmon,"yinmon[nmon]/D");
  RootTree->Branch("zinmon",&eve.zinmon,"zinmon[nmon]/D");
  RootTree->Branch("pxinmon",&eve.pxinmon,"pxinmon[nmon]/D");
  RootTree->Branch("pyinmon",&eve.pyinmon,"pyinmon[nmon]/D");
  RootTree->Branch("pzinmon",&eve.pzinmon,"pzinmon[nmon]/D");
  RootTree->Branch("xoutmon",&eve.xoutmon,"xoutmon[nmon]/D");
  RootTree->Branch("youtmon",&eve.youtmon,"youtmon[nmon]/D");
  RootTree->Branch("zoutmon",&eve.zoutmon,"zoutmon[nmon]/D");
  RootTree->Branch("pxoutmon",&eve.pxoutmon,"pxoutmon[nmon]/D");
  RootTree->Branch("pyoutmon",&eve.pyoutmon,"pyoutmon[nmon]/D");
  RootTree->Branch("pzoutmon",&eve.pzoutmon,"pzoutmon[nmon]/D");
  RootTree->Branch("demon",&eve.demon,"demon[nmon]/D");
  RootTree->Branch("almon",&eve.almon,"almon[nmon]/D");
  RootTree->Branch("timmon",&eve.timmon,"timmon[nmon]/D");

  RootTree->Branch("n2dc",&eve.n2dc,"n2dc/I");
  RootTree->Branch("id2dc",&eve.id2dc,"id2dc[n2dc]/I");
  RootTree->Branch("ipla2dc",&eve.ipla2dc,"ipla2dc[n2dc]/I");
  RootTree->Branch("iview2dc",&eve.iview2dc,"iview2dc[n2dc]/I");
  RootTree->Branch("icell2dc",&eve.icell2dc,"icell2dc[n2dc]/I");
  RootTree->Branch("xin2dc",&eve.xin2dc,"xin2dc[n2dc]/D");
  RootTree->Branch("yin2dc",&eve.yin2dc,"yin2dc[n2dc]/D");
  RootTree->Branch("zin2dc",&eve.zin2dc,"zin2dc[n2dc]/D");
  RootTree->Branch("pxin2dc",&eve.pxin2dc,"pxin2dc[n2dc]/D");
  RootTree->Branch("pyin2dc",&eve.pyin2dc,"pyin2dc[n2dc]/D");
  RootTree->Branch("pzin2dc",&eve.pzin2dc,"pzin2dc[n2dc]/D");
  RootTree->Branch("xout2dc",&eve.xout2dc,"xout2dc[n2dc]/D");
  RootTree->Branch("yout2dc",&eve.yout2dc,"yout2dc[n2dc]/D");
  RootTree->Branch("zout2dc",&eve.zout2dc,"zout2dc[n2dc]/D");
  RootTree->Branch("pxout2dc",&eve.pxout2dc,"pxout2dc[n2dc]/D");
  RootTree->Branch("pyout2dc",&eve.pyout2dc,"pyout2dc[n2dc]/D");
  RootTree->Branch("pzout2dc",&eve.pzout2dc,"pzout2dc[n2dc]/D");
  RootTree->Branch("de2dc",&eve.de2dc,"de2dc[n2dc]/D");
  RootTree->Branch("al2dc",&eve.al2dc,"al2dc[n2dc]/D");
  RootTree->Branch("tim2dc",&eve.tim2dc,"tim2dc[n2dc]/D");

  RootTree->Branch("nscint",&eve.nscint,"nscint/I");
  RootTree->Branch("idscint",&eve.idscint,"idscint[nscint]/I");
  RootTree->Branch("irowscint",&eve.irowscint,"irowscint[nscint]/I");
  RootTree->Branch("icolscint",&eve.icolscint,"icolscint[nscint]/I");
  RootTree->Branch("xinscint",&eve.xinscint,"xinscint[nscint]/D");
  RootTree->Branch("yinscint",&eve.yinscint,"yinscint[nscint]/D");
  RootTree->Branch("zinscint",&eve.zinscint,"zinscint[nscint]/D");
  RootTree->Branch("pxinscint",&eve.pxinscint,"pxinscint[nscint]/D");
  RootTree->Branch("pyinscint",&eve.pyinscint,"pyinscint[nscint]/D");
  RootTree->Branch("pzinscint",&eve.pzinscint,"pzinscint[nscint]/D");
  RootTree->Branch("xoutscint",&eve.xoutscint,"xoutscint[nscint]/D");
  RootTree->Branch("youtscint",&eve.youtscint,"youtscint[nscint]/D");
  RootTree->Branch("zoutscint",&eve.zoutscint,"zoutscint[nscint]/D");
  RootTree->Branch("pxoutscint",&eve.pxoutscint,"pxoutscint[nscint]/D");
  RootTree->Branch("pyoutscint",&eve.pyoutscint,"pyoutscint[nscint]/D");
  RootTree->Branch("pzoutscint",&eve.pzoutscint,"pzoutscint[nscint]/D");
  RootTree->Branch("descint",&eve.descint,"descint[nscint]/D");
  RootTree->Branch("alscint",&eve.alscint,"alscint[nscint]/D");
  RootTree->Branch("timscint",&eve.timscint,"timscint[nscint]/D");
  
  RootTree->Branch("ncry",&eve.ncry,"ncry/I");
  RootTree->Branch("idcry",&eve.idcry,"idcry[ncry]/I");
  RootTree->Branch("irowcry",&eve.irowcry,"irowcry[ncry]/I");
  RootTree->Branch("icolcry",&eve.icolcry,"icolcry[ncry]/I");
  RootTree->Branch("xincry",&eve.xincry,"xincry[ncry]/D");
  RootTree->Branch("yincry",&eve.yincry,"yincry[ncry]/D");
  RootTree->Branch("zincry",&eve.zincry,"zincry[ncry]/D");
  RootTree->Branch("pxincry",&eve.pxincry,"pxincry[ncry]/D");
  RootTree->Branch("pyincry",&eve.pyincry,"pyincry[ncry]/D");
  RootTree->Branch("pzincry",&eve.pzincry,"pzincry[ncry]/D");
  RootTree->Branch("xoutcry",&eve.xoutcry,"xoutcry[ncry]/D");
  RootTree->Branch("youtcry",&eve.youtcry,"youtcry[ncry]/D");
  RootTree->Branch("zoutcry",&eve.zoutcry,"zoutcry[ncry]/D");
  RootTree->Branch("pxoutcry",&eve.pxoutcry,"pxoutcry[ncry]/D");
  RootTree->Branch("pyoutcry",&eve.pyoutcry,"pyoutcry[ncry]/D");
  RootTree->Branch("pzoutcry",&eve.pzoutcry,"pzoutcry[ncry]/D");
  RootTree->Branch("decry",&eve.decry,"decry[ncry]/D");
  RootTree->Branch("alcry",&eve.alcry,"alcry[ncry]/D");
  RootTree->Branch("timcry",&eve.timcry,"timcry[ncry]/D");

  RootTree->Branch("ncross",&eve.ncross,"ncross/I");
  RootTree->Branch("idcross",&eve.idcross,"idcross[ncross]/I");
  RootTree->Branch("nregcross",&eve.nregcross,"nregcross[ncross]/I");
  RootTree->Branch("nregoldcross",&eve.nregoldcross,"nregoldcross[ncross]/I");
  RootTree->Branch("xcross",&eve.xcross,"xcross[ncross]/D");
  RootTree->Branch("ycross",&eve.ycross,"ycross[ncross]/D");
  RootTree->Branch("zcross",&eve.zcross,"zcross[ncross]/D");
  RootTree->Branch("pxcross",&eve.pxcross,"pxcross[ncross]/D");
  RootTree->Branch("pycross",&eve.pycross,"pycross[ncross]/D");
  RootTree->Branch("pzcross",&eve.pzcross,"pzcross[ncross]/D");
  RootTree->Branch("mcross",&eve.mcross,"mcross[ncross]/D");
  RootTree->Branch("chcross",&eve.chcross,"chcross[ncross]/D");
  RootTree->Branch("tcross",&eve.tcross,"tcross[ncross]/D");

  pEv = new Evento();

  //    loop sui file della lista ( if any)

  for(int idfile=0; idfile<numfiles;idfile++){
    cout<<endl<<"Now processing data from "<<infiles.at(idfile)<<" file!"<<endl;
    ReadError = false;
    
    pfile = fopen(infiles.at(idfile),"r");
    
    nread= fscanf(pfile,"%d %lf\n",&fragtrig,&Ethreshold);   
    if(nread!=2){
      ReadError= true;
      cout<<"Wrong run header: read Error!! exiting  "<<endl;
      return 1;
    }
    
    //	  loop sugli eventi del file  

    while((!feof(pfile))&&(!ReadError)){
      NumProcessed++;
      if (maxevpro>0) {
	if (NumProcessed>maxevpro) break;
      }
      
      eve.EventNumber = 0;
      eve.trn      = 0;
      eve.stn       = 0;
      eve.nvtx      = 0;
      eve.nmon      = 0;
      eve.n2dc      = 0;
      eve.nscint    = 0;
      eve.ncry      = 0;
      eve.ncross    = 0;

      if(NumProcessed%10000==0){ cout<<"# event = "<<NumProcessed<<endl;}
      status = pEv->Clean();

      //	leggo l'header

      nread= fscanf(pfile,"%d %d %d %d %d %d %d %d %d \n",&eve.EventNumber,
		    &eve.trn,&eve.stn,&eve.nmon,&eve.nvtx,&eve.n2dc,
		    &eve.nscint,&eve.ncry,&eve.ncross);
      if(nread!=9){
	cout<<"ReadError in ev header section: nread = "<<nread<<
	  " instead of 9; ev= "<<NumProcessed<<endl;
	ReadError = true;
      }
      
      //	leggo il common della cinematica

      if(!ReadError){
	for(int jj =0;jj<eve.trn;jj++){
	  nread = fscanf(pfile,
			 "%d %d %d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n ",
			 &eve.trid[jj],&eve.trgen[jj],&eve.trcha[jj],//trid->idpa
			 &eve.trreg[jj],&eve.trbar[jj],&eve.trdead[jj],
			 &eve.trpaid[jj],&eve.trix[jj],//trpaid->jpa
			 &eve.triy[jj],&eve.triz[jj],&eve.trfx[jj],&eve.trfy[jj],
			 &eve.trfz[jj],&eve.tripx[jj],&eve.tripy[jj],&eve.tripz[jj],
			 &eve.trfpx[jj],&eve.trfpy[jj],&eve.trfpz[jj],&eve.trmass[jj],
			 &eve.trtime[jj],&eve.tof[jj],&eve.trlen[jj]);
	  if(nread!=23){
	    cout<<"ReadError in kine section: nread = "<<nread<<
	      " instead of 23; ev= "<<NumProcessed<<endl;
	    
	    ReadError= true;
	    break;
	  }
	}
      }

      //	leggo i rilasci nello start counter

      if(!ReadError){
	for(int jj=0; jj<eve.stn;jj++){
	  nread= fscanf(pfile,
			"%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			&eve.stid[jj],
			&eve.stinx[jj],&eve.stiny[jj],&eve.stinz[jj],
			&eve.stoutx[jj],&eve.stouty[jj],&eve.stoutz[jj],
			&eve.stinpx[jj],&eve.stinpy[jj],&eve.stinpz[jj],
			&eve.stoutpx[jj],&eve.stoutpy[jj],&eve.stoutpz[jj],
			&eve.stde[jj],&eve.stal[jj],&eve.sttim[jj]);
	  if(nread!=16){
	    ReadError= true;
	    cout<<"ReadError in sc: nread = "<<nread<<
	      " instead of 16; ev= "<<NumProcessed<<endl;
	    break;
	  }
	}
      }

      //	leggo i rilasci nella prima drift chamber

      if(!ReadError){
 
	for(int jj=0; jj<eve.nmon;jj++){
	  nread = fscanf(pfile,
			 "%d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			 &eve.idmon[jj],&eve.iview[jj],&eve.ilayer[jj],
			 &eve.icell[jj],
			 &eve.xinmon[jj],&eve.yinmon[jj],&eve.zinmon[jj],
			 &eve.xoutmon[jj],&eve.youtmon[jj],&eve.zoutmon[jj],
			 &eve.pxinmon[jj],&eve.pyinmon[jj],&eve.pzinmon[jj],
			 &eve.pxoutmon[jj],&eve.pyoutmon[jj],&eve.pzoutmon[jj],
			 &eve.demon[jj],&eve.almon[jj],&eve.timmon[jj]);
	  if(nread!=19){
	    ReadError = true;
	    cout<<"ReadError in mon section: nread = "<<nread<<
	      " instead of 19; ev= "<<NumProcessed<<endl;
	    break;
	  }
	}
      }
      
      //	leggo i rilasci nel vertice

      if(!ReadError){
 
	for(int jj=0; jj<eve.nvtx;jj++){
	  nread = fscanf(pfile,
			 "%d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			 &eve.idvtx[jj],&eve.iplavtx[jj],&eve.irowvtx[jj],
			 &eve.icolvtx[jj],
			 &eve.xinvtx[jj],&eve.yinvtx[jj],&eve.zinvtx[jj],
			 &eve.xoutvtx[jj],&eve.youtvtx[jj],&eve.zoutvtx[jj],
			 &eve.pxinvtx[jj],&eve.pyinvtx[jj],&eve.pzinvtx[jj],
			 &eve.pxoutvtx[jj],&eve.pyoutvtx[jj],&eve.pzoutvtx[jj],
			 &eve.devtx[jj],&eve.alvtx[jj],&eve.timvtx[jj]);
	  if(nread!=19){
	    ReadError = true;
	    cout<<"ReadError in vertex section: nread = "<<nread<<
	      " instead of 19; ev= "<<NumProcessed<<endl;
	    break;
	  }
	}
      }

      //	leggo i rilasci nella seconda drift chamber

      if(!ReadError){
 
	for(int jj=0; jj<eve.n2dc;jj++){
	  nread = fscanf(pfile,
			 "%d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			 &eve.id2dc[jj],&eve.iview2dc[jj],&eve.ipla2dc[jj],
			 &eve.icell2dc[jj],
			 &eve.xin2dc[jj],&eve.yin2dc[jj],&eve.zin2dc[jj],
			 &eve.xout2dc[jj],&eve.yout2dc[jj],&eve.zout2dc[jj],
			 &eve.pxin2dc[jj],&eve.pyin2dc[jj],&eve.pzin2dc[jj],
			 &eve.pxout2dc[jj],&eve.pyout2dc[jj],&eve.pzout2dc[jj],
			 &eve.de2dc[jj],&eve.al2dc[jj],&eve.tim2dc[jj]);
	  if(nread!=19){
	    ReadError = true;
	    cout<<"ReadError in 2dc section: nread = "<<nread<<
	      " instead of 19; ev= "<<NumProcessed<<endl;
	    break;
	  }
	}
      }

      //	leggo i rilasci nello scint

      if(!ReadError){
	for(int jj=0; jj<eve.nscint;jj++){
	  nread= fscanf(pfile,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			&eve.idscint[jj],&eve.irowscint[jj],
			&eve.icolscint[jj],&eve.xinscint[jj],
			&eve.yinscint[jj],&eve.zinscint[jj],&eve.xoutscint[jj],
			&eve.youtscint[jj],&eve.zoutscint[jj],&eve.pxinscint[jj],
			&eve.pyinscint[jj],&eve.pzinscint[jj],&eve.pxoutscint[jj],
			&eve.pyoutscint[jj],&eve.pzoutscint[jj],&eve.descint[jj],
			&eve.alscint[jj],&eve.timscint[jj]);
	  if(nread!=18){
	    ReadError= true;
	    cout<<"ReadError in scint: nread = "<<nread<<
	      " instead of 18; ev= "<<NumProcessed<<endl;
	    break;
	  }
	}
      }

      //	leggo i rilasci nel calorimetro

      if(!ReadError){
	for(int jj=0; jj<eve.ncry;jj++){
	  nread= fscanf(pfile,
			"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			&eve.idcry[jj],&eve.irowcry[jj],&eve.icolcry[jj],
			&eve.xincry[jj],&eve.yincry[jj],&eve.zincry[jj],
			&eve.xoutcry[jj],&eve.youtcry[jj],&eve.zoutcry[jj],
			&eve.pxincry[jj],&eve.pyincry[jj],&eve.pzincry[jj],
			&eve.pxoutcry[jj],&eve.pyoutcry[jj],&eve.pzoutcry[jj],
			&eve.decry[jj],&eve.alcry[jj],&eve.timcry[jj]);
	  if(nread!=18){
	    ReadError= true;
	    cout<<"ReadError in pixel calo section: nread = "<<nread<<
	      " instead of 18; ev= "<<NumProcessed<<endl;
	    break;
	  }
	}
      }

      //	leggo i boundary crossing

      if(!ReadError){
	for(int jj=0; jj<eve.ncross;jj++){
	  nread = fscanf(pfile,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			 &eve.idcross[jj],&eve.nregcross[jj],&eve.nregoldcross[jj],&eve.xcross[jj],
			 &eve.ycross[jj],&eve.zcross[jj],&eve.pxcross[jj],
			 &eve.pycross[jj],&eve.pzcross[jj],&eve.mcross[jj],
			 &eve.chcross[jj],&eve.tcross[jj]);
	  if(nread!=12){
	    cout<<"ReadError in cross section: nread = "<<nread<<
	      " instead of 12; ev= "<<NumProcessed<<endl;
	    ReadError = true;
	    break;
	  }
	}
      }


      if( ReadError){
	break;
      }
      else{
	if((eve.trn<=MAXNUMP)&&(eve.stn<=MAXSC)&&(eve.nvtx<=MAXVTX)
	   &&(eve.nmon<=MAX1DC)&&(eve.n2dc<=MAX2DC)&&(eve.nscint<=MAXSCINT)
	   &&(eve.ncry<=MAXCRY)&&(eve.ncross<=MAXCROSS)){
	  RootTree->Fill() ;
	}
	else{
	  cout<<ReadError<<" "<<eve.EventNumber<<" "<<eve.trn
	      <<" "<<eve.stn<<" "<<eve.nmon<<" "<<eve.nvtx<<" "<<eve.n2dc
	      <<" "<<eve.nscint<<" "<<eve.ncry<<" "<<eve.ncross<<endl;  
	}
      }

    }
     
    fclose(pfile);
  }
  RootTree->Write();
  
  f_out->Close();
  cout<<" total number of event safely converted= "<<NumProcessed<<endl;

  return 0;
  
}
