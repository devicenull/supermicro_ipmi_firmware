SMASHDIR = $(PRJ_PATH)/SMASH/SMASH
CCPREFIX = ../../SFCC/sfcc
CB_SRC = $(PRJ_PATH)/SFCB
CCEXECPREFIX =  $(CCPREFIX)/local
CCINCDIR1 = $(CCEXECPREFIX)/include/CimClientLib
CCINCDIR2 = $(CCEXECPREFIX)/include/cimc
CCLIBDIR = $(CCEXECPREFIX)/lib
CURLLIBDIR = $(PRJ_PATH)/SFCC/curl/local/lib
INSTALLDIR = $(SMASHDIR)/lib

SONAME = libSFCC_API.so
LIBNAME = libSFCC_API.so.4
TARGET = $(SONAME)

DEBUGFLAGS =  -DDEBUG

LIBS = -L$(CCLIBDIR) -lpthread -lcmpisfcc -lcimcclient 
CXXFLAGS =  -I$(CCINCDIR1) -I$(CCINCDIR2)  -D_REENTRANT -Wall -fPIC

LDFLAGS = $(LIBS) 
OBJS = SFCC_API.o show.o

all : $(OBJS)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $(LIBNAME) $(OBJS) $(LDFLAGS)
	cp $(LIBNAME) $(INSTALLDIR);
	cd $(INSTALLDIR);if [ ! -f $(SONAME) ];then ln -s $(LIBNAME) $(SONAME);fi 

.c.o:
	$(CC) $(CXXFLAGS) -c $< -o $@ 
