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
	helper.GetFacesIndicesInBBox(mMin, mMax, mFaces);

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

	TiXmlElement *octree = new TiXmlElement("octree");
	container->LinkEndChild(octree);

#if 0
	// Append nodes
	mstl::list<Octree::Node *> q;
	mstl::list<TiXmlElement *> qx;
	q.push_front(&mRoot);
	qx.push_front(octree);

	while ( !q.empty() )
	{
		TiXmlElement *parent = qx.front();
		qx.pop_front();

		Octree::Node *node = q.front();
		q.pop_front();

		TiXmlElement *xmlNode = new TiXmlElement("node");

		TiXmlElement *min = new TiXmlElement("min");
		min->SetAttribute("x", node.mMin.mX);
		min->SetAttribute("y", node.mMin.mY);
		min->SetAttribute("z", node.mMin.mZ);
		xmlNode->LinkEndChild(min);

		TiXmlElement *max = new TiXmlElement("max");
		max->SetAttribute("x", node.mMax.mX);
		max->SetAttribute("y", node.mMax.mY);
		max->SetAttribute("z", node.mMax.mZ);
		xmlNode->LinkEndChild(max);

		// children

		for (uint32 i = 0, n = node->mChildren.size(); i < n; ++i)
		{
			
		}

		// faces

		parent->LinkEndChild(xmlNode);
	}
#endif

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

#if 0
model = root;

		printf("-- model '%s'\n", model->Value());
		//model->Attribute("type");
		//model->Attribute("version");
		//model->Attribute("name");

		TiXmlElement *cur = model->FirstChildElement();


		for( ; cur; cur = cur->NextSiblingElement() )
		{
			const char *key = cur->Value();

			printf("-- ? '%s'\n", cur->Value());

			if (key == NULL)
			{
			}
			else if ( !strncmp("skeleton", key, 8) )
			{						
				printf("-- skeleton\n");
				//key->Attribute("version");
				index_t skeleton = freyjaSkeletonCreate();
				freyjaModelAddSkeleton(0, skeleton);
				uint32 numJoints = 0;

				TiXmlElement *bones = cur->FirstChildElement();
				//if (bones) bones->Attribute("version");

				TiXmlElement *bone = (bones) ? bones->FirstChildElement() : NULL;
				for( ; bone; bone = bone->NextSiblingElement() )
				{
					const char *bsym = bone->Value();
					if ( strncmp("bone", bsym, 15) == 0 )
					{
						++numJoints;
						int id;
						cur->QueryIntAttribute("id", &id);
						const char *name = bone->Attribute("name");
						index_t bIndex = freyjaBoneCreate(skeleton);
						freyjaBoneName(bIndex, name);
						// Map bid, id might be useful

						printf("-- bone '%s'\n", name);

						TiXmlElement *b = bone->FirstChildElement();
						while (b)
						{
							if (b->Value() && !strcmp("position", b->Value()) )
							{
								float x, y, z;
								b->QueryFloatAttribute("x", &x);
								b->QueryFloatAttribute("y", &y);
								b->QueryFloatAttribute("z", &z);
								freyjaBoneTranslate3f(bIndex, x, y, z);
							}
							else if (b->Value() && !strcmp("rotation", b->Value()) )
							{
								float theta;
								b->QueryFloatAttribute("angle", &theta);
								TiXmlElement *axis = b->FirstChildElement();
								if (axis && !strcmp("axis", axis->Value()) )
								{
									float x, y, z;
									axis->QueryFloatAttribute("x", &x);
									axis->QueryFloatAttribute("y", &y);
									axis->QueryFloatAttribute("z", &z);

									printf("-- %f %f %f %f\n", theta, x, y, z);
#if 1
					hel::Vec3 v(x, y, z);
					v.Norm();
					
					float alpha, beta; 
					helSinCosf(theta*0.5f, &beta, &alpha);
					v *= beta;

					// -alpha <-- convert coord system
					//freyjaBoneRotateQuat4f(bIndex, -alpha, v.mX, v.mY, v.mZ);
					freyjaBoneRotateQuat4f(bIndex, -alpha, v.mX, v.mY, v.mZ);//v.mZ, v.mY);
#else
									hel::Quat q;
									q.SetByAxisAngles(theta*57.0f, x, y, z);
									freyjaBoneRotateQuat4f(bIndex, q.mW, q.mX, q.mY, q.mZ);
#endif

								}
							}
				
							b = b->NextSiblingElement();
						}
					}
				}


				TiXmlElement *h = bones->NextSiblingElement();
				printf("-- bonehierarchy '%s'\n", h ? h->Value() : NULL);
				//key->Attribute("version");

				//<boneparent bone="schienagiu" parent="addome"/>
				TiXmlElement *p = (h) ? h->FirstChildElement() : NULL;
				for ( ; p; p = p->NextSiblingElement() )
				{
					const char *bone = p->Attribute("bone");
					const char *parent = p->Attribute("parent");
					int parentIndex = -1, childIndex = -1;

					if (!bone || !parent)
						continue;

					printf("-- boneparent '%s' = %s, %s\n", p->Value(), bone, parent);
#if 1
					for (uint32 j = 0; j < numJoints; ++j)
					{
						// If we supported multi-skeleton this would need to be mapped
						const char *contestant = freyjaGetBoneNameString(j);

						if (contestant && !strcmp(parent, contestant))
						{
							parentIndex = j;
						}
						else if (contestant && !strcmp(bone, contestant))
						{
							childIndex = j;
						}
					}

					if (childIndex != parentIndex)
					{
						freyjaBoneAddChild(parentIndex, childIndex);
						freyjaBoneParent(childIndex, parentIndex);
					}	
#endif			
				}

				freyjaSkeletonUpdateBones(skeleton);
			}
			else if ( strncmp("weights", key, 7) == 0 )
			{
				printf("-- weights '%s'\n", cur->Value());
				//const char *version = cur->Attribute("version");

				TiXmlElement *child = cur->FirstChildElement();
				for( ; child; child = child->NextSiblingElement() )
				{
					const char *key2 = child->Value();
					printf("-- vertexweight '%s'\n", child->Value());

					if ( !strncmp("vertexweight", key2, 12) )
					{
						int v, b;
						float w;
						child->QueryIntAttribute("vertexindex", &v);
						child->QueryIntAttribute("boneindex", &b);
						child->QueryFloatAttribute("weight", &w);

						printf("-- %i %i %f\n", v, b, w);

						// FIXME: Import to freyja...
						freyjaMeshVertexWeight(0, v, b, w);
					}
				}
			}

		
	}

#endif 

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
