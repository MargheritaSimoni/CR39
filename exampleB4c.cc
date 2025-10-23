#undef MY_MULTITHREADED //  undefining it to run NCrystal

#include "B4cDetectorConstruction.hh"
#include "B4cActionInitialization.hh"

#ifdef MY_MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4PhysListFactory.hh"
#include "FTFP_BERT_HP.hh"
#include "QGSP_BIC_HP.hh"
//#include "G4NeutronHPElastic.hh" // Include header for neutron elastic scattering process
#include "G4HadronElasticPhysicsHP.hh"
#include "G4NeutronHPThermalScattering.hh"
#include "G4VPhysicsConstructor.hh"
//#include "CustomPhysicsConstructor.hh"
//#include "G4OpticalParameters.hh"
//#include "G4OpticalPhysics.hh"
//#include "PhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4NCrystal/G4NCrystal.hh"
#include "NCrystal/NCrystal.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " exampleB4c [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  NCrystal::libClashDetect();//Detect broken installation
  // Evaluate arguments
  //
  if ( argc > 7 ) {
    PrintUsage();
    return 1;
  }
  
  G4String macro;
  G4String session;

#ifdef MY_MULTITHREADED
  G4int nThreads = 0;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef MY_MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  long seed=30; // setting seed of random generartor, the line above sets to default, which is 0
  G4Random::setTheSeed(seed);
  // Construct the default run manager
  //

#ifdef MY_MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nThreads > 0 ) {
    runManager->SetNumberOfThreads(nThreads);
  }  
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  auto detConstruction = new B4cDetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

    ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


  //stessa cosa di sotto ma passando per factory, non serve includere l'header file specifico
  //G4PhysListFactory factory;
  //G4VModularPhysicsList* physicsList = factory.GetReferencePhysList("QGSP_BIC_HP");

  G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
  G4StepLimiterPhysics* stepLimitPhys = new G4StepLimiterPhysics(); // to be set inside detector construction
  //stepLimitPhys->SetApplyToAll(true); // without this only applies to charged particles
    //to change step
  physicsList-> RegisterPhysics(stepLimitPhys);

    // Set the customized physics list to the run manager
  runManager->SetUserInitialization(physicsList);

////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

    auto actionInitialization = new B4cActionInitialization();
  runManager->SetUserInitialization(actionInitialization);
  runManager->Initialize();

    //Install G4NCrystal:
  G4NCrystal::installOnDemand(); //nb.: NC has to be installed under initialization of runManager

  // Initialize visualization
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();


    // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();


  // Process macro or start UI session
  //
  if ( macro.size() ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else  {  
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !

  delete visManager;
  delete runManager;
    //Cleanup:
  //G4NCrystal::Manager::cleanup();//delete manager singleton, unref cached ncrystal objects (for valgrind).

  G4cout << "Application successfully ended.\nBye :-)" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
