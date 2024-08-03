#include "Sh13DetectorConstruction.hh"
#include "Sh13Detector.hh"
#include "globals.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"

#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "YamlReader.hpp"

#include "ExN02TrackerSD.hh"
#include "CloverSimAnalysisManager.hh"


using namespace CLHEP;
G4VPhysicalVolume *Sh13DetectorConstruction::Construct()
{
   ConstructMaterials();
   ConstructVisibilityAttributes();

   // Construct the WORLD Volume
   G4double worldHalf_x = 10.0 * m;
   G4double worldHalf_y = 10.0 * m;
   G4double worldHalf_z = 10.0 * m;

   G4Box *world_box = new G4Box("World_box", worldHalf_x, worldHalf_y, worldHalf_z);
   m_pWorld_logVol = new G4LogicalVolume(world_box, Vacuum, "World_logical", 0, 0, 0);
   m_pWorld_phyVol = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), m_pWorld_logVol, "World_physical", 0, 0, 0);

   //  ConstructLENSLongHall();
   //	if( m_UsePixelatedLattice )	ConstructLENSPixelatedLatticeScintillator();
   ConstructDetector();

   //  DumpGeometricalTree(m_pWorld_phyVol);// Get too long of an output and hence is confusing.

   return m_pWorld_phyVol;
}



void Sh13DetectorConstruction::ConstructMaterials()
{
   //   LENSLongDetectorConstruction::ConstructMaterials();
   //   if (m_PLA)
   //      delete m_PLA;
   // Germanium
   Ge = new G4Material("Germanium", 32., 72.64 * g / mole, 5.323 * g / cm3, kStateSolid, 77.36 * kelvin);

   // Vacuum
   Vacuum = new G4Material("Vacuum", 1., 1.01 * g / mole, universe_mean_density,
                           kStateGas, 2.73 * kelvin, 3.e-18 * pascal);

   return;
}



// AS & HPGe Detector
void Sh13DetectorConstruction::ConstructDetector()
{

   // Space
   G4double world_sizeXY = 5 * m;
   G4double world_sizeZ  = 5 * m;
   G4Material *worldMaterial = G4Material::GetMaterial("G4_AIR");
   G4Box *solidWorld = new G4Box("World", world_sizeXY, world_sizeXY, world_sizeZ);
   G4LogicalVolume *logicworld = new G4LogicalVolume(solidWorld, worldMaterial, "World");  
   
   // Al mylar Size
   G4double outerSizeX = 130.01 * mm;
   G4double outerSizeY = 100.01 * mm;
   G4double outerSizeZ = 10.01 * mm;
   G4Material *boxMaterial = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
   G4Box *solidOuterBox = new G4Box("OuterBox", outerSizeX, outerSizeY, outerSizeZ);
   G4double innerSizeX = 130 * mm;
   G4double innerSizeY = 100 * mm;
   G4double innerSizeZ = 10 * mm;
   G4Box *solidInnerBox = new G4Box("InnerBox", innerSizeX, innerSizeY, innerSizeZ);
   
   // Pla Size
   G4double PlaHeight = 130 *mm;
   G4double PlaWidth = 100 *mm;
   G4double PlaThickness = 10 *mm;
   G4SubtractionSolid *AlBox = new G4SubtractionSolid("AlMylar", solidOuterBox, solidInnerBox);
   G4Box *ActiveStopperSolid = new G4Box("ActiveStopperSoild", PlaHeight/2, PlaWidth/2, 2*PlaThickness/2); //2 Plastic were installed
   
   // AS Material
   G4Element *H = new G4Element("Hydrogen", "H", 1., 1.01 * g/mole);
   //G4Element *O = new G4Element("Oxygen", "O", 8., 16.0 * g/mole);
   G4Element *C = new G4Element("Carbon", "C", 6., 12.01 * g/mole);
   G4Material *Pla = new G4Material("Plastics", 1.032 * g/cm3, 2);
   Pla->AddElement(C, 9);
   //Pla->AddElement(O, 2);
   Pla->AddElement(H, 10);
   G4Material *Al = new G4Material("Aluminum", 13, 26.98 * g/mole, 2.7 * g/cm3, kStateSolid, 300 * kelvin, 1000 * pascal);

   // AS Logical Volume
   G4LogicalVolume *AlMylar = new G4LogicalVolume(AlBox, boxMaterial, "AlMylar");
   G4LogicalVolume *ActiveStopper = new G4LogicalVolume(ActiveStopperSolid, Pla, "ASsensitiveLV", 0, 0, 0);
   // G4LogicalVolume *ActiveStopper2 = new G4LogicalVolume(AlBox, Al, "ASsensitiveLV", 0, 0, 0);

   // AS Position
   const G4double ASdph = -45. * deg;
   G4RotationMatrix *ASangle = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   ASangle->rotateY(ASdph);
   G4ThreeVector ASposition = G4ThreeVector(0,0,0);
   ASposition.rotateY(ASdph);
   new G4PVPlacement(G4Transform3D(*ASangle,ASposition), ActiveStopper, "ActiveStopper", logicworld, false, 0, true);
   //new G4PVPlacement(G4Transform3D(*ASangle,ASposition), ActiveStopper2, "ActiveStopper", logicworld, false, 0, true);
   new G4PVPlacement(G4Transform3D(*ASangle,ASposition), AlMylar, "Almylar", logicworld, false, 0, true);

   // HPGe Detector Solid
   //G4double GeInner = 0 *mm;
   G4double Ge1Outer = 74.8 *mm;
   G4double Ge1Lenghth = 76.4 *mm;
   G4Tubs *SensitiveCylinder1 = new G4Tubs("sensitiveCylinder", 0 * mm, Ge1Outer/2, Ge1Lenghth/2, 0 * deg, 360 * deg);
   G4double Ge2Outer = 62.0 *mm;
   G4double Ge2Lenghth = 75.1 *mm;
   G4Tubs *SensitiveCylinder2 = new G4Tubs("sensitiveCylinder", 0 * mm, Ge2Outer/2, Ge2Lenghth/2, 0 * deg, 360 * deg);

   // HPGe Hole
   G4double HoleRadius1 = 10.9 * mm;
   G4double HoleHeight1 = 68.2 * mm;
   G4Tubs *Hole1 = new G4Tubs("Hole1", 0, HoleRadius1/2, HoleHeight1/2, 0, 360 * deg);
   G4double HoleRadius2 = 7.5 * mm;
   G4double HoleHeight2 = 45 * mm;
   G4Tubs *Hole2 = new G4Tubs("Hole2", 0, HoleRadius2/2, HoleHeight2/2, 0, 360 * deg);

   // HPGe Detector Material
   G4Material *Ge1 = new G4Material("Germanium1", 32, 72.64 * g / mole, 5.323 * g / cm3, kStateSolid, 300 * kelvin, 1000 * pascal);
   G4Material *Ge2 = new G4Material("Germanium2", 32, 72.64 * g / mole, 5.323 * g / cm3, kStateSolid, 300 * kelvin, 1000 * pascal);

   // Ge1 Ge2 Logical Volume
   G4SubtractionSolid *Ge1Scale = new G4SubtractionSolid("Ge1Scale", SensitiveCylinder1, Hole1);
   G4SubtractionSolid *Ge2Scale = new G4SubtractionSolid("Ge2Scale", SensitiveCylinder2, Hole2);
   G4LogicalVolume *layerLogicTube1 = new G4LogicalVolume(Ge1Scale, Ge1, "sensitiveLV", 0, 0, 0);
   G4LogicalVolume *layerLogicTube2 = new G4LogicalVolume(Ge2Scale, Ge2, "sensitiveLV", 0, 0, 0);

   // Ge1 Detector Position
   //G4ThreeVector SensitiveLogicTube1 = G4ThreeVector(100., 0, 0);
   const G4double dph1 = -180. * deg;
   G4RotationMatrix *Geangle1 = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   Geangle1->rotateY(dph1);
   G4ThreeVector Geposition1 = G4ThreeVector(-122.652 *mm, 0, 0);
   Geposition1.rotateY(dph1);
   new G4PVPlacement(G4Transform3D(*Geangle1,Geposition1), layerLogicTube1, "SensitiveGe1", logicworld, false, 0, true);
   
   // Ge2 Detector Position
   //G4ThreeVector SensitiveLogicTube2 = G4ThreeVector(-100., 0, 0);
   const G4double dph2 = 180. * deg;
   G4RotationMatrix *Geangle2 = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   Geangle2->rotateY(dph2);
   G4ThreeVector Geposition2 = G4ThreeVector(108.002 *mm, 0, 0);
   Geposition2.rotateY(dph2);
   new G4PVPlacement(G4Transform3D(*Geangle2,Geposition2), layerLogicTube2, "SensitiveGe2", logicworld , false, 0, true);

   // define sensitive detector
   G4SDManager *SDman = G4SDManager::GetSDMpointer();
   G4String trackerChamberSDname = "ExN02/TrackerChamberSD";
   ExN02TrackerSD *aTrackerSD = new ExN02TrackerSD(trackerChamberSDname);
   SDman->AddNewDetector(aTrackerSD);
   ActiveStopper->SetSensitiveDetector(aTrackerSD);
   layerLogicTube1->SetSensitiveDetector(aTrackerSD);
   layerLogicTube2->SetSensitiveDetector(aTrackerSD);  
   G4VisAttributes *layerVisAtt = new G4VisAttributes(G4Colour(1., 1., 1.));
   ActiveStopper->SetVisAttributes(layerVisAtt);
   layerLogicTube1->SetVisAttributes(layerVisAtt);
   layerLogicTube2->SetVisAttributes(layerVisAtt);
     
}