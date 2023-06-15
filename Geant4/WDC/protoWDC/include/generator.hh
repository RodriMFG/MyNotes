//Header file to function generator

#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

//inherited from class G4VUserPrimaryGeneratorAction
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    //construct and destruct generator
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    //function that creates the primaries for the particle
    //generator of action
    virtual void GeneratePrimaries(G4Event*);

private:
    //define the particle gun
    G4ParticleGun *fParticleGun;
};

#endif
