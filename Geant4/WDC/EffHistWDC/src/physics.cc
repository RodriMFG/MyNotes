//Functions that define the physics included in the simulation,
//such as the type of interactions that the particles go through

#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
    //Use G4 EM interactions and optical interactions
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}
