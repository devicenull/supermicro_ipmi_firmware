RS_COMPILE="$(CROSS_COMPILE)"

CBPREFIX = /SFCB
CB_SRC = $(PRJ_PATH)/SFCB
CBEXECPREFIX =  $(CBPREFIX)/local
CBINCDIR = $(CB_SRC)/sblim-sfcb-1.3.2
CBLIBDIR = $(CBEXECPREFIX)/lib
INSTALLDIR = $(CBEXECPREFIX)/lib/cmpi

SONAME = libcmpiIPMI_LogRecordProvider.so
LIBNAME = libcmpiIPMI_LogRecordProvider.so.4
TARGET = $(SONAME)

#ifdef (DEBUG)
DEBUGFLAGS = -g -DDEBUG
#else
#DEBUGFLAGS =
#endif

DEFS = -D_REENTRANT
LIBS = -lpthread -L$(CBLIBDIR)

LIB += -lmulti

INCLUDES = -I$(CBINCDIR)
CXX = $(CROSS_COMPILE)gcc
CXXFLAGS = $(DEBUGFLAGS) $(INCLUDES) $(DEFS) -Wall -fPIC

SRCS = \
cmpiIPMI_LogRecordProvider.c \
cmpiIPMI_LogRecord.c \
IPMI_LogRecord.c \
Host/$(MYMCU)/Board/$(BOARD)/LogRecordCallbackFunction.c


#LIBS +=  -lipmi -lchnl  -lm -lsol  -lutility -lnvram  -lipmicrypt -lcommon -lsys -lrt -luart -lupdate -llan -lmqueue -lgpio -lxml2
#LIBS +=  -lipmi -lchnl  -lm -lsol  -lutility -lnvram  -lipmicrypt -lcommon -lsys -lrt -luart -lupdate -llan -lmqueue -lgpio -lxml2 -lwsman_account_handler
#LDFLAGS = $(LIBS) -L$(PRJ_PATH)/lib

OBJS = $(addsuffix .o, $(basename $(SRCS)))

.SUFFIXES : .o .c

all:      $(LIBNAME)

$(LIBNAME):     $(OBJS)
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(LIBNAME) $(OBJS) $(LDFLAGS)
	$(STRIP) $(LIBNAME)
	ln -s $(LIBNAME) $(SONAME)

.c.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@