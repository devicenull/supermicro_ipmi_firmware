#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.

#execute file name 
EXECUTABLE = ikvmserver
OBJS= $(OBJECTFILES) ../ServerLib/dist/Release/Platform_GNU-Generic/libserverlib.a
#APP = IKVM_APP
ifeq ($(APP), IKVM_APP)
CROSS_COMPILE = ../../ToolChain/Host/AST2050/gcc-3.4.2-glibc-2.3.3/arm-aspeed-linux-gnu/bin/arm-aspeed-linux-gnu-
BOARD = HW_STG2SMCA
MYMCU = AST2050
endif
# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=$(CROSS_COMPILE)ranlib
CC=$(CROSS_COMPILE)gcc
CCC=$(CROSS_COMPILE)g++
CXX=$(CROSS_COMPILE)g++
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Release/Platform_GNU-Generic

# Object Files
#OBJECTFILES= \
	${OBJECTDIR}/ServerEntry.o

# C Compiler Flags
#CFLAGS=

# CC Compiler Flags
CCFLAGS += -D${MYMCU} -D${BOARD}
CXXFLAGS += -D${MYMCU} -D${BOARD}

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread ../ServerLib/dist/Release/Platform_GNU-Generic/libserverlib.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Release/Platform_GNU-Generic/ikvmserver

dist/Release/Platform_GNU-Generic/ikvmserver: ${BUILD_SUBPROJECTS}

dist/Release/Platform_GNU-Generic/ikvmserver: ${OBJECTFILES}
	${MKDIR} -p dist/Release/Platform_GNU-Generic
	${LINK.cc} -o dist/Release/Platform_GNU-Generic/ikvmserver -s ${OBJECTDIR}/ServerEntry.o ${LDLIBSOPTIONS} 

${OBJECTDIR}/ServerEntry.o: ServerEntry.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I../ServerLib/SRC/ServerLib -o ${OBJECTDIR}/ServerEntry.o ServerEntry.cc

# Subprojects
.build-subprojects:
	cd ../ServerLib && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/Platform_GNU-Generic/ikvmserver

# Subprojects
.clean-subprojects:
	cd ../ServerLib && ${MAKE}  -f Makefile CONF=Release clean
 
.extract:
	@echo IKVM $(CONF) .extract
	@$(ADDOBJLIST)
ifneq ($(APP), IKVM_APP)
include $(PRJ_PATH)/.config
endif
ADDOBJLIST += $(call ADD_SDKOBJ,$(CONFIG_IKVM),$(OBJS),,"CONFIG_IKVM")
#SDK folder name
FOLDER   = SDK/IKVM

MAKETEMPLATE = Makefile.ikvm
ifneq ($(APP), IKVM_APP)
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
endif
