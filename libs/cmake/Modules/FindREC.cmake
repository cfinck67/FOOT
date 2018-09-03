# - Finds REC instalation
# This module sets up REC information 
# It defines:
# REC_FOUND          If the REC is found
# REC_INCLUDE_DIR    PATH to the include directory
# REC_LIBRARIES      Most common libraries
# REC_LIBRARY_DIR    PATH to the library directory 

# check that library path for rec exist

set(BUILD_REC "${CMAKE_BINARY_DIR}/rec")
set(QAPIVI_REC "${CMAKE_SOURCE_DIR}/rec")
set(REC_INCLUDE_DIR ${QAPIVI_REC}/TAGbase ${QAPIVI_REC}/TAIRbase ${QAPIVI_REC}/TABMbase ${QAPIVI_REC}/TAVTbase ${QAPIVI_REC}/TAITbase ${QAPIVI_REC}/TAEMbase ${QAPIVI_REC}/TAMSDbase ${QAPIVI_REC}/TACAbase ${QAPIVI_REC}/TATWbase)


set(REC_LIBRARIES -lTAGbase -lTAGpxi -lTAIRbase -lTABMbase -lTAVTbase -lTAITbase -lTAEMbase -lTAMSDbase -lTACAbase -lTATWbase -L${BUILD_REC}/lib)
set(REC_LIBRARY_DIR ${QAPIVI_REC}/lib)

set(REC_GEOMAPS_DIR ${QAPIVI_REC}/geomaps)
set(REC_CONFIG_DIR  ${QAPIVI_REC}/config)
  
if(NOT REC_FIND_QUIETLY)
  message(STATUS "Found REC in ${QAPIVI_REC}")
 endif()
 
set(REC_FOUND TRUE)


