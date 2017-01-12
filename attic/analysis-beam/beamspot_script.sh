#!/bin/bash
#A shell script to run the Beam Spot Analysis code for multiple beam spot images
#Assign file name to output file variable
OUTPUTFILE="beamspot_data.txt"
PNGLocale="../Images/LiRoom_QCheck/"
CONFIGLIST="Currents/currents_shorttest.txt"

#If an output file of this name already exists, remove it
if [[ -e $OUTPUTFILE ]]; then
    echo "Remove old"
    rm $OUTPUTFILE
fi
#Poke it with a stick
touch $OUTPUTFILE

#Get the list of image files for analysis
FILELIST=$(cat $CONFIGLIST | cut -d\  -f1)
#-----------------------------------------
#Run each image through beamspot_analysis.C
#Obtain mean and standard deviation values for x and y
#Write everything to the output file
#-----------------------------------------
#Loop through each image file
for FILENAME in $FILELIST; do
    #Create ID for each image file from file names
    FILEID=$(echo ${FILENAME} | cut -d\/ -f4 | cut -d_ -f8 | cut -d. -f1)
    #Write statement to terminal as a check
    echo "Processing file $FILENAME with ID $FILEID"

#Obtain current values used to create the beam spot in the associated image file
current1=$(cat $CONFIGLIST | grep $FILENAME | cut -d\  -f2)
current2=$(cat $CONFIGLIST | grep $FILENAME | cut -d\  -f3)

PNGNAME=$(echo ${PNGLocale}$( echo ${FILENAME} | cut -d\/ -f4 | sed -s 's/.JPG/.png/g') )
echo $PNGNAME
PDFNAME=$(echo ${PNGLocale}$( echo ${FILENAME} | cut -d\/ -f4 | sed -s 's/.JPG/.pdf/g') )
echo $PDFNAME

#Run the image through the Beam Spot Analysis code


#root -b -q -L beamspot_analysis.C++\(\"${FILENAME}\"\) > tempoutput.txt
root -b -q beamspot_nocompile.C\(\"${FILENAME}\",\"${PNGNAME}\"\) > tempoutput.txt

convert ${PNGNAME} ${PDFNAME}
#Obtain mean and standard deviation values for x and y
meanx=$(cat tempoutput.txt | grep "Mean x:" | cut -d: -f2)
stdx=$(cat tempoutput.txt | grep "std x:" | cut -d: -f2)
meany=$(cat tempoutput.txt | grep "Mean y:" | cut -d: -f2)
stdy=$(cat tempoutput.txt | grep "std y:" | cut -d: -f2)

#Write everything to the output file
echo "$FILEID ${current1} ${current2} ${meanx} ${stdx} ${meany} ${stdy}" >> beamspot_data.txt

#Done with the for loop, although this command is pretty self-explanatory
done

gs -dBATCH -dNOPAUSE -sDEVICE=pdfwrite -sEPSCrop -sOutputFile=../Images/LiRoom_QCheck/LiRoomTest2.pdf ../Images/LiRoom_QCheck/IMG*.eps
