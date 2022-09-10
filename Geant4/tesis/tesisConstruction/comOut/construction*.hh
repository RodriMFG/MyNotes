#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class LXeMainVolume;
class G4Sphere;

#include "G4Material.hh"
//#include "LXeDetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "LXeScintSD.hh"
#include "LXePMTSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    //Functions to modify the geometry
  /*  void SetDimensions(G4ThreeVector );
    void SetHousingThickness(G4double );
    void SetNX(G4int );
    void SetNY(G4int );
    void SetNZ(G4int );
    void SetPMTRadius(G4double );
    void SetDefaults();*/

    //Get values
   /* G4int GetNX(){return fNx;}
    G4int GetNY(){return fNy;}
    G4int GetNZ(){return fNz;}
    G4double GetScintX(){return fScint_x;}
    G4double GetScintY(){return fScint_y;}
    G4double GetScintZ(){return fScint_z;}
    G4double GetHousingThickness(){return fD_mtl;}
    G4double GetPMTRadius(){return fOuterRadius_pmt;}
    G4double GetSlabZ(){return fSlab_z;}

    void SetSphereOn(G4bool );
    static G4bool GetSphereOn(){return fSphereOn;}

    void SetHousingReflectivity(G4double );
    G4double GetHousingReflectivity(){return fRefl;}

    void SetWLSSlabOn(G4bool b);
    G4bool GetWLSSlabOn(){return fWLSslab;}

    void SetMainVolumeOn(G4bool b);
    G4bool GetMainVolumeOn(){return fMainVolumeOn;}

    void SetNFibers(G4int n);
    G4int GetNFibers(){return fNfibers;}

    void SetMainScintYield(G4double );
    void SetWLSScintYield(G4double );*/

  private:
    G4VPhysicalVolume* ConstructDetector();
    void DefineMaterials();

    G4VPhysicalVolume* expHall_phys;
    //LXeDetectorMessenger* fDetectorMessenger;


    //Materials & Elements
    //G4Material* fLXe;
    G4Material* Al;
    G4Element* N;
    G4Element* O;
    //G4Material* Air;
    G4Material* Vacuum;
    G4Material* water;
    G4Material* Tyvek;
    G4Element* C;
    G4Element* H;
   /* G4Material* fGlass;
    G4Material* fPstyrene;
    G4Material* fPMMA;
    G4Material* fPethylene1;
    G4Material* fPethylene2;*/

    //Geometry
   G4double fExpHall_x;
    G4double fExpHall_y;
    G4double fExpHall_z;

    G4double fTank_x;
    G4double fTank_y;
    G4double fTank_z;

    G4double fBubble_x;
    G4double fBubble_y;
    G4double fBubble_z;

    G4double scint_x;
    G4double scint_y;
    G4double scint_z;
    G4double Scint_z;

    G4double outerRadius_pmtL;
    G4double outerRadius_pmtC;

    G4double move_x1;
    G4double move_y1;

    G4double move_x2;
    G4double move_y2;

    G4double move_x3;
    G4double move_y3;

    G4double move_x4;
    G4double move_y4;

    G4double wtyvek;
    G4double d_mtl;
    G4double hcal;

   /* LXeMainVolume* fMainVolume;

    G4MaterialPropertiesTable* fLXe_mt;
    G4MaterialPropertiesTable* fMPTPStyrene;*/

     G4LogicalVolume* waterTank_log_c;
     G4LogicalVolume* photocath_logL;
    //Sensitive Detectors
    G4Cache<LXeScintSD*> fScint_SD;
    G4Cache<LXePMTSD*> fPmt_SD;

};

#endif
