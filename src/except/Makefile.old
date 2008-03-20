# @(#) Example for libcxx exceptions
# Author: Andrew Wingorodov <http://andr.mobi/>
# Licence: LGPLv3
# $Id: Makefile 247 2008-02-08 14:48:36Z wingorodov $


CXXFLAGS+= -I.
HDRS= er.h report.h
TARGET= example

$(TARGET): $(TARGET).cc $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ $(TARGET).cc

clean clear:
	rm -rfv $(TARGET) *.o *.core
