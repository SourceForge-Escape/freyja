/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Plugin
 * License : GPL, also (C) 2000-2006 Mongoose
 * Comments: This is the Plugin management class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.10: (v0.0.4)
 * Mongoose - New interactive plugins
 *            API changes for new backend ( Egg9 )
 *
 *
 * 2004.03.23: (v0.0.3+)
 * Mongoose - New singleton implementation
 *            New plugin features for texture loading
 *            Removed last of old List STL use
 *
 *            New style, which affects compiler portability
 *            Mostly removing leading _underscores
 *
 * 200?.??.??: (v0.0.3)
 * Mongoose - Wha' Ha'pen?
 *
 * 2001.11.02: (v0.0.2)
 * Mongoose - Experimental export API added 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#ifndef GUARD__FREYJA_PLUGIN_H_
#define GUARD__FREYJA_PLUGIN_H_

#include <stdarg.h>
#include <string.h>
#include <mstl/stack.h>
#include <mstl/Vector.h>
#include "freyja.h"

using namespace mstl;


class FreyjaPluginDescEvent
{
public:

	FreyjaPluginDescEvent() : 
		eventId(-1)
	{
		name[0] = 0;
	}

	char name[64];

	int eventId;

	void *callback();
};


class FreyjaPluginDesc
{
public:
	
	FreyjaPluginDesc() :
		mFilename(NULL),
		mImportFlags(FREYJA_PLUGIN_NONE),
		mExportFlags(FREYJA_PLUGIN_NONE),
		mFloatArgsDesc(),
		mFloatArgs(),
		mIntArgsDesc(),
		mIntArgs(),
		mStringArgsDesc(),
		mStringArgs(),
		mId(mNextId)
	{
		mDescription[0] = 0;
		mExtention[0] = 0;
		++mNextId;
	}


	FreyjaPluginDesc(const FreyjaPluginDesc& p) :
		mFilename(NULL),
		mImportFlags(FREYJA_PLUGIN_NONE),
		mExportFlags(FREYJA_PLUGIN_NONE),
		mFloatArgsDesc(),
		mFloatArgs(),
		mIntArgsDesc(),
		mIntArgs(),
		mStringArgsDesc(),
		mStringArgs(),
		mId(p.mId)
	{
		BUG_ME("This shouldn't ever happen by design");
	}


	FreyjaPluginDesc &operator=(const FreyjaPluginDesc &fpd)
	{
		BUG_ME("This shouldn't ever happen by design");
		return *this;
	}


	~FreyjaPluginDesc()
	{
		if (mFilename) delete [] mFilename;
		
	}

	void setFilename(const char *filename)
	{
		if (filename && filename[0])
		{
			if (mFilename) delete [] mFilename;
			mFilename = strdup(filename);
		}
	}

	void setDescription(const char *s)
	{
		strncpy(mDescription, s, 64);
		mDescription[63] = 0;
	}

	void setExtention(const char *s)
	{
		strncpy(mExtention, s, 64);
		mExtention[63] = 0;
	}

	void setId(long id)
	{
		mId = id;
	}

	long getId()
	{
		return mId;
	}

	void print()
	{
		freyjaPrintMessage("%s", mDescription);
		freyjaPrintMessage("\tImport: %s%s%s",
						   (mImportFlags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
						   (mImportFlags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
						   (mImportFlags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
		freyjaPrintMessage("\tExport: %s%s%s",
						   (mExportFlags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
						   (mExportFlags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
						   (mExportFlags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
	}


	long getIntArg(const char *name)
	{
		long i, l;

		if (!name || !name[0])
			return -1; // oh well

		l = strnlen(name, 64);

		for (i = mIntArgsDesc.begin(); i < (long)mIntArgsDesc.end(); ++i)
		{
			if (!strncmp(name, mIntArgsDesc[i], l))
				return mIntArgs[i];
		}

		return -1;
	}


	float getFloatArg(const char *name)
	{
		long i, l;

		if (!name || !name[0])
			return -1; // oh well

		l = strnlen(name, 64);

		for (i = mFloatArgsDesc.begin(); i < (long)mFloatArgsDesc.end(); ++i)
		{
			if (!strncmp(name, mFloatArgsDesc[i], l))
				return mFloatArgs[i];
		}

		return -1;
	}


	char *getStringArg(const char *name)
	{
		long i, l;

		if (!name || !name[0])
			return 0x0; // oh well

		l = strnlen(name, 64);

		for (i = mStringArgsDesc.begin(); i < (long)mStringArgsDesc.end(); ++i)
		{
			if (!strncmp(name, mStringArgsDesc[i], l))
				return mStringArgs[i];
		}

		return 0x0;
	}

	void addIntArg(const char *name, long data)
	{
		char *s;
		long l = 0;

		if (!name || !name[0])
			return;

		l = strnlen(name, 64);
		s = new char[64];
		strncpy(s, name, 64);
		s[63] = 0;

		mIntArgs.pushBack(data);
		mIntArgsDesc.pushBack(s);
	}


	void addFloatArg(const char *name, float data)
	{
		char *s;
		long l = 0;

		if (!name || !name[0])
			return;

		l = strnlen(name, 64);
		s = new char[64];
		strncpy(s, name, 64);
		s[63] = 0;

		mFloatArgs.pushBack(data);
		mFloatArgsDesc.pushBack(s);
	}


	void addStringArg(const char *name, const char *data)
	{
		char *s;
		long l = 0;

		if (!name || !name[0])
			return;

		l = strnlen(name, 64);
		s = new char[64];
		strncpy(s, name, 64);
		s[63] = 0;

		mStringArgsDesc.pushBack(s);

		if (data && data[0])
		{
			l = strnlen(name, 64);
			s = new char[l+1];
			strncpy(s, data, l);
			s[l-1] = 0;
		}
		else
		{
			s = new char[64];
			s[0] = 0;
		}

		mStringArgs.pushBack(s);
	}


	char *mFilename;

	char mDescription[64];

	char mExtention[64];

	long mImportFlags;

	long mExportFlags;

	Vector<char *> mFloatArgsDesc;
	Vector<float> mFloatArgs;

	Vector<char *> mIntArgsDesc;
	Vector<long> mIntArgs;

	Vector<char *> mStringArgsDesc;
	Vector<char *> mStringArgs;

private:

	static long mNextId;

	long mId;
};

#endif


