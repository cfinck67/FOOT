#!/bin/bash
#WARNING: check carefully the beammonitor.cfg config file and remember to DELETE or not DELETE the config/shifts.cfg file!!!

#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try1garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try2garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try3garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try4garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try5garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try6garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try7garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try8garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try9garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try10garf_pol10.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try11garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try12garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try13garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try14garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try15garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try16garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try17garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try18garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try19garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
#~ ./DecodeMC -in data/msd_marzo/bmdata/6_March_2019/80MeV_HV2175_100kEv.dat -reco recohistos_try20garf.root -nev 100000 -data -bmconfile /config/beammonitor.cfg  &&
echo "strel_selfcalibration.sh ended without errors!"
