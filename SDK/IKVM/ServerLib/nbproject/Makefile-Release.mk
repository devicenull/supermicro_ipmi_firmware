#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.

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
CHIP_DIR=SRC/ServerLib/KVM/OS/Linux/Host/$(MYMCU)

# Object Files
OBJECTFILES := ${OBJECTDIR}/SRC/ServerLib/Network/NtwStream.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/KVM/ATENKeyboard.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Session/Session.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Session/SessionControl.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Util/aeslib.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Network/tcpsocket.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Protocol/UserDB.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/KVM/ATENMouse.o 
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Protocol/RFBprotocol.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Server/Server.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Protocol/RFBAuth.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Util/Cryto.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Util/AESCryto.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/OSPort/OSPort.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Protocol/ServerParameter.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/KVM/KVM.o
ifeq (${MYMCU}, YARKON)
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/strp/strp_if.o 
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gpio/gpio_if.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/aic/aic_if.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/ece/ece_if.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gcr/gcr_if.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/vcd/vcd_if.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/vcdOsSupport.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/vcdlib.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Util/rle.o
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/Util/huffman.o
endif

ifeq (${MYMCU}, HERMON)
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/vcdlib.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/vcdOsSupport.o
OBJECTFILES += ${OBJECTDIR}/$(CHIP_DIR)/VideoModes.o
endif

ifeq (${MYMCU}, PILOT3)
OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/KVM/OS/Linux/Host/$(MYMCU)/Pilot3VideoLib.o
endif

OBJECTFILES += ${OBJECTDIR}/SRC/ServerLib/KVM/OS/Linux/Host/$(MYMCU)/ATENVideo.o

# C Compiler Flags
ifneq ($(APP), IKVM_APP)
CFLAGS += -D${MYMCU} -D${BOARD} -I../../IPMI -ISRC/ServerLib/KVM/OS/Linux/Host/$(MYMCU)/ -ISRC/ServerLib
ifeq (${MYMCU}, YARKON)
CFLAGS += -DEXTERNAL_INCLUDE_FILE=$(CHIP_DIR)/hal.h
CFLAGS += -DCHIP_NAME=npcm650 -DBOARD_NAME=YarkonSVB -DVERBOSE_GLOBAL -ISRC/ServerLib
endif
else
CFLAGS += -D${MYMCU} -D${BOARD} -DINT_VGA -ISRC/ServerLib
endif

# CC Compiler Flags
CCFLAGS += -D${MYMCU} -D${BOARD} -ISRC/ServerLib/KVM/OS/Linux/Host/$(MYMCU)/
CXXFLAGS += -D${MYMCU} -D${BOARD} -ISRC/ServerLib/KVM/OS/Linux/Host/$(MYMCU)/

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Release/Platform_GNU-Generic/libserverlib.a

dist/Release/Platform_GNU-Generic/libserverlib.a: ${OBJECTFILES}
	${MKDIR} -p dist/Release/Platform_GNU-Generic
	${RM} dist/Release/Platform_GNU-Generic/libserverlib.a
	${AR} rv dist/Release/Platform_GNU-Generic/libserverlib.a ${OBJECTFILES} 
	$(RANLIB) dist/Release/Platform_GNU-Generic/libserverlib.a

${OBJECTDIR}/SRC/ServerLib/Network/NtwStream.o: SRC/ServerLib/Network/NtwStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Network
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Network/NtwStream.o SRC/ServerLib/Network/NtwStream.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/ATENKeyboard.o: SRC/ServerLib/KVM/ATENKeyboard.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/ATENKeyboard.o SRC/ServerLib/KVM/ATENKeyboard.cpp

${OBJECTDIR}/SRC/ServerLib/Session/Session.o: SRC/ServerLib/Session/Session.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Session
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Session/Session.o SRC/ServerLib/Session/Session.cpp

${OBJECTDIR}/SRC/ServerLib/Session/SessionControl.o: SRC/ServerLib/Session/SessionControl.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Session
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Session/SessionControl.o SRC/ServerLib/Session/SessionControl.cpp

${OBJECTDIR}/$(CHIP_DIR)/vcdlib.o: $(CHIP_DIR)/vcdlib.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/vcdlib.o $(CHIP_DIR)/vcdlib.c

${OBJECTDIR}/SRC/ServerLib/Util/aeslib.o: SRC/ServerLib/Util/aeslib.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Util
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/SRC/ServerLib/Util/aeslib.o SRC/ServerLib/Util/aeslib.c

${OBJECTDIR}/$(CHIP_DIR)/vcdOsSupport.o: $(CHIP_DIR)/vcdOsSupport.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/vcdOsSupport.o $(CHIP_DIR)/vcdOsSupport.c



${OBJECTDIR}/SRC/ServerLib/Network/tcpsocket.o: SRC/ServerLib/Network/tcpsocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Network
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Network/tcpsocket.o SRC/ServerLib/Network/tcpsocket.cpp

	
${OBJECTDIR}/SRC/ServerLib/Protocol/UserDB.o: SRC/ServerLib/Protocol/UserDB.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Protocol
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/SRC/ServerLib/Protocol/UserDB.o SRC/ServerLib/Protocol/UserDB.c

${OBJECTDIR}/SRC/ServerLib/KVM/ATENMouse.o: SRC/ServerLib/KVM/ATENMouse.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/ATENMouse.o SRC/ServerLib/KVM/ATENMouse.cpp

${OBJECTDIR}/$(CHIP_DIR)/VideoModes.o: $(CHIP_DIR)/VideoModes.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/VideoModes.o $(CHIP_DIR)/VideoModes.c

${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/strp/strp_if.o: $(CHIP_DIR)/BMC_HAL/Modules/strp/strp_if.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/strp/
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/strp/strp_if.o $(CHIP_DIR)/BMC_HAL/Modules/strp/strp_if.c	

${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gpio/gpio_if.o: $(CHIP_DIR)/BMC_HAL/Modules/gpio/gpio_if.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gpio/
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gpio/gpio_if.o $(CHIP_DIR)/BMC_HAL/Modules/gpio/gpio_if.c	

${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/aic/aic_if.o: $(CHIP_DIR)/BMC_HAL/Modules/aic/aic_if.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/aic/
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/aic/aic_if.o $(CHIP_DIR)/BMC_HAL/Modules/aic/aic_if.c	

${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/ece/ece_if.o: $(CHIP_DIR)/BMC_HAL/Modules/ece/ece_if.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/ece/
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/ece/ece_if.o $(CHIP_DIR)/BMC_HAL/Modules/ece/ece_if.c	

${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gcr/gcr_if.o: $(CHIP_DIR)/BMC_HAL/Modules/gcr/gcr_if.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gcr/
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/gcr/gcr_if.o $(CHIP_DIR)/BMC_HAL/Modules/gcr/gcr_if.c	
	
${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/vcd/vcd_if.o: $(CHIP_DIR)/BMC_HAL/Modules/vcd/vcd_if.c 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/vcd/
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/$(CHIP_DIR)/BMC_HAL/Modules/vcd/vcd_if.o $(CHIP_DIR)/BMC_HAL/Modules/vcd/vcd_if.c			

${OBJECTDIR}/$(CHIP_DIR)/Pilot3VideoLib.o: $(CHIP_DIR)/Pilot3VideoLib.c
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/
	$(COMPILE.c) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/$(CHIP_DIR)/Pilot3VideoLib.o $(CHIP_DIR)/Pilot3VideoLib.c
	
${OBJECTDIR}/SRC/ServerLib/Util/rle.o: SRC/ServerLib/Util/rle.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Util
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/SRC/ServerLib/Util/rle.o SRC/ServerLib/Util/rle.c	
	
${OBJECTDIR}/SRC/ServerLib/Util/huffman.o: SRC/ServerLib/Util/huffman.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Util
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/SRC/ServerLib/Util/huffman.o SRC/ServerLib/Util/huffman.c						

${OBJECTDIR}/$(CHIP_DIR)/ATENVideo.o: $(CHIP_DIR)/ATENVideo.cpp 
	${MKDIR} -p ${OBJECTDIR}/$(CHIP_DIR)/
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/$(CHIP_DIR)/ATENVideo.o $(CHIP_DIR)/ATENVideo.cpp

${OBJECTDIR}/SRC/ServerLib/Protocol/RFBprotocol.o: SRC/ServerLib/Protocol/RFBprotocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Protocol
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Protocol/RFBprotocol.o SRC/ServerLib/Protocol/RFBprotocol.cpp

${OBJECTDIR}/SRC/ServerLib/Server/Server.o: SRC/ServerLib/Server/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Server
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Server/Server.o SRC/ServerLib/Server/Server.cpp

${OBJECTDIR}/SRC/ServerLib/Protocol/RFBAuth.o: SRC/ServerLib/Protocol/RFBAuth.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Protocol
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Protocol/RFBAuth.o SRC/ServerLib/Protocol/RFBAuth.cpp

${OBJECTDIR}/SRC/ServerLib/Util/Cryto.o: SRC/ServerLib/Util/Cryto.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Util
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Util/Cryto.o SRC/ServerLib/Util/Cryto.cpp

${OBJECTDIR}/SRC/ServerLib/Util/AESCryto.o: SRC/ServerLib/Util/AESCryto.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Util
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/Util/AESCryto.o SRC/ServerLib/Util/AESCryto.cpp

${OBJECTDIR}/SRC/ServerLib/OSPort/OSPort.o: SRC/ServerLib/OSPort/OSPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/OSPort
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/OSPort/OSPort.o SRC/ServerLib/OSPort/OSPort.cpp

${OBJECTDIR}/SRC/ServerLib/KVM/KVM.o: SRC/ServerLib/KVM/KVM.cpp 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/KVM
	$(COMPILE.cc) -O2 -s -ISRC/ServerLib -o ${OBJECTDIR}/SRC/ServerLib/KVM/KVM.o SRC/ServerLib/KVM/KVM.cpp

${OBJECTDIR}/SRC/ServerLib/Protocol/ServerParameter.o: SRC/ServerLib/Protocol/ServerParameter.c 
	${MKDIR} -p ${OBJECTDIR}/SRC/ServerLib/Protocol
	$(COMPILE.c) -O2 -s -o ${OBJECTDIR}/SRC/ServerLib/Protocol/ServerParameter.o SRC/ServerLib/Protocol/ServerParameter.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/Platform_GNU-Generic/libserverlib.a

# Subprojects
.clean-subprojects:
