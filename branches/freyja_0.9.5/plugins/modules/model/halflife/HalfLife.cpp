/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : HalfLife
 * Comments: This is the HalfLife model class.
 *
 *           See file COPYING for license details.
 *
 *           Load based on code from Quest by ?? 
 *
 *           HalfLife (c) 1998, Valve LLC. All rights reserved.            
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2000-07-22:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>

#include "HalfLife.h"

using namespace mstl;


HalfLife::HalfLife()
{
#ifdef BUFFERED_READ
	mHeader = 0x0;
#endif
	mBodyParts = 0x0;
	mModels = 0x0;
	mMeshes = 0x0;
	mBones = 0x0;
	mVertices = 0x0;
	mFaces = 0x0;
	mTextures = 0x0;
	mImages = 0x0;

	mMeshCount = 0;
	mVertexCount = 0;
	mBoneCount = 0;
	mModelCount = 0;
	mBodyPartCount = 0;
	mTextureCount = 0;
}


HalfLife::~HalfLife()
{
	unsigned int i;


	if (mTextures)
		delete [] mTextures;

	if (mImages)
	{
		for (i = 0; i < mTextureCount; ++i)
		{
			if (mImages[i].image)
				delete [] mImages[i].image;
		}

		delete [] mImages;
	}
}


int HalfLife::load(const char *filename)
{
	SystemIO::BufferedFileReader f;
	unsigned int i, j, k, m;

	if (!f.Open(filename))
	{
		printf("HalfLife::load> Couldn't load file.\n");
		perror(filename);
		return -1;
	}

	//fread(&mHeader, sizeof(studiohdr_t), 1, f);
	mHeader.id = f.ReadLong();
	mHeader.version = f.ReadLong();
	f.ReadString(64, mHeader.name);
	mHeader.length = f.ReadLong();
	mHeader.eyeposition[0] = f.ReadFloat32();
	mHeader.eyeposition[1] = f.ReadFloat32();
	mHeader.eyeposition[2] = f.ReadFloat32();
	mHeader.min[0] = f.ReadFloat32();
	mHeader.min[1] = f.ReadFloat32();
	mHeader.min[2] = f.ReadFloat32();
	mHeader.max[0] = f.ReadFloat32();	
	mHeader.max[1]= f.ReadFloat32();	
	mHeader.max[2] = f.ReadFloat32();			
	mHeader.bbmin[0] = f.ReadFloat32();			
	mHeader.bbmin[1] = f.ReadFloat32();			
	mHeader.bbmin[2] = f.ReadFloat32();	
	mHeader.bbmax[0] = f.ReadFloat32();	
	mHeader.bbmax[1] = f.ReadFloat32();	
	mHeader.bbmax[2] = f.ReadFloat32();		
	mHeader.flags = f.ReadLong();
	mHeader.numbones = f.ReadLong();
	mHeader.boneindex = f.ReadLong();
	mHeader.numbonecontrollers = f.ReadLong(); 
	mHeader.bonecontrollerindex = f.ReadLong();
	mHeader.numhitboxes = f.ReadLong();
	mHeader.hitboxindex = f.ReadLong();			
	mHeader.numseq = f.ReadLong();	
	mHeader.seqindex = f.ReadLong();
	mHeader.numseqgroups = f.ReadLong();
	mHeader.seqgroupindex = f.ReadLong();
	mHeader.numtextures = f.ReadLong();
	mHeader.textureindex = f.ReadLong();
	mHeader.texturedataindex = f.ReadLong();
	mHeader.numskinref = f.ReadLong();	
	mHeader.numskinfamilies = f.ReadLong();
	mHeader.skinindex = f.ReadLong();
	mHeader.numbodyparts = f.ReadLong();		
	mHeader.bodypartindex = f.ReadLong();
	mHeader.numattachments = f.ReadLong();
	mHeader.attachmentindex = f.ReadLong();
	mHeader.soundtable = f.ReadLong();
	mHeader.soundindex = f.ReadLong();
	mHeader.soundgroups = f.ReadLong();
	mHeader.soundgroupindex = f.ReadLong();
	mHeader.numtransitions = f.ReadLong();
	mHeader.transitionindex = f.ReadLong();

	if (mHeader.id != 0x54534449)
	{
		printf("Not a valid halflife model\n");
		f.Close();
		return -2;
	}

	printf("Halflife {\n");
	printf(" version = 0x%lx\n", mHeader.id);
	printf(" name = '%s'\n", mHeader.name);

	printf(" eyepos <%g %g %g>\n min <%g %g %g>\n max <%g %g %g>\n",
		   mHeader.eyeposition[0], mHeader.eyeposition[1],
		   mHeader.eyeposition[2],
		   mHeader.min[0], mHeader.min[1], mHeader.min[2],
		   mHeader.max[0], mHeader.max[1], mHeader.max[2]);

	printf(" bbmin <%g %g %g>\n bbmax <%g %g %g>\n",
		   mHeader.bbmin[0], mHeader.bbmin[1], mHeader.bbmin[2],
		   mHeader.bbmax[0], mHeader.bbmax[1], mHeader.bbmax[2]);

	printf(" flags %li\n", mHeader.flags);
	printf(" bones %li\n boneindex %li\n", mHeader.numbones, mHeader.boneindex);
	printf(" numseqgroups %li \n", mHeader.numseqgroups);
	printf(" seqgroupindex %li\n", mHeader.seqgroupindex);
	printf(" bodyparts %li\n bodypartindex %li\n}\n\n",
		   mHeader.numbodyparts, mHeader.bodypartindex);


	/* Load Textures */
	//	if (0) // disabled for now
	if (mHeader.textureindex != 0 && mHeader.numtextures <= MAXSTUDIOSKINS)
	{
		byte palette[768]; // 256 RGB colors
		byte *image = 0x0;
		unsigned int size;
		byte pixel;

		mTextureCount = mHeader.numtextures;
		mTextures = new mstudio_texture_t[mTextureCount];
		mImages = new hl_image_t[mTextureCount];

		//fseek(f, mHeader.textureindex, SEEK_SET);
		f.SetOffset(mHeader.textureindex);
		//fread(mTextures, mTextureCount, sizeof(mstudio_texture_t), f);
		for (i = 0; i < mTextureCount; ++i)
		{
			f.ReadString(64, mTextures[i].name);
			mTextures[i].flags = f.ReadLong();
			mTextures[i].width = f.ReadLong();
			mTextures[i].height = f.ReadLong();
			mTextures[i].index = f.ReadLong();    
		}

		for (i = 0; i < mTextureCount; ++i)
		{
			//fseek(f, mTextures[i].index, SEEK_SET);
			f.SetOffset(mTextures[i].index);

			printf("Texture[%i] %lix%li @ offset = %li\n", 
				   i, 
				   mTextures[i].width, mTextures[i].height,
				   mTextures[i].index);

			mImages[i].w = mTextures[i].width;
			mImages[i].h = mTextures[i].height;
			size = mImages[i].w * mImages[i].h;
			mImages[i].image = new byte[size * 3];

			image = new byte[size];
			//fread(image, size, 1, f);
			//fread(palette, 768, 1, f);
			f.ReadBuffer(size, image);
			f.ReadBuffer(768, palette);

			/* Convert indexed image to RGB image using palette */
			for (j = 0; j < size; ++j)
			{
				pixel = image[j];

				mImages[i].image[j*3 + 0] = palette[pixel*3 + 0];
				mImages[i].image[j*3 + 1] = palette[pixel*3 + 1];
				mImages[i].image[j*3 + 2] = palette[pixel*3 + 2];
			}

			delete [] image;
		}

		printf("\n");
	}

	///////////////////////////////////////////////////////

	/* Load Bodyparts */
	mstudio_bodyparts_t bodyPart;
	mstudio_model_t model;
	mstudio_mesh_t mesh;
	//mstudio_trivert_t vertex;
	unsigned int size;

	mBodyPartCount = mHeader.numbodyparts;
	printf("Reading %li bodyparts...\n", mHeader.numbodyparts);

	mBodyParts = new hl_bodypart_t[mBodyPartCount];

	for (i = 0; i < mBodyPartCount; ++i)
	{
		size = sizeof(mstudio_bodyparts_t);
		//fseek(f, mHeader.bodypartindex + i * size, SEEK_SET);
		f.SetOffset(mHeader.bodypartindex + i * size);

		//fread(&bodyPart, 1, size, f);
		f.ReadString(64, bodyPart.name);
		bodyPart.nummodels = f.ReadLong();
		bodyPart.base = f.ReadLong();
		bodyPart.modelindex = f.ReadLong();

		printf("mBodyParts[%i].name = '%s'\n", i, bodyPart.name);
		printf("mBodyParts[%i].nummodels = %li\n", i, bodyPart.nummodels);

		mBodyParts[i].modelCount = bodyPart.nummodels;
		mBodyParts[i].models = new hl_model_t[bodyPart.nummodels];

		for (m = 0; (int)m < bodyPart.nummodels; ++m)
		{
			//bodyPart.modelindex
			//fseek(f, bodyPart.modelindex+((i/bodyPart.base)%bodyPart.nummodels), SEEK_SET);
			//fseek(f, m*sizeof(mstudio_model_t), SEEK_CUR);
			f.SetOffset(bodyPart.modelindex+((i/bodyPart.base)%bodyPart.nummodels)+m*sizeof(mstudio_model_t));

			//fread(&model, 1, sizeof(mstudio_model_t), f);
			f.ReadString(64, model.name);
			model.type = f.ReadLong();
			model.boundingradius = f.ReadFloat32();
			model.nummesh = f.ReadLong();
	 		model.meshindex = f.ReadLong();
	 		model.numverts = f.ReadLong();        
	 		model.vertinfoindex = f.ReadLong();  
	 		model.vertindex = f.ReadLong();      
	 		model.numnorms = f.ReadLong();       
	 		model.norminfoindex = f.ReadLong();  
			model.normindex = f.ReadLong();      
			model.numgroups = f.ReadLong();    
			model.groupindex = f.ReadLong();
	
			printf("  model.name = '%s'\n", model.name);
			printf("  model.nummesh = %li\n", model.nummesh);
			printf("  model.numverts = %li\n", model.numverts);

			if (model.nummesh < 0 || model.numverts < 0)
			{
				memset(&model, sizeof(mstudio_model_t), 0);
				continue;
			}

			mBodyParts[i].models[m].meshCount = model.nummesh;
			mBodyParts[i].models[m].meshes = new hl_mesh_t[model.nummesh];
			mBodyParts[i].models[m].groupCount = model.numgroups;
			mBodyParts[i].models[m].groups = new hl_group_t[model.numgroups];
			mBodyParts[i].models[m].vertexCount = model.numverts;
			mBodyParts[i].models[m].vertices = new vec3_t[model.numverts];
			mBodyParts[i].models[m].normalCount = model.numnorms;
			mBodyParts[i].models[m].normals = new vec3_t[model.numnorms];

			//fseek(f, model.vertindex, SEEK_SET);
			f.SetOffset(model.vertindex);
			//fread(mBodyParts[i].models[m].vertices, sizeof(vec3_t), model.numverts, f);
			for (j = 0; (int)j < model.numverts; ++j)
				f.ReadFloat32Array(3, mBodyParts[i].models[m].vertices[j]);
			//fseek(f, model.normindex, SEEK_SET);
			f.SetOffset(model.normindex);
			//fread(mBodyParts[i].models[m].normals, sizeof(vec3_t), model.numnorms, f);
			for (j = 0; (int)j < model.numnorms; ++j)
				f.ReadFloat32Array(3, mBodyParts[i].models[m].normals[j]);

			printf("    Reading %li meshes...\n", model.nummesh);

			for (j = 0; (int)j < model.nummesh; ++j)
			{
				size = sizeof(mstudio_mesh_t);
				//fseek(f, model.meshindex + j * size, SEEK_SET);
				f.SetOffset(model.meshindex + j * size);
				//fread(&mesh, size, 1, f);
				mesh.numtris = f.ReadLong();
				mesh.triindex = f.ReadLong();
				mesh.skinref = f.ReadLong();
				mesh.numnorms = f.ReadLong(); 
				mesh.index = f.ReadLong();    

				// Hacky stack space jujitsu to avoid more vectors/arrays
				short numFaces, vertex, norm;
				short st1, st2, st1A, st2A; 
				short centervertex, lastvertex, lastlastvertex;
				short centerst1, lastst1, lastlastst1;
				short centerst2, lastst2, lastlastst2;
				short centernorm, lastnorm, lastlastnorm;
				short normA, vertexA;
				unsigned int t, tn, tri;


				/* Face loader: triangle fans and strips */
				printf("Reading %li faces...\n", mesh.numtris);

				f.SetOffset(mesh.triindex);
				for (k = 0, tri = 0; k < mesh.numtris; ++k)
				{
					numFaces = f.ReadInt16();

					if (!numFaces)
						break;

					tri += (numFaces > 0) ? numFaces : -numFaces;

					for (; numFaces > 0; --numFaces)
					{
						f.ReadInt16(); // vertex
						f.ReadInt16(); // normal
						f.ReadInt16(); // s
						f.ReadInt16(); // t
					}
				}

				printf("Expecting %i actual faces...\n", tri);

				//printf("    mesh[%i].faces = %li...\n", j, mesh.numtris);

				mBodyParts[i].models[m].meshes[j].faceCount = mesh.numtris;
				mBodyParts[i].models[m].meshes[j].faces = new hl_face_t[tri];
				mBodyParts[i].models[m].meshes[j].normalCount = mesh.numnorms;
				mBodyParts[i].models[m].meshes[j].material = mesh.skinref;


				f.SetOffset(mesh.triindex);

				for (k = 0, tri = 0; k < mesh.numtris; ++k)
				{
					numFaces = f.ReadInt16();

					if (!numFaces)
						break;

					tn = (numFaces > 0) ? numFaces : -numFaces;

#define DEBUG_HALFLIFE_FACETS
#ifdef DEBUG_HALFLIFE_FACETS
					printf("# Reading %i %s...\n",
						   tn, 
						   (numFaces > 0) ? "triangle fans" : "triangle strips");
#endif

					for (t = 0; t < tn; ++t)
					{
						vertex= f.ReadInt16();
						norm= f.ReadInt16();
						st1 = f.ReadInt16();
						st2 = f.ReadInt16();

						if (t == 0)
						{
							centervertex = vertex;
							centernorm = norm;
							centerst1 = st1;
							centerst2 = st2;
						}
						else if (t > 2)
						{
							mstudio_trivert_t *vert;
							vert = mBodyParts[i].models[m].meshes[j].faces[tri].vertex; // Yeah, yeah -- it works

							if (numFaces < 0)  // fan -> triangle
							{
								vertexA = centervertex;
								normA = centernorm;
								st1A = centerst1;
								st2A = centerst2;
							}
							else if (numFaces > 0) // strip -> triangle
							{
								vertexA = lastlastvertex;
								normA = lastlastnorm;
								st1A = lastlastst1;
								st2A = lastlastst2;
							}

							printf("# %i\n", tri);
							printf("f %i %i %i\n", vertexA, lastvertex, vertex);

							vert[0].vertindex = vertexA;
							vert[1].vertindex = lastvertex;
							vert[2].vertindex = vertex;

							vert[0].normindex = normA;
							vert[1].normindex = lastnorm;
							vert[2].normindex = norm;

							vert[0].s = st1A;
							vert[0].t = st2A;
							vert[1].s = lastst1;
							vert[1].t = lastst2;
							vert[2].s = st1;
							vert[2].t = st2;

							++tri;
						}

						lastlastst1 = lastst1;
						lastlastst2 = lastst2;
						lastst1 = st1;
						lastst2 = st2;

						lastlastnorm = lastnorm;
						lastnorm = norm;

						lastlastvertex = lastvertex;
						lastvertex = vertex;
					}
				}

				//printf("%li == %i\n", mesh.numtris, triCounter);
			}
		}
	}

	mBoneCount = mHeader.numbones;
 	mBones = new mstudio_bone_t[mHeader.numbones];
	printf("Reading %li bones...\n", mHeader.numbones);
	//fseek(f, mHeader.boneindex, SEEK_SET);
	f.SetOffset(mHeader.boneindex);
	//fread(mBones, 1, mHeader.numbones*sizeof(mstudio_bone_t), f);
	
	for (i = 0; i < mHeader.numbones; ++i)
	{
		f.ReadString(32, mBones[i].name);
		mBones[i].parent = f.ReadLong(); 
		mBones[i].flags = f.ReadLong();
		f.ReadLongArray(6, mBones[i].bonecontroller);
		f.ReadFloat32Array(6, mBones[i].value);
		f.ReadFloat32Array(6, mBones[i].scale);
	}

#ifdef HALFLIFE_DEBUG_LOG
	for (i = 0; (int)i < mHeader.numbones; ++i)
	{
		printf("bone[%i]\n", i);

		for (j = 0; (int)j < 6; ++j)
		{
			printf(" %f", mBones[i].value[j]);
		}

		printf("\n");
	}
#endif

	return 0;
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <freyja/FreyjaPlugin.h>
#include <mstl/Map.h>

using namespace mstl;

extern "C" {

	int freyja_model__halflife_check(char *filename);
	int freyja_model__halflife_import(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginDescription1s("Halflife model (*.mdl)");
	freyjaPluginAddExtention1s("*.mdl");
	freyjaPluginImport1i(FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int import_model(char *filename)
{
	if (!freyja_model__halflife_check(filename))
		return freyja_model__halflife_import(filename);

	return -1;
}


int freyja_model__halflife_check(char *filename)
{
	SystemIO::FileReader r;

	if (!r.Open(filename))
	{
		perror("mdl_check> fopen failed");
		return -1;
	}

	char header[32];
	r.ReadString(32, header);
	header[4] = 0;
	r.Close();

	// HalfLife
	if (strcmp(header, "IDST") == 0)
	{
		return 0;
	}

	return -2;
}


int freyja_model__halflife_import(char *filename)
{
	HalfLife hl;
	Map<unsigned int, unsigned int> trans;
	unsigned int b, f, m, vert;
	vec_t scale = 0.5; // DoD needs 0.5
	long idx;


	if (hl.load(filename) < 0)
	{
		return -1;
	}

	freyjaBegin(FREYJA_MODEL);

	/* Read texture data */
	printf("Processing HalfLife textures...\n");

	for (uint32 i = 0; i < hl.mTextureCount; ++i)
	{
		if (hl.mImages[i].image && hl.mImages[i].w > 0 && hl.mImages[i].h > 0)
		{
			freyjaTextureCreateBuffer(hl.mImages[i].image, 3, 
								  hl.mImages[i].w, hl.mImages[i].h, RGB_24);
			printf("%i/%i\n", i, hl.mTextureCount);
		}
	}


	/* Read mesh data */
	printf("Processing HalfLife bodyparts...\n");

	for (b = 0; b < hl.mBodyPartCount; ++b)
	{
		freyjaBegin(FREYJA_MESH);

		for (unsigned int mdl = 0; mdl < hl.mBodyParts[b].modelCount; ++mdl)
		{

		freyjaBegin(FREYJA_VERTEX_GROUP);	
		trans.Clear();

		for (uint32 i = 0; i < hl.mBodyParts[b].models[mdl].vertexCount; ++i)
		{
			vert = freyjaVertexCreate3f(hl.mBodyParts[b].models[mdl].vertices[i*3][0],
								  hl.mBodyParts[b].models[mdl].vertices[i*3][1],
								  hl.mBodyParts[b].models[mdl].vertices[i*3][2]);
			//freyjaVertexNormal3fv(vert, hl.mBodyParts[b].models[mdl].normals[i*3]);
			
			// Mongoose 2002.02.09, Generates id translator list
			trans.Add(i, vert);	
		}

		freyjaEnd(); // FREYJA_GROUP

		for (m = 0; m < hl.mBodyParts[b].models[mdl].meshCount; ++m)
		{
			for (f = 0; f < hl.mBodyParts[b].models[mdl].meshes[m].faceCount; ++f)
			{
				// Start a new polygon
				freyjaBegin(FREYJA_POLYGON);

				// Store vertices by true id, using translation table
				freyjaPolygonVertex1i(trans[hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[0].vertindex]);
				freyjaPolygonVertex1i(trans[hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[1].vertindex]);
				freyjaPolygonVertex1i(trans[hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[2].vertindex]);


				/* Generate, Store, and link UVs to polygon */
				if (hl.mImages) // should only be null while fixing this plugin
				{
					int mat = hl.mBodyParts[b].models[mdl].meshes[m].material;
					vec_t w = 1.0f / (float)hl.mImages[mat].w;
					vec_t h = 1.0f / (float)hl.mImages[mat].h;
					vec_t s, t;

					freyjaPolygonMaterial1i(mat);

					for (uint32 i = 0; i < 3; ++i)
					{
						s = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[i].s;
						t = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[i].t;
						freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(s*w, t*h));
					}
				}

				freyjaEnd(); // FREYJA_POLYGON
			}
		}
		}

		freyjaEnd(); // FREYJA_MESH
	}


	freyjaBegin(FREYJA_SKELETON);

	
	for (b = 0; b < hl.mBoneCount; ++b)
	{
		freyjaBegin(FREYJA_BONE);
		idx = freyjaGetCurrent(FREYJA_BONE);
		freyjaBoneParent(idx, hl.mBones[b].parent);
		freyjaBoneName(idx, hl.mBones[b].name);

		if (b == 0)
		{
			freyjaBoneTranslate3f(idx,
								  hl.mBones[b].value[0]*scale, 
								  hl.mBones[b].value[2]*scale, 
								  hl.mBones[b].value[1]*scale);
			freyjaBoneRotateEuler3f(idx,
								   hl.mBones[b].value[3], 
								   hl.mBones[b].value[4] - 90.0f, 
								   hl.mBones[b].value[5]);
		}
		else
		{
			freyjaBoneTranslate3f(idx,
								  hl.mBones[b].value[0]*scale, 
								  hl.mBones[b].value[1]*scale, 
								  hl.mBones[b].value[2]*scale);
			freyjaBoneRotateEuler3f(idx,
									hl.mBones[b].value[3], 
									hl.mBones[b].value[4], 
									hl.mBones[b].value[5]);
		}

		for (uint32 i = 0; i < hl.mBoneCount; ++i)
		{
			if (hl.mBones[i].parent == (int)b)
			{ 
				freyjaBoneAddChild(idx, i);
			}
		}

		freyjaEnd(); // FREYJA_BONE
	}

	freyjaEnd(); // FREYJA_SKELETON
	freyjaEnd(); // FREYJA_MODEL

	return 0;
}


int freyja_model__halflife_export(char *filename)
{
	printf("freyja_model__halflife_export> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);

	return -1;
}
#endif


#ifdef UNIT_TEST_HALFLIFE
int main(int argc, char *argv[])
{
	HalfLife hl;


	printf("[HalfLife class test]\n");

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (!hl.Load(argv[2]))
				printf("main: Load reports success.\n");
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			printf("Not Implemented\n");
		}
		else if (strcmp(argv[1], "test") == 0 && argc > 3)
		{
			printf("Not Implemented\n");
		}
		else
			printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
				   argv[0]);
	}
	else
	{
		printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
			   argv[0]);
	}
}
#endif
