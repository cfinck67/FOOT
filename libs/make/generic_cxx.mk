#
# Compile options
#
# -- handle C
#   -O      optimize
#   -fPIC   position independent code
#   -Wall   all warnings
#
#   -g      request debugging info
#
ifdef CCCOMMAND
CC = $(CCCOMMAND)
endif
ifndef CCOPTFLAGS
CCOPTFLAGS = -O
endif
#
CC         = gcc
CFLAGS     = -fPIC -Wall $(CCOPTFLAGS) $(INCLFLAGS)
#
# -- handle C++
#
#   -O      optimize
#   -fPIC   position independent code
#   -Wall   all warnings
#
#   -g      request debugging info
#
# Note: size of a lib:     (no -g) : 60k;   -g2 : 385k  ; -g3 : 385k
#
ifdef  CXXCOMMAND
CXX = $(CXXCOMMAND)
endif
#
ifndef CXXOPTFLAGS
CXXOPTFLAGS = -O2
endif
#
CXXFLAGS   = -fPIC -Wall -Wno-deprecated $(CXXOPTFLAGS) $(INCLFLAGS)
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
#
LINK.o     = $(CXX) $(CXXOPTFLAGS) $(LDOPTFLAGS) $(LDFLAGS) $(TARGET_ARCH)
LDFLAGS    = -g
#
# Compile rule
#
%.o: %.cxx
	$(COMPILE.cc) $< $(OUTPUT_OPTION)
#
