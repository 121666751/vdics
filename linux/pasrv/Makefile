INCDIR = -I../depends/include -I../usemodel/include -I../kernels/include -I/usr/local/include/cc++2/cc++  -I../include -I/usr/include/libxml2 -I../common
LIBDIR = -L../lib -L../kernels/lib -L../depends/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib  -L../usemodel/lib
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE   -O3 $(INCDIR)
GCC = gcc
LIB = -ldl -lpthread -lstdc++ -lmysqlclient -lgsipdb -lidspub -ltilimgproc.1.0.0  -lcvaux -lhighgui -lcxcore -lcv -lTC_Bocom_PatternAnalysis -lTC_E13bOCR -lTC_NanJingBocom_CheckOCR -lTC_PrnOCR -ljasper -lml -lTC_ChnPrnOCR -lTC_BarCode128.1.1.0 -lTilImageLib.3.3.0
.SUFFIXES: .cpp  .o

DBNAME = gsipdb

TASKSRV = pasrv

OBJS =  authinput_main.o authinput.o ConfigFile.o gsip_util.o package.o PubFun.o

all:  $(TASKSRV) 

$(TASKSRV): $(OBJS)
	@echo
	@echo "Compile Targets......"
	$(GCC) -o $@ $(OBJS) $(LIBDIR) $(LIB)

.cpp.o:
	$(GCC) -c $(FLAG) $<

ConfigFile.o:
	$(GCC) -c $(FLAG) ../common/ConfigFile.cpp

package.o:
	$(GCC) -c $(FLAG) ../common/package.cpp

gsip_util.o:
	$(GCC) -c $(FLAG) ../common/gsip_util.cpp

PubFun.o:
	$(GCC) -c $(FLAG) ../common/PubFun.cpp

.c.o:
	$(GCC) -c $(FLAG) $<	
	
clean:
	@echo
	@echo "Removing object Files......"
	-rm *.o
	-rm pasrv

install:
	@echo "Copying Files "
	cp pasrv ../bin/.
	cp *.h ../include

# -- end --
