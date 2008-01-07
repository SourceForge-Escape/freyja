/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Material
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Material class for libfreyja, yadda yadda
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.01.08:
 * Mongoose - Created
 ==========================================================================*/

#include "Material.h"

using namespace mstl;
using namespace freyja;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Material::Material( const char* name ) :
	mName( name ),
	mFilename( ),
	mShaderFilename( ),
	mAlpha( false ),
	mBlending( false ),
	mShaderId( -1 ),
	mMetadata( ),
	mShader( ),
	mBlendSrc( "GL_ONE" ),
	mBlendDest( "GL_ONE" ),
	mShininess( 0.0f ),
	mTransparency( 0.0f ),
	mDiffuseMap( NULL ),
	mDecalMap( NULL ),
	mEmissiveMap( NULL ),
	mHeightMap( NULL ),
	mNormalMap( NULL ),
	mSpecularMap( NULL ),
	mTexture0Id( -1 ),
	mTexture1Id( -1 ),
	mTexture2Id( -1 ),
	mTexture3Id( -1 ),
	mTexture4Id( -1 )
{
	mAmbient[0]  = mAmbient[1]  = mAmbient[2]  = 0.2f;
	mAmbient[3]  = 1.0f;

	mDiffuse[0]  = mDiffuse[1]  = mDiffuse[2]  = 0.9f;
	mDiffuse[3]  = 1.0f;

	mSpecular[0] = mSpecular[1] = mSpecular[2] = 0.0f;
	mSpecular[3] = 1.0f;

	mEmissive[0] = mEmissive[1] = mEmissive[2] = 0.0f;
	mEmissive[3] = 1.0f;
}


Material::~Material()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Material::Serialize(XMLSerializerNode container) const
{
#if TINYXML_FOUND
	if (!container)
		return false;	
		
	TiXmlElement *mat = new TiXmlElement( GetType() );

	mat->SetAttribute( "version", GetVersion() );
	mat->SetAttribute( "name", GetName() );
	//mat->SetAttribute("flags", mFlags);

	//if (mTextureFilename.c_str() != 0x0)
	//{
	//	TiXmlElement *element = new TiXmlElement("texture");
	//	element->SetAttribute("diffuse", mTextureFilename.c_str() );
	//	mat->LinkEndChild(element);
	//}

	if ( mShaderFilename.c_str() != 0x0 )
	{
		TiXmlElement *element = new TiXmlElement("shader");
		element->SetAttribute("glsl", mShaderFilename.c_str() );
		mat->LinkEndChild(element);
	}

	{
		TiXmlElement *element = new TiXmlElement("blending");
		element->SetAttribute("blend-src", mBlendSrc.c_str() );
		element->SetAttribute("blend-dest", mBlendDest.c_str() );
		mat->LinkEndChild(element);
	}

	{
		TiXmlElement *element = new TiXmlElement("shininess");
		element->SetDoubleAttribute("exp", mShininess);
	}

	{
		char *rgba[] = {"r","g","b","a"};
		
		{
			TiXmlElement *color = new TiXmlElement("ambient");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mAmbient[i]);
			}

			mat->LinkEndChild(color);
		}

		{
			TiXmlElement *color = new TiXmlElement("diffuse");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mDiffuse[i]);
			}

			mat->LinkEndChild(color);
		}

		{
			TiXmlElement *color = new TiXmlElement("specular");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mSpecular[i]);
			}

			mat->LinkEndChild(color);
		}

		{
			TiXmlElement *color = new TiXmlElement("emissive");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mEmissive[i]);
			}

			mat->LinkEndChild(color);
		}
	}

	if (mMetadata.c_str() != NULL)
	{
		TiXmlElement *metadata = new TiXmlElement("metadata");
		TiXmlText *text = new TiXmlText( mMetadata.c_str() );
		metadata->LinkEndChild(text);
		mat->LinkEndChild(metadata);
	}

	container->LinkEndChild(mat);
	return true;
#else
	return false;
#endif
}


bool Material::Unserialize(XMLSerializerNode mat)
{
#if TINYXML_FOUND
	if (!mat)
		return false;	

	int attr;
	mat->QueryIntAttribute("version", &attr);

	if (attr != 3)
		return false;

	SetName( mat->Attribute("name") );

	mat->QueryIntAttribute("flags", &attr);
	//mFlags = attr < 0 ? INDEX_INVALID : attr;

	TiXmlElement *child = mat->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		if (s == "texture")
		{
			//mTextureFilename = child->Attribute("diffuse");
			//
			//if (mLoadTextureFunc) // callback to load texture, get id
			//{
			//	mTexture = mLoadTextureFunc( mTextureFilename.c_str() );
			//} 
		}
		else if (s == "shader")
		{
			mShaderFilename = child->Attribute("glsl");

			//if (mLoadShaderFunc) // callback to load shader, get id
			//{
			//	mShaderId = mLoadShaderFunc( mShaderFilename.c_str() );
			//} 
		}
		else if (s == "blending")
		{
			mBlendSrc = child->Attribute("blend-src");
			mBlendDest = child->Attribute("blend-dest");
		}
		else if (s == "shininess")
		{
			child->QueryFloatAttribute("exp", &mShininess);
		}
		else if (s == "ambient")
		{
			child->QueryFloatAttribute("r", mAmbient+0);
			child->QueryFloatAttribute("g", mAmbient+1);
			child->QueryFloatAttribute("b", mAmbient+2);
			child->QueryFloatAttribute("a", mAmbient+3);
		}
		else if (s == "diffuse")
		{
			child->QueryFloatAttribute("r", mDiffuse+0);
			child->QueryFloatAttribute("g", mDiffuse+1);
			child->QueryFloatAttribute("b", mDiffuse+2);
			child->QueryFloatAttribute("a", mDiffuse+3);
		}
		else if (s == "specular")
		{
			child->QueryFloatAttribute("r", mSpecular+0);
			child->QueryFloatAttribute("g", mSpecular+1);
			child->QueryFloatAttribute("b", mSpecular+2);
			child->QueryFloatAttribute("a", mSpecular+3);
		}
		else if (s == "emissive")
		{
			child->QueryFloatAttribute("r", mEmissive+0);
			child->QueryFloatAttribute("g", mEmissive+1);
			child->QueryFloatAttribute("b", mEmissive+2);
			child->QueryFloatAttribute("a", mEmissive+3);
		}
		else if (s == "metadata")
		{
			mMetadata = child->GetText();
		}
	}

	return true;
#else
	return false;
#endif	
}


freyja::Material* Material::Import( const char* filename )
{
#if TINYXML_FOUND


#if COMPRESSED_XML
	TiXmlDocument doc;

	/* Should handle files with and without gz compression. */
	{
		char* xml = NULL;
		unsigned int size;
		mstl::GzFileRead( filename, xml, size );
		doc.Parse( xml );
		
		/* FIXME: GzRead allocator should provide deallocator. */
		delete xml; 
	}
#else
	TiXmlDocument doc( filename );

	if ( !doc.LoadFile() )
	{
		printf("XML ERROR: %s, Line %i, Col %i\n", 
			   doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		return false;
	}
#endif

	if ( doc.Error() )
	{
		printf("XML ERROR: %s, Line %i, Col %i\n", 
			   doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		return false;
	}

	TiXmlElement* root = doc.RootElement(); 

	if (!root) 
	{
		printf("Couldn't find document root!\n");
		return false;
	}

	Material* mat = new Material( "?" );
	if ( !mat->Unserialize( root ) )
	{
		delete mat;
		mat = NULL;
	}

	return mat;

	//TiXmlElement* child = root->FirstChildElement();
	//for( ; child; child = child->NextSiblingElement() )
	//{
	//}
#else
	return NULL;
#endif
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAMATERIAL
int runMaterialUnitTest(int argc, char *argv[])
{
	// Removed obsolete unit test -- Mongoose, 2008.01.05
	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Material class test]\n");

	return runMaterialUnitTest(argc, argv);
}
#endif
