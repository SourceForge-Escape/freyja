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

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include "freyja.h"
#include "Mesh.h"

#include "Metadata.h"


#define freyja_print freyjaPrintMessage

using namespace freyja;
using namespace mstl;


Vector<Metadata*> gMetadataGobalPool;

// FIXME: Flush pool on exit!!!!


////////////////////////////////////////////////////////////
// Public 
////////////////////////////////////////////////////////////

bool Metadata::AddToPool()
{
	// This must not be in pool yet if uid is invalid.
	if ( mUid == INDEX_INVALID )
	{
		for (uint32 i = 0, count = gMetadataGobalPool.size(); i < count; ++i)
		{
			if ( gMetadataGobalPool[i] == 0x0 )
			{
				mUid = i;
				gMetadataGobalPool.assign( mUid, this );
				return true;
			}
			else if ( gMetadataGobalPool[i] == this )
			{
				// Possible error state was found, so quietly 'fix'.
				// This is mostly likely caused by calling the method repeatedly.
				mUid = i;
				return false;
			}
		}

		mUid = gMetadataGobalPool.size();
		gMetadataGobalPool.push_back( this );
		return true;
	}

	return false;
}


Metadata *Metadata::GetObjectByUid(index_t uid)
{
	if ( uid < gMetadataGobalPool.size() )
		return gMetadataGobalPool[uid];

	return NULL;
}


uint32 Metadata::GetObjectCount()
{
	return gMetadataGobalPool.size();
}


bool Metadata::RemoveFromPool()
{
	if ( mUid < gMetadataGobalPool.size() )
	{
		gMetadataGobalPool.assign(mUid, 0x0);
		mUid = INDEX_INVALID;
		return true;
	}

	// Possible error state was found, so quietly 'fix'.
	// This is mostly likely caused by calling the method repeatedly.
	mUid = INDEX_INVALID;
	return false;
}


void Metadata::SetModel(const char* model)
{
	if ( !model )
		return;

	// Do we already use the requested renderable instance?
	if ( GetModel() && !strcmp( GetModel(), model ) )
	{
		// Do some 'check cache' call here later optionally.
	}
	else
	{
		mRenderable = Renderable::ImportToCache( model );
	}
}


#if TINYXML_FOUND

bool Metadata::Serialize(TiXmlElement* container)
{
	if ( !container )
		return false;

	TiXmlElement *metadata = new TiXmlElement("freyja-metadata");
	container->LinkEndChild( metadata );
	metadata->SetAttribute("name", mName.c_str() );
	metadata->SetAttribute("type", mType.c_str() );

	TiXmlElement *pos = new TiXmlElement("pos");
	metadata->LinkEndChild( pos );
	pos->SetDoubleAttribute("x", mPos.mX);
	pos->SetDoubleAttribute("y", mPos.mY);
	pos->SetDoubleAttribute("z", mPos.mZ);	

	TiXmlElement *rot = new TiXmlElement("rot");
	metadata->LinkEndChild( rot );
	rot->SetDoubleAttribute("x", mRot.mX);
	rot->SetDoubleAttribute("y", mRot.mY);
	rot->SetDoubleAttribute("z", mRot.mZ);	
	rot->SetDoubleAttribute("w", mRot.mW);	

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

	if ( mMaterial.c_str() )
	{
		TiXmlElement *data = new TiXmlElement("material");
		TiXmlText *text = new TiXmlText( mMaterial.c_str() );
		//text->SetCDATA(true);
		data->LinkEndChild( text );
		metadata->LinkEndChild( data );
	}

	if ( GetModel() )
	{
		TiXmlElement *data = new TiXmlElement("model");
		TiXmlText *text = new TiXmlText( GetModel() );
		//text->SetCDATA(true);
		data->LinkEndChild( text );
		metadata->LinkEndChild( data );
	}

	return true;
}


bool Metadata::Unserialize(TiXmlElement* metadata)
{
	if ( !metadata )
		return false;

	{
		String s = metadata->Value(); 
		if ( s != "freyja-metadata" )
			return false;
	}

	mName = metadata->Attribute("name");

	mType = metadata->Attribute("type");

	TiXmlElement *child = metadata->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		if (s == "pos")
		{
			child->QueryFloatAttribute("x", &mPos.mX);
			child->QueryFloatAttribute("y", &mPos.mY);
			child->QueryFloatAttribute("z", &mPos.mZ);
		}
		else if (s == "scale")
		{
			// FIXME: Not supported as quat yet
			child->QueryFloatAttribute("x", &mScale.mX);
			child->QueryFloatAttribute("y", &mScale.mY);
			child->QueryFloatAttribute("z", &mScale.mZ);
		}
		else if (s == "rot")
		{
			// FIXME: Not supported as quat yet
			child->QueryFloatAttribute("x", &mRot.mX);
			child->QueryFloatAttribute("y", &mRot.mY);
			child->QueryFloatAttribute("z", &mRot.mZ);
			child->QueryFloatAttribute("w", &mRot.mW);
		}
		else if (s == "metadata")
		{
			mMetadata = child->GetText();
	//index_t mMaterialIndex;  // set via material loader using string as input ^
		}
		else if (s == "material")
		{
			mMaterial = child->GetText();
		}
		else if (s == "model")
		{
			SetModel( child->GetText() );
			// FIXME: Setup renderable!
			//mModel = MetadataRenderable::GetInstance( child->GetText() );
		}
	}

	return true;
}

#endif // TINYXML_FOUND


bool Metadata::Unserialize(const char* filename)
{
#if TINYXML_FOUND
	if (!filename)
		return false;

	TiXmlDocument doc(filename);

	if ( !doc.LoadFile() )
	{
		freyja_print("XML ERROR: %s, Line %i, Col %i\n", 
		             doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		return false;
	}

	TiXmlElement *root = doc.RootElement(); 

	if (!root) 
	{
		freyja_print("XML ERROR: Couldn't find document root for '%s'!\n", filename );
		return false;
	}

	TiXmlElement *child = root->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		// Only handle the first one encountered given this is a class method.
		if ( s == "freyja-metadata" )
		{
			return Unserialize( child );
		}
	}

#endif // TINYXML_FOUND

	return false;
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
