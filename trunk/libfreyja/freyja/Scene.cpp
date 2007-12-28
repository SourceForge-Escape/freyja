/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Scene
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the scene graph.
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SCENE - Builds class as a console unit test. 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.06:
 * Mongoose - Created
 ==========================================================================*/

#include "Scene.h"

using namespace freyja;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Scene::Scene( ) 
{
}


Scene::~Scene( )
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Scene::Serialize( const char* filename ) const
{
#if TINYXML_FOUND
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *container = new TiXmlElement("freyja");
	container->SetAttribute("version", VERSION);
	container->SetAttribute("build-date", __DATE__);
	container->SetAttribute("file-version", 1);
	doc.LinkEndChild(container);

	//FIXME SerializeMeshes( container );

	return doc.SaveFile(filename);;
#else
	return false;
#endif
}


bool Scene::Unserialize( const char* filename )
{
#if TINYXML_FOUND
	TiXmlDocument doc(filename);

	if ( !doc.LoadFile() )
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

	// FIXME
	
	return false;
#else
	return false;
#endif
}


bool Scene::Add( freyja::Mesh* mesh )
{
#warning FIXME
	return false;
}

freyja::Mesh* Scene::GetSelectedMesh() const
{
#warning FIXME
	return NULL;
}

void Scene::SetSelectedMesh( freyja::Mesh* mesh )
{
#warning FIXME
}

bool Scene::Remove( freyja::Mesh* mesh )
{
#warning FIXME
	return false;
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

#ifdef UNIT_TEST_BONE
int runBoneUnitTest(int argc, char *argv[])
{
	Bone bone;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Bone class test]\n");

	return runBoneUnitTest(argc, argv);
}
#endif




