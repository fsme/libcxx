INCLUDES = -I/usr/include\
           -I/usr/local/include\
           -I./conv\
           -I./db\
           -I./db/client\
           -I./db/stream\
           -I./dbstream\
           -I./env\
           -I./except\
           -I./fork\
           -I./static\
           -I./timeter\
           -I./logostream\
           -I./md5\
	   -I./threads

SUBDIRS = db/client\
          db/stream\
          dbstream\
          env\
          except\
          fork\
          logostream\
          static\
          timeter

lib_LTLIBRARIES = libcxx.la

libcxx_la_LDFLAGS = -fpic -shared

libcxx_la_SOURCES = env/iron.h\
                    logostream/logostream.hpp\
                    env/iron.cc\
                    logostream/logostream.cc\
                    md5/md5.c\
		    db/client/mc_client.hpp\
		    db/client/mc_client.cc\
		    threads/mutex.cc

libcxx_ladir = $(includedir)/libcxx

libcxx_la_HEADERS = ./db/client/myquery.hpp\
                    ./db/client/mysqlcli.hpp\
                    ./db/client/mc_client.hpp\
                    ./db/client/db_cli.hpp\
                    ./db/client/selquery.hpp\
                    ./env/ment.hpp\
                    ./env/iron.h\
                    ./except/er.h\
                    ./except/report.h\
                    ./logostream/logostream.hpp\
                    ./static/singleton.hpp\
                    ./timeter/timeter.h\
                    ./md5/md5.h\
		    ./threads/mutex.h\
		    ./threads/event.hpp\
		    ./threads/semaphore.hpp\
		    ./threads/thread.hpp
