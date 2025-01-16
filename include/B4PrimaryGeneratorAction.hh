#ifndef B4PrimaryGeneratorAction_h
#define B4PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"


class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class B4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    B4PrimaryGeneratorAction();
    virtual ~B4PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event);

    // set methods
    void SetRandomFlag(G4bool value);

private:

    #ifdef USE_GPS
    G4GeneralParticleSource* fGPS;
    #else
    G4ParticleGun* fParticleGun;
    #endif
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
