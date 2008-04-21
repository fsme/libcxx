# @(#) Build env::iron ment
# @(#) Licence: LGPLv3
# @(#) Author Andrew Wingorodov <http://andr.ru/>
# $Id: Makefile 247 2008-02-08 14:48:36Z wingorodov $

#CXX=g++33
CXXFLAGS+= -I.\
		   -I../except\
		   -I../logostream\

TARGET= example
OBJS= example.o iron.o ../logostream/logostream.o
HEADERS= iron.h ment.hpp

all: $(TARGET)

$(TARGET): $(HEADERS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

../logostream/logostream.o:
	make -C ../logostream ../logostream/logostream.o

clean clear:
	rm -rfv $(TARGET) *.o ../logostream/logostream.o *.core
