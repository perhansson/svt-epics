#!../../bin/linux-x86/svtDaqDpm0

## You may have to change svtDaqDpm0 to something else
## everywhere it appears in this file

< envPaths_x86

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/svtDaqDpm0.dbd"
svtDaqDpm0_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=trackerHost")
dbLoadRecords("db/dbDataDpm.db","DPM=7")

var mySubDebug 1

traceIocInit

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncxxx,"user=trackerHost"
