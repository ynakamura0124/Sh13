#ifndef CLOVER_SIM_DETECTOR_CONSTRUCTION_HH
#define CLOVER_SIM_DETECTOR_CONSTRUCTION_HH

#include "LENSLongDetectorConstruction.hh"
#include "CloverSimDetector.hh"

class CloverSimDetectorConstruction : public LENSLongDetectorConstruction {
public:
	CloverSimDetectorConstruction() {}
	~CloverSimDetectorConstruction() {}

	G4VPhysicalVolume* Construct();
	void ConstructDetector();
};

#endif