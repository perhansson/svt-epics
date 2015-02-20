#!/usr/bin/python

import subprocess

xpos = 1000
ypos = 0
yinc = 90
lines = 8

color = "-bg green -fg black -sb -sl 5000"
geometry  = "-geometry 70x" + str(lines)
#connection = '/u1/daq/rceScripts/connect_host.sh'
targetDir = '/u1/epics/svt-epics/svt-daq-epics/example/iocBoot/iocsvtDaqDpm0'

ypos = yinc
for dpm_index in range (0, 15):

	xpos = 1000
	if dpm_index%2 == 0:
		ypos = ypos + yinc 
		xpos = 500	
	position = geometry + "+" + str(xpos) + "+" + str(ypos)
	dpm = "dpm" + str(dpm_index)
	title = "-T DPM" + str(dpm_index)
	exeName = "svtDaqDpm" + str(dpm_index) + ".cmd"
	command = "xterm " + title + " " + color + " " + position
	#command += " -e " + targetDir + "/" + exeName 
	command += " -e 'pushd " + targetDir+ ";./" + exeName + ";bash'" 
	#command += " '/mnt/host/coda/run_roc_shell_ppapc;bash'"
	print command
	subprocess.Popen(command + "&", shell=True)

	#if dpm_index == 0 or dpm_index == 1:
	#	xpos = 250
	#	dtm = "dtm" + str(dpm_index)
	#	title = "-T DTM" + str(dpm_index) + "IOC"
	#	position = geometry + "+" + str(xpos) + "+" + str(ypos + dpm_index*yinc)
	#	command = "xterm " + title + " " + color + " " + position
	#	command += " -e " + connection + " " + dtm 
	#	command += " '/mnt/host/coda/run_roc_shell_ppapc;bash'"
	#	print command
	#	subprocess.Popen(command, shell=True)
