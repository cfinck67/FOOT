#!/bin/bash

if [ $# -eq 0 ]
  then
    FILE_NAMEOUT="Projections_a2d.pdf"
  else
    FILE_NAMEIN=$1
    FILE_NAMEOUT=$2
fi

gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -dAutoRotatePages=/None -sOutputFile=$FILE_NAMEOUT  $FILE_NAMEIN


echo $FILENAMEIN " are merged in " $FILE_NAMEOUT


