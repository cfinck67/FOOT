#include "MagneticField.hxx"

//ClassImp(MagneticField);

/*******************************************************************************/

MagneticField::MagneticField(Double_t Current, TVector3 BoundingBox,
			     TVector3 PositionOffset, TVector3 Orientation, TString dir) 
{
	fCurrent = Current;
	fBoundingBox = BoundingBox;
	fPositionOffset = PositionOffset;
	fOrientation = Orientation;
	//TODO: fOrientationAngle is missing still
	m_dir  = dir;
	InitMagnetic();
}

/*******************************************************************************/

MagneticField::MagneticField(Double_t Current, TString dir) 
{//current in A

	fCurrent = Current;

	fBoundingBox.SetXYZ(1., 1., 1.);
	fPositionOffset.SetXYZ(0., 0., 0.);
	fOrientation.SetXYZ(0., 0., 1.);
	//TODO: fOrientationAngle is missing still
	m_dir = dir;
	InitMagnetic();
}

/*******************************************************************************/

MagneticField::~MagneticField() 
{

}

/*******************************************************************************/

void MagneticField::SetCurrent(Double_t current) {

  fCurrent = current;
  return;

}
	
Double_t MagneticField::GetCurrent() {

  return fCurrent;

}
	
void MagneticField::InitMagnetic() 
{
	//fScaleWoodSaxon = 1.0;
	fScaleWoodSaxon = 0.00080832;
	//fScaleWoodSaxon=-12.710 // amplitude for WS potential [Tm]
	//fScaleWoodSaxon=-9.

	//complicated field map
	//InitFieldTrafo(); // commented - not used (MV)
	//InitFieldMap();   // commented - not used (MV)

	//simple field map
	InitFieldMapSimple();

}

/*******************************************************************************/

//get magnetic field, reference frame of ALADiN 
TVector3 MagneticField::GetField(TVector3 Position) 
{

	//TODO: add possibility for some shifting of Aladin frame wrt Field frame here:
	//TODO: apply trafo: LocalPosition=PositionTrafoAladin2Field(Position);
	//TODO: check for a general bounding box here, is not inside set field to zero
	//	if (IsInsideBox(fPositionOffset, fOrientation, fBoundingBox, Position)) { 

	return GetFieldMapSimple(Position);
}

/*******************************************************************************/
/* returns the field value of a Wood-Saxon potential 
 * 
 * */

//position is given ALADiN coordinates  
TVector3 MagneticField::GetFieldWoodSaxon(TVector3 Position) 
{
	TVector3 Field(0., 0., 0.);

	//for the moment:
	if ((Position.Z() > -100000.) && (Position.Z() < 100000.)) {//TODO: if we are within +/- 1000m ??? makes no sense to me
		//calculate WoodSaxon field strength;
		Field .SetY( fCurrent * fScaleWoodSaxon * -9. * (1.0 / (1.0 + exp( (fabs(Position.Z()) - 70.0) / 10.0))));

		//		Double_t WoodSaxonWidth=10.//UNITS: cm
		//		Field.SetY( fCurrent*fScaleWoodSaxon* ( 1.0 / (1.0 + exp( fabs(Position.Z())/ WoodSaxonWidth ))) )

	} else {
		Field = TVector3(0., 0., 0.);
	}
	cout << "DEBUG: MagneticField::GetFieldWoodSaxon(): Get field at ("
			<< setw(5) << Position.X() << "," << Position.Y() << ","
			<< Position.Z() << ")" << "Field: (" << Field.X() << ","
			<< Field.Y() << "," << Field.Z() << ")" << endl;
	return Field;

}

/*******************************************************************************/

//Reading the magnetic field stuff, just wrap it in an other layer of shit ...

/* Clear the field map structure */
void MagneticField::InitFieldMap() 
{
	LBI = 90;
	LBJ = 19;
	LBK = 21;

	RBI = 85;
	RBJ = 19;
	RBK = 17;

	Int_t i = 0;
	Int_t j = 0;
	Int_t k = 0;

	//cout << "DEBUG: We are in MagneticField::InitFieldMap()." << endl;
	/* The monstrous field map */

	theFieldMap.li = LBI;
	theFieldMap.lj = LBJ;
	theFieldMap.lk = LBK;
	theFieldMap.lstatus = 0;
	theFieldMap.ri = RBI;
	theFieldMap.rj = RBJ;
	theFieldMap.rk = RBK;
	theFieldMap.lstatus = 0;

	/* Clear the field maps */
	for (i = 0; i < theFieldMap.li; i++) {
		for (j = 0; j < theFieldMap.lj; j++) {
			for (k = 0; k < theFieldMap.lk; k++) {
				theFieldMap.lbx[i][j][k] = 0.0;
				theFieldMap.lby[i][j][k] = 0.0;
				theFieldMap.lbz[i][j][k] = 0.0;
			}
		}
	}

	for (i = 0; i < theFieldMap.ri; i++) {
		for (j = 0; j < theFieldMap.rj; j++) {
			for (k = 0; k < theFieldMap.rk; k++) {
				theFieldMap.rbx[i][j][k] = 0.0;
				theFieldMap.rby[i][j][k] = 0.0;
				theFieldMap.rbz[i][j][k] = 0.0;
			}
		}
	}

	FillFieldMap();
	return;

}

/*******************************************************************************/

/* Precalculates the geometry values for the coordinate transformation between ALADIN */
/* and box reference frames.                                                          */
void MagneticField::InitFieldTrafo() 
{
	Float_t cosa = 0.0;
	Float_t v1[2] = { 0.0, 0.0 };
	Float_t v2[2] = { 0.0, 0.0 };

	//all values are in mm
	/* The measurement geometry */
	theFieldTrafo.box1[0][0] = 1233.00;
	theFieldTrafo.box1[1][0] = 0.00;
	theFieldTrafo.box1[2][0] = -100.00;
	theFieldTrafo.box1[0][1] = -1232.24;
	theFieldTrafo.box1[1][1] = 0.00;
	theFieldTrafo.box1[2][1] = -100.00;

	theFieldTrafo.box2[0][0] = 1233.00;
	theFieldTrafo.box2[1][0] = 0.00;
	theFieldTrafo.box2[2][0] = 100.00;
	theFieldTrafo.box2[0][1] = -1231.97;
	theFieldTrafo.box2[1][1] = 0.00;
	theFieldTrafo.box2[2][1] = 100.00;

	theFieldTrafo.mag1[0][0] = 1233.19;
	theFieldTrafo.mag1[1][0] = -2.61;
	theFieldTrafo.mag1[2][0] = -94.08;
	theFieldTrafo.mag1[0][1] = -1230.22;
	theFieldTrafo.mag1[1][1] = -2.61;
	theFieldTrafo.mag1[2][1] = 0.92;

	theFieldTrafo.mag2[0][0] = 1232.59;
	theFieldTrafo.mag2[1][0] = -2.10;
	theFieldTrafo.mag2[2][0] = 104.11;
	theFieldTrafo.mag2[0][1] = -1230.22;
	theFieldTrafo.mag2[1][1] = -2.10;
	theFieldTrafo.mag2[2][1] = 0.92;

	theFieldTrafo.off1[0] = theFieldTrafo.box1[0][0] - theFieldTrafo.mag1[0][0];
	theFieldTrafo.off1[1] = theFieldTrafo.box1[1][0] - theFieldTrafo.mag1[1][0];
	theFieldTrafo.off1[2] = theFieldTrafo.box1[2][0] - theFieldTrafo.mag1[2][0];

	theFieldTrafo.off2[0] = theFieldTrafo.box2[0][0] - theFieldTrafo.mag2[0][0];
	theFieldTrafo.off2[1] = theFieldTrafo.box2[1][0] - theFieldTrafo.mag2[1][0];
	theFieldTrafo.off2[2] = theFieldTrafo.box2[2][0] - theFieldTrafo.mag2[2][0];

	/* Box 1 calculations */
	v1[0] = theFieldTrafo.box1[0][1] - theFieldTrafo.box1[0][0];
	v1[1] = theFieldTrafo.box1[2][1] - theFieldTrafo.box1[2][0];

	v2[0] = theFieldTrafo.mag1[0][1] + theFieldTrafo.off1[0]
			- theFieldTrafo.box1[0][0];
	v2[1] = theFieldTrafo.mag1[2][1] + theFieldTrafo.off1[2]
			- theFieldTrafo.box1[2][0];

	cosa = v1[0] * v2[0] + v1[1] * v2[1];
	cosa /= sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
	cosa /= sqrt(v2[0] * v2[0] + v2[1] * v2[1]);

	theFieldTrafo.alpha1 = acos(cosa);

	/* Box 2 calculations */
	v1[0] = theFieldTrafo.box2[0][1] - theFieldTrafo.box2[0][0];
	v1[1] = theFieldTrafo.box2[2][1] - theFieldTrafo.box2[2][0];

	v2[0] = theFieldTrafo.mag2[0][1] + theFieldTrafo.off2[0]
			- theFieldTrafo.box2[0][0];
	v2[1] = theFieldTrafo.mag2[2][1] + theFieldTrafo.off2[2]
			- theFieldTrafo.box2[2][0];

	cosa = v1[0] * v2[0] + v1[1] * v2[1];
	cosa /= sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
	cosa /= sqrt(v2[0] * v2[0] + v2[1] * v2[1]);

	theFieldTrafo.alpha2 = -acos(cosa);

	/* added the "-" to make it identical with the fortran 6.12.07 */
	theFieldTrafo.status = 1;

	return;
}

/*******************************************************************************/

//TTB coordinates in ALADiN reference frame 

/* get_field() takes a particle (with position information) and delivers the fields. */
/* It does the necessary coordinate transformations.                                 */
TVector3 MagneticField::GetFieldMap(TVector3 Position) 
{
	TVector3 Field;

	//  TTCoordFieldMap my_mapper1;
	TTCoordFieldMap mp1;

	//  TTCoordFieldMap my_mapper2;
	TTCoordFieldMap mp2;

	Float_t xmap = 0.0;
	Float_t ymap = 0.0;
	Float_t zmap = 0.0;
	Float_t xzw = 0.0;
	Float_t zzw = 0.0;

	/* Geometry parameters for measurement boxes */
	float STEP_X = 3.0;
	float STEP_Y = 3.0;
	float STEP_Z = 4.0;

	Float_t pbox1[3] = { 0.0, 0.0, 0.0 };
	Float_t pbox2[3] = { 0.0, 0.0, 0.0 };

	Int_t result = 0;
	/*
	 mp1 = &my_mapper1;
	 mp2 = &my_mapper2;
	 */
	/* Coordinate transformations */

	/* added 09.01.2008 to account for the B=0 for y>19.6 */
	if (Position.Y() > 19.6 && Position.Y() < 22.2) {
		Position.SetY(-Position.Y()); //TTB if between 19.6cm to 22.2 switch the Y coordinate to get still some field
	}

	//TTB now switch to coordinate system and units of maps

	xmap = Position.Z() * 10.0; /* change coordinate system, use [mm]     */
	ymap = Position.Y() * 10.0; /* this coordinate fits already, use [mm] */
	zmap = -Position.X() * 10.0; /* change coordinate system, use [mm]     */

	/* Box 1 coordinates */
	pbox1[0] = xmap + theFieldTrafo.off1[0];
	pbox1[1] = ymap + theFieldTrafo.off1[1];
	pbox1[2] = zmap + theFieldTrafo.off1[2];

	pbox1[0] = pbox1[0] - theFieldTrafo.box1[0][0];
	pbox1[2] = pbox1[2] - theFieldTrafo.box1[2][0];

	xzw = pbox1[0] * cos(-theFieldTrafo.alpha1) + pbox1[2] * sin(
			-theFieldTrafo.alpha1);
	zzw = -pbox1[0] * sin(-theFieldTrafo.alpha1) + pbox1[2] * cos(
			-theFieldTrafo.alpha1);

	pbox1[0] = xzw + theFieldTrafo.box1[0][0];
	pbox1[2] = zzw + theFieldTrafo.box1[2][0];

	/* We have now coordinates in box 1 - need to go to [xx,xy,xz,yy,zz] */
	mp1.pos[0] = (pbox1[0] + 1400.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xx coordinate */
	mp1.pos[1] = (pbox1[0] + 1390.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xy coordinate */
	mp1.pos[2] = (pbox1[0] + 1380.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xz coordinate */
	mp1.pos[3] = (pbox1[1] + 190.0) * 0.1 / STEP_Y + 1.0 + 2.0; /* y coordinate  */
	mp1.pos[4] = (pbox1[2] + 660.0) * 0.1 / STEP_Z + 1.0 + 0.0; /* z coordinate  */
	mp1.field[0] = 0.0;
	mp1.field[1] = 0.0;
	mp1.field[2] = 0.0;
	mp1.status = 0;

	/* Now comes field1_int() */

	/* Box 2 coordinates */
	pbox2[0] = xmap + theFieldTrafo.off2[0];
	pbox2[1] = ymap + theFieldTrafo.off2[1];
	pbox2[2] = zmap + theFieldTrafo.off2[2];

	pbox2[0] = pbox2[0] - theFieldTrafo.box2[0][0];
	pbox2[2] = pbox2[2] - theFieldTrafo.box2[2][0];

	xzw = pbox2[0] * cos(-theFieldTrafo.alpha2) + pbox2[2] * sin(
			-theFieldTrafo.alpha2);
	zzw = -pbox2[0] * sin(-theFieldTrafo.alpha2) + pbox2[2] * cos(
			-theFieldTrafo.alpha2);

	pbox2[0] = xzw + theFieldTrafo.box2[0][0];
	pbox2[2] = zzw + theFieldTrafo.box2[2][0];

	/* We have now coordinates in box 2 - need to go to [xx,xy,xz,yy,zz] */
	mp2.pos[0] = (pbox2[0] + 1250.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xx coordinate */
	mp2.pos[1] = (pbox2[0] + 1240.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xy coordinate */
	mp2.pos[2] = (pbox2[0] + 1230.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xz coordinate */
	mp2.pos[3] = (pbox2[1] + 190.0) * 0.1 / STEP_Y + 1.0 + 2.0; /* y coordinate  */
	mp2.pos[4] = (pbox2[2] - 60.0) * 0.1 / STEP_Z + 1.0 - 1.0; /* z coordinate  */
	mp2.field[0] = 0.0;
	mp2.field[1] = 0.0;
	mp2.field[2] = 0.0;
	mp2.status = 0;

	GetFieldMap1(&mp1);
	GetFieldMap2(&mp2);

	/* Now we do the interpolation between the two measurement boxes, if necessary */
	if (mp1.status == 1) {
		/* First box delivered a measured value */
		xzw = mp1.field[0] * cos(theFieldTrafo.alpha1) + mp1.field[2] * sin(
				theFieldTrafo.alpha1);
		zzw = -mp1.field[0] * sin(theFieldTrafo.alpha1) + mp1.field[2] * cos(
				theFieldTrafo.alpha1);

		mp1.field[0] = -zzw;
		mp1.field[2] = xzw;
	}

	if (mp2.status == 1) {
		/* First box delivered a measured value */
		xzw = mp2.field[0] * cos(theFieldTrafo.alpha2) + mp2.field[2] * sin(
				theFieldTrafo.alpha2);
		zzw = -mp2.field[0] * sin(theFieldTrafo.alpha2) + mp2.field[2] * cos(
				theFieldTrafo.alpha2);

		mp2.field[0] = -zzw;
		mp2.field[2] = xzw;
	}

	if ((mp1.status == 1) && (mp2.status == 1)) {
		/* Both boxes delivered measured field values */
		//TODO: why do first set the max values and then the mean value???!!!
		Field.SetXYZ(
				10.0 * ((fabs(mp1.field[0]) > fabs(mp2.field[0]) ? mp1.field[0]
						: mp2.field[0])),
				10.0 * ((fabs(mp1.field[1]) > fabs(mp2.field[1]) ? mp1.field[1]
						: mp2.field[1])),
				10.0 * ((fabs(mp1.field[2]) > fabs(mp2.field[2]) ? mp1.field[2]
						: mp2.field[2])));

		Field.SetXYZ(5.0 * (mp1.field[0] + mp2.field[0]),
				5.0 * (mp1.field[1] + mp2.field[1]),
				5.0 * (mp1.field[2] + mp2.field[2]));
	}

	if ((mp1.status == 1) && (mp2.status == 0)) {
		/* Only left box delivered measured field values */
		Field.SetXYZ(10.0 * mp1.field[0], 10.0 * mp1.field[1],
				10.0 * mp1.field[2]);
	}

	if ((mp1.status == 0) && (mp2.status == 1)) {
		/* Only right box delivered measured field values */
		Field.SetXYZ(10.0 * mp2.field[0], 10.0 * mp2.field[1],
				10.0 * mp2.field[2]);
	}

	if ((mp1.status == 0) && (mp2.status == 0)) {
		/* No box delivered a measured field values */
		Field.SetXYZ(0., 0., 0.);

	}

	/* added 09.01.2008 to account for the B=0 for y>19.6 */
	if (Position.Y() < -19.6 && Position.Y() > -22.2) {
		Position.SetY(-Position.Y()); // set the coordinate back to before
		Field.SetX(-Field.X());
		Field.SetZ(-Field.Z());
	}

	result = 1 * mp1.status + 2 * mp2.status;

	//	Field=10000.*Field;//FOR TESTING
	//	cout <<setprecision(5);
	//	cout<<"DEBUG: MagneticField::GetFieldMap(): Get field at ("<<Position.X()<<","<<Position.Y()<<","<<Position.Z()<<")"
	//		<<" Field: ("<<Field.X()<<","<<Field.Y()<<","<<Field.Z()<<")"<<endl;

	return Field;
}

/*******************************************************************************/

void MagneticField::GetFieldMap1(TTCoordFieldMap *mp1) 
{
	Int_t ix1 = 0;
	Int_t ix2 = 0;
	Int_t iy1 = 0;
	Int_t iy2 = 0;
	Int_t iz1 = 0;
	Int_t iz2 = 0;
	Int_t store1 = 0;
	Int_t store2 = 0;
	Float_t fx = 0.0;
	Float_t fy = 0.0;
	Float_t fz = 0.0;
	Int_t limx[21] = { 91, 86, 77, 69, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			69, 77, 86, 91 };
	Float_t g[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	/* Calculate for xx position */
	ix1 = (Int_t)(mp1->pos[0]);
	if (mp1->pos[0] < 0) {
		ix1--;
	}
	ix2 = ix1 + 1;

	iy1 = (Int_t)(mp1->pos[3]);
	if (mp1->pos[3] < 0) {
		iy1--;
	}
	iy2 = iy1 + 1;

	iz1 = (Int_t)(mp1->pos[4]);
	if (mp1->pos[4] < 0) {
		iz1--;
	}
	iz2 = iz1 + 1;

	store1 = iy1;
	store2 = iy2;

	if ((ix2 > theFieldMap.li - 1) || (ix1 < 0) || (iz2 > theFieldMap.lk - 1)
			|| (iz1 < 0)) {
		/* Out of x and z boundaries */
		mp1->field[0] = 0.0;
		mp1->field[1] = 0.0;
		mp1->field[2] = 0.0;
		mp1->status = 0;
		return;
	}

	if ((iy2 > theFieldMap.lj - 1) || (iy2 < 0)) {
		/* at least one y delimiter is out of field map */
		if ((iy1 >= -3 + 2) && (iy2 <= 17 + 2)) /* -3:17 */
		{
			/* our point is just on the border line */
			if ((ix1 - 1 < limx[iy1 - 2 + 3]) || (ix1 - 1 < limx[iy2 - 2 + 3])) {
				if ((ix1 - 1 < limx[iy1 - 2 + 3]) && (ix1 - 1 >= limx[iy1 - 2
						+ 1 + 3])) {
					iy1++;
					iy2++;
				} else if ((ix1 - 1 < limx[iy2 - 2 + 3]) && (ix1 - 1
						>= limx[iy2 - 2 - 1 + 3])) {
					iy1--;
					iy2--;
				} else {
					mp1->field[0] = 0.0;
					mp1->field[1] = 0.0;
					mp1->field[2] = 0.0;
					mp1->status = 0;
					return;
				}
			}
		} else {
			mp1->field[0] = 0.0;
			mp1->field[1] = 0.0;
			mp1->field[2] = 0.0;
			mp1->status = 0;
			return;
		}
	}

	fx = mp1->pos[0] - ix1;
	fy = mp1->pos[3] - iy1;
	fz = mp1->pos[4] - iz1;

	if ((ix1 < 0) || (ix1 > theFieldMap.li - 1) || (ix2 < 0) || (ix2
			> theFieldMap.li - 1) || (iy1 < 0) || (iy1 > theFieldMap.lj - 1)
			|| (iy2 < 0) || (iy2 > theFieldMap.lj - 1) || (iz1 < 0) || (iz1
			> theFieldMap.lk - 1) || (iz2 < 0) || (iz2 > theFieldMap.lk - 1)) {
		//      printf("# LMAPX ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
		//	     ix1, theFieldMap.li-1, ix2, iy1, theFieldMap.lj-1, iy2, iz1, theFieldMap.lk-1, iz2 );
	}

	g[0] = theFieldMap.lbx[ix1][iy1][iz1] + fx
			* (theFieldMap.lbx[ix2][iy1][iz1] - theFieldMap.lbx[ix1][iy1][iz1]);
	g[1] = theFieldMap.lbx[ix1][iy2][iz1] + fx
			* (theFieldMap.lbx[ix2][iy2][iz1] - theFieldMap.lbx[ix1][iy2][iz1]);
	g[2] = theFieldMap.lbx[ix1][iy2][iz2] + fx
			* (theFieldMap.lbx[ix2][iy2][iz2] - theFieldMap.lbx[ix1][iy2][iz2]);
	g[3] = theFieldMap.lbx[ix1][iy1][iz2] + fx
			* (theFieldMap.lbx[ix2][iy1][iz2] - theFieldMap.lbx[ix1][iy1][iz2]);
	g[4] = g[0] + fy * (g[1] - g[0]);
	g[5] = g[3] + fy * (g[2] - g[3]);

	mp1->field[0] = g[4] + fz * (g[5] - g[4]);

	/* xx part done */

	/* Calculate for xy position */
	iy1 = store1;
	iy2 = store2;

	ix1 = (Int_t)(mp1->pos[1]);
	if (mp1->pos[1] < 0) {
		ix1--;
	}
	ix2 = ix1 + 1;

	if ((ix2 > theFieldMap.li - 1) || (ix1 < 0)) {
		mp1->field[0] = 0.0;
		mp1->field[1] = 0.0;
		mp1->field[2] = 0.0;
		mp1->status = 0;
		return;
	}

	if ((iy1 >= -3 + 2) && (iy2 <= 17 + 2)) {
		if ((ix1 - 1 < limx[iy1 - 2 + 3]) || (ix1 - 1 < limx[iy2 - 2 + 3])) {
			if ((ix1 - 1 < limx[iy1 - 2 + 3]) && (ix1 - 1 >= limx[iy1 - 2 + 1
					+ 3])) {
				iy1++;
				iy2++;
			} else if ((ix1 - 1 < limx[iy2 - 2 + 3]) && (ix1 - 1 >= limx[iy2
					- 2 - 1 + 3])) {
				iy1--;
				iy2--;
			} else {
				mp1->field[0] = 0.0;
				mp1->field[1] = 0.0;
				mp1->field[2] = 0.0;
				mp1->status = 0;
				return;
			}
		}
	} else {
		mp1->field[0] = 0.0;
		mp1->field[1] = 0.0;
		mp1->field[2] = 0.0;
		mp1->status = 0;
		return;
	}

	fx = mp1->pos[1] - ix1;

	if ((ix1 < 0) || (ix1 > theFieldMap.li - 1) || (ix2 < 0) || (ix2
			> theFieldMap.li - 1) || (iy1 < 0) || (iy1 > theFieldMap.lj - 1)
			|| (iy2 < 0) || (iy2 > theFieldMap.lj - 1) || (iz1 < 0) || (iz1
			> theFieldMap.lk - 1) || (iz2 < 0) || (iz2 > theFieldMap.lk - 1)) {
		/*
		 *       printf("# LMAPY ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
		 * 	     ix1, theFieldMap.li-1, ix2, iy1, theFieldMap.lj-1, iy2, iz1, theFieldMap.lk-1, iz2 );
		 */
	}

	g[0] = theFieldMap.lby[ix1][iy1][iz1] + fx
			* (theFieldMap.lby[ix2][iy1][iz1] - theFieldMap.lby[ix1][iy1][iz1]);
	g[1] = theFieldMap.lby[ix1][iy2][iz1] + fx
			* (theFieldMap.lby[ix2][iy2][iz1] - theFieldMap.lby[ix1][iy2][iz1]);
	g[2] = theFieldMap.lby[ix1][iy2][iz2] + fx
			* (theFieldMap.lby[ix2][iy2][iz2] - theFieldMap.lby[ix1][iy2][iz2]);
	g[3] = theFieldMap.lby[ix1][iy1][iz2] + fx
			* (theFieldMap.lby[ix2][iy1][iz2] - theFieldMap.lby[ix1][iy1][iz2]);
	g[4] = g[0] + fy * (g[1] - g[0]);
	g[5] = g[3] + fy * (g[2] - g[3]);

	mp1->field[1] = g[4] + fz * (g[5] - g[4]);

	/* xy part done */

	/* Calculate for xz position */
	iy1 = store1;
	iy2 = store2;

	ix1 = (Int_t)(mp1->pos[2]);
	if (mp1->pos[2] < 0) {
		ix1--;
	}
	ix2 = ix1 + 1;

	if ((ix2 > theFieldMap.li - 1) || (ix1 < 0)) {
		mp1->field[0] = 0.0;
		mp1->field[1] = 0.0;
		mp1->field[2] = 0.0;
		mp1->status = 0;
		return;
	}

	if ((iy1 >= -3 + 2) && (iy2 <= 17 + 2)) {
		if ((ix1 - 1 < limx[iy1 - 2 + 3]) || (ix1 - 1 < limx[iy2 - 2 + 3])) {
			if ((ix1 - 1 < limx[iy1 - 2 + 3]) && (ix1 - 1 >= limx[iy1 - 2 + 1
					+ 3])) {
				iy1++;
				iy2++;
			} else if ((ix1 - 1 < limx[iy2 - 2 + 3]) && (ix1 - 1 >= limx[iy2
					- 2 - 1 + 3])) {
				iy1--;
				iy2--;
			} else {
				mp1->field[0] = 0.0;
				mp1->field[1] = 0.0;
				mp1->field[2] = 0.0;
				mp1->status = 0;
				return;
			}
		}
	} else {
		mp1->field[0] = 0.0;
		mp1->field[1] = 0.0;
		mp1->field[2] = 0.0;
		mp1->status = 0;
		return;
	}

	fx = mp1->pos[2] - ix1;

	if ((ix1 < 0) || (ix1 > theFieldMap.li - 1) || (ix2 < 0) || (ix2
			> theFieldMap.li - 1) || (iy1 < 0) || (iy1 > theFieldMap.lj - 1)
			|| (iy2 < 0) || (iy2 > theFieldMap.lj - 1) || (iz1 < 0) || (iz1
			> theFieldMap.lk - 1) || (iz2 < 0) || (iz2 > theFieldMap.lk - 1)) {
		/*
		 *       printf("# LMAPZ ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
		 * 	     ix1, theFieldMap.li-1, ix2, iy1, theFieldMap.lj-1, iy2, iz1, theFieldMap.lk-1, iz2 );
		 */
	}

	g[0] = theFieldMap.lbz[ix1][iy1][iz1] + fx
			* (theFieldMap.lbz[ix2][iy1][iz1] - theFieldMap.lbz[ix1][iy1][iz1]);
	g[1] = theFieldMap.lbz[ix1][iy2][iz1] + fx
			* (theFieldMap.lbz[ix2][iy2][iz1] - theFieldMap.lbz[ix1][iy2][iz1]);
	g[2] = theFieldMap.lbz[ix1][iy2][iz2] + fx
			* (theFieldMap.lbz[ix2][iy2][iz2] - theFieldMap.lbz[ix1][iy2][iz2]);
	g[3] = theFieldMap.lbz[ix1][iy1][iz2] + fx
			* (theFieldMap.lbz[ix2][iy1][iz2] - theFieldMap.lbz[ix1][iy1][iz2]);
	g[4] = g[0] + fy * (g[1] - g[0]);
	g[5] = g[3] + fy * (g[2] - g[3]);

	mp1->field[2] = g[4] + fz * (g[5] - g[4]);

	mp1->status = 1;

	/* xz part done */

	return;
}

/*******************************************************************************/

void MagneticField::GetFieldMap2(TTCoordFieldMap *mp2) 
{
	Int_t ix1 = 0;
	Int_t ix2 = 0;
	Int_t iy1 = 0;
	Int_t iy2 = 0;
	Int_t iz1 = 0;
	Int_t iz2 = 0;
	Float_t fx = 0.0;
	Float_t fy = 0.0;
	Float_t fz = 0.0;
	Int_t limx[21] = { 91, 82, 73, 64, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			64, 67, 75, 91 };
	Float_t g[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	/* Calculate for xx position */
	ix1 = (Int_t)(mp2->pos[0]);
	if (mp2->pos[0] < 0) {
		ix1--;
	}
	ix2 = ix1 + 1;

	iy1 = (Int_t)(mp2->pos[3]);
	if (mp2->pos[3] < 0) {
		iy1--;
	}
	iy2 = iy1 + 1;

	iz1 = (Int_t)(mp2->pos[4]);
	if (mp2->pos[4] < 0) {
		iz1--;
	}
	iz2 = iz1 + 1;

	/*
	 *   printf("# get_field2(): xx: (%d|%7.2f|%d) (%d|%7.2f|%d) (%d|%7.2f|%d) \n", 
	 * 	 ix1, mp2->pos[0], ix2, iy1, mp2->pos[3], iy2, iz1, mp2->pos[4], iz2);
	 */

	if ((ix2 > theFieldMap.ri - 1) || (ix1 < 0) || (iz2 > theFieldMap.rk - 1)
			|| (iz1 < 0)) {
		/* Out of x and z boundaries */
		mp2->field[0] = 0.0;
		mp2->field[1] = 0.0;
		mp2->field[2] = 0.0;
		mp2->status = 0;
		return;
	}

	if ((iy2 > theFieldMap.rj - 1) || (iy2 < 0)) {
		/* at least one y delimiter is out of field map */
		if ((iy1 >= -3 + 2) && (iy2 <= 17 + 2)) /* -3:17 */
		{
			/* our point is just on the border line */
			if ((ix1 - 1 < limx[iy1 - 2 + 3]) || (ix1 - 1 < limx[iy2 - 2 + 3])) {
				if ((ix1 - 1 < limx[iy1 - 2 + 3]) && (ix1 - 1 >= limx[iy1 - 2
						+ 1 + 3])) {
					iy1++;
					iy2++;
				} else if ((ix1 - 1 < limx[iy2 - 2 + 3]) && (ix1 - 1
						>= limx[iy2 - 2 - 1 + 3])) {
					iy1--;
					iy2--;
				} else {
					mp2->field[0] = 0.0;
					mp2->field[1] = 0.0;
					mp2->field[2] = 0.0;
					mp2->status = 0;
					return;
				}
			}
		} else {
			mp2->field[0] = 0.0;
			mp2->field[1] = 0.0;
			mp2->field[2] = 0.0;
			mp2->status = 0;
			return;
		}
	}

	fx = mp2->pos[0] - ix1;
	fy = mp2->pos[3] - iy1;
	fz = mp2->pos[4] - iz1;

	if ((ix1 < 0) || (ix1 > theFieldMap.ri - 1) || (ix2 < 0) || (ix2
			> theFieldMap.ri - 1) || (iy1 < 0) || (iy1 > theFieldMap.rj - 1)
			|| (iy2 < 0) || (iy2 > theFieldMap.rj - 1) || (iz1 < 0) || (iz1
			> theFieldMap.rk - 1) || (iz2 < 0) || (iz2 > theFieldMap.rk - 1)) {
		//      printf("# RMAPX ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
		//	     ix1, theFieldMap.ri-1, ix2, iy1, theFieldMap.rj-1, iy2, iz1, theFieldMap.rk-1, iz2 );
	}

	g[0] = theFieldMap.rbx[ix1][iy1][iz1] + fx
			* (theFieldMap.rbx[ix2][iy1][iz1] - theFieldMap.rbx[ix1][iy1][iz1]);
	g[1] = theFieldMap.rbx[ix1][iy2][iz1] + fx
			* (theFieldMap.rbx[ix2][iy2][iz1] - theFieldMap.rbx[ix1][iy2][iz1]);
	g[2] = theFieldMap.rbx[ix1][iy2][iz2] + fx
			* (theFieldMap.rbx[ix2][iy2][iz2] - theFieldMap.rbx[ix1][iy2][iz2]);
	g[3] = theFieldMap.rbx[ix1][iy1][iz2] + fx
			* (theFieldMap.rbx[ix2][iy1][iz2] - theFieldMap.rbx[ix1][iy1][iz2]);
	g[4] = g[0] + fy * (g[1] - g[0]);
	g[5] = g[3] + fy * (g[2] - g[3]);

	mp2->field[0] = g[4] + fz * (g[5] - g[4]);

	/* xx part done */

	/* Calculate for xy position */
	ix1 = (Int_t)(mp2->pos[1]);
	if (mp2->pos[1] < 0) {
		ix1--;
	}
	ix2 = ix1 + 1;

	if ((ix2 > theFieldMap.ri - 1) || (ix1 < 0)) {
		mp2->field[0] = 0.0;
		mp2->field[1] = 0.0;
		mp2->field[2] = 0.0;
		mp2->status = 0;
		return;
	}

	if ((iy1 >= -3 + 2) && (iy2 <= 17 + 2)) {
		if ((ix1 - 1 < limx[iy1 - 2 + 3]) || (ix1 - 1 < limx[iy2 - 2 + 3])) {
			if ((ix1 - 1 < limx[iy1 - 2 + 3]) && (ix1 - 1 >= limx[iy1 - 2 + 1
					+ 3])) {
				iy1++;
				iy2++;
			} else if ((ix1 - 1 < limx[iy2 - 2 + 3]) && (ix1 - 1 >= limx[iy2
					- 2 - 1 + 3])) {
				iy1--;
				iy2--;
			} else {
				mp2->field[0] = 0.0;
				mp2->field[1] = 0.0;
				mp2->field[2] = 0.0;
				mp2->status = 0;
				return;
			}
		}
	} else {
		mp2->field[0] = 0.0;
		mp2->field[1] = 0.0;
		mp2->field[2] = 0.0;
		mp2->status = 0;
		return;
	}

	fx = mp2->pos[1] - ix1;

	if ((ix1 < 0) || (ix1 > theFieldMap.ri - 1) || (ix2 < 0) || (ix2
			> theFieldMap.ri - 1) || (iy1 < 0) || (iy1 > theFieldMap.rj - 1)
			|| (iy2 < 0) || (iy2 > theFieldMap.rj - 1) || (iz1 < 0) || (iz1
			> theFieldMap.rk - 1) || (iz2 < 0) || (iz2 > theFieldMap.rk - 1)) {
		/*
		 *       printf("# RMAPY ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
		 * 	     ix1, theFieldMap.ri-1, ix2, iy1, theFieldMap.rj-1, iy2, iz1, theFieldMap.rk-1, iz2 );
		 */
	}

	g[0] = theFieldMap.rby[ix1][iy1][iz1] + fx
			* (theFieldMap.rby[ix2][iy1][iz1] - theFieldMap.rby[ix1][iy1][iz1]);
	g[1] = theFieldMap.rby[ix1][iy2][iz1] + fx
			* (theFieldMap.rby[ix2][iy2][iz1] - theFieldMap.rby[ix1][iy2][iz1]);
	g[2] = theFieldMap.rby[ix1][iy2][iz2] + fx
			* (theFieldMap.rby[ix2][iy2][iz2] - theFieldMap.rby[ix1][iy2][iz2]);
	g[3] = theFieldMap.rby[ix1][iy1][iz2] + fx
			* (theFieldMap.rby[ix2][iy1][iz2] - theFieldMap.rby[ix1][iy1][iz2]);
	g[4] = g[0] + fy * (g[1] - g[0]);
	g[5] = g[3] + fy * (g[2] - g[3]);

	mp2->field[1] = g[4] + fz * (g[5] - g[4]);

	/* xy part done */

	/* Calculate for xz position */
	ix1 = (Int_t)(mp2->pos[2]);
	if (mp2->pos[2] < 0) {
		ix1--;
	}
	ix2 = ix1 + 1;

	if ((ix2 > theFieldMap.ri - 1) || (ix1 < 0)) {
		mp2->field[0] = 0.0;
		mp2->field[1] = 0.0;
		mp2->field[2] = 0.0;
		mp2->status = 0;
		return;
	}

	if ((iy1 >= -3 + 2) && (iy2 <= 17 + 2)) {
		if ((ix1 - 1 < limx[iy1 - 2 + 3]) || (ix1 - 1 < limx[iy2 - 2 + 3])) {
			if ((ix1 - 1 < limx[iy1 - 2 + 3]) && (ix1 - 1 >= limx[iy1 - 2 + 1
					+ 3])) {
				iy1++;
				iy2++;
			} else if ((ix1 - 1 < limx[iy2 - 2 + 3]) && (ix1 - 1 >= limx[iy2
					- 2 - 1 + 3])) {
				iy1--;
				iy2--;
			} else {
				mp2->field[0] = 0.0;
				mp2->field[1] = 0.0;
				mp2->field[2] = 0.0;
				mp2->status = 0;
				return;
			}
		}
	} else {
		mp2->field[0] = 0.0;
		mp2->field[1] = 0.0;
		mp2->field[2] = 0.0;
		mp2->status = 0;
		return;
	}

	fx = mp2->pos[2] - ix1;

	if ((ix1 < 0) || (ix1 > theFieldMap.ri - 1) || (ix2 < 0) || (ix2
			> theFieldMap.ri - 1) || (iy1 < 0) || (iy1 > theFieldMap.rj - 1)
			|| (iy2 < 0) || (iy2 > theFieldMap.rj - 1) || (iz1 < 0) || (iz1
			> theFieldMap.rk - 1) || (iz2 < 0) || (iz2 > theFieldMap.rk - 1)) {
		//      printf("# RMAPZ ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
		//	     ix1, theFieldMap.ri-1, ix2, iy1, theFieldMap.rj-1, iy2, iz1, theFieldMap.rk-1, iz2 );
	}
	g[0] = theFieldMap.rbz[ix1][iy1][iz1] + fx
			* (theFieldMap.rbz[ix2][iy1][iz1] - theFieldMap.rbz[ix1][iy1][iz1]);
	g[1] = theFieldMap.rbz[ix1][iy2][iz1] + fx
			* (theFieldMap.rbz[ix2][iy2][iz1] - theFieldMap.rbz[ix1][iy2][iz1]);
	g[2] = theFieldMap.rbz[ix1][iy2][iz2] + fx
			* (theFieldMap.rbz[ix2][iy2][iz2] - theFieldMap.rbz[ix1][iy2][iz2]);
	g[3] = theFieldMap.rbz[ix1][iy1][iz2] + fx
			* (theFieldMap.rbz[ix2][iy1][iz2] - theFieldMap.rbz[ix1][iy1][iz2]);
	g[4] = g[0] + fy * (g[1] - g[0]);
	g[5] = g[3] + fy * (g[2] - g[3]);

	mp2->field[2] = g[4] + fz * (g[5] - g[4]);

	mp2->status = 1;

	/* xz part done */

	return;
}

/*******************************************************************************/

/* Fill the bx by bz fields from mapping files, do interpolation between different */
/* current settings if needed.                                                     */
/* Bug found: for the "right" box, k index was not corrected by -1                 */

Int_t MagneticField::FillFieldMap() 
{
	cout << "Enter FillFieldMap()" << endl;
	Int_t current = int(fCurrent + 0.5);//set the current in A
	static const Int_t AladinMapFilesNb = 10;

	Int_t match = 0;
	char filel[80];
	char fileh[80];
	FILE *infilel = NULL;
	FILE *infileh = NULL;
	char linel[255];
	char lineh[255];
	Int_t result = 0;
	Int_t i = 0;

	/* File I/O variables */
	Int_t fm_current[AladinMapFilesNb] = { 0, 500, 1100, 1300, 1500, 1700,
			1900, 2100, 2300, 2500 };
	Int_t fm_match = 0;
	Int_t fm_low = 0;
	Int_t fm_high = 0;

	/* Variables read by file (low field) */
	Int_t rdl_current = 0;
	Int_t rdl_box = 0;
	Int_t rdl_i = 0;
	Int_t rdl_j = 0;
	Int_t rdl_k = 0;
	Float_t rdl_bx = 0.;
	Float_t rdl_by = 0.;
	Float_t rdl_bz = 0.;
	Float_t rdl_b = 0.;
	/* Variables read by file (high field) */
	Int_t rdh_current = 0;
	Int_t rdh_box = 0;
	Int_t rdh_i = 0;
	Int_t rdh_j = 0;
	Int_t rdh_k = 0;
	Float_t rdh_bx = 0.;
	Float_t rdh_by = 0.;
	Float_t rdh_bz = 0.;
	Float_t rdh_b = 0.;

	//cout << "DEBUG: We are in FillFieldMap()." << endl;

	/* Check for reasonable setting of magnet current */
	if ((current < kAladinCurrentMin) || (current > kAladinCurrentMax)) {
		/* Bad... current is out of range */
		printf("# FillFieldMap(): I_Aladin (%4d) is out of range [%d:%d]\n",
				current, kAladinCurrentMin, kAladinCurrentMax);
		result = -10;
		return result;
	}

	/* We are in range, so we can go on */
	printf("# FillFieldMap(): I_Aladin (%4d) is in range [%d:%d].\n", current,
			kAladinCurrentMin, kAladinCurrentMax);

	/* We check for an existing field map */
	fm_match = -1;
	for (i = 0; i < AladinMapFilesNb; i++) {
		if (current == fm_current[i]) {
			fm_match = i;
		}
		if (fm_current[i] < current) {
			fm_low = i;
			fm_high = i + 1;
		}
	}

	printf("# match: %2d low: %2d high: %2d\n", fm_match, fm_low, fm_high);
	if (fm_match != -1) {
		/* We have found a matching measured field map */
		printf("# Matching field map found: %4d (%4d).\n",
				fm_current[fm_match], current);
		fm_low = fm_current[fm_match];
		fm_high = fm_current[fm_match];

		sprintf(filel, "%s/fieldmap/ala_%04d.dat", m_dir.Data(), fm_low);
		sprintf(fileh, "%s/fieldmap/ala_%04d.dat", m_dir.Data(), fm_high);
		printf("# FillFieldMap(): low  -> %s\n", filel);
		printf("# FillFieldMap(): high -> %s\n", fileh);

		/* We have a matching file */
		match = 1;
		printf("# FillFieldMap(): matching mapping available.\n");

		infilel = fopen(filel, "r");
		if (infilel == NULL) {
			/* We couldn't open the input file */
			printf("# ERROR: FillFieldMap() couldn't open %s.\n", filel);
			result = -10;
			return result;
		} else {
			/* We go ahead - read the stuff */
			while ((fgets(linel, 256, infilel) != NULL)) {

				/* We could read one line */
				if (linel[0] != '#') {
					/* We have a normal data line, not a comment line */
					//		  if( sscanf(linel, "%d %d %d %d %d %f %f %f %f", 
					if (sscanf(linel, "%d %d %d %d %d %e %e %e %e",
							&rdl_current, &rdl_box, &rdl_k, &rdl_j, &rdl_i,
							&rdl_bx, &rdl_by, &rdl_bz, &rdl_b) == 9) {
						/* Good input lines */

						/* Now sort it into the field map */
						if (rdl_box == 0) {
							if ((rdl_i - 1 < 0) || (rdl_i - 1 > LBI - 1)
									|| (rdl_j + 2 < 0) || (rdl_j + 2 > LBJ - 1)
									|| (rdl_k < 0) || (rdl_k > LBK - 1)) {
							} else {
								/* Left measurement box */
								theFieldMap.lbx[rdl_i - 1][rdl_j + 2][rdl_k]
										= rdl_bx;
								theFieldMap.lby[rdl_i - 1][rdl_j + 2][rdl_k]
										= rdl_by;
								theFieldMap.lbz[rdl_i - 1][rdl_j + 2][rdl_k]
										= rdl_bz;

								//								cout <<"DEBUG OLD: "<< rdl_bx << " " << rdl_by << " "
								//										<< rdl_bz << endl;

							}
						} else if (rdl_box == 1) {
							if ((rdl_i - 1 < 0) || (rdl_i - 1 > RBI - 1)
									|| (rdl_j + 2 < 0) || (rdl_j + 2 > RBJ - 1)
									|| (rdl_k - 1 < 0) || (rdl_k - 1 > RBK - 1)) {
							}
							{
								/* Right measurement box */
								theFieldMap.rbx[rdl_i - 1][rdl_j + 2][rdl_k - 1]
										= rdl_bx; /* I'm not sure about the "-1" term here */
								theFieldMap.rby[rdl_i - 1][rdl_j + 2][rdl_k - 1]
										= rdl_by;
								theFieldMap.rbz[rdl_i - 1][rdl_j + 2][rdl_k - 1]
										= rdl_bz;
								//cout <<"DEBUG OLD: "<< rdl_bx << " " << rdl_by << " "
								//	<< rdl_bz << endl;

							}
						} else {
							/* Error: non-existing measurement box */
							printf(
									"# ERROR: FillFieldMap(): non-existing box to fill.\n");
							result = -1;
						}
					} else {
						/* Bad input line */
						printf("# ERROR: FillFieldMap(): bad input line.\n");
						result = -1;
					}
				}
			} /* end of read loop */
		} /* end of non-interpolated case */

	} else {
		/* We must interpolate between two currents */
		printf("# Interpolation between %4d (%4d) %4d.\n", fm_current[fm_low],
				current, fm_current[fm_high]);
		fm_low = fm_current[fm_low];
		fm_high = fm_current[fm_high];

		sprintf(filel, "%s/fieldmap/ala_%04d.dat", m_dir.Data(), fm_low);
		sprintf(fileh, "%s/fieldmap/ala_%04d.dat", m_dir.Data(), fm_high);
		printf("# FillFieldMap(): low  -> %s\n", filel);
		printf("# FillFieldMap(): high -> %s\n", fileh);

		/* We must interpolate between two current settings */
		match = 0;
		printf("# FillFieldMap(): interpolation necessary.\n");

		infilel = fopen(filel, "r");
		infileh = fopen(fileh, "r");
		if ((infilel == NULL) || (infileh == NULL)) {
			/* We couldn't open the input file */
			printf("# ERROR: FillFieldMap() couldn't open %s or %s.\n", filel,
					fileh);
			result = -10;
			return result;
		} else {
			/* We go ahead - read the stuff */

			/* ################## */

			while ((fgets(linel, 256, infilel) != NULL) && (fgets(lineh, 256,
					infileh) != NULL)) {
				/* We could read one line */
				if ((linel[0] != '#') && (lineh[0] != '#')) {
					/* We have a normal data line, not a comment line */
					//		  if( (sscanf(linel, "%d %d %d %d %d %f %f %f %f", 
					if ((sscanf(linel, "%d %d %d %d %d %g %g %g %g",
							&rdl_current, &rdl_box, &rdl_k, &rdl_j, &rdl_i,
							&rdl_bx, &rdl_by, &rdl_bz, &rdl_b) == 9) &&
					//		      (sscanf(lineh, "%d %d %d %d %d %f %f %f %f", 
							(sscanf(lineh, "%d %d %d %d %d %g %g %g %g",
									&rdh_current, &rdh_box, &rdh_k, &rdh_j,
									&rdh_i, &rdh_bx, &rdh_by, &rdh_bz, &rdh_b)
									== 9)) {
						/* Good input line */
						if ((rdl_box != rdh_box) || (rdl_i != rdh_i) || (rdl_j
								!= rdh_j) || (rdl_k != rdh_k)) {
							/*
							 * 			  printf("# WARNING! (%d|%d) (%d|%d) (%d|%d) (%d|%d)\n",
							 * 				 rdl_box, rdh_box, rdl_i, rdh_i, rdl_j, rdh_j, rdl_k, rdh_k);
							 * 			  printf("# low : %s", linel);
							 * 			  printf("# high: %s", lineh);
							 */
						}

						if (rdl_box == 0) {

							if ((rdl_i - 1 < 0) || (rdl_i - 1 > LBI - 1)
									|| (rdl_j + 2 < 0) || (rdl_j + 2 > LBJ - 1)
									|| (rdl_k < 0) || (rdl_k > LBK - 1)) {
								printf("# ARGL! IL %d (%d) %d (%d) %d (%d)\n",
										rdl_i - 1, LBI - 1, rdl_j + 2, LBJ - 1,
										rdl_k, LBK - 1);
							} else {
								/* Left measurement box */
								theFieldMap.lbx[rdl_i - 1][rdl_j + 2][rdl_k]
										= rdl_bx + (current - fm_low) * (rdh_bx
												- rdl_bx) / (fm_high - fm_low);
								theFieldMap.lby[rdl_i - 1][rdl_j + 2][rdl_k]
										= rdl_by + (current - fm_low) * (rdh_by
												- rdl_by) / (fm_high - fm_low);
								theFieldMap.lbz[rdl_i - 1][rdl_j + 2][rdl_k]
										= rdl_bz + (current - fm_low) * (rdh_bz
												- rdl_bz) / (fm_high - fm_low);

							}

						} else if (rdl_box == 1) {
							/* Right measurement box */
							if ((rdl_i - 1 < 0) || (rdl_i - 1 > RBI - 1)
									|| (rdl_j + 2 < 0) || (rdl_j + 2 > RBJ - 1)
									|| (rdl_k - 1 < 0) || (rdl_k - 1 > RBK - 1)) {

								printf("# ARGL! IR %d (%d) %d (%d) %d (%d)\n",
										rdl_i - 1, RBI - 1, rdl_j + 2, RBJ - 1,
										rdl_k - 1, RBK - 1);

							} else {

								theFieldMap.rbx[rdl_i - 1][rdl_j + 2][rdl_k - 1]
										= rdl_bx + (current - fm_low) * (rdh_bx
												- rdl_bx) / (fm_high - fm_low);
								theFieldMap.rby[rdl_i - 1][rdl_j + 2][rdl_k - 1]
										= rdl_by + (current - fm_low) * (rdh_by
												- rdl_by) / (fm_high - fm_low);
								theFieldMap.rbz[rdl_i - 1][rdl_j + 2][rdl_k - 1]
										= rdl_bz + (current - fm_low) * (rdh_bz
												- rdl_bz) / (fm_high - fm_low);

							}

						} else {
							/* Error: non-existing measurement box */
							printf(
									"# ERROR: FillFieldMap(): non-existing box to fill.\n");
							result = -1;
						}

					} else {
						/* Bad input line */
						printf("# ERROR: FillFieldMap(): bad input line.\n");
						result = -1;
					}
				}
			} /* end of read loop */
		} /* end of interpolated area */
	}

	/* Clean up */
	if (infilel != NULL) {
		fclose(infilel);
	}
	if (infileh != NULL) {
		fclose(infileh);
	}

	if (result == 0) {
		theFieldMap.lstatus = 1;
		theFieldMap.rstatus = 1;
		theFieldMap.li = LBI;
		theFieldMap.lj = LBJ;
		theFieldMap.lk = LBK;
		theFieldMap.ri = RBI;
		theFieldMap.rj = RBJ;
		theFieldMap.rk = RBK;
	} else {
		theFieldMap.lstatus = -1;
		theFieldMap.rstatus = -1;
		theFieldMap.li = -1;
		theFieldMap.lj = -1;
		theFieldMap.lk = -1;
		theFieldMap.ri = -1;
		theFieldMap.rj = -1;
		theFieldMap.rk = -1;
	}

	cout << "Exit FillFieldMap()" << endl;

	return result;
}

/*******************************************************************************/

//TTB coordinates in ALADiN reference frame
//this returns values from the simple map!
Bool_t MagneticField::IsInsideBox(TVector3 Point, TVector3 BoxPosition, TVector3 BoxDimension){
  
  
  if ( (fabs(BoxDimension.X())>fabs(Point.X()-BoxPosition.X()))
       &&(fabs(BoxDimension.Y())>fabs(Point.Y()-BoxPosition.Y()))
       &&(fabs(BoxDimension.Z())>fabs(Point.Z()-BoxPosition.Z())) ){
    return true;	
  }else{
    return false;	
  }
  
}

TVector3 MagneticField::GetFieldMapSimple(TVector3 Position) 
{
	TVector3 Field(0., 0., 0.);

   // check if point is in magnetic field
	if(!IsInsideBox(Position,TVector3(0.,0.,0.),0.5*fSimpleMapRange)){
	  return Field;
	}


   //fractional position within magnetic field region
   TVector3 Fraction= Position-fSimpleMapMin;
   Fraction.SetXYZ(Fraction.X()/fSimpleMapRange.X(),
				   Fraction.Y()/fSimpleMapRange.Y(),
				   Fraction.Z()/fSimpleMapRange.Z());

   //get the indices of the nearest tabulated point whose coordinates are all less than those of the given point
   Int_t Index[3];
   Index[0]=(int)(Fraction.X()*(fSimpleMapXN-1));
   Index[1]=(int)(Fraction.Y()*(fSimpleMapYN-1));
   Index[2]=(int)(Fraction.Z()*(fSimpleMapZN-1));

   //get fractional part
   Double_t IndexFrac[3];
   IndexFrac[0]=Fraction.X()*(fSimpleMapXN-1)-Index[0];
   IndexFrac[1]=Fraction.Y()*(fSimpleMapYN-1)-Index[1];
   IndexFrac[2]=Fraction.Z()*(fSimpleMapZN-1)-Index[2];

   //check that we are not at upper boundaries of field
   if((Index[0]==fSimpleMapXN-1)||(Index[1]==fSimpleMapYN-1)||(Index[2]==fSimpleMapZN-1)){
	  return Field;
}
// trilinear interpolation
//cout<<"DEBUG: index:"<<Index[0]<<" "<<Index[1]<<" "<<Index[2]<<endl;
	Field=fSimpleMap[Index[0]][Index[1]][Index[2]].Field
		*(1-IndexFrac[0])*(1-IndexFrac[1])*(1-IndexFrac[2])

		+fSimpleMap[Index[0]][Index[1]][Index[2]+1].Field
		*(1-IndexFrac[0])*(1-IndexFrac[1])*  IndexFrac[2]
		+fSimpleMap[Index[0]][Index[1]+1][Index[2]].Field
		*(1-IndexFrac[0])*    IndexFrac[1]*(1-IndexFrac[2])
		+fSimpleMap[Index[0]+1][Index[1]][Index[2]].Field
		*    IndexFrac[0]*(1-IndexFrac[1])*(1-IndexFrac[2])

		+fSimpleMap[Index[0]][Index[1]+1][Index[2]+1].Field
		*(1-IndexFrac[0])*   IndexFrac[1]*  IndexFrac[2]
		+fSimpleMap[Index[0]+1][Index[1]+1][Index[2]].Field
		*   IndexFrac[0] *    IndexFrac[1]*(1-IndexFrac[2])
		+fSimpleMap[Index[0]+1][Index[1]][Index[2]+1].Field
		*    IndexFrac[0]*(1-IndexFrac[1])*   IndexFrac[2]

		+fSimpleMap[Index[0]+1][Index[1]+1][Index[2]+1].Field
		*    IndexFrac[0]*   IndexFrac[1]*   IndexFrac[2]
		;


//	cout<<"Exit in MagneticField::GetFieldMapSimple()"<<endl;
	return Field;
}

/*******************************************************************************/

// This routine reads in the magnetic field maps and interpolates them for a given current
// (interpolation not implemented yet due to missing maps, just linear scaling is applied, shifting and rotation not used)
// and generates a new file which is read by magfld.f. Also position and rotation of the magnet is saved in this file.
// WATCH OUT: coordinate system of magfield map is NOT the same as for geometry
void MagneticField::InitFieldMapSimple() 
{
   //cout <<" Enter in MagneticField::InitFieldMapSimple() "<<endl;

	TString FileInS[] = { m_dir+"/fieldmap/aladin25.map" };
	Double_t FileInCurrent[] = { 2500.0 };
	//norm from SPALADiN: -0.760 (-.53 default, -0.760 for Si, -0.960 for Xe)

	ifstream FileIn;

	TString Read;

	Int_t ix, iy, iz;
	Int_t NX[3], NY[3], NZ[3];
	Double_t MinX[3], MinY[3], MinZ[3];
	Double_t MaxX[3], MaxY[3], MaxZ[3];
	Double_t ShiftX[3], ShiftY[3], ShiftZ[3];

	Double_t GetX = -1., GetY = -1., GetZ = -1.;
	Double_t GetBX = -1., GetBY = -1., GetBZ = -1.;

	Double_t Normalize;

	if (FileInCurrent[0] == 0.0) {
		cout << "ERROR fFileInCurrent[0]=" << FileInCurrent[0] << endl;
	} else {
		Normalize = fCurrent / FileInCurrent[0];
	}
	cout << "Load simple magnetic field and scale to: " << fCurrent << "A"
			<< endl;
	cout << "READ DATA from: " << FileInS[0] << " ...";
	cout.flush();
	FileIn.open(FileInS[0]);

	if (!FileIn.good()) {
		cerr << "ERROR: MagneticField::InitFieldMapSimple(): File Error!"
				<< endl;
		FileIn.close();
		exit(-1);
	}

	//read & write header
	//fFileOut<<"# Interpolated field map for ALADiN magnet"<<endl;
	//fFileOut<<"# for a current of "<<current<<" A"<<endl;
	//fFileOut<<" "<<shiftX<<" "<<shiftY<<" "<<shiftZ<<endl;
	//fFileOut<<" "<<angleY<<endl;

	Read.ReadLine(FileIn);
	if (strstr(Read, "5") == NULL) {
		cerr
				<< "ERROR: MagneticField::InitFieldMapSimple(): False File content!"
				<< endl;
		exit(-1);
	}
	//fFileOut << fRead << endl;
	Read.ReadLine(FileIn);
	if (strstr(Read, "nnn") == NULL) {
		cerr
				<< "ERROR: MagneticField::InitFieldMapSimple(): False File content!"
				<< endl;
		exit(-1);
	}
	//fFileOut << fRead << endl;

	//watch out: Z<->Y switched and X -> -X
	Read.ReadLine(FileIn);
	sscanf(Read, "\n %d %lf %lf \n", &NX[0], &MaxX[0], &MinX[0]);//is switched!!!
	if (NX[0] < 1) {
		fprintf(stderr, "\n!!!\n!!! ERROR: fNX = %d < 1 \n!!!", NX[0]);
	}

	//invert x
	MaxX[0] *= -1.;
	MinX[0] *= -1.;

	if (MinX[0] >= MaxX[0]) {
		fprintf(stderr,
				"\n!!!\n!!! ERROR: Field Min = %0.1f > Max = %0.1f \n!!!",
				MinX[0], MaxX[0]);
	}

	Read.ReadLine(FileIn);
	sscanf(Read, "\n %d %lf %lf \n", &NZ[0], &MinZ[0], &MaxZ[0]);
	if (NZ[0] < 1) {
		fprintf(stderr, "\n!!!\n!!! ERROR: fNZ = %d < 1 \n!!!", NZ[0]);
	}
	if (MinZ[0] >= MaxZ[0]) {
		fprintf(stderr,
				"\n!!!\n!!! ERROR: Field Min = %0.1f > Max = %0.1f \n!!!",
				MinZ[0], MaxZ[0]);
	}

	Read.ReadLine(FileIn);
	sscanf(Read, "\n %d %lf %lf \n", &NY[0], &MinY[0], &MaxY[0]);
	if (NY[0] < 1) {
		fprintf(stderr, "\n!!!\n!!! ERROR: fNY = %d < 1 \n!!!", NY[0]);
	}
	if (MinY[0] >= MaxY[0]) {
		fprintf(stderr,
				"\n!!!\n!!! ERROR: Field Min = %0.1f > Max = %0.1f \n!!!",
				MinY[0], MaxY[0]);
	}

	//scale mm->cm
	MaxX[0] /= 10.;
	MinX[0] /= 10.;
	MaxY[0] /= 10.;
	MinY[0] /= 10.;
	MaxZ[0] /= 10.;
	MinZ[0] /= 10.;

	Read.ReadLine(FileIn);

	ShiftX[0] = MaxX[0] + MinX[0];
	ShiftY[0] = MaxY[0] + MinY[0];
	ShiftZ[0] = MaxZ[0] + MinZ[0];
	
	//fSimpleMapRange.SetXYZ(MaxX[0] - MinX[0],MaxY[0] - MinY[0],MaxZ[0] - MinZ[0]);
	// VM 29/5/2013 - region limits enlarged like in simulation
	fSimpleMapRange.SetXYZ((MaxX[0] - MinX[0])*1.17,(MaxY[0] - MinY[0])*1.39,MaxZ[0] - MinZ[0]);

	fSimpleMapMin=-0.5*fSimpleMapRange;
	fSimpleMapMax= 0.5*fSimpleMapRange;

	//read & write body
	for (iy = 0; iy < NY[0]; iy++) {
		for (iz = 0; iz < NZ[0]; iz++) {
			for (ix = NX[0]; ix > 0; ix--) {
				Read.ReadLine(FileIn);
				sscanf(Read, "%lf %lf %lf %lf %lf %lf \n", &GetX, &GetZ, &GetY,
						&GetBX, &GetBZ, &GetBY);
				//scale mm->cm and switch: Z<->Y and X -> -X
				GetX *= -1;
				GetBX *= -1;
				GetX /= 10.;
				GetY /= 10.;
				GetZ /= 10.;

				//origin centred
				//scale y since the field does not cover our aperture (y corresponds to z , scale 1.39)
				//scale also x (x corresponds to x , scale 1.17)
				
				// Wrong scaling (adjusted) MV
				//fSimpleMap[ix][iy][iz].Position.SetXYZ(
				//		1.17 * (GetX - ShiftX[0] / 2.0),
				//		1. * (GetY - ShiftY[0] / 2.0),
				//		1.39 * (GetZ - ShiftZ[0] / 2.0));	
				// changed (MV). 
			
				fSimpleMap[ix][iy][iz].Position.SetXYZ(
						1.17 * (GetX - ShiftX[0] / 2.0),
						1.39 * (GetY - ShiftY[0] / 2.0),
						1. * (GetZ - ShiftZ[0] / 2.0));

				fSimpleMap[ix][iy][iz].Field.SetXYZ(Normalize * GetBX,
						Normalize * GetBY, Normalize * GetBZ);
			}
		}
	}

	FileIn.close();
	cout << "DONE" << endl;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

