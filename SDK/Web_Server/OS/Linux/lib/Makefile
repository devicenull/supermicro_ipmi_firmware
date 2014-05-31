ARFLAGS=crv
AR=${CROSS}ar
RANLIB=${CROSS}ranlib

CC=${CROSS}gcc
CFLAGS=-Wall -O2 
OBJS=template.o var.o session_link.o

all:${OBJS} libcom.a

%.o :%.c 
	${CC} -o $@ -c $?  ${CFLAGS}

libcom.a : ${OBJS}
	${AR} ${ARFLAGS} $@  $?
	${RANLIB} $@

clean:
	rm -rf *.o *.a
