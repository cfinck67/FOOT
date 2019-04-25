#!/bin/bash
#WARNING: check carefully the beammonitor.cfg config file and remember to DELETE or not DELETE the config/shifts.cfg file!!!

./DecodeMC -in data/MSDmarzo/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos1.root -nev 50000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/MSDmarzo/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos2.root -nev 50000 -data -bmconfile /config/beammonitor.cfg  &&
echo "strel_selfcalibration.sh ended without errors!"
