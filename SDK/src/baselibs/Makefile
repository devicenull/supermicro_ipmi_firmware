# Copy over all the standard libraries necessary.  These are provided by the cross-compile toolkit, and are not 
# built from seperate packages.  You'd need to recompile gcc/glibc in order to udpate these
all:
	cp -f $(TC_LOCAL)/lib/ld-linux.so.2 $(FS_PATH)/lib/
	cp -f $(TC_LOCAL)/lib/libcrypt.so.1 $(FS_PATH)/lib/
	cp -f $(TC_LOCAL)/lib/libc.so.6 $(FS_PATH)/lib/

