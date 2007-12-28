/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project: Freyja
 * Author : Terry 'Mongoose' Hendrix II
 * Website: http://www.icculus.org/freyja/
 * Email  : mongooseichiban@gmail.com
 * License: No use w/o permission (C) 2004-2006 Mongoose
 *
 * Object    : Weight
 * Owner     : Mesh
 * References: Bone, Vertex
 * Provides  : Weight value for Vertex:Bone
 *
 * Comments: Might want to use conditional constructor.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Refactored.
 *
 * 2006.12.09:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_WEIGHT_H_
#define GUARD__FREYJA_WEIGHT_H_

#include "freyja.h"


namespace freyja {

class Vertex;
class Bone;

class Weight 
{
public:
	
	Weight( freyja::Bone* bone, freyja::Vertex* vertex, vec_t weight ) :
		mBone(bone), 
		mVertex(vertex),
		mWeight(weight)
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	Weight( const Weight &weight ) :
		mBone(weight.mBone),
		mVertex(weight.mVertex),
		mWeight(weight.mWeight)
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Copy Constructor.
	 *
	 ------------------------------------------------------*/

	~Weight()
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	freyja::Bone* mBone;

	freyja::Vertex* mVertex;

	vec_t mWeight;
};



#if 0
	//TINYXML_FOUND
	bool SerializeWeight(TiXmlElement *container)
	{	
		if (!container)
			return false;	
		
		TiXmlElement *weight = new TiXmlElement("weight");
		int attr;
		
		attr = (mVertexIndex == INDEX_INVALID) ? -1 : mVertexIndex;
		weight->SetAttribute("vertex", attr);
		
		attr = (mBoneIndex == INDEX_INVALID) ? -1 : mBoneIndex;
		weight->SetAttribute("bone", attr);

		weight->SetDoubleAttribute("value", mWeight);
		container->LinkEndChild(weight);

		return true;
	}

	bool UnserializeWeight(TiXmlElement *container)
	{
		if (!container)
			return false;

		TiXmlElement *weight = container;//->FirstChildElement("weight");

		if (!weight)
			return false;

		int attr;
		weight->QueryIntAttribute("vertex", &attr);
		mVertexIndex = attr < 0 ? INDEX_INVALID : attr;
		weight->QueryIntAttribute("bone", &attr);
		mBoneIndex = attr < 0 ? INDEX_INVALID : attr;
		weight->QueryFloatAttribute("value", &mWeight);

		return true;
	}
#endif

} // End namespace freyja


#endif // GUARD__FREYJA_WEIGHT_H_
