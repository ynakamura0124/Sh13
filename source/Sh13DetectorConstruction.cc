#include "Sh13DetectorConstruction.hh"
#include "globals.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"

#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "YamlReader.hpp"

#include "ExN02TrackerSD.hh"

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

void Sh13DetectorConstruction::ConstructDetector()
{
   G4Tubs *sensitiveCylinder =
       new G4Tubs("sensitiveCylinder", 0 * cm, 18. * mm, 0.5 * 60. * mm, 0 * deg, 360 * deg);
   G4LogicalVolume *layerLogicTube =
       new G4LogicalVolume(sensitiveCylinder, Ge, "sensitiveLV", 0, 0, 0);
   G4RotationMatrix *angle = new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg);
   G4ThreeVector sensitivePosit1 = G4ThreeVector(0, 0, 100.);
   G4ThreeVector sensitivePosit2 = G4ThreeVector(0, 0, -100.);

   new G4PVPlacement(angle, sensitivePosit1, layerLogicTube,
                     "Sensitive", m_pWorld_logVol, false, 0);
   new G4PVPlacement(angle, sensitivePosit2, layerLogicTube,
                     "Sensitive", m_pWorld_logVol, false, 1);
   // define sensitive detector
   G4SDManager *SDman = G4SDManager::GetSDMpointer();

   G4String trackerChamberSDname = "ExN02/TrackerChamberSD";
   ExN02TrackerSD *aTrackerSD = new ExN02TrackerSD(trackerChamberSDname);
   SDman->AddNewDetector(aTrackerSD);
   layerLogicTube->SetSensitiveDetector(aTrackerSD);

   G4VisAttributes *layerVisAtt = new G4VisAttributes(G4Colour(1., 1., 1.));
   layerLogicTube->SetVisAttributes(layerVisAtt);
}
