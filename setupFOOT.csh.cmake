setenv ASOFTREF @CMAKE_BINARY_DIR@/libs/
setenv FOOTMAIN @CMAKE_BINARY_DIR@
setenv FOOTREF $ASOFTREF/src
setenv FOOTLEVEL0 $FOOTMAIN/Reconstruction/level0
setenv FOOTCONFIG $FOOTLEVEL0/config
setenv FOOTRES $FOOTLEVEL0/results
setenv FOOTSIMU $FOOTMAIN/Simulation

setenv LD_LIBRARY_PATH=$ROOTSYS/lib:./:$ASOFTREF/lib:$ASOFTREF/GenFit_build/lib
setenv DYLD_LIBRARY_PATH=$ROOTSYS/lib:./:$ASOFTREF/lib:$ASOFTREF/GenFit_build/lib