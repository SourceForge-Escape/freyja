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

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include "Octree.h"

using namespace hel;


class XMLNode
{
public:
	XMLNode() :
		mNode(NULL),
		mElement(NULL)
	{}

	XMLNode(OctreeNode* node, TiXmlElement* element) :
		mNode(node),
		mElement(element)
	{}

	OctreeNode* mNode;
	TiXmlElement* mElement;
};

//template<>
//XMLNode* mstl::list<XMLNode*>::mDefault = NULL; 


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

#if TINYXML_FOUND

bool Octree::Serialize(TiXmlElement* octree)
{
	if ( !octree )
		return false;

	// Metadata
	if ( mMetadata.c_str() )
	{
		TiXmlElement *metadata = new TiXmlElement("metadata");
		TiXmlText *text = new TiXmlText( mMetadata.c_str() );
		//text->SetCDATA(true);
		metadata->LinkEndChild(text);
		octree->LinkEndChild( metadata );
	}

	// Append nodes to octree
	mstl::list<XMLNode *> queue;

	queue.push_back( new XMLNode(&mRoot, octree) );

	while ( queue.front() )
	{
		XMLNode* xmlnode = queue.front();
		queue.pop_front();

		OctreeNode *node = xmlnode->mNode;
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

	return true;	
}


bool Octree::Unserialize(TiXmlElement* octree)
{
	if ( !octree )
		return false;

	Reset();

	// Append nodes to octree
	mstl::list<XMLNode *> stack;

	stack.push_front( new XMLNode(&mRoot, octree) );

	while ( stack.front() )
	{
		XMLNode* xmlnode = stack.front();
		TiXmlElement* element = xmlnode->mElement;
		OctreeNode* node = xmlnode->mNode;
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
				OctreeNode* child = new OctreeNode();
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

	return true;
}

#endif // TINYXML_FOUND


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
