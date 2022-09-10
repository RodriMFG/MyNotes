//Header to define the classes used in the physics list

#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

//inherited from G4VModularPhysicsList
class MyPhysicsList : public G4VModularPhysicsList
{
public:
    //construct and destruct the physics list
    MyPhysicsList();
    ~MyPhysicsList();
};

#endif
