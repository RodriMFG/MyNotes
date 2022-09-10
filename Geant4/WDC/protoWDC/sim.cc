//Main function of simulation

#include <iostream>

//include the relevant G4 headers
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//include the header files we wrote
#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
	//class that controls all the actions and stepping
    G4RunManager *runManager = new G4RunManager();

    //use the detector in the simulation
    runManager->SetUserInitialization(new MyDetectorConstruction());
    //use the types of physics we defined for the simulation
    runManager->SetUserInitialization(new MyPhysicsList());
    //
    runManager->SetUserInitialization(new MyActionInitialization());

    runManager->Initialize();

    G4UIExecutive *ui = 0;

    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}
