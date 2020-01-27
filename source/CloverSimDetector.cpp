#include "CloverSimDetector.hh"

void CloverSimDetector::ConstructDetector(LENSLongDetectorConstruction* mainDetectorConstruction) {
	ConstructLiquidDetector(mainDetectorConstruction);
	return;
}