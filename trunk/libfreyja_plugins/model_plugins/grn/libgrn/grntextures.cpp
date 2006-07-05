#include "grntype.h"
#include "grntextures.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

Textures::Textures()
{
}

Textures::~Textures()
{
}

void Textures::load( cGrannyStream * file, dword tOffset, dword baseOffset, dword peers )
{
	dword x;
	dword oldPos;
	dword numPolys;
	for(dword i = 0; i < peers ; )
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0e00:	//texture
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0e02:	//texture poly
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0e03:	//texture poly datalist
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0e04:	//texture poly data
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0e05:	//texture poly data
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0e06:	//texture polys
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				numPolys=file->readDword();
				for (x=0;x<numPolys;x++)
				{
					gPolygon *polygon=new gPolygon();
					for (int p=0;p<4;p++)
						polygon->nodes[p]=file->readDword();
					polygons.push_back(*polygon);
				}
				file->seekg(oldPos);
				break;
			case 0xCA5E0e07:	//texturelist
				load(file,offset,baseOffset,children);
				break;
			default:
				hex( cerr );
				cerr<<"Unknown Textures Chunk: "<<chunk<<endl;
				exit(1);
		}
		i+=children+1;
	}
}
