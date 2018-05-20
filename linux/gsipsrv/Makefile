INCDIR = -I../depends/include -I../usemodel/include -I../kernels/include -I../include -I/usr/include/libxml2
LIBDIR = -L../lib -L../usemodel/lib -L../depends/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE   -O3 $(INCDIR)
GCC = gcc
LIB = -ldl -lpthread -lstdc++ -lidspub  -ltilimgproc.1.0.0
.SUFFIXES: .cpp  .o

	
GSIPSRV = gsipsrv

OBJS =  gsipsrv.o PubFun.o ConfigFile.o

all:  $(GSIPSRV) 

$(GSIPSRV): $(OBJS)
	@echo
	@echo "Compile Targets......"
	$(GCC) -o $@ $(OBJS) $(LIBDIR) $(LIB)

.c.o:
	$(GCC) -c $(FLAG) $<

.cpp.o:
	$(GCC) -c $(FLAG) $<
PubFun.o:
	$(GCC) -c $(FLAG) ../common/PubFun.cpp
	
ConfigFile.o:
	$(GCC) -c $(FLAG) ../common/ConfigFile.cpp

clean:
	@echo
	@echo "Removing object Files......"
	rm *.o
	rm gsipsrv

install:
	@echo "Copying Files "
	cp gsipsrv ../bin/.

# -- end --
