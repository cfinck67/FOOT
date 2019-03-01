
#ifndef TCGphysicsQMD_h
#define TCGphysicsQMD_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"


class TCGphysicsQMD: public G4VModularPhysicsList
{
    public:
    TCGphysicsQMD();
    virtual ~TCGphysicsQMD();

    virtual void SetCuts();
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    private:
    G4VPhysicsConstructor* hadronElastic;
    G4VPhysicsConstructor* hadronInelastic;
    G4VPhysicsConstructor* ionElastic;
    G4VPhysicsConstructor* ionInelastic;
    G4VPhysicsConstructor* electromagnetic;
    G4VPhysicsConstructor* decay;
    G4VPhysicsConstructor* radioactiveDecay;
};

#endif


