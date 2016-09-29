TOPDIR:=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))/
CFLAGS=-O3 -g
CFLAGS+= -I$(TOPDIR)include -I$(TOPDIR)../../cybozulib/include/ -I$(TOPDIR)../../mcl/include -I$(TOPDIR)../../xbyak/ -I$(TOPDIR)../include/ -I$(TOPDIR)../../PBWT-sec/include
LDFLAGS+= -lgmp -lgmpxx -lcrypto -lmcl -L../../mcl/lib/
ROT_OBJ=../../PBWT-sec/src/rot.o
COMM_OBJ=../../PBWT-sec/src/comm.o
SERVER_OBJS=$(ROT_OBJ) sjac.o $(COMM_OBJ) server.o
CLIENT_OBJS=$(ROT_OBJ) sjac.o $(COMM_OBJ) client.o
JACCARD_OBJS=$(ROT_OBJ) sjac.o jaccard_main.o
MCL_LIB=../../mcl/lib/libmcl.a

all: pbwt_sec jaccard_main server client

pbwt_sec:
	make -C ../../PBWT-sec/src/

clean:
	rm -rf jaccard server client$(JACCARD_OBJS) $(SERVER_OBJS) $(CLIENT_OBJS) *~

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) -c $< -o $@ $(CFLAGS) -fopenmp

server: $(SERVER_OBJS) $(MCL_LIB)
	$(CXX) $(SERVER_OBJS) -o $@ $(LDFLAGS) -fopenmp
	mv server $(TOPDIR)../bin/
	mkdir -p $(TOPDIR)../comm/
	mkdir -p $(TOPDIR)../comm/server
	mkdir -p $(TOPDIR)../comm/client

client: $(CLIENT_OBJS) $(MCL_LIB)
	$(CXX) $(CLIENT_OBJS) -o $@ $(LDFLAGS) -fopenmp
	mv client $(TOPDIR)../bin/
	mkdir -p $(TOPDIR)../comm/
	mkdir -p $(TOPDIR)../comm/server
	mkdir -p $(TOPDIR)../comm/client

jaccard_main: $(JACCARD_OBJS) $(MCL_LIB)
	$(CXX) $(JACCARD_OBJS) -o $@ $(LDFLAGS) -fopenmp
	mkdir -p $(TOPDIR)../bin/
	mv jaccard_main $(TOPDIR)../bin/

test: $(TEST_OBJS) $(MCL_LIB)
	$(CXX) $(TEST_OBJS) -o $@ $(LDFLAGS) -fopenmp
	mkdir -p $(TOPDIR)../bin/
	mv test $(TOPDIR)../bin/
