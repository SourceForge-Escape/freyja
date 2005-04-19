#ifndef __GRNTEXTCHUNK_H__
#define __GRNTEXTCHUNK_H__

#include "GrannyStream.h"
#include <string>
#include <vector>

class TextChunk
{
public:
  TextChunk();
  ~TextChunk();
  void load( cGrannyStream *file, dword offset, dword baseOffset, dword children);
  dword findString( std::string name );
	std::string findID(dword id);
private:
	dword numEntries;
	std::vector< std::string > entries;
};

#endif
