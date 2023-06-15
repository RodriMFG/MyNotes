//Header file for function to detect the photons

#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"

//#include "run.hh"

//inherited from G4VSensitiveDetector
class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    //construct and destruct
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();

private:
    //main function for detection
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif
