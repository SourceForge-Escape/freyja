
////////////////////////////////////////////////////////////
// Plugin hooks
////////////////////////////////////////////////////////////

#include "Nif5.h"

#ifdef FREYJA_PLUGINS
#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>

using namespace mstl;

extern "C" {

	int freyja_model__nif5_check(char *filename);
	int freyja_model__nif5_import(char *filename);
	int freyja_model__nif5_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("nif5");
	freyjaPluginDescription1s("NetImmerse 5 Model (*.nif)");
	freyjaPluginAddExtention1s("*.nif");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}


int import_model(char *filename)
{
	if (!freyja_model__nif5_check(filename))
		return freyja_model__nif5_import(filename);

	return -1;
}


int freyja_model__nif5_check(char *filename)
{	
	SystemIO::FileReader r;
	char header[48];
	unsigned int headerSz = 48;

	if (r.Open(filename) == false)
		return -1;

	/* Get header and check to see if this is a version match. */
	r.ReadString(headerSz, header);
	r.Close();

	if (strncmp(NIF5_HEADER_START, header, 38))
		return -1;

	return 0;
}


int freyja_model__nif5_import(char *filename)
{
	Vector<unsigned int> vertices;
	Vector<unsigned int> texcoords;
	Nif5 Nif5;
	long i, j, idx;


	if (Nif5.loadModel(filename) == false)
		return -1;

	freyjaBegin(FREYJA_MODEL);


	for (i = Nif5.mNiTriShapeData.begin(); i < (int)Nif5.mNiTriShapeData.end(); ++i)
	{
		Nif5::NiTriShapeData *data = Nif5.mNiTriShapeData[i];

		if (!data)
			continue;

		vertices.clear();
		texcoords.clear();

		freyjaBegin(FREYJA_MESH);
		freyjaBegin(FREYJA_VERTEX_GROUP);

		for (j = 0; j < data->num_vertices; ++j)
		{
			idx = freyjaVertexCreate3f(data->coordinates[j].x,
								 data->coordinates[j].z,
								 data->coordinates[j].y);

			freyjaVertexNormal3f(idx,
								 data->normals[j].x,
								 data->normals[j].z,
								 data->normals[j].y);

			if (data->num_uv_sets > 0)
			{
				freyjaTexCoord2f(idx,
								 data->uv_set[0].textureinfo[j].u,
								 data->uv_set[0].textureinfo[j].v);
			}

			vertices.push_back(idx);
		}

		freyjaEnd(); // FREYJA_VERTEX_GROUP

		for (j = 0; j < data->num_triangles; ++j)
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonMaterial1i(0);
			freyjaPolygonVertex1i(vertices[data->triangles[j].v[0]]);
			freyjaPolygonVertex1i(vertices[data->triangles[j].v[1]]);
			freyjaPolygonVertex1i(vertices[data->triangles[j].v[2]]);
			freyjaEnd(); // FREYJA_POLYGON
		}

		freyjaEnd(); // FREYJA_MESH		
	}

	freyjaEnd(); // FREYJA_MODEL

	return 0;
}


int freyja_model__nif5_export(char *filename)
{
	freyjaPrintError("NIF5_export> ERROR: Not implemented.\n");
	return -1;
}
#endif

