TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC support library

LIBRARY_IOC += svtDaqDpm0Support

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# Install devXxxSoft.dbd into <top>/dbd
#DBD += xxxSupport.dbd

#USR_INCLUDES += -I/root/daq/include
USR_INCLUDES += -I/usr/include/libxml2

# Compile and add the code to the support library
# Link locally-provided code into the support library,
# rather than directly into the IOC application.
svtDaqDpm0Support_SRCS += dbSubDpm.c
svtDaqDpm0Support_SRCS += initTrace.c


svtDaqDpm0Support_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = svtDaqDpm0
# svtDaqDpm0.dbd will be created and installed
DBD += svtDaqDpm0.dbd

# svtDaqDpm0.dbd will be made up from these files:
svtDaqDpm0_DBD += base.dbd
svtDaqDpm0_DBD += initTrace.dbd


# Include dbd files from all support applications:
#svtDaqDpm0_DBD += xxx.dbd
svtDaqDpm0_DBD += dbSubDpm.dbd

# Add all the support libraries needed by this IOC
#svtDaqDpm0_LIBS += xxx
svtDaqDpm0_LIBS += svtDaqDpm0Support
xml2_DIR += /usr/lib  
xml2_LIBS += xml2  
svtDaqDpm0_LIBS += xml2


# svtDaqDpm0_registerRecordDeviceDriver.cpp derives from svtDaqDpm0.dbd
svtDaqDpm0_SRCS += svtDaqDpm0_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
svtDaqDpm0_SRCS_DEFAULT += svtDaqDpm0Main.cpp
svtDaqDpm0_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#svtDaqDpm0_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary





# Finally link to the EPICS Base libraries
svtDaqDpm0_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

