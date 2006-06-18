#ifndef __GRNTEXTURES_H__
#define __GRNTEXTURES_H__

#include <vector>
#include "GrannyStream.h"

class Textures
{
public:
  Textures();
  ~Textures();
  void load( cGrannyStream *file, dword offset, dword baseOffset, dword children );
  std::vector< gPolygon > polygons;
private:
};

#endif
