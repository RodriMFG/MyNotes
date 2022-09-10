//Function to define what action function will generate

#include "generator.hh"

//construct and destruct particle gun
MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    //tell the program to get the list of particles
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    //set the search name in type G4 string
    G4String particleName="proton";
    //assign the result of search for proton to particle
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");

    //define vector for the initial conditions of the particle
    G4ThreeVector pos(0.,0., 20*m);
    G4ThreeVector mom(0.,0., -1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(0.9 * GeV);
    fParticleGun->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

//function that generates an event (particle)
void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
