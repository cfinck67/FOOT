# CMake for Qapivi_rec
# Ch. Finck

#------------------------------------------------------------------------------
# Check cmake version is compatible
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#----------------------------------------------------------------------------
# Setup the project
#
#
MESSAGE("  src:")

#----------------------------------------------------------------------------
# Add sub directories
#
add_subdirectory(TAGdaq)
add_subdirectory(TAGdaqApi)
add_subdirectory(TAGbase)
add_subdirectory(TATRbase)
add_subdirectory(TAIRbase)
add_subdirectory(TABMbase)
add_subdirectory(TAVTbase)
add_subdirectory(TAITbase)
add_subdirectory(TAMSDbase)
add_subdirectory(TAMCbase)
add_subdirectory(TATWbase)
add_subdirectory(TACAbase)
add_subdirectory(TAGfoot)

