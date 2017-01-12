#!/bin/zsh

LISTFILE=$1

if [[ $LISTFILE == "" ]]; then
    echo "ERROR: Please specify list of data files to calibrate as first argument"
    exit
fi

if [[ -d 'data_calib' ]]; then
    echo "Output directory: data_calib FOUND"
else
    echo "Output directory: data_calib NOT FOUND- making it now."
    mkdir data_calib
fi

echo $LISTFILE

for FILENAME in $(cat $LISTFILE | grep ".txt" | cut -f1 | grep -v '#'); do

    if [[ $(cat $LISTFILE | cut -f1 | grep -v '#' | grep $FILENAME | wc -l) -gt 1 ]]; then
	echo "ERROR: More than one line for file $FILENAME found. Abort."
	exit
    fi

    CALIBNAME=$(cat $LISTFILE | grep $FILENAME | cut -f3 | head -1);
    echo "Calibrate data file $FILENAME with calibration $CALIBNAME"

    root -b -q DataFile_to_Root.C\(\"$FILENAME\",\"$CALIBNAME\"\)

done
