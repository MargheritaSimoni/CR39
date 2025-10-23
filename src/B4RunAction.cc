#include "B4RunAction.hh"
#include "B4Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4AccumulableManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4RunAction::B4RunAction()
: G4UserRunAction()
{


    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);
    
    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace
    // in B4Analysis.hh
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;
    
    // Create directories
    //analysisManager->SetHistoDirectoryName("histograms");
    //analysisManager->SetNtupleDirectoryName("ntuple");
    analysisManager->SetVerboseLevel(2);
    analysisManager->SetNtupleMerging(false);
    // Note: merging ntuples is available only with Root output
    
    // Book histograms, ntuple
    //
G4double maxTrack = 5*mm;
G4double maxEnergy = 100*MeV;

    // Creating histograms // name and description of istograms
    analysisManager->CreateH1("E0","Initial Neutron Energy",200, 0.5e-04*eV, maxEnergy,"eV","none","log"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(0, "Energy [eV]");
    analysisManager->SetH1YAxisTitle(0, "Counts");

    analysisManager->CreateH1("ENeutron","Neutron Energy in Detector",200, 0.5e-04*eV, maxEnergy,"eV","none","log"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(1, "Energy [eV]");
    analysisManager->SetH1YAxisTitle(1, "Counts");

    analysisManager->CreateH1("TENeutron","Transmitted neutron Energy ",200, 0.5e-04*eV,maxEnergy,"eV","none","log"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(2, "Energy [eV]");
    analysisManager->SetH1YAxisTitle(2, "Counts");

    analysisManager->CreateH1("EBoundary","Energy on boundary of solid ",200, 0.5e-04*eV,maxEnergy,"eV","none","log"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(3, "Energy [eV]");
    analysisManager->SetH1YAxisTitle(3, "Counts");

    analysisManager->CreateH1("ZBoundary","Z coordinate on boundary of solid ",200, -0.5*mm, 0.5*mm,"mm"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(4, "z [mm]");
    analysisManager->SetH1YAxisTitle(4, "Counts");

    analysisManager->CreateH1("SecTrackL","track lengths of secondaries ",500, 0.*um, maxTrack,"um"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(5, "z [um]");
    analysisManager->SetH1YAxisTitle(5, "Counts");

    analysisManager->CreateH1("SecTrackL_500nm","track lengths of secondaries ",500, 0.*um, maxTrack,"um"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(6, "z [um]");
    analysisManager->SetH1YAxisTitle(6, "Counts");

    analysisManager->CreateH1("SecTrackL_1um","track lengths of secondaries ",500, 0.*um, maxTrack,"um"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(7, "z [um]");
    analysisManager->SetH1YAxisTitle(7, "Counts");

    /*
    analysisManager->CreateH1("ENside","Energy of neutrons that come out of one side ",200, 0.5e-04*eV, 14.1*MeV,"eV","none","log"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
    analysisManager->SetH1XAxisTitle(5, "Energy [eV]");
    analysisManager->SetH1YAxisTitle(5, "Counts");
*/


    /*
analysisManager->CreateH1("EGamma","Energy of gamma from foil ",200, 100.*eV, 10*MeV,"MeV","none","log"); // third entry is type of binning (log binning)//nb min can't be 0 in log scale//nb da anche il max dell'istogramma
analysisManager->SetH1XAxisTitle(6, "Energy [MeV]");
analysisManager->SetH1YAxisTitle(6, "Counts");
*/

    //2D istograms
    analysisManager->CreateH2("DetPos","Position in detector", 200, -2.5*cm, 2.5*cm, 200, -2.5*cm, 2.5*cm, "cm", "cm");
    analysisManager->SetH2XAxisTitle(0, "x [cm]");
    analysisManager->SetH2YAxisTitle(0, "y [cm]");

    analysisManager->CreateH2("TPos","Position in transmission detector", 200, -2.5*cm, 2.5*cm, 200, -2.5*cm, 2.5*cm, "cm", "cm");
    analysisManager->SetH2XAxisTitle(1, "x [cm]");
    analysisManager->SetH2YAxisTitle(1, "y [cm]");

    analysisManager->CreateH2("GenPos","Generator Position", 200, -5*cm, 5*cm, 200, -5*cm, 5*cm, "cm", "cm");
    analysisManager->SetH2XAxisTitle(2, "x [cm]");
    analysisManager->SetH2YAxisTitle(2, "y [cm]");

    analysisManager->CreateH2("AvsTrackLength","Atomic number vs tracklength", 500, 0.*um, maxTrack, 21, -0.5, 20.5, "um");
    analysisManager->SetH2XAxisTitle(3,  "Track length [um]");
    analysisManager->SetH2YAxisTitle(3,"Atomic mass");

    analysisManager->CreateH2("EvsTrackLength","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(4,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(4,"Track length [um]");

    analysisManager->CreateH2("AvsE","Atomic number vs energy", 500, 0*MeV, maxEnergy, 21, -0.5, 20.5, "MeV");
    analysisManager->SetH2XAxisTitle(5,  "Energy [MeV]");
    analysisManager->SetH2YAxisTitle(5,"Atomic mass");


    analysisManager->CreateH2("EvsTrackL_H","Energy vs tracklength", 500, 0*MeV,maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(6,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(6,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_C","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(7,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(7,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_O","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(8,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(8,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_H_1nm","Energy vs tracklength", 500, 0*MeV,maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(9,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(9,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_C_1nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(10,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(10,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_O_1nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(11,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(11,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_H_100nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(12,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(12,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_C_100nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(13,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(13,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_O_100nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(14,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(14,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_H_500nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(15,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(15,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_C_500nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(16,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(16,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_O_500nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(17,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(17,"Track length [um]");

    analysisManager->CreateH2("AvsZ","Atomic number vs energy", 21, -0.5, 20.5, 21, -0.5, 20.5);
    analysisManager->SetH2XAxisTitle(18,  "Atomic number");
    analysisManager->SetH2YAxisTitle(18,"Atomic mass");


    analysisManager->CreateH2("EvsTrackL_He_1nm","Energy vs tracklength", 500, 0*MeV,maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(19,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(19,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_Be_1nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(20,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(20,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_B_1nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(21,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(21,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_Li_1nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(22,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(22,"Track length [um]");

    analysisManager->CreateH2("EvsTrackL_N_1nm","Energy vs tracklength", 500, 0*MeV, maxEnergy, 500, 0.*um, maxTrack, "MeV", "um");
    analysisManager->SetH2XAxisTitle(23,"Energy [MeV]" );
    analysisManager->SetH2YAxisTitle(23,"Track length [um]");

    /*

    analysisManager->CreateH2("SidePos","position on side of detector", 200, -5*cm, 5*cm, 200, -5.4*cm, 5.4*cm, "cm", "cm");
    analysisManager->SetH2XAxisTitle(3, "x [cm]");
    analysisManager->SetH2YAxisTitle(3, "y [cm]");
*/
    // Creating ntuple
    //

  /*  analysisManager->CreateNtuple("B4", "Data Tree");
    analysisManager->CreateNtupleDColumn("SecondaryZ");
    analysisManager->CreateNtupleDColumn("SecondaryA");
    analysisManager->CreateNtupleDColumn("SecondaryTracklength");
    analysisManager->CreateNtupleDColumn("SecondaryEnergy");

    //analysisManager->CreateNtupleDColumn("X0");
    //analysisManager->CreateNtupleDColumn("Y0");
    analysisManager->FinishNtuple();*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4RunAction::~B4RunAction()
{
    //delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
    //inform the runManager to save random number seed
    //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root"); // necessary in geant4-v11.3.2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Open an output file
    //
    G4String fileName = "B4";
    analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::EndOfRunAction(const G4Run* run)
{


    // print histogram statistics
    //
    auto analysisManager = G4AnalysisManager::Instance();
    if ( analysisManager->GetH1(0) ) {
        G4cout << G4endl << " ----> print histograms statistic ";
        if(isMaster) {
            G4cout << "for the entire run " << G4endl << G4endl;
        }
        else {
            G4cout << "for the local thread " << G4endl << G4endl;
        }

        G4int nofEvents = run->GetNumberOfEvent();
        G4cout << nofEvents << " events were processed." << G4endl;


        G4cout << " " << G4endl;
        G4cout << " Number of neutrons that come out of solid= "
               << analysisManager->GetH1(3)->entries() << G4endl;
        G4cout << " their average energy is = "
               << analysisManager->GetH1(3)->mean() << " eV "
               << ", their root mean square is= "
               << analysisManager->GetH1(3)->rms() << " eV" << G4endl;

        G4cout << " " << G4endl;
        G4cout << "Number of secondary tracks above 1 nm: "
               << analysisManager->GetH1(5)->entries() << G4endl;
        G4cout << " " << G4endl;


        G4cout << "Protons: "
               << analysisManager->GetH2(9)->entries() << G4endl;
        G4cout << "Carbons: "
               << analysisManager->GetH2(10)->entries() << G4endl;
        G4cout << "Oxygens: "
               << analysisManager->GetH2(11)->entries() << G4endl;


/*
        G4cout << " " << G4endl;
        G4cout << " Number of gamma rays produced in the foil is = "
               << analysisManager->GetH1(6)->entries() << G4endl;
        G4cout << " their average energy is = "
               << G4BestUnit(analysisManager->GetH1(6)->mean(), "Energy")
               << ", their root mean square is= "
               << G4BestUnit(analysisManager->GetH1(6)->rms(),  "Energy") << G4endl;
*/
        G4cout << " " << G4endl;
    }
    
    // save histograms & ntuple
    //
    analysisManager->Write();
    analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
