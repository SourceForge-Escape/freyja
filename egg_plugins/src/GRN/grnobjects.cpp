#include "grntype.h"
#include "grnobjects.h"
#include <iostream>

using namespace std;

Object::Object()
{
	numKeys=0;
}

Object::~Object()
{
}

void Object::addValue(dword key,dword value)
{
	keys[numKeys]=key;
	values[numKeys]=value;
	numKeys++;
}

dword Object::getValue(dword key)
{
	for (int i=0;i<numKeys;i++)
		if (keys[i]==key)
			return values[i];
	return 0;
}

bool Object::findValue(dword key,dword value)
{
	for (int i=0;i<numKeys;i++)
		if (keys[i]==key && values[i]==value)
			return true;
	return false;
}

Objects::Objects()
{
}

Objects::~Objects()
{
}

static dword objNum=1;
void Objects::load( cGrannyStream * file, dword objOffset, dword baseOffset, dword peers )
{
	dword oldOfs,val;
	for (int i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0f00:	//object
				object=new Object();
				objects.push_back(object);
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0f01:	//object key
				oldOfs=file->tellg();
				file->seekg(offset+baseOffset);
				key=file->readDword();
				file->seekg(oldOfs);
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0f02:	//object value
				oldOfs=file->tellg();
				file->seekg(offset+baseOffset);
				file->readDword();
				val=file->readDword();
				object->addValue(key,val);
				file->seekg(oldOfs);
				break;
			case 0xCA5E0f05:	//object key list
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0f06:	//object value list
				load(file,offset,baseOffset,children);
				break;
			default:
				hex(cerr);
				cerr<<"Unknown Objects Chunk: "<<chunk<<endl;
				exit(1);
		}
		i+=children+1;
	}
}

dword Objects::getValue(dword obj,dword key)
{
	return objects[obj]->getValue(key);
}

dword Objects::findValue(dword key,dword value)
{
	vector<Object *>::iterator iobj;
	dword obj=1;
	for (iobj=objects.begin();iobj!=objects.end() &&
			(*iobj)->findValue(key,value)==false;iobj++,obj++);
	return obj;
}
