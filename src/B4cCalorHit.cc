#include "B4cCalorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

// nb: scambiato tutti gli fPhotons con fNeutrons

G4ThreadLocal G4Allocator<B4cCalorHit>* B4cCalorHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cCalorHit::B4cCalorHit()
 : G4VHit(),
   fEdep(0.),
   fTrackLength(0.),
   fNeutrons(0),
   eNeutron(-999),
   xPos(-999),
   yPos(-999),
   boundaryVector(-1000,-1000,-1000),
   boundaryEnergy(-999),
   gammaEnergies(),
   secondaryAtomicNumber(200,-1),
   secondaryAtomicMass(200,-1),
   secondaryTrackLengths(200,-1),
   secondaryDepositedEnergy(200,0)

{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cCalorHit::~B4cCalorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cCalorHit::B4cCalorHit(const B4cCalorHit& right)
  : G4VHit()
{
    fEdep = right.fEdep;
    fTrackLength = right.fTrackLength;
    fNeutrons = right.fNeutrons;
    eNeutron = right.eNeutron;
    xPos = right.xPos;
    yPos = right.yPos;
    boundaryVector = right.boundaryVector;
    boundaryEnergy = right.boundaryEnergy;
    gammaEnergies = right.gammaEnergies;
    secondaryAtomicNumber = right.secondaryAtomicNumber;
    secondaryAtomicMass = right.secondaryAtomicMass;
    secondaryTrackLengths = right.secondaryTrackLengths;
    secondaryDepositedEnergy = right.secondaryDepositedEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const B4cCalorHit& B4cCalorHit::operator=(const B4cCalorHit& right)
{
    fEdep = right.fEdep;
    fTrackLength = right.fTrackLength;
    fNeutrons = right.fNeutrons;
    eNeutron = right.eNeutron;
    xPos = right.xPos;
    yPos = right.yPos;
    boundaryVector = right.boundaryVector;
    boundaryEnergy = right.boundaryEnergy;
    gammaEnergies = right.gammaEnergies;
    secondaryAtomicNumber = right.secondaryAtomicNumber;
    secondaryAtomicMass = right.secondaryAtomicMass;
    secondaryTrackLengths = right.secondaryTrackLengths;
    secondaryDepositedEnergy = right.secondaryDepositedEnergy;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B4cCalorHit::operator==(const B4cCalorHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cCalorHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " track length: " 
     << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cCalorHit::AddNeutronInDetector(G4double neutronE) {
        fNeutrons += 1;
        eNeutron = neutronE;
   };

void B4cCalorHit::AddPositionInDetector(G4double posx, G4double posy) {
    xPos = posx;
    yPos = posy;
};

void B4cCalorHit::AddBoundaryTracking(G4ThreeVector posOnBoundary, G4double EOnBoundary){
    boundaryVector = posOnBoundary;
    boundaryEnergy = EOnBoundary;
};

void B4cCalorHit::AddGammasInFoil(std::vector<G4double> EGammas){
    gammaEnergies = EGammas;
};


void B4cCalorHit::AddSecondaryTrackInfo(G4int trackid, G4int AtomicNumber, G4int AtomicMass, G4double DepositedEnergy, G4double trackLengths){
    secondaryAtomicNumber.at(trackid-2) = AtomicNumber;
    secondaryAtomicMass.at(trackid-2)  = AtomicMass;
    secondaryDepositedEnergy.at(trackid-2)  += DepositedEnergy;
    secondaryTrackLengths.at(trackid-2)  = trackLengths;
};