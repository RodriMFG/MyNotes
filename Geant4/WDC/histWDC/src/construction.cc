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
    G4double worldSize_x = 20*m;
    G4double worldSize_y = 20*m;
    G4double worldSize_z = 20*m;

    G4double tankRadius = 3*m;
    G4double tankHeight = 3*m;
    G4double tankPos_z = -10*m;

    G4double wtyvek = 1*mm;
    G4double wal = 1*cm;
    G4double detThick = 1*cm;

    //MATERIALS///////////////////////////////////////////////////////////
    //class that contains properties of materials
    G4NistManager *nist = G4NistManager::Instance();

    //ELEMENTS
    G4Element *H = new G4Element("H", "H", 1., 1.01*g/mole);
    G4Element *C = new G4Element("C", "C", 6., 12.01*g/mole);
    G4Element *O = new G4Element("O", "O", 8., 16.00*g/mole);
    G4Element *A = new G4Element("Al", "Al", 13., 26.89*g/mole);

    //Air
    //class to find and define materials
    //we include the predefined material air
    //G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *worldMat = new G4Material("Vacuum", 1., 1.01*g/mole,
        universe_mean_density, kStateGas, 0.1*kelvin, 1.e-19*pascal);

    //Water
    //G4Material *water = nist->FindOrBuildMaterial("G4_WATER");
    G4Material *water = new G4Material("water", 1.00*g/cm3, 2);
    water->AddElement(H, 2);
    water->AddElement(O, 1);

    //Al
    G4Material *Al = new G4Material("Al", 2.70*g/cm3, 1);
    Al->AddElement(A, 1);

    //Tyvek
    G4Material *Tyvek = new G4Material("Tyvek", 0.94*g/cm3, 2);
    Tyvek->AddElement(C,2);
    Tyvek->AddElement(H,4);

    //Pp
    G4Material *Pp = new G4Material("Pp", 0.95*g/cm3, 2);
    Pp->AddElement(C, 1);
    Pp->AddElement(H, 3);

    //Pen
    G4Material *Pen = new G4Material("Pen", 1.38*g/cm3, 3);
    Pen->AddElement(C, 14);
    Pen->AddElement(H, 10);
    Pen->AddElement(O, 4);

    //Properties
    //From PhysicsMaters
    //G4double energy[2] = {1.2398419398*eV/0.9, 1.2398419398*eV/0.2};
    G4double energy[2] = {1.9074491*eV, 4.9593677*eV};
    G4double rIndexWorld[2] = {1.0, 1.0};
    G4double rIndexWater[2] = {1.3, 1.3};

    //Air
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rIndexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    //Water
    G4MaterialPropertiesTable *mptWater = new G4MaterialPropertiesTable();
    mptWater->AddProperty("RINDEX", energy, rIndexWater, 2);
    water->SetMaterialPropertiesTable(mptWater);

    //Tyvek
    G4double tyvekReflectivity[2] = {0.92, 0.63};
    G4MaterialPropertiesTable *mptTyvek = new G4MaterialPropertiesTable();
    mptTyvek->AddProperty("REFLECTIVITY", energy, tyvekReflectivity, 2);
    //Tyvek's optical surface
    G4OpticalSurface *tyvekSurface = new G4OpticalSurface("tyvekSurface",
        unified, polishedtyvekair, dielectric_metal);
    tyvekSurface->SetMaterialPropertiesTable(mptTyvek);
    Tyvek->SetMaterialPropertiesTable(mptTyvek);


    //VOLUMES//////////////////////////////////////////////////////////////
    //WORLD////////////////////////////////////////////////////////////////
    //this class defines a box of name, x half-size, y, z
    G4Box *solidWorld = new G4Box("solidWorld",
        worldSize_x, worldSize_y, worldSize_z);

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

    //TANK//////////////////////////////////////////////////////////////
    //construccion del tanque
    G4Tubs *waterSolid = new G4Tubs("waterSolid", 0.,
        tankRadius, tankHeight, 0., 360.*deg);

    G4LogicalVolume *waterLogic = new G4LogicalVolume(waterSolid, water,
        "waterLogic");

    G4VPhysicalVolume *waterPhys = new G4PVPlacement(0,
        G4ThreeVector(0., 0., tankPos_z),
        waterLogic, "waterPhys", logicWorld, false, 0, true);

    //TYVEK///////////////////////////////////////////////////////////
    //Cylinder walls
    G4Tubs *tCylSolid = new G4Tubs("tCylSolid", tankRadius,
        tankRadius + wtyvek, tankHeight + 0.5*m, 0., 360.*deg);

    G4LogicalVolume *tCylLogic = new G4LogicalVolume(tCylSolid, Tyvek,
        "tCylLogic");
    new G4LogicalSkinSurface("tCyl_surface", tCylLogic, tyvekSurface);

    G4VPhysicalVolume *tCylPhys = new G4PVPlacement(0,
        G4ThreeVector(0., 0., tankPos_z - wtyvek + 0.5*m),
        tCylLogic, "tCylPhys", logicWorld, false, 0, true);

    //Al///////////////////////////////////////////////////////////////
    //Cylinder walls
    G4Tubs *alCylSolid = new G4Tubs("tCylSolid", tankRadius + wtyvek,
        tankRadius + wtyvek + wal, tankHeight + 0.5*m,
        0., 360.*deg);

    G4LogicalVolume *alCylLogic = new G4LogicalVolume(alCylSolid, Al,
        "alCylLogic");

    G4VPhysicalVolume *alCylPhys = new G4PVPlacement(0,
        G4ThreeVector(0., 0., tankPos_z + 0.5*m),
        alCylLogic, "alCylPhys", logicWorld, false, 0, true);

    //PMTs/////////////////////////////////////////////////////////////
    //Detector de prueba
    //the sensitive volume we later define has to be able
    //to refer to logical volume outside this function,
    //so we add the function in the header file

    G4int n = 100;
    G4double detSize = 2*tankRadius/n;

    G4Box *solidDetector = new G4Box("solidDetector",
        detSize/2, detSize/2, detThick/2);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat,
        "logicDetector");

    for (G4int j = 0; j < n+1; j++)
    {
        for (G4int i = 0; i < n+1; i++)
        {
            G4VPhysicalVolume *physDetector = new G4PVPlacement(0,
                G4ThreeVector(-tankRadius + i*detSize,
                    -tankRadius + j*detSize,
                    tankPos_z - tankHeight - 2*detThick),
                logicDetector, "physDetector", logicWorld, false, j*m + i, true);
        }
    }

    //VIS/////////////////////////////////////////////////////////////
    //Visualizacion del agua
    G4VisAttributes *water_va= new G4VisAttributes(G4Colour::Blue());
    water_va->SetForceAuxEdgeVisible(true);
    water_va->SetForceWireframe(true);
    water_va->SetForceSolid(false);
    water_va->SetVisibility(true);
    waterLogic->SetVisAttributes(water_va);

    //Al
    G4VisAttributes *al_va= new G4VisAttributes(G4Colour::Yellow());
    al_va->SetForceAuxEdgeVisible(true);
    al_va->SetForceWireframe(true);
    al_va->SetForceSolid(false);
    al_va->SetVisibility(true);
    alCylLogic->SetVisAttributes(al_va);

    //Det
    G4VisAttributes *det_va= new G4VisAttributes(G4Colour::Red());
    det_va->SetForceAuxEdgeVisible(true);
    det_va->SetForceWireframe(true);
    det_va->SetForceSolid(false);
    det_va->SetVisibility(true);
    logicDetector->SetVisAttributes(det_va);

    //program should return the physical volume
    return physWorld;
}

//DETECTOR////////////////////////////////////////////////
void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new
        MySensitiveDetector("SensitiveDetector");

    logicDetector->SetSensitiveDetector(sensDet);
}
