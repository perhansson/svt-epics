#!/bin/tcsh

echo "Setup environment"
pushd ../svt-daq-epics
echo $PWD
source setup_epics.csh
popd

echo "Start SVT DAQ Setup GUI"
set cmd = ""
if($#argv == 0) then
    echo "Need to supply x for execution or e for edit"
else
    if($argv[1] == "x") then
	set cmd = "-x"
	#echo "what"
    else
	#echo "dfsd"
	set cmd = ""
    endif
    echo $cmd
    medm $cmd svtDaqMap.adl &
    medm $cmd svtDaqSocketSetup.adl &
    medm $cmd svtTemp.adl &
    medm $cmd svtHybridMain.adl &    
    medm $cmd svtFebMain.adl &
    medm $cmd svtFlangeMain.adl &
    medm $cmd svtBiasMain.adl &
    echo "Done"
endif

