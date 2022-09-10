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
//
/// \file optical/LXe/src/LXeDetectorConstruction.cc
/// \brief Implementation of the LXeDetectorConstruction class
//
//
#include "LXeDetectorConstruction.hh"

#include "construction.hh"

#include "LXeDetectorMessenger.hh"
#include "LXeMainVolume.hh"
#include "LXePMTSD.hh"
#include "LXeScintSD.hh"
#include "LXeWLSSlab.hh"

#include "globals.hh"
#include "G4Box.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4OpticalSurface.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UImanager.hh"
#include "G4VisAttributes.hh"

G4bool LXeDetectorConstruction::fSphereOn = true;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorConstruction::LXeDetectorConstruction()
  : fLXe_mt(nullptr)
  , fMPTPStyrene(nullptr)
{
  fExperimentalHall_box  = nullptr;
  fExperimentalHall_log  = nullptr;
  fExperimentalHall_phys = nullptr;

  fLXe = fAl = fAir = fVacuum = fGlass = nullptr;
  fPstyrene = fPMMA = fPethylene1 = fPethylene2 = nullptr;

  fN = fO = fC = fH = nullptr;

  fSaveThreshold = 0;
  SetDefaults();

  DefineMaterials();
  fDetectorMessenger = new LXeDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorConstruction::~LXeDetectorConstruction()
{
  if(fMainVolume)
  {
    delete fMainVolume;
  }
  delete fLXe_mt;
  delete fDetectorMessenger;
  delete fMPTPStyrene;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::DefineMaterials()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;

  G4int polyPMMA = 1;
  G4int nC_PMMA  = 3 + 2 * polyPMMA;
  G4int nH_PMMA  = 6 + 2 * polyPMMA;

  G4int polyeth = 1;
  G4int nC_eth  = 2 * polyeth;
  G4int nH_eth  = 4 * polyeth;

  //***Elements
  fH = new G4Element("H", "H", z = 1., a = 1.01 * g / mole);
  fC = new G4Element("C", "C", z = 6., a = 12.01 * g / mole);
  fN = new G4Element("N", "N", z = 7., a = 14.01 * g / mole);
  fO = new G4Element("O", "O", z = 8., a = 16.00 * g / mole);

  //***Materials
  // Liquid Xenon
  fLXe = new G4Material("LXe", z = 54., a = 131.29 * g / mole,
                        density = 3.020 * g / cm3);



  // Aluminum
  fAl = new G4Material("Al", z = 13., a = 26.98 * g / mole,
                       density = 2.7 * g / cm3);
  // Vacuum
  fVacuum = new G4Material("Vacuum", z = 1., a = 1.01 * g / mole,
                           density = universe_mean_density, kStateGas,
                           0.1 * kelvin, 1.e-19 * pascal);
  // Air
  fAir = new G4Material("Air", density = 1.29 * mg / cm3, 2);
  fAir->AddElement(fN, 70 * perCent);
  fAir->AddElement(fO, 30 * perCent);
  // Glass
  fGlass = new G4Material("Glass", density = 1.032 * g / cm3, 2);
  fGlass->AddElement(fC, 91.533 * perCent);
  fGlass->AddElement(fH, 8.467 * perCent);
  // Polystyrene
  fPstyrene = new G4Material("Polystyrene", density = 1.03 * g / cm3, 2);
  fPstyrene->AddElement(fC, 8);
  fPstyrene->AddElement(fH, 8);
  // Fiber(PMMA)
  fPMMA = new G4Material("PMMA", density = 1190. * kg / m3, 3);
  fPMMA->AddElement(fH, nH_PMMA);
  fPMMA->AddElement(fC, nC_PMMA);
  fPMMA->AddElement(fO, 2);
  // Cladding(polyethylene)
  fPethylene1 = new G4Material("Pethylene1", density = 1200. * kg / m3, 2);
  fPethylene1->AddElement(fH, nH_eth);
  fPethylene1->AddElement(fC, nC_eth);
  // Double cladding(flourinated polyethylene)
  fPethylene2 = new G4Material("Pethylene2", density = 1400. * kg / m3, 2);
  fPethylene2->AddElement(fH, nH_eth);
  fPethylene2->AddElement(fC, nC_eth);

  //***Material properties tables

  std::vector<G4double> lxe_Energy = { 7.0 * eV, 7.07 * eV, 7.14 * eV };

  std::vector<G4double> lxe_SCINT = { 0.1, 1.0, 0.1 };
  std::vector<G4double> lxe_RIND  = { 1.59, 1.57, 1.54 };
  std::vector<G4double> lxe_ABSL  = { 35. * cm, 35. * cm, 35. * cm };
  fLXe_mt = new G4MaterialPropertiesTable();
  fLXe_mt->AddProperty("SCINTILLATIONCOMPONENT1", lxe_Energy, lxe_SCINT);
  fLXe_mt->AddProperty("SCINTILLATIONCOMPONENT2", lxe_Energy, lxe_SCINT);
  fLXe_mt->AddProperty("RINDEX", lxe_Energy, lxe_RIND);
  fLXe_mt->AddProperty("ABSLENGTH", lxe_Energy, lxe_ABSL);
  fLXe_mt->AddConstProperty("SCINTILLATIONYIELD", 12000. / MeV);
  fLXe_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  fLXe_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns);
  fLXe_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 45. * ns);
  fLXe_mt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  fLXe_mt->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
  fLXe->SetMaterialPropertiesTable(fLXe_mt);

  // Set the Birks Constant for the LXe scintillator
  fLXe->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  std::vector<G4double> glass_AbsLength = { 420. * cm, 420. * cm, 420. * cm };
  G4MaterialPropertiesTable* glass_mt   = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH", lxe_Energy, glass_AbsLength);
  glass_mt->AddProperty("RINDEX", "Fused Silica");
  fGlass->SetMaterialPropertiesTable(glass_mt);

  G4MaterialPropertiesTable* vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", "Air");
  fVacuum->SetMaterialPropertiesTable(vacuum_mt);
  fAir->SetMaterialPropertiesTable(vacuum_mt);  // Give air the same rindex

  std::vector<G4double> wls_Energy = { 2.00 * eV, 2.87 * eV, 2.90 * eV,
                                       3.47 * eV };

  std::vector<G4double> rIndexPstyrene = { 1.5, 1.5, 1.5, 1.5 };
  std::vector<G4double> absorption1    = { 2. * cm, 2. * cm, 2. * cm, 2. * cm };
  std::vector<G4double> scintilFast    = { 0.0, 0.0, 1.0, 1.0 };
  fMPTPStyrene = new G4MaterialPropertiesTable();
  fMPTPStyrene->AddProperty("RINDEX", wls_Energy, rIndexPstyrene);
  fMPTPStyrene->AddProperty("ABSLENGTH", wls_Energy, absorption1);
  fMPTPStyrene->AddProperty("SCINTILLATIONCOMPONENT1", wls_Energy, scintilFast);
  fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
  fMPTPStyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);
  fMPTPStyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10. * ns);
  fPstyrene->SetMaterialPropertiesTable(fMPTPStyrene);

  // Set the Birks Constant for the Polystyrene scintillator
  fPstyrene->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  std::vector<G4double> AbsFiber    = { 9.0 * m, 9.0 * m, 0.1 * mm, 0.1 * mm };
  std::vector<G4double> EmissionFib = { 1.0, 1.0, 0.0, 0.0 };
  G4MaterialPropertiesTable* fiberProperty = new G4MaterialPropertiesTable();
  fiberProperty->AddProperty("RINDEX", "PMMA");
  fiberProperty->AddProperty("WLSABSLENGTH", wls_Energy, AbsFiber);
  fiberProperty->AddProperty("WLSCOMPONENT", wls_Energy, EmissionFib);
  fiberProperty->AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);
  fPMMA->SetMaterialPropertiesTable(fiberProperty);

  std::vector<G4double> RefractiveIndexClad1 = { 1.49, 1.49, 1.49, 1.49 };
  G4MaterialPropertiesTable* clad1Property   = new G4MaterialPropertiesTable();
  clad1Property->AddProperty("RINDEX", wls_Energy, RefractiveIndexClad1);
  clad1Property->AddProperty("ABSLENGTH", wls_Energy, AbsFiber);
  fPethylene1->SetMaterialPropertiesTable(clad1Property);

  std::vector<G4double> RefractiveIndexClad2 = { 1.42, 1.42, 1.42, 1.42 };
  G4MaterialPropertiesTable* clad2Property   = new G4MaterialPropertiesTable();
  clad2Property->AddProperty("RINDEX", wls_Energy, RefractiveIndexClad2);
  clad2Property->AddProperty("ABSLENGTH", wls_Energy, AbsFiber);
  fPethylene2->SetMaterialPropertiesTable(clad2Property);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LXeDetectorConstruction::Construct()
{
  // The experimental hall walls are all 1m away from housing walls
  G4double expHall_x = 1.5*km;
  G4double expHall_y = 1.5*km;
  G4double expHall_z = 10.0*km;

  // Create experimental hall
  fExperimentalHall_box =
    new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  fExperimentalHall_log =
    new G4LogicalVolume(fExperimentalHall_box, fVacuum, "expHall_log", 0, 0, 0);
  fExperimentalHall_phys = new G4PVPlacement(
    0, G4ThreeVector(), fExperimentalHall_log, "expHall", 0, false, 0);

  fExperimentalHall_log->SetVisAttributes(G4VisAttributes::GetInvisible());

  // Place the main volume
  if(fMainVolumeOn)
  {
    fMainVolume = new LXeMainVolume(0, G4ThreeVector(), fExperimentalHall_log,
                                    false, 0, this);
  }

  // Place the WLS slab
  if(fWLSslab)
  {
    G4VPhysicalVolume* slab = new LXeWLSSlab(
      0, G4ThreeVector(0., 0., -fScint_z / 2. - fSlab_z - 1. * cm),
      fExperimentalHall_log, false, 0, this);

    // Surface properties for the WLS slab
    G4OpticalSurface* scintWrap = new G4OpticalSurface("ScintWrap");

    new G4LogicalBorderSurface("ScintWrap", slab, fExperimentalHall_phys,
                               scintWrap);

    scintWrap->SetType(dielectric_metal);
    scintWrap->SetFinish(polished);
    scintWrap->SetModel(glisur);

    std::vector<G4double> pp           = { 2.0 * eV, 3.5 * eV };
    std::vector<G4double> reflectivity = { 1.0, 1.0 };
    std::vector<G4double> efficiency   = { 0.0, 0.0 };

    G4MaterialPropertiesTable* scintWrapProperty =
      new G4MaterialPropertiesTable();

    scintWrapProperty->AddProperty("REFLECTIVITY", pp, reflectivity);
    scintWrapProperty->AddProperty("EFFICIENCY", pp, efficiency);
    scintWrap->SetMaterialPropertiesTable(scintWrapProperty);
  }

  //return fExperimentalHall_phys;

  G4double worldSize_x = 20*m;
G4double worldSize_y = 20*m;
G4double worldSize_z = 40*m;

G4double tankRadius = 8*m;
G4double tankHeight = 5*m;
G4double tankPos_z = -30*m;

G4double wtyvek = 1*mm;
G4double wal = 1*cm;

//MATERIALS///////////////////////////////////////////////////////////
//class that contains properties of materials
G4NistManager *nist = G4NistManager::Instance();

//ELEMENTS
G4Element *H = new G4Element("H", "H", 1., 1.01*g/mole);
G4Element *C = new G4Element("C", "C", 6., 12.01*g/mole);

//Air
//class to find and define materials
//we include the predefined material air
//G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
G4Material *worldMat = new G4Material("Vacuum", 1., 1.01*g/mole,
    universe_mean_density, kStateGas, 0.1*kelvin, 1.e-19*pascal);

//Water
//G4Material *water = nist->FindOrBuildMaterial("G4_WATER");

G4Material *water = new G4Material("water", 1.000*g/cm3, 2);
water->AddElement(nist->FindOrBuildElement("H"), 2);
water->AddElement(nist->FindOrBuildElement("O"), 1);

//Al
G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

//Tyvek
G4Material *Tyvek = new G4Material("Tyvek", 0.935*g/cm3,2);
Tyvek->AddElement(C,2);
Tyvek->AddElement(H,4);

//Properties
G4double energy[2] = {1.2398419398*eV/0.9, 1.2398419398*eV/0.2};
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
/*
G4CutTubs *waterSolid = new G4CutTubs("waterSolid", 5*m,
    tankRadius, tankHeight, 0., 360.*deg, G4ThreeVector(0,0,0),
    G4ThreeVector(0,0,0));
*/
G4Tubs *waterSolid = new G4Tubs("waterSolid", 0.,
    tankRadius, tankHeight, 0., 360.*deg);

G4LogicalVolume *waterLogic = new G4LogicalVolume(waterSolid, water,
    "waterLogic");

G4VPhysicalVolume *waterPhys = new G4PVPlacement(0,
    G4ThreeVector(0., 0., tankPos_z),
    waterLogic, "waterPhys", logicWorld, false, 0, true);

//TYVEK///////////////////////////////////////////////////////////
//Base of cylinder
G4Tubs *tBaseSolid = new G4Tubs("tBaseSolid", 0.,
    tankRadius + wtyvek, wtyvek, 0., 360.*deg);

G4LogicalVolume *tBaseLogic = new G4LogicalVolume(tBaseSolid, Tyvek,
    "tBaseLogic");

G4VPhysicalVolume *tBasePhys = new G4PVPlacement(0,
    G4ThreeVector(0., 0., tankPos_z - tankHeight - wtyvek/2),
    tBaseLogic, "tBasePhys", logicWorld, false, 0, true);

//Cylinder walls
G4Tubs *tCylSolid = new G4Tubs("tCylSolid", tankRadius,
    tankRadius + wtyvek, tankHeight + wtyvek + 1*m, 0., 360.*deg);

G4LogicalVolume *tCylLogic = new G4LogicalVolume(tCylSolid, Tyvek,
    "tCylLogic");

G4VPhysicalVolume *tCylPhys = new G4PVPlacement(0,
    G4ThreeVector(0., 0., tankPos_z - wtyvek/2 + 1*m),
    tCylLogic, "tCylPhys", logicWorld, false, 0, true);

//Al///////////////////////////////////////////////////////////////
//Bases of cylinder
G4Tubs *alBaseSolid = new G4Tubs("alBaseSolid", 0.,
    tankRadius + wtyvek + wal, wal, 0., 360.*deg);

G4LogicalVolume *alBaseLogic = new G4LogicalVolume(alBaseSolid, Al,
    "alBaseLogic");

G4VPhysicalVolume *alBasePhys1 = new G4PVPlacement(0,
    G4ThreeVector(0., 0., tankPos_z - tankHeight - wtyvek - wal/2),
    alBaseLogic, "alBasePhys2", logicWorld, false, 0, true);

//Cylinder walls
G4Tubs *alCylSolid = new G4Tubs("tCylSolid", tankRadius + wtyvek,
    tankRadius + wtyvek + wal, tankHeight + wtyvek + wal + 1*m,
    0., 360.*deg);

G4LogicalVolume *alCylLogic = new G4LogicalVolume(alCylSolid, Al,
    "alCylLogic");

G4VPhysicalVolume *alCylPhys = new G4PVPlacement(0,
    G4ThreeVector(0., 0., tankPos_z - wtyvek - wal/2 + 1*m),
    alCylLogic, "alCylPhys", logicWorld, false, 0, true);

//PMTs/////////////////////////////////////////////////////////////
//Detector de prueba
/*G4Box *solidDetector = new G4Box("solidDetector",
    0.5*m, 0.5*m, 0.5*m);

//the sensitive volume we later define has to be able
//to refer to logical volume outside this function,
//so we add the function in the header file
logicDetector = new G4LogicalVolume(solidDetector, worldMat,
    "logicDetector");

G4VPhysicalVolume *physDetector = new G4PVPlacement(0,
    G4ThreeVector(2*m, 2*m, 2*m),
    logicDetector, "physDetector", logicWorld, false, 0, true);
*/
//VIS/////////////////////////////////////////////////////////////
//Visualizacion del agua
G4VisAttributes *water_va= new G4VisAttributes(G4Colour::Blue());
water_va->SetForceAuxEdgeVisible (true);
water_va->SetForceWireframe(true);
water_va->SetForceSolid(false);
water_va->SetVisibility(true);
waterLogic->SetVisAttributes(water_va);

//Tyvek
G4VisAttributes *tyvek_va= new G4VisAttributes(G4Colour::Red());
tyvek_va->SetForceAuxEdgeVisible (true);
tyvek_va->SetForceWireframe(true);
tyvek_va->SetForceSolid(false);
tyvek_va->SetVisibility(true);
//tBaseLogic->SetVisAttributes(tyvek_va);
//tCylLogic->SetVisAttributes(tyvek_va);

//Al
G4VisAttributes *al_va= new G4VisAttributes(G4Colour::Yellow());
al_va->SetForceAuxEdgeVisible (true);
al_va->SetForceWireframe(true);
al_va->SetForceSolid(false);
al_va->SetVisibility(true);
alBaseLogic->SetVisAttributes(al_va);
alCylLogic->SetVisAttributes(al_va);

//program should return the physical volume
return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::ConstructSDandField()
{
  if(!fMainVolume)
    return;

  // PMT SD

  LXePMTSD* pmt = fPmt_SD.Get();
  if(!pmt)
  {
    // Created here so it exists as pmts are being placed
    G4cout << "Construction /LXeDet/pmtSD" << G4endl;
    LXePMTSD* pmt_SD = new LXePMTSD("/LXeDet/pmtSD");
    fPmt_SD.Put(pmt_SD);

    pmt_SD->InitPMTs();
    pmt_SD->SetPmtPositions(fMainVolume->GetPmtPositions());
  }
  else
  {
    pmt->InitPMTs();
    pmt->SetPmtPositions(fMainVolume->GetPmtPositions());
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fPmt_SD.Get());
  // sensitive detector is not actually on the photocathode.
  // processHits gets done manually by the stepping action.
  // It is used to detect when photons hit and get absorbed & detected at the
  // boundary to the photocathode (which doesn't get done by attaching it to a
  // logical volume.
  // It does however need to be attached to something or else it doesn't get
  // reset at the begining of events

  SetSensitiveDetector(fMainVolume->GetLogPhotoCath(), fPmt_SD.Get());

  // Scint SD

  if(!fScint_SD.Get())
  {
    G4cout << "Construction /LXeDet/scintSD" << G4endl;
    LXeScintSD* scint_SD = new LXeScintSD("/LXeDet/scintSD");
    fScint_SD.Put(scint_SD);
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fScint_SD.Get());
  SetSensitiveDetector(fMainVolume->GetLogScint(), fScint_SD.Get());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetDimensions(G4ThreeVector dims)
{
////////////////////////////////
//  scint_x  = dims[0];
//  scint_y  = dims[1];
//  scint_z  = dims[2];
/////////////////////////////////
  fScint_x = dims[0];
  fScint_y = dims[1];
  fScint_z = dims[2];
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetHousingThickness(G4double d_mtl)
{
  fD_mtl = d_mtl;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetNX(G4int nx)
{
  fNx = nx;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetNY(G4int ny)
{
  fNy = ny;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetNZ(G4int nz)
{
  fNz = nz;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetPMTRadius(G4double outerRadius_pmt)
{
  fOuterRadius_pmt = outerRadius_pmt;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetDefaults()
{
  // Resets to default values
  fD_mtl = 0.0635 * cm;

  fScint_x = 1.78 * m;
  fScint_y = 1.78 * m;
  fScint_z = 1.2 *  m;
///////////////////////////////////////////////
  //scint_x = 1.78*m; // RADIO tanque AUGER CHARM
  //scint_y = 1.78*m; //
//***********************radio del tanque
  //scint_x = 3.65*m;  //VAMOS (RADIO=D/2)
  //scint_y = 3.65*m;
//*********************altura del tanque***************************
   //scint_z = 1.2*m; //AUGER CHARM
// scint_z = 5.0*m; //VAMOS

///////////////////////////////////////////////

  fNx = 1;
  fNy = 0;
  fNz = 0;

  fOuterRadius_pmt = 2.3 * cm;

  fSphereOn = true;
  fRefl     = 1.0;

  fNfibers      = 1;
  fWLSslab      = false;
  fMainVolumeOn = true;
  fMainVolume   = nullptr;
  fSlab_z       = 2.5 * mm;

  G4UImanager::GetUIpointer()->ApplyCommand(
    "/LXe/detector/scintYieldFactor 1.");

  if(fLXe_mt)
    fLXe_mt->AddConstProperty("SCINTILLATIONYIELD", 12000. / MeV);
  if(fMPTPStyrene)
    fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetSphereOn(G4bool b)
{
  fSphereOn = b;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetHousingReflectivity(G4double r)
{
  fRefl = r;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetWLSSlabOn(G4bool b)
{
  fWLSslab = b;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetMainVolumeOn(G4bool b)
{
  fMainVolumeOn = b;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetNFibers(G4int n)
{
  fNfibers = n;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetMainScintYield(G4double y)
{
  fLXe_mt->AddConstProperty("SCINTILLATIONYIELD", y / MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetWLSScintYield(G4double y)
{
  fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD", y / MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorConstruction::SetSaveThreshold(G4int save)
{
  // Sets the save threshold for the random number seed. If the number of
  // photons generated in an event is lower than this, then save the seed for
  // this event in a file called run###evt###.rndm

  fSaveThreshold = save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
