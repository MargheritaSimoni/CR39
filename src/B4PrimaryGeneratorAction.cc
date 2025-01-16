#include "B4PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Threading.hh"

#ifdef USE_GPS //preprocessor set in cmake file!!!
#include "G4GeneralParticleSource.hh"
#else
#include "G4ParticleGun.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double pos_z = -19. * cm;

B4PrimaryGeneratorAction::B4PrimaryGeneratorAction()
        : G4VUserPrimaryGeneratorAction(),
        #ifdef USE_GPS
        fGPS(nullptr)
        #else
        fParticleGun(nullptr)
        #endif
{

    #ifdef USE_GPS
    fGPS = new G4GeneralParticleSource();
    #else
    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);
    #endif
    // default particle kinematic
    //
    auto particleDefinition
            = G4ParticleTable::GetParticleTable()->FindParticle("neutron");

#ifdef USE_GPS
    //generalParticleSource implementation
    fGPS->SetParticleDefinition(particleDefinition);
    fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0.025*eV);
    fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., pos_z));

#else
    //ParticleGun implementation
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -16.*mm));
    fParticleGun->SetParticleEnergy(0.025*eV);
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4PrimaryGeneratorAction::~B4PrimaryGeneratorAction()
{
    #ifdef USE_GPS
    delete fGPS;
    #else
    delete fParticleGun;
    #endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

    G4double halfX = 1.2 * cm;
    G4double halfY = 1.2 * cm;
    G4double px = G4UniformRand() * (2.* halfX) - halfX;
    G4double py = G4UniformRand() * (2.* halfY) - halfY;

    // Set gun position
    #ifdef USE_GPS
    fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(px * mm, py * mm, pos_z));
    #else
    fParticleGun->SetParticlePosition(G4ThreeVector(px * mm, py * mm, pos_z));
    #endif

    //set primary vertex
    #ifdef USE_GPS
    fGPS->GeneratePrimaryVertex(anEvent);
    #else
    fParticleGun->GeneratePrimaryVertex(anEvent);
    #endif

    //G4double energy = G4RandGauss(2.5e-3, 1e-4);
    //fParticleGun->SetParticleEnergy(energy * eV);
}
