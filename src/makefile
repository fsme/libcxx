# @(#) Build CXX library for FreeBSD
# Author Andrew Wingorodov <http://andr.mobi/>
# Licence: LGPLv3
# $Id$

CXXFLAGS+=\
	-I./db/client\
	-I./env\
	-I./except\
	-I./logostream\
	-I./static\
	-I./timeter\

LIBFLAGS= -fpic -shared
OBJS=\
	logostream.o\
	iron.o\

LIBA= libcxx.a
LIBSO= libcxx.so

#
$(LIBSO): $(LIBA) 
	$(CXX) $(LIBFLAGS) $(CXXFLAGS) $(OBJS) -o $@
#
$(LIBA): $(OBJS)
	ar rv $@ $?
	chmod 444 $@
	: $@

# OBJS
LOGOSTREAM_SRC=\
	./logostream/logostream.hpp\
	./logostream/logostream.cc\
	
logostream.o: $(LOGOSTREAM_SRC)
	$(CXX) $(CXXFLAGS) -c ./logostream/logostream.cc

IRON_SRC=\
	./except/er.h\
	./env/ment.hpp\
	./env/iron.h\
	./env/iron.cc\

iron.o: $(IRON_SRC)
	$(CXX) $(CXXFLAGS) -c ./env/iron.cc

clean clear:
	@rm -rfv $(LIBSO) $(LIBA) *.o 
	make -C logostream clean
	make -C env clean
	make -C except clean
	make -C timeter clean
	make -C static clean
	make -C db/client clean
