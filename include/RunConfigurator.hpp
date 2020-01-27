#ifndef RUN_CONFIGURATOR_HPP
#define RUN_CONFIGURATOR_HPP

#include <iostream>
#include "G4VUserDetectorConstruction.hh"
#include "G4VUserPhysicsList.hh"
#include "G4UserRunAction.hh"
#include "G4UserEventAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "YamlReader.hpp"

#include "PhysicsList.hh"
#include "LENSLongRunAction.hh"
#include "LENSLongEventAction.hh"
#include "LENSLongPrimaryGeneratorAction.hh"
#include "LENSLongDetectorConstruction.hh"
#include "CloverSimDetectorConstruction.hh"
#include "PointGammaSourceGeneratorAction.hh"

class RunConfigurator {
public:
	RunConfigurator() {}
	virtual ~RunConfigurator();

	int Configure(G4RunManager* runManager);
	G4VUserDetectorConstruction* GetDetectorConstruction() { return detector_construction_; }

protected:
	G4VUserDetectorConstruction* detector_construction_ = nullptr;
	G4VUserPhysicsList* physics_list_ = nullptr;
	G4UserRunAction* run_action_ = nullptr;
	G4UserEventAction* event_action_ = nullptr;
	G4VUserPrimaryGeneratorAction* primary_generator_action_ = nullptr;
};
#endif