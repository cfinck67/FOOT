#!/bin/bash
#WARNING: check carefully the beammonitor.cfg config file and remember to DELETE or not DELETE the config/shifts.cfg file!!!
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2200_100kEv.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551900359.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551900359.root
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551901240.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551901240.root
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/228MeV_HV2175_100kEv.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551902227.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551902227.root
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2200_100kEv_tilt5deg.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551904521.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551904521.root
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/228MeV_HV2200_100kEv_tilt5deg.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551903443.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551903443.root
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/228MeV_HV2200_100kEv_tilt10deg.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551906384.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551906384.root

#~ ./DecodeMC -in 03_2019_Trento/acq_data/6_March_2019/80MeV_HV2200_100kEv.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551900359.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551900359.root
#~ ./DecodeMC -in 03_2019_Trento/acq_data/6_March_2019/80MeV_HV2175_100kEv.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551901240.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551901240.root
#~ ./DecodeMC -in 03_2019_Trento/acq_data/6_March_2019/228MeV_HV2175_100kEv.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551902227.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551902227.root
#~ ./DecodeMC -in 03_2019_Trento/acq_data/6_March_2019/80MeV_HV2200_100kEv_tilt5deg.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551904521.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551904521.root
#~ ./DecodeMC -in 03_2019_Trento/acq_data/6_March_2019/228MeV_HV2200_100kEv_tilt5deg.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551903443.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551903443.root
#~ ./DecodeMC -in 03_2019_Trento/acq_data/6_March_2019/228MeV_HV2200_100kEv_tilt10deg.dat -data -reco msd_strelcal_bmfiles/recohistos_tcut380_1551906384.root -tree msd_strelcal_bmfiles/recotree_tcut380_1551906384.root

echo "strel_msdfiles.sh ended without errors!"
