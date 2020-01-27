cmake_minimum_required (VERSION 3.8)
#----------------------------------------------------------------------------
# Setup the project
project ("CloverSim")

# find yaml-cpp package
find_package(yaml-cpp)
set(YAML_CPP_LIB_DIR ${YAML_CPP_CMAKE_DIR}/../../../lib CACHE PATH "yaml-cpp lib dir")

# find Geant4 package
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# find ROOT package.
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Tree)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
#include(${ROOT_USE_FILE})

# Set the default install prefix to [source_dir]/install
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "install dir" FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/PhysicsList/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/LENSLong/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${YAML_CPP_INCLUDE_DIR})
link_directories(${YAML_CPP_LIB_DIR})
link_directories(${PWD}/PhysicsList/source)
link_directories(${PWD}/LENSLong/source)
link_directories(${PWD}/source)
link_directories(${ROOTSYS}/lib)

add_subdirectory(PhysicsList)
add_subdirectory(LENSLong)
add_subdirectory(source)
add_executable (CloverSim "CloverSim.cpp")

target_link_libraries(CloverSim CloverSimLib PhysicsListLib LENSLongLib)

install(TARGETS CloverSim
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

