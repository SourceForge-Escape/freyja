#ifndef __GRNOBJECTS_H__
#define __GRNOBJECTS_H__

#include <vector>
#include "GrannyStream.h"

class Object
{
	public:
		Object();
		~Object();
		void addValue(dword key,dword value);
		dword getValue(dword key);
		bool findValue(dword key,dword value);
	private:
		dword keys[5],values[5];
		dword numKeys;
};

class Objects
{
public:
  Objects();
  ~Objects();
  void load( cGrannyStream *file, dword offset, dword baseOffset, dword children );
	dword getValue( dword object, dword key );
	dword findValue( dword key, dword value );
private:
	std::vector<Object *> objects;

	Object *object;
	dword key;

};

#endif
