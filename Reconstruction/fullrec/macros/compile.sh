#!/bin/bash

src="run_PrintFlatNtuple.C"
out="PrintFlatNtuple"

g++ ${src} $(root-config --cflags --libs) -o ${out} 

