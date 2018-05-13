



#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTparMap.hxx"



#include "TAVTntuHit.hxx"





ClassImp(TAVTntuHit) // Description of Single Detector TAVTntuHit 



//______________________________________________________________________________
//  build a hit from a rawHit
TAVTntuHit::TAVTntuHit(Int_t aSensorNumber, TAVTrawHit* pixel)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1)
{
   // constructor of a TAVTntuHit from a base pixel
   
    fPixelLine    = pixel->GetLineNumber();
    fPixelColumn  = pixel->GetColumnNumber();
    // fPixelIndex   = pixel->GetIndex();

    fRawValue     = pixel->GetValue();
    fPulseHeight  = fRawValue;   

    m_origins = "data";
    Initialise();
    m_layer = m_geometry->GetLayerFromSensorID( aSensorNumber );
}



// lo cancellerei....................................................................................................
//______________________________________________________________________________
//  build the hit from the index
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue, string aorigin )
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  // fPixelIndex(aPixelIndex),
  fPixelLine(0),
  fPixelColumn(0),
  fRawValue(aValue)
{
    m_origins = aorigin;
    Initialise();

    m_layer = m_geometry->GetLayerFromSensorID( aSensorNumber );

    fPulseHeight    = fRawValue; 
}




//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn, string aorigin )
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  // fPixelIndex(0),
  fPixelLine(aLine),
  fPixelColumn(aColumn),
  fRawValue(aValue)
{
    
    m_origins = aorigin;
    Initialise();

    m_layer = m_geometry->GetLayerFromSensorID( aSensorNumber );

    fPulseHeight    = fRawValue; 
}



//______________________________________________________________________________
//  
void TAVTntuHit::Initialise() {

    fPosition.SetXYZ(0, 0, 0);

    fMCPos.SetXYZ(0, 0, 0);
    fMCP.SetXYZ(0, 0, 0);

    m_genPartIndex = -1;
    m_genPartPointer = NULL;

    m_originalMC_HitID = -1;
    m_originalMC_Hit = NULL;   

    // check the hit origin is allowed
    if ( !GlobalPar::GetPar()->CheckAllowedHitOrigin(m_origins) )  {
        cout << "ERROR >> TAVTntuHit::Initialise()  -->  the required hit origin (" << m_origins<< ") is not allowed. \nThe allowed ones are: ";
        GlobalPar::GetPar()->PrintAllowedHitOrigin();
        exit(0);
    }

    // take the detector geometry
    m_geometry = (TAVTparGeo*) gTAGroot->FindParaDsc("vtGeo", "TAVTparGeo")->Object();

    // set center position
    if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "TAVTntuHit::Initialise()  ::  line = " << fPixelLine << " col = " << fPixelColumn << endl;
    SetPosition( m_geometry->GetPixelPos_detectorFrame( fSensorNumber, fPixelColumn, fPixelLine ) );
    // SetPosition( m_geometry->GetPixelPos_Local( fSensorNumber, fPixelColumn, fPixelLine ) );


}





//______________________________________________________________________________
void TAVTntuHit::SetGenPartID( int agenPartID ) { 
    m_genPartIndex = agenPartID; 
    
    // find the pointer in the list
    if( !GlobalPar::GetPar()->IncludeEvent() )  return;

    TAGntuMCeve* ntup = (TAGntuMCeve*) gTAGroot->FindDataDsc("myn_mceve", "TAGntuMCeve")->Object();
    for (int i = 0; i < ntup->GetHitN(); i++) {   // over all sensors
        if ( ntup->Hit( i )->FlukaID() == m_genPartIndex ) {
            m_genPartPointer = ntup->Hit( i );
            // ntup->Hit( i )->AddVTXhit( this );  // x Alberto to implement <3
            return;
        }
    }
}




//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCPosition_sensorFrame() {
    TVector3 glob = fMCPos;
    m_geometry->Detector2Sensor_frame( fSensorNumber, &glob ); 
    return glob; 
}


//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCPosition_footFrame() { 
    TVector3 glob = fMCPos;
    m_geometry->Local2Global( &glob ); 
    return glob; 
};



//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCMomentum_footFrame() { 
    TVector3 globP = fMCP;
    m_geometry->Local2Global_RotationOnly( &globP ); 
    return globP; 
};



//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(TAVTntuHit &aPixel)
{
   return Distance(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(TAVTntuHit &aPixel)
{
   return DistanceU(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.x();
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(TAVTntuHit &aPixel)
{
   return DistanceV(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.y();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

























