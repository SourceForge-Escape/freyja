#ifndef __GRNBONES_H__
#define __GRNBONES_H__

#include "grnmatrix.h"
#include "GrannyStream.h"
#include <vector>

class Bone
{
	public:
		Bone();
		~Bone();
		dword parent,id;
		Point translate;
		Point quaternion;
		GrnMatrix matrix,curMatrix;
		std::vector< class Bone * > children;
};

class Bones
{
public:
  Bones();
  ~Bones();
  void load( cGrannyStream * file,dword offset,dword baseOffset,dword children);
  std::vector<Bone *>bones;
private:
};

#endif
