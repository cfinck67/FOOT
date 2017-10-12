#include "Materials.hxx"

Materials::Materials(){

	m_tmpAppendCompoundName = "";
	m_storeMat.clear();
	m_tmpCompoundData.clear();
	m_storeComp.clear();
	m_compoundDensity = 0;

	//********temporary definition of some materials not defined in .inp**********
	//****************************************************************************
	TGeoMaterial *maAr = new TGeoMaterial( "ARGON", 39.948, 18., 0.001662 );
	TGeoMaterial *maC = new TGeoMaterial(  "CARBON", 12.0107, 6., 2.6 );
	TGeoMaterial *maO = new TGeoMaterial( "OXYGEN", 16., 8., 0.0013315 );
	TGeoMaterial *maAl = new TGeoMaterial( "ALUMINUM", 26.981539, 13., 2.6989 );
	TGeoMaterial *maH = new TGeoMaterial( "HYDROGEN", 1.008, 1., 0.000089 );
	TGeoMaterial *maN = new TGeoMaterial( "NITROGEN", 14.007, 7., 0.001251 );
	TGeoMaterial *maFe = new TGeoMaterial( "IRON", 55.845, 26., 7.874 );
	TGeoMaterial *maSi = new TGeoMaterial( "SILICON", 28.085, 14., 2.329 );

	m_storeMat["ARGON"] = maAr;
	m_storeMat["CARBON"] = maC;
	m_storeMat["OXYGEN"] = maO;
	m_storeMat["ALUMINUM"] = maAl;
	m_storeMat["HYDROGEN"] = maH;
	m_storeMat["NITROGEN"] = maN;
	m_storeMat["IRON"] = maFe;
	m_storeMat["SILICON"] = maSi;

	//****************************************************************************
	//****************************************************************************
	ReadFile();
}


//put Materials found in .inp in a map <string, TGeoMaterial*>; every material
//could have different information (A,Z,rho)

void Materials::WriteMaterial( vector<string> tmpVecStr ){

	string nome = tmpVecStr.back();
	TGeoMaterial *mat=new TGeoMaterial( nome.c_str() );
	switch ( tmpVecStr.size() ) {
	  case 2:
	    mat->SetDensity( atof( tmpVecStr[0].c_str() ) );
	    //cout<<"case 2 done!"<<endl;
	    break;
	  case 3:
	    mat->SetZ( atof( tmpVecStr[0].c_str() ) );
	    mat->SetDensity( atof(tmpVecStr[1].c_str() ) );
	    //cout<<"case 3 done!"<<endl;
	    break;
	  case 4:
	    mat->SetZ( atof( tmpVecStr[0].c_str() ) );
	    mat->SetA( atof( tmpVecStr[1].c_str() ) );
	    mat->SetDensity( atof( tmpVecStr[2].c_str() ) );
	    //cout<<"case 4 done!"<<endl;
	    break;
	  default:
	    cout<<"error in vec size"<<endl;
	    exit(0);
	}
	m_storeMat[nome] = mat;
	//cout<<"IN FUNC SIZE OF MYMAP IS "<<tmpmap->size()<<endl;
}




//evaluate if we can write compound or if we have to wait next line
bool Materials::ChooseIfWriteCompound( string provamarcopolo ){
	if ( provamarcopolo.find("COMPOUND") != string::npos ) 		return false;
	else 		return true;
}


//Compound definition continues in next line so we decide to append information
//of current line in a global vector to be written when compound definition is over
void Materials::AppendCompound( vector<string> tmpVecStr ){

	m_tmpAppendCompoundName = tmpVecStr.back();
	tmpVecStr.pop_back();
	// for (unsigned int i = 0; i<tmpVecStr.size(); ++i){
	// 	cout <<  tmpVecStr[i] << endl;
	// }

	for ( unsigned int i=0; i<tmpVecStr.size(); ++i ){
		RemoveSpace( &tmpVecStr[i] );
		m_tmpCompoundData.push_back( tmpVecStr[i] );
	}
}

//Compound definition does NOT continue in next line so we write compound remembering
//also previous information of compound. We have to decide how to write compound too:
//by weight, by atoms, by volume
void Materials::ChooseHowToWriteCompound( vector<string> tmpVecStr ){
	//only to control if they are the same compound
	if( m_tmpCompoundData.empty() == false && tmpVecStr.back() == m_tmpAppendCompoundName )
		{ cout << "procedo con la scrittura" << endl; }
	else if ( m_tmpCompoundData.empty() == true )
		{ cout << "dovremmo essere in un compound singolo" << endl; }
	else if ( m_tmpCompoundData.empty() == false && tmpVecStr.back() != m_tmpAppendCompoundName )
		{ cout << "spero di non vederla mai" << endl; exit(0); }

	AppendCompound( tmpVecStr );

	//***************WRITE HERE************************
	cout << "VERIFICA ESITO " << endl;
	cout << m_tmpAppendCompoundName << " ===>";
	for ( unsigned int j=0; j<m_tmpCompoundData.size(); ++j )
	{
		cout << m_tmpCompoundData[j] << " ";
	}
	cout << endl;
	if ( (m_tmpCompoundData[0]).find("-")!=string::npos && (m_tmpCompoundData[1]).find("-")!=string::npos )
		{ WriteByVolume(); }
	else if ( (m_tmpCompoundData[0]).find("-")!=string::npos && (m_tmpCompoundData[1]).find("-")==string::npos )
		{ WriteByWeight(); }
	else if ( (m_tmpCompoundData[0]).find("-")==string::npos && (m_tmpCompoundData[1]).find("-")==string::npos )
		{ WriteByAtoms(); }

	//*************************************************
	m_tmpAppendCompoundName = "";
	m_compoundDensity = 0;
	m_tmpCompoundData.clear();

}

void Materials::WriteByVolume(){
	cout << "write by volume" << endl;

	return;

}

void Materials::WriteByWeight(){
	cout << "write by weight" << endl;

	if ( m_tmpCompoundData.size()%2 != 0 )

			cout << "ERROR::Materials::WriteByWeight  -->  compound vector does NOT have a even size!" << endl, exit(0);

	double weightSum = 0;
	//evaluate weight sum to normalize single weights, loop every two elements of vector
	for( unsigned int i = 0; i<m_tmpCompoundData.size(); i++ ){
		cout << m_tmpCompoundData[i] << endl;
	}

	string whichCompMap = "";
	for( unsigned int i = 0; i<m_tmpCompoundData.size(); i+=2 ){
		// check the material is already stored in one of the maps: material map or
		//compound map if we have to define a compound which contains a compound
		if ( m_storeMat.find( m_tmpCompoundData[i+1] ) != m_storeMat.end() )
			whichCompMap = "mat";
		else if ( m_storeComp.find( m_tmpCompoundData[i+1] ) != m_storeComp.end() )
			whichCompMap = "comp";
		else
			cout << "Mat or Comp not found" << endl, exit(0);

		weightSum += atof( m_tmpCompoundData[i].c_str() );
		cout << m_tmpCompoundData[i] << "   " << weightSum << endl;
	}
	TGeoMixture *comp = new TGeoMixture( m_tmpAppendCompoundName.c_str(), m_tmpCompoundData.size()/2, m_compoundDensity );

	for( unsigned int i = 1; i<m_tmpCompoundData.size(); i+=2 ){
		cout << m_tmpCompoundData[i] << "   " << atof( m_tmpCompoundData[i-1].c_str() ) << "  " << atof( m_tmpCompoundData[i-1].c_str() )/weightSum << endl;
		if ( whichCompMap == "mat" )
			comp->AddElement( m_storeMat[m_tmpCompoundData[i]], atof(m_tmpCompoundData[i-1].c_str())/weightSum );
		else if ( whichCompMap == "comp" )
			comp->AddElement( m_storeComp[m_tmpCompoundData[i]], atof(m_tmpCompoundData[i-1].c_str())/weightSum );
		cout << "Added" << endl;
	}

	m_storeComp[m_tmpAppendCompoundName] = comp;
}




void Materials::WriteByAtoms(){
	cout << "write by atoms" << endl;
	if ( m_tmpCompoundData.size()%2 != 0 )
			cout << "ERROR::Materials::WriteByAtoms  -->  compound vector does NOT have a even size!" << endl, exit(0);
	//https://root.cern.ch/root/roottalk/roottalk04/2863.html
	TGeoMixture *comp = new TGeoMixture( m_tmpAppendCompoundName.c_str(), m_tmpCompoundData.size()/2, m_compoundDensity );
	int count = 0;
	for( unsigned int i = 1; i<m_tmpCompoundData.size(); i+=2 ){
		cout << m_tmpCompoundData[i] << "   " << atof( m_tmpCompoundData[i-1].c_str() ) << endl;
		if ( m_storeMat.find( m_tmpCompoundData[i] ) != m_storeMat.end() ){
			comp->DefineElement( count, m_storeMat[ m_tmpCompoundData[i] ]->GetZ(), atoi( m_tmpCompoundData[i-1].c_str() ) );
			count++;
		}
		else

			cout << "Mat not found" << endl, exit(0);

	}

	m_storeComp[m_tmpAppendCompoundName] = comp;

}



// usa quello globale???
string Materials::StrReplace( string original,string erase,string add ){

  int found=original.find( erase );
  if ((size_t) found != string::npos){
		int cutlen = erase.size();
    original.replace( found, cutlen, add );
    return original;
    }

  else {
    cout<<"not found "<<erase<<" in "<<original<<endl;
    return original;
  }
}




vector<string> Materials::StrSplit( const string& str, int splitLength = 10 )
{
	int NumSubstrings = str.length() / splitLength;
	//cout << str.length() << "    " << NumSubstrings <<endl;
  vector<string> ret;
 	//**********************************ROUGH ADD OF A ITER***********************
	//**********cause lines have different lenghts because of \n******************
  for ( int i = 0; i < NumSubstrings +1; i++ ){
  	ret.push_back(str.substr( i * splitLength, splitLength ) );
	}

  return ret;
}



void Materials::RemoveEmpty( vector<string>* tmpVecStr ){
	for ( unsigned int i = 0; i < tmpVecStr->size(); ++i ){
		if ( tmpVecStr->at(i).find_first_not_of (' ') == string::npos ) {
			tmpVecStr->erase(tmpVecStr->begin()+i);
			i--;
		}
	}
}

void Materials::RemoveSpace( string* s ) {
	s->erase( ::remove_if(s->begin(), s->end(), ::isspace), s->end() );
}


void Materials::ReadFile(){

	int count=0;

	ifstream proofinput;
	proofinput.open( ( ( (string) (getenv ("FOOTMAIN") ) + "/Simulation/foot.inp" ) ).c_str());
	if ( !proofinput.is_open() ){cout << "ERROR" << endl;}


	string line="";
	while(getline(proofinput,line)){


		if(line == "") continue;
		if(line.find("*")!=string::npos||line.find("#")!=string::npos||line.find("!")!=string::npos)
		continue;

	  	if(line.find("MATERIAL")!=string::npos){

	    	vector<string> datawholeline;
	    	string materialString=StrReplace( line,"MATERIAL","" );
	    	istringstream materialStream( materialString );

			streampos oldPos = proofinput.tellg();
			getline( proofinput,line );

			bool flagWriteMat = ChooseIfWriteCompound( line );

			string tmpString="";
	    	while ( materialStream>>tmpString )
				datawholeline.push_back( tmpString );

			if ( flagWriteMat ) {
				proofinput.seekg ( oldPos );   // get back to the position
		  		WriteMaterial( datawholeline );

			}
	    	else {
	    		if (datawholeline.size() != 2 )
	    			cout << "ERROR::Materials::ReadFile  -->  Compound density not found. Line standard not respected!" << endl, exit(0);
	    		m_compoundDensity = atof( datawholeline[0].c_str() );
	    		//cout << "datawholeline   " << datawholeline[0].c_str() << endl;
	    	}
	    }

		if(line.find("COMPOUND")!=string::npos){

			count++;
			vector<string> dataCompound;
			string compoundString=StrReplace( line,"COMPOUND  ","" );
			dataCompound = StrSplit( compoundString );
			RemoveEmpty( &dataCompound );
			string marcopolo="";

			streampos oldPos = proofinput.tellg();
			getline( proofinput,marcopolo );
			bool flagWriteCompound = ChooseIfWriteCompound( marcopolo );
			// cout << "**********************************" << endl;
			// for ( unsigned int j=0; j < dataCompound.size(); ++j ){ cout << dataCompound[j] << endl; }
			// cout << "**********************************" << endl;
			proofinput.seekg ( oldPos );   // get back to the position
			if ( !flagWriteCompound ) 	AppendCompound( dataCompound );
			if ( flagWriteCompound  ) 	ChooseHowToWriteCompound( dataCompound );

		}
	}

	proofinput.close();
	cout <<"Found "<< count << " compounds" << endl;
}





void Materials::PrintMatMap(){

	for ( map<string, TGeoMaterial*>::iterator it=m_storeMat.begin(); it!=m_storeMat.end(); ++it )
	cout << it->first << " => " << it->second->GetName() << " => "<< it->second->GetDensity() << '\n';
}

void Materials::PrintCompMap(){

	for ( map<string, TGeoMixture*>::iterator it=m_storeComp.begin(); it!=m_storeComp.end(); ++it )
	cout << it->first << " => " << it->second->GetName() << " => "<< it->second->GetDensity() << "\t" << it->second->GetNelements() << '\n';
}
