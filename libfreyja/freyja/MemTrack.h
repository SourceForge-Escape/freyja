/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the FreyjaMesh and classes that compose it.
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, split from freyja header
 ==========================================================================*/

#ifndef GUARD__FREYJA_MEMTRACK__H_
#define GUARD__FREYJA_MEMTRACK__H_

#include <mstl/String.h>


namespace freyja {

class MemTrack
{
public:
	MemTrack() :
		mFunc(),
		mFile(),
		mLine(0),
		mPtr(NULL)
	{
	}

	MemTrack(const MemTrack &track) :
		mFunc(track.mFunc),
		mFile(track.mFile),
		mLine(track.mLine),
		mPtr(track.mPtr)
	{
	}

	MemTrack(const char *file, const char *func, long line, void *ptr) :
		mFunc(func),
		mFile(file),
		mLine(line),
		mPtr(ptr)
	{
	}

	MemTrack &operator=(const MemTrack &track)
	{
		mFunc = track.mFunc;
		mFile = track.mFile;
		mLine = track.mLine;
		mPtr = track.mPtr;
		return *this;
	}

	~MemTrack()
	{
	}

	String mFunc;
	String mFile;
	long mLine;
	void *mPtr;
};

} // End namespace freyja

#endif // GUARD__FREYJA_MEMTRACK__H_
