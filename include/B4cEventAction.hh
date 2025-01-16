#ifndef B4cEventAction_h
#define B4cEventAction_h 1

#include "G4UserEventAction.hh"

#include "B4cCalorHit.hh"

#include "globals.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles in Absober and Gap layers 
/// stored in the hits collections.

class B4cEventAction : public G4UserEventAction
{
public:
  B4cEventAction();
  virtual ~B4cEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
    
private:
  // methods
  B4cCalorHitsCollection* GetHitsCollection(G4int hcID,
                                            const G4Event* event) const;
  void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,
                            G4double gapEdep, G4double xPos, G4double yPos, G4ThreeVector gp) const;
  
  // hits collections IDs
  G4int  fFoilHCID;
  G4int  fdetectorHCID;
  G4int  fSampleHCID;

};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
