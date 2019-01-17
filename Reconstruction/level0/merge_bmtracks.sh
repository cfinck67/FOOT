#!/bin/bash

cd ./results/BMtracks

if [ $# -eq 0 ]
  then
    FILE_NAME="BM_track.pdf"
  else
    FILE_NAME=$1.pdf
fi

gs -q -sDEVICE=pdfwrite -dNOPAUSE -dBATCH -dSAFER -sOutputFile=$FILE_NAME BM_track_*.eps  
mv $FILE_NAME ../../

echo "BM tracks stored in ./results/BMtracks/* are merged in" $FILE_NAME

