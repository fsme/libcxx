# @(#) Build EnvIron ment
# @(#) Licence: LGPLv3
# @(#) Author Andrew Wingorodov <http://andr.ru/>
# $Id: Makefile,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

CXXFLAGS+= -I.\
		   -I../fork\
		   -I../err\
		   -I../logostream\
		   -I../libcxx

TARGET= example
OBJS= example.o iron.o ../logostream/logostream.o

include /etc/default/mk

../logostream/logostream.o:
	make -C ../logostream logostream.o

clean clear:
	rm -rfv $(TARGET) *.o *.core
