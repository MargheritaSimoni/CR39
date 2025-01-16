
#ifndef B4cCalorHit_h
#define B4cCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength


// ho cambiato Photons in Neutrons
class B4cCalorHit : public G4VHit
{
  public:
    B4cCalorHit();
    B4cCalorHit(const B4cCalorHit&);
    virtual ~B4cCalorHit();

    // operators
    const B4cCalorHit& operator=(const B4cCalorHit&);
    G4bool operator==(const B4cCalorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double dl);
    void AddNeutronInDetector(G4double neutronE);
    void AddPositionInDetector(G4double posx, G4double posy);
    void AddBoundaryTracking(G4ThreeVector posOnBoundary, G4double EOnBoundary);
    void AddGammasInFoil(std::vector<G4double> gammaEnergies);
    void AddSecondaryTrackInfo(G4int trackid, G4int secondaryAtomicNumber, G4int secondaryAtomicMass, G4double secondaryDepositedEnergy, G4double secondaryTrackLengths);

    // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;
    G4int GetNNeutronsInDetector() const;
    G4double GetENeutronsInDetector() const;
    G4double GetXposInDetector() const;
    G4double GetYposInDetector() const;
    G4ThreeVector GetBoundaryPosition() const;
    G4double GetBoundaryEnergy() const;
    std::vector<G4double> GetGammaFoilEnergies() const;
    std::vector<G4int> GetSecondaryAtomicNumber() const;
    std::vector<G4int> GetSecondaryAtomicMass() const;
    std::vector<G4double> GetSecondaryTrackLengths() const;
    std::vector<G4double> GetSecondaryDepositedEnergy() const;


private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength; ///< Track length in the  sensitive volume
    G4double fNeutrons;
    G4double eNeutron;
    G4double xPos;
    G4double yPos;
    G4ThreeVector boundaryVector;
    G4double boundaryEnergy;
    std::vector<G4double> gammaEnergies;
    std::vector<G4int> secondaryAtomicNumber;
    std::vector<G4int> secondaryAtomicMass;
    std::vector<G4double> secondaryTrackLengths;
    std::vector<G4double> secondaryDepositedEnergy;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using B4cCalorHitsCollection = G4THitsCollection<B4cCalorHit>;

extern G4ThreadLocal G4Allocator<B4cCalorHit>* B4cCalorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* B4cCalorHit::operator new(size_t)
{
  if (!B4cCalorHitAllocator) {
    B4cCalorHitAllocator = new G4Allocator<B4cCalorHit>;
  }
  void *hit;
  hit = (void *) B4cCalorHitAllocator->MallocSingle();
  return hit;
}

inline void B4cCalorHit::operator delete(void *hit)
{
  if (!B4cCalorHitAllocator) {
    B4cCalorHitAllocator = new G4Allocator<B4cCalorHit>;
  }
  B4cCalorHitAllocator->FreeSingle((B4cCalorHit*) hit);
}

inline void B4cCalorHit::Add(G4double de, G4double dl) {
  fEdep += de; 
  fTrackLength += dl;
}

inline G4double B4cCalorHit::GetEdep() const { 
  return fEdep; 
}

inline G4double B4cCalorHit::GetTrackLength() const { 
  return fTrackLength; 
}

inline G4int B4cCalorHit::GetNNeutronsInDetector() const {
  return int(fNeutrons);
}

inline G4double B4cCalorHit::GetENeutronsInDetector() const {
  return eNeutron;
}

inline G4double B4cCalorHit::GetXposInDetector() const {
  return xPos;
}

inline G4double B4cCalorHit::GetYposInDetector() const {
  return yPos;
}

inline G4ThreeVector B4cCalorHit::GetBoundaryPosition() const {
  return boundaryVector;
}

inline G4double B4cCalorHit::GetBoundaryEnergy() const {
    return boundaryEnergy;
}

inline std::vector<G4double> B4cCalorHit::GetGammaFoilEnergies() const {
    return gammaEnergies;
}

inline std::vector<G4int> B4cCalorHit::GetSecondaryAtomicNumber() const {
    return secondaryAtomicNumber;
}

inline std::vector<G4int> B4cCalorHit::GetSecondaryAtomicMass() const {
    return secondaryAtomicMass;
}

inline std::vector<G4double> B4cCalorHit::GetSecondaryTrackLengths() const {
    return secondaryTrackLengths;
}

inline std::vector<G4double> B4cCalorHit::GetSecondaryDepositedEnergy() const {
    return secondaryDepositedEnergy;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
