#ifndef FOOTBOX_HXX
#define FOOTBOX_HXX



class FootBox {

public:

	FootBox( TVector3 origin, TVector3 dimension, string materialName, string materialRegion, string bodyName, string regionName ):
			m_origin(origin), m_dimension(dimension), m_materialName(materialName),
			m_materialRegion(materialRegion), m_bodyName(bodyName), m_regionName(regionName)
	{};
	~FootBox() {};

	TVector3 GetPosition() { return m_origin; };
	TVector3 GetDimension() { return m_dimension; };

	string GetMaterialName() { return materialName; };
	string GetMaterialRegion() { return m_materialRegion; };

	string GetBodyName() { return m_bodyName; };
	string GetRegionName() { return m_regionName; };

	


protected:

	TVector3 m_origin;
	TVector3 m_dimension;

	string m_materialName;		// the name found in the .inp file
	string m_materialRegion;	// the name of the variable corresponding to the material name in the foot_geo.h file


	string m_bodyName;
	string m_regionName;

	vector<string> m_bodiesToBeSubtracted;

};


#endif



































