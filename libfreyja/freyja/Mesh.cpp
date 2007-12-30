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
	mVertexArray(),
	mColorArray(),
	mNormalArray(),
	mTexcoordArray(),
	mGaps(),
	mSelectedVertices(),
	mSelectedFaces(),
	mSelectedEdges(),
	mFaces(),
	mVertices(),
	mEdges()
{ 
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


const hel::Quat& Mesh::GetWorldOrientation() const
{
#warning FIXME
	return mOrientation;
}


const hel::Vec3& Mesh::GetWorldPosition() const
{
#warning FIXME
	return mPosition;
}


void Mesh::DuplicateChildren( freyja::Node* parent, bool recurse )
{
#warning FIXME
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
