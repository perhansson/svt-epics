TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build the IOC support library

LIBRARY_IOC += dummySupport

# xxxRecord.h will be created from xxxRecord.dbd
DBDINC += xxxRecord
# Install devXxxSoft.dbd into <top>/dbd
DBD += xxxSupport.dbd

# Compile and add the code to the support library
dummySupport_SRCS += xxxRecord.c
dummySupport_SRCS += devXxxSoft.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application.
dummySupport_SRCS += dbSubExample.c
dummySupport_SRCS += dummyHello.c
dummySupport_SRCS += initTrace.c

dummySupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = dummy
# dummy.dbd will be created and installed
DBD += dummy.dbd

# dummy.dbd will be made up from these files:
dummy_DBD += base.dbd
dummy_DBD += xxxSupport.dbd
dummy_DBD += dbSubExample.dbd
dummy_DBD += dummyHello.dbd
dummy_DBD += initTrace.dbd

# dummy_registerRecordDeviceDriver.cpp derives from dummy.dbd
dummy_SRCS += dummy_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
dummy_SRCS_DEFAULT += dummyMain.cpp
dummy_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#dummy_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link in the code from the support library
dummy_LIBS += dummySupport

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

ifneq ($(SNCSEQ),)
    # Build sncExample into dummySupport
    sncExample_SNCFLAGS += +r
    dummy_DBD += sncExample.dbd
    dummySupport_SRCS += sncExample.stt
    dummySupport_LIBS += seq pv
    dummy_LIBS += seq pv

    # Build sncProgram as a standalone program
    PROD_HOST += sncProgram
    sncProgram_SNCFLAGS += +m
    sncProgram_SRCS += sncProgram.st
    sncProgram_LIBS += seq pv
    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Finally link to the EPICS Base libraries
dummy_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

