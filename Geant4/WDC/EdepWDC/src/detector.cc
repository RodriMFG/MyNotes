//Function to apply detection

#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    quEff = new G4PhysicsOrderedFreeVector;

    std::ifstream datafile;
    datafile.open("eff.dat");

    while(1)
    {
        G4double wlen, queff;

        datafile >> wlen >> queff;

        if(datafile.eof())
            break;

        G4cout << wlen << " " << queff << G4endl;

        quEff->InsertValues(wlen, queff/100);
    }

    datafile.close();
}

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
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;

    //get access to the physical volume of the sensors
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    //get  the copy number of out cell
    G4int copyNo = touchable->GetCopyNumber();

    //cout the copy number the hit cell
    //G4cout << "Copy number: " << copyNo << G4endl;

    //function to get position of hit detector
    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()
        ->GetEventID();

    //cout of the hit cell position
    G4cout << "Detector position: " << posDetector << G4endl;

    G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();

    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, posPhoton[0]);
    man->FillNtupleDColumn(0, 2, posPhoton[1]);
    man->FillNtupleDColumn(0, 3, posPhoton[2]);
    man->FillNtupleDColumn(0, 4, wlen);
    man->AddNtupleRow(0);

    if(G4UniformRand() < quEff->Value(wlen))
    {
        man->FillNtupleIColumn(1, 0, evt);
        man->FillNtupleDColumn(1, 1, posDetector[0]);
        man->FillNtupleDColumn(1, 2, posDetector[1]);
        man->FillNtupleDColumn(1, 3, posDetector[2]);
        man->FillNtupleDColumn(1, 4, wlen);
        man->AddNtupleRow(1);
    }

    return true;
}

/* To view a color histogram of the hits in XY
$
    root output.root
root[]
    Hits/Photons->Draw("fX:fY", "", "colz")
*/
