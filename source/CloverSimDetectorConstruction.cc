#include "CloverSimDetectorConstruction.hh"
#include "globals.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"

#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"



using namespace CLHEP;
G4VPhysicalVolume* CloverSimDetectorConstruction::Construct()
{
	ConstructMaterials();
	ConstructVisibilityAttributes();

// Construct the WORLD Volume
  G4double worldHalf_x = 10.0*m;
  G4double worldHalf_y = 10.0*m;
  G4double worldHalf_z = 10.0*m;
	
  G4Box* world_box = new G4Box( "World_box", worldHalf_x, worldHalf_y, worldHalf_z );
  m_pWorld_logVol = new G4LogicalVolume( world_box,  m_Vacuum, "World_logical", 0, 0, 0 );
  m_pWorld_phyVol = new G4PVPlacement( 0, G4ThreeVector(0.0,0.0,0.0), m_pWorld_logVol, "World_physical", 0, 0, 0 );
	
//  ConstructLENSLongHall();
//	if( m_UsePixelatedLattice )	ConstructLENSPixelatedLatticeScintillator();
	ConstructLENSLongScintillator();

//  DumpGeometricalTree(m_pWorld_phyVol);// Get too long of an output and hence is confusing.

  return m_pWorld_phyVol;
}

void CloverSimDetectorConstruction::ConstructDetector() {
   G4String SDname;
   m_LENSLongBoxScintillatorSD = new CloverSimDetector( SDname = "/LiquidScintillator" );
// The "this" pointer alows the LENSLongLiquidScintillator to get things such as BunkerWidth(), Materials, etc...
	m_LENSLongBoxScintillatorSD->ConstructLiquidDetector( this ); 

}

