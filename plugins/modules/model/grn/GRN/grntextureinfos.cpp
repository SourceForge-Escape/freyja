#include "grntype.h"
#include "grntextureinfos.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

TextureInfo::TextureInfo()
{
}
TextureInfo::~TextureInfo()
{
}

void TextureInfo::load( cGrannyStream * file, dword tiOffset, dword baseOffset, dword peers )
{
	dword oldPos;
	for (dword i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0303:	//Geometry
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				width=file->readDword();
				height=file->readDword();
				depth=file->readDword();
				file->seekg(oldPos);
				break;
			case 0xCA5E0305:	//Geometry Container
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0f04:	//textureid
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				id=file->readDword();
				file->seekg(oldPos);
				break;
			default:
				hex(cerr);
				cerr<<"Unknown TextureInfo Chunk: "<<chunk<<endl;
				exit(1);
		}
		i+=children+1;
	}
}
dword TextureInfo::getID()
{
	return id;
}

TextureInfos::TextureInfos()
{
}

TextureInfos::~TextureInfos()
{
}

void TextureInfos::load(cGrannyStream * file,dword texOffset,
		dword baseOffset,dword peers)
{
  //dword oldPos;
	TextureInfo *texture;
	for (dword i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0301:	//Texture
				texture=new TextureInfo();
				texture->load(file,offset,baseOffset,children);
				textures.push_back(*texture);
				break;
			default:
				hex(cerr);
				cerr<<"Unknown TextureInfos Chunk: "<<chunk<<endl;
				exit(1);
		}
		i+=children+1;
	}
}

dword TextureInfos::getTextureID()
{
	if (!textures.size())
		return 0;
	return textures[0].getID();
}
