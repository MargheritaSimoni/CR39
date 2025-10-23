#include "B4cDetectorConstruction.hh"
#include "B4cCalorimeterSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4UserLimits.hh" // to set step limits manually
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4NCrystal/G4NCrystal.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
G4GlobalMagFieldMessenger* B4cDetectorConstruction::fMagFieldMessenger = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cDetectorConstruction::B4cDetectorConstruction()
: G4VUserDetectorConstruction(),
fCheckOverlaps(true),
fNofLayers(-1),
fStepLimit(NULL)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cDetectorConstruction::~B4cDetectorConstruction()
{
    delete fStepLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B4cDetectorConstruction::Construct()
{
    // Define materials
    DefineMaterials();

    // Define volumes
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cDetectorConstruction::DefineMaterials()
{
    //VACUUM
    G4double z, a, density;
    new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                   kStateGas, 2.73*kelvin, 3.e-18*pascal);


    //STANDARD GEANT4 MATERIALS
    //auto nistManager = G4NistManager::Instance();
    // CR39  C12H18O7
    // Define elements
    G4Element* elC = new G4Element("Carbon", "C", z=6., a=12.01*g/mole);
    G4Element* elH = new G4Element("Hydrogen", "H", z=1., a=1.01*g/mole);
    G4Element* elO = new G4Element("Oxygen", "O", z=8., a=16.00*g/mole);
    //define material
    G4Material* matCR39 = new G4Material("CR39", density=1.30*g/cm3, 3);
    matCR39->AddElement(elC, 12);
    matCR39->AddElement(elH, 18);
    matCR39->AddElement(elO, 7);
    matCR39->SetName("G4_CR39");

    // Cd
    //nistManager->FindOrBuildMaterial("G4_Cd");

    // MATERIALS DEFINED USING THERMAL LIBRARIES CROSS SECTIONS
//PMMA uses thermal XS of H inside polyethylene
/*
    //G4Element* elOO = new G4Element("Oxygen" ,"O", z= 8., 16.00*g/mole);
    G4Element* elTSHPE = new G4Element("TS_H_of_Polyethylene" , "h_polyethylene", 1.0, 1.0079*g/mole);
    G4Material* matPMMA_TS = new G4Material("G4_PLEXIGLASS_TS", density=1.18*g/cm3, 3, kStateSolid,  293.15*kelvin);
    matPMMA_TS->AddElement(elTSHPE,8);
    matPMMA_TS->AddElement(elO,2);
    matPMMA_TS->AddElement(elC, 5);
*/

    //MATERIALS DEFINED USING NCRYSTAL LIBRARIES nb: NC has a standard temperature of 293..15 instead of 273.15 og G4
    //nb.: NC takes the density from the file
    G4Material * matCR39_NC = G4NCrystal::createMaterial("CR39.ncmat");
    matCR39_NC->SetName("NC_CR39");


    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B4cDetectorConstruction::DefineVolumes()
{
    // Geometry parameters
    // world
    G4double maxStep = 2*nm; // max step dimension for charged particles

    G4double worldSizeXY = 50. *cm;
    G4double worldSizeZ  = 50. *cm;

    //detector
    G4double detectorXY =  5.*cm;
    G4double detectorZ =  0.1*mm;

    //Polymer solid
    G4double dimPolymerXY=2.4*cm;//20.*cm;
    G4double dimPolymerZ=1.*mm;

    //Cd foil
    /*
    G4double foilCdX =  1*mm;
    G4double foilCdY =  10*cm;
G4double foilCdZ = 9.9*cm;
     */

    // Positions
    G4double samplePositionZ= 0.0; //
    G4double detectorPositionZ= dimPolymerZ/2.+detectorZ/2.+5*cm;//0.5*mm;
    /*
    G4double foilPositionX= foilCdX/2 + dimPolymerXY/2 + 0.1*mm; //

    // Create a rotation matrix
    G4RotationMatrix* rotAroundWorldAxis = new G4RotationMatrix();
    rotAroundWorldAxis->rotateY(-0*deg); //45
    G4RotationMatrix* rotAroundObjectAxis = new G4RotationMatrix();
    rotAroundObjectAxis->rotateY(0*deg); //45
    */
    fNofLayers = 1;

    // Get materials
    auto polymer = G4Material::GetMaterial("G4_CR39");
    auto worldMaterial = G4Material::GetMaterial("Galactic");
    //auto cadmium = G4Material::GetMaterial("G4_Cd");

    if (  !polymer ||  !worldMaterial ) {
        G4ExceptionDescription msg;
        msg << "Cannot retrieve materials already defined.";
        G4Exception("B4DetectorConstruction::DefineVolumes()",
                    "MyCode0001", FatalException, msg);
    }

    //
    // World
    //
    auto worldS
            = new G4Box("World",           // its name
                        worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size

    auto worldLV
            = new G4LogicalVolume(
                    worldS,           // its solid
                    worldMaterial,  // its material
                    "World");         // its name

    auto worldPV
            = new G4PVPlacement(
                    0,                // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    worldLV,          // its logical volume
                    "World",          // its name
                    0,                // its mother  volume
                    false,            // no boolean operation
                    0,                // copy number
                    fCheckOverlaps);  // checking overlaps

    //
    // Detector
    //

    auto detectorS
            = new G4Box("Detector",     // its name
                        detectorXY/2, detectorXY/2, detectorZ/2); // its size

    auto detectorLV
            = new G4LogicalVolume(
                    detectorS,     // its solid
                    worldMaterial,  // its material
                    "detectorLV");   // its name

    new G4PVPlacement(
            0,                // no rotation
            G4ThreeVector(0,0,detectorPositionZ+detectorZ/2),  // at (0,0,0)
            detectorLV,          // its logical volume
            "Detector",    // its name
            worldLV,          // its mother  volume
            false,            // no boolean operation
            0,                // copy number
            fCheckOverlaps);  // checking overlaps

    //
    // cadmium foil
    //
/*
    auto foilCdS
            = new G4Box("foilCd",             // its name
                        foilCdX/2, foilCdY/2, foilCdZ/2); // its size

    auto foilCdLV
            = new G4LogicalVolume(
                    foilCdS,             // its solid
                    cadmium,      // its material
                    "foilCdLV");         // its name
    new G4PVPlacement(
            rotAroundObjectAxis,                // no rotation
            (*rotAroundWorldAxis)*G4ThreeVector(foilPositionX,0,0), // its position // multiplying it by the matrix to rotate (nb.: not by its pointer)
            foilCdLV,            // its logical volume
            "foilCd",            // its name
            worldLV,          // its mother  volume
            false,            // no boolean operation
            0,                // copy number
            fCheckOverlaps);  // checking overlaps
*/
    ////////////////////////////////////////////////////////////////////////////


    auto VolumePolymerS
            = new G4Box("VolumePolymer",             // its name
                        dimPolymerXY/2, dimPolymerXY/2, dimPolymerZ/2); // its size

    auto VolumePolymerLV
            = new G4LogicalVolume(
                    VolumePolymerS,             // its solid
                    polymer,      // its material
                    "VolumePolymerLV");         // its name
    new G4PVPlacement(
            0,                // no rotation
            G4ThreeVector(0,0,-samplePositionZ), // its position
            VolumePolymerLV,            // its logical volume
            "VolumePolymer",            // its name
            worldLV,          // its mother  volume
            false,            // no boolean operation
            0,                // copy number
            fCheckOverlaps);  // checking overlaps



    //
    // Visualization attributes
    //
    worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

    G4VisAttributes* polymerAtt = new G4VisAttributes(G4Colour(1., 0., 1.));
    polymerAtt->SetVisibility(true);
    polymerAtt->SetForceSolid(true);
    VolumePolymerLV->SetVisAttributes(polymerAtt);

    // Set transparency attributes
    G4VisAttributes* visAttributesDetector = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5)); // Red color with 50% transparency
    visAttributesDetector->SetForceWireframe(true); // Display wireframe
    visAttributesDetector->SetForceSolid(true);
    detectorLV->SetVisAttributes(visAttributesDetector);

    // foil
    /*
    G4VisAttributes* visAttributesFoilCd = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5)); // Green color with 50% transparency
    visAttributesFoilCd->SetForceSolid(true);
    foilCdLV->SetVisAttributes(visAttributesFoilCd);
*/

    // Example of User Limits
    //
    // Below is an example of how to set tracking constraints in a given
    // logical volume
    //
    // Sets a max step length in the tracker region, with G4StepLimiter

    fStepLimit = new G4UserLimits(maxStep);
    VolumePolymerLV->SetUserLimits(fStepLimit);

    /// Set additional constraints on the track, with G4UserSpecialCuts
    ///
    /// G4double maxLength = 2*trackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
    /// trackerLV->SetUserLimits(new G4UserLimits(maxStep,
    ///                                           maxLength,
    ///                                           maxTime,
    ///                                           minEkin));

    //
    // Always return the physical World
    //
    return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4cDetectorConstruction::ConstructSDandField()
{
    // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    //
    // Sensitive detectors
    //
    /*
    auto foilSD
    = new B4cCalorimeterSD("foilSD", "foilHitsCollection", fNofLayers);
    G4SDManager::GetSDMpointer()->AddNewDetector(foilSD);
    SetSensitiveDetector("foilCdLV",foilSD);
*/

    //Sensitive detector is bound to eventAction class, always check RunAction as well
    //definition of foil SD operation in also still present in calorimeter classes but is not used


    auto detectorSD
            = new B4cCalorimeterSD("detectorSD", "detectorHitsCollection", fNofLayers);
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector("detectorLV",detectorSD);

    auto sampleSD
    = new B4cCalorimeterSD("sampleSD", "sampleHitsCollection", fNofLayers);
    G4SDManager::GetSDMpointer()->AddNewDetector(sampleSD);
    SetSensitiveDetector("VolumePolymerLV",sampleSD);

    //
    // Magnetic field
    //
    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    G4ThreeVector fieldValue;
    fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    fMagFieldMessenger->SetVerboseLevel(1);

    // Register the field messenger for deleting
    G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
