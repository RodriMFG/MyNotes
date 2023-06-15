//Function to apply detection

#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

//function to process hits
G4bool MySensitiveDetector::ProcessHits(G4Step *aStep,
G4TouchableHistory *ROhist)
{
    //function to get track of the particle
    G4Track *track = aStep->GetTrack();

    //function to avoid processing the photons that don't enter
    //from the front of the detecting cells
    track->SetTrackStatus(fStopAndKill);

    //beginning and end point of the particle entering
    //and exiting the sensitive volume
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();

    //get access to the physical volume of the sensors
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    //get  the copy number of out cell
    G4int copyNo = touchable->GetCopyNumber();

    //cout the copy number the hit cell
    //G4cout << "Copy number: " << copyNo << G4endl;

    //function to get position of hit detector
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();

    //cout of the hit cell position
    G4cout << "Detector position: " << posDetector << G4endl;

    return true;
}
