
#ifndef TCGprimaryGeneratorAction_hxx
#define TCGprimaryGeneratorAction_hxx

#include "G4VUserPrimaryGeneratorAction.hh"
#include "TAGparGeo.hxx"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class TCGprimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
   TCGprimaryGeneratorAction(TAGparGeo* pParGeo);
   ~TCGprimaryGeneratorAction();
   
public:
   void GeneratePrimaries(G4Event* anEvent);
   
private:
   G4ParticleGun* fParticleGun;      
   TAGparGeo*     fpParGeo;
   Bool_t         fHasPartDefined;
};


#endif


