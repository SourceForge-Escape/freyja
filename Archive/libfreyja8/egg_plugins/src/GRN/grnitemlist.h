#ifndef __GRNITEMLIST_H__
#define __GRNITEMLIST_H__

#include "grntype.h"

#include "grntextchunk.h"
#include "grnobjects.h"
#include "grnmeshes.h"
#include "grnbones.h"
#include "grnboneties.h"
#include "grntextureinfos.h"
#include "grntextures.h"
#include "grnanim.h"
#include "grnunhandled.h"

#include "GrannyStream.h"
#include <string>

class ItemList
{
public:
  ItemList();
  ~ItemList();
  void load( cGrannyStream * file );
   std::string getTextureName();
	Meshes &getMeshes();
	Bones &getBones();
	BoneTies &getTies();
	Textures &getTextures();
	BoneAnim &getBoneAnim(dword id);
	Animations &getAnimations();
	dword findString( std::string str );
	dword getValue( dword obj, dword key );
	dword findValue( dword key, dword value );
	std::string findID( dword id );
private:
	void loadList( dword peers, cGrannyStream  * file );
	TextChunk textChunk;
  Objects objects;
  Meshes meshes;
  Bones bones;
	BoneTies boneTies;
  TextureInfos textureInfos;
  Textures textures;
  Animations animations;
	Unhandled unhandled;
	dword listOffset;
};

#endif
