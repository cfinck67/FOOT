
#include "FootBox.hxx"




FootBox::FootBox ( 	TVector3 origin, TVector3 dimension, 
					string materialName, string materialRegionName, string bodyName, string regionName, 
					int volumeID ):

					m_origin(origin), m_dimension(dimension), 
					m_materialName(materialName), m_materialRegionName(materialRegionName), m_bodyName(bodyName), m_regionName(regionName), 
					m_volumeID(volumeID)
{

	// box == region
	m_name = m_regionName;

	// check if the mterial of the box is defined
	if ( !gGeoManager->GetMedium( m_materialName.c_str() ) ) {
		cout << "ERROR >> FootBox::FootBox  ::  Material " << m_materialName << " not defined in gGeoManager for the region " << m_regionName;
		exit(0);
	}

	// if the volume of the box is NOT defined  -->  define it
	if ( !gGeoManager->GetVolume( m_materialRegionName.c_str() ) ) {

		TGeoVolume *rootVolume = gGeoManager->MakeBox( m_materialRegionName.c_str() ,gGeoManager->GetMedium( m_materialName.c_str() ),
                                                m_dimension.x()/2, m_dimension.y()/2, m_dimension.z()/2); 
        rootVolume->SetLineColor(kOrange); 

	}

};