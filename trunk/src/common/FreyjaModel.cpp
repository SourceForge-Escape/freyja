/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
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
 =================================================================*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "MaterialManager.h"

#include "FreyjaModel.h"
#include "FreyjaResource.h"
#include "freyja_events.h"
#include "utils.h"

//#define WATCH_FREYJAMODEL_EVENTS



FreyjaModel::FreyjaModel()
{
	char *pluginDir;

	_edit_mode = FreyjaModel::TransformMesh;
	_palette_filename = 0;

	pluginDir = freyja_rc_map("plugins/");

	_egg = new Egg();
	_plugin = new EggPlugin(_egg, pluginDir);

	_plugin->addModule("psk");

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


/* Mongoose 2004.03.24, 
 * FIXME This sucks, period 
 * Replacing it will be an ordeal but it needs to be 
 * done when everything else is refactored first */
int FreyjaModel::handleEvent(int mode, int cmd)
{
#ifdef WATCH_FREYJAMODEL_EVENTS
	printf("FreyjaModel::Event> %i:%i\n", mode, cmd);
#endif



	switch (mode)
	{
	case EVENT_TRANSFORM:
		switch (cmd)
		{
		case CMD_TRANSFORM_SCALE_DOWN_XYZ:
			Transform(_edit_mode, Egg::SCALE, 0.9, 0.9, 0.9);
			event_refresh();
			break;
		case CMD_TRANSFORM_SCALE_UP_XYZ:
			Transform(_edit_mode, Egg::SCALE, 1.1, 1.1, 1.1);
			event_refresh();
			break;
		case CMD_TRANSFORM_SCALE_DOWN_X:
			Transform(_edit_mode, Egg::SCALE, 0.99, 1.0, 1.0);
			break;
		case CMD_TRANSFORM_SCALE_UP_X:
			Transform(_edit_mode, Egg::SCALE, 1.01, 1.0, 1.0);
			break;
		case CMD_TRANSFORM_SCALE_DOWN_Y:
			Transform(_edit_mode, Egg::SCALE, 1.0, 0.99, 1.0);
			break;
		case CMD_TRANSFORM_SCALE_UP_Y:
			Transform(_edit_mode, Egg::SCALE, 1.0, 1.01, 1.0);
			break;
		case CMD_TRANSFORM_SCALE_DOWN_Z:
			Transform(_edit_mode, Egg::SCALE, 1.0, 1.0, 0.99);
			break;
		case CMD_TRANSFORM_SCALE_UP_Z:
			Transform(_edit_mode, Egg::SCALE, 1.0, 1.0, 1.01);
			break;
		case CMD_TRANSFORM_TRANS_DOWN_X:
			Transform(_edit_mode, Egg::TRANSLATE, -0.5, 0.0, 0.0);
			break;
		case CMD_TRANSFORM_TRANS_UP_X:
			Transform(_edit_mode, Egg::TRANSLATE, 0.5, 0.0, 0.0);
			break;
		case CMD_TRANSFORM_TRANS_DOWN_Y:
			Transform(_edit_mode, Egg::TRANSLATE, 0.0, -0.5, 0.0);
			break;
		case CMD_TRANSFORM_TRANS_UP_Y:
			Transform(_edit_mode, Egg::TRANSLATE, 0.0, 0.5, 0.0);
			break;
		case CMD_TRANSFORM_TRANS_DOWN_Z:
			Transform(_edit_mode, Egg::TRANSLATE, 0.0, 0.0, -0.5);
			break;
		case CMD_TRANSFORM_TRANS_UP_Z:
			Transform(_edit_mode, Egg::TRANSLATE, 0.0, 0.0, 0.5);
			break;
		case CMD_TRANSFORM_ROT_DOWN_X:
			Transform(_edit_mode, Egg::ROTATE, -0.5, 0.0, 0.0);
			break;
		case CMD_TRANSFORM_ROT_UP_X:
			Transform(_edit_mode, Egg::ROTATE, 0.5, 0.0, 0.0);
			break;
		case CMD_TRANSFORM_ROT_DOWN_Y:
			Transform(_edit_mode, Egg::ROTATE, 0.0, -0.5, 0.0);
			break;
		case CMD_TRANSFORM_ROT_UP_Y:
			Transform(_edit_mode, Egg::ROTATE, 0.0, 0.5, 0.0);
			break;
		case CMD_TRANSFORM_ROT_DOWN_Z:
			Transform(_edit_mode, Egg::ROTATE, 0.0, 0.0, -0.5);
			break;
		case CMD_TRANSFORM_ROT_UP_Z:
			Transform(_edit_mode, Egg::ROTATE, 0.0, 0.0, 0.5);
			break;
		default:
			break;
		}
		break;


	case EVENT_MAIN:
		switch (cmd)
		{
		case CMD_MAIN_DEBUG_EGG:
			Debug(!Debug());
			event_print("Egg debug [%s]", Debug() ? "ON" : "OFF");
			break;
		case CMD_MAIN_NEW_MODEL:
			Clear();
			event_print("Erasing Model...");
			break;
		}
		break;


	case EVENT_ANIMATION:
		switch (cmd)
		{
		case CMD_ANIMATION_NEXT:
			setCurrentAnimation(getCurrentAnimation() + 1);
			break;
		case CMD_ANIMATION_PREV:
			if (getCurrentAnimation())
				setCurrentAnimation(getCurrentAnimation() - 1);
			else
				setCurrentAnimation(0);
		}
		break;


	case EVENT_MESH:
		switch (cmd)
		{
		case CMD_MESH_GENERATE_CAN:
			eggGenerateCylinder(4, 16, 64.0, 32.0);
			event_refresh();
			break;
		case CMD_MESH_GENERATE_SPHERE:
			eggGenerateSphere(16, 16, 64.0);
			event_refresh();
			break;
		case CMD_MESH_GENERATE_CUBE:
			eggGenerateCube(64.0);
			event_refresh();
			break;
		case CMD_MESH_GENERATE_TRIS:
			eggGenerateTriangleStrip(10, 64.0);
			event_refresh();
			break;
		case CMD_MESH_CLONE:
			if (1)
			{
				egg_mesh_t *mesh = CachedMesh();
				egg_group_t *grp;
				
				
				if (mesh && (grp = _egg->getGroup(mesh->group[0])))
				{
					mList.copy(grp->vertex);
					
					if (!mList.empty())
					{
						event_print("Mesh cloned");
						_egg->addMesh(_egg->MeshCopy(mesh, &mList));
					}
				}
			}
			break;
		case CMD_MESH_ROTATE:
			_edit_mode = FreyjaModel::TransformMesh;
			break;
		case CMD_MESH_COPY2:
			MeshCopy();
			break;
		case CMD_MESH_CUT:
			event_print("Mesh partially cut (split)");
			MeshCut();
			break;
		case CMD_MESH_PASTE:
			event_print("Model> ERROR Mesh Pasting not impelmented");
			// _egg->AddMesh(_egg->MeshMerge(_egg->MeshNew(), _list, CachedMesh()));
			// _list.Clear();
			break;
		case CMD_MESH_NEXT:
			setCurrentMesh(getCurrentMesh() + 1);
			break;
		case CMD_MESH_PREV:
			if (getCurrentMesh())
				setCurrentMesh(getCurrentMesh() - 1);
			break;
		case CMD_MESH_MIRROR:
			MeshMirror(getCurrentMesh());
			break;
		case CMD_MESH_DUP_FRAME:// FIXME: make CMD_MESH_FRAME_CLONE
			MeshFrameClone(getCurrentMesh(), getCurrentGroup());
			break;
		case CMD_MESH_DUP_SCENE: // FIXME: make CMD_GROUP_CLONE
			GroupClone(getCurrentGroup());
			break;
		case CMD_MESH_ADD:
			MeshNew();
			break;
		case CMD_MESH_DELETE:
			MeshDel();
			break;
		}
		break;


	case EVENT_MISC:
		switch (cmd)
		{
		case CMD_MISC_LOAD_MAP:
			setFlags(FL_LOAD_MAP, !(getFlags() & FL_LOAD_MAP));
			break;
		case CMD_MISC_EDITMODE:
			_edit_mode++;
     
			if (_edit_mode == 3)
				_edit_mode = 0;
     
			switch (_edit_mode)
			{
			case FreyjaModel::TransformMesh:
				event_print("Edit mode set to MESH");
				break;
			case FreyjaModel::TransformVertexFrame:
				event_print("Edit mode set to FRAME");
				break;
			case FreyjaModel::TransformScene:
				event_print("Edit mode set to SCENE");
				break;	   
			}
			break;
		case CMD_MISC_SCENE_ROTATE:
			_edit_mode = FreyjaModel::TransformScene;
		case CMD_MISC_TEXTURE_SET:
			event_print("Switching all of mesh %i ploygons to texture %i",
						getCurrentMesh(), getCurrentTextureIndex());
			TextureShift();
			break;
		case CMD_MISC_GEN_TEXMAP_XY:
			generateUVMap();
			break;
		case CMD_MISC_VERTEX_UV:
			setFlags(FL_VERTEX_UV, !(getFlags() & FL_VERTEX_UV));
			event_print("Vertex with UV texels [%s]", 
						(getFlags() & FL_VERTEX_UV) ? "on" : "off");
			break;
		case CMD_MISC_PAL_NEXT:
			setFlags(FL_QUAKE_PAL, !(getFlags() & FL_QUAKE_PAL));
			break;
		case CMD_MISC_TEXTURE_NEXT:
			setCurrentTextureIndex(getCurrentTextureIndex() + 1);
			event_print("Texture[%i] in edit mode", getCurrentTextureIndex());
			break;
		case CMD_MISC_TEXTURE_PREV:
			if (getCurrentTextureIndex())
			{
				setCurrentTextureIndex(getCurrentTextureIndex() - 1);
			}

			event_print("Texture[%i] in edit mode", getCurrentTextureIndex());
			break;
		case CMD_MISC_SIDES_M:
			setCurrentPolygonEdgeCount(getCurrentPolygonEdgeCount()-1);
			event_print("Making %i sided polygons", 
						getCurrentPolygonEdgeCount());
			break;
		case CMD_MISC_SIDES_P:
			setCurrentPolygonEdgeCount(getCurrentPolygonEdgeCount()+1);
			event_print("Making %i sided polygons", 
						getCurrentPolygonEdgeCount());
			break;
		case CMD_MISC_DUMPTEXTURE:
			setFlags(FL_DUMP_TEXTURE, !(getFlags() & FL_DUMP_TEXTURE));
			break;
		case CMD_MISC_ZOOM_IN:
			if (getZoom() <= 0.02)
			{
				setZoom(getZoom() + 0.0001);
			}
			else
			{
				setZoom(getZoom() + 0.01);
			}

			event_refresh();
			break;
		case CMD_MISC_ZOOM_OUT:
			if (getZoom() <= 0.02)
			{
				setZoom(getZoom() - 0.0001);
			}
			else
			{
				setZoom(getZoom() - 0.01);
			}

			event_refresh();
			break;
		case CMD_MISC_SCROLL_UP_X:
			adjustSceneTranslation(1.0, 0.0, 0.0);
			break;
		case CMD_MISC_SCROLL_DOWN_X:
			adjustSceneTranslation(-1.0, 0.0, 0.0);
			break;
		case CMD_MISC_SCROLL_UP_Y:
			adjustSceneTranslation(0.0, 1.0, 0.0);
			break;
		case CMD_MISC_SCROLL_DOWN_Y:
			adjustSceneTranslation(0.0, -1.0, 0.0);
			break;
		case CMD_MISC_SCROLL_UP_Z:
			adjustSceneTranslation(0.0, 0.0, 1.0);
			break;
		case CMD_MISC_SCROLL_DOWN_Z:
			adjustSceneTranslation(0.0, 0.0, -1.0);
			break;
		case CMD_MISC_SCROLL_UP:
			switch (CurrentPlane())
			{
			case Egg::PLANE_XY:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_UP_Y);
				break;
			case Egg::PLANE_XZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_UP_Z);
				break;
			case Egg::PLANE_YZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_UP_Z);
				break;
			}
			break;
		case CMD_MISC_SCROLL_DOWN:
			switch (CurrentPlane())
			{
			case Egg::PLANE_XY:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_DOWN_Y);
				break;
			case Egg::PLANE_XZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_DOWN_Z);
				break;
			case Egg::PLANE_YZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_DOWN_Z);
				break;
			}
			break;
		case CMD_MISC_SCROLL_RIGHT:
			switch (CurrentPlane())
			{
			case Egg::PLANE_XY:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_DOWN_X);
				break;
			case Egg::PLANE_XZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_DOWN_X);
				break;
			case Egg::PLANE_YZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_DOWN_Y);
				break;
			}
			break;
		case CMD_MISC_SCROLL_LEFT:
			switch (CurrentPlane())
			{
			case Egg::PLANE_XY:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_UP_X);
				break;
			case Egg::PLANE_XZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_UP_X);
				break;
			case Egg::PLANE_YZ:
				handleEvent(EVENT_MISC, CMD_MISC_SCROLL_UP_Y);
				break;
			}
			break;
		}
		break;


	case EVENT_BONE:
		switch (cmd)
		{
		case CMD_BONE_NEXT:
			setCurrentBone(getCurrentBone() + 1);
			break;
		case CMD_BONE_PREV:
			if (getCurrentBone())
				setCurrentBone(getCurrentBone() - 1);
			break;
		case CMD_BONE_NEW:
			TagNew(0.0, 0.0, 0.0, 0x0);
			event_print("New Bone Tag");
			break;
		case CMD_BONE_ADD_MESH:
			TagAddMesh(getCurrentBone(), getCurrentMesh());
			event_print("New Bone[%i] now contains mesh %i",
						getCurrentBone(), getCurrentMesh());
			break;
		case CMD_BONE_DELETE_MESH:
			TagDelMesh(getCurrentBone(), getCurrentMesh());
			event_print("New Bone[%i] no longer contains mesh %i",
						getCurrentBone(), getCurrentMesh());
			break;
		case CMD_BONE_DELETE:
			TagDel();
			break;
		case CMD_BONE_ROTATE_UP_X:
			Transform(FreyjaModel::TransformBone, Egg::ROTATE, 1.0, 0.0, 0.0);
			break;
		case CMD_BONE_ROTATE_UP_Y:
			Transform(FreyjaModel::TransformBone, Egg::ROTATE, 0.0, 1.0, 0.0);
			break;
		case CMD_BONE_ROTATE_UP_Z:
			Transform(FreyjaModel::TransformBone, Egg::ROTATE, 0.0, 0.0, 1.0);
			break;
		case CMD_BONE_ROTATE_DOWN_X:
			Transform(FreyjaModel::TransformBone, Egg::ROTATE, -1.0, 0.0, 0.0);
			break;
		case CMD_BONE_ROTATE_DOWN_Y:
			Transform(FreyjaModel::TransformBone, Egg::ROTATE, 0.0, -1.0, 0.0);
			break;
		case CMD_BONE_ROTATE_DOWN_Z:
			Transform(FreyjaModel::TransformBone, Egg::ROTATE, 0.0, 0.0, -1.0);
			break;
		}
		break;


	case EVENT_GROUP:
		switch (cmd)
		{
		case CMD_GRP_NEXT:
			setCurrentAnimationFrame(getCurrentAnimationFrame() + 1);
			break;
		case CMD_GRP_PREV:
			if (getCurrentAnimationFrame())
				setCurrentAnimationFrame(getCurrentAnimationFrame() - 1);
			else
				setCurrentAnimationFrame(0);
			break;
		case CMD_GRP_MIRROR:
			event_print("FreyjaModel::Event> GRP_MIRROR not implemented for 8.x, %s:%i\n",
						__FILE__, __LINE__);
			break; 
		}
		break;

	default:
		;
	}

	return 0;
}


void FreyjaModel::setFlags(option_flag_t flag, int op)
{
	_defaults |= flag;

	if (!op)
		_defaults ^= flag;

	// Pretty output in freyja console
	if (flag & FL_QUAKE_PAL)
		event_print("Using %s palette for MDLs", op ? "Quake" : "Hexen2");

	if (flag & FL_DUMP_TEXTURE)
		event_print("Texture dumping is %s", op ? "on" : "off");

	if (flag & FL_LOAD_MAP)
	{
		event_print("TombRaider Map loading is %s", op ? "on" : "off");

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
	_zoom = 1.0f;
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


void FreyjaModel::TextureShift()
{
	egg_mesh_t *mesh;
	egg_polygon_t *poly;
  

	mesh = CachedMesh();

	if (!mesh)
	{
		event_print("FreyjaModel::TextureShift> ERROR: Invalid current mesh\n");
		return;
	}

	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{  
		poly = _egg->getPolygon(mesh->polygon.current());

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
	float u, v;


	mesh = CachedMesh();

	if (!mesh)
		return;

	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{  
		poly = _egg->getPolygon(mesh->polygon.current());

		if (!poly)
			continue;

		// Generate texels and record their ids
		for (poly->vertex.start(); poly->vertex.forward(); poly->vertex.next())
		{
			vertex = _egg->getVertex(poly->vertex.current());

			if (vertex)
			{
				// Mongoose 2002.01.18, Generate UV from vertex XYZ
				eggGenerateUVFromXYZ(vertex->pos, &u, &v);

				texel = _egg->getTexel(poly->texel[poly->vertex.currentIndex()]);

				texel->st[0] = u;
				texel->st[1] = v;

#ifdef DEBUG_GEN_TEXEL
				printf("*** %f %f\n", texel->st[0], texel->st[1]);
#endif
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
	printf("FreyjaModel::getCurrentVertex> Not Implemented %s:%d\n",
		   __FILE__, __LINE__);
	return 0;
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
		
		event_print("Mesh[%i]", getCurrentMesh());
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
		event_print("Group[%i]", getCurrentGroup());
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
		event_print("Bone Tag[%i]", getCurrentBone());
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
	event_print("Texture[%i]", getCurrentTextureIndex());
}

void FreyjaModel::setCurrentAnimation(unsigned int index)
{
	if (index < _egg->getAnimationCount())
	{
		_current_animation_frame = index;
		setCurrentSkeleton(0);
		
		event_print("Animation[%i] Frame[%i]", 
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
	
	event_print("Animation[%i].frame[%i] = %i", 
				getCurrentAnimation(), getCurrentAnimationFrame(), 
				getCurrentSkeleton());
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


void FreyjaModel::Transform(int mode, enum Egg::egg_transform type, 
							float x, float y, float z)
{
	switch (mode)
	{
	case FreyjaModel::TransformVertexFrame:
		_egg->Transform(CachedGroup(), type, x, y, z);
		break;
	case FreyjaModel::TransformMesh:
		if (type == Egg::ROTATE)
		{
			type = Egg::ROTATE_ABOUT_CENTER;

			// Mongoose 2002.01.18, Scaled rotation for finer control
			x *= 0.1;
			y *= 0.1;
			z *= 0.1;
		}

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


void FreyjaModel::setZoom(float zoom)
{
	assert(zoom > 0.0f);

	_zoom = zoom;
	event_print("Zoom set to %f", zoom);
}


float FreyjaModel::getZoom()
{
	return _zoom;
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

void FreyjaModel::TagSelect(float xx, float yy)
{
	egg_tag_t *bone;

	bone = _egg->getNearestTag(xx, yy, CurrentPlane());

	if (bone)
		setCurrentBone(bone->id);
}

void FreyjaModel::TagConnect(unsigned int master, unsigned int slave)
{
	_egg->connectTag(master, slave);
}

void FreyjaModel::VertexBuffer(float xx, float yy)
{
	VertexSelect(xx, yy);

	if (!_cached_vertex)
	{
		event_print("Control> refused to use bad vertex in list");
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
		event_print("Control> Bad vertex, not selecting polygon");
	}
	else
	{
		vertices.pushBack(_cached_vertex->id);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		event_print("Control> Polygon selected");

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
		event_print("Control> Bad vertex, not removing polygon");
	}
	else
	{
		vertices.pushBack(_cached_vertex->id);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		event_print("Control> Polygon removed");

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
	unsigned int polygonId;


	VertexSelect(xx, yy);
    
	if (!_cached_vertex)
	{
		vertices.clear();
		event_print("Control> Bad vertex, not building polygon");
	}
	else
	{
		vertices.pushBack(_cached_vertex->id);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		event_print("Control> New Polygon made");

		// Generate texels and record their ids
		for (vertices.start(); vertices.forward(); vertices.next())
		{
			vertex = _egg->getVertex(vertices.current());

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

void FreyjaModel::MeshCut()
{
	/***************
  egg_mesh_t *mesh = CachedMesh();
  _egg->AddMesh(_egg->MeshSplit(mesh, _list));
  _list.Clear();
	***************/

	fprintf(stderr,"FreyjaModel::MeshCut> Not implemented, %s:%i\n",
			__FILE__, __LINE__);
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
		event_print("New %s mesh cloned", whole ? "whole" : "partial");
		_egg->MeshCopy(mesh, &mList);
		mList.clear();
	}
}

void FreyjaModel::TagDisconnect(unsigned int master, unsigned int slave)
{
	_egg->TagDisconnect(master, slave);
}

void FreyjaModel::MeshDel()
{
	event_print("Mesh[%u] deleted\n", _current_mesh);
	_egg->delMesh(_current_mesh);
}

void FreyjaModel::TagDel()
{
	fprintf(stderr,"FreyjaModel::TagDel> Not implemented, %s:%i\n",
			__FILE__, __LINE__);
}

void FreyjaModel::MeshMirror(unsigned int id)
{
	fprintf(stderr,"FreyjaModel::MeshMirror> Not implemented, %s:%i\n",
			__FILE__, __LINE__); 
}


void FreyjaModel::TagMoveCenter(float xx, float yy)
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



void FreyjaModel::TagMove(float xx, float yy)
{
	unit_t x = 0, y = 0, z = 0;
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
  
	Transform(FreyjaModel::TransformBone, Egg::TRANSLATE, x, y, z);
}


void FreyjaModel::MeshMove(float xx, float yy)
{
	unit_t x = 0, y = 0, z = 0;


	if (!_cached_vertex)
		return;

	switch (CurrentPlane())
	{
	case Egg::PLANE_XY:
		x = xx - _cached_vertex->pos[0];
		y = yy - _cached_vertex->pos[1];
		z = 0;
		break;
	case Egg::PLANE_XZ:
		x = xx - _cached_vertex->pos[0];
		y = 0;
		z = yy - _cached_vertex->pos[2];
		break;
	case Egg::PLANE_YZ:
		x = 0;
		y = xx - _cached_vertex->pos[1];
		z = yy - _cached_vertex->pos[2];
		break;
	}
  
	Transform(FreyjaModel::TransformMesh, Egg::TRANSLATE, x, y, z);  
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
		event_print("FreyjaModel::VertexNew> Point requires Mesh; Try Mesh->New");
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
    
		event_print("Vertex[%i] (%.3f %.3f %.3f), %i, %i", 
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
		event_print("VertexCombine(%i, ?)", _cached_vertex->id);
	}
	else if (_cached_vertex != v0)
	{
		event_print("VertexCombine(%i, %i)", v0->id, _cached_vertex->id);
		_egg->combineVertices(v0->id, _cached_vertex->id);

		_cached_vertex = v0;
		v0 = NULL;
	}
}


void FreyjaModel::VertexMove(float xx, float yy)
{
	if (!_cached_vertex)
	{
		event_print("FreyjaModel::VertexMove> No vertex to move!\n");
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

void FreyjaModel::Bbox(point_t min, point_t max, Vector<unsigned int> **list)
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

	dist0 = mtkDist2d(xx, yy, mSelectBBox[0][x], mSelectBBox[0][y]);
	dist1 = mtkDist2d(xx, yy, mSelectBBox[1][x], mSelectBBox[1][y]);

	if (dist0 < dist1)
		_bbox = 0;
	else
		_bbox = 1;
  
	//event_print("Vertex bbox select is %i", _bbox);
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


	//event_print("Vertex bbox select is %i @ %f,%f", _bbox, xx, yy);
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
	mList.print(__print_unsigned_int);
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

	event_print("Mesh[%i] made with group %i", mesh->id, grp->id);
}


void FreyjaModel::MeshSelect(float xx, float yy)
{
	egg_mesh_t *mesh;
	Vector<egg_mesh_t *> *meshlst;
	egg_group_t *frame;

	
	frame = _egg->getNearestGroup(xx, yy, CurrentPlane());

	meshlst = _egg->MeshList();

	if (!meshlst || !frame)
	{
		event_print("MeshSelect assertion failed #1 %p %p\n", frame, meshlst);
		return;
	}

	for (meshlst->start(); meshlst->forward(); meshlst->next())
	{
		mesh = meshlst->current();
    
		if (mesh && mesh->group.SearchIndex(frame->id) != UINT_MAX)
		{
			event_print("Selected mesh %u, group %u\n", mesh->id, frame->id);
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


void FreyjaModel::MeshFrameClone(unsigned int mesh, unsigned int grp)
{
	fprintf(stderr,"FreyjaModel::MeshFrameClone> Not implemented, %s:%i\n",
			__FILE__, __LINE__);
}


void FreyjaModel::GroupClone(unsigned int grp)
{
	fprintf(stderr,"FreyjaModel::GroupClone> Not implemented, %s:%i\n",
			__FILE__, __LINE__);
}


void FreyjaModel::TexelCombine(float s, float t)
{
	static int a = -1;


	TexelSelect(s, t);

	if (a == -1)
	{
		a = _current_texel;
		event_print("TexelCombine(%i, ?)", a);
	}
	else if ((int)_current_texel != a)
	{
		event_print("TexelCombine(%i, %i)", a, _current_texel);
		_egg->combineTexels(a, _current_texel);
		a = -1;
	}
}


void FreyjaModel::TexelSelect(float s, float t)
{
	egg_mesh_t *mesh = CachedMesh();
	egg_polygon_t *polygon = NULL;
	egg_texel_t *texel = NULL;
	egg_texel_t *best_texel = NULL;
	unit_t dist = 0.0;
	unit_t best_dist = 0.0;


#ifdef DEBUG_TEXEL
	printf("s = %f, t = %f\n", s, t);
#endif

	// Mongoose: Removed 5 layers of scoping from old code ( loops, ew )
	if (!mesh || mesh->polygon.empty())
	{
		event_print("No polygons found! Mesh %i [%p]", getCurrentMesh(), mesh);
		return;
	}

	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{
		polygon = _egg->getPolygon(mesh->polygon.current());

		if (!polygon)
			continue;

		// FIXME: Replace with call to shader for texture id
		//        once shaders are put in engine
		if (polygon->shader < 0 || 
			polygon->shader != (int)getCurrentTextureIndex())
			continue;

		for (polygon->texel.start();
			 polygon->texel.forward();
			 polygon->texel.next())
		{
			texel = _egg->getTexel(polygon->texel.current());
   
			if (!texel)
				continue;

			// Mongoose: slight optimization, to avoid init branch  =)
			dist = mtkDist2d(s, t, texel->st[0], texel->st[1]);

			if (dist < best_dist)
				best_texel = NULL;
		
			if (best_texel == NULL)
			{ 
				best_texel = texel;
				best_dist = dist;

#ifdef DEBUG_TEXEL
				printf("New best_texel = %u @ %f\n", _current_texel, dist);
#endif
			}
		}
	}
      
	// Mongoose: now we set the new _current_texel

	if (best_texel)
		_current_texel = best_texel->id;
}


void FreyjaModel::TexelMove(float s, float t)
{
	egg_texel_t *texel = _egg->getTexel(getCurrentTexCoord());
  

	if (!texel)
		return;

#ifdef DEBUG_TEXELMOVE
	printf("Moving texel[%u] (%f, %f) -> (%f, %f)\n", 
		   texel->id, texel->st[0], texel->st[1], s, t);
#endif

	texel->st[0] = s;
	texel->st[1] = t;
}

void FreyjaModel::TagNew(float x, float y, float z, unsigned char flag)
{
	_egg->addTag(x, y, z, flag);
}

void FreyjaModel::TagAddMesh(unsigned int tag, unsigned int mesh)
{
	_egg->TagAddMesh(CachedTag(), getCurrentMesh());
}

void FreyjaModel::TagDelMesh(unsigned int tag, unsigned int mesh)
{
	_egg->TagDelMesh(CachedTag(), getCurrentMesh());
}


callback_bone_t *generateSkeletalUI(Egg *egg, egg_tag_t *tag, 
									callback_bone_t *bone)
{
	egg_tag_t *tagChild;
	unsigned int t;


	if (!tag)
	{
		printf("ERROR: NULL skeletal bone_tag!\n");
		return 0x0;
	}

	if (bone == 0x0)
	{
		bone = new callback_bone_t;
		snprintf(bone->name, 64, "root");	
		bone->parent = 0x0;
	}
	else
	{
		snprintf(bone->name, 64, "bone%03i", tag->id);
	}

	if (tag->name[0])
	{
		snprintf(bone->name, 64, "%s", tag->name);
	}

	bone->id = tag->id;
	bone->numChildren = tag->slave.size();
	bone->children = 0x0;

	printf("-- %s : %i/%i children\n", 
		   bone->name, bone->numChildren, tag->slave.size());

	if (bone->numChildren == 0)
		return bone->parent;

	bone->children = new callback_bone_t[bone->numChildren+1];
	int i = 0;
	for (tag->slave.start(); tag->slave.forward(); tag->slave.next())
	{
		t = tag->slave.current();
		tagChild = egg->getTag(t);

		if (tagChild)
		{
			bone->children[i].parent = bone;

			printf("   -- parent %i <-- child %i\n", bone->id, t);
			generateSkeletalUI(egg, tagChild, &bone->children[i++]);
		}
	}

	return (bone->parent) ? bone->parent : bone;
}


void testBone(callback_bone_t *bone, unsigned int space)
{
	unsigned int i;

	for (i = 0; i < space; ++i)
	{
		printf(" ");
	}

	printf("+ T_BONE %d\n", bone->id);

	for (i = 0; i < bone->numChildren; ++i)
	{
		testBone(&bone->children[i], space+1);
	}
}

void updateSkeletonUI(Egg *egg)
{
	callback_bone_t *bone;
	unsigned int i;

	bone = generateSkeletalUI(egg, egg->getTag(0), 0x0);

	for (i = 0; i < egg->getTagCount(); ++i)
	{
		printf("TAG %d :: ", i);
		egg_tag_t *tag = egg->getTag(i);

		for (tag->slave.start(); tag->slave.forward(); tag->slave.next())
		{
			printf("%d ", tag->slave.current());
		}
		printf("\n");
	}

	// testBone(bone, 0);

	callback_update_skeleton(bone);
}


int FreyjaModel::loadModel(const char *filename)
{
	int err;


	err = _plugin->importModel((char*)filename);  // try plug-in modules last

	updateSkeletonUI(_egg);		
 
	if (err)
	{
		printf("ERROR: File '%s' not found or unknown format\n", filename);
		printf("ERROR CODE %i\n", err);
	}
	else
	{
		unsigned int i, w, h, bpp, type;
		unsigned char *image = 0x0;
		char *textureFilename = 0x0; /* Texture filename */

		/* Texture image was stored as external file from model */
		for (i = 0; !eggGetTextureFilename(i, &textureFilename); ++i)
		{
			if (textureFilename && textureFilename[0])
			{
				printf("FrejaModel::loadModel> Loading texture %i from file\n", i);
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
				event_print("%s> ERROR: Unsupported texture colormap %d",
							"FreyjaModel::loadModel", type);
			}
		}

		//event_print("FreyjaModel::loadModel> Loaded %s, %i polys, %i verts\n", 
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
		event_print("Unknown file export extention: '%s', try using '.egg'", 
					ext);
		return ret;
	}
  
	return 0;
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

