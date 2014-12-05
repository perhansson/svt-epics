#!/bin/tcsh

echo "Start SVT DAQ Setup GUI"
set cmd = ""
if($#argv == 0) then
    echo "Need to supply x for execution or e for edit"
else
    if($argv[1] == "x") then
	#medm -x svtDaqSetup.adl svtTempSensors.adl svtStatusPS_latest_onoff_control.adl &
	set cmd = "-x"
	#echo "what"
    else
	#echo "dfsd"
	set cmd = ""
    endif
    echo $cmd
    medm $cmd svtDaqSetup.adl &
    medm $cmd svtTempSensors.adl &
    medm $cmd svtStatusPS_latest_onoff_control.adl &    
    medm $cmd hps_svt_FELV_all.adl &
    medm $cmd hps_svt_FLLV_all.adl &
    medm $cmd hps_svt_bias_all_onoff_control.adl &
    echo "Done"
endif

