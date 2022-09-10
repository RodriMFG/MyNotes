//Function to define what action function will generate

#include "generator.hh"

//construct and destruct particle gun
MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

//function that generates an event (particle)
void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4double fExpHall_z = 20.0*m;
    //tell the program to get the list of particles
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    //set the search name in type G4 string
    G4String particleName="proton";
    //assign the result of search for proton to particle
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            //define vector for the initial conditions of the particle
            G4ThreeVector pos(2*m - 0.8*i*m, 2*m - 0.8*j*m, fExpHall_z);
            G4ThreeVector mom(0.,0.,-1.);

            //set the initial conditions that we defined
            fParticleGun->SetParticlePosition(pos);
            fParticleGun->SetParticleMomentumDirection(mom);
            fParticleGun->SetParticleMomentum(10.*GeV);
            //define the type of particle
            fParticleGun->SetParticleDefinition(particle);

            //tell G4 to generate the event
            fParticleGun->GeneratePrimaryVertex(anEvent);
        }
    }
}
