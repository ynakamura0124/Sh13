#ifndef CLOVER_SIM_DETECTOR_HH
#define CLOVER_SIM_DETECTOR_HH

#include "LENSLongLiquidScintillator.hh"

class CloverSimDetector : public LENSLongLiquidScintillator {
public:
	CloverSimDetector( G4String name ) : LENSLongLiquidScintillator( name ) {};
	virtual ~CloverSimDetector() {};

	void ConstructDetector(LENSLongDetectorConstruction* mainDetectorConstructor);
};

#endif