# @(#) Build application CXX library
# @(#) Licence: LGPLv3
# @(#) Author Andrew Wingorodov <http://andr.ru/>
# $Id: Makefile,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

ver= 0.0
DISTR= libcxx-$(ver)
dirs= usr usr/include usr/lib usr/include/libcxx

all: $(DISTR).tar.gz

include /etc/default/mk

CXXFLAGS+= -I./usr/include/libcxx
OBJS=\
	logostream.o\
	iron.o\
	args.o\
	proc.o\
	fork.o

LIBCXX= usr/lib/libcxx.a
HDIR= usr/include/libcxx

HDS=$(HDIR)/logostream.h\
	$(HDIR)/args.h\
	$(HDIR)/fork.h\
	$(HDIR)/proc.h\
	$(HDIR)/localtime.h\
	$(HDIR)/ment.h\
	$(HDIR)/iron.h\
	$(HDIR)/err.h\
	$(HDIR)/libcxx.h

$(DISTR).tar.gz: $(dirs) $(HDS) $(LIBCXX)
	tar cfv $(DISTR).tar usr/
	gzip -9 -f $(DISTR).tar 
	chmod 444 $@
	@touch $@
#
$(LIBCXX): $(OBJS)
	ar rv $@ $?
	chmod 444 $@
	@touch $@
	: $@

# HEADERS
$(HDIR)/libcxx.h: libcxx.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/logostream.h: ../logostream/logostream.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/fork.h: ../fork/fork.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/proc.h: ../fork/proc.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/args.h: ../fork/args.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/localtime.h: ../local/localtime.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/err.h: ../err/err.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/iron.h: ../env/iron.h
	@cp -vp $? $@
	chmod 644 $@
$(HDIR)/ment.h: ../env/ment.h
	@cp -vp $? $@
	chmod 644 $@

# OBJECTS
logostream.o: $(HDIR)/logostream.h ../logostream/logostream.cc
	$(CXX) $(CXXFLAGS) -c ../logostream/logostream.cc
	@touch $@

iron.o: $(HDIR)/err.h $(HDIR)/ment.h $(HDIR)/iron.h ../env/iron.cc
	$(CXX) $(CXXFLAGS) -c ../env/iron.cc
	@touch $@

args.o: $(HDIR)/args.h ../fork/args.cc
	$(CXX) $(CXXFLAGS) -c ../fork/args.cc
	@touch $@

proc.o: args.o $(HDIR)/err.h $(HDIR)/proc.h ../fork/proc.cc
	$(CXX) $(CXXFLAGS) -c ../fork/proc.cc
	@touch $@

fork.o: proc.o $(HDIR)/fork.h ../fork/fork.cc
	$(CXX) $(CXXFLAGS) -c ../fork/fork.cc
	@touch $@

clean clear:
	@rm -rfv $(LIBCXX) $(HDS) $(dirs) *.o *.err *.tar *.gz
