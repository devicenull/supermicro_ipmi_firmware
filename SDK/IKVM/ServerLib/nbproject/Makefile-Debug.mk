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
	${OBJECTDIR}/SRC/ServerLib/Network/NtwStream.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/ATENKeyboard.o \
	${OBJECTDIR}/SRC/ServerLib/Session/Session.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/vcdlib.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/vcdOsSupport.o \
	${OBJECTDIR}/SRC/ServerLib/Network/tcpsocket.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/ATENMouse.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/VideoModes.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/ASTVideo.o \
	${OBJECTDIR}/SRC/ServerLib/Protocol/RFBprotocol.o \
	${OBJECTDIR}/SRC/ServerLib/Server/Server.o \
	${OBJECTDIR}/SRC/ServerLib/Protocol/RFBAuth.o \
	${OBJECTDIR}/SRC/ServerLib/Util/Cryto.o \
	${OBJECTDIR}/SRC/ServerLib/Util/AESCryto.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/HermonVideo.o \
	${OBJECTDIR}/SRC/ServerLib/OSPort/OSPort.o \
	${OBJECTDIR}/SRC/ServerLib/KVM/KVM.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/ARM-Aspeed-Linux-GNU-Generic/libserverlib.a

dist/Debug/ARM-Aspeed-Linux-GNU-Generic/libserverlib.a: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/ARM-Aspeed-Linux-GNU-Generic
	${RM} dist/Debug/ARM-Aspeed-Linux-GNU-Generic/libserverlib.a
	${AR} rv dist/Debug/ARM-Aspeed-Linux-GNU-Generic/libserverlib.a ${OBJECTFILES} 
	$(RANLIB) dist/Debug/ARM-Aspeed-Linux-GNU-Generic/libserverlib.a

${OBJECTDIR}/SRC/ServerLib/Network/NtwStream.o: SRC/ServerLib/Network/NtwStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Network
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Network/NtwStream.o SRC/ServerLib/Network/NtwStream.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/ATENKeyboard.o: SRC/ServerLib/KVM/ATENKeyboard.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/ATENKeyboard.o SRC/ServerLib/KVM/ATENKeyboard.cpp

${OBJECTDIR}/SRC/ServerLib/Session/Session.o: SRC/ServerLib/Session/Session.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Session
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Session/Session.o SRC/ServerLib/Session/Session.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/vcdlib.o: SRC/ServerLib/KVM/HermonChip/vcdlib.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip
	$(COMPILE.c) -g -o ${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/vcdlib.o SRC/ServerLib/KVM/HermonChip/vcdlib.c

${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/vcdOsSupport.o: SRC/ServerLib/KVM/HermonChip/vcdOsSupport.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip
	$(COMPILE.c) -g -o ${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/vcdOsSupport.o SRC/ServerLib/KVM/HermonChip/vcdOsSupport.c

${OBJECTDIR}/SRC/ServerLib/Network/tcpsocket.o: SRC/ServerLib/Network/tcpsocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Network
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Network/tcpsocket.o SRC/ServerLib/Network/tcpsocket.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/ATENMouse.o: SRC/ServerLib/KVM/ATENMouse.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/ATENMouse.o SRC/ServerLib/KVM/ATENMouse.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/VideoModes.o: SRC/ServerLib/KVM/HermonChip/VideoModes.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip
	$(COMPILE.c) -g -o ${OBJECTDIR}/SRC/ServerLib/KVM/HermonChip/VideoModes.o SRC/ServerLib/KVM/HermonChip/VideoModes.c

${OBJECTDIR}/SRC/ServerLib/KVM/ASTVideo.o: SRC/ServerLib/KVM/ASTVideo.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/ASTVideo.o SRC/ServerLib/KVM/ASTVideo.cpp

${OBJECTDIR}/SRC/ServerLib/Protocol/RFBprotocol.o: SRC/ServerLib/Protocol/RFBprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Protocol
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Protocol/RFBprotocol.o SRC/ServerLib/Protocol/RFBprotocol.cpp

${OBJECTDIR}/SRC/ServerLib/Server/Server.o: SRC/ServerLib/Server/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Server
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Server/Server.o SRC/ServerLib/Server/Server.cpp

${OBJECTDIR}/SRC/ServerLib/Protocol/RFBAuth.o: SRC/ServerLib/Protocol/RFBAuth.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Protocol
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Protocol/RFBAuth.o SRC/ServerLib/Protocol/RFBAuth.cpp

${OBJECTDIR}/SRC/ServerLib/Util/Cryto.o: SRC/ServerLib/Util/Cryto.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Util
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Util/Cryto.o SRC/ServerLib/Util/Cryto.cpp

${OBJECTDIR}/SRC/ServerLib/Util/AESCryto.o: SRC/ServerLib/Util/AESCryto.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Util
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Util/AESCryto.o SRC/ServerLib/Util/AESCryto.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/HermonVideo.o: SRC/ServerLib/KVM/HermonVideo.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/HermonVideo.o SRC/ServerLib/KVM/HermonVideo.cpp

${OBJECTDIR}/SRC/ServerLib/OSPort/OSPort.o: SRC/ServerLib/OSPort/OSPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/OSPort
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/OSPort/OSPort.o SRC/ServerLib/OSPort/OSPort.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/KVM.o: SRC/ServerLib/KVM/KVM.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -g -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/KVM.o SRC/ServerLib/KVM/KVM.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/ARM-Aspeed-Linux-GNU-Generic/libserverlib.a

# Subprojects
.clean-subprojects:
