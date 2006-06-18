#ifndef __GRNMESHES_H__
#define __GRNMESHES_H__

#include "GrannyStream.h"
#include <list>
#include <vector>

class BoneWeight
{
	public:
		BoneWeight() {numWeights=0;}
		~BoneWeight() {}
		void addWeight(dword bone,float weight);
		dword bones[10];
		float weights[10];
		dword numWeights;
	private:
};

class Mesh
{
	public:
		Mesh();
		~Mesh();
		void load( cGrannyStream *file, dword offset, dword baseOffset, dword children );
		std::vector<Point>points;
		std::vector<Point>normals;
		std::vector<Point>textureMap;
		std::vector<BoneWeight>weights;
		std::vector<gPolygon>polygons;
	private:
		void loadPoints( cGrannyStream *file );
		void loadNormals( cGrannyStream *file );
		void loadTextureMap( cGrannyStream *file );
		void loadWeights( cGrannyStream *file );
		void loadPolygons( cGrannyStream *file );
		dword id;

		dword pointOffset;
		dword normalOffset;
		dword textureOffset;
		dword weightOffset;
		dword polygonOffset;
		dword meshIDOffset;
};

class Meshes
{
public:
  Meshes();
  ~Meshes();
  void load( cGrannyStream *file, dword offset, dword baseOffset, dword children );
  std::list<Mesh> meshes;
};

#endif
