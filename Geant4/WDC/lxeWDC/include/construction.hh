//Header to defines classes used in the construction of
//the detector

//to not include the heder file multiple times
#ifndef CONSTRUCTION_HH
//define header file
#define CONSTRUCTION_HH

//relevant G4 classes
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"

#include "detector.hh"

//class is inherited from G4VUserDetectorConstruction
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    //construct and destruct the detector
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    //main function that constructs detector geometry
    virtual G4VPhysicalVolume *Construct();

private:
    //function that is refered to outside of construction.cc
    G4LogicalVolume *logicDetector;

    //function to construct the detector
    virtual void ConstructSDandField();
};

#endif
