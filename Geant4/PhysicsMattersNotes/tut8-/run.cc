#include "run.hh"

MyRunAction::MyRunAction()
{
    //
    G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(0);
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

    man->OpenFile("output"+strRunID.str()+".root");
    //

    /*man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(0);*/
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}
