#cmake version required
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#project name
project(Simulation)

#this should happen automatically if G4 is correctly sourced
#also add vis and ui packages
find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

#include all the .cc and .hh files
#this directory is the current folder
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

#name of the executable, built from the 3 other variables
add_executable(sim sim.cc ${sources} ${headers})
#find the rest of the libraries
target_link_libraries(sim ${Geant4_LIBRARIES})

#project name, DEPENDS, name of executable
add_custom_target(Simulation DEPENDS sim)
