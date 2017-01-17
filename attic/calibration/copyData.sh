#!/bin/zsh

LISTFILE=$1

echo $LISTFILE

for FILENAME in $(cat $LISTFILE | grep ".txt" | awk '{print $1}'); do
  echo "Check for local copy of file $FILENAME..."

  if [[ -e data_local/$FILENAME ]]; then
    echo "... FOUND."
  else
    echo "... COPY HERE."
    cp data_all/$FILENAME data_local/
  fi
done
