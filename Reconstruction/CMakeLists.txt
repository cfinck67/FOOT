# CMake for FOOT
# Ch. Finck

#------------------------------------------------------------------------------
# Project Name 
#
project (Reconstruction)
#
MESSAGE("${PROJECT_NAME}:")

#----------------------------------------------------------------------------
# Globals 
#
add_subdirectory(level0)
add_subdirectory(fullrec)
