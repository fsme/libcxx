# @(#) Example for err::no
# @(#) Author Andrew Wingorodov <http://andr.ru/>
# $Id: Makefile 247 2008-02-08 14:48:36Z wingorodov $


CXXFLAGS+= -I.
HDRS= singleton.hpp
TARGET= example

$(TARGET): $(TARGET).cc $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ $(TARGET).cc

clean clear:
	rm -rfv $(TARGET) *.o *.core
