This is the source code to the SuperMicro IPMI Firmware. Apparently, not all of this is open source, so some of the files are provided as binaries.

This was originally extracted from ftp://ftp.supermicro.com/GPL/SMT/SDK_SMT_X9_317.tar.gz

I removed the SDK/ToolChain/Host/x86_64 directory. This was apparently for building this against an x86_64 target, which doesn't seem to be a useful thing.

## Compiling on a Debian 7 x86_64 Machine

The cross compiler provided is a i386 binary, so you need to install the necessary i386 libs:

    dpkg --add-architecture i386
    aptitude update
    aptitude install ia32-libs

## Subversion

Whoever packaged this didn't bother to remove their .svn directories. These refereneced the following svn server:

    svn://66.120.31.113/ATEN_X9/Branch/STD_X9_237_Security_X10/

Sadly, this server is down (and would probably require authentication anyway). I've removed all the .svn content.

## Building

This is pretty self contained, so there aren't really a whole lot of requirements for the host machine itself.  You can build everything via:

    cd SDK
    make

