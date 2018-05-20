INCDIR = -I../depends/include -I../usemodel/include -I../kernels/include -I/usr/local/include/cc++2/cc++  -I../include -I/usr/include/libxml2 -I../common
LIBDIR = -L../usemodel/lib -L../lib -L../kernels/lib -L../depends/lib -L/usr/lib64 -L/usr/lib 
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE   -O3 $(INCDIR)
GCC = gcc
LIB = -ldl -lpthread -lstdc++ -lmysqlclient -lgsipdb -lidspub -ltilimgproc.1.0.0  -lcvaux -lhighgui -lcxcore -lcv -lTC_Bocom_PatternAnalysis -lTC_E13bOCR -lTC_NanJingBocom_CheckOCR -lTC_PrnOCR -ljasper -lml -lTilImageLib.3.3.0 -lTC_BarCode128.1.1.0
.SUFFIXES: .cpp  .o

DBNAME = gsipdb

SERVER = pvsrv

OBJS =  vouchers_main.o vouchers.o ConfigFile.o gsip_util.o package.o PubFun.o

all:  $(SERVER) 

$(SERVER): $(OBJS)
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
	rm pvsrv

install:
	@echo "Copying Files "
	cp pvsrv ../bin/.
	cp *.h  ../include

# -- end --
