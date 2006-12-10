/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005,2006 Mongoose
 * Comments: This is the ABI to expose Pak VFS class and utils to C.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.12.09:
 * Mongoose - Created, Split from FreyjaPluginABI.cpp
 ==========================================================================*/

#include "freyja.h"
#include "PakABI.h"


///////////////////////////////////////////////////////////////////////
//  Pak VFS 0.9.3-0.9.5 ABI
///////////////////////////////////////////////////////////////////////

#include "FreyjaPakReader.h"

Vector<FreyjaPakReader*> gFreyjaPaks;
uint32 gFreyjaPakCount = 0;


FreyjaPakReader *freyjaGetPakReader(index_t uid)
{
	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		return gFreyjaPaks[uid];
	}

	return 0x0;
}


void freyjaPakDelete(index_t uid)
{
	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		delete gFreyjaPaks[uid];
		gFreyjaPaks.assign(uid, 0x0);
		--gFreyjaPakCount;
	}
}


index_t freyjaPakBegin(const char *filename)
{
	FreyjaPakReader *pak = new FreyjaPakReader(filename);
	uint32 i, count;
	index_t uid;
	bool found = false;


	/* Setup UID and class container reference */
	uid = count = gFreyjaPaks.size();

	for (i = 0; i < count; ++i)
	{
		if (gFreyjaPaks[i] == 0x0)
		{
			uid = i;
			gFreyjaPaks.assign(uid, pak);

			found = true;
			break;
		}	
	}

	if (!found)
	{
		gFreyjaPaks.pushBack(pak);
	}

	++gFreyjaPakCount;

	pak->mUID = uid;

	return uid;
}


void freyjaPakAddFullPathFile(index_t pakIndex,
							  const char *vfsFilename, int32 offset, int32 size)
{
	index_t uid = pakIndex;

	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		gFreyjaPaks[uid]->addFullPathFileDesc(vfsFilename, offset, size);
	}
}


void freyjaPakDecryptBufferXOR1c(unsigned char *buffer, 
								 unsigned int size, unsigned char key)
{
	unsigned int i;


	for (i = 0; i < size; ++i)
	{
		buffer[i] ^= key;
	}
}


void freyjaPakAddFullPathFileXOR1c(index_t pakIndex, unsigned char key,
								   const char *vfsFilename, 
								   int32 offset, int32 size)
{
	index_t uid = pakIndex;

	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		gFreyjaPaks[uid]->addFullPathFileDesc(vfsFilename, offset, size, key);
	}
}


void freyjaPakEnd(index_t pakIndex)
{
	// ATM this does nothing, just here for reserved use
}

