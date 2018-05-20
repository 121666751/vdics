INCDIR = -I../depends/include -I../usemodel/include -I../kernels/include  -I../include -I/usr/include/libxml2 -I../common
LIBDIR = -L../lib -L../kernels/lib -L../depends/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib
FLAG = -D__OS_LINUX_VER -D_GNU_SOURCE  -fPIC -O3 $(INCDIR) 
LIB = -ldl -lpthread -lstdc++ -lmysqlclient
GCC = gcc
.SUFFIXES: .cpp  .o

PUBLIB = libgsipdb.so

OBJS =  db_common.o

all:  $(PUBLIB) 

$(PUBLIB): $(OBJS)
	@echo
	@echo "Compile Targets......"
	$(GCC) -shared -g -o $@ $(OBJS) $(LIBDIR) $(LIB)
	
.cpp.o:
	$(GCC) -c $(FLAG) $<
	
.c.o:
	$(GCC) -c $(FLAG) $<	
	
clean:
	@echo
	@echo "Removing object Files......"
	rm *.o *.so
	rm $@

install:
	@echo "Copying Files "
	cp libgsipdb.so ../bin/.
	cp libgsipdb.so ../lib/.
	cp *.h ../include/.

# -- end --
