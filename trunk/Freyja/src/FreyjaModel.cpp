/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaModel
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the backend of the modeler
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000-09-09:
 * Mongoose - Created
 ==========================================================================*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <freyja8/EggFileReader.h> 

#include "MaterialManager.h"

#include "FreyjaModel.h"
#include "freyja_events.h"

extern BezierPatch gTestPatch;


FreyjaModel::FreyjaModel()
{
	char *pluginDir;

	appendMode = false;

	_palette_filename = 0;

	pluginDir = freyja_rc_map("plugins/");

	_egg = new FreyjaEgg();
	_plugin = new FreyjaEggPlugin(_egg, pluginDir);

	_plugin->addModule("psk");
	_plugin->addModule("nod");
	_plugin->addModule("mdx");
	_plugin->addModule("grn");
	_plugin->addModule("md5");
	_plugin->addModule("smd");
	_plugin->addModule("obj");
	_plugin->addModule("halflife");

	delete [] pluginDir;

	// Mongoose: initalize|reset private data members
	Clear();
}


FreyjaModel::~FreyjaModel()
{
	if (_plugin)
		delete _plugin;

	if (_egg)
		delete _egg;
}


void FreyjaModel::setFlags(option_flag_t flag, int op)
{
	_defaults |= flag;

	if (!op)
		_defaults ^= flag;

	// Pretty output in freyja console
	if (flag & FL_DUMP_TEXTURE)
		freyja_print("Texture dumping is %s", op ? "on" : "off");

	if (flag & FL_LOAD_MAP)
	{
		freyja_print("TombRaider Map loading is %s", op ? "on" : "off");

		if (_plugin)
			_plugin->eggFlags(FL_EGGP_LOAD_MAP);
	}
}


unsigned int FreyjaModel::getFlags()
{
	return _defaults;
}


void FreyjaModel::Clear()
{
	_cached_vertex = NULL;
	_defaults = 0;
	_poly_sz = 3;
	_texture_num = 0;
	_current_plane = Egg::PLANE_XY;
	_current_texel = 0;
	_current_vertex = 0;
	_current_group = 0;
	_current_mesh = 0;
	_current_tag = 0;
	_current_bone_frame = 0;
	_current_frame = 0;
	_current_animation_frame = 0;
	_current_polygon = 0;
	_bbox = 0;

	_scroll[0] = 0.0f;
	_scroll[1] = -18.0f;
	_scroll[2] = 0.0f;
	
	mSelectBBox[0][0] = mSelectBBox[0][1] = mSelectBBox[0][2] = -16.0;
	mSelectBBox[1][0] = mSelectBBox[1][1] = mSelectBBox[1][2] = 16.0;
	
	_light0_pos[0] = _light0_pos[0] = _light0_pos[0] = 0.0;
	_light0_pos[0] = 0.0;

	if (_egg)
	{
		_egg->clear();
	}
}


void FreyjaModel::printInfo()
{
	freyja_print("%d bones, %d meshes, %d polygons, %d vertices",
				_egg->getTagCount(), 
				_egg->getMeshCount(), 
				_egg->getPolygonCount(), 
				_egg->getVertexCount());
}


void FreyjaModel::TextureShift()
{
	egg_mesh_t *mesh;
	egg_polygon_t *poly;
	unsigned int i;


	mesh = CachedMesh();

	if (!mesh)
	{
		freyja_print("FreyjaModel::TextureShift> ERROR: Invalid current mesh\n");
		return;
	}

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{  
		poly = _egg->getPolygon(mesh->polygon[i]);

		if (!poly)
			continue;

		poly->shader = getCurrentTextureIndex();
	}
}


void FreyjaModel::generateUVMap()
{
	egg_mesh_t *mesh;
	egg_polygon_t *poly;
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	unsigned int i, j;
	float u, v;


	mesh = CachedMesh();

	if (!mesh)
		return;

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{  
		poly = _egg->getPolygon(mesh->polygon[i]);

		if (!poly)
			continue;

		// Generate texels and record their ids
		for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
		{
			vertex = _egg->getVertex(poly->vertex[j]);

			if (!vertex)
				continue;

			// Mongoose 2002.01.18, Generate UV from vertex XYZ
			eggGenerateUVFromXYZ(vertex->pos, &u, &v);

#ifdef DEBUG_GEN_TEXEL
			freyja_print("FreyjaModel::generateUVMap> %f %f\n", u, v);
#endif

			if (poly->texel.empty())
			{
				texel = 0x0;
			}
			else
			{
				texel = _egg->getTexel(j);
			}
			
			if (!texel)
			{
				vertex->uv[0] = u;
				vertex->uv[1] = v;
			}
			else
			{
				texel->st[0] = u;
				texel->st[1] = v;
			}
		}
	}
}

void FreyjaModel::CurrentPlane(Egg::egg_plane p)
{
	_current_plane = p;
}


enum Egg::egg_plane FreyjaModel::CurrentPlane()
{
	return _current_plane;
}

unsigned int FreyjaModel::getCurrentMesh()
{
	return _current_mesh;
}

unsigned int FreyjaModel::getCurrentPolygon()
{
	return _current_polygon;
}

unsigned int FreyjaModel::getCurrentGroup()
{
	return _current_group;
}

unsigned int FreyjaModel::getCurrentSkeleton()
{
	return _current_bone_frame;
}

unsigned int FreyjaModel::getCurrentBone()
{
	return _current_tag;
}

unsigned int FreyjaModel::getCurrentTexCoord()
{
	return _current_texel;
}

unsigned int FreyjaModel::getCurrentVertex()
{
	return _current_vertex;
}

unsigned int FreyjaModel::getCurrentVertexFrame()
{
	printf("FreyjaModel::getCurrentVertexFrame> Not Implemented %s:%d\n",
		   __FILE__, __LINE__);
	return 0;
}

unsigned int FreyjaModel::getCurrentPolygonEdgeCount()
{
	return _poly_sz;
}

unsigned int FreyjaModel::getCurrentTextureIndex()
{
	return _texture_num;
}

unsigned int FreyjaModel::getCurrentAnimationFrame()
{
	return _current_frame;
}

unsigned int FreyjaModel::getCurrentAnimation()
{
	return _current_animation_frame;
}


void FreyjaModel::setCurrentMesh(unsigned int index)
{
	if (index < _egg->getMeshCount())
	{
		egg_mesh_t *mesh = _egg->getMesh(index);
		_current_mesh = index;
		_current_group = 0;
		
		if (mesh)
		{
			_current_group = mesh->group[0];
		}
		
		freyja_print("Mesh[%i]", getCurrentMesh());
	}
}


void FreyjaModel::setCurrentPolygon(unsigned int index)
{
	if (index < _egg->getPolygonCount())
	{
		_current_polygon = index;
	}
}


void FreyjaModel::setCurrentGroup(unsigned int index)
{
	if (index < _egg->getGroupCount())
	{
		_current_group = index;
		freyja_print("Group[%i]", getCurrentGroup());

#ifdef FIXME
		if (0) // this is the old egg way of vertex morph frame anim
		{
			egg_mesh_t *mesh = _egg->getMesh(getCurrentMesh());
			unsigned int i, j;

			for (i = mesh->group.begin(); i < mesh->group.end(); ++i)
			{
				if (mesh->group[i] && mesh->group[i] == getCurrentGroup())
				{
					if (mesh->flags & FL_MESH__VERTEX_FRAME_GROUPS)
					{
						// blah
						
						for (j = mesh->polygon.begin(); j < mesh->polygon.end(); ++i)
						{

						mesh->r_polygon.clear(); // force render recache
					}
				}
			}
			
		}
#endif
	}
}

void FreyjaModel::setCurrentSkeleton(unsigned int index)
{
	if (index < _egg->getBoneFrameCount())
	{
		_current_bone_frame = index;
	}
}


void FreyjaModel::setCurrentBone(unsigned int index)
{
	if (index < _egg->getTagCount())
	{
		_current_tag = index;
		freyja_print("Bone Tag[%i]", getCurrentBone());
	}
}


void FreyjaModel::setCurrentTexCoord(unsigned int index)
{
	if (index < _egg->getTexelCount())
	{
		_current_texel = index;
	}
}


void FreyjaModel::setCurrentVertex(unsigned int index)
{
	if (index < _egg->getVertexCount())
	{
		_current_vertex = index;
	}
}


void FreyjaModel::setCurrentVertexFrame(unsigned int index)
{
}


void FreyjaModel::setCurrentPolygonEdgeCount(unsigned int count)
{
	if (count < 3)
	{
		_poly_sz = 3;
	}
	else if (count > 6)
	{
		_poly_sz = 6;
	}
	else
	{ 
		_poly_sz = count;
	}
}


void FreyjaModel::setCurrentTextureIndex(unsigned int index)
{
	_texture_num = index;
	freyja_print("Texture[%i]", getCurrentTextureIndex());
}

void FreyjaModel::setCurrentAnimation(unsigned int index)
{
	if (index < _egg->getAnimationCount())
	{
		_current_animation_frame = index;
		setCurrentSkeleton(0);
		
		freyja_print("Animation[%i] Frame[%i]", 
					_current_animation_frame, _current_frame);
	}
}


void FreyjaModel::setCurrentAnimationFrame(unsigned int index)
{
	egg_animation_t *animation_frame;


	animation_frame = _egg->getAnimation(getCurrentAnimation());

	if (animation_frame && animation_frame->frame.size())
	{
		if (index >= animation_frame->frame.size())
		{
			_current_frame = 0;  
		}
		else
		{
			_current_frame = index;
		}
		
		_current_bone_frame = animation_frame->frame[_current_frame];
	}
	else
	{
		_current_frame = 0;
		_current_bone_frame = 0;
	}
	
	freyja_print("Animation[%i].frame[%i] = %i", 
				getCurrentAnimation(), getCurrentAnimationFrame(), 
				getCurrentSkeleton());
}


void FreyjaModel::deleteAnimationFrame(unsigned int index)
{
	egg_animation_t *animation;
	unsigned int i;
	int frame_index = -1;

	animation = _egg->getAnimation(getCurrentAnimation());

	if (animation && animation->frame.size() && index < animation->frame.size())
	{
		frame_index = animation->frame[index];

		for (i = animation->frame.begin(); i+1 < animation->frame.end(); ++i)
		{
			if (i >= index)
			{
				animation->frame.assign(i, i+1);
			}
		}

		//FIXME _egg->deleteBoneFrame(frame_index);
	}
}



///////////////////////////////////////////////////////////////////////////////


void FreyjaModel::getBoneTranslation(float *x, float *y, float *z)
{
	egg_tag_t *tag;


	tag = _egg->getTag(getCurrentBone());

	if (tag)
	{
		*x = tag->center[0];
		*y = tag->center[1];
		*z = tag->center[2];
	}
}


void FreyjaModel::setBoneTranslation(float x, float y, float z)
{
	egg_tag_t *tag;


	tag = _egg->getTag(getCurrentBone());

	if (tag)
	{
		tag->center[0] = x;
		tag->center[1] = y;
		tag->center[2] = z;
	}
}


void FreyjaModel::getBoneRotation(float *x, float *y, float *z)
{
	egg_tag_t *tag;


	tag = _egg->getTag(getCurrentBone());

	if (tag)
	{
		*x = tag->rot[0];
		*y = tag->rot[1];
		*z = tag->rot[2];
	}
}


void FreyjaModel::setBoneRotation(float x, float y, float z)
{
	egg_tag_t *tag;


	tag = _egg->getTag(getCurrentBone());

	if (tag)
	{
		tag->rot[0] = x;
		tag->rot[1] = y;
		tag->rot[2] = z;
	}
}


void FreyjaModel::transform(int mode, enum Egg::egg_transform type, 
							float x, float y, float z)
{
	switch (mode)
	{
	case FreyjaModel::TransformVertexFrame:	
		_egg->Transform(CachedGroup(), type, x, y, z);
		break;

	case FreyjaModel::TransformMesh:
		_egg->Transform(CachedMesh(), type, x, y, z);
		break;

	case FreyjaModel::TransformBone:
		if (type != Egg::ROTATE)
			_egg->Transform(CachedTag(), type, x, y, z);
		else
			_egg->TagRotateAbout(getCurrentBone(), x, y, z);
		break;

	case FreyjaModel::TransformScene:
		_egg->Transform(type, x, y, z);
		break;
	}
}


void FreyjaModel::setSceneTranslation(vec_t x, vec_t y, vec_t z)
{
	_scroll[0] = x;
	_scroll[1] = y;
	_scroll[2] = z;
}

void FreyjaModel::adjustSceneTranslation(vec_t x, vec_t y, vec_t z)
{
	_scroll[0] += x;
	_scroll[1] += y;
	_scroll[2] += z;
}

void FreyjaModel::getSceneTranslation(vec3_t scroll)
{
	scroll[0] = _scroll[0];
	scroll[1] = _scroll[1];
	scroll[2] = _scroll[2];
}


//// BONES /////////////////////////////////////////
unsigned int FreyjaModel::newBone(float x, float y, float z, unsigned char flag)
{
	egg_tag_t *tag;
	char name[64];

	tag = _egg->addTag(x, y, z, flag);

	if (tag->id == 0)
		snprintf(name, 64, "root");
	else
		snprintf(name, 64, "bone %i", tag->id);

	setNameBone(tag->id, name);
	updateSkeletalUI();

	return tag->id;
}


void FreyjaModel::addMeshToBone(unsigned int tag, unsigned int mesh)
{
	_egg->TagAddMesh(CachedTag(), getCurrentMesh());
	updateSkeletalUI();
}


void FreyjaModel::removeMeshFromBone(unsigned int tag, unsigned int mesh)
{
	_egg->TagDelMesh(CachedTag(), getCurrentMesh());
	updateSkeletalUI();
}


void FreyjaModel::selectBone(float xx, float yy)
{
	egg_tag_t *bone;

	bone = _egg->getNearestTag(xx, yy, CurrentPlane());

	if (bone)
		setCurrentBone(bone->id);
}

void FreyjaModel::connectBone(unsigned int master, unsigned int slave)
{
	_egg->connectTag(master, slave);
	updateSkeletalUI();
}


void FreyjaModel::VertexBuffer(float xx, float yy)
{
	VertexSelect(xx, yy);

	if (!_cached_vertex)
	{
		freyja_print("Control> refused to use bad vertex in list");
		mList.clear();
	}
	else
	{
		mList.pushBack(_cached_vertex->id);
	}
}


void FreyjaModel::PolygonSelectVertex(float xx, float yy)
{
	static Vector<unsigned int> vertices;


	VertexSelect(xx, yy);

	if (!_cached_vertex)
	{
		vertices.clear();
		freyja_print("Control> Bad vertex, not selecting polygon");
	}
	else
	{
		vertices.pushBack(_cached_vertex->id);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		freyja_print("Control> Polygon selected");

		_current_polygon = _egg->selectPolygon(&vertices);
		vertices.clear();
	}
}


void FreyjaModel::PolygonDelVertex(float xx, float yy)
{
	static Vector<unsigned int> vertices;

	VertexSelect(xx, yy);

	if (!_cached_vertex)
	{
		vertices.clear();
		freyja_print("Control> Bad vertex, not removing polygon");
	}
	else
	{
		vertices.pushBack(_cached_vertex->id);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		freyja_print("Control> Polygon removed");

		_egg->delPolygon(&vertices);
		vertices.clear();
	}
}


void FreyjaModel::PolygonAddVertex(float xx, float yy)
{
	static Vector<unsigned int> vertices;
	static Vector<unsigned int> texels;
	egg_mesh_t *mesh = CachedMesh();
	egg_vertex_t *vertex;
	egg_polygon_t *polygon;
	unsigned int i, polygonId;


	VertexSelect(xx, yy);
    
	if (!_cached_vertex)
	{
		vertices.clear();
		freyja_print("Control> Bad vertex, not building polygon");
	}
	else
	{
		vertices.pushBack(_cached_vertex->id);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		freyja_print("Control> New Polygon made");

		// Generate texels and record their ids
		for (i = vertices.begin(); i < vertices.end(); ++i)
		{
			vertex = _egg->getVertex(vertices[i]);

			if (vertex)
			{
				// Mongoose 2002.01.18, Generate UV from vertex XYZ
				eggGenerateUVFromXYZ(vertex->pos, &vertex->uv[0], &vertex->uv[1]);
			}
			else
			{
				texels.pushBack(_egg->addTexel(0.5, 0.5));
			}
		}

		polygonId = _egg->addPolygon(vertices, texels, 0);

		if (polygonId == UINT_MAX)
		{
			printf("FIXME: %s:%i\n", __FILE__, __LINE__);
			return;
		}

		polygon = _egg->getPolygon(polygonId);
		mesh->polygon.pushBack(polygon->id);
		mesh->r_polygon.pushBack(polygon);

		texels.clear();
		vertices.clear();
	}
}


bool FreyjaModel::pasteSelectedPatch()
{
	Vector<unsigned int> transV;
	unsigned int index, i, n;


	gTestPatch.generatePolygonMesh(7);

	eggBegin(FREYJA_MESH);
	eggMeshFlags1u(0x0);

	eggBegin(FREYJA_GROUP);
	eggGroupCenter3f(0, 0, 0);

	for (i = gTestPatch.vertices.begin(); i < gTestPatch.vertices.end(); ++i)
	{
		index = eggVertexStore3f(gTestPatch.vertices[i]->mVec[0],
								 gTestPatch.vertices[i]->mVec[1],
								 gTestPatch.vertices[i]->mVec[2]);
		eggVertexNormalStore3f(index, 0, 1, 0); // FIXME
		eggVertexUVStore2f(index, gTestPatch.texcoords[i]->mVec[0],
						   gTestPatch.texcoords[i]->mVec[1]);
		transV.pushBack(index);
	}

	eggEnd(); // FREYJA_GROUP

	n = gTestPatch.vertices.end() - 2;
	for (i = gTestPatch.vertices.begin(); i < n; ++i)
	{
		eggBegin(FREYJA_POLYGON);
		eggTexture1i(gTestPatch.texture);
		eggVertex1i(transV[i]);
		eggVertex1i(transV[i+1]);
		eggVertex1i(transV[i+2]);
		eggEnd(); // FREYJA_POLYGON
	}

	eggEnd(); // FREYJA_MESH

	freyja_print("Mesh generated from patch...");

	return true;
}


bool FreyjaModel::pasteSelectedMesh()
{
	Vector<unsigned int> transV;
	Vector<unsigned int> transT;
	CopyVertex *v;
	CopyTexCoord *t;
	CopyPolygon *p;
	unsigned int i, j, index;


	if (mCopyMesh.vertices.empty() || mCopyMesh.polygons.empty())
		return false;

	eggBegin(FREYJA_MESH);
	eggMeshFlags1u(mCopyMesh.flags);

	eggBegin(FREYJA_GROUP);
	eggGroupCenter3f(mCopyMesh.center[0], 
					 mCopyMesh.center[1],
					 mCopyMesh.center[2]);

	for (i = mCopyMesh.vertices.begin(); i < mCopyMesh.vertices.end(); ++i)
	{
		v = mCopyMesh.vertices[i];
		//index = 0;

		if (v)
		{
			index = eggVertexStore3f(v->pos[0], v->pos[1], v->pos[2]);
			eggVertexNormalStore3f(index, v->norm[0], v->norm[1], v->norm[2]);
			eggVertexUVStore2f(index, v->uv[0], v->uv[1]);
		}

		transV.pushBack(index);
	}

	eggEnd(); // FREYJA_GROUP

	for (i = mCopyMesh.texcoords.begin(); i < mCopyMesh.texcoords.end(); ++i)
	{
		t = mCopyMesh.texcoords[i];
		//index = 0;

		if (t)
		{
			index = eggTexCoordStore2f(t->uv[0], t->uv[1]);
		}

		transT.pushBack(index);
	}

	for (i = mCopyMesh.polygons.begin(); i < mCopyMesh.polygons.end(); ++i)
	{
		p = mCopyMesh.polygons[i];

		if (!p)
			continue;

		eggBegin(FREYJA_POLYGON);

		eggTexture1i(p->material);

		for (j = p->vertices.begin(); j < p->vertices.end(); ++j)
			eggVertex1i(transV[p->vertices[j]]);

		for (j = p->texcoords.begin(); j < p->texcoords.end(); ++j)
			eggTexCoord1i(transT[p->texcoords[j]]);

		eggEnd(); // FREYJA_POLYGON
	}
	

	eggEnd(); // FREYJA_MESH

	freyja_print("Mesh pasted...");

	return true;
}


bool FreyjaModel::copySelectedMesh()
{
	CopyVertex *v;
	CopyTexCoord *t;
	CopyPolygon *p;
	egg_mesh_t *mesh; 
	egg_group_t *group;
	egg_polygon_t *polygon;
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	unsigned int i, j, index;


	mesh = CachedMesh();
	group = CachedGroup();

	if (!mesh || !group)
		return false;

	// FIXME: You can add a back buffer stack/list here
	if (!appendMode)
		mCopyMesh.erase();

	mCopyMesh.flags = mesh->flags;
	mCopyMesh.center[0] = group->center[0];
	mCopyMesh.center[1] = group->center[1];
	mCopyMesh.center[2] = group->center[2];

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{
		index = mesh->polygon[i];
		polygon = _egg->getPolygon(index);

		if (!polygon)
			continue;

		p = new CopyPolygon();
		p->material = polygon->shader;

		for (j = polygon->vertex.begin(); j < polygon->vertex.end(); ++j)
		{
			index = polygon->vertex[j];
			vertex = _egg->getVertex(index);

			v = new CopyVertex();

			if (vertex)  // This is to handle a whacky uv alignment issue
			{
				v->pos[0] = vertex->pos[0];
				v->pos[1] = vertex->pos[1];
				v->pos[2] = vertex->pos[2];
				v->uv[0] = vertex->uv[0];
				v->uv[1] = vertex->uv[1];
				v->norm[0] = vertex->norm[0];
				v->norm[1] = vertex->norm[1];
				v->norm[2] = vertex->norm[2];
			}
			else
			{
				freyja_print("Selected mesh to copy has invalid vertex %i", 
							 index);
			}

			//FIXME: Add weight copy here, but this build handles
			//       weights so poorly no need until they're fixed

			mCopyMesh.vertices.pushBack(v);
			p->vertices.pushBack(mCopyMesh.vertices.end()-1);
		}

		for (j = polygon->texel.begin(); j < polygon->texel.end(); ++j)
		{
			index = polygon->texel[j];
			texel = _egg->getTexel(index);

			t = new CopyTexCoord();
			
			if (texel)
			{
				t->uv[0] = texel->st[0];
				t->uv[1] = texel->st[1];
			}
			else
			{
				freyja_print("Selected mesh to copy has invalid texcoord %i", 
							 index);
			}

			mCopyMesh.texcoords.pushBack(t);
			p->texcoords.pushBack(mCopyMesh.texcoords.end()-1);	
		}

		mCopyMesh.polygons.pushBack(p);
	}	


	/* FIXME: Drop Animation frames for now ( duping them could waste memory,
	   and isn't likely to be expected by user ) */
	for (i = mesh->group.begin(); i < mesh->group.end(); ++i)
	{
		// FIXME: Remember if this is implemented, you have to skip the
		//        current group and reorder since its vertices are duped
	}	

	freyja_print("Mesh[%i] copied...", mesh->id);

	return true;
}


void FreyjaModel::MeshCopy()
{
	egg_mesh_t *mesh = CachedMesh();
	egg_group_t *grp = CachedGroup();
	bool whole = false;


	// Mongoose 2002.07.07, If empty copy the whole thing
	if (mList.empty() && grp)
	{
		mList.copy(grp->vertex);
		whole = true;
	}

	if (mesh && !mList.empty())
	{
		freyja_print("New %s mesh cloned", whole ? "whole" : "partial");
		_egg->MeshCopy(mesh, &mList);
		mList.clear();
	}
}


void FreyjaModel::MeshDel()
{
	freyja_print("Mesh[%u] deleted\n", _current_mesh);
	_egg->delMesh(_current_mesh);
}


void FreyjaModel::setNameBone(unsigned int bone, const char *name)
{
	egg_tag_t *boneA = _egg->getTag(bone);

	if (getFlags() & fDontUpdateBoneName)
		return;

	if (boneA && boneA->id == (int)bone && name && name[1])
	{
		if (strncmp(boneA->name, name, 64) != 0)
		{
			strncpy(boneA->name, name, 64);
			freyja_print("bone[%i].name = '%s'", bone, name);
			updateSkeletalUI();
		}
	}
}

const char *FreyjaModel::getNameBone(unsigned int bone)
{
	egg_tag_t *boneA = _egg->getTag(bone);

	if (boneA)
	{
		return boneA->name;
	}

	return 0x0;
}


void FreyjaModel::disconnectBone(unsigned int master, unsigned int slave)
{
	_egg->TagDisconnect(master, slave);
	updateSkeletalUI();
}


void addVertexToBone(unsigned int bone, unsigned int vertex)
{
	eggVertexWeightStore(vertex, 1.0f, bone);
}


void FreyjaModel::removeVertexFromBone(unsigned int bone, unsigned int vertex)
{
	egg_vertex_t *vert = _egg->getVertex(vertex);
	egg_weight_t *weight;
	unsigned int i;


	if (vert)
	{
		for (i = vert->weights.begin(); i < vert->weights.end(); ++i)
		{
			weight = vert->weights[i];

			if (weight && weight->bone == bone)
			{
				delete vert->weights[i];  /* Safe to null this out?  */
			}
		}
	}
}


void FreyjaModel::moveBoneCenter(float xx, float yy)
{
	egg_tag_t *bone;


	if (!(bone = CachedTag()))
		return;
  
	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		bone->center[0] = xx;
		bone->center[1] = yy;
		break;
	case Egg::PLANE_XZ:
		bone->center[0] = xx;
		bone->center[2] = yy;
		break;
	case Egg::PLANE_YZ:
		bone->center[1] = xx;
		bone->center[2] = yy;
		break;
	}
}



void FreyjaModel::moveBone(float xx, float yy)
{
	vec_t x = 0, y = 0, z = 0;
	egg_tag_t *bone;


	if (!(bone = CachedTag()))
		return;

	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		x = xx - bone->center[0];
		y = yy - bone->center[1];
		z = 0;
		break;
	case Egg::PLANE_XZ:
		x = xx - bone->center[0];
		y = 0;
		z = yy - bone->center[2];
		break;
	case Egg::PLANE_YZ:
		x = 0;
		y = xx - bone->center[1];
		z = yy - bone->center[2];
		break;
	}
  
	transform(FreyjaModel::TransformBone, Egg::TRANSLATE, x, y, z);
}


void FreyjaModel::MeshMove(float xx, float yy)
{
	vec_t x = 0, y = 0, z = 0;
	egg_group_t *mesh = CachedGroup();


	if (!mesh)
		return;
	
	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		x = xx - mesh->center[0];
		y = yy - mesh->center[1];
		z = 0;
		break;
	case Egg::PLANE_XZ:
		x = xx - mesh->center[0];
		y = 0;
		z = yy - mesh->center[2];
		break;
	case Egg::PLANE_YZ:
		x = 0;
		y = xx - mesh->center[1];
		z = yy - mesh->center[2];
		break;
	}

	transform(FreyjaModel::TransformMesh, Egg::TRANSLATE, x, y, z);  
}


void FreyjaModel::getCurrentMeshCenter(vec3_t center)
{
	egg_group_t *mesh = CachedGroup();


	if (!mesh)
		return;
	
	center[0] = mesh->center[0];
	center[1] = mesh->center[1];
	center[2] = mesh->center[2];
}


void FreyjaModel::MeshMoveCenter(float xx, float yy)
{
	egg_group_t *frame;


	if (!(frame = CachedGroup()))
		return;

	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		frame->center[0] = xx;
		frame->center[1] = yy;
		break;
	case Egg::PLANE_XZ:
		frame->center[0] = xx;
		frame->center[2] = yy;
		break;
	case Egg::PLANE_YZ:
		frame->center[1] = xx;
		frame->center[2] = yy;
		break;
	}
}


void FreyjaModel::VertexNew(float xx, float yy)
{
	egg_group_t *frame;


	if (!(frame = CachedGroup()))
	{
		freyja_print("FreyjaModel::VertexNew> Point requires Mesh; Try Mesh->New");
		return;
	}

	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		_cached_vertex = _egg->addVertex(xx, yy, 0.0);
    
		if (_cached_vertex)
			frame->vertex.add(_cached_vertex->id);
		break;
	case Egg::PLANE_XZ:
		_cached_vertex = _egg->addVertex(xx, 0.0, yy);
    
		if (_cached_vertex)
			frame->vertex.add(_cached_vertex->id);
		break;
	case Egg::PLANE_YZ:
		_cached_vertex = _egg->addVertex(0.0, xx, yy);
    
		if (_cached_vertex)
			frame->vertex.add(_cached_vertex->id);
		break;
	}

	if (_cached_vertex)
	{
		if (_defaults & FL_VERTEX_UV)
		{
			eggGenerateUVFromXYZ(_cached_vertex->pos, 
						  &_cached_vertex->uv[0], &_cached_vertex->uv[1]);
			//vertex_uv_gen(_cached_vertex, &u, &v);
			//_cached_vertex->texel = _egg->TexelAdd(u, v);
		}
    
		freyja_print("Vertex[%i] (%.3f %.3f %.3f), %i, %i", 
					_cached_vertex->id,
					_cached_vertex->pos[0],
					_cached_vertex->pos[1], 
					_cached_vertex->pos[2],
					-1,
					-1);
	}
}


void FreyjaModel::VertexCombine(float xx, float yy)
{
	static egg_vertex_t *v0 = NULL;

  
	VertexSelect(xx, yy);

	if (!_cached_vertex)
	{
		return;
	}

	if (!v0)
	{
		v0 = _cached_vertex;
		freyja_print("VertexCombine(%i, ?)", _cached_vertex->id);
	}
	else if (_cached_vertex != v0)
	{
		freyja_print("VertexCombine(%i, %i)", v0->id, _cached_vertex->id);
		_egg->combineVertices(v0->id, _cached_vertex->id);

		_cached_vertex = v0;
		v0 = NULL;
	}
}


void FreyjaModel::selectPatchControlPoint(float xx, float yy)
{
	unsigned int besti = 0, bestj = 0, i, j, x, y;
	Vector3d u, v;
	vec_t dist, closest = 99999;


	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		x = 0;
		y = 1;
		break;

	case Egg::PLANE_XZ:
		x = 0;
		y = 2;
		break;

	case Egg::PLANE_YZ:
		x = 1;
		y = 2;
		break;
	}

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			v = Vector3d(xx, yy, 0);
			u = Vector3d(gTestPatch.control[i][j].mVec[x], 
						 gTestPatch.control[i][j].mVec[y], 0);
			dist = helDist3v(v.mVec, u.mVec);
			
			if (dist < closest)
			{
				closest = dist;

				besti = i;
				bestj = j;
			}
		}
	}

	gTestPatch.x = besti;
	gTestPatch.y = bestj;
}


void FreyjaModel::movePatchControlPoint(float xx, float yy)
{
	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[0] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[1] = yy;
		break;

	case Egg::PLANE_XZ:
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[0] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[2] = yy;
		break;

	case Egg::PLANE_YZ:
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[1] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[2] = yy;
		break;
	}
}


void FreyjaModel::VertexMove(float xx, float yy)
{
	if (!_cached_vertex)
	{
		freyja_print("FreyjaModel::VertexMove> No vertex to move!\n");
		return;
	}

	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		_cached_vertex->pos[0] = xx;
		_cached_vertex->pos[1] = yy;      
		break;
	case Egg::PLANE_XZ:
		_cached_vertex->pos[0] = xx;
		_cached_vertex->pos[2] = yy;
		break;
	case Egg::PLANE_YZ:
		_cached_vertex->pos[1] = xx;               
		_cached_vertex->pos[2] = yy;
		break;
	}
}


void FreyjaModel::VertexSelect(float xx, float yy)
{
	egg_group_t *frame;


	if (!(frame = CachedGroup()))
		return;

	_cached_vertex = _egg->getNearestVertex(frame, xx, yy, CurrentPlane());
}


void FreyjaModel::Bbox(vec3_t min, vec3_t max, Vector<unsigned int> **list)
{
	min[0] = mSelectBBox[0][0];
	min[1] = mSelectBBox[0][1];
	min[2] = mSelectBBox[0][2];
  
	max[0] = mSelectBBox[1][0];
	max[1] = mSelectBBox[1][1];
	max[2] = mSelectBBox[1][2];
  
	*list = &_selection_list;
}


void FreyjaModel::BBoxSelect(float xx, float yy)
{
	int x, y;
	float dist0, dist1;


	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		x = 0; 
		y = 1;
		break;
	case Egg::PLANE_XZ:
		x = 0; 
		y = 2;
		break;
	default:
		//case Egg::PLANE_YZ:
		x = 1; 
		y = 2;
		break;
	}

	dist0 = helDist3v((Vector3d(xx, yy, 0)).mVec,
					  (Vector3d(mSelectBBox[0][x], mSelectBBox[0][y], 0)).mVec);
	dist1 = helDist3v((Vector3d(xx, yy, 0)).mVec,
					  (Vector3d(mSelectBBox[1][x], mSelectBBox[1][y], 0)).mVec);

	if (dist0 < dist1)
		_bbox = 0;
	else
		_bbox = 1;
  
	//freyja_print("Vertex bbox select is %i", _bbox);
}


void FreyjaModel::BBoxMove(float xx, float yy)
{
	float swap;


	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		mSelectBBox[_bbox][0] = xx;
		mSelectBBox[_bbox][1] = yy;
		break;
	case Egg::PLANE_XZ:
		mSelectBBox[_bbox][0] = xx;
		mSelectBBox[_bbox][2] = yy;
		break;
	case Egg::PLANE_YZ:
		mSelectBBox[_bbox][1] = xx;               
		mSelectBBox[_bbox][2] = yy;
		break;
	}

	// Keep min and max!
	if (mSelectBBox[1][0] < mSelectBBox[0][0] ||
		mSelectBBox[0][0] > mSelectBBox[1][0])
	{
		swap = mSelectBBox[0][0];
		mSelectBBox[0][0] = mSelectBBox[1][0];
		mSelectBBox[1][0] = swap;
	}

	if (mSelectBBox[1][1] < mSelectBBox[0][1] ||
		mSelectBBox[0][1] > mSelectBBox[1][1])
	{
		swap = mSelectBBox[0][1];
		mSelectBBox[0][1] = mSelectBBox[1][1];
		mSelectBBox[1][1] = swap;
	}

	if (mSelectBBox[1][2] < mSelectBBox[0][2] ||
		mSelectBBox[0][2] > mSelectBBox[1][2])
	{
		swap = mSelectBBox[0][2];
		mSelectBBox[0][2] = mSelectBBox[1][2];
		mSelectBBox[1][2] = swap;
	}


	//freyja_print("Vertex bbox select is %i @ %f,%f", _bbox, xx, yy);
}	

void FreyjaModel::BBoxListBuild()
{
	Vector<unsigned int> *list;
	egg_group_t *grp;


	mList.clear();

	grp = CachedGroup();

	if (!grp)
		return;

	list = eggFindVerticesInBox(mSelectBBox, grp->vertex);
	mList = *list;
	//	mList.print(__print_unsigned_int);
}


void FreyjaModel::MeshNew()
{
	egg_mesh_t *mesh;
	egg_group_t *grp;

	mesh = _egg->newMesh();
	_egg->addMesh(mesh);
  
	grp = _egg->newGroup();
	_egg->addGroup(grp);
  
	mesh->group.pushBack(grp->id);

	freyja_print("Mesh[%i] made with group %i", mesh->id, grp->id);
}


void FreyjaModel::MeshSelect(float xx, float yy)
{
	egg_mesh_t *mesh;
	Vector<egg_mesh_t *> *meshlst;
	egg_group_t *frame;
	unsigned int i;

	
	frame = _egg->getNearestGroup(xx, yy, CurrentPlane());

	meshlst = _egg->MeshList();

	if (!meshlst || !frame)
	{
		freyja_print("MeshSelect assertion failed #1 %p %p\n", frame, meshlst);
		return;
	}

	for (i = meshlst->begin(); i < meshlst->end(); ++i)
	{
		mesh = (*meshlst)[i];
    
		if (mesh && mesh->group.SearchIndex(frame->id) != UINT_MAX)
		{
			freyja_print("Selected mesh %u, group %u\n", mesh->id, frame->id);
			setCurrentGroup(frame->id);
			setCurrentMesh(mesh->id);
			return;
		}
	}
}


void FreyjaModel::VertexDelete()
{
	if (_cached_vertex)
	{
		// Don't really need to pass id
		_egg->delVertex(_cached_vertex->id);  
		_cached_vertex = NULL;
	}
}


void FreyjaModel::TexelCombine(float s, float t)
{
	static int a = -1;


	TexelSelect(s, t);

	if (a == -1)
	{
		a = _current_texel;
		freyja_print("TexelCombine(%i, ?)", a);
	}
	else if ((int)_current_texel != a)
	{
		freyja_print("TexelCombine(%i, %i)", a, _current_texel);
		_egg->combineTexels(a, _current_texel);
		a = -1;
	}
}


void FreyjaModel::TexelSelect(float s, float t)
{
	egg_mesh_t *mesh = CachedMesh();
	egg_polygon_t *polygon = NULL;
	egg_polygon_t *best_polygon = NULL;
	egg_texel_t *texel = NULL;
	egg_texel_t *best_texel = NULL;
	egg_vertex_t *best_vertex = NULL;
	egg_vertex_t *vertex = NULL;
	vec_t dist = 0.0;
	vec_t best_dist = 0.0;
	unsigned int i, j;


#ifdef DEBUG_TEXEL
	printf("s = %f, t = %f\n", s, t);
#endif

	// Mongoose: Removed 5 layers of scoping from old code ( loops, ew )
	if (!mesh || mesh->polygon.empty())
	{
		freyja_print("No polygons found! Mesh %i [%p]", getCurrentMesh(), mesh);
		return;
	}

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{
		polygon = _egg->getPolygon(mesh->polygon[i]);

		if (!polygon)
			continue;

		// FIXME: Replace with call to shader for texture id
		//        once shaders are put in engine
		if (polygon->shader < 0 || 
			polygon->shader != (int)getCurrentTextureIndex())
			continue;

		for (j = polygon->texel.begin(); j < polygon->texel.end(); ++j)
		{
			texel = _egg->getTexel(polygon->texel[j]);
   
			if (!texel)
				continue;

			// Mongoose: slight optimization, to avoid init branch  =)
			dist = helDist3v((Vector3d(s, t, 0)).mVec,
							 (Vector3d(texel->st[0], texel->st[1], 0)).mVec);

			if (dist < best_dist)
				best_texel = NULL;
		
			if (best_texel == NULL)
			{ 
				best_polygon = polygon;
				best_texel = texel;
				best_dist = dist;

#ifdef DEBUG_TEXEL
				printf("New best_texel = %u @ %f\n", _current_texel, dist);
#endif
			}
		}

		if (polygon->texel.empty())
		{
			for (j = polygon->vertex.begin(); j < polygon->vertex.end(); ++j)
			{
				vertex = _egg->getVertex(polygon->vertex[j]);
				
				if (!vertex)
					continue;
				
				// Mongoose: slight optimization, to avoid init branch  =)
				dist = helDist3v((Vector3d(s, t, 0)).mVec, 
								 (Vector3d(vertex->uv[0], vertex->uv[1], 0)).mVec);
				
				if (dist < best_dist)
					best_vertex = NULL;
				
				if (best_vertex == NULL)
				{ 
					best_polygon = polygon;
					best_vertex = vertex;
					best_dist = dist;
					
#ifdef DEBUG_TEXEL
					printf("New best_texel = %u @ %f\n", _current_vertex, dist);
#endif
				}
			}
		}
	}
      
	// Mongoose: now we set the new _current_texel

	if (best_vertex)
	{
		_current_polygon = best_polygon->id;
		_current_vertex = best_vertex->id;
		return;
	}

	if (best_texel)
	{
		_current_polygon = best_polygon->id;
		_current_texel = best_texel->id;
	}
}


/* Mongoose 2004.04.19, 
 * This is like one of the first methods to die when v9 goes public */
void FreyjaModel::TexelMove(float s, float t)
{
	egg_texel_t *texel = _egg->getTexel(getCurrentTexCoord());
  

	if (texel)
	{
		texel->st[0] = s;
		texel->st[1] = t;
	}
	else
	{
		egg_vertex_t *vertex = _egg->getVertex(getCurrentVertex());
		
		if (vertex)
		{
			vertex->uv[0] = s;
			vertex->uv[1] = t;
		}
	}
}

// FIXME
#include <mgtk/mgtk_events.h>
mgtk_tree_t *generateSkeletalUI(Egg *egg, egg_tag_t *tag, 
								mgtk_tree_t *bone)
{
	egg_tag_t *tagChild;
	unsigned int i, tagIndex, count;


	if (!tag)
	{
		freyja_print("!ERROR: NULL skeletal bone_tag!\n");
		return 0x0;
	}

	if (bone == 0x0)
	{
		bone = new mgtk_tree_t;
		snprintf(bone->label, 64, "root");	
		bone->parent = 0x0;
	}
	else
	{
		snprintf(bone->label, 64, "bone%03i", tag->id);
	}

	if (tag->name[0])
	{
		snprintf(bone->label, 64, "%s", tag->name);
	}

	bone->id = tag->id;
	bone->numChildren = tag->slave.size();
	bone->children = 0x0;

#ifdef DEBUG_BONE_LOAD
	printf("-- %s : %i/%i children\n", 
		   bone->label, bone->numChildren, tag->slave.size());
#endif

	if (bone->numChildren == 0)
		return bone->parent;

	bone->children = new mgtk_tree_t[bone->numChildren+1];

	for (count = 0, i = tag->slave.begin(); i < tag->slave.end(); ++i)
	{
		tagIndex = tag->slave[i];
		tagChild = egg->getTag(tagIndex);

		if (tagChild)
		{
			bone->children[count].parent = bone;

#ifdef DEBUG_BONE_LOAD
			printf("   -- parent %i <-- child %i\n", bone->id, tagIndex);
#endif
			generateSkeletalUI(egg, tagChild, &bone->children[count++]);
		}
	}

	return (bone->parent) ? bone->parent : bone;
}


void testBone(mgtk_tree_t *bone, unsigned int space)
{
	unsigned int i;

	for (i = 0; i < space; ++i)
	{
		freyja_print(" ");
	}

	freyja_print("+ T_BONE %d\n", bone->id);

	for (i = 0; i < bone->numChildren; ++i)
	{
		testBone(&bone->children[i], space+1);
	}
}

void updateSkeletonUI(Egg *egg)
{
	mgtk_tree_t *bone;
	unsigned int i, j;

	bone = generateSkeletalUI(egg, egg->getTag(0), 0x0);

	for (i = 0; i < egg->getTagCount(); ++i)
	{
#ifdef DEBUG_BONE_LOAD
		freyja_print("TAG %d :: ", i);
#endif
		egg_tag_t *tag = egg->getTag(i);

		for (j = tag->slave.begin(); j < tag->slave.end(); ++j)
		{
#ifdef DEBUG_BONE_LOAD
			freyja_print("%d ", tag->slave[j]);
#endif
		}
#ifdef DEBUG_BONE_LOAD
		freyja_print("\n");
#endif
	}

	// testBone(bone, 0);
	extern void mgtk_event_update_tree(unsigned int id, mgtk_tree_t *tree);
	extern int gSkelTreeWidgetIndex;
	mgtk_event_update_tree(gSkelTreeWidgetIndex, bone);
}


void fixTexCoordsHACK(Egg *egg)
{
	unsigned int i;
	egg_texel_t *t;
	egg_vertex_t *v;


	for (i = 0; i < egg->getTexelCount(); ++i)
	{
		t = egg->getTexel(i);

		if (t)
		{
			if (t->st[0] < 0.0f)
				t->st[0] = 0.0f;
			else if (t->st[0] > 1.0f)
				t->st[0] = 1.0f;

			if (t->st[1] < 0.0f)
				t->st[1] = 0.0f;
			else if (t->st[1] > 1.0f)
				t->st[1] = 1.0f;
		}
	}

	for (i = 0; i < egg->getVertexCount(); ++i)
	{
		v = egg->getVertex(i);

		if (v)
		{
			if (v->uv[0] < 0.0f)
				v->uv[0] = 0.0f;
			else if (v->uv[0] > 1.0f)
				v->uv[0] = 1.0f;

			if (v->uv[1] < 0.0f)
				v->uv[1] = 0.0f;
			else if (v->uv[1] > 1.0f)
				v->uv[1] = 1.0f;
		}
	}
}


void FreyjaModel::updateSkeletalUI()
{
	updateSkeletonUI(_egg);
}


int FreyjaModel::loadModel(const char *filename)
{
	int err;


	err = _plugin->importModel((char*)filename);  // try plug-in modules last

	updateSkeletonUI(_egg);		
 
	if (err)
	{
		freyja_print("ERROR: File '%s' not found or unknown format\n", filename);
		freyja_print("ERROR CODE %i\n", err);
	}
	else
	{
		fixTexCoordsHACK(_egg);

		unsigned int i, w, h, bpp, type;
		unsigned char *image = 0x0;
		char *textureFilename = 0x0; /* Texture filename */

		/* Texture image was stored as external file from model */
		for (i = 0; !eggGetTextureFilename(i, &textureFilename); ++i)
		{
			if (textureFilename && textureFilename[0])
			{
				freyja_print("FrejaModel::loadModel> Loading texture %i from file\n", i);
				gMaterialManager->mTextureId = i;
				//gMaterialManager->setGeneralFlag(MaterialManager::fLoadTextureInSlot);
				gMaterialManager->loadTexture(textureFilename);
				//gMaterialManager->clearGeneralFlag(MaterialManager::fLoadTextureInSlot);
			}
		}
	 
		/* Texture image was stored as raw buffer */
		for (i = 0; !eggGetTextureImage(i, &w, &h, &bpp, &type, &image); ++i)
		{
			if (!image)
				continue;

			switch (type)
			{
			case EGG_COLORMODE_RGBA:
				gMaterialManager->loadTextureBuffer(image, w, h, 32, 
													Texture::RGBA);
				break;
			case EGG_COLORMODE_RGB:
				gMaterialManager->loadTextureBuffer(image, w, h, 24,
													Texture::RGB);
				break;
			case EGG_COLORMODE_INDEXED:
				gMaterialManager->loadTextureBuffer(image, w, h, 8, 
													Texture::INDEXED);
				break;
			default:
				freyja_print("%s> ERROR: Unsupported texture colormap %d",
							"FreyjaModel::loadModel", type);
			}
		}

		//freyja_print("FreyjaModel::loadModel> Loaded %s, %i polys, %i verts\n", 
		//	   filename, _egg->getPolygonCount(), _egg->getVertexCount());
	}

	return err;
}


int FreyjaModel::saveModel(const char *filename)
{
	unsigned int l, s, i;
	int ret;
	char ext[32];


  
	if (!filename)
		return -1000;

	l = strlen(filename);
  
	for (s = l; s > 0; s--)
	{
		if (filename[s] == '.')
			break;
	}

	if (s == 0 || (l - s) > 30)
		return -100;

	s++;

	memset(ext, 0, 32);

	for (i = 0; s < l; s++, i++)
	{
		ext[i] = filename[s];
	}

	ret = _plugin->exportModel((char *)filename, ext);

	if (ret)
	{
		freyja_print("Unknown file export extention: '%s', try using '.egg'", 
					ext);
		return ret;
	}
  
	return 0;
}


int FreyjaModel::saveAnimation(const char *filename)
{
	unsigned int l, s, i;
	int ret;
	char ext[32];


  
	if (!filename)
		return -1000;

	l = strlen(filename);
  
	for (s = l; s > 0; s--)
	{
		if (filename[s] == '.')
			break;
	}

	if (s == 0 || (l - s) > 30)
		return -100;

	s++;

	memset(ext, 0, 32);

	for (i = 0; s < l; s++, i++)
	{
		ext[i] = filename[s];
	}

	ret = _plugin->exportModel((char *)filename, "smd"); // ext);

	if (ret)
	{
		freyja_print("Unknown file export extention: '%s', try using '.smd'", 
					ext);
		return ret;
	}
  
	return 0;
}


bool FreyjaModel::isCurrentBoneAllocated()
{
	return (_egg->getTag(getCurrentBone()) != 0x0);
}



/////// PRIVATE METHODS //////////////////////////////////////


egg_group_t *FreyjaModel::CachedGroup()
{
	return _egg->getGroup(getCurrentGroup());    
}

egg_tag_t *FreyjaModel::CachedTag()
{
	return _egg->getTag(getCurrentBone());
}


egg_mesh_t *FreyjaModel::CachedMesh()
{
	return _egg->getMesh(getCurrentMesh());
}

