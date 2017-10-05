#include "Materials.hxx"

Materials::Materials(){

	FillStore();
}

void Materials::FillStore(){

	ReadFile();

}

void Materials::WriteMaterial(vector<string> tmpVecStr){

	string nome = tmpVecStr.back();
	TGeoMaterial *mat=new TGeoMaterial(nome.c_str());
	switch (tmpVecStr.size()) {
	  case 2:
	    mat->SetDensity(atof(tmpVecStr[0].c_str()));
	    //cout<<"case 2 done!"<<endl;
	    break;
	  case 3:
	    mat->SetZ(atof(tmpVecStr[0].c_str()));
	    mat->SetDensity(atof(tmpVecStr[1].c_str()));
	    //cout<<"case 3 done!"<<endl;
	    break;
	  case 4:
	    mat->SetZ(atof(tmpVecStr[0].c_str()));
	    mat->SetA(atof(tmpVecStr[1].c_str()));
	    mat->SetDensity(atof(tmpVecStr[2].c_str()));
	    //cout<<"case 4 done!"<<endl;
	    break;
	  default:
	    cout<<"error in vec size"<<endl;
	    exit(0);
	}
	//m_storeMat[nome]=mat;
	m_storeMat.insert( pair<string, TGeoMaterial*> (nome,mat) );
	//cout<<"IN FUNC SIZE OF MYMAP IS "<<tmpmap->size()<<endl;
	return;

}

//evaluate if we can write compound or if we have to wait next line
bool Materials::ChooseIfWriteCompound(string provamarcopolo){

	if (provamarcopolo.find("COMPOUND") != string::npos) {
		//cout<<"c'è un compound qui"<<endl;
		m_flagWriteCompound = false;
	}

	else {
		m_flagWriteCompound = true;
		//cout << "non c'è nulla qua sotto" << '\n';
	}
	return m_flagWriteCompound;
}

void Materials::AppendCompound(vector<string> tmpVecStr){

	m_tmpAppendCompoundName = tmpVecStr.back();
	tmpVecStr.pop_back();
	for (unsigned int i=0; i<tmpVecStr.size(); ++i){
		m_tmpCompoundData.push_back(tmpVecStr[i]);
	}
	return;
}

void Materials::ChooseHowToWriteCompound(vector<string> tmpVecStr){

	//only to control if they are the same compound
	if( m_tmpCompoundData.empty() == false && tmpVecStr.back() == m_tmpAppendCompoundName )
		{ cout << "tutto a posto a ferragosto" << endl; }
	else if ( m_tmpCompoundData.empty() == true )
		{ cout << "dovremmo essere in un compound singolo" << endl; }
	else if ( m_tmpCompoundData.empty() == false && tmpVecStr.back() != m_tmpAppendCompoundName )
		{ cout << "spero di non vederla mai" << endl; return; }
	else { cout << "qualcosa non va come vorresti, Riccardo" << endl; return; }

	m_tmpAppendCompoundName = tmpVecStr.back();
	tmpVecStr.pop_back();
	for (unsigned int i=0; i<tmpVecStr.size(); ++i){
		m_tmpCompoundData.push_back(tmpVecStr[i]);
	}
	//***************WRITE HERE************************
	cout << "VERIFICA ESITO " << endl;
	cout << m_tmpAppendCompoundName << " ===> ";
	for (unsigned int j=0; j<m_tmpCompoundData.size(); ++j)
	{
		cout << "  " << m_tmpCompoundData[j];
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
	m_tmpCompoundData.clear();

}

void Materials::WriteByVolume(){
	cout << "write by volume" << endl;

	return;

}

void Materials::WriteByWeight(){
	cout << "write by weight" << endl;
	double weightSum = 0;
	//evaluate weight sum to normalize single weights, loop every two elements of vector
	for(unsigned int i = 0; i<m_tmpCompoundData.size(); i+=2){
		weightSum += atof(m_tmpCompoundData[i].c_str());
	}
	cout << "riga 125" << endl;
	TGeoMixture *comp = new TGeoMixture(m_tmpAppendCompoundName.c_str(), m_tmpCompoundData.size()/2);

	for(unsigned int i = 1; i<m_tmpCompoundData.size(); i+=2){
		comp->AddElement( m_storeMat[m_tmpCompoundData[i]], atof(m_tmpCompoundData[i-1].c_str())/weightSum );
	}

	m_storeComp.insert( pair<string, TGeoMixture*> (m_tmpAppendCompoundName, comp) );
	return;
}

void Materials::WriteByAtoms(){
	cout << "write by atoms" << endl;
	//https://root.cern.ch/root/roottalk/roottalk04/2863.html

	return;


}
string Materials::StrReplace(string original,string erase,string add){

  int found=original.find(erase);
  if ((size_t) found != string::npos){
		int cutlen=erase.size();
    original.replace(found, cutlen, add);
    return original;
    }

  else {
    cout<<"not found "<<erase<<" in "<<original<<endl;
    return original;
  }
}

vector<string> Materials::StrSplit(const string& str, int splitLength = 10)
{
   int NumSubstrings = str.length() / splitLength;
	 //cout << str.length() << "    " << NumSubstrings <<endl;
   vector<string> ret;
	 //rough add of a iter
   for (int i = 0; i < NumSubstrings +1; i++)
   {
  		ret.push_back(str.substr(i * splitLength, splitLength));
   }

   return ret;
}



void Materials::ReadFile(){

	ifstream proofinput;
	vector<string> datawholeline;
	vector<string> dataCompound;
	int count=0;
	m_flagWriteCompound = false;
	m_tmpAppendCompoundName = "";
	m_storeMat.clear();
	m_tmpCompoundData.clear();
	m_storeComp.clear();
	proofinput.open( ( ( (string) (getenv ("FOOTMAIN") ) + "/Simulation/foot.inp" ) ).c_str());
	if ( !proofinput.is_open() ){cout << "ERROR" << endl;}

	//temporary definition of some materials not defined in .inp
	//**********************************************************
	TGeoMaterial *maAr = new TGeoMaterial("ARGON", 39.948, 18., 0.001662);//densità viene da flair,
	TGeoMaterial *maC = new TGeoMaterial("CARBON", 12.0107, 6., 2.26);
	TGeoMaterial *maO = new TGeoMaterial("OXYGEN", 16., 8., 0.0013315);
	TGeoMaterial *maAl = new TGeoMaterial("ALUMINIUM", 26.981539, 13., 2.6989);
	TGeoMaterial *maH = new TGeoMaterial("HYDROGEN", 1.008, 1., 0.000089);
	TGeoMaterial *maN = new TGeoMaterial("NITROGEN", 14.007, 7., 0.001251);
	TGeoMaterial *maFe = new TGeoMaterial("IRON", 55.845, 26., 7.874);

	m_storeMat.insert ( pair<string,TGeoMaterial*>("ARGON",maAr) );
	m_storeMat.insert ( pair<string,TGeoMaterial*>("CARBON",maC) );
	m_storeMat.insert ( pair<string,TGeoMaterial*>("OXYGEN",maO) );
	m_storeMat.insert ( pair<string,TGeoMaterial*>("ALUMINIUM",maAl) );
	m_storeMat.insert ( pair<string,TGeoMaterial*>("HYDROGEN",maH) );
	m_storeMat.insert ( pair<string,TGeoMaterial*>("NITROGEN",maN) );
	m_storeMat.insert ( pair<string,TGeoMaterial*>("IRON",maFe) );

	//********************************************************************

	string line="";
	while(getline(proofinput,line)){

	  if(line=="") continue;
	  if(line.find("*")!=string::npos||line.find("#")!=string::npos||line.find("!")!=string::npos)
	  continue;

	  	if(line.find("MATERIAL")!=string::npos){

	    	datawholeline.clear();
	    	string materialString=StrReplace(line,"MATERIAL","");
	    	istringstream materialStream(materialString);
	    	string tmpString="";
	    	while (materialStream>>tmpString) {

	      	datawholeline.push_back(tmpString);
	      }

	  		WriteMaterial(datawholeline);
				continue;
	    }

			if(line.find("COMPOUND")!=string::npos){

				count++;
				dataCompound.clear();
				string compoundString=StrReplace(line,"COMPOUND  ","");
				dataCompound = StrSplit(compoundString);
				string marcopolo="";
				streampos oldPos = proofinput.tellg();
				getline(proofinput,marcopolo);
				m_flagWriteCompound = ChooseIfWriteCompound(marcopolo);
				cout << "**********************************" << endl;
				for (unsigned int j=0; j < dataCompound.size(); ++j){cout << dataCompound[j] << endl;}
				cout << "**********************************" << endl;
				proofinput.seekg (oldPos);   // get back to the position
				if ( m_flagWriteCompound == false) { AppendCompound( dataCompound );}
				if ( m_flagWriteCompound == true) { ChooseHowToWriteCompound( dataCompound );}
				continue;
			 }
	}

	proofinput.close();
	cout <<"ho trovato questi compound "<< count << endl;

	return;
}

void Materials::PrintMatMap(){

	for (map<string, TGeoMaterial*>::iterator it=m_storeMat.begin(); it!=m_storeMat.end(); ++it)
	cout << it->first << " => " << it->second->GetName() << " => "<< it->second->GetDensity() << '\n';
}

void Materials::PrintCompMap(){

	for (map<string, TGeoMixture*>::iterator it=m_storeComp.begin(); it!=m_storeComp.end(); ++it)
	cout << it->first << " => " << it->second->GetName() << " => "<< it->second->GetDensity() << it->second->GetNelements() << '\n';
}
