#!/usr/bin/python

import subprocess
import math

xpos = 10
ypos = 0
xinc = 510
yinc = 120
lines = 8

color = "-bg green -fg black -sb -sl 5000"
geometry  = "-geometry 70x" + str(lines)
targetDir = '/u1/epics/svt-epics/svt-daq-epics/example/iocBoot/iocsvtDaqDpm0'

#ypos = yinc
for dpm_index in range (0, 15):
	ix = dpm_index % 3
	iy = math.floor(dpm_index/3)
	xpos = ix*xinc
	ypos = yinc + iy*yinc
	#print "x ", ix, " -> ", xpos, " y ", iy, " -> ", ypos
	position = geometry + "+" + str(xpos) + "+" + str(int(ypos))
	dpm = "dpm" + str(dpm_index)
	title = "-T DPM" + str(dpm_index)
	exeName = "svtDaqDpm" + str(dpm_index) + ".cmd"
	command = "xterm " + title + " " + color + " " + position
	command += " -e 'pushd " + targetDir+ ";./" + exeName + ";bash'" 
	#print command
	subprocess.Popen(command + "&", shell=True)

	if dpm_index == 0 or dpm_index == 1:
		xpos = dpm_index*510
		ypos = 0
		dtm = "dtm" + str(dpm_index)
		title = "-T DTM" + str(dpm_index) + "IOC"
		position = geometry + "+" + str(int(xpos)) + "+" + str(int(ypos))
		exeName = "svtDaqDtm" + str(dpm_index) + ".cmd"
		command = "xterm " + title + " " + color + " " + position
		command += " -e 'pushd " + targetDir+ ";./" + exeName + ";bash'" 		
		#print command
		subprocess.Popen(command + "&", shell=True)
		
