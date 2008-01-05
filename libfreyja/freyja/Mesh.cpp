/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaMesh and classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created
 ==========================================================================*/

#include "MeshRenderable.h"
#include "Mesh.h"


using namespace freyja;
using namespace hel;

#define GROUP_FACE_WITHOUT_BITMAPS 0


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Mesh::Mesh( const char* name ) :
	SceneNode( name ),
	mMeshOptions(0),
	mVertexArray( 0, 3 ),
	mColorArray( 0, 3 ),
	mNormalArray( 0, 3 ),
	mTexcoordArray( 0, 2 ),
	mGaps(),
	mSelectedVertices(),
	mSelectedFaces(),
	mSelectedEdges(),
	mFaces(),
	mVertices(),
	mEdges()
{ 
	mRenderables.push_back( new MeshRenderable( this, NULL ) );
}


Mesh::Mesh(const Mesh &mesh) :
	SceneNode( mesh.mName.c_str() ),
	mMeshOptions( mesh.mMeshOptions ),
	mVertexArray( mesh.mVertexArray ),
	mColorArray( mesh.mColorArray ),
	mNormalArray( mesh.mNormalArray ),
	mTexcoordArray( mesh.mTexcoordArray ),
	mGaps( mesh.mGaps ),
	mSelectedVertices( mesh.mSelectedVertices ),
	mSelectedFaces( mesh.mSelectedFaces ),
	mSelectedEdges( mesh.mSelectedEdges ),
	mFaces( mesh.mFaces ),
	mVertices( mesh.mVertices ),
	mEdges( mesh.mEdges )
{
	mesh.CopyNodeMembers( this );
	mRenderables.push_back( new MeshRenderable( this, NULL ) ); // FIXME: Need to copy over renderables.
}


Mesh::~Mesh()
{
	mFaces.clear();
	mVertices.clear();
	mEdges.clear();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Mesh::CopyVertexArray( freyja::FloatArray& array ) const
{ 
	array = mVertexArray;
	return ( array.GetSize() >= mVertexArray.GetSize() );
}


XMLSerializerNode Mesh::CreateXMLSerializerNode( ) const
{
	XMLSerializerNode mesh = Node::CreateXMLSerializerNode( );

	/* Use TinyXML's element->SetValue() to overwrite tag type string to handle derived classes. */

	/* Properties. */
	TiXmlElement* properties = new TiXmlElement( "mesh-properties" );
	properties->SetAttribute( "options", mMeshOptions );
	mesh->LinkEndChild( properties );

	/* Encapsulate arrays. */
	TiXmlElement* varray = new TiXmlElement( "vertex-array" );
	mVertexArray.Serialize( varray );
	mesh->LinkEndChild( varray );

	TiXmlElement* carray = new TiXmlElement( "color-array" );
	mColorArray.Serialize( carray );
	mesh->LinkEndChild( carray );

	TiXmlElement* narray = new TiXmlElement( "normal-array" );
	mNormalArray.Serialize( narray );
	mesh->LinkEndChild( narray );

	TiXmlElement* tarray = new TiXmlElement( "texcoord-array" );
	mTexcoordArray.Serialize( tarray );
	mesh->LinkEndChild( tarray );

	/* Gaps. */
	TiXmlElement* gaps = new TiXmlElement( "gaps" );
	mstl::list<uint32>::iterator it = mGaps.begin();
	while ( *it )
	{
		TiXmlElement* gap = new TiXmlElement( "gap" );
		properties->SetAttribute( "index", *it );
		gaps->LinkEndChild( gap );
	}	
	mesh->LinkEndChild( gaps );



	mstl::Vector<freyja::Vertex*> mVertices;

#warning FIXME Not finished.
	//mPlanes;
	//mEdges;
	
	{
		TiXmlElement* faces = new TiXmlElement( "faces" );
		mstl::Vector<freyja::Face*> mFaces;

		uint32 i;
		foreach( mFaces, i )
		{
			Face* face = mFaces[i];
			
		}

		mesh->LinkEndChild( faces );
	}

	return mesh;
}


bool Mesh::Serialize( XMLSerializerNode parent ) const
{
#if FIXME
	byte mMeshOptions;                     /* Options bitmap. */

	freyja::FloatArray mVertexArray;       /* Arrays to define geometry. */
	freyja::FloatArray mColorArray;
	freyja::FloatArray mNormalArray;
	freyja::FloatArray mTexcoordArray;

	mstl::stack<uint32> mGaps;             /* Gaps in array usage. */

	mstl::Vector<freyja::Face*> mFaces;

	mstl::Vector<freyja::Vertex*> mVertices;

	mstl::Vector<freyja::Plane*> mPlanes;

	mstl::Vector<freyja::Edge*> mEdges;
#endif
	return true;
}

bool Mesh::Unserialize( XMLSerializerNode node )
{
#warning FIXME
	return false;
}


mstl::String Mesh::GetInfo() const
{
#warning FIXME
	return mstl::String( "Mesh" );
}


void Mesh::DuplicateChildren( freyja::Node* parent, bool recurse )
{
#warning FIXME
}


void Mesh::AddVertexToFace( freyja::Face* face, freyja::Vertex* vertex )
{
	if ( !face || !vertex )
		return;

	face->AddVertex( vertex );

	if ( face->mVertices.size( ) == 3 )
	{
		uint32 i = 0, n = face->mVertices.size( ) - 3;
		for ( VertexIterator it = face->mVertices.begin(); it != it.end(); it++, ++i )
		{
			if ( i == n )
			{
				index_t a = (*it)->GetIndex();
				it++;
				index_t b = (*it)->GetIndex();
				it++;
				index_t c = (*it)->GetIndex();
				face->mTriangles.push_back( face->mRenderable->ReserveIndexTriangle( a/3, b/3, c/3 ) );
				break;
			}
		}
	}
	else if ( face->mVertices.size( ) > 3 )
	{
		/* Fans -> polygons... */
		uint32 i = 0, n = face->mVertices.size( ) - 2;
		index_t a;
		for ( VertexIterator it = face->mVertices.begin(); it != it.end(); it++, ++i )
		{
			if ( !i )
			{
				a = (*it)->GetIndex();
			}
			else if ( i == n )
			{
				index_t c = (*it)->GetIndex();
				it++;
				index_t d = (*it)->GetIndex();
				face->mTriangles.push_back( face->mRenderable->ReserveIndexTriangle( a/3, c/3, d/3 ) );
				break;
			}
		}
		
	}
}


freyja::Face* Mesh::CreateFace( )
{
	RenderableIterator it = mRenderables.begin( );
	freyja::Face* face = new freyja::Face( this, ((MeshRenderable*)(*it) ) );
	return face;
}


freyja::Vertex* Mesh::CreateVertex( const hel::Vec3 pos )
{
	hel::Vec3 tmp( 0.0f, 0.0f, 0.0f );
	index_t idx = mVertexArray.AppendElement( pos.mVec );
	mNormalArray.AppendElement( tmp.mVec );
	mTexcoordArray.AppendElement( tmp.mVec );

	freyja::Vertex* vertex = new freyja::Vertex( idx );
	return vertex;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAMESH
int runFreyjaMeshUnitTest(int argc, char *argv[])
{
	FreyjaMesh test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaMesh class test]\n");

	return runFreyjaMeshUnitTest(argc, argv);
}
#endif
