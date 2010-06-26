# @(#) Build test for DB clients
# Author: Andrew Wingorodov <http://andr.mobi/>
# $Id: Makefile 247 2008-02-08 14:48:36Z wingorodov $

.PHONY: all clean clear

PREFIX?= /usr/local
MYSQL?= $(PREFIX)

CXXFLAGS+= \
		-I.\
		-I/usr/local/include\
		-I/usr/local/include/libcxx\
		-I../../static\
		-L/usr/local/lib/mysql\
		-L/usr/local/lib\

LDLIBS+=\
	-lmemcache\
	-lcxx\
	-lmd\
	-lmysqlclient\

OBJS= mras.o normal.o mime_parser.o
HEADERS= mc_client.hpp

SRC= example.cc
TARGET= example
SQLTEST= sqltest
MCEXAMPLE= mc_example
DB_TEST= db_test

TEST= $(DB_TEST) $(SQLTEST) $(MCEXAMPLE)
CORE= *.core

all:  $(DB_TEST) $(SQLTEST) $(MCEXAMPLE)

$(DB_TEST): $(DB_TEST).cc db_cli.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(DB_TEST).cc

$(MC_EXAMPLE): $(MC_EXAMPLE).cc mc_client.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(MC_EXAMPLE).cc

clean clear:
	rm -rfv $(TARGET) $(TEST) $(LIBA) $(LIB) $(OBJS) $(CORE) *dia

