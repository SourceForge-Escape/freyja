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
 * 2004.04.27:
 * Mongoose - Created, port and fix of Load 3ds by Alexi Leontopolis, WVU VEL
 ==========================================================================*/

#include "3ds.h"

//Constructor with no file name (produces and error)
File3ds::File3ds ()
{
	cerr << "Error:  Filename must be supplied";
	exit(1);
}

//Constructor with file name.  Loads the file given.
File3ds::File3ds(char *fname) 
{
	//Set up object lists.
	headObject = tailObject = object = NULL;
	headMaterial = tailMaterial = material = NULL;
	nObjects = 0;
	nMaterials = 0;

	//open the file.
	fin.open(fname, ios::in | ios::binary); // ios::nocreate | 
	if (fin.fail())
	{
		cerr << "Can't open " << fname << " file for input:\n";
		exit(1);
	}

	//call Parse to retrieve all the information.
	Parse3dsMain();
	
	//Close the file.
	fin.close();
}
//**********************************

// File3ds::Destructors ************

File3ds::~File3ds() 
{
	if (headMaterial) DeleteMaterial	(headMaterial);
	if (headObject) DeleteObject  		(headObject);
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
	streampos	end, curr;


	//  Reads a word -- Presumably the first Chunk ID 
	fin.read((char *) &id_chunk, sizeof id_chunk);

	//  Check to see if this is a proper 3DS file
	if (id_chunk == MAIN_CHUNK){
		fin.read((char *) &fsize, sizeof fsize); 
		header = sizeof ( word ) + sizeof ( long );

		end	 = fin.tellg() + fsize - header;		//	Sets the end of this Block
													//	at the current position plus
													//	the length of the block minus
													//	the size of the chunk id and
													//	size of chunk variables that
													//	have already been read.

		do
		{ 
			//  Read Subchunk ID inside the Main Chunk	
			fin.read((char *) &id_chunk, sizeof id_chunk);
			switch(id_chunk)
			{
			case VERSION_3DS:
				fin.read((char *) &len,	sizeof len);
				fin.read((char *) &version,	sizeof version );
				break;
			case EDITOR:
				fin.read((char *) &len, sizeof len);
				Parse3dsEditor(len);
				break;
			default:
				fin.read((char *) &len,	sizeof len);
				fin.seekg(curr + len);
				break;
			}
			curr = fin.tellg();
		} while(end != curr);
	}
	else
	{
		cerr << "Error: Not 3DS file";
		exit(1);
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
	streampos	end, curr;

	header = sizeof ( word ) + sizeof ( long );
	end	 = fin.tellg() + len - header;		//	Sets the end of this Block
											//	at the current position plus
											//	the length of the block minus										//	the size of the chunk id and
											//	size of chunk variables that
											//	have already been read.
	do
	{
		int x;
		fin.read((char *) &id_chunk, sizeof id_chunk);
		switch(id_chunk)
		{
		case OBJECT_BLOCK:
			NewObject();
			fin.read((char *) &sub_len, sizeof sub_len);
   			ParseObjectBlock(sub_len);
			for(x = 0; x < object->nFaces; x++)
			{
				object->face[x].material = getMaterial(object->face[x].matName);
			}
			break;
		case MESH_VERSION:
			fin.read((char *) &sub_len, sizeof sub_len);
			fin.read((char *) &mversion, sizeof mversion);
			break;
		case ONE_UNIT:
			fin.read((char *) &sub_len, sizeof sub_len);
			fin.read((char *) &fdata, sizeof fdata);
			break;
		case MATERIAL_EDITOR:
			fin.read((char *) &len, sizeof len);
			NewMaterial();
			ParseMaterialBlock(len);
			break;
		default:
			fin.read((char *) &sub_len,	sizeof sub_len);
			fin.seekg(curr + sub_len);
			break;
		}
		curr = fin.tellg();
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
	streampos end, curr;

	
	header = sizeof ( word ) + sizeof ( long );
	end	 = fin.tellg() + len - header;				//	Sets the end of this Block
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
		fin.read(&c, sizeof c);
		*strpos = c;
		strpos++;
	} 

	curr = fin.tellg();

	do
	{
		fin.read((char *) &id_chunk, sizeof id_chunk);
	
		switch(id_chunk)
		{
		case TRIANGLE_MESH:
			fin.read((char *) &sub_len,		 sizeof sub_len);
			object->ParseMeshBlock(sub_len, &fin);
			break;
		default:
			fin.read((char *) &sub_len,	sizeof sub_len);
			fin.seekg(curr + sub_len);
			curr = fin.tellg();
			break;
		}
		curr = fin.tellg();
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

	streampos	end, curr;

	header = sizeof ( word ) + sizeof ( long );
	end	 = fin.tellg() + len - header;				//	Sets the end of this Block
													//	at the current position plus
													//	the length of the block minus
													//	the size of the chunk id and
													//	size of chunk variables that
													//	have already been read.
	curr = fin.tellg();
	if (*material->texture1.filename)
		memset((void *) material->texture1.filename, '\0', sizeof MAXLEN);
	do
	{
		fin.read((char *) &id_chunk, sizeof id_chunk);
		switch(id_chunk)
		{
		case MATERIAL_NAME:
			fin.read((char *) &sub_len, sizeof sub_len);
			strpos = material->name;
			*strpos = '\0';
			c = 1;
			while (c != 0)
			{
				fin.read(&c, sizeof c);
				*strpos = c;
				strpos++;
			} 
			break;
		case MATERIAL_AMBIENT:
			fin.read((char *) &sub_len,			  sizeof sub_len);
			fin.read((char *) &sub_id,			  sizeof sub_id);
			fin.read((char *) &sub_sub_len,		  sizeof sub_sub_len);
			fin.read((char *) &material->ambient, sizeof (RGBByte));
			break;
		case MATERIAL_DIFFUSE:
			fin.read((char *) &sub_len,		sizeof sub_len);
			fin.read((char *) &sub_id,		sizeof sub_id);
			fin.read((char *) &sub_sub_len, sizeof sub_sub_len);
			fin.read((char *) &material->diffuse, sizeof (RGBByte));
			break;
		case MATERIAL_SPECULAR:
			fin.read((char *) &sub_len,		sizeof sub_len);
			fin.read((char *) &sub_id,		sizeof sub_id);
			fin.read((char *) &sub_sub_len,	sizeof sub_sub_len);
			fin.read((char *) &material->specular, sizeof (RGBByte));
			break;
		case MATERIAL_SHINE_PER:
			fin.read((char *) &sub_len,		sizeof sub_len);
			fin.read((char *) &sub_id,		sizeof sub_id);
			fin.read((char *) &sub_sub_len,	sizeof sub_sub_len);
			fin.read((char *) &material->MatShinPer, sizeof (word));
			break;
		case MATERIAL_SHINE_STR:
			fin.read((char *) &sub_len,		sizeof sub_len);
			fin.read((char *) &sub_id,		sizeof sub_id);
			fin.read((char *) &sub_sub_len,	sizeof sub_sub_len);
			fin.read((char *) &material->MatShinStr,	sizeof (word));
			break;
		case MATERIAL_TRANS_PER:
			fin.read((char *) &sub_len,		sizeof sub_len);
			fin.read((char *) &sub_id,		sizeof sub_id);	
			fin.read((char *) &sub_sub_len,	sizeof sub_sub_len);
			fin.read((char *) &material->MatTransPer, sizeof (word));
			break;
		case MATERIAL_TEXTURE1:
			fin.read((char *) &sub_len,		sizeof sub_len);
			curr = fin.tellg();
			ParseSubMapBlock(sub_len, &material->texture1);
			break;
		default:
			fin.read((char *) &sub_len,	sizeof sub_len);
			fin.seekg(curr + sub_len);
			break;
		}
		curr = fin.tellg();
	} while(end != curr);
}
//************************************


// File3ds::ParseSubMapBlock *********
void File3ds::ParseSubMapBlock  (long len, SubMap *sm)
{
	word		id_chunk;
	long		sub_len;
	long		header;
	char		*strpos, c;
	streampos	end, curr;

	header = sizeof ( word ) + sizeof ( long );
	end	 = fin.tellg() + len - header;				//	Sets the end of this Block
													//	at the current position plus
													//	the length of the block minus
													//	the size of the chunk id and
													//	size of chunk variables that
													//	have already been read.
	sm->UScale = 1.0;
	sm->VScale = 1.0;

	curr = fin.tellg();
	do
	{
		fin.read((char *) &id_chunk, sizeof id_chunk);

		switch(id_chunk)
		{
		case MAPPING_FILENAME:
			fin.read((char *) &sub_len, sizeof sub_len);
			strpos = sm->filename;
			*strpos = '\0';
			c = 1;
			while (c != 0)
			{
				fin.read(&c, sizeof c);
				*strpos = c;
				strpos++;
			} 

			break;
		case U_SCALE:
			fin.read((char *) &sub_len, sizeof sub_len);
			fin.read((char *) &sm->UScale,  sizeof (float));
			break;
		case V_SCALE:
			fin.read((char *) &sub_len, sizeof sub_len);
			fin.read((char *) &sm->VScale,  sizeof (float));
			break;
		default:
			fin.read((char *) &sub_len,	sizeof sub_len);
			fin.seekg(curr + sub_len);
			break;
		}
		curr = fin.tellg();
	} while(end != curr);
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
void Object3d::ParseMeshBlock(long len, ifstream *fin)
{
	word	id_chunk;
	long	sub_len;
	long	header;
	int		x;
	

	streampos end, curr;
	
	header = sizeof ( word ) + sizeof ( long );
	end	 = fin->tellg() + len - header;				//	Sets the end of this Block
													//	at the current position plus
													//	the length of the block minus
													//	the size of the chunk id and
													//	size of chunk variables that
													//	have already been read.
	
	do
	{
		int i;
		word nVertTemp;								// A dummy value since number of vertices has
													// already been stored
		fin->read((char *) &id_chunk, sizeof id_chunk);
		switch(id_chunk)
		{
		case VERTEX_LIST:
			fin->read((char *) &sub_len,	sizeof sub_len);
			fin->read((char *) &nVertices,	sizeof nVertices);
			vertex = new XYZ3DS[nVertices];
			for (x = 0; x < nVertices; x++)
			{
				fin->read((char *) &vertex[x], sizeof( XYZ3DS));
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
			fin->read((char *) &sub_len,	sizeof sub_len);
			fin->read((char *) &nFaces,		sizeof nFaces);
			face = new Face3DS[nFaces];
			for (x = 0; x < nFaces; x++)
			{
				fin->read((char *) &face[x], (sizeof (word)) * 4);
			}
			break;
		case FACE_MAT_LIST:
			word nEntries;
			char	*strpos, tmpstr[MAXLEN], c;	

			fin->read((char *) &sub_len,	sizeof sub_len);
			strpos = tmpstr;
			*strpos = '\0';
			c = 1;
			while (c != 0)
			{ 
				curr = fin->tellg();
				fin->read(&c, sizeof c);
				*strpos = c;
				strpos++;
			} 
			fin->read((char *) &nEntries, sizeof (word));
			for (i = 0; i < nEntries; i++)
			{
				word faceIndex;
				fin->read((char *) &faceIndex, sizeof (word));
				strcpy(face[faceIndex].matName, tmpstr);
			}
			break;
		case VERTEX_MAP_COORD:
			curr = fin->tellg();
			fin->read((char *) &sub_len,	sizeof sub_len);
			curr = fin->tellg();
			fin->read((char *) &nVertTemp,	sizeof (word));
			curr = fin->tellg();
			matmap = new MatMap[nVertTemp];
			for (i = 0; i < nVertTemp; i++)
			{
				fin->read((char *) &matmap[i],	sizeof (MatMap)); 
			}
	
			break;
		default:
			fin->read((char *) &sub_len,	sizeof sub_len);
			fin->seekg(curr + sub_len);
			break;
		}
		curr = fin->tellg();
	} while(end != curr);
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



#ifdef FREYJA_MODEL_PLUGINS

#include <freyja8/EggPlugin.h>
#include <stdio.h>


/* Export as C functions */
extern "C" {

	int freyja_model__3ds_check(char *filename);
	int freyja_model__3ds_import(char *filename);
	int freyja_model__3ds_export(char *filename);
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
	unsigned int i, t, v, o;

	
	if (freyja_model__3ds_check(filename) < 0)
	{
		fprintf(stderr, "freyja_model__3ds_import> Failed to load file\n");
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
		

		// Start a new mesh
		eggBegin(FREYJA_MESH);
		
		// Start a new vertex group
		eggBegin(FREYJA_GROUP);  
		
		// Clear for each group
		trans.Clear();

		for (i = 0; i < vertCount; i++) 
		{
			// Store vertices in group
			v = eggVertexStore3f(verts[i].x, verts[i].z, -verts[i].y); // XZY
			
			// Generates id translator list
			trans.Add(i, v);
		}
		
		eggEnd(); // GROUP
		
		for (i = 0; i < faceCount; i++) 
		{
			// Start a new polygon
			eggBegin(FREYJA_POLYGON);
			
			// Store vertices by true id, using translator list
			eggVertex1i(trans[faces[i].vcA]);
			eggVertex1i(trans[faces[i].vcB]);
			eggVertex1i(trans[faces[i].vcC]);

			// Store texels by true id, using translator list
			if (uvs)
			{
				t = eggTexCoordStore2f(uvs[faces[i].vcA].U, uvs[faces[i].vcA].V);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(uvs[faces[i].vcB].U, uvs[faces[i].vcB].V);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(uvs[faces[i].vcC].U, uvs[faces[i].vcC].V);
				eggTexCoord1i(t);				
			}
			else
			{
				t = eggTexCoordStore2f(0.0, 0.5);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(0.5, 0.5);
				eggTexCoord1i(t);
				t = eggTexCoordStore2f(0.0, 0.0);
				eggTexCoord1i(t);
			}

			if (faces[i].material)
			{
				t = eggTextureStoreFilename(faces[i].material->texture1.filename);
				eggTexture1i(t);
			}
			else
			{
				eggTexture1i(0);
			}

			eggEnd(); // polygon
		}

		eggEnd(); // mesh
	}

	return 0;
}


int freyja_model__3ds_export(char *filename)
{
	printf("freyja_model__3ds_export> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);
	
	return -1;
}

#endif
