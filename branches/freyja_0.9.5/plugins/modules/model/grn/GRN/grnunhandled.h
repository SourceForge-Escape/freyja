#ifndef __GRNUNHANDLED_H__
#define __GRNUNHANDLED_H__

#include "GrannyStream.h"

class Unhandled
{
public:
  Unhandled();
  ~Unhandled();
  void load( cGrannyStream *file, dword offset, dword baseOffset, dword children );
};

#endif
