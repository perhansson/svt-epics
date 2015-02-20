import sys, subprocess

for dpm in range(10,15):
    print dpm
    namenew = "svtDaqDpm" + str(dpm) + ".cmd.new"
    name = "svtDaqDpm" + str(dpm) + ".cmd"
    fnew = open(namenew, "w")
    f = open(name, "r")
    for l in f:
        if "DPM=0" in l:
            print l
            l = l.replace("DPM=0","DPM="+str(dpm))
            print l
        fnew.write(l)
    fnew.close()
    f.close()
    subprocess.call("mv " + namenew + " " + name, shell=True)

