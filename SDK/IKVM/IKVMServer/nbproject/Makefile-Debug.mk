#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Debug/ARM-Aspeed-Linux-GNU-Generic

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ServerEntry.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../ServerLib/dist/Debug/ARM-Aspeed-Linux-GNU-Generic/libserverlib.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/ARM-Aspeed-Linux-GNU-Generic/ikvmserver

dist/Debug/ARM-Aspeed-Linux-GNU-Generic/ikvmserver: ${BUILD_SUBPROJECTS}

dist/Debug/ARM-Aspeed-Linux-GNU-Generic/ikvmserver: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/ARM-Aspeed-Linux-GNU-Generic
	${LINK.cc} -lpthread -o dist/Debug/ARM-Aspeed-Linux-GNU-Generic/ikvmserver ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/ServerEntry.o: ServerEntry.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I../../ATEN\ Server/ServerLib -o ${OBJECTDIR}/ServerEntry.o ServerEntry.cc

# Subprojects
.build-subprojects:
	cd ../ServerLib && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/ARM-Aspeed-Linux-GNU-Generic/ikvmserver

# Subprojects
.clean-subprojects:
	cd ../ServerLib && ${MAKE}  -f Makefile CONF=Debug clean
