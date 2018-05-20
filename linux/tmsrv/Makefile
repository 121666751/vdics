INCDIR = -I../depends/include  -I../kernels/include -I../include -I/usr/include/libxml2 -I../common -I../usemodel/include
LIBDIR = -L ../usemodel/lib -L../lib -L../kernels/lib -L../depends/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib 
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE   -O3 $(INCDIR)
GCC = gcc
LIB = -ldl -lz -lpthread -lstdc++ -lmysqlclient -lidspub -lgsipdb -ltilimgproc.1.0.0 
.SUFFIXES: .cpp  .o

DBNAME = gsipdb

TASKSRV = tmsrv

OBJS =  main.o TaskManager.o PbcFile.o ConfigFile.o gsip_util.o package.o PubFun.o encrypt.o uncrypt.o

all:  $(TASKSRV) 

$(TASKSRV): $(OBJS)
	@echo
	@echo "Compile Targets......"
	$(GCC) -o $@ $(OBJS) $(LIBDIR) $(LIB)

.cpp.o:
	$(GCC) -c $(FLAG) $<

package.o:
	$(GCC) -c $(FLAG) ../common/package.cpp

gsip_util.o:
	$(GCC) -c $(FLAG) ../common/gsip_util.cpp

ConfigFile.o:
	$(GCC) -c $(FLAG) ../common/ConfigFile.cpp

PubFun.o:
	$(GCC) -c $(FLAG) ../common/PubFun.cpp

.c.o:
	$(GCC) -c $(FLAG) $<	
	
clean:
	@echo
	@echo "Removing object Files......"
	rm *.o
	rm tmsrv

install:
	@echo "Copying Files "
	cp tmsrv ../bin/.
	cp *.h ../include

# -- end --
