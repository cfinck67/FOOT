/*!
  \file
  \version $Id: adapted from TABMactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TADCactNtuMC.
*/

#include "TAGroot.hxx"
#include "TADCntuRaw.hxx"
#include "TADCparCon.hxx"
#include "TADCactNtuMC.hxx"

//First
#include "TAGgeoTrafo.hxx"

#include "GlobalPar.hxx"

/*!
  \class TADCactNtuMC TADCactNtuMC.hxx "TADCactNtuMC.hxx"
  \brief NTuplizer for DC raw hits. **
*/

ClassImp(TADCactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCactNtuMC::TADCactNtuMC(const char* name,
			   TAGdataDsc* p_nturaw, 
			   TAGparaDsc* p_parcon, 
         TAGparaDsc* p_pargeo, 
			   EVENT_STRUCT* evStr)
  : TAGaction(name, "TADCactNtuMC - NTuplize ToF raw data"),
    fpNtuMC(p_nturaw),
    fpParCon(p_parcon),
    fpParGeo(p_pargeo),
    fpEvtStr(evStr)
{
  Info("Action()"," Creating the Drif Chamber MC tuplizer action\n");
  AddPara(p_parcon, "TADCparCon");
  AddPara(p_pargeo, "TADCparGeo");
  AddDataOut(p_nturaw, "TADCntuRaw");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCactNtuMC::~TADCactNtuMC() {}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TADCactNtuMC::Action() {

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  TADCntuRaw* p_nturaw = (TADCntuRaw*) fpNtuMC->Object();
  TADCparCon* p_parcon = (TADCparCon*) fpParCon->Object();
  TADCparGeo* p_pargeo = (TADCparGeo*) fpParGeo->Object();

  int cell, view, lay;
  Int_t nhits(0);
  if (!p_nturaw->h) p_nturaw->SetupClones();
  // double locx, locy, locz;
  Double_t resolution;
  //The number of hits inside the DC is DCHn
  Info("Action()","Processing n :: %2d hits \n",fpEvtStr->DCHn);


// FILL THE HITS - hit loop
// fpEvtStr = EVENT_STRUCT
  if ( GlobalPar::GetPar()->Debug() > 1 ) {
    cout << "Filling info...... "  << endl;
    cout << "\tN true part: " << fpEvtStr->TRn << endl;
    cout << "\tN DC part: " << fpEvtStr->DCHn << endl;
  }
    for (Int_t i = 0; i < fpEvtStr->DCHn; i++) {
        //   if(i<32) {      // a caso???   pare ci sia un limite nel fitter

        /*
          Pre processing of INFO to compute the PCA info + rDrift and tDrift
          PCA = piont of closest approach -> I would say useless using KalmanFilter
        */
        /*
          write(*,*)'PCA= ',xca(ii), yca(ii), zca(ii) 
          write(*,*)'p at PCA= ',pxca(ii), pyca(ii), pzca(ii) 
          write(*,*)'rdrift= ',rdrift(ii),' tdrift= ', tdrift(ii), 
        */

        
        int genPartID = fpEvtStr->DCHid[i] - 1;
        if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << endl;
        // double momentum = sqrt( fpEvtStr->TRipx[genPartID]*fpEvtStr->TRipx[genPartID] +
        //                   fpEvtStr->TRipy[genPartID]*fpEvtStr->TRipy[genPartID] +
        //                   fpEvtStr->TRipz[genPartID]*fpEvtStr->TRipz[genPartID] );
        // cout << "\t\t\tmomentum: " << momentum << endl;
        if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;

        // check true particle ID linked to the hit is in the correct range
        if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
          cout << "TADCactNtuMC::Action :: ERROR >> wrong generate particle ID: "<< genPartID << " (nPart= " << fpEvtStr->TRn << endl;
          exit(0);
        }

        // take only hits linked to specific particle
        // if ( fpEvtStr->TRfid[genPartID] != -6 || fpEvtStr->TRcha[genPartID] != 2 )    continue;
        // cout << "ALPHA found! "  << endl;
        // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 3 )    continue;
        // cout << "\tLITIUM found! "  << endl;
        if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 6 )    continue;
        // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 6 || 
        //           fpEvtStr->TRmass[genPartID] < 10 ||  fpEvtStr->TRmass[genPartID] > 10.5 )    continue;
        if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "CARBONIUM found! "  << endl;

        // sense wire of the hit
        cell = fpEvtStr->DCHicell[i];   // 0-6
        lay = fpEvtStr->DCHilay[i];     // 0-6
        view = fpEvtStr->DCHiview[i];   // 1 o -1

        // cout << "TADCactNtuMC::Action() cell, lay, view >> " << cell << "  " << lay << "  " << view << endl;
        // cout << "TADCactNtuMC::Action() id >> " << fpEvtStr->DCHid[i] << endl;

        // X Y Z needs to be placed in Local coordinates.
        // :( !! It should be done by the DCgeo class
        TVector3 loc(fpEvtStr->DCHxin[i],fpEvtStr->DCHyin[i],fpEvtStr->DCHzin[i]);
        // TVector3 loc = fpFirstGeo->FromGlobalToDCLocal(gloc);       // non funziona
        if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "\t\tTrue Global Position: ", loc.Print();
        p_pargeo->Global2Local( &loc );
        // TVector3 loc = TVector3( gloc.x()-p_pargeo->GetCenter().x(),
        //                          gloc.y()-p_pargeo->GetCenter().y(),
        //                          gloc.z()-p_pargeo->GetCenter().z()    );
        
        // Px Py Pz
        TVector3 mom(fpEvtStr->DCHpxin[i],fpEvtStr->DCHpyin[i],fpEvtStr->DCHpzin[i]);
        if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "\t\tTrue Global Momentum: ", mom.Print();
        p_pargeo->Global2Local_RotationOnly( &mom );
        
        // TVector3 gmom(fpEvtStr->DCHpxin[i],fpEvtStr->DCHpyin[i],fpEvtStr->DCHpzin[i]);
        // TVector3 mom = fpFirstGeo->VecFromGlobalToDCLocal(gmom);       // do not make translation!!!
        // VecFromGlobalToDCLocal -> da ROOT fa solo rotazione

        // if(fpEvtStr->DCHid[i]-1 == 0) {    // prense solo prima particella, perche?
        // initialise the hit inside ntuRaw
        TADCntuHit *mytmp = new( (*(p_nturaw->h))[nhits]) 
                        TADCntuHit(fpEvtStr->DCHid[i],     
                        view,    lay,     cell,     // sense wire address
                        loc.X(), loc.Y(), loc.Z(),           // real local coordinates
                        // 0, 0, 0, //Will become PCA that is needed @ tracking level. Useless with KFitter, Frank'd sey
                        mom.X(), mom.Y(), mom.Z(),          // momentum initial
                        // fpEvtStr->DCHpxin[i], fpEvtStr->DCHpyin[i], fpEvtStr->DCHpzin[i],  //mom @ entrance in cell ----- Wrong, use global coordinates
                        0, // Rdrift is set later on (see FindRdrift) 
                        0, // tdrift has no meaning for now for MC
                        fpEvtStr->DCHtim[i] );
        
        // store generated particle info
        mytmp->SetGeneratedParticleInfo ( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                        fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                        TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                        TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID]) );

        

        // set sense wire position and lenght as vector in the hit object
        mytmp->SetAW(p_pargeo);

        //Finds and sets rdrift for a given hit
        mytmp->SetRdrift(loc,mom);
        resolution = p_parcon->ResoEval(mytmp->Dist());
        mytmp->SetSigma(resolution);
        
        mytmp->SetTrkAss(0);        //?????
        nhits++;
        
    }

    p_nturaw->nhit  = nhits;

    fpNtuMC->SetBit(kValid);
    return kTRUE;

}






// // reading event example

// TADCactNtuMC::Action() cell, lay, view >> 2  -1  1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 1  -1  -1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 2  0  1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 0  0  -1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 2  1  1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 1  1  -1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 2  2  1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 0  2  -1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 2  3  1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 1  3  -1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 2  4  1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 0  4  -1
// TADCactNtuMC::Action() id >> 2
// TADCactNtuMC::Action() cell, lay, view >> 1  -1  1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 1  -1  -1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 2  0  1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 1  0  -1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 1  1  1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 1  1  -1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 2  2  1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 1  2  -1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 1  3  1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 1  3  -1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 2  4  1
// TADCactNtuMC::Action() id >> 14
// TADCactNtuMC::Action() cell, lay, view >> 0  4  -1
// TADCactNtuMC::Action() id >> 14
// Processed:: 0 evts!

















