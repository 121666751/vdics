INCDIR = -I../common -I../depends/include -I../usemodel/include -I../kernels/include -I/usr/local/include/cc++2/cc++  -I../include -I/usr/include/libxml2
LIBDIR = -L../lib -L../usemodel/lib -L../depends/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE   -O3 $(INCDIR)
GCC = gcc
 LIB = -ldl -lpthread -lstdc++ -lmysqlclient -lgsipdb -lidspub -ltilimgproc.1.0.0
.SUFFIXES: .cpp  .o

DBNAME = gsipdb

SendVoucherSRV = svsrv

OBJS =  main.o PubFun.o SendVouchers.o Dboperate.o ConfigFile.o

all:  $(SendVoucherSRV) 

$(SendVoucherSRV): $(OBJS)
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
	rm svsrv

install:
	@echo "Copying Files "
	cp svsrv ../bin/.
	cp *.h ../include/.

# -- end --
