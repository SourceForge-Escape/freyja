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

#include "HalfLife.h"


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


#ifdef BUFFERED_READ
int HalfLife::loadBuffered(const char *filename)
{
	FILE *f;
	unsigned char *buffer;
	long size, magic;


	f = fopen(filename, "rb");

	if (!f)
	{
		printf("HalfLife::load> Couldn't load file.\n");
		perror(filename);

		return -1;
	}

	fread(&magic, 4, 1, f);

	if (magic != 0x54534449)
	{
		printf("Not a valid halflife model\n");
		fclose(f);

		return -2;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	buffer = new unsigned char[size];
	rewind(f);
	fread(buffer, size, 1, f);
	fclose(f);
}
#endif


int HalfLife::load(const char *filename)
{
	FILE *f;
	unsigned int i, j, k, m;


	f = fopen(filename, "rb");

	if (!f)
	{
		printf("HalfLife::load> Couldn't load file.\n");
		perror(filename);

		return -1;
	}

	fread(&mHeader, sizeof(studiohdr_t), 1, f);

	if (mHeader.id != 0x54534449)
	{
		printf("Not a valid halflife model\n");
		fclose(f);

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
	if (mHeader.textureindex > 0 && mHeader.numtextures <= MAXSTUDIOSKINS)
	{
		byte palette[768]; // 256 RGB colors
		byte *image = 0x0;
		unsigned int size;
		byte pixel;


		mTextureCount = mHeader.numtextures;
		mTextures = new mstudio_texture_t[mTextureCount];
		mImages = new hl_image_t[mTextureCount];

		fseek(f, mHeader.textureindex, SEEK_SET);
		fread(mTextures, mTextureCount, sizeof(mstudio_texture_t), f);
    
		for (i = 0; i < mTextureCount; ++i)
		{
			printf("Texture[%i] %ix%i index= %i\n", 
				   i, 
				   mTextures[i].width, mTextures[i].height,
				   mTextures[i].index);

			if (mTextures[i].width < 1 || mTextures[i].height < 1)
			{
				mImages[i].image = 0x0;
				mImages[i].w = 0;
				mImages[i].h = 0;
				continue;
			}

			size = mImages[i].w * mImages[i].h;

			mImages[i].w = mTextures[i].width;
			mImages[i].h = mTextures[i].height;
			mImages[i].image = new byte[size * 3];

			image = new byte[size];

			fseek(f, mTextures[i].index, SEEK_SET);
			fread(image, size, 1, f);

			fseek(f, mTextures[i].index + size, SEEK_SET);
			fread(palette, 1, 768, f);

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

		fseek(f, mHeader.bodypartindex + i * size, SEEK_SET);
		fread(&bodyPart, 1, size, f);

		printf("mBodyParts[%i].name = '%s'\n", i, bodyPart.name);
		printf("mBodyParts[%i].nummodels = %li\n", i, bodyPart.nummodels);

		mBodyParts[i].modelCount = bodyPart.nummodels;
		mBodyParts[i].models = new hl_model_t[bodyPart.nummodels];

		for (m = 0; (int)m < bodyPart.nummodels; ++m)
		{
			//bodyPart.modelindex
			fseek(f, bodyPart.modelindex+((i/bodyPart.base)%bodyPart.nummodels), SEEK_SET);
			fseek(f, m*sizeof(mstudio_model_t), SEEK_CUR);
			fread(&model, 1, sizeof(mstudio_model_t), f);

			printf("  model.name = '%s'\n", model.name);
			printf("  model.nummesh = %li\n", model.nummesh);
			printf("  model.numverts = %li\n", model.numverts);

			if (model.nummesh < 0 || model.numverts < 0)
			{
				mBodyParts[i].models[m].meshCount = 0;
				mBodyParts[i].models[m].meshes = 0x0;
				mBodyParts[i].models[m].groupCount = 0;
				mBodyParts[i].models[m].groups = 0x0;
				mBodyParts[i].models[m].vertexCount = 0;
				mBodyParts[i].models[m].vertices = 0x0;
				mBodyParts[i].models[m].normalCount = 0;
				mBodyParts[i].models[m].normals = 0x0;
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

			fseek(f, model.vertindex, SEEK_SET);
			fread(mBodyParts[i].models[m].vertices, sizeof(vec3_t),
				  model.numverts, f);
			fseek(f, model.normindex, SEEK_SET);
			fread(mBodyParts[i].models[m].normals, sizeof(vec3_t),
				  model.numnorms, f);

			printf("    Reading %li meshes...\n", model.nummesh);

			for (j = 0; (int)j < model.nummesh; ++j)
			{
				size = sizeof(mstudio_mesh_t);

				fseek(f, model.meshindex + j * size, SEEK_SET);
				fread(&mesh, size, 1, f);

				printf("    mesh[%i].faces = %li...\n", j, mesh.numtris);

				mBodyParts[i].models[m].meshes[j].faceCount = mesh.numtris;
				mBodyParts[i].models[m].meshes[j].faces = new hl_face_t[mesh.numtris];
				mBodyParts[i].models[m].meshes[j].normalCount = mesh.numnorms;
				mBodyParts[i].models[m].meshes[j].material = mesh.skinref;

				fseek(f, mesh.triindex, SEEK_SET);

				size = sizeof(mstudio_trivert_t);

#define HALFLIFE_EASY_FACE
#ifdef HALFLIFE_EASY_FACE
				// Hacky stack space jujitsu to avoid more vectors/arrays
				short numFaces, vertex, light, st1, st2;
				short centervertex, lastvertex, lastlastvertex;
				short centerst1, lastst1, lastlastst1;
				short centerst2, lastst2, lastlastst2;
				unsigned int t, tn, tri;

				/* Face loader: triangle fans and strips */
				for (k = 0; (int)k < mesh.numtris; ++k)
				{
					fseek(f, mesh.triindex + 2*k, SEEK_SET);
					fread(&numFaces, 1, 2, f);

#ifdef DEBUG_HALFLIFE_FACETS
					printf("Reading %i %s...\n",
						   (numFaces > 0) ? numFaces : -numFaces, 
						   (numFaces > 0) ? "triangle fans" : "triangle strips");
#endif
					tn = (numFaces > 0) ? numFaces : -numFaces;

					for (tri = 0, t = 0; t < tn; ++t)
					{
						fread(&vertex, 1, 2, f);
						fread(&light, 1, 2, f);
						fread(&st1, 1, 2, f);
						fread(&st2, 1, 2, f);

						if (t == 0)
						{
							centervertex = vertex;
							centerst1 = st1;
							centerst2 = st2;
						}
						else if (numFaces > 0 && t > 2) // fan -> triangle
						{
							mBodyParts[i].models[m].meshes[j].faces[tri].vertex[0].vertindex = centervertex;
							mBodyParts[i].models[m].meshes[j].faces[tri].vertex[1].vertindex = lastvertex;
							mBodyParts[i].models[m].meshes[j].faces[tri].vertex[2].vertindex = vertex;
							//printf("f %i %i %i\n", 
							//	   centervertex, lastvertex, vertex);

							hl.mBodyParts[b].models[mdl].meshes[m].faces[tri].vertex[0].s = centerst1;
							hl.mBodyParts[b].models[mdl].meshes[m].faces[tri].vertex[0].t = centerst2;
							hl.mBodyParts[b].models[mdl].meshes[m].faces[tri].vertex[1].s = lastst1;
							hl.mBodyParts[b].models[mdl].meshes[m].faces[tri].vertex[1].t = lastst2;
							hl.mBodyParts[b].models[mdl].meshes[m].faces[tri].vertex[2].s = st1;
							hl.mBodyParts[b].models[mdl].meshes[m].faces[tri].vertex[2].t = st2;

							++tri;
						}
						else if (numFaces < 0 && t > 2) // strip -> triangle
						{
							mBodyParts[i].models[m].meshes[j].faces[tri].vertex[0].vertindex = lastlastvertex;
							mBodyParts[i].models[m].meshes[j].faces[tri].vertex[1].vertindex = lastvertex;
							mBodyParts[i].models[m].meshes[j].faces[tri].vertex[2].vertindex = vertex;
							//printf("s %i %i %i\n", 
							//	   lastlastvertex, lastvertex, vertex);

							++tri;
						}

						lastlastvertex = lastvertex;
						lastvertex = vertex;
					}
				}
			}
		}
	}

	mBoneCount = mHeader.numbones;
 	mBones = new mstudio_bone_t[mHeader.numbones];
	printf("Reading %li bones...\n", mHeader.numbones);
	fseek(f, mHeader.boneindex, SEEK_SET);
	fread(mBones, 1, mHeader.numbones*sizeof(mstudio_bone_t), f);

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

#ifdef FREYJA_MODEL_PLUGINS

#include <freyja8/EggPlugin.h>


extern "C" {

	int freyja_model__halflife_check(char *filename);
	int freyja_model__halflife_import(char *filename);
}



int freyja_model__halflife_check(char *filename)
{
	FILE *f;
	char header[32];


	f = fopen(filename, "rb");

	if (!f)
	{
		perror("mdl_check> fopen failed");
		return -1;
	}

	fread(&header, 32, 1, f);      
	fclose(f);

	header[4] = 0;

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
	unsigned int i, b, f, m, vert;
	float u, v, w, h;
	short s, t;
	vec_t scale = 0.5; // DoD needs 0.5


	if (hl.load(filename) < 0)
	{
		return -1;
	}


	/* Read texture data */
	printf("Processing HalfLife textures: ");

	for (i = 10; i < hl.mTextureCount; ++i)
	{
		if (hl.mImages[i].image && hl.mImages[i].w > 0 && hl.mImages[i].h > 0)
		{
			eggTextureStoreBuffer(hl.mImages[i].image, 3, 
								  hl.mImages[i].w, hl.mImages[i].h,
								  EGG_COLORMODE_RGB);
		}
	}


	/* Read mesh data */
	printf("Processing HalfLife bodyparts:\n");

	for (b = 0; b < hl.mBodyPartCount; ++b)
	{
		eggBegin(FREYJA_MESH);

		for (unsigned int mdl = 0; mdl < hl.mBodyParts[b].modelCount; ++mdl)
		{

		eggBegin(FREYJA_GROUP);	
		trans.Clear();

		for (i = 0; i < hl.mBodyParts[b].models[mdl].vertexCount; ++i)
		{
			vert = eggVertexStore3f(hl.mBodyParts[b].models[mdl].vertices[i*3][0],
									hl.mBodyParts[b].models[mdl].vertices[i*3][1], 
									hl.mBodyParts[b].models[mdl].vertices[i*3][2]);
			
			// Mongoose 2002.02.09, Generates id translator list
			trans.Add(i, vert);	
		}

		eggEnd(); // FREYJA_GROUP

		for (m = 0; m < hl.mBodyParts[b].models[mdl].meshCount; ++m)
		{
			for (f = 0; f < hl.mBodyParts[b].models[mdl].meshes[m].faceCount; ++f)
			{
				// Start a new polygon
				eggBegin(FREYJA_POLYGON);

				// Store vertices by true id, using translation table
				eggVertex1i(trans[hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[0].vertindex]);
				eggVertex1i(trans[hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[1].vertindex]);
				eggVertex1i(trans[hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[2].vertindex]);


				/* Generate, Store, and link UVs to polygon */
				if (hl.mImages) // should only be null while fixing this plugin
				{
					w = (float)hl.mImages[hl.mBodyParts[b].models[mdl].meshes[m].material].w;
					h = (float)hl.mImages[hl.mBodyParts[b].models[mdl].meshes[m].material].h;

					s = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[0].s;
					t = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[0].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					s = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[1].s;
					t = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[1].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					s = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[2].s;
					t = hl.mBodyParts[b].models[mdl].meshes[m].faces[f].vertex[2].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					eggTexture1i(hl.mBodyParts[b].models[mdl].meshes[m].material);
				}

				eggEnd(); // FREYJA_POLYGON
			}
		}
		}

		eggEnd(); // FREYJA_MESH
	}


	eggBegin(FREYJA_SKELETON);

	
	for (b = 0; b < hl.mBoneCount; ++b)
	{
		eggBegin(FREYJA_BONE);
		eggSetBoneParent(hl.mBones[b].parent);
		eggTagName(hl.mBones[b].name);

		if (b == 0)
		{
			eggTagPos3f(hl.mBones[b].value[0]*scale, 
						hl.mBones[b].value[2]*scale, 
						hl.mBones[b].value[1]*scale);
			eggTagRotate3f(hl.mBones[b].value[3]*57.295779513082323, 
						   hl.mBones[b].value[4]*57.295779513082323 -90.0, 
						   hl.mBones[b].value[5]*57.295779513082323);
		}
		else
		{
			eggTagPos3f(hl.mBones[b].value[0]*scale, 
						hl.mBones[b].value[1]*scale, 
						hl.mBones[b].value[2]*scale);
			eggTagRotate3f(hl.mBones[b].value[3]*57.295779513082323, 
						   hl.mBones[b].value[4]*57.295779513082323, 
						   hl.mBones[b].value[5]*57.295779513082323);
		}

		for (i = 0; i < hl.mBoneCount; ++i)
		{
			if (hl.mBones[i].parent == (int)b)
			{ 
				eggTagAddSlave1u(i);
			}
		}

		eggEnd(); // FREYJA_BONE
	}

	eggEnd(); // FREYJA_SKELETON

	return 0;
}


int freyja_model__halflife_export(char *filename)
{
	printf("freyja_model__halflife_export> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);

	return -1;
}


// FIXME: Finish porting EggPlugin code
#ifdef FIXME
void FreyjaModel::HalfLifeImport(HalfLife *hl)
{
  Map<unsigned int> trans;
  egg_mesh_t *mesh;
  egg_tag_t *tag;
  egg_group_t *grp;
  egg_vertex_t *vert;
  MtkImage img;
  int b, t, m, v, i;
  float st[2];
  studiohdr_t *hl_header;
  vec3_t **hl_verts;
  mstudiomesh_t **hl_mesh;
  mstudiomodel_t *hl_mdl;
  trisy_keen_dood_t ***hl_tris;
  hlskin_t *hl_skin;
  char buffer[64];


  if (!_egg || !hl)
    return;

  hl_skin = hl.Skin();
  hl_header = hl.Header();
  hl_mesh = hl.Meshes();
  hl_mdl = hl.Mdls();
  hl_verts = hl.Vertices();
  hl_tris = hl.Tris();


  /* Read texture data */
  for (i = 0; i < hl_header->numtextures; i++)
  {
    eggTextureStoreBuffer(hl_skin[i].image, 3, hl_skin[i].w, hl_skin[i].h, 
			  EGG_COLORMODE_RGB);
  }


  /* Read mesh data */
  for (b = 0; b < hl_header->numbodyparts; b++)
  {
    eggBegin(FREYJA_BONE_FRAME);
    eggBegin(FREYJA_BONE_TAG);

    eggTagCenter(0.0, 0.0, 0.0);

    trans.Clear();

    for (v = 0; v < hl_mdl[b].numverts; v++)
    {
      vert = _egg->VertexAdd(hl_verts[b][v][0], hl_verts[b][v][1], 
			     hl_verts[b][v][2]);

      
      trans.Add(v, vert->id);
    }

    for (m = 0; m < hl_mdl[b].nummesh; m++)
    {
      mesh = _egg->MeshNew();
      _egg->MeshAdd(mesh);
      _egg->TagAddMesh(tag, mesh->id);
      grp = _egg->GroupNew();
      _egg->GroupAdd(grp);
      mesh->group.Add(grp->id);

      for (t = 0; t < hl_mesh[b][m].numtris; t++)
      {
	// Clear the dictionary/indexed lists
	vertex.Clear();
	texel.Clear();

        if (grp->vertex.SearchKey(trans[hl_tris[b][m][t].tri[0]]) == UINT_MAX)
          grp->vertex.Add(trans[hl_tris[b][m][t].tri[0]]);

        if (grp->vertex.SearchKey(trans[hl_tris[b][m][t].tri[1]]) == UINT_MAX)
          grp->vertex.Add(trans[hl_tris[b][m][t].tri[1]]);

        if (grp->vertex.SearchKey(trans[hl_tris[b][m][t].tri[2]]) == UINT_MAX)
          grp->vertex.Add(trans[hl_tris[b][m][t].tri[2]]);

	// Store vertices by true id, using translation table
	vertex.Add(trans[hl_tris[b][m][t].tri[0]]);
	vertex.Add(trans[hl_tris[b][m][t].tri[1]]);
	vertex.Add(trans[hl_tris[b][m][t].tri[2]]);

	st[0] = hl_tris[b][m][t].st[0][0] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].w;
	st[1] = hl_tris[b][m][t].st[0][1] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].h;
	texel.Add(_egg->TexelAdd(st[0], st[1]));
	//printf("st = %f %f\n", st[0], st[1]);

	st[0] = hl_tris[b][m][t].st[1][0] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].w;
	st[1] = hl_tris[b][m][t].st[1][1] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].h;
	texel.Add(_egg->TexelAdd(st[0], st[1]));
	//printf("st = %f %f\n", st[0], st[1]);

	st[0] = hl_tris[b][m][t].st[2][0] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].w;
	st[1] = hl_tris[b][m][t].st[2][1] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].h;
	texel.Add(_egg->TexelAdd(st[0], st[1]));
	//printf("st = %f %f\n", st[0], st[1]);

	mesh->polygon.Add(_egg->PolygonAdd(&vertex, &texel, 
					   hl_mesh[b][m].skinref));
      }
    }
  }

  return 0;
}
#endif

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
