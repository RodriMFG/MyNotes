//Functions to construct the detector

//include appropiate header file
#include "construction.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"
#include "G4VSensitiveDetector.hh"

#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4Cons.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"


//construct while in scope
MyDetectorConstruction::MyDetectorConstruction()
{}

//deconstruct once function goes out of scope
MyDetectorConstruction::~MyDetectorConstruction()
{}

//function to define detector geometry and materials
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4double fExpHall_x = 100.0*m;
    G4double fExpHall_y =  100.0*m;
    G4double fExpHall_z = 20.0*m;
    G4double fTank_x = 5.0*m;
    G4double fTank_y = 5.0*m;
    G4double fTank_z =  5.0*m;
    G4double fBubble_x = 0.5*m;
    G4double fBubble_y = 0.5*m;
    G4double fBubble_z = 0.5*m;

    G4double scint_x = 7.3*m;
    G4double scint_y = 7.3*m;    //Diámetro
    G4double scint_z = 1.0*m;             //Altura
    G4double Scint_z = 4.2*m;
    G4double outerRadius_pmtL = 10.3*cm;    //Radio de los fototubos laterales 8 pulgadas

    //Desplazamiento de posición del tanque tank 2 respecto del tanque 1
    G4double move_x2 = 24.74*m;	//Desplazamiento en x
    G4double move_y2 = 0.0*m;  //Desplazamiento en y

    //Desplazamiento de posición del tanque tank 3 respecto del tanque 1
    G4double move_x3 = 17.56*m;	//Desplazamiento en x
    G4double move_y3 = -21.64*m;  //Desplazamiento en y

    //desplazamiento tanque central
    G4double move_x4 = 14.10*m;
    G4double move_y4 = -7.22*m;

    G4double wtyvek=0.2*mm;   //grosor tyvek=200 micras
    G4double d_mtl=0.635*cm; //width aluminum cylinder

    //Scientillator plastic
    G4double hcal= 0.15*m;
    //////////////////////////////////////////////////////////

    G4double a;  // atomic mass
    G4double z;  // atomic number
    G4double density;
    G4int nelements;
    std::vector<G4String> elements;
    std::vector<G4int> natoms;

    //class that contains properties of materials
    G4NistManager *nist = G4NistManager::Instance();

    G4Element *H = new G4Element("H", "H", z=1., a=1.01*g/mole);
    G4Element *C = new G4Element("C", "C", z=6., a=12.01*g/mole);
    G4Element *N = new G4Element("N", "N", z=7., a= 14.01*g/mole);
    G4Element *O = new G4Element("O"  , "O", z=8., a= 16.00*g/mole);

    G4Material *Al = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);


    G4Material *Vacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
                            density=universe_mean_density,kStateGas,0.1*kelvin,
                            1.e-19*pascal);
    //
    G4Material *water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
    water->AddElement(H, 2);
    water->AddElement(O, 1);

    G4Material *Tyvek = new G4Material("Tyvek",density=0.935*g/cm3,2);
    Tyvek->AddElement(C,2);
    Tyvek->AddElement(H,4);

    //Material properties missing!!!

    //class to find and define materials
    //we include the predefined material air
    //G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //all volumes in G4 require the solid that defines its size
    //the logical volume defines the material properties
    //the physical volumes places the object in the world and
    //can perform rotations and copies of said volume

    //this class defines a box of name, x half-size, y, z
    //G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);
    G4Box *expHall_box = new G4Box("World",fExpHall_x,fExpHall_y,fExpHall_z);


    //class defines the logical volume of
    //volume it is based on, material, name
    //G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat,
    //     "logicWorld");
    G4LogicalVolume *expHall_log
= new G4LogicalVolume(expHall_box, Vacuum, "World",0,0,0);

    //class to define the physical volume
    //the function places the volume, it takes
    //rotation, where the volume is centered around,
    //logical volume it is defined on,
    //name, mother volume (if volume is placed inside another one),
    //boolean operations, copy number, check for overlaps
    //G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
    //    G4ThreeVector(0., 0., 0.), logicWorld, "physWorld",
    //     0, false, 0, true);
    G4VPhysicalVolume *expHall_phys
= new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);


    G4Tubs* waterTank_box_c = new  G4Tubs("Tank_c",		//Nombre
                                           0., 		//radio interno
                                           scint_x/2.,	//radio externo
                                           scint_z/2.,	//mitad de longitud en z
                                           0.*deg, 	//comienzo en phi
                                           360.*deg);	//segmento del ángulo

    G4LogicalVolume *waterTank_log_c
= new G4LogicalVolume(waterTank_box_c, water,"Tank_c",0,0,0);

    G4VPhysicalVolume* waterTank_phys4
= new G4PVPlacement(0,G4ThreeVector(0.0*m,0.0*m,0.0*m),waterTank_log_c,"Tank4",
                      expHall_log,false,1);

    G4Tubs* tivek_cylinder_c = new G4Tubs("tivek_cylinder_c",
                                                scint_x/2.,
                                                scint_x/2.+ wtyvek,
                                                scint_z/2, //hcal/2
                                                0.*deg,
                                                360.*deg);//cilindro completo

    G4LogicalVolume* tyvek_log_c = new G4LogicalVolume(tivek_cylinder_c,
            G4Material::GetMaterial("Tyvek"),"tivek_log",0,0,0);

    G4VPhysicalVolume* tyvek_phys4 = new G4PVPlacement(0,                                   //sin rotación
            G4ThreeVector(0.0*m,0.0*m),//en (x, y, z) // -hcal/2 con centellador
            tyvek_log_c,                           //su volúmen lógico
            "tivek_cylinder4",                             //su nombre
            expHall_log,                         //su volúmen madre
            false,                               //sin operaciones booleanas
            4);
    //
    G4Tubs* tyvektop_c = new G4Tubs("tyvektop_c",
        0, scint_x/2.+wtyvek, wtyvek/2., 0.*deg, 360.*deg);//tapa

    G4LogicalVolume* tyvektop_log_c = new G4LogicalVolume(tyvektop_c,
        G4Material::GetMaterial("Tyvek"),"tyvektop_log_c",0,0,0);

    G4VPhysicalVolume* tyvektop_phys4 =  new G4PVPlacement(0,
                                  G4ThreeVector(0.0*m,0.0*m,scint_z/2.+wtyvek/2.),
                                  tyvektop_log_c,
                                  "tyvektop4",
                                  expHall_log,
                                  false,
                                  4);
    //
    G4Tubs* tyvekbottom_c = new G4Tubs("tyvekbottom_c",
        0., scint_x/2. + wtyvek, wtyvek/2., 0.*deg, 360.*deg);//base

    G4LogicalVolume* tyvekbottom_log_c = new G4LogicalVolume(tyvekbottom_c,
        G4Material::GetMaterial("Tyvek"),"tyvekbottom_log_c",0,0,0);

    G4VPhysicalVolume* tyvekbottom_phys4 = new G4PVPlacement(0,
        G4ThreeVector(0.0*m,0.0*m,-scint_z/2.-wtyvek/2. ),//- hcal con centellador
        tyvekbottom_log_c,
        "tyvekbottom1_c",
        expHall_log,
        false,
        0);
    //
    G4Box* tyvekwallshortx1 = new G4Box("tyvekwallx", scint_x/2., wtyvek/2., scint_z/2.);

    G4LogicalVolume* tyvekwallshortx1_log  = new G4LogicalVolume(tyvekwallshortx1,
        G4Material::GetMaterial("Tyvek"), "tyvekwallshortx1_log",0,0,0);

    G4VPhysicalVolume* tyvekwallshortx1_phys = new G4PVPlacement(0,				//Rotación del mother frame
                                           G4ThreeVector(0, 0, 0),	//Posición en el frame rotado
                                           tyvekwallshortx1_log,		//Nombre del volúmen lógico hija
                                           "tyvekwallshortx1",		//Nombre
                                           waterTank_log_c,			//Nombre lógico del volúmen madre
                                           false,
                                           0);


    //*********************************************************************************
    //TyvekWallshort y1
    //*********************************************************************************
    G4Box* tyvekwallshorty1 = new G4Box("tyvekwally1", wtyvek/2., scint_x/4.-wtyvek/4, scint_z/2.);

    G4LogicalVolume* tyvekwallshorty1_log  = new G4LogicalVolume(tyvekwallshorty1,
        G4Material::GetMaterial("Tyvek"), "tyvekwallshorty1_log",0,0,0);

    //First y wall
    //
    G4VPhysicalVolume* tyvekwallshorty1_phys = new G4PVPlacement(0,				//Rotación del mother frame
                                           G4ThreeVector(0, -scint_y/4.-wtyvek/2, 0),	//Posición en el frame rotado
                                           tyvekwallshorty1_log,		//Nombre del volúmen lógico hija
                                           "tyvekwallshorty1",		//Nombre
                                           waterTank_log_c,			//Nombre lógico del volúmen madre
                                           false,
                                           0);


    //Second y wall
    //
    G4VPhysicalVolume* tyvekwallshorty2_phys = new G4PVPlacement(0,				//Rotación del mother frame
                                           G4ThreeVector(0, scint_y/4.+wtyvek/2, 0),	//Posición en el frame rotado
                                           tyvekwallshorty1_log,		//Nombre del volúmen lógico hija
                                           "tyvekwallshorty2",		//Nombre
                                          waterTank_log_c,			//Nombre lógico del volúmen madre
                                           false,
                                           1);

    //
    G4double sphere_w = 0.5*mm;	//sphere_width
    G4double cone_w = 0.0*mm;	//cone_width
    G4double long_cone = 30*cm;//long cone
    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateY(180*deg);
    G4double sshift=outerRadius_pmtL*0.5;

    G4Sphere* photocathL = new G4Sphere("photocathL",outerRadius_pmtL-sphere_w,outerRadius_pmtL,0.*deg,360.*deg,0.*deg,60.*deg);

    G4LogicalVolume *photocath_logL= new G4LogicalVolume(photocathL,
                                            G4Material::GetMaterial("Al"),
                                            "photocath_logL");
    //*********************************************************************************
    //PMTs first tank
    //*********************************************************************************

    G4VPhysicalVolume* photocatht1_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm,203.0*cm,scint_z/2.+sshift),
                                                                                  photocath_logL,"photocatht1",
                                                                                  waterTank_log_c ,false,1);

    G4VPhysicalVolume* photocatht2_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm,203.0*cm,scint_z/2.+sshift),
                                                                                  photocath_logL,"photocatht2",
                                                                                  waterTank_log_c ,false,2);

    G4VPhysicalVolume* photocatht3_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm,-203.0*cm,scint_z/2.+sshift),
                                                                                  photocath_logL,"photocatht3",
                                                                                  waterTank_log_c,false,3);

    G4VPhysicalVolume* photocatht4_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm,-203.0*cm,scint_z/2.+sshift),
                                                                                  photocath_logL,"photocatht4",
                                                                                  waterTank_log_c,false,4);
    //*********************************************************************************

    G4Tubs* AlCylinder_c = new G4Tubs("AlCylinder_c",
                            scint_x/2.+wtyvek,
                            scint_x/2.+wtyvek + d_mtl,
                            scint_z/2., //+ hcal/2 con sentellador
                            0.*deg,
                            360.*deg);//base
    G4LogicalVolume* AlCylinder_log_c = new G4LogicalVolume(AlCylinder_c,G4Material::GetMaterial("Al"),"AlCylinder_log_c",0,0,0);
    G4VPhysicalVolume* AlCylider_phys4 = new G4PVPlacement(0,
                                             G4ThreeVector(0,0,0), //-hcal/2 con centellador
                                             AlCylinder_log_c,
                                             "AlCylinder4",
                                             expHall_log,
                                             false,
                                             4);
    //
    ///VISUALIZATION ATTRIBUTES

    G4VisAttributes* alum= new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)) ;  // cyan
    AlCylinder_log_c -> SetVisAttributes(alum);
    alum -> SetForceAuxEdgeVisible (true);//visualización del cilindro
    alum -> SetForceWireframe(true);
    alum -> SetForceSolid(false);
    alum -> SetVisibility(true);

    //central
    G4VisAttributes* water_va_c= new G4VisAttributes(G4Colour::Blue()) ;  // cyan G4Colour(0.0, 1.0, 1.0)
    waterTank_log_c -> SetVisAttributes(water_va_c);
    water_va_c -> SetForceAuxEdgeVisible (true);//visualización del cilindro
    water_va_c -> SetForceWireframe(true);
    water_va_c -> SetForceSolid(false);
    water_va_c -> SetVisibility(true);

    //always return the physical World


    G4VisAttributes* photocath_vaL= new G4VisAttributes(G4Colour::Yellow()) ;  // cyan
    photocath_logL -> SetVisAttributes(photocath_vaL);
    photocath_vaL-> SetForceSolid(true);
    photocath_vaL-> SetVisibility(true);


    G4VisAttributes* tyvek_blue= new G4VisAttributes(G4Colour::Green()) ;  // cyan G4Colour(0.0, 1.0, 1.0)
    tyvek_log_c -> SetVisAttributes(tyvek_blue);
    tyvek_blue-> SetForceSolid(true);
    tyvek_blue-> SetVisibility(true);

    /*
    G4VisAttributes* Scitillator= new G4VisAttributes(G4Colour::Red());  // cyan
    fHadCalScintiLogical -> SetVisAttributes(Scitillator);
    Scitillator-> SetForceAuxEdgeVisible (true);//visualización de pared
    Scitillator-> SetForceWireframe(true);
    //Scitillator-> SetForceSolid(true);
    Scitillator-> SetVisibility(true);*/



    G4VisAttributes* bottom = new G4VisAttributes(G4Colour::Yellow());  // cyan
    tyvekbottom_log_c  -> SetVisAttributes(bottom);
    bottom-> SetForceAuxEdgeVisible (true);//visualización de pared
    bottom-> SetForceWireframe(true);
    //Scitillator-> SetForceSolid(true);
    bottom-> SetVisibility(true);


    G4VisAttributes* top = new G4VisAttributes(G4Colour::Yellow());  // cyan
    tyvektop_log_c  -> SetVisAttributes(top);
    top-> SetForceAuxEdgeVisible (true);//visualización de pared
    top-> SetForceWireframe(true);
    //Scitillator-> SetForceSolid(true);
    top-> SetVisibility(true);


    G4VisAttributes* tyvek_shortx1_gray= new G4VisAttributes(G4Colour::White()) ;  // cyan
    tyvekwallshortx1_log -> SetVisAttributes(tyvek_shortx1_gray);
    tyvek_shortx1_gray-> SetForceAuxEdgeVisible (true);//visualización de pared
    tyvek_shortx1_gray-> SetForceWireframe(true);
    tyvek_shortx1_gray-> SetForceSolid(true);
    tyvek_shortx1_gray-> SetVisibility(true);

    G4VisAttributes* tyvek_shorty1_gray= new G4VisAttributes(G4Colour::White()) ;  // cyan
    tyvekwallshorty1_log -> SetVisAttributes(tyvek_shorty1_gray);
    tyvek_shorty1_gray-> SetForceAuxEdgeVisible (true);//visualización de pared
    tyvek_shorty1_gray-> SetForceWireframe(true);
    tyvek_shorty1_gray-> SetForceSolid(true);
    tyvek_shorty1_gray-> SetVisibility(true);


    //program should return the physical volume which includes
    //all its properties
    return expHall_phys;
}
