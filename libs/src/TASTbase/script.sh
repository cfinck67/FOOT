#!/bin/bash

find . -regex "\./TAST*[a-zA-Z0-9]*\.cxx" -exec sed -i "s/TATR/TAST/g" {} \;
find . -regex "\./TAST*[a-zA-Z0-9]*\.hxx" -exec sed -i "s/TATR/TAST/g" {} \; 
find . -regex "\./TAST*[a-zA-Z0-9]*\.icc" -exec sed -i "s/TATR/TAST/g" {} \;
find . -regex "\./TAST*[a-zA-Z0-9]*\.h" -exec sed -i "s/TATR/TAST/g" {} \; 
