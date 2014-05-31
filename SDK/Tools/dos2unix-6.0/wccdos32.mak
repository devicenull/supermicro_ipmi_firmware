!include version.mk

CC      = wcc386
SRCDIR = .
DEFINES = -DVER_REVISION="$(DOS2UNIX_VERSION)" -DVER_DATE="$(DOS2UNIX_DATE)"
CFLAGS  = $(DEFINES) -i=$(SRCDIR) -w4 -e25 -zq -od -d2 -5r -bt=dos -mf
WATCOMSRC = $(%WATCOM)\src\startup

TARGET = causeway

all: dos2unix.exe unix2dos.exe mac2unix.exe unix2mac.exe

cflagsDS.cfg:
	@%create cflagsDS.cfg
	@%append cflagsDS.cfg $(CFLAGS)

dos2unix.exe: dos2unix.obj querycp.obj common.obj wildargv.obj
	@%create dos2unix.lnk
	@%append dos2unix.lnk FIL dos2unix.obj,querycp.obj,common.obj,wildargv.obj
	wlink name dos2unix d all SYS $(TARGET) op inc op m op st=64k op maxe=25 op q op symf @dos2unix.lnk
	del dos2unix.lnk

unix2dos.exe: unix2dos.obj querycp.obj common.obj wildargv.obj
	@%create unix2dos.lnk
	@%append unix2dos.lnk FIL unix2dos.obj,querycp.obj,common.obj,wildargv.obj
	wlink name unix2dos d all SYS $(TARGET) op inc op m op st=64k op maxe=25 op q op symf @unix2dos.lnk
	del unix2dos.lnk


dos2unix.obj :  $(SRCDIR)\dos2unix.c $(SRCDIR)\querycp.h $(SRCDIR)\common.h cflagsDS.cfg
	$(CC) @cflagsDS.cfg $(SRCDIR)\dos2unix.c

unix2dos.obj :  $(SRCDIR)\unix2dos.c $(SRCDIR)\querycp.h $(SRCDIR)\common.h cflagsDS.cfg
	$(CC) @cflagsDS.cfg $(SRCDIR)\unix2dos.c

querycp.obj :  $(SRCDIR)\querycp.c $(SRCDIR)\querycp.h cflagsDS.cfg
	$(CC) @cflagsDS.cfg $(SRCDIR)\querycp.c

common.obj :  $(SRCDIR)\common.c $(SRCDIR)\common.h cflagsDS.cfg
	$(CC) @cflagsDS.cfg $(SRCDIR)\common.c

wildargv.obj : $(WATCOMSRC)\wildargv.c
	$(CC) @cflagsDS.cfg $(WATCOMSRC)\wildargv.c

mac2unix.exe : dos2unix.exe
	copy /v dos2unix.exe mac2unix.exe

unix2mac.exe : unix2dos.exe
	copy /v unix2dos.exe unix2mac.exe

strip
	wstrip dos2unix.exe
	wstrip unix2dos.exe
	wstrip mac2unix.exe
	wstrip unix2mac.exe

clean
	-del *.obj
	-del *.exe
	-del *.SYM
	-del *.map
	-del *.ilk 
	-del cflagsDS.cfg
