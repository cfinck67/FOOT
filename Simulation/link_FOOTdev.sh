#!/bin/sh
cd ROUTINES
$FLUKA/flutil/fff  usrini.f
$FLUKA/flutil/fff  usrein.f
$FLUKA/flutil/fff  usreou.f
$FLUKA/flutil/fff  usrout.f
$FLUKA/flutil/fff  mgdraw.f
$FLUKA/flutil/fff  mgdraw_lib.f
$FLUKA/flutil/fff  UpdateCurrentParticle.f

$FLUKA/flutil/ldpm3qmd -m fluka usrini.o usrout.o usreou.o usrein.o  mgdraw.o  mgdraw_lib.o UpdateCurrentParticle.o -o fluka_FOOT.exe

rm *.o

mv fluka_FOOT.exe ../

cd ../




