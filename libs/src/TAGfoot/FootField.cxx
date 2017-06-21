


#include "FootField.hxx"

#include "GlobalPar.hxx"


//	Real field constructor read info from file
FootField::FootField ( string fileName ) {

	// parameter to be set outside, switch between different field type
	m_fieldSetting = "realFieldMap";

	ifstream ifile;
    ifile.open( ("../../Simulation/"+fileName).c_str() );

    cout << "\tB center =  " << MAG_AIR_X <<"  "<< MAG_AIR_Y << "  "<< MAG_AIR_Z << endl;

    if ( !ifile.is_open() )        
    	cout<< "wrong input in AbsBField constructor for file " << fileName << endl, exit(0);

    // read position and field  -->	 fill a multidimensional map called lattice3D = map< double, map< double, map< double, TVector3 > > >
    string line = "";
    while( getline( ifile, line ) ) {  

        if (line == "")  continue;
        if ( line.find("#") != string::npos || line.find("*") != string::npos )
            continue;
        // if ( line.find("#") != string::npos || line.find("//") != string::npos )     continue;

        double x = -1;
        double y = -1;
        double z = -1;
        double bx = -1;
        double by = -1;
        double bz = -1;

        istringstream getall(line);
        getall  >> x;
        getall  >> y;
        getall  >> z;
        getall  >> bx;
        getall  >> by;
        getall  >> bz;

        // cout << "x " << x << " y " << y << " z " << z << " px " <<  bx << " by " << by << " bz " << bz << endl;
        // multidimensional map called lattice3D = map< double, map< double, map< double, TVector3 > > >
        m_filedMap[x][y][z] = TVector3(bx, by, bz);

    }  
    ifile.close();

}


// constant simple field constructor, Field along y axis ONLY in the magnet reagion taken from the foot_geo.h file
FootField::FootField ( float constValue ) {
	
	m_fieldSetting = "constField";
	
	if ( GlobalPar::GetPar()->Debug() > 0 )    {
		cout << "Creating Constant B field" << endl;
		// cout << "\tB min vertex =  " << MAG_AIR_X-MAG_AIR_WIDTH <<"  "<< MAG_AIR_Y-MAG_AIR_HEIGHT << "  "<< MAG_AIR_Z-MAG_AIR_LENGTH << endl;
		// cout << "\tB min vertex =  " << MAG_AIR_X+MAG_AIR_WIDTH <<"  "<< MAG_AIR_Y+MAG_AIR_HEIGHT << "  "<< MAG_AIR_Z+MAG_AIR_LENGTH << endl;
		// m_filedMap[ MAG_AIR_X-MAG_AIR_WIDTH ][ MAG_AIR_Y-MAG_AIR_HEIGHT ][ MAG_AIR_Z-MAG_AIR_LENGTH ] = TVector3(0, constValue, 0);
		// m_filedMap[ MAG_AIR_X+MAG_AIR_WIDTH ][ MAG_AIR_Y+MAG_AIR_HEIGHT ][ MAG_AIR_Z+MAG_AIR_LENGTH ] = TVector3(0, constValue, 0);

		cout << "\tB center =  " << MAG_AIR_X <<"  "<< MAG_AIR_Y << "  "<< MAG_AIR_Z << endl;
		cout << "\tB MIN vertex local =  " << -MAG_AIR_WIDTH/2 <<"  "<< -MAG_AIR_HEIGHT/2 << "  "<< -MAG_AIR_LENGTH/2 << endl;
		cout << "\tB MAX vertex local =  " << +MAG_AIR_WIDTH/2 <<"  "<< +MAG_AIR_HEIGHT/2 << "  "<< MAG_AIR_LENGTH/2 << endl;
		cout << "\n\tB MIN vertex global =  " << MAG_AIR_X-MAG_AIR_WIDTH/2 <<"  "<< MAG_AIR_Y-MAG_AIR_HEIGHT/2 << "  "<< MAG_AIR_Z-MAG_AIR_LENGTH/2 << endl;
		cout << "\tB MAX vertex = global " << MAG_AIR_X+MAG_AIR_WIDTH/2 <<"  "<< MAG_AIR_Y+MAG_AIR_HEIGHT/2 << "  "<< MAG_AIR_Z+MAG_AIR_LENGTH/2 << endl;
	}
	
	// local coordiantes 
	m_filedMap[  - MAG_AIR_WIDTH/2 ][  - MAG_AIR_HEIGHT/2 ][ -MAG_AIR_LENGTH/2 ] = TVector3(0, constValue, 0);
	m_filedMap[  + MAG_AIR_WIDTH/2 ][  + MAG_AIR_HEIGHT/2 ][ MAG_AIR_LENGTH/2 ] = TVector3(0, constValue, 0);

} 




// return B as a vector, given vector position
TVector3 FootField::get(const TVector3& position) const {
	const TVector3 localPosition = TVector3( position.x() - MAG_AIR_X, position.y() - MAG_AIR_Y, position.z() - MAG_AIR_Z );	//to local coord
	return const_cast<FootField*>(this)->Interpolate( localPosition );
}




// first 3 variables are the input position components, last 3 var are the output b components
void FootField::get(const double& posX, const double& posY, const double& posZ, double& Bx, double& By, double& Bz) const { 
		
	const TVector3 position = TVector3(posX - MAG_AIR_X, posY - MAG_AIR_Y, posZ - MAG_AIR_Z );	//to local coord

	TVector3 outField = const_cast<FootField*>(this)->Interpolate( position );

	Bx = outField.X(); 
	By = outField.Y(); 
	Bz = outField.Z(); 
}




// same for real and const field
TVector3 FootField::Interpolate( const TVector3 &position ) {
	//    il procedimento seguente prende spunto da
	//    https://en.wikipedia.org/wiki/Trilinear_interpolation
	//	  Output in  10^4 Gauss (o 10^-1 T ) perche' vengono usate queste unita' di misura da cerebrolesi

	TVector3 outField = TVector3(0,0,0);

	// if position out of the field boudaries, return a zero field
	// cout << "magnetic debug: "<< endl;
	// cout << "\tAlong X: " << position.x() << "  "<< m_filedMap.begin()->first <<"  "<< m_filedMap.rbegin()->first << endl;
	if ( position.x() < m_filedMap.begin()->first || position.x() > m_filedMap.rbegin()->first )	return outField;
	// cout << "\tAlong Y: " << position.y() << "  "<< m_filedMap.begin()->second.begin()->first <<"  "<< m_filedMap.rbegin()->second.rbegin()->first << endl;
	if ( position.y() < m_filedMap.begin()->second.begin()->first || position.y() > m_filedMap.rbegin()->second.rbegin()->first )	return outField;
	// cout << "\tAlong Z: " << position.z() << "  "<< m_filedMap.begin()->second.begin()->second.begin()->first <<"  "<< m_filedMap.rbegin()->second.rbegin()->second.rbegin()->first << endl;
	if ( position.z() < m_filedMap.begin()->second.begin()->second.begin()->first || position.z() > m_filedMap.rbegin()->second.rbegin()->second.rbegin()->first )	return outField;

	if (m_fieldSetting == "constField") {
		// cout << "Const magnetic val: ";
		// m_filedMap.begin()->second.begin()->second.begin()->second.Print();
		return m_filedMap.begin()->second.begin()->second.begin()->second;
	}

	// find position boundaries
	double xMin=0, xMax=0, yMin=0, yMax=0, zMin=0, zMax=0;
	for ( lattice3D::iterator xIt=m_filedMap.begin(); xIt!=m_filedMap.end();) {
		xMin = (*xIt).first;
		if ( position.x() < (*(++xIt)).first ) {
		// if ( position.x() > (*xIt).first ) {
			
			xMax = (*xIt).first;
			break;
		}
	}
	for ( lattice2D::iterator yIt=m_filedMap.begin()->second.begin(); yIt!=m_filedMap.begin()->second.end();) {
		yMin = (*yIt).first;
		if ( position.y() < (*(++yIt)).first ) {
		// if ( position.y() > (*yIt).first ) {
			
			yMax = (*yIt).first;
			break;
		}
	}
	for ( map< double, TVector3 >::iterator zIt=m_filedMap.begin()->second.begin()->second.begin(); zIt!=m_filedMap.begin()->second.begin()->second.end(); ) {
		zMin = (*zIt).first;
		if ( position.z() < (*(++zIt)).first ) {
		// if ( position.z() > (*zIt).first ) {
			// zMin = (*zIt).first;
			zMax = (*(zIt)).first;
			break;
		}
	}
	double xDiff = (position.x() - xMin)/(xMax-xMin);
	double yDiff = (position.y() - yMin)/(yMax-yMin);
	double zDiff = (position.z() - zMin)/(zMax-zMin);

	// cout << "X:\tmin " << xMin << " max " << xMax << endl;
	// cout << "Y:\tmin " << yMin << " max " << yMax << endl;
	// cout << "Z:\tmin " << zMin << " max " << zMax << endl;

	// cout << "\n Diff:\tx " << xDiff << " y " << yDiff << " z " << zDiff << endl;

	// cout << m_filedMap[xMin][yMin][zMin].x() << endl;
	// cout << m_filedMap[xMin][yMin][zMax].x() << endl;
	// cout << m_filedMap[xMin][yMax][zMin].x() << endl;
	// cout << m_filedMap[xMin][yMax][zMax].x() << endl;

	// cout << endl<< m_filedMap[xMax][yMin][zMin].x() << endl;
	// cout << m_filedMap[xMax][yMin][zMax].x() << endl;
	// cout << m_filedMap[xMax][yMax][zMin].x() << endl;
	// cout << m_filedMap[xMax][yMax][zMax].x() << endl;

	// cout << endl<<endl<< "y" << endl << m_filedMap[xMin][yMin][zMin].y() << endl;
	// cout << m_filedMap[xMin][yMin][zMax].y() << endl;
	// cout << m_filedMap[xMin][yMax][zMin].y() << endl;
	// cout << m_filedMap[xMin][yMax][zMax].y() << endl;

	// cout << endl<< m_filedMap[xMax][yMin][zMin].y() << endl;
	// cout << m_filedMap[xMax][yMin][zMax].y() << endl;
	// cout << m_filedMap[xMax][yMax][zMin].y() << endl;
	// cout << m_filedMap[xMax][yMax][zMax].y() << endl;

	// cout << endl<<endl<< "z"<< endl << m_filedMap[xMin][yMin][zMin].z() << endl;
	// cout << m_filedMap[xMin][yMin][zMax].z() << endl;
	// cout << m_filedMap[xMin][yMax][zMin].z() << endl;
	// cout << m_filedMap[xMin][yMax][zMax].z() << endl;

	// cout << endl<< m_filedMap[xMax][yMin][zMin].z() << endl;
	// cout << m_filedMap[xMax][yMin][zMax].z() << endl;
	// cout << m_filedMap[xMax][yMax][zMin].z() << endl;
	// cout << m_filedMap[xMax][yMax][zMax].z() << endl;

	// 10^4 Gauss = 1 T

	// interpolate
	// X
	double B_00x = m_filedMap[xMin][yMin][zMin].x() * (1-xDiff) + m_filedMap[xMax][yMin][zMin].x() * xDiff;
	double B_01x = m_filedMap[xMin][yMin][zMax].x() * (1-xDiff) + m_filedMap[xMax][yMin][zMax].x() * xDiff;
	double B_10x = m_filedMap[xMin][yMax][zMin].x() * (1-xDiff) + m_filedMap[xMax][yMax][zMin].x() * xDiff;
	double B_11x = m_filedMap[xMin][yMax][zMax].x() * (1-xDiff) + m_filedMap[xMax][yMax][zMax].x() * xDiff;

	double B_0x = B_00x * (1 - yDiff) + B_10x * yDiff;
	double B_1x = B_01x * (1 - yDiff) + B_11x * yDiff;

	outField.SetX( (B_0x * (1 - zDiff) + B_1x * zDiff)*1e-3 );		// 10^3 Gauss, 10^-1 T

	// Y
	double B_00y = m_filedMap[xMin][yMin][zMin].y() * (1-xDiff) + m_filedMap[xMax][yMin][zMin].y() * xDiff;
	double B_01y = m_filedMap[xMin][yMin][zMax].y() * (1-xDiff) + m_filedMap[xMax][yMin][zMax].y() * xDiff;
	double B_10y = m_filedMap[xMin][yMax][zMin].y() * (1-xDiff) + m_filedMap[xMax][yMax][zMin].y() * xDiff;
	double B_11y = m_filedMap[xMin][yMax][zMax].y() * (1-xDiff) + m_filedMap[xMax][yMax][zMax].y() * xDiff;

	double B_0y = B_00y * (1 - yDiff) + B_10y * yDiff;
	double B_1y = B_01y * (1 - yDiff) + B_11y * yDiff;

	outField.SetY( (B_0y * (1 - zDiff) + B_1y * zDiff)*1e-3 );		// 10^3 Gauss, 10^-1 T

	// X
	double B_00z = m_filedMap[xMin][yMin][zMin].z() * (1-xDiff) + m_filedMap[xMax][yMin][zMin].z() * xDiff;
	double B_01z = m_filedMap[xMin][yMin][zMax].z() * (1-xDiff) + m_filedMap[xMax][yMin][zMax].z() * xDiff;
	double B_10z = m_filedMap[xMin][yMax][zMin].z() * (1-xDiff) + m_filedMap[xMax][yMax][zMin].z() * xDiff;
	double B_11z = m_filedMap[xMin][yMax][zMax].z() * (1-xDiff) + m_filedMap[xMax][yMax][zMax].z() * xDiff;

	double B_0z = B_00z * (1 - yDiff) + B_10z * yDiff;
	double B_1z = B_01z * (1 - yDiff) + B_11z * yDiff;

	outField.SetZ( (B_0z * (1 - zDiff) + B_1z * zDiff)*1e-3 );		// 10^3 Gauss, 10^-1 T

	// outField.Print();
	// cout << "end" << endl;
	// cin.get();

	return outField;
	// return outField.Unit();		// per il double dipole?

}
















