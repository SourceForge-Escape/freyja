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
#include <mstl/String.h>
#include "freyja.h"


namespace freyja {

class PluginDescEvent
{
public:

	PluginDescEvent() : 
		mName(""),
		mEventId(-1),
		mCallback(NULL)
	{
	}

	mstl::String mName;

	int mEventId;

	void (*mCallback)();
};


class PluginDesc
{
public:
	
	class PluginDescArg
	{
	public:

		PluginDescArg() : 
			mName(""), mType("None"), mValue("") { }

		PluginDescArg(const char *name, const char *type, const char *value) : 
			mName(name), mType(type), mValue(value) { }

		PluginDescArg(const PluginDescArg &arg) : 
			mName(arg.mName), mType(arg.mType), mValue(arg.mValue)  { }

		PluginDescArg &operator=(const PluginDescArg &arg)
		{
			mName = arg.mName;
			mType = arg.mType;
			mValue = arg.mValue;
			return *this;
		}

		~PluginDescArg() { }

		mstl::String &GetStringName() { return mName; }

		const char *GetName() { return mName.c_str(); }

		void SetName(const char *name) { mName = name; }

		mstl::String &GetStringType() { return mType; }

		const char *GetType() { return mType.c_str(); }

		void SetType(const char *type) { mType = type; }

		const char *GetValue() { return mValue.c_str(); }

		void SetValue(const char *value) { mValue = value; }
		// How generic can you get w/o language generics? 
		// Embedd mlisp/python scripts if you want it's a string =p

	private:

		mstl::String mName;

		mstl::String mType;

		mstl::String mValue;
	};


	PluginDesc() :
		mName(""),
		mFilename(""),
		mDescription(""),
		mExtention(""),
		mArgs(),
		mImportFlags(FREYJA_PLUGIN_NONE),
		mExportFlags(FREYJA_PLUGIN_NONE),
		mId(mNextId)
	{
		++mNextId;
	}


	PluginDesc(const PluginDesc& desc) :
		mName(desc.mName),
		mFilename(desc.mFilename),
		mDescription(desc.mDescription),
		mExtention(desc.mExtention),
		mArgs(desc.mArgs),
		mImportFlags(desc.mImportFlags),
		mExportFlags(desc.mExportFlags),
		mId(desc.mId)
	{
	}


	PluginDesc &operator=(const PluginDesc &desc)
	{
		mName = desc.mName;
		mFilename = desc.mFilename;
		mDescription = desc.mDescription;
		mExtention = desc.mExtention;
		mImportFlags = desc.mImportFlags;
		mExportFlags = desc.mExportFlags;
		mArgs = desc.mArgs;
		mId = desc.mId;

		return *this;
	}


	~PluginDesc() { }

	void SetFilename(const char *filename)
	{
		if (filename && filename[0])
		{
			mFilename = filename;
		}
	}

	void SetDescription(const char *s) { mDescription = s; }

	void SetExtention(const char *s) { mExtention = s; }

	// Id property
	void SetId(long id) { mId = id; }

	long GetId() { return mId; }


	const char *QueryArgList(const char *type, const char *name)
	{
		mstl::String keyType = type;
		mstl::String keyName = name;
		unsigned int i;

		foreach ( mArgs, i)
		{
			if (mArgs[i].GetStringType() == keyType &&
				mArgs[i].GetStringName() == keyName)
			{
				return mArgs[i].GetValue();
			}
		}

		return NULL;
	}

	void InsertArg(const char *type, const char *name, const char *value)
	{
		mArgs.push_back( PluginDescArg(name, type, value) );
	}


	// Old helper methods reimplemented for old ABI use, isn't that nice
	int32 GetIntArg(const char *name) 
	{ 
		const char *s = QueryArgList("int32", name); 
		return s ? atoi(s) : 0;
	}

	void AddIntArg(const char *name, int32 val) 
	{
		mstl::String s;
		s.Set("%i", val);
		InsertArg("int32", name, s.c_str()); 
	}

	float GetFloatArg(const char *name) 
	{ 
		const char *s = QueryArgList("float", name); 
		return s ? atof(s) : 0.0f;
	}

	void AddFloatArg(const char *name, float val) 
	{
		mstl::String s;
		s.Set("%f", val);
		InsertArg("float", name, s.c_str()); 
	}

	/*const*/ char *GetStringArg(const char *name) 
	{ 
		// removing const for deprecated calls, which use 'char *'
		return (char *)QueryArgList("char*", name);
	}

	void AddStringArg(const char *name, const char *val) 
	{
		InsertArg("char*", name, val); 
	}

	mstl::String mName;

	mstl::String mFilename;

	mstl::String mDescription;

	mstl::String mExtention;

	mstl::Vector<PluginDescArg> mArgs;

	long mImportFlags;

	long mExportFlags;

private:

	static long mNextId;

	long mId;
};

} // namespace freyja

#endif // GUARD__FREYJA_PLUGIN_H_


