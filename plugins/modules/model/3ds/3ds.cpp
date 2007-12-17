/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the 3DS class
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2006.11.11:
 * Mongoose - Removed the trash endian unsafe file I/O,
 *            and replaced with mstl readers ( removed lots of annoying crap 
 *            like stdio and exit() calls everywhere also fixed some 'gcc 3.4 workaround' bullshit )
 *
 * 2004.04.27:
 * Mongoose - Created, port and fix of Load 3ds by Alexi Leontopolis, WVU VEL
 ==========================================================================*/


#include "3ds.h"

//Constructor with no file name (produces and error)
File3ds::File3ds() : fin()
{
	MSTL_ASSERTMSG(false, "This class orignally didn't allow this constructor");
}

//Constructor with file name.  Loads the file given.
File3ds::File3ds(char *fname) : fin()
{
	//Set up object lists.
	headObject = tailObject = object = NULL;
	headMaterial = tailMaterial = material = NULL;
	nObjects = 0;
	nMaterials = 0;

	//open the file.  
	if (!fin.Open(fname))
	{
		MSTL_MSG("Can't open %s file for input\n", fname);
	}
	else
	{
		//call Parse to retrieve all the information.
		Parse3dsMain();
	
		//Close the file.
		fin.Close();
	}
}
//**********************************

// File3ds::Destructors ************

File3ds::~File3ds() 
{
	if (headMaterial) DeleteMaterial(headMaterial);
	if (headObject) DeleteObject(headObject);
}
// **********************************

// File3ds::numFaces   **************
//returns the number of faces
word File3ds::numFaces(word object) 
{
	return getObject(object)->nFaces;
}
//***********************************

// File3ds::numVertices *************
//returns the number of vertices
word File3ds::numVertices(word object)
{
	return getObject(object)->nVertices;
}
//***********************************

// File3ds::getOrigin ****************
//returns the origin of the object.
XYZ3DS File3ds::getOrigin (word object)
{
	return getObject(object)->O;
}
//************************************

// File3ds::getMidpoint **************
//returns the midpoint of an object.
XYZ3DS File3ds::getMidpoint(word object) 
{
	
	XYZ3DS min, max, midpoint;

	min = getObject(object)->min;
	max = getObject(object)->max;

	midpoint.x = (max.x + min.x)/2.0f;
	midpoint.y = (max.y + min.y)/2.0f;
	midpoint.z = (max.z + min.z)/2.0f;

	return midpoint;
}
//************************************

// File3ds::getName ******************
//returns the name of an object.
char* File3ds::getName(word object)
{
	
	return getObject(object)->name;
}
//************************************

// File3ds::getFaceHead **************
//returns the head of a face (for polygons)
Face3DS* File3ds::getFaceHead(word object)
{
	return getObject(object)->face;
}
//************************************

// File3ds::getVertexHead ************
//returns the vertex head
XYZ3DS*	File3ds::getVertexHead(word object) 
{
	return getObject(object)->vertex;
}
//************************************

// File3ds::getMatMap ****************
//returns the material map of the object.
MatMap* File3ds::getMatMap(word object)
{
	return getObject(object)->matmap;
}	
//************************************

// File3ds::getObject  **************
//returns an entire object.
Object3d* File3ds::getObject(word index)
{
	object = headObject;
	for (int x = 0; x < index; x++)
	{
		object = object->next;
	}
	return object;
}
//************************************

// File3ds::getMaterial **************
// returns an entire material.
Material* File3ds::getMaterial(char *name)
{
	Material *material;
	material = headMaterial;
	while (material!=NULL && (strcmp(material->name, name) != 0))
	{
		material = material->next;
	}
	return material;
}
//************************************************************************




//*****************  File3ds Class Private Members ***********************

// File3ds::Parse3ds *****************
// parses a 3ds file, retrieving all information stored within.
void File3ds::Parse3dsMain()
{

	//setup
	word		id_chunk;
	long		len;
	long		header;
	long	end, curr, tmp;


	//  Reads a word -- Presumably the first Chunk ID 
	id_chunk = fin.ReadInt16U();

	//  Check to see if this is a proper 3DS file
	if (id_chunk == MAIN_CHUNK)
	{
		fsize = fin.ReadLong();
		header = 6;//sizeof ( word ) + sizeof ( long );

		end = fin.GetOffset() + fsize - header;
		// I left this here at a warning to the next person! work around? =)
		//end	 = fin.GetOffset()(); 
		//end += (fsize - header);// gcc 3.4 work around
								//	Sets the end of this Block
								//	at the current position plus
								//	the length of the block minus
								//	the size of the chunk id and
								//	size of chunk variables that
								//	have already been read.

		do
		{ 
			//  Read Subchunk ID inside the Main Chunk	
			id_chunk = fin.ReadInt16U();
			switch(id_chunk)
			{
			case VERSION_3DS:
				len = fin.ReadLong();
				version = fin.ReadLongU();
				break;
			case EDITOR:
				len = fin.ReadLong();
				Parse3dsEditor(len);
				break;
			default:
				len = fin.ReadLong();
				tmp = curr; tmp += len; // gcc 3.4 work around
				fin.SetOffset(tmp);
				break;
			}
			curr = fin.GetOffset();
		} while(end != curr);
	}
	else
	{
		MSTL_MSG("Error: Not 3DS file");
		//exit(1);
	}
}
//************************************

// File3ds::Parse3dsEditor ***********
void File3ds::Parse3dsEditor  (long len)
{
	word		id_chunk;
	long		sub_len;
	float		fdata;
	long		header;
	long	end, curr, tmp;

	header = 6;//sizeof ( word ) + sizeof ( long );
	end	 = fin.GetOffset(); end += len - header;  // gcc 3.4 work around
											//	Sets the end of this Block
											//	at the current position plus
											//	the length of the block minus										//	the size of the chunk id and
											//	size of chunk variables that
											//	have already been read.
	do
	{
		int x;
		id_chunk = fin.ReadInt16U();
		switch(id_chunk)
		{
		case OBJECT_BLOCK:
			NewObject();
			sub_len = fin.ReadLong();
   			ParseObjectBlock(sub_len);
			for(x = 0; x < object->nFaces; x++)
			{
				object->face[x].material = getMaterial(object->face[x].matName);
			}
			break;
		case MESH_VERSION:
			sub_len = fin.ReadLong();
			mversion = fin.ReadLongU();
			break;
		case ONE_UNIT:
			sub_len = fin.ReadLong();
			fdata = fin.ReadFloat32(); 
			break;
		case MATERIAL_EDITOR:
			len = fin.ReadLong();
			NewMaterial();
			ParseMaterialBlock(len);
			break;
		default:
			sub_len = fin.ReadLong();
			tmp = curr; tmp += sub_len;  // gcc 3.4 work around
			fin.SetOffset(tmp);
			break;
		}
		curr = fin.GetOffset();
	} while(end != curr);
}
//************************************

// File3ds::ParseObjectBlock *********
void File3ds::ParseObjectBlock(long len)
{
	word	id_chunk;
	long	sub_len;
	long	header; 
	char	*strpos, c;
	long end, curr, tmp;

	
	header = 6;//sizeof ( word ) + sizeof ( long );
	end	 = fin.GetOffset(); end += len - header;  // gcc 3.4 work around
													//	Sets the end of this Block
													//	at the current position plus
													//	the length of the block minus
													//	the size of the chunk id and
													//	size of chunk variables that
													//	have already been read.
	
	strpos = object->name;
	*strpos = '\0';
	c = 1;
	
	while (c != 0)
	{
		c = fin.ReadByte();
		*strpos = c;
		strpos++;
	} 

	curr = fin.GetOffset();

	do
	{
		id_chunk = fin.ReadInt16U();
	
		switch(id_chunk)
		{
		case TRIANGLE_MESH:
			sub_len = fin.ReadLong();
			object->ParseMeshBlock(sub_len, fin);
			break;
		default:
			sub_len = fin.ReadLong();
			tmp = curr; tmp += sub_len;  // gcc 3.4 work around
			fin.SetOffset(tmp);
			curr = fin.GetOffset();
			break;
		}
		curr = fin.GetOffset();
	} while(end != curr);
}
//************************************

// File3ds::ParseMaterialBlock *******
void File3ds::ParseMaterialBlock  (long len)
{
	word		id_chunk;
	long		sub_len;
	long		sub_sub_len;
	word		sub_id;
	long		header;
	char		*strpos, c;

	long	end, curr, tmp;

	header = sizeof ( word ) + sizeof ( long );
	end	 = fin.GetOffset(); end += len - header; // gcc 3.4 work around
				//	Sets the end of this Block
													//	at the current position plus
													//	the length of the block minus
													//	the size of the chunk id and
													//	size of chunk variables that
													//	have already been read.
	curr = fin.GetOffset();
	if (*material->texture1.filename)
		memset((void *) material->texture1.filename, '\0', sizeof MAXLEN);
	do
	{
		id_chunk = fin.ReadInt16U();
		switch(id_chunk)
		{
		case MATERIAL_NAME:
			sub_len = fin.ReadLong();
			strpos = material->name;
			*strpos = '\0';
			c = 1;
			while (c != 0)
			{
				c = fin.ReadByte();
				*strpos = c;
				strpos++;
			} 
			break;

		case MATERIAL_AMBIENT:
			sub_len = fin.ReadLong();
			sub_id = fin.ReadInt16U();
			sub_sub_len = fin.ReadLong();
			material->ambient.r = fin.ReadByte();
			material->ambient.g = fin.ReadByte();
			material->ambient.b = fin.ReadByte();
			break;

		case MATERIAL_DIFFUSE:
			sub_len = fin.ReadLong();
			sub_id = fin.ReadInt16U();
			sub_sub_len = fin.ReadLong();
			material->diffuse.r = fin.ReadByte();
			material->diffuse.g = fin.ReadByte();
			material->diffuse.b = fin.ReadByte();
			break;

		case MATERIAL_SPECULAR:
			sub_len = fin.ReadLong();
			sub_id = fin.ReadInt16U();
			sub_sub_len = fin.ReadLong();
			material->specular.r = fin.ReadByte();
			material->specular.g = fin.ReadByte();
			material->specular.b = fin.ReadByte();
			break;

		case MATERIAL_SHINE_PER:
			sub_len = fin.ReadLong();
			sub_id = fin.ReadInt16U();
			sub_sub_len = fin.ReadLong();
			material->MatShinPer = fin.ReadInt16U();
			break;

		case MATERIAL_SHINE_STR:
			sub_len = fin.ReadLong();
			sub_id = fin.ReadInt16U();
			sub_sub_len = fin.ReadLong();
			material->MatShinStr = fin.ReadInt16U();
			break;

		case MATERIAL_TRANS_PER:
			sub_len = fin.ReadLong();
			sub_id = fin.ReadInt16U();
			sub_sub_len = fin.ReadLong();
			material->MatTransPer = fin.ReadInt16U();
			break;

		case MATERIAL_TEXTURE1:
			sub_len = fin.ReadLong();
			curr = fin.GetOffset();
			ParseSubMapBlock(sub_len, &material->texture1);
			break;

		default:
			sub_len = fin.ReadLong();
			 tmp = curr; tmp += sub_len;// gcc 3.4 work around
			fin.SetOffset(tmp);
			break;
		}
		curr = fin.GetOffset();
	} while(end != curr && curr < end);
}
//************************************


// File3ds::ParseSubMapBlock *********
void File3ds::ParseSubMapBlock  (long len, SubMap *sm)
{
	word		id_chunk;
	long		sub_len;
	long		header;
	char		*strpos, c;
	long	end, curr, tmp;

	header = sizeof ( word ) + sizeof ( long );
	end	 = fin.GetOffset(); end += len - header; // gcc 3.4 work around
				//	Sets the end of this Block
													//	at the current position plus
													//	the length of the block minus
													//	the size of the chunk id and
													//	size of chunk variables that
													//	have already been read.
	sm->UScale = 1.0;
	sm->VScale = 1.0;

	curr = fin.GetOffset();
	do
	{
		id_chunk = fin.ReadInt16U();

		switch(id_chunk)
		{
		case MAPPING_FILENAME:
			sub_len = fin.ReadLong();
			strpos = sm->filename;
			*strpos = '\0';
			c = 1;
			while (c != 0)
			{
				c = fin.ReadByte();
				*strpos = c;
				strpos++;
			} 
			break;

		case U_SCALE:
			sub_len = fin.ReadLong();
			sm->UScale = fin.ReadFloat32();
			break;

		case V_SCALE:
			sub_len = fin.ReadLong();
			sm->VScale = fin.ReadFloat32();
			break;

		default:
			sub_len = fin.ReadLong();

			if (!sub_len)
				continue;

			tmp = curr; tmp += sub_len;  // gcc 3.4 work around
			fin.SetOffset(tmp);
			break;
		}
		curr = fin.GetOffset();
	} while(end != curr && curr < end); // FIX for unknown 3ds materials
}
//************************************

// File3ds::NewObject ****************
void File3ds::NewObject()
{
	if (headObject == NULL) 
	{
		object = new Object3d;
		headObject = tailObject = object;
	}
	else
	{
		object->next = new Object3d;
		tailObject = object = object->next;
	}
	nObjects++;
}
//************************************

// File3ds::NewMaterial **************
void File3ds::NewMaterial()
{
	if (headMaterial == NULL) 
	{
		material = new Material;
		headMaterial = tailMaterial = material;
	}
	else
	{
		material->next = new Material;
		tailMaterial = material = material->next;
	}
	nMaterials++;
}
//************************************

// File3ds::DeleteMaterial ***********
void File3ds::DeleteMaterial(Material *material)
{
	if (material->next)
	{
		DeleteMaterial(material->next);
	}
	delete material;
}
//************************************

// File3ds::DeleteObject *************

void File3ds::DeleteObject(Object3d *object)
{
	if (object->next)
	{
		DeleteObject(object->next);
	}
	delete object;
}
//************************************


// Object3d Constructor **************
Object3d::Object3d ()
{
	next		=NULL;
	nFaces		=0;
	nVertices	=0;
	XYZ3DSInit(&X1);
	XYZ3DSInit(&X2);
	XYZ3DSInit(&X3);
	XYZ3DSInit(&O);
	vertex  = NULL;
	face	= NULL;
	matmap	= NULL;

}
//************************************


// Object3d::ParseMeshBlock **********
void Object3d::ParseMeshBlock(long len, mstl::SystemIO::FileReader &fin)
{
	word id_chunk;
	long sub_len;
	long header;
	int x;
	long end, curr, tmp;
	
	header = 2 + 4;
	end	= fin.GetOffset() + len - header;
	curr = fin.GetOffset();

	// gcc 3.4 work around
	//	Sets the end of this Block
	//	at the current position plus
	//	the length of the block minus
	//	the size of the chunk id and
	//	size of chunk variables that
	//	have already been read.

	do
	{
		int i;
		word nVertTemp; // dummy value since vertex count has already been stored
		id_chunk = fin.ReadInt16U();
		switch(id_chunk)
		{
		case VERTEX_LIST:
			sub_len = fin.ReadLong();
			nVertices = fin.ReadInt16U();
			vertex = new XYZ3DS[nVertices];
			for (x = 0; x < nVertices; x++)
			{
				vertex[x].x = fin.ReadFloat32();
				vertex[x].y = fin.ReadFloat32();
				vertex[x].z = fin.ReadFloat32();

				if (x) 
				{
					if (vertex[x].x > max.x)
						max.x = vertex[x].x;
					if (vertex[x].y > max.y)
						max.y = vertex[x].y;
					if (vertex[x].z > max.z)
						max.z = vertex[x].z;
					if (vertex[x].x > min.x)
						min.x = vertex[x].x;
					if (vertex[x].y > min.y)
						min.y = vertex[x].y;
					if (vertex[x].z > min.z)
						min.z = vertex[x].z;
				}
					
			}
			break;

		case FACE_LIST:
			sub_len = fin.ReadLong();
			nFaces = fin.ReadInt16U();

			face = new Face3DS[nFaces];
			for (x = 0; x < nFaces; x++)
			{
				face[x].vcA = fin.ReadInt16U();
				face[x].vcB = fin.ReadInt16U();
				face[x].vcC = fin.ReadInt16U();
				face[x].fFlag = fin.ReadInt16U();
			}
			break;

		case FACE_MAT_LIST:
			word nEntries;
			char	*strpos, tmpstr[MAXLEN], c;	

			sub_len = fin.ReadLong();
			strpos = tmpstr;
			*strpos = '\0';
			c = 1;
			while (c != 0)
			{ 
				curr = fin.GetOffset();
				c = fin.ReadByte();
				*strpos = c;
				strpos++;
			} 
			nEntries = fin.ReadInt16U();
			for (i = 0; i < nEntries; i++)
			{
				word faceIndex;
				faceIndex = fin.ReadInt16U();
				strcpy(face[faceIndex].matName, tmpstr);
			}
			break;

		case VERTEX_MAP_COORD:
			curr = fin.GetOffset();
			sub_len = fin.ReadLong();
			curr = fin.GetOffset();
			nVertTemp = fin.ReadInt16U();
			curr = fin.GetOffset();
			matmap = new MatMap[nVertTemp];
			for (i = 0; i < nVertTemp; i++)
			{
				matmap[i].U = fin.ReadFloat32();
				matmap[i].V = fin.ReadFloat32();
			}
	
			break;

		default:
			sub_len = fin.ReadLong();
			tmp = curr; tmp += sub_len;  // gcc 3.4 work around
			fin.SetOffset(tmp);
			break;
		}

		curr = fin.GetOffset();

	} while (end != curr && curr < end); // safety feature for bad reads
}
//************************************

// Object3d Destructor **************
Object3d::~Object3d ()
{
	if (vertex)
		delete [] vertex;
	if (face)
		delete [] face;
	if (matmap)
		delete [] matmap;

}
//************************************

// XYZ3DSInit ************************

void XYZ3DSInit(XYZ3DS *v) 
{
	v->x = v->y = v->z = 0.0;
}
//************************************


// Material Constructor **************
Material::Material ()
{
	next		= NULL;
	memset((void *) name,		0, MAXLEN);
	memset((void *) &ambient,	127, sizeof (RGBByte));
	memset((void *) &diffuse,	255, sizeof (RGBByte));
	memset((void *) &specular,	0, sizeof (RGBByte));
	memset((void *) &texture1,	0, sizeof (SubMap));
	MatShinPer = 0;
	MatShinStr = 0;

}
//************************************



#ifdef FREYJA_PLUGINS
#include <freyja/PluginABI.h>
#include <freyja/MeshABI.h>
#include <freyja/ModelABI.h>
#include <freyja/TextureABI.h>
#include <mstl/Map.h>
#include <stdio.h>

using namespace mstl;

/* Export as C functions */
extern "C" {

	int import_model(char *filename);
	int export_model(char *filename);

	int freyja_model__3ds_check(char *filename);
	int freyja_model__3ds_import(char *filename);
	int freyja_model__3ds_export(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginName1s("3ds");
	freyjaPluginDescription1s("3ds model (*.3ds)");
	freyjaPluginAddExtention1s("*.3ds");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}



int import_model(char *filename)
{
	return freyja_model__3ds_import(filename);
}


int export_model(char *filename)
{
	return freyja_model__3ds_export(filename);
}


int freyja_model__3ds_check(char *filename)
{
	char header[8];
	FILE *f;


	f = fopen(filename, "rb");
	
	if (!f)
		return -1;
	
	fread(&header, 8, 1, f);
	fclose(f);
	
	if (header[0] == 'M' && header[1] == 'M')
	{
		return 0;
	}
	else
	{
		return -3;
	}
}


int freyja_model__3ds_import(char *filename)
{
	Map<unsigned int, unsigned int> trans;
	Map<unsigned int, unsigned int> trans2;
	unsigned int i, o;

	if (freyja_model__3ds_check(filename) < 0)
	{
		return -1;
	}

	File3ds tds(filename);

	//word		numFaces		(word object);
	//word		numVertices		(word object);
	//XYZ3DS	getOrigin		(word object);
	//XYZ3DS	getMidpoint		(word object);
	//char* 	getName			(word object);
	//Face3DS*	getFaceHead		(word object);
	//XYZ3DS*	getVertexHead	(word object);
	//MatMap*	getMatMap		(word object);
	
	for (o = 0; o < tds.numObjects(); ++o) 
	{
        XYZ3DS *verts = tds.getVertexHead(o);
		MatMap *uvs = tds.getMatMap(o);
		Face3DS *faces = tds.getFaceHead(o);
		unsigned int vertCount = tds.numVertices(o);
		unsigned int faceCount = tds.numFaces(o);
		
		index_t model = freyjaModelCreate();
		index_t mesh = freyjaMeshCreate();
		freyjaModelAddMesh(model, mesh);

		// Clear for each group
		trans.Clear();

		for (i = 0; i < vertCount; i++) 
		{
			// Store vertices in group
			vec3_t v = {verts[i].x, verts[i].z, -verts[i].y};
			index_t vertex = freyjaMeshVertexCreate3fv(mesh, v);
			
			// Generates id translator list
			trans.Add(i, vertex);
		}
		
		for (i = 0; i < faceCount; i++) 
		{
			index_t face = freyjaMeshPolygonCreate(mesh);

			if (faces[i].material && faces[i].material->texture1.filename &&
			    faces[i].material->texture1.filename[0])
			{
				index_t texture = freyjaTextureCreateFilename(faces[i].material->texture1.filename);
				freyjaMeshPolygonMaterial(mesh, face, texture);
			}
			else
			{
				freyjaMeshPolygonMaterial(mesh, face, 0);
			}
						
			// Store vertices by true id, using translator list
			freyjaMeshPolygonAddVertex1i(mesh, face, trans[faces[i].vcA]);
			freyjaMeshPolygonAddVertex1i(mesh, face, trans[faces[i].vcB]);
			freyjaMeshPolygonAddVertex1i(mesh, face, trans[faces[i].vcC]);

			// Store texels by true id, using translator list
			if (uvs)
			{
				vec_t u = uvs[faces[i].vcA].U;
				vec_t v = uvs[faces[i].vcA].V;
				index_t texcoord = freyjaMeshTexCoordCreate2f(mesh, u, v);
				freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

				u = uvs[faces[i].vcB].U;
				v = uvs[faces[i].vcB].V;
				texcoord = freyjaMeshTexCoordCreate2f(mesh, u, v);
				freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

				u = uvs[faces[i].vcC].U;
				v = uvs[faces[i].vcC].V;
				texcoord = freyjaMeshTexCoordCreate2f(mesh, u, v);
				freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);			
			}

		}

	}

	return 0;
}


int freyja_model__3ds_export(char *filename)
{
	freyjaPrintError("freyja_model__3ds_export> Not implemented, %s:%i\n", 
					 __FILE__, __LINE__);
	
	return -1;
}

#endif
