/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Octree
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.08.25:
 * Mongoose - Created
 ==========================================================================*/

#include <mstl/list.h>
#include <tinyxml/tinyxml.h>

#include "Octree.h"

class XMLNode
{
public:
	XMLNode() :
		mNode(NULL),
		mElement(NULL)
	{}

	XMLNode(Octree::Node* node, TiXmlElement* element) :
		mNode(node),
		mElement(element)
	{}

	Octree::Node* mNode;
	TiXmlElement* mElement;
};

template<>
XMLNode* mstl::list<XMLNode*>::mDefault = NULL; 


void Octree::Node::SubdivChild(OctreeHelper &helper, hel::Vec3 &min, hel::Vec3 &max)
{
	// Test for geometry in the node region.
	uint32 count = helper.GetFaceCountInBBox(min, max); 

	if ( count )
	{
		Octree::Node *child = new Octree::Node(min, max);
		mChildren.push_back( child );

		if ( count > helper.GetMaxCount() )
		{
			child->Subdiv(helper);
		}
		else
		{
			// Only save face indices in leaves for now.
			helper.GetFacesIndicesInBBox(min, max, child->mFaces);
		}
	}
}


void Octree::Node::Subdiv(OctreeHelper &helper)
{
	//helper.GetFacesIndicesInBBox(mMin, mMax, mFaces);

	// Compute the axis aligned bounding box (AABB) centroid.
	hel::Vec3 center = (mMin + mMax) * 0.5f;

	// Use the centroid to compute 8 new AABBs for subdivision.

	// TODO: Threading would be nice here.  This means you'd have to
	// use internal data structure for CS access control.  freyja::Mesh is not
	// thread safe for example.
	//
	// Using mstl thread delegate on the SubdivChild calls would be easy then.

	// 'Bottom' AABBs
	SubdivChild(helper, mMin, center);

	{
		hel::Vec3 min(mMin), max(center); 
		min.mX = center.mX;
		max.mX = mMax.mX;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(mMin), max(center); 
		min.mZ = center.mZ;
		max.mZ = mMax.mZ;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(center), max(mMax); 
		min.mY = mMin.mY;
		max.mY = center.mY;
		SubdivChild(helper, min, max);
	}

	// 'Top' AABBs.
	SubdivChild(helper, center, mMax);

	{
		hel::Vec3 min(center), max(mMax); 
		min.mX = mMin.mX;
		max.mX = center.mX;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(center), max(mMax); 
		min.mZ = mMin.mZ;
		max.mZ = center.mZ;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(mMin), max(center); 
		min.mY = center.mY;
		max.mY = mMax.mY;
		SubdivChild(helper, min, max);
	}
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Octree::Octree() :
	mMetadata(),
	mVertices(),
	mFaces(),
	mRoot()
{
}


Octree::~Octree()
{
}


bool Octree::Serialize(const char *filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *container = new TiXmlElement("freyja");
	doc.LinkEndChild(container);

	TiXmlElement *root = new TiXmlElement("octree");
	container->LinkEndChild( root );

	// FIXME: Store metadata.

	// Append nodes to octree
	mstl::list<XMLNode *> queue;

	queue.push_back( new XMLNode(&mRoot, root) );

	while ( queue.front() )
	{
		XMLNode* xmlnode = queue.front();
		queue.pop_front();

		Octree::Node *node = xmlnode->mNode;
		TiXmlElement *parent = xmlnode->mElement;
		delete xmlnode;

		TiXmlElement *child = new TiXmlElement("node");

		TiXmlElement *min = new TiXmlElement("min");
		min->SetDoubleAttribute("x", node->mMin.mX);
		min->SetDoubleAttribute("y", node->mMin.mY);
		min->SetDoubleAttribute("z", node->mMin.mZ);
		child->LinkEndChild(min);

		TiXmlElement *max = new TiXmlElement("max");
		max->SetDoubleAttribute("x", node->mMax.mX);
		max->SetDoubleAttribute("y", node->mMax.mY);
		max->SetDoubleAttribute("z", node->mMax.mZ);
		child->LinkEndChild(max);

		// children of 'child'
		for (uint32 i = 0, n = node->mChildren.size(); i < n; ++i)
		{
			if ( node->mChildren[i] )
				queue.push_back( new XMLNode(node->mChildren[i], child) );
		}

		// faces		
		TiXmlElement *faces = new TiXmlElement("faces");
		for (uint32 i = 0, n = node->mFaces.size(); i < n; ++i)
		{
			TiXmlElement *face = new TiXmlElement("face");
			face->SetAttribute("index", node->mFaces[i]);
			faces->LinkEndChild(face);
		}

		child->LinkEndChild(faces);

		parent->LinkEndChild(child);
	}

	doc.SaveFile(filename);

	return true;
}


bool Octree::Unserialize(const char *filename)
{
	TiXmlDocument doc(filename);

	printf("@ Loading XML...\n");

	/* File loading and error reporting/handling. */
	{
		bool fatal = false;

		if ( !doc.LoadFile() )
		{
			fatal = true;
		}

		if ( doc.Error() )
		{
			printf("XML ERROR: %s, Line %i, Col %i\n", 
				doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		}

		//mDoc.Print();

		if ( fatal )
			return false;
	}

	printf("@ XML loading successful.\n");

	TiXmlElement *root = doc.RootElement(); 

	if (!root) 
	{
		printf("Couldn't find document root!\n");
		return false;
	}

	// FIXME: Abstract octree and walkmesh to functions and test for tags.

	// <octree>
	TiXmlElement *octree = root->FirstChildElement();

	// Append nodes to octree
	mstl::list<XMLNode *> stack;

	stack.push_front( new XMLNode(&mRoot, octree) );

	while ( stack.front() )
	{
		XMLNode* xmlnode = stack.front();
		TiXmlElement* element = xmlnode->mElement;
		Octree::Node* node = xmlnode->mNode;
		stack.pop_front();

		delete xmlnode;

		TiXmlElement *cur = element->FirstChildElement();

		for( ; cur; cur = cur->NextSiblingElement() )
		{
			if ( !cur->Value() )
				continue;

			if ( !strncmp("min", cur->Value(), 3) )
			{
				cur->QueryFloatAttribute("x", &node->mMin.mX);
				cur->QueryFloatAttribute("y", &node->mMin.mY);
				cur->QueryFloatAttribute("z", &node->mMin.mZ);
			}
			else if ( !strncmp("max", cur->Value(), 3) )
			{
				cur->QueryFloatAttribute("x", &node->mMax.mX);
				cur->QueryFloatAttribute("y", &node->mMax.mY);
				cur->QueryFloatAttribute("z", &node->mMax.mZ);
			}
			else if ( !strncmp("node", cur->Value(), 4) )
			{
				Octree::Node* child = new Octree::Node();
				node->mChildren.push_back( child );
				stack.push_front( new XMLNode(child, cur) );			
			}
			else if ( !strncmp("faces", cur->Value(), 5) )
			{	
				TiXmlElement *face = cur->FirstChildElement();

				for( ; face; face = face->NextSiblingElement() )
				{
					int idx;
					face->QueryIntAttribute("index", &idx);
					node->mFaces.push_back( idx );
				}
			}
		}
	}


	// <walkmesh>
	TiXmlElement *walkmesh = root->FirstChildElement();

	TiXmlElement *cur = walkmesh->FirstChildElement();

	for( ; cur; cur = cur->NextSiblingElement() )
	{
		if ( !strncmp("faces", cur->Value(), 5) )
		{
			// FIXME: scan uints from text, store in mFaces
		}
		else if ( !strncmp("vertices", cur->Value(), 8) )
		{
			// FIXME: scan floats from text, store in mVertices
		}
	}

	return true;
}






////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


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

#ifdef UNIT_TEST_OCTREE
int runOctreeUnitTest(int argc, char *argv[])
{
	Octree test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Octree class test]\n");

	return runOctreeUnitTest(argc, argv);
}
#endif
