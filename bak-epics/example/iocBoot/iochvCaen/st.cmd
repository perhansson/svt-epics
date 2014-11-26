#!../../bin/linux-x86_64/hvCaen

## You may have to change hvCaen to something else
## everywhere it appears in this file

#< envPaths

## Register all support components
dbLoadDatabase("../../dbd/hvCaen.dbd",0,0)
hvCaen_registerRecordDeviceDriver(pdbbase) 

## Load record instances
dbLoadRecords("../../db/hvCaen.db","user=tracker")

iocInit()

## Start any sequence programs
#seq snchvCaen,"user=tracker"
