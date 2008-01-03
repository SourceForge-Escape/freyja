/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : MetaData
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: This is for doing things that can't be done.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.04.29:
 * Mongoose - Created
 ==========================================================================*/

#include <mstl/Vector.h>

#include "freyja.h"
#include "Mesh.h"

#include "Metadata.h"


#define freyja_print freyjaPrintMessage

using namespace freyja;
using namespace mstl;


////////////////////////////////////////////////////////////
// Public 
////////////////////////////////////////////////////////////

void Metadata::SetModel(const char* model)
{
	// FIXME: Load mesh into scene if not already loaded, and
	// set renderable to this metadata's model.
}


bool Metadata::Serialize(XMLSerializerNode container) const
{
#if TINYXML_FOUND
	if ( !container )
		return false;

	TiXmlElement *metadata = new TiXmlElement("freyja-metadata");
	container->LinkEndChild( metadata );
	metadata->SetAttribute("name", mName.c_str() );
	metadata->SetAttribute("type", mMetadataType.c_str() );

	TiXmlElement *pos = new TiXmlElement("position");
	metadata->LinkEndChild( pos );
	pos->SetDoubleAttribute("x", mPosition.mX);
	pos->SetDoubleAttribute("y", mPosition.mY);
	pos->SetDoubleAttribute("z", mPosition.mZ);	

	TiXmlElement *rot = new TiXmlElement("orientation");
	metadata->LinkEndChild( rot );
	rot->SetDoubleAttribute("x", mOrientation.mX);
	rot->SetDoubleAttribute("y", mOrientation.mY);
	rot->SetDoubleAttribute("z", mOrientation.mZ);	
	rot->SetDoubleAttribute("w", mOrientation.mW);	

	TiXmlElement *scale = new TiXmlElement("scale");
	metadata->LinkEndChild( scale );
	scale->SetDoubleAttribute("x", mScale.mX);
	scale->SetDoubleAttribute("y", mScale.mY);
	scale->SetDoubleAttribute("z", mScale.mZ);	

	if ( mMetadata.c_str() )
	{
		TiXmlElement *data = new TiXmlElement("metadata");
		TiXmlText *text = new TiXmlText( mMetadata.c_str() );
		//text->SetCDATA(true);
		data->LinkEndChild( text );
		metadata->LinkEndChild( data );
	}

#warning FIXME Need to serialize face:material relationships via MeshRenderables.
#if FIXME
	if ( GetMaterial() )
	{
		TiXmlElement *data = new TiXmlElement("material");
		TiXmlText *text = new TiXmlText( GetMaterial()->GetName() );
		//text->SetCDATA(true);
		data->LinkEndChild( text );
		metadata->LinkEndChild( data );
	}
#endif

	if ( GetModel() )
	{
		TiXmlElement *data = new TiXmlElement("model");
		TiXmlText *text = new TiXmlText( GetModel() );
		//text->SetCDATA(true);
		data->LinkEndChild( text );
		metadata->LinkEndChild( data );
	}

	return true;
#else // TINYXML_FOUND
	return false;
#endif // TINYXML_FOUND
}


bool Metadata::Unserialize(XMLSerializerNode metadata)
{
#if TINYXML_FOUND
	if ( !metadata )
		return false;

	{
		String s = metadata->Value(); 
		if ( s != "freyja-metadata" )
			return false;
	}

	mName = metadata->Attribute("name");
	mMetadataType = metadata->Attribute("type");

	TiXmlElement *child = metadata->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		if (s == "position")
		{
			child->QueryFloatAttribute("x", &mPosition.mX);
			child->QueryFloatAttribute("y", &mPosition.mY);
			child->QueryFloatAttribute("z", &mPosition.mZ);
		}
		else if (s == "scale")
		{
			child->QueryFloatAttribute("x", &mScale.mX);
			child->QueryFloatAttribute("y", &mScale.mY);
			child->QueryFloatAttribute("z", &mScale.mZ);
		}
		else if (s == "orientation")
		{
			// FIXME: Not supported as quat yet
			child->QueryFloatAttribute("x", &mOrientation.mX);
			child->QueryFloatAttribute("y", &mOrientation.mY);
			child->QueryFloatAttribute("z", &mOrientation.mZ);
			child->QueryFloatAttribute("w", &mOrientation.mW);
		}
		else if (s == "metadata")
		{
			mMetadata = child->GetText();
		}
		else if (s == "material")
		{
			// FIXME: Load from resource manager!
			//mMaterial = child->GetText();
		}
		else if (s == "model")
		{
			SetModel( child->GetText() );
		}
	}

	return true;
#else // TINYXML_FOUND
	return false;
#endif // TINYXML_FOUND
}




////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_METADATA
int runMetadataUnitTest(int argc, char *argv[])
{
	Metadata test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Metadata class test]\n");

	return runMetadataUnitTest(argc, argv);
}
#endif
