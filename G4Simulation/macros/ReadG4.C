/*
 * Task          : Readout of Geant4 output
 * Author        : Marie Vanstalle
 * Date          : 28/03/19
 * Framework     : FOOT project
 */

#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TString.h>
#include <TStyle.h>
#include <TFile.h>
#include <TH1.h>
#include <TMath.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLegend.h>
#include <TRandom.h>
#include <iostream>

#include "TAMCevent.hxx"
#include "TAMCntuHit.hxx"
#include "TAMCntuEve.hxx"


void ReadG4()
{
    TString fInName = "ionCa1000.root";
    TFile *fIn = new TFile(fInName.Data(),"read") ;
    TTree *tree = (TTree*)fIn->Get("EventTree") ;
    TAMCntuHit *listOfHitsStc = new TAMCntuHit() ;
    TAMCntuHit *listOfHitsBmn = new TAMCntuHit() ;
    TAMCntuHit *listOfHitsVtx = new TAMCntuHit() ;
    TAMCntuHit *listOfHitsTof = new TAMCntuHit() ;
    TAMChit *hit = new TAMChit();

    tree->SetBranchAddress(TAMCntuHit::GetStcBranchName(),&listOfHitsStc) ;
    tree->SetBranchAddress(TAMCntuHit::GetBmBranchName(),&listOfHitsBmn) ;
    tree->SetBranchAddress(TAMCntuHit::GetVtxBranchName(),&listOfHitsVtx) ;
    tree->SetBranchAddress(TAMCntuHit::GetTofBranchName(),&listOfHitsTof) ;

    int trackId ;
    TVector3 outPos(0,0,0) ;
    int nEntries = tree->GetEntries() ;
    int nHitsBm = 0 ;

    TH1F *hXoutBm = new TH1F("hXoutBm","",100,-10.0,10.0);
    TH1F *hYoutBm = new TH1F("hYoutBm","",100,-10.0,10.0);
    TH1F *hZoutBm = new TH1F("hZoutBm","",100,-30.0,30.0);

    for(int k=0 ; k<nEntries ; k++){
        tree->GetEntry(k) ;
        nHitsBm = listOfHitsBmn->GetHitsN();
        hit->Clear();
        for(int j=0 ; j<nHitsBm ; j++){
            hit = (TAMChit*)listOfHitsBmn->GetHit(j);
            outPos = hit->GetOutPosition();
            hXoutBm->Fill(outPos[0]);
            hYoutBm->Fill(outPos[1]);
            hZoutBm->Fill(outPos[2]);
        }
    }

    //-----------------------------
    // Drawing

    TCanvas *canvas1 = new TCanvas ("canvas1","X pos",10,10,900,600) ;
    canvas1->cd() ;
    hXoutBm->SetLineColor(kBlue);
    hXoutBm->SetLineWidth(1);
    hXoutBm->SetTitle("");
    hXoutBm->Draw("hist") ;

    TCanvas *canvas2 = new TCanvas ("canvas2","Y pos",10,10,900,600) ;
    canvas2->cd() ;
    hYoutBm->SetLineColor(kBlue);
    hYoutBm->SetLineWidth(1);
    hYoutBm->SetTitle("");
    hYoutBm->Draw("hist") ;

    TCanvas *canvas3 = new TCanvas ("canvas3","Z pos",10,10,900,600) ;
    canvas3->cd() ;
    hZoutBm->SetLineColor(kBlue);
    hZoutBm->SetLineWidth(1);
    hZoutBm->SetTitle("");
    hZoutBm->Draw("hist") ;
    
}




