#include <stdio.h>
#include <iostream>
#include <fstream>
#include "TAMCevent.hxx"
#include "TAMCntuHit.hxx"
#include "TAMCntuEve.hxx"

using namespace std;

ClassImp(TAMCevent);

/*-----------------------------------------------------------------*/
TAMCevent::TAMCevent()
: fEventNumber(-1)
{
    fTrack  = new TAMCntuEve();
    fHitSTC = new TAMCntuHit();
    fHitBMN = new TAMCntuHit();
    fHitVTX = new TAMCntuHit();
    fHitITR = new TAMCntuHit();
    fHitMSD = new TAMCntuHit();
    fHitTW  = new TAMCntuHit();
    fHitCAL = new TAMCntuHit();
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::Clean()
{
    fTrack->Clear();
    fHitSTC->Clear();
    fHitBMN->Clear();
    fHitVTX->Clear();
    fHitITR->Clear();
    fHitMSD->Clear();
    fHitTW->Clear();
    fHitCAL->Clear();

    return 0;
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::AddPart(Int_t aTRpaid, Int_t aTRgen, Int_t aTRcharge, Int_t aTRreg,
                         Int_t aTRbaryon, Int_t aTRdead, Int_t aTRflukid, TVector3 aTRipos,
                         TVector3 aTRfpos, TVector3 aTRip, TVector3 aTRfp, Double_t aTRmass,
                         Double_t aTRtof, Double_t aTRtime, Double_t aTRtrlen)
{
    fTrack->NewHit(aTRflukid,aTRcharge,aTRgen,-1,aTRbaryon,-1,aTRmass,aTRpaid,aTRtime,aTRtof,aTRtrlen,aTRipos,aTRfpos,aTRip,aTRfp,TVector3(0,0,0),TVector3(0,0,0),-1);
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::AddSTC(Int_t aSTCid,
                        TVector3 aSTCinpos, TVector3 aSTCoutpos,
                        TVector3 aSTCpin, TVector3 aSTCpout,
                        Double_t aSTCde, Double_t aSTCal, Double_t aSTCtof)
{
    fHitSTC->NewHit(aSTCid,-99,-99,-99,aSTCinpos,aSTCoutpos,aSTCpin,aSTCpout,aSTCde,aSTCtof,-99);
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::AddBMN(Int_t aBMNid, Int_t aBMNilay, Int_t aBMNiview,
                        Int_t aBMNicell, TVector3 aBMNinpos, TVector3 aBMNoutpos,
                        TVector3 aBMNpin, TVector3 aBMNpout, Double_t aBMNde,
                        Double_t aBMNal, Double_t aBMNtof)
{
    fHitBMN->NewHit(aBMNid,aBMNilay,aBMNiview,aBMNicell,aBMNinpos,aBMNoutpos,aBMNpin,aBMNpout,aBMNde,aBMNtof,-99);
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::AddVTX(Int_t aVTXid, Int_t aVTXilay,
                        TVector3 aVTXinpos, TVector3 aVTXoutpos, TVector3 aVTXpin,
                        TVector3 aVTXpout, Double_t aVTXde, Double_t aVTXal,
                        Double_t aVTXtof)
{
    fHitVTX->NewHit(aVTXid,aVTXilay,-99,-99,aVTXinpos,aVTXoutpos,aVTXpin,aVTXpout,aVTXde,aVTXtof,-99);
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::AddITR(Int_t aITRid, Int_t aITRilay,
                        Int_t aITRiplume, Int_t aITRimimo,
                        TVector3 aITRinpos, TVector3 aITRoutpos, TVector3 aITRpin,
                        TVector3 aITRpout, Double_t aITRde, Double_t aITRal,
                        Double_t aITRtof)
{
    fHitITR->NewHit(aITRid,aITRilay,aITRiplume,aITRimimo,aITRinpos,aITRoutpos,aITRpin,aITRpout,aITRde,aITRtof,-99);
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::AddMSD(Int_t aMSDid, Int_t aMSDilay, Int_t aMSDistripx,
                        Int_t aMSDistripy, TVector3 aMSDinpos, TVector3 aMSDoutpos,
                        TVector3 aMSDpin, TVector3 aMSDpout,
                        Double_t aMSDde, Double_t aMSDal, Double_t aMSDtof)
{
    fHitMSD->NewHit(aMSDid,aMSDilay,aMSDistripx,aMSDistripy,aMSDinpos,aMSDoutpos,aMSDpin,aMSDpout,aMSDde,aMSDtof,-99);
}

/*-----------------------------------------------------------------*/

Int_t TAMCevent::AddTW(Int_t aTWid, Int_t aTWibar, Int_t aTWiview,
                       TVector3 aTWinpos, TVector3 aTWoutpos, TVector3 aTWpin,
                       TVector3 aTWpout, Double_t aTWde, Double_t aTWal, Double_t aTWtof )
{
    fHitTW->NewHit(aTWid,aTWibar,aTWiview,-99,aTWinpos,aTWoutpos,aTWpin,aTWpout,aTWde,aTWtof,-99);
}

/*-----------------------------------------------------------------*/
Int_t TAMCevent::AddCAL(Int_t aCALid, Int_t aCALicry,
                        TVector3 aCALinpos, TVector3 aCALoutpos, TVector3 aCALpin,
                        TVector3 aCALpout, Double_t aCALde, Double_t aCALal,
                        Double_t aCALtof)
{
    fHitCAL->NewHit(aCALid,aCALicry,-99,-99,aCALinpos,aCALoutpos,aCALpin,aCALpout,aCALde,aCALtof,-99);
}

///*-----------------------------------------------------------------*/
///// NB : crossing particles should be TAMCntuEve !
//Int_t TAMCevent::AddCROSS(Int_t aCROSSid, Int_t aCROSSnreg, Int_t aCROSSnregold,
//                          TVector3 aCROSSpos,TVector3 aCROSSp, Double_t aCROSSm,
//                          Double_t aCROSSch, Double_t aCROSSt)
//{
//    fHitCROSS->NewHit(aCROSSid,aCROSSnreg,aCROSSnregold,-99,aCROSSpos,0,aCROSSp,0,aCROSSm,aCROSSch,-99);
//}

/*-----------------------------------------------------------------*/
void TAMCevent::SetBranches(TTree *RootTree){

    RootTree->Branch("EventNumber",&fEventNumber,"EventNumber/I");
    RootTree->Branch("Track",&fTrack);
    RootTree->Branch(fHitSTC->GetStcBranchName(),&fHitSTC);
    RootTree->Branch(fHitBMN->GetBmBranchName(),&fHitBMN);
    RootTree->Branch(fHitVTX->GetVtxBranchName(),&fHitVTX);
    RootTree->Branch(fHitITR->GetItrBranchName(),&fHitITR);
    RootTree->Branch(fHitMSD->GetMsdBranchName(),&fHitMSD);
    RootTree->Branch(fHitTW->GetTofBranchName(),&fHitTW);
    RootTree->Branch(fHitCAL->GetCalBranchName(),&fHitCAL);
}

/*-----------------------------------------------------------------*/
void TAMCevent::FindBranches(TTree *RootTree)
{
    RootTree->SetBranchAddress(fHitSTC->GetStcBranchName(),&fHitSTC);
    RootTree->SetBranchAddress(fHitBMN->GetBmBranchName(),&fHitBMN);
    RootTree->SetBranchAddress(fHitVTX->GetVtxBranchName(),&fHitVTX);
    RootTree->SetBranchAddress(fHitITR->GetItrBranchName(),&fHitITR);
    RootTree->SetBranchAddress(fHitMSD->GetMsdBranchName(),&fHitMSD);
    RootTree->SetBranchAddress(fHitTW->GetTofBranchName(),&fHitTW);
    RootTree->SetBranchAddress(fHitCAL->GetCalBranchName(),&fHitCAL);
}


/*-----------------------------------------------------------------*/
void TAMCevent::Dump() const
{
    return;
    
}

/*-----------------------------------------------------------------*/
TAMCevent::~TAMCevent()
{
    delete fTrack;
    delete fHitSTC;
    delete fHitBMN;
    delete fHitVTX;
    delete fHitITR;
    delete fHitMSD;
    delete fHitTW;
    delete fHitCAL;
}


