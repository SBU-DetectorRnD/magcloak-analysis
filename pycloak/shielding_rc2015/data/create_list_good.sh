#!/bin/zsh

# check if valid directory is given as first argument
if [ "$#" -ne 1 ] || ! [ -d "$1" ]; then
  echo "Usage: $0 DIRECTORY" >&2
  exit 1
fi

LISTFILE="$1/list_good.txt"

# if old file list exists: delete it
if [ -e $LISTFILE ]; then
    rm $LISTFILE
fi

# create new empty file list
touch $LISTFILE

# make list of all data files in directory
files=($(ls $1 | grep ^DataFile_......_.......txt))

# loop over all data files- if they pass quality check, add them to file list
for f in $files; do

    if [[ $(wc -l $1/$f | awk '{print $1}') -ge 30 ]]; then
	echo "File $f PASSED"
	echo "$f" >> $LISTFILE
    else
	echo "File $f HAS < 30 LINES -> NO PASS"
    fi

done

