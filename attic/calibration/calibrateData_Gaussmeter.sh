#!/bin/bash

LISTFILE=$1

INPUTDIR="data/DATA_Gaussmeter"
OUTPUTDIR="data-calib/DATA_Gaussmeter"

if [[ $LISTFILE == "" ]]; then
    echo "ERROR: Please specify list of data files to calibrate as first argument"
    exit
fi

if ! [[ -d $INPUTDIR ]]; then
    echo "Input directory: $INPUTDIR NOT FOUND"
    exit
fi

if ! [[ -d $OUTPUTDIR ]]; then
    echo "Output directory: $OUTPUTDIR NOT FOUND"
    exit
fi

echo "Using list file $LISTFILE"

for FILENAME in $(cat $LISTFILE | grep ".txt" | cut -d\  -f1 | grep -v '#'); do

    if [[ $(cat $LISTFILE | cut -f1 | grep -v '#' | grep $FILENAME | wc -l) -gt 1 ]]; then
	echo "ERROR: More than one line for file $FILENAME found. Abort."
	exit
    fi

#    CALIBNAME=$(cat $LISTFILE | grep $FILENAME | cut -f3  | head -1)
    CALIBNAME=$(cat $LISTFILE | grep $FILENAME | cut -d ' '  -f1 )  #  | head -1)
echo $CALIBNAME

#    echo "Calibrate data file $FILENAME with calibration $CALIBNAME"

    #root -b -q DataFile_to_Root.C\(\"$FILENAME\",\"$CALIBNAME\"\)

done
