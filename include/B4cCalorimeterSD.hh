#ifndef B4cCalorimeterSD_h
#define B4cCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"

#include "B4cCalorHit.hh"

#include <vector>


class G4Step;
class G4HCofThisEvent;

/// Calorimeter sensitive detector class
///
/// In Initialize(), it creates one hit for each calorimeter layer and one more
/// hit for accounting the total quantities in all layers.
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.

class B4cCalorimeterSD : public G4VSensitiveDetector
{

  public:
    B4cCalorimeterSD(const G4String& name, 
                     const G4String& hitsCollectionName, 
                     G4int nofCells
                     );
    virtual ~B4cCalorimeterSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    void Kill(G4Step *step);
    bool Mcross(const G4Step* aStep, const G4String& vol1_name, const G4String& vol2_name);
    bool Tcross(const G4Track* aStep, const G4String& vol1_name, const G4String& vol2_name);

    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    B4cCalorHitsCollection* fHitsCollection;
    G4int  fNofCells;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

