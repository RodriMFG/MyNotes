#include "run.hh"

MyRunAction::MyRunAction()
{
    //
    G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();

    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fWlen");
    man->FinishNtuple(0);

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fWlen");
    man->FinishNtuple(1);

    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(2);
    //
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();

    //
    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output.root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}

/* To view a color histogram of the hits in XY
$ root output.root
root[] Hits->Draw("fX:fY", "", "colz")
*/
