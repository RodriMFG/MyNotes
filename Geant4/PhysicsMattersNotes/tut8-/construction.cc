#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    //G4Material function takes the name, density, number of components
    /*AddElement function takes the element,
    number of components and adds it to our material*/
    G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

    //now we create water using the same method
    G4Material *H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    //in the case of carbon we just need the element
    G4Element *C = nist->FindOrBuildElement("C");

    //now we can add our previously created materials and elements
    //function takes name, density, and number of components
    G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    //add the components
    Aerogel->AddMaterial(SiO2, 62.5*perCent);
    Aerogel->AddMaterial(H2O, 37.4*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    //array with refractive index for red light and blue light
    G4double energy[2] = {1.2398419398*eV/0.9, 1.2398419398*eV/0.2};
    G4double rIndexAerogel[2] = {1.1, 1.1};
    G4double rIndexWorld[2] = {1.0, 1.0};

    /*AddProperty function takes name, energy, rIndex,
    number of elements of array*/
    G4MaterialPropertiesTable *mptAerogel = new
G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rIndexAerogel, 2);

    //add the properties to the Aerogel
    Aerogel->SetMaterialPropertiesTable(mptAerogel);


    //world creation ....................................................
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //create optical properties to the world material (air)
    G4MaterialPropertiesTable *mptWorld = new
        G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rIndexWorld, 2);

    //add optical material properties to our world
    worldMat->SetMaterialPropertiesTable(mptWorld);

    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,
worldMat, "logicWorld");

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
G4ThreeVector(0., 0., 0.),
logicWorld, "physWorld", 0, false, 0, true);
    //end of world creation ...........................................


    //radiator creation ..............................................

    //we create the radiator based on a G4Box class
    //we define it's dimensions
    //(smaller or eq than the world volume) and its name
    G4Box *solidRadiator = new G4Box("solidRadiator",
0.4*m, 0.4*m, 0.01*m);

    //we define its properties
    //solid volume, material, name
    G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator,
Aerogel, "logicRadiator");

    //we place said volume
    //rotation, placement, name, logical volume, mother volume,
    //no boolean, copies, check for overlaps
    G4VPhysicalVolume *physRadiator = new G4PVPlacement(0,
G4ThreeVector(0., 0., 0.25*m), logicRadiator,
"physRadiator", logicWorld, false, 0, true);
    //end of radiator creation ......................................


    //photosensors construction......................................

    //we create the radiator based on a G4Box class
    //we define its name and dimensions
    //(smaller or eq than the world volume)
    G4Box *solidDetector = new G4Box("solidDetector",
0.005*m, 0.005*m, 0.01*m);

    //the sensitive volume we later define has to be able
    //to refer to logical volume outside this function,
    //so we add the function in the header file

    //we define its properties
    //solid volume, material, name
    logicDetector = new G4LogicalVolume(solidDetector, worldMat,
        "logicDetector");

    //we now create the instances of the detector
    for (G4int i = 0; i < 100; i++)
    {
        for(G4int j = 0; j < 100; j++)
        {
        //we place each of the photosensors in consecutive positions
        //rotation, placement, name, logical volume, mother volume,
        //no boolean, copies, check for overlaps
            G4VPhysicalVolume *physDetector = new G4PVPlacement(0,
G4ThreeVector(-0.5*m+(i+0.5)*m/100, -0.5*m+(j+0.5)*m/100, 0.48*m),
logicDetector, "physDetector", logicWorld, false, j+i*100, true);
//if i is 0, j counts from 0 to 99, and once we go to the next
//i it starts from 100, moving onto 199, etc.
        }
    }

    //end of photosensors construction ............................

    return physWorld;
}

//sensitive detector creation .................................

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new
MySensitiveDetector("SensitiveDetector");

    logicDetector->SetSensitiveDetector(sensDet);
}

//end of sensitive detector creation .........................
