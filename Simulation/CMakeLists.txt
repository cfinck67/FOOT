# Ch. Finck
#
project (Simulation)
#
MESSAGE("${PROJECT_NAME}:")
#
#----------------------------------------------------------------------------
# Copy foot.inp DoubleDipole.table
#
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/link_FOOT.sh DESTINATION ${CMAKE_BINARY_DIR}/Simulation/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/link_FOOT_mag.sh DESTINATION ${CMAKE_BINARY_DIR}/Simulation/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/foot.inp DESTINATION ${CMAKE_BINARY_DIR}/Simulation/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ROUTINES/  DESTINATION ${CMAKE_BINARY_DIR}/Simulation/ROUTINES)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/TXT2ROOT/  DESTINATION ${CMAKE_BINARY_DIR}/Simulation/TXT2ROOT)

#----------------------------------------------------------------------------
# internal and external dependencies
#
set( EXTRA_INTERNAL_LIBRARIES ${TAG} ${TAVT} ${TAIT} ${TAMSD} ${TABM} ${TATW} ${TACA} ${TATR} ${TAMC} ${TAFO})
set( EXTRA_EXTERNAL_LIBRARIES ${ROOT_LIBRARIES} ${GENFIT_LIBRARIES})


# add includes : root + current + dependencies
#
include_directories(${ROOT_INCLUDE_DIR})
include_directories(${TAG_INCLUDE_DIR})
include_directories(${TATR_INCLUDE_DIR})
include_directories(${TABM_INCLUDE_DIR})
include_directories(${TAVT_INCLUDE_DIR})
include_directories(${TAIT_INCLUDE_DIR})
include_directories(${TAMC_INCLUDE_DIR})
include_directories(${TAMSD_INCLUDE_DIR})
include_directories(${TATW_INCLUDE_DIR})
include_directories(${TACA_INCLUDE_DIR})
include_directories(${TAFO_INCLUDE_DIR})
include_directories(${GENFIT_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#
link_directories(${ROOT_LIBRARY_DIR})
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#----------------------------------------------------------------------------
# Add the executable, and link it to the libraries
#
add_executable(makeGeo MakeGeo.cxx)
#
target_link_libraries(makeGeo ${EXTRA_EXTERNAL_LIBRARIES}  ${EXTRA_INTERNAL_LIBRARIES} )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS makeGeo DESTINATION bin)


#----------------------------------------------------------------------------
# Add the executable, and link it to the libraries
#
add_executable(Txt2Root Txt2Root.cxx)
#
target_link_libraries(Txt2Root ${EXTRA_EXTERNAL_LIBRARIES}  ${EXTRA_INTERNAL_LIBRARIES} )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Txt2Root DESTINATION bin)
