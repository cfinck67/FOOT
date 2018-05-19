



#include "TString.h"
#include "TClonesArray.h"

#include "TAITntuRaw.hxx"
#include "TAITparMap.hxx"



#include "TAITntuHit.hxx"





ClassImp(TAITntuHit) // Description of Single Detector TAITntuHit 



//______________________________________________________________________________
//  build a hit from a rawHit
TAITntuHit::TAITntuHit(Int_t aSensorNumber, TAITrawHit* pixel)
: TObject(),
  m_sensorID(aSensorNumber),
  fMCid(-1)
{
   // constructor of a TAITntuHit from a base pixel
   
    fPixelLine    = pixel->GetLineNumber();
    fPixelColumn  = pixel->GetColumnNumber();
    // fPixelIndex   = pixel->GetIndex();

    fRawValue     = pixel->GetValue();
    fPulseHeight  = fRawValue;   

    m_origins = "data";
    Initialise();
    m_layer = m_geometry->GetLayerFromSensorID( aSensorNumber );
    m_plume = m_geometry->GetPlumeFromSensorID( aSensorNumber );
    m_chip = m_geometry->GetChipFromSensorID( aSensorNumber );
}



// lo cancellerei....................................................................................................
//______________________________________________________________________________
//  build the hit from the index
TAITntuHit::TAITntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue, string aorigin )
: TObject(),
  m_sensorID(aSensorNumber),
  fMCid(-1),
  // fPixelIndex(aPixelIndex),
  fPixelLine(0),
  fPixelColumn(0),
  fRawValue(aValue)
{
    m_origins = aorigin;
    Initialise();

    m_layer = m_geometry->GetLayerFromSensorID( aSensorNumber );
    m_plume = m_geometry->GetPlumeFromSensorID( aSensorNumber );
    m_chip = m_geometry->GetChipFromSensorID( aSensorNumber );

    fPulseHeight    = fRawValue; 
}




//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAITntuHit::TAITntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn, string aorigin )
: TObject(),
  m_sensorID(aSensorNumber),
  fMCid(-1),
  // fPixelIndex(0),
  fPixelLine(aLine),
  fPixelColumn(aColumn),
  fRawValue(aValue)
{
    
    m_origins = aorigin;
    Initialise();

    m_layer = m_geometry->GetLayerFromSensorID( aSensorNumber );
    m_plume = m_geometry->GetPlumeFromSensorID( aSensorNumber );
    m_chip = m_geometry->GetChipFromSensorID( aSensorNumber );

    fPulseHeight    = fRawValue; 
}



//______________________________________________________________________________
//  
void TAITntuHit::Initialise() {

    fPosition.SetXYZ(0, 0, 0);

    fMCPos.SetXYZ(0, 0, 0);
    fMCP.SetXYZ(0, 0, 0);

    m_genPartIndex = -1;
    m_genPartPointer = NULL;

    m_originalMC_HitID = -1;
    m_originalMC_Hit = NULL;   

    // check the hit origin is allowed
    if ( !GlobalPar::GetPar()->CheckAllowedHitOrigin(m_origins) )  {
        cout << "ERROR >> TAITntuHit::Initialise()  -->  the required hit origin (" << m_origins<< ") is not allowed. \nThe allowed ones are: ";
        GlobalPar::GetPar()->PrintAllowedHitOrigin();
        exit(0);
    }

    // take the detector geometry
    m_geometry = (TAITparGeo*) gTAGroot->FindParaDsc("itGeo", "TAITparGeo")->Object();

    // set center position
    if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "TAITntuHit::Initialise()  ::  line = " << fPixelLine << " col = " << fPixelColumn << endl;
    SetPosition( m_geometry->GetPixelPos_detectorFrame( m_sensorID, fPixelColumn, fPixelLine ) );
    // SetPosition( m_geometry->GetPixelPos_Local( m_sensorID, fPixelColumn, fPixelLine ) );


}





//______________________________________________________________________________
void TAITntuHit::SetGenPartID( int agenPartID ) { 
    m_genPartIndex = agenPartID; 
    
    // find the pointer in the list
    if( !GlobalPar::GetPar()->IncludeEvent() )  return;

    TAGntuMCeve* ntup = (TAGntuMCeve*) gTAGroot->FindDataDsc("myn_mceve", "TAGntuMCeve")->Object();
    for (int i = 0; i < ntup->GetHitN(); i++) {   // over all sensors
        if ( ntup->Hit( i )->FlukaID() == m_genPartIndex ) {
            m_genPartPointer = ntup->Hit( i );
            // ntup->Hit( i )->AddITXhit( this );  // x Alberto to implement <3
            return;
        }
    }
}




//______________________________________________________________________________
TVector3 TAITntuHit::GetMCPosition_sensorFrame() {
    TVector3 glob = fMCPos;
    m_geometry->Detector2Sensor_frame( m_sensorID, &glob ); 
    return glob; 
}


//______________________________________________________________________________
TVector3 TAITntuHit::GetMCPosition_footFrame() { 
    TVector3 glob = fMCPos;
    m_geometry->Local2Global( &glob ); 
    return glob; 
};



//______________________________________________________________________________
TVector3 TAITntuHit::GetMCMomentum_footFrame() { 
    TVector3 globP = fMCP;
    m_geometry->Local2Global_RotationOnly( &globP ); 
    return globP; 
};



//______________________________________________________________________________
//  
Double_t TAITntuHit::Distance(TAITntuHit &aPixel)
{
   return Distance(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceU(TAITntuHit &aPixel)
{
   return DistanceU(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.x();
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceV(TAITntuHit &aPixel)
{
   return DistanceV(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAITntuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.y();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

























