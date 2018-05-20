INCDIR = -I../depends/include -I../usemodel/include -I../kernels/include -I/usr/local/include/cc++2/cc++  -I../include -I/usr/include/libxml2 -I../common
LIBDIR = -L../lib -L../kernels/lib -L../depends/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib -L../usemodel/lib
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE   -O3 $(INCDIR)
GCC = gcc
LIB = -ldl -lpthread -lstdc++ -lmysqlclient -lgsipdb -lidspub -ltilimgproc.1.0.0
.SUFFIXES: .cpp  .o

DBNAME = gsipdb

SERVER = smsrv

OBJS =  main.o PubFun.o SendMultis.o Dboperate.o ConfigFile.o

all:  $(SERVER) 

$(SERVER): $(OBJS)
	@echo
	@echo "Compile Targets......"
	$(GCC) -o $@ $(OBJS) $(LIBDIR) $(LIB)

.cpp.o:
	$(GCC) -c $(FLAG) $<
PubFun.o:
	$(GCC) -c $(FLAG) ../common/PubFun.cpp
	
ConfigFile.o:
	$(GCC) -c $(FLAG) ../common/ConfigFile.cpp
	
Dboperate.o:
	$(GCC) -c $(FLAG) ../dbolayer/Dboperate.cpp
.c.o:
	$(GCC) -c $(FLAG) $<	
	
clean:
	@echo
	@echo "Removing object Files......"
	rm *.o
	rm smsrv

install:
	@echo "Copying Files "
	cp smsrv ../bin/.
	cp *.h ../include/.

# -- end --
