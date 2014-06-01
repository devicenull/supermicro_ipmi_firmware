This is the source code to the SuperMicro IPMI Firmware. Apparently, not all of this is open source, so some of the files are provided as binaries.

This was originally extracted from ftp://ftp.supermicro.com/GPL/SMT/SDK_SMT_X9_317.tar.gz

I removed the SDK/ToolChain/Host/x86_64 directory. This was apparently for building this against an x86_64 target, which doesn't seem to be a useful thing.

## Compiling on a Debian 7 x86_64 Machine

The cross compiler provided is a i386 binary, so you need to install the necessary i386 libs:

    dpkg --add-architecture i386
    aptitude update
    aptitude install ia32-libs zip

## Subversion

Whoever packaged this didn't bother to remove their .svn directories. These refereneced the following svn server:

    svn://66.120.31.113/ATEN_X9/Branch/STD_X9_237_Security_X10/

Sadly, this server is down (and would probably require authentication anyway). I've removed all the .svn content.

## Building

This is pretty self contained, so there aren't really a whole lot of requirements for the host machine itself.  You can build everything via:

    cd SDK
    make
    sudo make install
    
`make install` needs to be run as root, as the image filesystem contains some files that can only be read as root.  This should not install anything to the host machine, it just generates the final image.

    
## Software Versions

    Linux Kernel 2.6.17.13
    Lighttpd 1.4.32
    pcre 8.31
    pcre 8.33
    msmtp 1.4.16
    tree 1.5.2.2
    flex 2.5.35
    readline 5.2
    termcap 1.3.1
    BIND 9.8.1-P1
    busybox 1.12.0
    ntp 4.2.4p4
    openssl 0.9.8h
    openlldp 0.3alpha
    wide-dhcpv6 20080615
    openldap 2.4.11
    zlib 1.2.3
    glibc 2.3.5
    gcc 3.4.4
    libxml2 2.6.32
    
## Important Locations

    SDK/Web_Server/OS/Linux/ # Contains the CGI files for the web interface (no source code)
    SDK/Web_Server/OS/Linux/Web_Style/SUPERMICRO/page/ # Contains the HTML templates for the web server

