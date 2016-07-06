#
# Dependency generation rules
#
%.d: %.c
	@ echo "$(CC) -MM  $< | sed ... > $@"
	@ $(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< \
		| sed '\''s/\($*\.o\)[ :]*/\1 $@ : /g'\'' > $@'
%.d: %.cxx
	@ echo "$(CXX) -MM  $< | sed ... > $@"
	@ $(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< \
		| sed '\''s/\($*\.o\)[ :]*/\1 $@ : /g'\'' > $@'
#
# Note: moving the `include $(DEP_all)' here didn't work !
#       building of an .so failed, only the first .o was created !
#
