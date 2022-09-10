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

	//class for ui
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);

	//class for vis managing
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

	//class for managing ui
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    //use openGL
    UImanager->ApplyCommand("/vis/open OGL");
    //set initial viewpoint
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    //tell G4 to draw the volume
    UImanager->ApplyCommand("/vis/drawVolume");
    //update everytime and event is created
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    //smoothen trajectories
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");

    ui->SessionStart();
    
    delete visManager;
    delete runManager;

    return 0;
}
