#include "RunConfigurator.hpp"

RunConfigurator::~RunConfigurator() {
	//if (yaml_reader_) {
	//	delete yaml_reader_;
	//}
	//if (detector_construction_)
	//	delete detector_construction_;
}

int RunConfigurator::Configure(G4RunManager* runManager) {

	// reads the yaml file and selects UserInitializaions and UserActions
	YamlReader yaml_reader("RunConfigurator");

	if (detector_construction_) {
		delete detector_construction_;
		detector_construction_ = nullptr;
	}
	if (physics_list_) {
		delete physics_list_;
		physics_list_ = nullptr;
	}
	if (run_action_) {
		delete run_action_;
		run_action_ = nullptr;
	}
	if (event_action_) {
		delete event_action_;
		event_action_ = nullptr;
	}
	if (primary_generator_action_) {
		delete primary_generator_action_;
		primary_generator_action_ = nullptr;
	}

	// Selection of the DetectorConstruction from Yaml
	std::map<std::string, std::function<void()>> dc_map;
	dc_map.emplace("LENSLongDetectorConstruction", [&]() {detector_construction_ = new LENSLongDetectorConstruction(); });
	dc_map.emplace("CloverSimDetectorConstruction", [&]() {detector_construction_ = new CloverSimDetectorConstruction(); });
	dc_map[yaml_reader.GetString("DetectorConstruction")]();
	
	// Selection of the PhysicsList from Yaml
	std::map<std::string, std::function<void()>> pl_map;
	pl_map.emplace("PhysicsList", [&]() {physics_list_ = new PhysicsList(); });
	pl_map[yaml_reader.GetString("PhysicsList")]();
	
	if (!(detector_construction_)||!(physics_list_)) {
		std::cout << "[RunConfigurator]: failed to configure from the yaml file." << std::endl;
		return 1;
	}

	runManager->SetUserInitialization(detector_construction_);
	runManager->SetUserInitialization(physics_list_);
	runManager->Initialize();
	
	// Selection of the PrimaryGeneratorAction from Yaml
	std::map<std::string, std::function<void()>> pga_map;
	pga_map.emplace("LENSLongPrimaryGeneratorAction", [&]() {primary_generator_action_ = new LENSLongPrimaryGeneratorAction(); });
	pga_map.emplace("PointGammaSourceGeneratorAction", [&]() {primary_generator_action_ = new PointGammaSourceGeneratorAction(); });
	pga_map[yaml_reader.GetString("PrimaryGeneratorAction")]();

	// Selection of the RunAction from Yaml
	std::map<std::string, std::function<void()>> ra_map;
	ra_map.emplace("LENSLongRunAction", [&]() {run_action_ = new LENSLongRunAction(); });
	ra_map[yaml_reader.GetString("RunAction")]();
	
	// Selection of the EventAction from Yaml
	std::map<std::string, std::function<void()>> ea_map;
	ea_map.emplace("LENSLongEventAction", [&]() {event_action_ = new LENSLongEventAction(); });
	ea_map[yaml_reader.GetString("EventAction")]();
	
	if (!(primary_generator_action_)||!(run_action_)||!(event_action_)) {
		std::cout << "[RunConfigurator]: failed to configure from the yaml file." << std::endl;
		return 1;
	}

	runManager->SetUserAction(primary_generator_action_);
	runManager->SetUserAction(run_action_);
	runManager->SetUserAction(event_action_);

	return 0;
}