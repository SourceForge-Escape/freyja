#ifndef __GRNTEXTUREINFOS_H__
#define __GRNTEXTUREINFOS_H__

#include "GrannyStream.h"
#include <vector>

class TextureInfo
{
	public:
		TextureInfo();
		~TextureInfo();
		void load( cGrannyStream *file, dword offset, dword baseOffset, dword children );
		dword getID();
	private:
		dword width,height,depth,id;
};

class TextureInfos
{
public:
  TextureInfos();
  ~TextureInfos();
  void load( cGrannyStream *file, dword offset, dword baseOffset, dword children );
	dword getTextureID();
private:
	std::vector<TextureInfo> textures;
};

#endif
