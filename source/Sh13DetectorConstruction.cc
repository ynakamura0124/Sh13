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
#include "G4UnionSolid.hh"

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

/*
   // Space
   G4double world_sizeXY = 3 * m;
   G4double world_sizeZ  = 3 * m;
   G4Material *worldMaterial = G4Material::GetMaterial("G4_AIR");
   G4Box *solidWorld = new G4Box("World", world_sizeXY, world_sizeXY, world_sizeZ);
   G4LogicalVolume *logicworld = new G4LogicalVolume(solidWorld, worldMaterial, "World");  
*/
   
   // Al mylar Size
   G4double outerSizeX = 130.01 * mm;
   G4double outerSizeY = 100.01 * mm;
   G4double outerSizeZ = 10.01 * mm;
   // G4Material *boxMaterial = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
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
   Pla->AddElement(H, 10);
   // Pla->AddElement(O, 2);
   G4Material *Al = new G4Material("Aluminum", 13, 26.98 * g/mole, 2.7 * g/cm3, kStateSolid, 300 * kelvin, 1000 * pascal);

   // AS Logical Volume
   // G4LogicalVolume *AlMylar = new G4LogicalVolume(AlBox, boxMaterial, "AlMylar");
   G4LogicalVolume *ActiveStopper = new G4LogicalVolume(ActiveStopperSolid, Pla, "ASsensitiveLV", 0, 0, 0);
   G4LogicalVolume *AlMylar = new G4LogicalVolume(AlBox, Al, "ASsensitiveLV", 0, 0, 0);

   // AS Position
   const G4double ASdph = -45. * deg;
   G4RotationMatrix *ASangle = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   ASangle->rotateY(ASdph);
   G4ThreeVector ASposition = G4ThreeVector(0,0,0);
   ASposition.rotateY(ASdph);
   new G4PVPlacement(G4Transform3D(*ASangle,ASposition), ActiveStopper, "ActiveStopper", m_pWorld_logVol, false, 0, true);
   //new G4PVPlacement(G4Transform3D(*ASangle,ASposition), ActiveStopper2, "ActiveStopper", m_pWorld_logVol, false, 0, true);
   new G4PVPlacement(G4Transform3D(*ASangle,ASposition), AlMylar, "AlMylar", m_pWorld_logVol, false, 0, true);

/*
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
   //G4ThreeVector SensitiveLogicTube1 = G4ThreeVector(100, 0, 0);
   const G4double dph1 = -180. * deg;
   G4RotationMatrix *Geangle1 = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   Geangle1->rotateY(dph1);
   G4ThreeVector Geposition1 = G4ThreeVector(-122.652 *mm, 0, 0);
   Geposition1.rotateY(dph1);
   new G4PVPlacement(G4Transform3D(*Geangle1,Geposition1), layerLogicTube1, "SensitiveGe1", m_pWorld_logVol, false, 0, true);

   // Ge2 Detector Position
   // G4ThreeVector SensitiveLogicTube2 = G4ThreeVector(-100, 0, 0);
   const G4double dph2 = 180. * deg;
   G4RotationMatrix *Geangle2 = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   Geangle2->rotateY(dph2);
   G4ThreeVector Geposition2 = G4ThreeVector(108.002 *mm, 0, 0);
   Geposition2.rotateY(dph2);
   new G4PVPlacement(G4Transform3D(*Geangle2,Geposition2), layerLogicTube2, "SensitiveGe2", m_pWorld_logVol, false, 0, true);
*/

   // Ge1 Detector
   // Ge1 Cylinder
   G4double Ge1OuterDiameter = 74.8 * mm;
   G4double Ge1Height = 76.4 * mm;
   G4Tubs *Ge1OuterCylinder = new G4Tubs("Ge1OuterCylinder", 0, Ge1OuterDiameter/2, Ge1Height/2, 0, 360. * deg);
   // Ge1 Hole
   G4double Ge1HoleDiameter = 10.9 * mm;
   G4double Ge1HoleHeight = 68.2 * mm;
   G4Tubs *Ge1Hole = new G4Tubs("Ge1Hole", 0, Ge1HoleDiameter/2, Ge1HoleHeight/2, 0, 360. * deg);
   G4ThreeVector Ge1HolePosition(0, 0, -(Ge1Height - Ge1HoleHeight)/2); // Hole in the center of the bottom
   // Ge1 Cylinder with Hole
   G4SubtractionSolid *Ge1CylWithHole = new G4SubtractionSolid("Ge1CylWithHole", Ge1OuterCylinder, Ge1Hole, 0, Ge1HolePosition); // Leave
   G4Material *Ge1 = new G4Material("Germanium1", 32, 72.64 * g / mole, 5.323 * g / cm3, kStateSolid, 300 * kelvin, 1000 * pascal);
   G4LogicalVolume *Ge1Detector = new G4LogicalVolume(Ge1CylWithHole, Ge1, "Ge1LV");
   // Ge1 Position
   const G4double dph1 = 90. * deg;
   G4RotationMatrix *Geangle1 = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   Geangle1->rotateY(dph1); //
   G4ThreeVector Geposition1 = G4ThreeVector(-126.652 *mm, 0, 0);
   Geposition1.rotateY(dph1); //
   new G4PVPlacement(G4Transform3D(*Geangle1,Geposition1), Ge1Detector, "Ge1Detector", m_pWorld_logVol, false, 0, true);

   // Ge1 Cup1
   // Ge1 Cup1 Size
   G4double Ge1Cup1OuterDiameter = 80 * mm; // unknown
   G4double Ge1Cup1InnerDiameter = 78.4 * mm; // side thickness 0.8 mm
   G4double Ge1Cup1Height = 130 * mm;
   G4double Ge1Cup1TopThickness = 0.03 mm;
   G4double Ge1Cup1BottomThickness = 3 * mm;
   G4double Ge1Cup1HoleDiameter = 10.9 mm;
   // Ge1 Cup1 Side Volume
   G4Tubs *Ge1Cup1Side = new G4Tubs("Ge1Cup1Side", Ge1Cup1InnerDiameter/2, Ge1Cup1OuterDiameter/2, (Ge1Cup1Height - Ge1Cup1TopThickness - Ge1Cup1BottomThickness)/2, 0, 360. * deg);
   // Ge1 Cup1 Top Volume
   G4Tubs *Ge1Cup1Top = new G4Tubs("Ge1Cup1Top", 0, Ge1Cup1OuterDiameter/2, Ge1Cup1TopThickness/2, 0, 360. * deg);
   // Ge1 Cup1 Bottom Volume
   G4Tubs *Ge1Cup1BottomDisk = new G4Tubs("Ge1Cup1BottomDisk", 0, Ge1Cup1OuterDiameter/2, Ge1Cup1BottomThickness/2, 0, 360. * deg);
   G4Tubs *Ge1Cup1BottomHole = new G4Tubs("Ge1Cup1BottomHole", 0, Ge1Cup1HoleDiameter/2, Ge1Cup1BottomThickness/2, 0, 360. * deg);
   G4SubtractionSolid *Ge1Cup1Bottom = new G4SubtractionSolid("Ge1Cup1Bottom", Ge1Cup1BottomDisk, Ge1Cup1BottomHole);
   // Ge1 Cup1 Volume Combining
   G4ThreeVector Ge1Cup1TopPosition(0, 0, (Ge1Cup1Height - Ge1Cup1TopThickness)/2);
   G4ThreeVector Ge1Cup1BottomPosition(0, 0, (Ge1Cup1BottomThickness - Ge1Cup1Height)/2);
   G4UnionSolid *Ge1Cup1TopWithSide = new G4UnionSolid("Ge1Cup1TopWithSide", Ge1Cup1Side, Ge1Cup1Top, 0, Ge1Cup1TopPosition); // Top placed
   G4UnionSolid *Ge1Cup1CompleteCylinder = new G4UnionSolid("Ge1Cup1CompleteCylinder", Ge1Cup1TopWithSide, Ge1Cup1Bottom, 0, Ge1Cup1BottomPosition);
   // Ge1 Cup1 Material
   G4LogicalVolume *Ge1Cup1 = new G4LogicalVolume(Ge1Cup1CompleteCylinder, Al, "Ge1Cup1CompleteCylinderLV");
   // Ge1 Cup1 Position
   G4ThreeVector Ge1Cup1Position = G4ThreeVector(-126.652 *mm, 0, 0); // Geposition1
   Ge1Cup1Position.rotateY(dph1);
   new G4PVPlacement(G4Transform3D(*Geangle1,Ge1Cup1Position), Ge1Cup1, "Ge1Cup1", m_pWorld_logVol, false, 0, true);

   // Ge1 Cup2
   // Ge1 Cup2 Size
   G4double Ge1Cup2Height = 150 * mm; // unknown
   G4double Ge1Cup2AlPartThickness = 1.5 * mm;
   G4double Ge1Cup2OuterDiameter = 100 * mm; // unknown
   G4double Ge1Cup2InnerDiameter = 97 * mm; // Cup2 Thickness = 1.5 mm
   G4double Ge1Cup2CarbonFiberDiameter = 80 * mm; // unknown
   G4double Ge1Cup2CarbonThickness = 0.9 * mm;
   // Ge1 Cup2 Side Volume
   G4Tubs *Ge1Cup2Outer = new G4Tubs("OuterCup", 0, Ge1Cup2OuterDiameter/2, Ge1Cup2Height/2, 0, 360. *deg);
   G4Tubs *Ge1Cup2Inner = new G4Tubs("InnerCup", 0, Ge1Cup2InnerDiameter/2, Ge1Cup2Height/2, 0, 360. *deg);
   G4SubtractionSolid *Ge1Cup2Side = new G4SubtractionSolid("Ge1Cup2Side", Ge1Cup2Outer);
   // Ge1 Cup2 Top Al Volume
   G4Tubs *Ge1Cup2TopOuter = new G4Tubs("Ge1Cup2TopOuter", 0, Ge1Cup2OuterDiameter/2, Ge1Cup2AlPartThickness/2, 0, 360. *deg);
   G4Tubs *Ge1Cup2TopInner = new G4Tubs("Ge1Cup2TopInner", 0, Ge1Cup2CarbonFiberDiameter/2, Ge1Cup2AlPartThickness/2, 0, 360. *deg);
   G4SubtractionSolid *Ge1Cup2TopAlPart = new G4SubtractionSolid("Ge1Cup2TopAlPart", Ge1Cup2TopOuter, Ge1Cup2TopInner);
   // Ge1 Cup2 Top Carbon Fiber
   G4Tubs *Ge1Cup2CarbonFiber = new G4Tubs("Ge1Cup2CarbonFiber", 0, Ge1Cup2CarbonFiberDiameter/2, Ge1Cup2AlPartThickness/2, 0, 360. *deg);
   G4double C_Density = 2.267 * g/cm3;
   G4Material *C_Mat = new G4Material("Carbon_Material", C_Density, 1);
   C_Mat->AddElement(C, 1);
   // Ge1 Cup2 Volume Combining
   G4ThreeVector Ge1Cup2TopAlPartPosition(0, 0, (Ge1Cup2Height - Ge1Cup2AlPartThickness)/2);
   G4ThreeVector Ge1Cup2CarbonFiberPosition(0, 0, (Ge1Cup2Height - Ge1Cup2CarbonThickness)/2);
   G4UnionSolid *Ge1Cup2TopWithSide = new G4UnionSolid("Ge1Cup2TopWithSide", Ge1Cup2Side, Ge1Cup2TopAlPart, 0, Ge1Cup2TopAlPartPosition); // 
   G4LogicalVolume *Ge1Cup2 = new G4LogicalVolume(Ge1Cup2TopWithSide, Al, "Ge1Cup2TopWithSide");
   G4UnionSolid *Ge1Cup2WithCarbon = new G4UnionSolid("Ge1Cup2WithCarbon", Ge1Cup2TopWithSide, Ge1Cup2CarbonFiber, 0, Ge1Cup2CarbonFiberPosition);
   G4LogicalVolume *Ge1Cup2CarbonFiberLV = new G4LogicalVolume(Ge1Cup2CarbonFiber, C_Mat, "Ge1Cup2CarbonFiberLV");
   // Ge1 Cup2 Position
   G4ThreeVector Ge1Cup2Position = G4ThreeVector(-122.562 *mm, 0, 0); // Ge1Cup1Position + Space
   Ge1Cup2Position.rotateY(dph1);
   new G4PVPlacement(G4Transform3D(*Geangle1,Ge1Cup2Position), Ge1Cup2, "Ge1Cup2", m_pWorld_logVol, false, 0, true);
   new G4PVPlacement(G4Transform3D(*Geangle1,Ge1Cup2Position), Ge1Cup2CarbonFiberLV, "Ge1Cup2CarbonFiber", m_pWorld_logVol, false, 0, true);

   // Ge2 Detector
   // Ge2 Cylinder
   G4double Ge2OuterDiameter = 62.0 * mm;
   G4double Ge2Height = 75.1 * mm;
   G4Tubs *Ge2OuterCylinder = new G4Tubs("Ge2OuterCylinder", 0, Ge2OuterDiameter/2, Ge2Height/2, 0, 360. *deg);
   // Ge2 Hole
   G4double Ge2HoleDiameter = 7.5 * mm;
   G4double Ge2HoleHeight = 45 * mm;
   G4Tubs *Ge2Hole = new G4Tubs("Ge2Hole", 0, Ge2HoleDiameter/2, Ge2HoleHeight/2, 0, 360. *deg);
   G4ThreeVector Ge2HolePosition(0, 0, -(Ge2Height - Ge2HoleHeight)/2);
   // Ge2 Cylinder with Hole
   G4SubtractionSolid *Ge2CylWithHole = new G4SubtractionSolid("Ge2CylWithHole", Ge2OuterCylinder, Ge2Hole, 0, Ge2HolePosition);
   G4Material *Ge2 = new G4Material("Germanium2", 32, 72.64 * g / mole, 5.323 * g / cm3, kStateSolid, 300 * kelvin, 1000 * pascal);
   G4LogicalVolume *Ge2Detector = new G4LogicalVolume(Ge2CylWithHole, Ge2, "Ge2LV");
   // Ge2 Position
   const G4double dph2 = -90. * deg;
   G4RotationMatrix *Geangle2 = new G4RotationMatrix(0. * deg, 0 * deg, 0 * deg);
   Geangle2->rotateY(dph2);
   G4ThreeVector Geposition2 = G4ThreeVector(113.002 * mm, 0, 0);
   Geposition2.rotateY(dph2);
   new G4PVPlacement(G4Transform3D(*Geangle2, Geposition2), Ge2Detector, "Ge2Detector", m_pWorld_logVol, false, 0, true);

   // Ge2 CrystalHolder
   // Ge2 CrystalHolder Size
   G4double Ge2CrystalHolderHeight = 100 * mm; // unknown
   G4double Ge2CrystalHolderTopThickness = 0.0003 * mm;
   G4double Ge2CrystalHolderSideThickness = 0.76 * mm;
   G4double Ge2CrystalHolderBottomThickness = 3.2 * mm;
   G4double Ge2CrystalHolderInnerDiameter = 62 * mm;
   G4double Ge2CrystalHolderOuterDiameter = (Ge2CrystalHolderInnerDiameter + (2 * Ge2CrystalHolderSideThickness));
   // Ge2 CrystalHolder Cylinder Part
   G4Tubs *Ge2CrystalHolderForm = new G4Tubs("Ge2CrystalHolderForm", 0, Ge2CrystalHolderOuterDiameter/2, Ge2CrystalHolderHeight/2, 0, 360. * deg);
   G4Tubs *Ge2CrystalHolderHollow = new G4Tubs("Ge2CrystalHolderHollow", 0, Ge2CrystalHolderInnerDiameter/2, (Ge2CrystalHolderHeight - Ge2CrystalHolderTopThickness - Ge2CrystalHolderBottomThickness)/2, 0, 360. * deg);
   G4ThreeVector Ge2CrystalHolderHollowPosition(0, 0, (Ge2CrystalHolderHeight - Ge2CrystalHolderTopThickness - Ge2CrystalHolderBottomThickness)/2);
   G4SubtractionSolid *Ge2CrystalHolderCylinder = new G4SubtractionSolid("Ge2CylWithHole", Ge2CrystalHolderForm, Ge2CrystalHolderHollow, 0, Ge2CrystalHolderHollowPosition);
   
   
   // Ge2 CrystalHolder Side Al Projection Part
   //G4double Ge2CrystalHolderSideProHeight = 8.6 * mm;
   //G4double Ge2CrystalHolderSideProWide = (2.7 - Ge2CrystalHolderSideThickness) * mm;
   //G4double Ge2CrystalHolderBottomProHoleDiameter = Ge2HoleDiameter;
   
   
   // Ge2 CrystalHolder Bottom Al Projection Part
   G4double Ge2CrystalHolderBottomProHeight = 40 * mm; // unknown
   G4double Ge2CrystalHolderBottomProDiameter = 31 * mm; // unknown
   G4double Ge2CrystalHolderBottomProHoleDiameter = Ge2HoleDiameter;
   G4double Ge2CrystalHolderBottomProHoleHeight = 10 * mm; // unknown Ge2Ge2CrystalHolderBottomPro
   G4Tubs *Ge2CrystalHolderBottomProCyl = new G4Tubs("Ge2CrystalHolderBottomProCyl", 0, Ge2CrystalHolderBottomProDiameter/2, Ge2CrystalHolderBottomProHeight/2, 0, 360. * deg);
   G4Tubs *Ge2CrystalHolderBottomProHole = new G4Tubs("Ge2CrystalHolderBottomProHole", 0, Ge2CrystalHolderBottomProHoleDiameter/2, Ge2CrystalHolderBottomProHoleHeight/2, 0, 360. * deg);
   G4ThreeVector Ge2CrystalHolderBottomProHolePosition(0, 0, -(Ge2CrystalHolderBottomProHeight - Ge2CrystalHolderBottomProHoleHeight)/2);
   G4SubtractionSolid *Ge2CrystalHolderBottomPro = new G4SubtractionSolid("Ge2CrystalHolderBottomPro", Ge2CrystalHolderBottomProCyl, Ge2CrystalHolderBottomProHole, 0, Ge2CrystalHolderBottomProHolePosition);
   // Ge2  CrystalHolder Teflon Part
   G4double Ge2CrystalHolderTeflonHeight = 20 * mm; // unknown
   G4double Ge2CrystalHolderTeflonDiameter = 15 * mm; // unknown
   G4double Ge2CrystalHolderTeflonHoleDiameter = Ge2HoleDiameter;
   G4double Ge2CrystalHolderTeflonHoleHeight = 10 * mm; // unknown
   G4Tubs *Ge2CrystalHolderTeflonCyl = new G4Tubs("Ge2CrystalHolderTeflonCyl", 0, Ge2CrystalHolderTeflonDiameter/2, Ge2CrystalHolderTeflonHeight/2, 0, 360. * deg);
   G4Tubs *Ge2CrystalHolderTeflonHole = new G4Tubs("Ge2CrystalHolderTeflonHole", 0, Ge2CrystalHolderTeflonHoleDiameter/2, Ge2CrystalHolderTeflonHoleHeight/2, 0, 360. * deg);
   G4ThreeVector Ge2CrystalHolderTeflonHolePosition(0, 0, (Ge2CrystalHolderTeflonHeight - Ge2CrystalHolderTeflonHoleHeight)/2);
   G4SubtractionSolid *Ge2CrystalHolderTeflon = new G4SubtractionSolid("Ge2CrystalHolderTeflon", Ge2CrystalHolderTeflonCyl, Ge2CrystalHolderTeflonHole, 0, Ge2CrystalHolderTeflonHolePosition);


   // Ge2 CrystalHolder Volume Combining
   // G4ThreeVector Ge2CrystalHolderSideProPosition(0, 0, (Ge2EndCupHeight - Ge2EndCupAlPartThickness)/2);
   // G4UnionSolid Ge2CrystalHolderCylinderWithBottomPro = new G4UnionSolid("Ge2CrystalHolderCylinderWithBottomPro",);


   G4ThreeVector Ge2CrystalHolderBottomProPosition(0, 0, -(Ge2CrystalHolderHeight - Ge2CrystalHolderTopThickness - Ge2CrystalHolderBottomThickness)/2);
   G4UnionSolid *Ge2CrystalHolderCylinderWithBottomPro = new G4UnionSolid("Ge2CrystalHolderCylinderWithBottomPro", Ge2CrystalHolderCylinder, Ge2CrystalHolderBottomPro, 0, Ge2CrystalHolderBottomProPosition);
   G4LogicalVolume *Ge2CrystalHolder = new G4LogicalVolume(Ge2CrystalHolderCylinderWithBottomPro, Al, "Ge2CrystalHolderLV");
   // Ge2 CrystalHolder add Teflon
   G4ThreeVector Ge2CrystalHolderTeflonPosition(0, 0, -(Ge2CrystalHolderHeight - Ge2CrystalHolderTopThickness - Ge2CrystalHolderBottomThickness)/2);
   G4UnionSolid *Ge2CrystalHolderCylinderComp = new G4UnionSolid("Ge2CrystalHolderCylinderComp", Ge2CrystalHolderCylinderWithBottomPro, Ge2CrystalHolderTeflon, 0, Ge2CrystalHolderTeflonPosition);
   G4Material *Tef = new G4Material("Teflon", 2.2 * g/cm3, 2);
   Tef->AddElement(C, 2);
   Tef->AddElement(H, 4);
   G4LogicalVolume *Ge2CrystalHolderTeflonLV = new G4LogicalVolume(Ge2CrystalHolderTeflon, Tef, "Ge2CrystalHolderTeflonLV");
   // Ge2 CrystalHolder Position
   G4ThreeVector Ge2CrystalHolderPosition = G4ThreeVector(113.002 *mm, 0, 0); // Geposition2
   Ge2CrystalHolderPosition.rotateY(dph2);
   new G4PVPlacement(G4Transform3D(*Geangle2, Ge2CrystalHolderPosition), Ge2CrystalHolder, "Ge2CrystalHolder", m_pWorld_logVol, false, 0, true);
   new G4PVPlacement(G4Transform3D(*Geangle2, Ge2CrystalHolderPosition), Ge2CrystalHolderTeflonLV, "Ge2CrystalHolderTeflon", m_pWorld_logVol, false, 0, true);

   // Ge2 EndCup
   // Ge2 EndCup Size
   G4double Ge2EndCupHeight = 150 * mm; // unknown
   G4double Ge2EndCupAlPartThickness = 1.5 * mm;
   G4double Ge2EndCupOuterDiameter = 100 * mm; // unknown
   G4double Ge2EndCupInnerDiameter = (Ge2EndCupOuterDiameter - (2 * Ge2EndCupAlPartThickness));
   G4double Ge2EndCupCarbonDiameter = 90 * mm; // unknown
   G4double Ge2EndCupCarbonThickness = 0.6 * mm;
   // Ge2 EndCup Side Volume
   G4Tubs *Ge2EndCupOuter = new G4Tubs("Ge2EndCupOuter", 0, Ge2EndCupOuterDiameter/2, Ge2EndCupHeight/2, 0, 360. * deg);
   G4Tubs *Ge2EndCupInner = new G4Tubs("Ge2EndCupInner", 0, Ge2EndCupInnerDiameter/2, Ge2EndCupHeight/2, 0, 360. * deg);
   G4SubtractionSolid *Ge2EndCupSide = new G4SubtractionSolid("Ge2EndCupSide", Ge2EndCupOuter, Ge2EndCupInner);
   G4Tubs *Ge2EndCupBottom = new G4Tubs("Ge2EndCupBottom", 0, Ge2EndCupInnerDiameter/2, Ge2EndCupAlPartThickness/2, 0, 360. * deg);
   G4ThreeVector Ge2EndCupBottomPosition(0, 0, (Ge2EndCupAlPartThickness - Ge2EndCupHeight)/2);
   G4UnionSolid *Ge2EndCupSideWithBottom = new G4UnionSolid("Ge2EndCupSideWithBottom", Ge2EndCupSide, Ge2EndCupBottom, 0, Ge2EndCupBottomPosition);
   // Ge2 EndCup Top Al Part
   G4Tubs *Ge2EndCupTopOuter = new G4Tubs("Ge2EndCupTopOuter", 0, Ge2EndCupOuterDiameter/2, Ge2EndCupAlPartThickness/2, 0, 360. * deg);
   G4Tubs *Ge2EndCupTopInner = new G4Tubs("Ge2EndCupTopInner", 0, Ge2EndCupCarbonDiameter/2, Ge2EndCupAlPartThickness/2, 0, 360. * deg);
   G4SubtractionSolid *Ge2EndCupTopAlPart = new G4SubtractionSolid("Ge2EndCupTopAlPart", Ge2EndCupTopOuter, Ge2EndCupTopInner);
   // Ge2 EndCup Top Carbon
   G4Tubs *Ge2EndCupCarbon = new G4Tubs("Ge2EndCupCarbon", 0, Ge2EndCupCarbonDiameter/2, Ge2EndCupCarbonThickness/2, 0, 360. * deg);
   // Ge2 EndCup Volume Combining
   G4ThreeVector Ge2EndCupTopAlPartPosition(0, 0, (Ge2EndCupHeight - Ge2EndCupAlPartThickness)/2);
   G4ThreeVector Ge2EndCupCarbonPosition(0, 0, (Ge2EndCupHeight - Ge2EndCupCarbonThickness)/2);
   G4UnionSolid *Ge2EndCupTopWithSide = new G4UnionSolid("Ge2EndCupTopWithSide", Ge2EndCupSideWithBottom, Ge2EndCupTopAlPart, 0, Ge2EndCupTopAlPartPosition);
   G4LogicalVolume *Ge2EndCup = new G4LogicalVolume(Ge2EndCupTopWithSide, Al, "Ge2EndCupCompleteCylinder");
   G4UnionSolid *Ge2EndCupWithCarbon = new G4UnionSolid("Ge2EndCupWithCarbon", Ge2EndCupTopWithSide, Ge2EndCupCarbon, 0, Ge2EndCupCarbonPosition);
   G4LogicalVolume *Ge2EndCupCarBonLV = new G4LogicalVolume(Ge2EndCupCarbon, C_Mat, "Ge2EndCupCarBonLV");
   // Ge2 EndCup Position
   G4ThreeVector Ge2EndCupPosition = G4ThreeVector(108.002 *mm, 0, 0); // Ge1Cup1Position + Space
   Ge2EndCupPosition.rotateY(dph2);
   new G4PVPlacement(G4Transform3D(*Geangle2, Ge2EndCupPosition), Ge2EndCup, "Ge2EndCup", m_pWorld_logVol, false, 0, true);
   new G4PVPlacement(G4Transform3D(*Geangle2, Ge2EndCupPosition), Ge2EndCupCarBonLV, "Ge2EndCupCarBon", m_pWorld_logVol, false, 0, true);

   // define sensitive detector
   G4SDManager *SDman = G4SDManager::GetSDMpointer();
   G4String trackerChamberSDname = "ExN02/TrackerChamberSD";
   ExN02TrackerSD *aTrackerSD = new ExN02TrackerSD(trackerChamberSDname);
   SDman->AddNewDetector(aTrackerSD);
   // AlMylar->SetSensitiveDetector(aTrackerSD);
   // ActiveStopper->SetSensitiveDetector(aTrackerSD);
   Ge1Detector->SetSensitiveDetector(aTrackerSD);
   // Ge1Cup1->SetSensitiveDetector(aTrackerSD);
   // Ge1Cup2->SetSensitiveDetector(aTrackerSD);
   Ge2Detector->SetSensitiveDetector(aTrackerSD);
   // Ge2Cup1->SetSensitiveDetector(aTrackerSD);
   // Ge2CrystalHolder->SetSensitiveDetector(aTrackerSD);
   // Ge2EndCup->SetSensitiveDetector(aTrackerSD);
   G4VisAttributes *layerVisAtt = new G4VisAttributes(G4Colour(1., 0., 0.));
   // AlMylar->SetVisAttributes(layerVisAtt);
   // ActiveStopper->SetVisAttributes(layerVisAtt);
   Ge1Detector->SetVisAttributes(layerVisAtt);
   // Ge1Cup1->SetVisAttributes(layerVisAtt);
   // Ge1Cup2->SetVisAttributes(layerVisAtt);
   Ge2Detector->SetVisAttributes(layerVisAtt);
   // Ge2Cup1->SetVisAttributes(layerVisAtt);
   // Ge2CrystalHolder->SetVisAttributes(layerVisAtt);
   // Ge2EndCup->SetVisAttributes(layerVisAtt);
     
}