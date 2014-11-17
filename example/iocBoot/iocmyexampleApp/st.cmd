#!../../bin/linux-x86_64/myexample

## You may have to change myexample to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}


## Register all support components
dbLoadDatabase "dbd/myexample.dbd"
myexample_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate "db/userHost.substitutions"
#dbLoadRecords "db/hybLV.db"
#dbLoadRecords "db/dbSubExample.db", "user=trackerHost"
dbLoadRecords "db/dbHybT.db"
dbLoadRecords "db/dbHybLV.db"
dbLoadRecords "db/dbFebTemp.db"
dbLoadRecords "db/dbSVT.db"

## Set this to see messages from mySub
var mySubDebug 3

## Run this to trace the stages of iocInit
traceIocInit

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncExample, "user=rootHost"
#seq sncExample, "user=trackerHost"



