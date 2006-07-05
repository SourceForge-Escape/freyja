#include "grntype.h"
#include "grnitemlist.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

ItemList::ItemList()
{
}

ItemList::~ItemList()
{
}

void ItemList::load(cGrannyStream * file)
{
//  assert(file);
  file->readDword();	//unknown
  listOffset=file->readDword();
  int i;
  for (i=0;i<2;i++)
		file->readDword();	//unknown
	dword oldPos=file->tellg();

	file->seekg(listOffset);
	dword children=file->readDword();
	for (i=0;i<3;i++)
		file->readDword();	//unknown
	loadList(children,file);
	file->seekg(oldPos);
}

void ItemList::loadList(dword peers,cGrannyStream * file)
{
//  assert(file);
  for (int i=0;i<(int)peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();

		switch (chunk)
		{
			case 0xCA5E0200:
				textChunk.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0304:
				textureInfos.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0507:
				bones.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0602:
				meshes.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0a01:
				unhandled.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0b01:
				boneTies.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0c01:
				boneTies.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0d01:
				unhandled.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0e01:
				textures.load(file,offset,listOffset,children);
				break;
			case 0xCA5E0f03:
				objects.load(file,offset,listOffset,children);
				break;
			case 0xCA5E1003:
				unhandled.load(file,offset,listOffset,children);
				break;
			case 0xCA5E1205:
				animations.load(file,offset,listOffset,children);
				break;
			case 0xCA5Effff:			//end of list
				break;
			default:
				hex(cerr);
				cerr<<"Unknown ItemList Chunk: "<<chunk<<endl;
				exit(1);
		}
		i+=children+1;
	}
}

string ItemList::getTextureName()
{
	dword id=textureInfos.getTextureID();
	if (!id)
		return "";
	dword key = textChunk.findString( "__FileName" );
	dword value = objects.getValue( id - 1, key );
	return textChunk.findID( value - 1 );
}

Meshes &ItemList::getMeshes()
{
	return meshes;
}

Bones &ItemList::getBones()
{
	return bones;
}

BoneTies &ItemList::getTies()
{
	return boneTies;
}

Textures &ItemList::getTextures()
{
	return textures;
}

BoneAnim &ItemList::getBoneAnim(dword id)
{
	return animations.getBoneAnim(id);
}

Animations &ItemList::getAnimations()
{
	return animations;
}

dword ItemList::findString(string str)
{
	return textChunk.findString(str);
}

dword ItemList::getValue(dword obj,dword key)
{
	return objects.getValue(obj-1,key);
}

dword ItemList::findValue(dword key,dword value)
{
	return objects.findValue( key, value );
}

std::string ItemList::findID(dword id)
{
	return textChunk.findID( id );
}
