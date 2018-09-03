export ASOFTREF=@CMAKE_BINARY_DIR@/libs/
export FOOTMAIN=@CMAKE_BINARY_DIR@
export FOOTREF=$ASOFTREF/src
export FOOTLEVEL0=$FOOTMAIN/Reconstruction/level0
export FOOTCONFIG=$FOOTLEVEL0/config
export FOOTRES=$FOOTLEVEL0/results
export FOOTSIMU=$FOOTMAIN/Simulation

export LD_LIBRARY_PATH=$ROOTSYS/lib:./:$ASOFTREF/lib:$ASOFTREF/GenFit_build/lib
export DYLD_LIBRARY_PATH=$ROOTSYS/lib:./:$ASOFTREF/lib:$ASOFTREF/GenFit_build/lib
