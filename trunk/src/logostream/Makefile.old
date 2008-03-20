# @(#) Build log stream
# @(#) Author Andrew Wingorodov <http://andr.ru/>
# $Id: Makefile 247 2008-02-08 14:48:36Z wingorodov $

.PHONY: all clean clear

#CXX=g++33
CXXFLAGS+= -Wno-deprecated -I.

TARGET=example
OBJS= example.o logostream.o

all: $(TARGET)

logostream.o: logostream.cc logostream.hpp
	$(CXX) $(CXXFLAGS) -c logostream.cc

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean clear:
	rm -rfv $(TARGET) $(OBJS) *.core
