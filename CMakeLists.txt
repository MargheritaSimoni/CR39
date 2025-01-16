
#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(B4c)


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#NCrystal
execute_process( COMMAND ncrystal-config --show cmakedir
                   OUTPUT_VARIABLE NCrystal_DIR
                   OUTPUT_STRIP_TRAILING_WHITESPACE)

find_package(NCrystal REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#Ncrystal directories

#include_directories(${HDRS_NCG4})
#include_directories(${SRCS_NCG4})

include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

add_executable(exampleB4c exampleB4c.cc ${sources} ${headers})
target_link_libraries(exampleB4c ${Geant4_LIBRARIES})

#NCrystal
#target_link_libraries( exampleB4c NCrystal::NCrystal )
find_package(NCrystal REQUIRED COMPONENTS GEANT4BINDINGS )
target_link_libraries(exampleB4c NCrystal::G4NCrystal)

# Add this line to define the macro USE_GPS to use GeneralParticlaSource instead of ParticleGun
# Define an option to enable or disable GPS support
option(USE_GPS "Enable GPS support" ON) #to disable use : cmake -DUSE_GPS=OFF .. 

# Check if the USE_GPS option is set to ON
if(USE_GPS)
    # Add the preprocessor definition -DUSE_GPS
    add_definitions(-DUSE_GPS)
endif()

#defining my own variable for multithreading, G4MULTITHREAD is defined in geant installation and can't be undefined inside code
option(MY_MULTITHREADED "Enable Geant4 multi-threaded support" ON)

if(MY_MULTITHREADED)
    add_definitions(-DMY_MULTITHREADED)
endif()
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B4c. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB4C_SCRIPTS
  exampleB4c.out
  exampleB4.in
  gui.mac
  init_vis.mac
  plotHisto.C
  plotNtuple.C
  run1.mac
  gps.mac
  vis.mac
  scriptLoop.mac
  sourceMacroMaterials.txt
  )

foreach(_script ${EXAMPLEB4C_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS exampleB4c DESTINATION bin)
