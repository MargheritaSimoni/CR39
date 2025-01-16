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

#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* B4cDetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cDetectorConstruction::B4cDetectorConstruction()
: G4VUserDetectorConstruction(),
fCheckOverlaps(true),
fNofLayers(-1)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4cDetectorConstruction::~B4cDetectorConstruction()
{ 
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
    // Lead material defined using NIST Manager
    auto nistManager = G4NistManager::Instance();
    // air
    nistManager->FindOrBuildMaterial("G4_AIR");   
    // water
    nistManager->FindOrBuildMaterial("G4_WATER");   
    // PMMA
    nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
    
    // Oil defined by me, for now it is only defined as oleic acid C18H34O2, density is set as average density of olive oil
    G4Element* elC = nistManager->FindOrBuildElement("C");
    G4Element* elH = nistManager->FindOrBuildElement("H");
    G4Element* elO = nistManager->FindOrBuildElement("O");
    
    G4double ldensity = 0.93 * g/cm3;
    
    G4Material* fOIL = new G4Material("M_OIL", ldensity, 3); // 3 is the number of components (3 elements)
    fOIL->AddElement(elC, 18);
    fOIL->AddElement(elH, 34);
    fOIL->AddElement(elO, 2);
    
    
    new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                   kStateGas, 2.73*kelvin, 3.e-18*pascal);

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B4cDetectorConstruction::DefineVolumes()
{
    // Geometry parameters
    
   	  //detector
    G4double detectorXY =  4.*mm;
    G4double detectorZ =  1.*mm;	  	  
    	  //air layer in front of the detector used to count neutrons
    auto airLayerXY =  detectorXY; 
    auto airLayerZ =  detectorZ; 
	  // PMMA cylinder
    G4double rCylinderPMMA= 1.6*mm;
    G4double hCylinderPMMA= 3*mm;
    
  	  // water cylinder
    G4double rCylinderH2O= 1.4*mm;
    G4double hCylinderH2O= 2.5*mm;
  	  // air cylinder
    auto rCylinderAir= rCylinderH2O;
    auto hCylinderAir= hCylinderH2O+((hCylinderPMMA-hCylinderH2O)/2);
    auto traslCylinderAir=(hCylinderPMMA-hCylinderH2O)/4;
   	  // oil sphere
    G4double rSphereOil= 1.2*mm;
          // trasl. from origin
    auto samplePosition= rCylinderPMMA+10.*mm;     
    auto detectorPosition= detectorZ+10.*mm;  
    
    // Create a rotation matrix for a 90-degree rotation around the y-axis
    G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(90.0 * deg);
    
          // world
    auto worldSizeXY = 2. * detectorXY;
    auto worldSizeZ  = samplePosition+detectorPosition+10.*mm;
    
     
    
    //fNofLayers = 1;
    //G4double xtalLength =  2.*cm;
    //G4double xtalXY = 2.*cm;

    //G4double pmtXY = 20.*mm;
    //G4double pmtZ = 1.*mm;
    
    //G4double calorSizeXY  = xtalXY;
    //G4double calorSizeZ = xtalLength + pmtZ;
    
    //auto worldSizeXY = 2. * calorSizeXY;
    //auto worldSizeZ  = 2. * calorSizeZ;
    
    
    // Get materials
    auto water = G4Material::GetMaterial("G4_WATER");
    auto air = G4Material::GetMaterial("G4_AIR");
    auto PMMA = G4Material::GetMaterial("G4_PLEXIGLASS");
    auto oil = G4Material::GetMaterial("M_OIL");
    auto defMat = G4Material::GetMaterial("Galactic");
    
    if ( ! water || ! air || ! PMMA || ! oil ) { //gives an error if materials are not correctly defined 
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
                          defMat,  // its material
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
                          air,  // its material
                          "detectorLV");   // its name
                          
    auto detectorPV    
    = new G4PVPlacement(
                      0,                // no rotation
                      G4ThreeVector(0,0,detectorPosition+detectorZ/2),  // at (0,0,0)
                      detectorLV,          // its logical volume
                      "Detector",    // its name
                      worldLV,          // its mother  volume
                      false,            // no boolean operation
                      0,                // copy number
                      fCheckOverlaps);  // checking overlaps
      
    //
    // air layer for detector
    //
    
    auto airLayerS
    = new G4Box("AirLayer",             // its name
                airLayerXY/2, airLayerXY/2, airLayerZ/2); // its size
    
    auto airLayerLV
    = new G4LogicalVolume(
                          airLayerS,             // its solid
                          defMat,      // its material
                          "airLayerLV");         // its name
    auto airLayerPV
    = new G4PVPlacement(
                      0,                // no rotation
                      G4ThreeVector(0,0,detectorPosition-detectorZ/2), // its position
                      airLayerLV,            // its logical volume
                      "AirLayer",            // its name
                      worldLV,          // its mother  volume
                      false,            // no boolean operation
                      0,                // copy number
                      fCheckOverlaps);  // checking overlaps
    
    //
    // PMMA cylinder
    //
   
    auto cylinderPMMAS
    = new G4Tubs("CylinderPMMA",             // its name
                0., rCylinderPMMA, hCylinderPMMA/2, 0., 2*pi); // its size
    
    auto cylinderPMMALV
    = new G4LogicalVolume(
                          cylinderPMMAS,             // its solid
                          PMMA,      // its material
                          "CylinderPMMALV");         // its name
    auto cylinderPMMAPV
    = new G4PVPlacement(
                      rotationMatrix,                // no rotation
                      G4ThreeVector(0,0,-samplePosition), // its position
                      cylinderPMMALV,            // its logical volume
                      "CylinderPMMA",            // its name
                      worldLV,          // its mother  volume
                      false,            // no boolean operation
                      0,                // copy number
                      fCheckOverlaps);  // checking overlaps

    //
    // Air cylinder
    //
    
    auto cylinderAirS
    = new G4Tubs("CylinderAir",             // its name
                0., rCylinderAir, hCylinderAir/2, 0., 2*pi); // its size
    
    auto cylinderAirLV
    = new G4LogicalVolume(
                          cylinderAirS,             // its solid
                          air,      // its material
                          "CylinderAirLV");         // its name
    auto cylinderAirPV
    = new G4PVPlacement(
                      0,                // no rotation
                      G4ThreeVector(0,0,traslCylinderAir), // its position
                      cylinderAirLV,            // its logical volume
                      "CylinderAir",            // its name
                      cylinderPMMALV,          // its mother  volume
                      false,            // no boolean operation
                      0,                // copy number
                      fCheckOverlaps);  // checking overlaps
       
    //
    // Water cylinder
    //
    
    auto cylinderH2OS
    = new G4Tubs("CylinderH2O",             // its name
                0., rCylinderH2O, hCylinderH2O/2, 0., 2*pi); // its size
    
    auto cylinderH2OLV
    = new G4LogicalVolume(
                          cylinderH2OS,             // its solid
                          water,      // its material
                          "CylinderH2OLV");         // its name
    auto cylinderH2OPV
    = new G4PVPlacement(
                      0,                // no rotation
                      G4ThreeVector(0,0,-traslCylinderAir), // its position
                      cylinderH2OLV,            // its logical volume
                      "CylinderH2O",            // its name
                      cylinderAirLV,          // its mother  volume
                      false,            // no boolean operation
                      0,                // copy number
                      fCheckOverlaps);  // checking overlaps 
                      


    //
    // Oil sphere
    //
    
    auto sphereOilS
    = new G4Sphere("SphereOil",             // its name
                0., rSphereOil, 0, 360.0 * degree, 0, 180.0 * degree); // its size
    
    auto sphereOilLV
    = new G4LogicalVolume(
                          sphereOilS,             // its solid
                          oil,      // its material
                          "SphereOilLV");         // its name
    auto sphereOilPV
    = new G4PVPlacement(
                      0,                // no rotation
                      G4ThreeVector(0,0,0), // its position
                      sphereOilLV,            // its logical volume
                      "SphereOil",            // its name
                      cylinderH2OLV,          // its mother  volume
                      false,            // no boolean operation
                      0,                // copy number
                      fCheckOverlaps);  // checking overlaps 

    
    //
    // Visualization attributes
    //
    worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());
    
    auto cylinderPMMAVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0, 0.3));
    cylinderPMMAVisAtt->SetVisibility(true);
    cylinderPMMAVisAtt->SetForceSolid(true);
    cylinderPMMALV->SetVisAttributes(cylinderPMMAVisAtt);
    
    G4VisAttributes* cylH2oAtt = new G4VisAttributes(G4Colour(0., 0., 1., 0.6)); // last is opacity
    cylH2oAtt->SetVisibility(true);
    cylH2oAtt->SetForceSolid(true);
    cylinderH2OLV->SetVisAttributes(cylH2oAtt);
    
    G4VisAttributes* oilAtt = new G4VisAttributes(G4Colour(1., 0., 1.));
    oilAtt->SetVisibility(true);
    oilAtt->SetForceSolid(true);
    sphereOilLV->SetVisAttributes(oilAtt);
    
    // Set transparency attributes
    G4VisAttributes* visAttributesDetector = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5)); // Red color with 50% transparency
    visAttributesDetector->SetForceWireframe(true); // Display wireframe
    visAttributesDetector->SetForceSolid(true);
    detectorLV->SetVisAttributes(visAttributesDetector);
    
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
    
        // //qui assegno il sensitive detector
    auto detectorSD
    = new B4cCalorimeterSD("detectorSD", "detectorHitsCollection", fNofLayers);
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector("detectorLV",detectorSD);
    
   // //qui creo solo il sensitive detector ma in realtà non lo uso poi
    auto airLayerSD
    = new B4cCalorimeterSD("airLayerSD", "airLayerHitsCollection", fNofLayers);
    G4SDManager::GetSDMpointer()->AddNewDetector(airLayerSD);
    SetSensitiveDetector("airLayerLV",airLayerSD);


    //
    // Magnetic field
    //
    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    G4ThreeVector fieldValue = G4ThreeVector(0, 0, 0);
    fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    fMagFieldMessenger->SetVerboseLevel(1);
    
    // Register the field messenger for deleting
    G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
