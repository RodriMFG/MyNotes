//Functions to construct the detector

//include appropiate header file
#include "construction.hh"

//construct while in scope
MyDetectorConstruction::MyDetectorConstruction()
{}

//deconstruct once function goes out of scope
MyDetectorConstruction::~MyDetectorConstruction()
{}

//function to define detector geometry and materials
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    //class that contains properties of materials
    G4NistManager *nist = G4NistManager::Instance();

    //class to find and define materials
    //we include the predefined material air
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //all volumes in G4 require the solid that defines its size
    //the logical volume defines the material properties
    //the physical volumes places the object in the world and
    //can perform rotations and copies of said volume

    //this class defines a box of name, x half-size, y, z
    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);

    //class defines the logical volume of
    //volume it is based on, material, name
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat,
         "logicWorld");

    //class to define the physical volume
    //the function places the volume, it takes
    //rotation, where the volume is centered around,
    //logical volume it is defined on,
    //name, mother volume (if volume is placed inside another one),
    //boolean operations, copy number, check for overlaps
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
        G4ThreeVector(0., 0., 0.), logicWorld, "physWorld",
         0, false, 0, true);

    //program should return the physical volume which includes
    //all its properties
    return physWorld;
}
