/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : EggPlugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 *
 *-- Test Defines -------------------------------------------
 *           
 * UNIT_TEST_EGGPLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#include <sys/types.h>
#include <dirent.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#   undef MODEL_PLUGINS
#endif

#ifdef MODEL_PLUGINS
#   include <dlfcn.h> 
#endif

#include <hel/math.h>
#include <hel/Quaternion.h>
#include <hel/Vector3d.h>

#include "FreyjaFileReader.h"
#include "EggPlugin.h"


EggPlugin *EggPlugin::mEggPlugin = 0x0;


EggPlugin::EggPlugin(Egg *egg)
{
	mFlags = 0;

	mEgg = egg;
	mTag = 0x0;
	mMesh = 0x0;
	mGroup = 0x0;
	mAnimation = 0x0;
	mBoneFrame = 0x0;
	mCurrentPlugin = 0x0;
	mTextureId = 0;

#ifdef unix
   	addPluginDirectory("/usr/lib/freyja/modules/model");
   	addPluginDirectory("/usr/local/lib/freyja/modules/model");
	addPluginDirectory("/usr/share/freyja/modules/model");
#else
	addPluginDirectory("./modules/model");
#endif

	EggPlugin::mEggPlugin = this;
}


EggPlugin::~EggPlugin()
{
	mTextures.erase();
	mTextureFiles.erase();
	mPluginDirectories.erase();
	mPlugins.erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

long EggPlugin::freyjaGetCount(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MODEL:
		return 1; // NOTE: Old Egg backend has no concept of multiple models
		break;

	case FREYJA_VERTEX:
		return mEgg->getVertexCount();
		break;

	case FREYJA_TEXCOORD:
		return mEgg->getTexelCount();
		break;

	case FREYJA_MESH:
		return mEgg->getMeshCount();
		break;

	case FREYJA_VERTEX_GROUP:
		return mEgg->getGroupCount();
		break;

	case FREYJA_POLYGON:
		return mEgg->getPolygonCount();
		break;

	case FREYJA_BONE:
		return mEgg->getTagCount();
		break;

	case FREYJA_SKELETON:
		return mEgg->getBoneFrameCount();
		break;

	case FREYJA_MESHTREE_FRAME:
		return mEgg->getAnimationCount();
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_TEXTURE:
	case FREYJA_ANIMATION:
	case FREYJA_MATERIAL:
	case FREYJA_MESHTREE_TAG:
		return 0;
		break;
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaIterator(freyja_object_t type, int item)
{
	Vector<egg_vertex_t *> *vertex;
	Vector<egg_texel_t *> *texel;
	Vector<egg_mesh_t *> *mesh;
	Vector<egg_group_t *> *group;
	Vector<egg_polygon_t *> *polygon;
	Vector<egg_tag_t *> *tag;
	Vector<egg_boneframe_t *> *boneframe;
	Vector<egg_animation_t *> *skelanim;


	switch (type)
	{
	case FREYJA_MODEL:
		if (item < 1 && item > 0)
		{
			return 0;
		}
		break;

	case FREYJA_VERTEX:
		vertex = mEgg->VertexList();
    
		if (!vertex)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexVertex = vertex->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexVertex;
			break;
		default:
			mIndexVertex = item;
		}

		if (mIndexVertex < vertex->end())
		{
			return mIndexVertex;
		}
		break;
	case FREYJA_TEXCOORD:
		texel = mEgg->TexelList();
    
		if (!texel)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexTexCoord = texel->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexTexCoord;
			break;
		default:
			mIndexTexCoord = item;
		}

		if (mIndexTexCoord < texel->end())
		{
			return mIndexTexCoord;
		}
		break;
	case FREYJA_MESH:
		mesh = mEgg->MeshList();
    
		if (!mesh)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexMesh = mesh->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexMesh;
			break;
		default:
			mIndexMesh = item;
		}

		if (mIndexMesh < mesh->end())
		{
			return mIndexMesh;
		}
		break;
	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
		group = mEgg->GroupList();
    
		if (!group)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexGroup = group->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexGroup;
			break;
		default:
			mIndexGroup = item;
		}

		if (mIndexGroup < group->end())
		{
			return mIndexGroup;
		}
		break;
	case FREYJA_POLYGON:
		polygon = mEgg->PolygonList();
    
		if (!polygon)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexPolygon = polygon->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexPolygon;
			break;
		default:
			mIndexPolygon = item;
		}

		if (mIndexPolygon < polygon->end())
		{
			return mIndexPolygon;
		}
		break;


	case FREYJA_MESHTREE_TAG:
	case FREYJA_BONE:
		tag = mEgg->TagList();
    
		if (!tag)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexBone = tag->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexBone;
			break;
		default:
			mIndexBone = item;
		}

		if (mIndexBone < tag->end())
		{
			mTag = (*tag)[mIndexBone];
			return mIndexBone;
		}
		break;


	case FREYJA_SKELETON:
		boneframe = mEgg->BoneFrameList();
    
		if (!boneframe)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexSkeleton = boneframe->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexSkeleton;
			break;
		default:
			mIndexSkeleton = item;
		}

		if (mIndexSkeleton < boneframe->end())
		{
			return mIndexSkeleton;
		}
		break;
	case FREYJA_MESHTREE_FRAME:
		skelanim = mEgg->AnimationList();
    
		if (!skelanim)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexSkeletonAnim = skelanim->begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexSkeletonAnim;
			break;
		default:
			mIndexSkeletonAnim = item;
		}

		if (mIndexSkeletonAnim < skelanim->end())
		{
			return mIndexSkeletonAnim;
		}
		break;

	case FREYJA_MATERIAL:
	case FREYJA_TEXTURE:
	case FREYJA_ANIMATION:
		break;
	}

	return FREYJA_PLUGIN_ERROR;
}


void EggPlugin::freyjaGetVertex(vec3_t xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end() || index < 0)
		return;// FREYJA_PLUGIN_ERROR;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return;// FREYJA_PLUGIN_ERROR;

	xyz[0] = vert->pos[0];
	xyz[1] = vert->pos[1];
	xyz[2] = vert->pos[2];

	//return vert->id;
}


void EggPlugin::freyjaGetTexCoord(long index, vec2_t uv)
{
	Vector<egg_texel_t *> *texel;
	egg_texel_t *tex;


	texel = mEgg->TexelList();

	if (!texel || index >= (int)texel->end() || index < 0)
		return;// FREYJA_PLUGIN_ERROR;
 
	tex = (*texel)[index];

	if (!tex)
		return;// FREYJA_PLUGIN_ERROR;

	uv[0] = tex->st[0];
	uv[1] = tex->st[1];

	//return vert->id;
}


void EggPlugin::freyjaGetVertexByIndex(long index, vec3_t xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || index >= (int)vertex->end() || index < 0)
		return;// FREYJA_PLUGIN_ERROR;
 
	vert = (*vertex)[index];

	if (!vert)
		return;// FREYJA_PLUGIN_ERROR;

	xyz[0] = vert->pos[0];
	xyz[1] = vert->pos[1];
	xyz[2] = vert->pos[2];

	//return vert->id;
}


void EggPlugin::freyjaGetVertexNormal(vec3_t xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end())
		return;// FREYJA_PLUGIN_ERROR;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return;// FREYJA_PLUGIN_ERROR;

	xyz[0] = vert->norm[0];
	xyz[1] = vert->norm[1];
	xyz[2] = vert->norm[2];

	//return vert->id;
}


void EggPlugin::freyjaGetVertexTexCoord(vec2_t uv)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end())
		return;// FREYJA_PLUGIN_ERROR;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return;// FREYJA_PLUGIN_ERROR;

	uv[0] = vert->uv[0];
	uv[1] = vert->uv[1];

	//return vert->id;
}


long EggPlugin::getPolygonVertexIndex(long polygonIndex, long element)
{
	egg_polygon_t *polygon;


	polygon = getPolygon(polygonIndex);

	if (!polygon || element < 0 || element >= (int)polygon->vertex.end())
		return -1; 

	return polygon->vertex[element];
}


long EggPlugin::getPolygonTexCoordIndex(long polygonIndex, long element)
{
	egg_polygon_t *polygon;


	polygon = getPolygon(polygonIndex);

	if (!polygon || element < 0 || element >= (int)polygon->texel.end())
		return -1; 

	return polygon->texel[element];
}


long EggPlugin::getPolygonMaterial(long polygonIndex)
{
	egg_polygon_t *polygon;


	polygon = getPolygon(polygonIndex);

	if (!polygon)
		return 0; 

	return polygon->shader;
}


long EggPlugin::getPolygonEdgeCount(long polygonIndex)
{
	egg_polygon_t *polygon;


	polygon = getPolygon(polygonIndex);

	if (!polygon)
		return 0;

	return polygon->vertex.end();
}


long EggPlugin::getPolygonFlags(long polygonIndex)
{
	egg_polygon_t *polygon;
	long flags = 0x0;


	polygon = getPolygon(polygonIndex);

	if (!polygon)
		return 0; 

	if (polygon->vertex.end() == polygon->texel.end())
		flags |= fPolygon_PolyMapped;

	if (polygon->vertex.end() == polygon->texel.end()*2)
		flags |= fPolygon_ColorMapped;

	return flags;
}


long EggPlugin::freyjaGetPolygon(freyja_object_t type, long item, 
								 long *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return FREYJA_PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return FREYJA_PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return FREYJA_PLUGIN_ERROR;

		*value = vert->id;
		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return FREYJA_PLUGIN_ERROR;

		*value = tex->id;
		return tex->id;
		break;
	default:
		return FREYJA_PLUGIN_ERROR;
	}

	return FREYJA_PLUGIN_ERROR;
}


egg_polygon_t *EggPlugin::getPolygon(long index)
{
	Vector<egg_polygon_t *> *polygons;
	egg_polygon_t *poly;
	
	polygons = mEgg->PolygonList();

	if (!polygons || polygons->empty() || index > (int)polygons->size()-1)
		return 0x0;

	poly = (*polygons)[index];

	return poly;
}


egg_mesh_t *EggPlugin::getMesh(long index)
{
	Vector<egg_mesh_t *> *meshes;
	egg_mesh_t *mesh;
	
	meshes = mEgg->MeshList();

	if (!meshes || meshes->empty() || index > (int)meshes->size()-1)
		return 0x0;

	mesh = (*meshes)[index];

	return mesh;
}


egg_tag_t *EggPlugin::getBone(long index)
{
	Vector<egg_tag_t *> *bones;
	egg_tag_t *bone;
	
	bones = mEgg->TagList();

	if (!bones || bones->empty() || index > (int)bones->size()-1)
		return 0x0;

	bone = (*bones)[index];

	return bone;
}


egg_vertex_t *EggPlugin::getVertex(long index)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *v;
	
	vertex = mEgg->VertexList();

	if (!vertex || vertex->empty() || index > (int)vertex->size()-1)
		return 0x0;

	v = (*vertex)[index];

	return v;
}


long EggPlugin::freyjaGetPolygon(freyja_object_t type, long item, float *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return FREYJA_PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return FREYJA_PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return FREYJA_PLUGIN_ERROR;

		value[0] = vert->pos[0];
		value[1] = vert->pos[1];
		value[2] = vert->pos[2];

		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return FREYJA_PLUGIN_ERROR;

		value[0] = tex->st[0];
		value[1] = tex->st[1];
    
		return tex->id;
		break;
	default:
		return FREYJA_PLUGIN_ERROR;
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetBoneMeshIndex(long element)
{
	if (mTag)
	{
		return mTag->mesh[element];
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetBoneMeshCount()
{
	if (mTag)
	{
		return mTag->mesh.size();
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetCurrent(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_VERTEX:
		return mIndexVertex;
		break;

	case FREYJA_TEXCOORD:
		return mIndexTexCoord;
		break;

	case FREYJA_MESH:
		if (mMesh)
			return mMesh->id;
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
		if (mGroup)
			return mGroup->id;
		break;

	case FREYJA_POLYGON:
		return mIndexPolygon;
		break;

	case FREYJA_MESHTREE_TAG:
	case FREYJA_BONE:
		if (mTag)
			return mTag->id;
		break;

	case FREYJA_SKELETON:
		if (mBoneFrame)
			return mBoneFrame->id;
		break;

	case FREYJA_MESHTREE_FRAME:
		if (mAnimation)
			return mAnimation->id;
		break;

	case FREYJA_MODEL:
	case FREYJA_MATERIAL:
	case FREYJA_TEXTURE:
	case FREYJA_ANIMATION:
		return 0;
		break;
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetBoneRotate(vec_t *x, vec_t *y, vec_t *z)
{
	if (mTag)
	{
		*x = mTag->rot[0];
		*y = mTag->rot[1];
		*z = mTag->rot[2];
   
		return 0;
	}

	return FREYJA_PLUGIN_ERROR;
}

	
void EggPlugin::freyjaPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	freyjaPrintError(format, &args);
	va_end(args);
}


void EggPlugin::freyjaPrintError(char *format, va_list *args)
{
	if (mPrinter)
	{
		mPrinter->errorArgs(format, args);
	}
	else
	{
		vfprintf(stderr, format, *args);
		fprintf(stderr, "\n");
	}
}


void EggPlugin::freyjaPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	freyjaPrintMessage(format, &args);
	va_end(args);
}


void EggPlugin::freyjaPrintMessage(char *format, va_list *args)
{
	if (mPrinter)
	{
		mPrinter->messageArgs(format, args);
	}
	else
	{
		vfprintf(stdout, format, *args);
		printf("\n");
	}
}


bool EggPlugin::checkModel(const char *filename)
{
	if (freyjaCheckModel((char *)filename) == 0)
		return true;

	if (Egg::checkFile((char *)filename) == 0)
		return true;

	return false;
}


bool EggPlugin::saveModel(const char *filename)
{
	if (freyjaSaveModel((char*)filename) == 0)
		return true;

	return false;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool EggPlugin::loadModel(const char *filename)
{
	if (freyjaCheckModel((char *)filename) == 0)
	{
		if (freyjaLoadModel((char *)filename) == 0)
			return true;
	}
	else
	{
		if (mEgg->loadFile((char *)filename) == 0)
			return true;
	}

	return false;
}


void EggPlugin::setPrinter(FreyjaPrinter *printer)
{
	mPrinter = printer;
}


void EggPlugin::addPluginDirectory(const char *dir)
{
	FreyjaFileReader r;
	unsigned int l;
	char *dir2;


	if (!dir || !dir[0] || !r.isDirectory(dir))
		return;

	l = strlen(dir);

	dir2 = new char[l+1];
	strncpy(dir2, dir, l);
	dir2[l] = 0;

	mPluginDirectories.pushBack(dir2);
	setupPlugins();
}


void EggPlugin::setupPlugins()
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	bool loaded = false, done = false;
	char *module_filename;
	int (*import)(char *filename);
	void *handle;
	char *error;
	unsigned int i;


	mPlugins.erase();

	freyjaPrintMessage("[FreyjaPlugin (Egg) module loader invoked]");

	/* Check for other format */
	for (i = mPluginDirectories.begin(); i < mPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while (!done && (module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
				continue;

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				freyjaPrintError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
				}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				freyjaPrintMessage("Module '%s' opened.", module_filename);

				import = (int (*)(char *filename))dlsym(handle, "plugin_properities");

				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
					continue;
				}
				
				done = !(*import)((char*)module_filename);

				if (done)
				{
					loaded = true;
					freyjaPrintMessage("Module '%s' success.", module_filename);
				}

				if ((error = dlerror()) != NULL) 
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
			}
		}

		reader.closeDirectory();

		if (done)
		{
			break;
		}
	}

	freyjaPrintMessage("[FreyjaPlugin (Egg) module loader sleeps now]\n");
#endif
}


long EggPlugin::importModel(const char *filename)
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	bool loaded = false, done = false;
	char *module_filename;
	int (*import)(char *filename);
	void *handle;
	char *error;
	unsigned int i;


	freyjaPrintMessage("[FreyjaPlugin (Egg) module loader invoked]");

	if (!reader.doesFileExist(filename))
	{
		freyjaPrintError("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	/* Check for native format */
	if (checkModel(filename))
	{
		if (loadModel(filename))
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	/* Check for other format */
	for (i = mPluginDirectories.begin(); i < mPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while (!done && (module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
				continue;

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				freyjaPrintError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
				}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				freyjaPrintMessage("Module '%s' opened.", module_filename);

#ifdef NO_DUPE_DL_SYM_HACK
				/* Mongoose 2004.11.01, 
				 * temp fix? */
				unsigned int l = strlen(module_filename);
				char tmp[128];
				module_filename[l-3] = 0;
				snprintf(tmp, 64, "%s_import_model", basename(module_filename));
				freyjaPrintMessage("Symbol '%s' import...", tmp);
				import = (int (*)(char *filename))dlsym(handle, tmp);
#else
				import = (int (*)(char *filename))dlsym(handle, "import_model");
#endif

				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
					continue;
				}
				
				done = !(*import)((char*)filename);

				if (done)
				{
					loaded = true;
					freyjaPrintMessage("Module '%s' success.", module_filename);
				}

				if ((error = dlerror()) != NULL) 
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
			}
		}

		reader.closeDirectory();

		if (done)
		{
			break;
		}
	}

	freyjaPrintMessage("[FreyjaPlugin (Egg) module loader sleeps now]\n");

	if (loaded)
		return 0; // sucess
#endif
	return -1;
}


long EggPlugin::exportModel(const char *filename, const char *type)
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	bool saved = false;
	char module_filename[128];
	char module_export[128];
	char *name;
	int (*export_mdl)(char *filename);
	void *handle;
	char *error;
	unsigned long i;


	if (!type || !filename)
		return -100;

	/* Check for native format or temp use of EGG here */
	if (strcmp(type, "ja") == 0)
	{
		return !saveModel(filename);
	}
	else if (strcmp(type, "egg") == 0)
	{
		return mEgg->saveFile((char *)filename);
	}

	freyjaPrintMessage("[FreyjaPlugin module loader invoked]\n");

	name = (char*)type;

	/* Check for other format */
	for (i = mPluginDirectories.begin(); i < mPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory");
			continue;
		}

		sprintf(module_filename, "%s/%s.so", mPluginDirectories[i], name);
		sprintf(module_export, "freyja_model__%s_export", name);  // use 'model_export'?

		if (!(handle = dlopen(module_filename, RTLD_NOW)))
		{
			freyjaPrintError("\tERROR: In module '%s'.\n", module_filename);

			if ((error = dlerror()) != NULL)  
			{
				freyjaPrintError("\tERROR: %s\n", error);
			}
		}
		else
		{
			freyjaPrintMessage("\tModule '%s' opened.\n", module_filename);
    
			export_mdl = (int (*)(char * filename))dlsym(handle, module_export);

			if ((error = dlerror()) != NULL)  
			{
				freyjaPrintError("\tERROR: %s\n", error);
				dlclose(handle);
			}

			saved = (!(*export_mdl)((char*)filename));

			if ((error = dlerror()) != NULL) 
			{
				dlclose(handle);
			}

			dlclose(handle);
		}

		if (saved)
			break;
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // sucess
#else
	if (!type || !filename)
		return -100;

	/* Check for native format or temp use of EGG here */
	if (strcmp(type, "ja") == 0)
	{
		return !saveModel(filename);
	}
	else if (strcmp(type, "egg") == 0)
	{
		return mEgg->saveFile((char *)filename);
	}
#endif
	return -1;
}


void EggPlugin::freyjaBegin(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MESH:
		mStack.push(FREYJA_MESH);
		mMesh = mEgg->newMesh();
		mEgg->addMesh(mMesh);
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
		mStack.push(FREYJA_VERTEX_GROUP);

		if (!mMesh)
		{
			freyjaPrintError("EggPlugin::freyjaBegin> GROUP defined outside MESH.");
		}
		else
		{
			mGroup = mEgg->newGroup();
			mGroup->flags = 0x0;
			mEgg->addGroup(mGroup);
			mMesh->group.pushBack(mGroup->id); 
			mGroup->mesh = mMesh->id;

			if (type == FREYJA_VERTEX_FRAME)
			{
				mGroup->flags = 0xBADA55;
			}
		}
		break;

	case FREYJA_POLYGON:
		mStack.push(FREYJA_POLYGON);
		mVertexList.clear();
		mTexCoordList.clear();

		if (!mMesh)
		{
			//freyjaPrintError("freyjaBegin: WARNING, POLYGON outside MESH");
		}
		break;

	case FREYJA_BONE:
		mStack.push(FREYJA_BONE);
		mTag = mEgg->addTag(0.0, 0.0, 0.0, 0x00);
		mTag->name[0] = 0;
		mTag->parent = -1;
		break;

	case FREYJA_SKELETON:
		mStack.push(FREYJA_SKELETON);
		break;

	case FREYJA_MESHTREE_FRAME:
		mStack.push(FREYJA_MESHTREE_FRAME);
		mAnimation = new egg_animation_t;
		mEgg->addAnimation(mAnimation);
		break;

	case FREYJA_MODEL:
		mStack.push(FREYJA_MODEL);
		break;

	default:
		freyjaPrintError("freyjaBegin(%i): Unknown type", type);
		mStack.push(type);
	}
}


void EggPlugin::freyjaEnd()
{
	unsigned int polygon;


	switch (mStack.pop())
	{
	case FREYJA_POLYGON:
		polygon = mEgg->addPolygon(mVertexList, mTexCoordList, mTextureId);

		if (polygon == UINT_MAX)
		{
			freyjaPrintMessage("EggPlugin::freyjaEnd> Polygon is invalid\n");
			//return FREYJA_PLUGIN_ERROR;
		}

		if (mMesh)
		{
			mMesh->polygon.pushBack(polygon);
			mMesh->r_polygon.pushBack(mEgg->getPolygon(polygon));
		}
		else
		{
			freyjaPrintError("freyjaEnd: WARNING, FREYJA_POLYGON outside FREYJA_MESH");
		}
		break;

	case FREYJA_SKELETON:
		mEgg->updateBones();
		break;

	default:
		;
	}
}


long EggPlugin::freyjaTextureStore(EggTextureData *textureData)
{
	if (textureData)
	{
		mTextures.pushBack(textureData);
		return mTextures.size() - 1;
	}

	return -1;
}


long EggPlugin::freyjaTextureStoreFilename(const char *filename)
{
	unsigned int i;


	if (!filename || !filename[0])
	{
		return 0;
	}

	if (!mTextureFiles.empty())
	{
		for (i = mTextureFiles.begin(); i < mTextureFiles.end(); ++i)
		{
			if (!strcmp(mTextureFiles[i], filename))
				return i;
		}
	}

	mTextureFiles.pushBack(strdup(filename));
	return mTextureFiles.size() - 1;
}

long EggPlugin::freyjaGetTextureFilename(long index, char **filename)
{
	*filename = 0x0;


	if (mTextureFiles.empty() || index > (int)mTextureFiles.size()-1)
		return -1;

	*filename = mTextureFiles[index];

	return 0;
}


long EggPlugin::freyjaGetTextureImage(long index, 
									  unsigned int *w, unsigned int *h, 
									  unsigned int *depth,
									  unsigned int *type,
									  unsigned char **image)
{
	EggTextureData *t;


	if (mTextures.empty() || index > (int)mTextures.size()-1)
		return -1;

	t = mTextures[index];

	// Init
	*image = 0x0;
	*depth = 0x0;
	*type = 0x0;
	*w = 0x0;
	*h = 0x0;

	if (!t)
		return -1;

	if (t->mImage && t->mWidth && t->mHeight && t->mBitDepth)
	{
		*image = t->mImage;
		*depth = t->mBitDepth;
		*type = t->mType;
		*w = t->mWidth;
		*h = t->mHeight;

		return 0;
	}
  
	return -1;
}


long EggPlugin::freyjaTexCoord2f(vec_t s, vec_t t)
{
	return mEgg->addTexel(s, t);
}


long EggPlugin::freyjaVertex3f(vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert;


	vert = mEgg->addVertex(x, y, z);

	if (vert)
	{
		if (mGroup)
		{
			mGroup->vertex.pushBack(vert->id);
		}
		else
		{
#ifdef EGGPLUGIN_WARN_VERTEX_OUTSIDE_GROUP
			freyjaPrintError("freyjaVertex3f: WARNING Vertex[%i] outside GROUP!",
							 vert->id);
#endif
		}

		return vert->id;
	}
	else
		return FREYJA_PLUGIN_ERROR;
}


void EggPlugin::freyjaVertexWeight(long index, vec_t weight, long bone)
{
	egg_vertex_t *vert = mEgg->getVertex(index);
	egg_weight_t *vWeight;
	int emptySlot = -1;
	vec_t total = 0.0f;
	unsigned int i;


	if (!vert)
		return;

	for (i = vert->weights.begin(); i < vert->weights.end(); ++i)
	{
		vWeight = vert->weights[i];

		if (vWeight)
		{
			if (weight <= 0.0) // Remove weight
			{
				delete vWeight;
				vert->weights.assign(i, 0x0);
				return;
			}

			if ((int)vWeight->bone == bone) // Alter weight
			{
				vWeight->weight = weight;
				return;
			}

			total = vWeight->weight;
		}
		else
		{
			emptySlot = i;
		}
	}

	if (weight <= 0.0) // Don't add dead weight ( remove requested )
		return;

	if (total + weight > 1.0)  // Just give a warning for now
		freyjaPrintError("WARNING: Weight overflow not handled here %s:%d\n", 
					  __FILE__, __LINE__);

	vWeight = new egg_weight_t;
	vWeight->weight = weight;
	vWeight->bone = bone;

	if (emptySlot > -1)
	{
		vert->weights.assign(emptySlot, vWeight);
	}
	else
	{
		vert->weights.pushBack(vWeight);
	}

	return;
}


void EggPlugin::freyjaVertexTexCoord2f(long index, vec_t u, vec_t v)
{
	egg_vertex_t *vert;

  
	vert = mEgg->getVertex(index);

	if (!vert)
		return;

	vert->uv[0] = u;
	vert->uv[1] = v;
}


void EggPlugin::freyjaVertexNormal3f(long index, vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert;


	vert = mEgg->getVertex(index);

	if (!vert)
		return;

	vert->norm[0] = x;
	vert->norm[1] = y;
	vert->norm[2] = z;
}


void EggPlugin::freyjaPolygonVertex1i(long index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mVertexList.pushBack(index);
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaVertex1i> Vertex defined outside POLYGON!\n");
	}
}


void EggPlugin::freyjaMeshFlags1u(unsigned int flags)
{
	if (mStack.peek() == FREYJA_MESH || !mMesh)
	{
		mMesh->flags = flags;
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaMeshFlags1u> Flag defined outside MESH!\n");
	}
}


void EggPlugin::freyjaPolygonTexCoord1i(long index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTexCoordList.pushBack(index);
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaTexCoord1i> Texel defined outside POLYGON!\n");
	}
}


void EggPlugin::freyjaPolygonMaterial1i(long index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTextureId = index;
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaTexture1i> Texture defined outside POLYGON!\n");
	}
}


unsigned int EggPlugin::freyjaFlags()
{
	return mFlags;
}


void EggPlugin::freyjaFlags(unsigned int set)
{
	mFlags = set;
}


void EggPlugin::freyjaGroupCenter(vec_t x, vec_t y, vec_t z)
{
	if (mStack.peek() == FREYJA_VERTEX_GROUP)
	{
		if (!mGroup)
		{
			freyjaPrintError("EggPlugin::freyjaGroupCenter> GROUP isn't allocated!\n");
			return;// FREYJA_PLUGIN_ERROR;
		}
		else 
		{
			mGroup->center[0] = x;
			mGroup->center[1] = y;
			mGroup->center[2] = z;
		}
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaGroupCenter> Center defined outside GROUP!\n");
		return;// FREYJA_PLUGIN_ERROR;
	}

	//return 0;
}


void EggPlugin::freyjaBoneName(char *name)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBonePos> BONEMTAG isn't allocated!\n");
			return;// FREYJA_PLUGIN_ERROR;
		}
		else 
		{
			strncpy(mTag->name, name, 64);
			mTag->name[63] = 0;
		}
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaBonePos> Pos defined outside BONEMTAG!\n");
		return;// FREYJA_PLUGIN_ERROR;
	}

	//return 0;
}


void EggPlugin::freyjaBonePos(float x, float y, float z)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBonePos> BONEMTAG isn't allocated!\n");
			return;// FREYJA_PLUGIN_ERROR;
		}
		else 
		{
			mTag->center[0] = x;
			mTag->center[1] = y;
			mTag->center[2] = z;
		}
	}
	else if (mTag) // HACK
	{
		mTag->center[0] = x;
		mTag->center[1] = y;
		mTag->center[2] = z;
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaBonePos> Pos defined outside BONEMTAG!\n");
		return;// FREYJA_PLUGIN_ERROR;
	}

	//return 0;
}


void EggPlugin::freyjaBoneFlags(unsigned int flags)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBoneFlags> BONEMTAG isn't allocated!\n");
			return;// FREYJA_PLUGIN_ERROR;
		}
		else 
		{
			mTag->flag = flags;
		}
	}
	else if (mTag) // HACK
	{
		mTag->flag = flags;
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaBoneFlags> Flag defined outside BONEMTAG!\n");
		return;// FREYJA_PLUGIN_ERROR;
	}

	//return 0;
}


void EggPlugin::freyjaBoneAddMesh(long mesh)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError( 
					"EggPlugin::freyjaBoneAddMesh> BONEMTAG isn't allocated!\n");
			return;//  FREYJA_PLUGIN_ERROR;
		}
		else 
		{
			mTag->mesh.pushBack(mesh);
		}
	}
	else if (mTag) // HACK
	{
		mTag->mesh.pushBack(mesh);
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaBoneAddMesh> Mesh defined outside BONEMTAG!\n");
		return;//  FREYJA_PLUGIN_ERROR;
	}

	//return 0;
}


void EggPlugin::freyjaBoneAddChild(long tag)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBoneAddSlave> BONEMTAG isn't allocated!\n");
      
			return;// FREYJA_PLUGIN_ERROR;
		}
		else 
		{
			egg_tag_t *child =  mEgg->getTag(tag);
			
			if (child)
				child->parent = mTag->id;

			// If it fails here it's got to be picked up in SKELETON 

			mTag->slave.pushBack(tag);
		}
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaBoneAddSlave> Slave defined outside BONEMTAG!\n");

		return;// FREYJA_PLUGIN_ERROR;
	}

	//return 0;
}


void EggPlugin::freyjaMeshTreeAddFrame(vec_t x, vec_t y, vec_t z)
{
	if (mAnimation)
	{
		mBoneFrame = mEgg->BoneFrame(mEgg->BoneFrameAdd(x, y, z));    
		mAnimation->frame.pushBack(mBoneFrame->id);
   
		//return 0;
	}

	//return FREYJA_PLUGIN_ERROR;
}


void EggPlugin::freyjaBoneRotate(vec_t x, vec_t y, vec_t z)
{
	if (mTag)
	{
		mTag->rot[0] = x;
		mTag->rot[1] = y;
		mTag->rot[2] = z;
   
		//return 0;
	}

	//return FREYJA_PLUGIN_ERROR;
}


void EggPlugin::freyjaMeshTreeFrameAddBone(long tag)
{
	if (mBoneFrame)
	{
		mBoneFrame->tag.pushBack(tag);
		//return 0;
	}

	//return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaTextureStoreBuffer(unsigned char *image, 
										 unsigned int depth,
										 unsigned int width, 
										 unsigned int height,
										 freyja_colormode_t type)
{
	EggTextureData *t;
	unsigned int size;


	if (!image || !depth || !width || !height)
		return FREYJA_PLUGIN_ERROR;

	size = width*height*depth;

	t = new EggTextureData();
 
	t->mWidth = width;
	t->mHeight = height;
	t->mType = type;
	t->mBitDepth = depth * 8;
	t->mImage = new unsigned char[size];
  
	memcpy(t->mImage, image, size);

	return freyjaTextureStore(t);
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_EGGPLUGIN

int runEggPluginUnitTest(int argc, char *argv[])
{
	EggPlugin ep;

	return 0;
}


int main(int argv, char *argc[])
{
 	printf("[EggPlugin class test]\n");

	return runEggPluginUnitTest(argc, argv); 
  
	return 0;
}

#endif
