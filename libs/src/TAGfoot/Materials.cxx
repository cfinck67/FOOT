

#include "Materials.hxx"


// using namespace std;

Materials::Materials(){

	FillStore();
}

void Materials::FillStore(){

	ReadFile();

}

void Materials::WriteMaterial(vector<string> tmpVecStr){

	string nome = tmpVecStr.back();
	TGeoMaterial *mat=new TGeoMaterial(nome.c_str());
	//mat.SetName(name); VEDIAMO SE C'E METODO PER NOME
	//cout<<"size in switch is: "<<v.size()<<endl;
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
	m_store[nome]=mat;
	//cout<<"IN FUNC SIZE OF MYMAP IS "<<tmpmap->size()<<endl;
	return;

}

string Materials::StrReplace(string original,string erase,string add){


      int found=original.find(erase);
      if ((size_t) found != string::npos){int cutlen=erase.size();
      original.replace(found, cutlen, add);
      //cout<<"ReadFile line 12 is ok"<<endl;
      return original;
    }

      else {

        cout<<"not found "<<erase<<" in "<<original<<endl;
        return original;
      }

}



void Materials::ReadFile(){
	
	//ifstream proofinput;
	//std::vector<Material> mymatvec;
	//std::map<string, Material> mymap;
	//Material mat;   //attenzione che usando lo stesso potrebbe non sovrascriere info
	ifstream proofinput;
	vector<string> datawholeline;
	map<string, TGeoMaterial*> mymap;
	proofinput.open("FOOT_new.inp");
	if (!proofinput.is_open()){cout<<"ERROR"<<endl;}

	string line="";
	while(getline(proofinput,line)){

	  if(line=="") continue;
	  if(line.find("*")!=string::npos||line.find("#")!=string::npos||line.find("!")!=string::npos)
	  continue;

	  if(line.find("MATERIAL")!=string::npos){

	    datawholeline.clear();
	    string materialstring=StrReplace(line,"MATERIAL","");
	    istringstream materialstream(materialstring);
	    string tmpString="";
	    while (materialstream>>tmpString) {

	      datawholeline.push_back(tmpString);
	      //cout<<"riga 113"<<endl;
	      }

	        /*cout << "myvector contains:";
	         for (unsigned i=0; i<datawholeline.size(); i++)
	           {cout << ' ' << datawholeline.at(i)<<endl;
	            }
	            cout<<" size is "<<datawholeline.size()<<endl;
	            cout<<"last element is "<<datawholeline.back()<<endl;
	*/

	  WriteMaterial(datawholeline);
	  //cout<<"INTERMEDIATE SIZE OF MYMAP IS "<<mymap.size()<<endl;
	    }


	}

	proofinput.close();
	cout<<"FINAL SIZE OF MYMAP IS "<<mymap.size()<<endl;

	return;
}

void Materials::PrintMap(){

	for (map<string, TGeoMaterial*>::iterator it=m_store.begin(); it!=m_store.end(); ++it)
	cout << it->first << " => " << it->second->GetName() << " => "<< it->second->GetDensity() << '\n';
}


