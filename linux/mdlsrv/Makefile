INCDIR = -I../depends/include -I../usemodel/include -I../kernels/include -I/usr/local/include/cc++2/cc++  -I../include -I/usr/include/libxml2 -I../common
LIBDIR = -L../usemodel/lib -L../lib -L../kernels/lib -L../depends/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib 
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE   -O3 $(INCDIR)
GCC = gcc
LIB = -ldl -lpthread -lstdc++ -lmysqlclient -lidspub -ltilimgproc.1.0.0  -lcvaux -lhighgui -lcxcore -lcv -lTC_Bocom_PatternAnalysis -lTC_E13bOCR -lTC_NanJingBocom_CheckOCR -lTC_PrnOCR -ljasper -lml
.SUFFIXES: .cpp  .o

SERVER = mdlsrv

OBJS =  mdlsrv_main.o mdlsrv.o 

all:  $(SERVER) 

$(SERVER): $(OBJS)
	@echo
	@echo "Compile Targets......"
	$(GCC) -o $@ $(OBJS) $(LIBDIR) $(LIB)

.cpp.o:
	$(GCC) -c $(FLAG) $<

.c.o:
	$(GCC) -c $(FLAG) $<	
	
clean:
	@echo
	@echo "Removing object Files......"
	rm *.o
	rm pvsrv

install:
	@echo "Copying Files "
	cp mdlsrv mdl.txt ../bin/.
	cp *.h  ../include

# -- end --
