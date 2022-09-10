//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: LXeEventAction.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical/LXe/src/LXeEventAction.cc
/// \brief Implementation of the LXeEventAction class
//
//
#include "LXeEventAction.hh"
#include "LXeScintHit.hh"
#include "LXePMTHit.hh"
#include "LXeUserEventInformation.hh"
#include "LXeTrajectory.hh"
#include "LXeRecorderBase.hh"
#include "B5Analysis.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeEventAction::LXeEventAction(LXeRecorderBase* r)
  : fRecorder(r),fSaveThreshold(0),fScintCollID(-1),fPMTCollID(-1),fVerbose(0),
   fPMTThreshold(1),fForcedrawphotons(false),fForcenophotons(false)
{
  fEventMessenger = new LXeEventMessenger(this);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeEventAction::~LXeEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeEventAction::BeginOfEventAction(const G4Event* anEvent){
 
  //New event, add the user information object
  G4EventManager::
    GetEventManager()->SetUserInformation(new LXeUserEventInformation);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fScintCollID<0)
    fScintCollID=SDman->GetCollectionID("scintCollection");
  if(fPMTCollID<0)
    fPMTCollID=SDman->GetCollectionID("pmtHitCollection");

  if(fRecorder)fRecorder->RecordBeginOfEvent(anEvent);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeEventAction::EndOfEventAction(const G4Event* anEvent){

 //
  auto analysisManager = G4AnalysisManager::Instance();//////////////

 G4int gamma=1;
    G4int Posit1=2;
    G4int Elect1=3;
    G4int Muonp1=5;
    G4int Muonm1=6;

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  
   G4ParticleDefinition* Gamma = particleTable->FindParticle("gamma");
   G4ParticleDefinition* Posit  = particleTable->FindParticle("e+");
   G4ParticleDefinition* Elect = particleTable->FindParticle("e-");
   G4ParticleDefinition* Muonp = particleTable->FindParticle("mu+");
   G4ParticleDefinition* Muonm = particleTable->FindParticle("mu-");
 
 LXeUserEventInformation* eventInformation
    =(LXeUserEventInformation*)anEvent->GetUserInformation();
 
  G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
 
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // extract the trajectories and draw them
  if (G4VVisManager::GetConcreteInstance()){
    for (G4int i=0; i<n_trajectories; i++){
      LXeTrajectory* trj = (LXeTrajectory*)
        ((*(anEvent->GetTrajectoryContainer()))[i]);
      if(trj->GetParticleName()=="opticalphoton"){
        trj->SetForceDrawTrajectory(fForcedrawphotons);
        trj->SetForceNoDrawTrajectory(fForcenophotons);
      }
      trj->DrawTrajectory();
    }
  }
 
  LXeScintHitsCollection* scintHC = 0;
  LXePMTHitsCollection* pmtHC = 0;
  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
 
  //Get the hit collections
  if(hitsCE){
    if(fScintCollID>=0)scintHC = (LXeScintHitsCollection*)(hitsCE->GetHC(fScintCollID));
    if(fPMTCollID>=0)pmtHC = (LXePMTHitsCollection*)(hitsCE->GetHC(fPMTCollID));
  }

  //Hits in scintillator
  if(scintHC){
    int n_hit = scintHC->entries();
    G4ThreeVector  eWeightPos(0.);
    G4double edep;
    G4double edepMax=0;
    G4int particleName;

    for(int i=0;i<n_hit;i++){ //gather info on hits in scintillator
     auto hit1 = static_cast<LXeScintHit*>(scintHC->GetHit(i));
     G4ParticleDefinition* partId = hit1->GetPartId();
     //Given a number for each particle
  if ( partId == Gamma)
            {
              particleName = 1;//gamma
           // analysisManager->FillH1(2,gamma);
            }
          else if (  partId ==Posit )
            {                       //e+
              particleName = 2;
             // analysisManager->FillH1(3,Posit1);
            }
          else if (  partId == Elect)
            {                       //e-
              particleName = 3 ;
              //analysisManager->FillH1(4,Elect1);
            }
          else if (  partId == Muonp)
            {                       //mu+
              particleName = 5;
              //analysisManager->FillH1(5,Muonp1);
            }
          else if (  partId == Muonm)
            {                       //mu-
              particleName = 6;
             // analysisManager->FillH1(6,Muonm1);
            }
     //G4String PartId = particleTable->GetParticleName(parId);
  //Fill histogram
      analysisManager->FillH1(2, particleName);

      edep=(*scintHC)[i]->GetEdep();
      eventInformation->IncEDep(edep); //sum up the edep
      eWeightPos += (*scintHC)[i]->GetPos()*edep;//calculate energy weighted pos
      if(edep>edepMax){
        edepMax=edep;//store max energy deposit
        G4ThreeVector posMax=(*scintHC)[i]->GetPos();
        eventInformation->SetPosMax(posMax,edep);
      }
    }
    if(eventInformation->GetEDep()==0.){
      if(fVerbose>0)G4cout<<"No hits in the scintillator this event."<<G4endl;
    }
    else{
      //Finish calculation of energy weighted position
      eWeightPos/=eventInformation->GetEDep();
      eventInformation->SetEWeightPos(eWeightPos);
      if(fVerbose>0){
        G4cout << "\tEnergy weighted position of hits in LXe : "
               << eWeightPos/mm << G4endl;
      }
    }
    if(fVerbose>0){
    G4cout << "\tTotal energy deposition in scintillator : "
           << eventInformation->GetEDep() / keV << " (keV)" << G4endl;
    }
  }
 
  if(pmtHC){
    G4ThreeVector reconPos(0.,0.,0.);
    G4int pmts=pmtHC->GetSize();//entries
   // G4double hitime;
    
    //Gather info from all PMTs
    for(G4int i=0;i<pmts;i++){
    // hitime =(*pmtHC)[i]->GetTime();
     auto hit = static_cast<LXePMTHit*>(pmtHC->GetHit(i));
      eventInformation->IncHitCount((*pmtHC)[i]->GetPhotonCount());
      reconPos+=(*pmtHC)[i]->GetPMTPos()*(*pmtHC)[i]->GetPhotonCount();
      // fill ntuple
      //analysisManager->FillNtupleDColumn(0,hit->GetTime());
       analysisManager->FillH1(0, hit->GetTime());
      
      if((*pmtHC)[i]->GetPhotonCount()>=fPMTThreshold){
        eventInformation->IncPMTSAboveThreshold();
      }
      else{//wasnt above the threshold, turn it back off
        (*pmtHC)[i]->SetDrawit(false);
      }
    }
    
 
    if(eventInformation->GetHitCount()>0){//dont bother unless there were hits
      reconPos/=eventInformation->GetHitCount();
      if(fVerbose>0){
        G4cout << "\tReconstructed position of hits in LXe : "
               << reconPos/mm << G4endl;
      }
      eventInformation->SetReconPos(reconPos);
    }
    pmtHC->DrawAllHits();
  }

//analysisManager->AddNtupleRow();
  if(fVerbose>0){
    //End of event output. later to be controlled by a verbose level
    G4cout << "\tNumber of photons that hit PMTs in this event : "
           << eventInformation->GetHitCount() << G4endl;
    G4cout << "\tNumber of PMTs above threshold("<<fPMTThreshold<<") : "
           << eventInformation->GetPMTSAboveThreshold() << G4endl;
    G4cout << "\tNumber of photons produced by scintillation in this event : "
           << eventInformation->GetPhotonCount_Scint() << G4endl;
    G4cout << "\tNumber of photons produced by cerenkov in this event : "
           << eventInformation->GetPhotonCount_Ceren() << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
           << eventInformation->GetAbsorptionCount() << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
           << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
           << G4endl;
    G4cout << "Unacounted for photons in this event : "
           << (eventInformation->GetPhotonCount_Scint() +
               eventInformation->GetPhotonCount_Ceren() -
               eventInformation->GetAbsorptionCount() -
               eventInformation->GetHitCount() -
               eventInformation->GetBoundaryAbsorptionCount())
           << G4endl;
  }
  //If we have set the flag to save 'special' events, save here
  if(fSaveThreshold&&eventInformation->GetPhotonCount() <= fSaveThreshold)
    G4RunManager::GetRunManager()->rndmSaveThisEvent();

  if(fRecorder)fRecorder->RecordEndOfEvent(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeEventAction::SetSaveThreshold(G4int save){
/*Sets the save threshold for the random number seed. If the number of photons
generated in an event is lower than this, then save the seed for this event
in a file called run###evt###.rndm
*/
  fSaveThreshold=save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
  //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
}
