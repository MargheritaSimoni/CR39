#include "B4cCalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cCalorimeterSD::B4cCalorimeterSD(
                                   const G4String& name,
                                   const G4String& hitsCollectionName,
                                   G4int nofCells
                                   )
: G4VSensitiveDetector(name),
fHitsCollection(nullptr),
fNofCells(nofCells)
{
    collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cCalorimeterSD::~B4cCalorimeterSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
    // Create hits collection
    fHitsCollection
    = new B4cCalorHitsCollection(SensitiveDetectorName, collectionName[0]);
    
    // Add this collection in hce
    auto hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection );
    
    // Create hits
    // fNofCells for cells + one more for total sums
    for (G4int i=0; i<fNofCells+1; i++ ) {
        fHitsCollection->insert(new B4cCalorHit());
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B4cCalorimeterSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
    // energy deposit
     auto edep = step->GetTotalEnergyDeposit();
    
    // step length
    G4double stepLength = 0.;
    stepLength = step->GetStepLength();

    if ( stepLength == 0. ) return false;
    auto touchable = (step->GetPreStepPoint()->GetTouchable());
    
    // Get calorimeter cell id
    auto layerNumber = touchable->GetReplicaNumber(1);
    
    // Get hit accounting data for this cell
    auto hit = (*fHitsCollection)[layerNumber];
    if ( ! hit ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hit " << layerNumber;
        G4Exception("B4cCalorimeterSD::ProcessHits()",
                    "MyCode0004", FatalException, msg);
        }
    
    // Get hit for total accounting
    auto hitTotal
    = (*fHitsCollection)[fHitsCollection->entries()-1];
    
    // Add values
    hit->Add(edep, stepLength);
    hitTotal->Add(edep, stepLength);
    
    // Qui prende la definizione della particle ID (pid)
    auto pid = step->GetTrack()->GetDefinition()->GetParticleName();
    auto volumeName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

    // Questa funzione controlla se la traccia passa dal volume xtal al volume pmt
    if (volumeName == "Detector"){
    G4bool isNeutronInDetector = Mcross(step, "Detector", "World");
        if (pid == "neutron" && isNeutronInDetector) {

            G4double neutronE = step->GetPreStepPoint()->GetKineticEnergy();
            G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
            hit->AddNeutronInDetector(neutronE);
            hit->AddPositionInDetector(pos.x(), pos.y());
            hitTotal->AddNeutronInDetector(neutronE);
            hitTotal->AddPositionInDetector(pos.x(), pos.y());
            Kill(step);
        }
    }

    if (volumeName == "VolumePolymer") {
        G4bool isNeutronOutOfBox = Mcross(step, "VolumePolymer", "World");
        if (pid == "neutron" && isNeutronOutOfBox) {

            G4double neutronE = step->GetPostStepPoint()->GetKineticEnergy();
            G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();
            hit->AddBoundaryTracking(pos, neutronE);
            hitTotal->AddBoundaryTracking(pos, neutronE);
        }

        /////////////////////////////////////////////
        // code for double checks
        /*
               const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();

               if (!secondaries->empty()) {
                   for (size_t i = 0; i < secondaries->size(); ++i) {
                       const G4Track *secondaryTrack = (*secondaries)[i];
                       G4double secEnergy = secondaryTrack->GetKineticEnergy();
                       G4cout << step->GetTrack()->GetTrackID() << " SECONDARY"
                              << secondaryTrack->GetDefinition()->GetParticleName() << " secEnergy "
                              << G4BestUnit(secEnergy, "Energy") << G4endl;

                                             if (secondaryTrack->GetDefinition()->GetParticleName() == "gamma") {
                                                 G4int atomicNumber = step->GetTrack()->GetDefinition()->GetAtomicNumber();
                                                 G4cout << "SECONDARY gamma Atomic number: " << atomicNumber << G4endl;
                                             }

                                             G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
                                             G4cout << " SECONDARY Event ID: " << eventID << G4endl;
                                             for (size_t i = 0; i < secondaries->size(); ++i) {
                                                 const G4Track *secondaryTrack = (*secondaries)[i];
                                                 G4cout << " SECONDARY name: " << secondaryTrack->GetDefinition()->GetParticleName() << G4endl;

                   }
               }
  */
        /////////////////////////////////////////////

        //get secondaries for track length
        G4int trackID = step->GetTrack()->GetTrackID();
        if (trackID != 1 ) { //&& atomicNumber != 0
                G4int atomicNumber = step->GetTrack()->GetDefinition()->GetAtomicNumber();
                G4int atomicMass = step->GetTrack()->GetDefinition()->GetAtomicMass();
                G4double secondaryDepositedEnergy = step->GetTotalEnergyDeposit();
                G4double secondaryTrackLength = step->GetTrack()->GetTrackLength();
           // G4int stepNumber = step->GetTrack()->GetCurrentStepNumber();
           // G4cout <<stepNumber << "ID" <<trackID<< " SECONDARY in SD Atomic number: " << atomicNumber << " GetSecondaryTrackLengths "<< G4BestUnit(secondaryTrackLength, "Length") << " GetSecondaryDepositedEnergy " << G4BestUnit(secondaryDepositedEnergy, "Energy") << G4endl;
            G4cout << step->GetTrack()->GetTrackID() << " SECONDARY"
                   << step->GetTrack()->GetDefinition()->GetParticleName() << " secEnergy "
                   << G4BestUnit(secondaryDepositedEnergy, "Energy") << G4endl;
                hit->AddSecondaryTrackInfo(trackID, atomicNumber, atomicMass, secondaryDepositedEnergy, secondaryTrackLength);
                hitTotal->AddSecondaryTrackInfo(trackID, atomicNumber, atomicMass, secondaryDepositedEnergy, secondaryTrackLength);

        }
    }

    if (volumeName == "foilCd") {
        // Get the secondary particles produced in the current step
        const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();

        G4int trackID = step->GetTrack()->GetTrackID();
        // Loop over the secondary particles
        if (!secondaries->empty() && trackID==1 ){
                std::vector<G4double> gammaEnergies;
            for (size_t i = 0; i < secondaries->size(); ++i) {
                const G4Track* secondaryTrack = (*secondaries)[i];

                // Get the particle definition of the secondary particle
                G4ParticleDefinition* particle = secondaryTrack->GetDefinition();

                // Now you can access information about the secondary particle, for example its name
                G4String particleName = particle->GetParticleName();
                if (particleName == "gamma"){
                    G4double gammaEnergy = secondaryTrack->GetKineticEnergy();
                    gammaEnergies.push_back(gammaEnergy);
                }
            }
            hit->AddGammasInFoil(gammaEnergies);
            hitTotal->AddGammasInFoil(gammaEnergies);
        }

    }

    return true;
}


void B4cCalorimeterSD::Kill(G4Step *step){
    step->GetTrack()->SetTrackStatus(fStopAndKill);
}

//returns true if the particle crosses the boundary between vol1 and vol2
//Note: use the Physical Volumes names!
bool B4cCalorimeterSD::Mcross(const G4Step* aStep, const G4String& vol1_name, const G4String& vol2_name){
    G4bool did_cross=false;
    if (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()==vol1_name && aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()==vol2_name){
        did_cross = true;
    }
    return did_cross;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cCalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
    if ( verboseLevel>1 ) {
        G4int nofHits = fHitsCollection->entries();
        G4cout
        << G4endl
        << "-------->Hits Collection: in this event they are " << nofHits
        << " hits in the tracker chambers: " << G4endl;
        for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
    }
}
