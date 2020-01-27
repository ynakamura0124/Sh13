add_library(CloverSimLib SHARED
	YamlParameter.cpp
	YamlReader.cpp
	GSStateG4.cpp
	GSource4G4.cpp
	CloverSimDetectorConstruction.cc
	PointGammaSourceGeneratorAction.cpp
	RunConfigurator.cpp
)

target_link_libraries(CloverSimLib ${YAML_CPP_LIBRARIES} ${ROOT_LIBRARIES} ${Geant4_LIBRARIES})

install(TARGETS CloverSimLib DESTINATION lib)