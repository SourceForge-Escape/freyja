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

#include <mstl/System/IO/GzFile.h>
#include "Scene.h"

using namespace freyja;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Scene::Scene( ) :
	mFilename(),
	mRenderList(),
	mSelectedBones(),
	mCurrentBone( NULL ),
	mCameras( "Camera", mResourceDict ),
	mCurrentCamera( NULL ),
	mLights( "Light", mResourceDict ),
	mCurrentLight( NULL ),
	mMeshes( "Mesh", mResourceDict ),
	mCurrentMesh( NULL ),
	mMetadata( "Metadata", mResourceDict ),
	mCurrentMetadata( NULL ),
	mSkeletons( "Skeleton", mResourceDict ),
	mCurrentSkeleton( NULL ),
	mMaterials(),
	mPixelBuffers()
{ 
}


Scene::~Scene( )
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Scene::Serialize( const char* filename, bool compression ) const
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

	mCameras.Serialize( container );
	mLights.Serialize( container );
	mMeshes.Serialize( container );
	mMetadata.Serialize( container );
	mSkeletons.Serialize( container );

	bool ret = false;
	if ( compression )
	{
		TiXmlPrinter printer;
		printer.SetIndent( "\t" );
		doc.Accept( &printer );
		const char* xml = printer.CStr();

		if ( xml )
		{
			ret = mstl::GzFileWrite( filename, xml, strlen(xml) );
		}
	}
	else
	{
		ret = doc.SaveFile(filename);
	}

	return ret;
#else
	return false;
#endif
}


bool Scene::Unserialize( const char* filename )
{
#if TINYXML_FOUND

	TiXmlDocument doc;

#if 0	
	TiXmlDocument doc( filename );

	if ( !doc.LoadFile() )
	{
		printf("XML ERROR: %s, Line %i, Col %i\n", 
			   doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		return false;
	}
#endif

	/* Should handle files with and without gz compression. */
	{
		char* xml;
		unsigned int size;
		mstl::GzFileRead( filename, xml, size );
		doc.Parse( xml );
		
		/* FIXME: GzRead allocator should provide deallocator. */
		delete xml; 
	}

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

	TiXmlElement* child = root->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		const char* s = child->Value();
		
		if ( mCameras.GetType() == s )
		{
			Camera* node = new Camera( child->Attribute( "name" ) );
			node->Unserialize( child );
			Add( node );
		}
		else if ( mLights.GetType() == s )
		{
			Light* node = new Light( child->Attribute( "name" ) );
			node->Unserialize( child );
			Add( node );
		}
		else if ( mMeshes.GetType() == s )
		{
			Mesh* node = new Mesh( child->Attribute( "name" ) );
			node->Unserialize( child );
			Add( node );
		}
		else if ( mMetadata.GetType() == s )
		{
			Metadata* node = new Metadata( child->Attribute( "name" ) );
			node->Unserialize( child );
			Add( node );
		}
		else if ( mSkeletons.GetType() == s )
		{
			Skeleton* node = new Skeleton( child->Attribute( "name" ) );
			node->Unserialize( child );
			Add( node );
		}
	}

	return false;
#else
	return false;
#endif
}


bool Scene::Add( freyja::Node* node )
{
	bool ret = false;

	if ( node )
	{
		mstl::String key = node->GetType();
		SceneResource* resource = mResourceDict.find( key );

		if ( resource )
		{
			bool found = false;
			for ( NodeIterator it = resource->GetList().begin(); *it; it++ )
			{
				if ( *it == node )
				{
					found = true;
					break;
				}
			}

			if ( !found )
			{					
				// FIXME: Check for name collision!
				
				resource->GetList().push_back( node );					
				//resource->GetList().unique();
				key = node->GetName();
				resource->GetDictionary().insert( key, node );
			}

			ret = true;
		}
	}

	return ret;
}


bool Scene::Remove( freyja::Node* node )
{
	NodeList* lst = GetList( node );
	bool ret = false;

	if ( lst)
	{
		// FIXME: Remove from selected list also.
		lst->remove( node );
		ret = true;
	}

	return ret;
}


void Scene::Select( freyja::Node* node )
{
	NodeList* lst = GetSelectionList( node );
	if ( lst)
	{
		bool found = false;
		for ( NodeIterator it = lst->begin(); *it; it++ )
		{
			if ( *it == node )
			{
				found = true;
				break;
			}
		}

		if ( !found )
			lst->push_back( node );
		
		//lst->unique();
	}
}


void Scene::Unselect( freyja::Node* node )
{
	NodeList* lst = GetSelectionList( node );
	if ( lst)
	{
		lst->remove( node );
	}
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Protected mutators.
////////////////////////////////////////////////////////////

NodeList* Scene::GetList( freyja::Node* node )
{
	NodeList* lst = NULL;
	if ( node )
	{
		mstl::String s = node->GetType();
		SceneResource* resource = mResourceDict.find( s );

		if ( resource )
		{
			lst = &(resource->GetList());
		}
	}

	return lst;
}


NodeList* Scene::GetSelectionList( freyja::Node* node )
{
	NodeList* lst = NULL;
	if ( node )
	{
		mstl::String s = node->GetType();

		/* Special case. */
		if ( s == "Bone" )
		{
			lst = &mSelectedBones;
		}
		else
		{
			SceneResource* resource = mResourceDict.find( s );
			if ( resource )
			{
				lst = &(resource->GetSelectedList());
			}
		}
	}

	return lst;
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJA_SCENE
int FreyjaSceneUnitTest( int argc, char* argv[] )
{
	Scene scene;

	return 0;
}


int main( int argc, char* argv[] )
{
	printf("[Scene class test]\n");

	return FreyjaSceneUnitTest(argc, argv);
}
#endif




