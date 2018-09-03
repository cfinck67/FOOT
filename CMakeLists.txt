# CMake for FOOT
# Ch. Finck

#------------------------------------------------------------------------------
# Check cmake version is compatible
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_MACOSX_RPATH 0)
#------------------------------------------------------------------------------
# Project Name 
#
project (FOOT)
#
MESSAGE("${PROJECT_NAME}:")

#------------------------------------------------------------------------------
# Enforce an out-of-source builds before anything else
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "FOOT requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

#----------------------------------------------------------------------------
# Add specific modules
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/libs/cmake/Modules/")

MESSAGE(" --> Including cmake additional path: ${CMAKE_MODULE_PATH}")

#----------------------------------------------------------------------------
# Include macro for root
#
include(RootNewMacros)

#----------------------------------------------------------------------------
# Try and Find Root
#
find_package(ROOT)
if (ROOT_FOUND)
   # add ROOT includes and libs
      include_directories(${ROOT_INCLUDE_DIR})
   else()
      MESSAGE(" --> root has NOT been found")
endif()

#----------------------------------------------------------------------------
# Copy setup files
#
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setupFOOT.sh.cmake" "${CMAKE_BINARY_DIR}/setupFOOT.sh"  @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setupFOOT.csh.cmake" "${CMAKE_BINARY_DIR}/setupFOOT.csh"  @ONLY)


#----------------------------------------------------------------------------
# Set the output paths
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs/lib)

#----------------------------------------------------------------------------
# Set src libraries name
#
Set(TAG   "TAGbase")
Set(TAIR  "TATRbase")
Set(TAIR  "TAIRbase")
Set(TABM  "TABMbase")
Set(TAVT  "TAVTbase")
Set(TAIT  "TAITbase")
Set(TAMSD "TAMSDbase")
Set(TATW  "TATWbase")
Set(TACA  "TACAbase")
Set(TAMC  "TAGmclib")
Set(TAFO  "TAGfoot")
add_definitions(-std=c++11)

#----------------------------------------------------------------------------
# Set src includes
#
set(TAG_INCLUDE_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TAG}")
set(TAIR_INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TATR}")
set(TAIR_INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TAIR}")
set(TABM_INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TABM}")
set(TAVT_INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TAVT}")
set(TAIT_INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TAIT}")
set(TAMSD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TAMSD}")
set(TATW_INCLUDE_DIR " ${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TATW}")
set(TACA_INCLUDE_DIR " ${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TACA}")
set(TAMC_INCLUDE_DIR " ${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TAMC}")
set(TAFO_INCLUDE_DIR " ${CMAKE_CURRENT_SOURCE_DIR}/libs/src/${TAFO}")

#----------------------------------------------------------------------------
# GenFit
#
set ( GENFIT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/GenFit/fitters/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/GenFit/trackReps/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/GenFit/measurements/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/GenFit/core/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/GenFit/utilities/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/GenFit/fields/include )

set ( GENFIT_LIBRARY_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/libs/GenFit_build/lib) # put it by hand cmake of GenFit not compatible
set ( GENFIT_LIBRARIES   genfit2)


#----------------------------------------------------------------------------
# Globals 
#
add_subdirectory(libs)
add_subdirectory(Reconstruction)
add_subdirectory(Simulation)
