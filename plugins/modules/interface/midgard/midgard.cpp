/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : perlinnoise plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.23:
 * Mongoose - Created, Based on freyja prototype
 ==========================================================================*/

#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <hel/Quat.h>
#include <hel/Mat44.h>
#include <mstl/SystemIO.h>
#include <freyja/ModelABI.h>
#include <freyja/BoneABI.h>
#include <freyja/MeshABI.h>
#include <freyja/PluginABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/FreyjaImage.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>
#include <tinyxml/tinyxml.h>

using namespace mstl;
using namespace mgtk;
using namespace hel;
using namespace freyja;

extern "C" {

	void freyja_midgard_init(void (*func)(const char*, void*));
}

void MidgardEventsAttach();
void MidgardGUIAttach();

void freyja_midgard_init(void (*func)(const char*, void*))
{
	ResourcePlugin *plugin;
	plugin = new ResourcePlugin(MidgardEventsAttach, MidgardGUIAttach);	
}


void eMidgardNop()
{
}


void eMidgardSaveMeshAsXML1s(const char *filename)
{
	// Make xml: <?xml ..><Hello>World</Hello>
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *container = new TiXmlElement("Freyja");
	doc.LinkEndChild(container);

	TiXmlElement *mesh = new TiXmlElement("Mesh");
	container->LinkEndChild(mesh);

	doc.SaveFile(filename);

#if 0
window->SetAttribute("name", "Circle");
window->SetAttribute("x", 5);
window->SetAttribute("y", 15);
window->SetDoubleAttribute("radius", 3.14159);
TiXmlText *text = new TiXmlText("sdfdsa fd fdsf dsdfda sf");
#endif
}


void eMidgardLoadXML1s(const char *filename)
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
			return;
	}

	printf("@ XML loading successful.\n");

	TiXmlElement *root = doc.RootElement(); 

	if (!root) 
	{
		printf("Couldn't find document root!\n");
		return;
	}

	TiXmlElement *model = root;//root->FirstChildElement();//"model");

	if (model)
	{
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
	}
}


void eMidgardLoadXML()
{
	char *path = mgtk_rc_map("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Open Midgard XML...", path, 0,
							  "Midgard Demo Models (*.xml)", "*.xml");

	if (path) 
	{
		delete [] path;
	}

	mgtk_print("! Importing: '%s'\n", filename);
	eMidgardLoadXML1s(filename);
	mgtk_filechooser_blocking_free(filename);
}


inline 
float GetHeight(byte *map, uint32 width, int x, int y)
{
	unsigned int xx = x % width;
	unsigned int yy = y % width;
	return map[xx + (yy * width)];
}


void eMidgardGenerateHeightmap()
{
	byte *map = NULL; 
	uint32 width = 512;
	uint32 step = 16;
	float scale = 0.1f;

	//////////////////////////////////
	char *path = "/home/mongoose/Projects/mage/Data/Maps"; //mgtk_rc_map("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Open Midgard Heightmap...", path, 0,
							  "Midgard heightmap (*.tga)", "*.tga");

	if (path) 
	{
		//delete [] path;
	}

	mgtk_print("! Importing: '%s'\n", filename);
	
	FreyjaImage img;
	img.loadImage(filename);
	width = img.getWidth();
	img.getImage(&map);

	mgtk_filechooser_blocking_free(filename);
	//////////////////////////////////

	if (map == NULL)	
		return;

	bool flip = false;

	index_t mesh = freyjaMeshCreate();

	for (uint32 i = 0; i <= width; i += step)
	{
		// Render alternating strips of the heightmap.
		if (flip)
		{
			for (int32 j = width; j >= 0; j -= step)
			{
				Vec3 p;

				// Bottom left vertex		
				p.mX = i;							
				p.mY = GetHeight(map, width, i, j); //(float)map[(i%width) + ((j%width) * width)];
				p.mZ = j;

				{
					Vec3 t(p.mX, p.mZ, 0.0f);
					t *= 1.0f/width;
					p *= scale;

					index_t idx = freyjaMeshVertexCreate3fv(mesh, p.mVec);
					freyjaMeshVertexTexCoord3fv(mesh, idx, t.mVec);
				}

				// Bottom right vertex		
				p.mX = i + step;							
				p.mY = GetHeight(map, width, i+step, j); //(float)map[((i+step)%width) + ((j%width) * width)];
				p.mZ = j;

				{
					Vec3 t(p.mX, p.mZ, 0.0f);
					t *= 1.0f/width;
					p *= scale;

					index_t idx = freyjaMeshVertexCreate3fv(mesh, p.mVec);
					freyjaMeshVertexTexCoord3fv(mesh, idx, t.mVec);
				}
			}
		}
		else
		{	
			for (uint32 j = 0; j <= width; j += step)
			{
				Vec3 p;
				// Bottom right vertex	
				p.mX = i + step;							
				p.mY = GetHeight(map, width, i+step, j); //(float)map[((i+step)%width) + ((j%width) * width)];
				p.mZ = j;

				{
					Vec3 t(p.mX, p.mZ, 0.0f);
					t *= 1.0f/width;
					p *= scale;

					index_t idx = freyjaMeshVertexCreate3fv(mesh, p.mVec);
					freyjaMeshVertexTexCoord3fv(mesh, idx, t.mVec);
				}


				// Bottom left vertex		
				p.mX = i;							
				p.mY = GetHeight(map, width, i, j); //(float)map[(i%width) + ((j%width) * width)];
				p.mZ = j;

				{
					Vec3 t(p.mX, p.mZ, 0.0f);
					t *= 1.0f/width;
					p *= scale;

					index_t idx = freyjaMeshVertexCreate3fv(mesh, p.mVec);
					freyjaMeshVertexTexCoord3fv(mesh, idx, t.mVec);
				}
			}
		}



		flip = !flip;
	}


	index_t a = 0, b = 1;

	uint32 count = freyjaGetMeshVertexCount(mesh);
	for (uint32 i = 2; i <= count; ++i)
	{
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, mesh); // matId = meshId
		freyjaMeshPolygonAddVertex1i(mesh, face, a);
		freyjaMeshPolygonAddVertex1i(mesh, face, b);
		freyjaMeshPolygonAddVertex1i(mesh, face, i);

		a = b;
		b = i;
	}
}


void eMidgardRemoveRotFromSkel()
{
	Vector<Vec3> transforms;

	uint32 n = freyjaGetBoneCount();
	for (uint32 i = 0; i < n; ++i)
	{
		// Get bone's bind transform pos.
		Vec3 p;
		freyjaBoneBindTransformVertex(i, p.mVec, 1.0f);

		index_t idx = freyjaGetBoneParent(i);

		/*while*/if (idx != INDEX_INVALID)
		{
			Vec3 t;
			freyjaBoneBindTransformVertex(idx, t.mVec, 1.0f);

			p -= t;

			idx = freyjaGetBoneParent(idx);
		}

		transforms.push_back( p );
	}

	Quat q;

	for (uint32 i = 0; i < n; ++i)
	{
		Vec3 p = transforms[i];
		freyjaBoneTranslate3f(i, p.mX, p.mY, p.mZ);
		freyjaBoneRotateQuat4f(i, q.mW, q.mX, q.mY, q.mZ);
	}
}


void MidgardEventsAttach()
{
	ResourceEventCallback::add("eMidgardPluginsMenu", &eMidgardNop);
	ResourceEventCallback::add("eMidgardLoadXML", &eMidgardLoadXML);
	ResourceEventCallback::add("eMidgardGenerateHeightmap", &eMidgardGenerateHeightmap);

	ResourceEventCallback::add("eMidgardRemoveRotFromSkel", &eMidgardRemoveRotFromSkel);
}


void MidgardGUIAttach()
{
#if 0
	char *basename = "plugins/midgard.mlisp";
	char *filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;
#endif

	int mainMenuId = Resource::mInstance->getIntByName("ePluginMenu");
	int pluginMenuId = Resource::mInstance->getIntByName("eMidgardPluginsMenu");
	mgtk_append_menu_to_menu(mainMenuId, "Midgard", pluginMenuId);

	int id = Resource::mInstance->getIntByName("eMidgardLoadXML");
	mgtk_append_item_to_menu(pluginMenuId, "Open XML...", id);

	id = Resource::mInstance->getIntByName("eMidgardGenerateHeightmap");
	mgtk_append_item_to_menu(pluginMenuId, "Open Heightmap...", id);

	id = Resource::mInstance->getIntByName("eMidgardRemoveRotFromSkel");
	mgtk_append_item_to_menu(pluginMenuId, "Purge skel rot", id);
}

