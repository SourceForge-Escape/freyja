/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry Hendrix ( Mongoose )
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : SystemIO w/ endian support
 * License : No use w/o permission (C) 2001-2006 Mongoose
 * Comments: Lots of nice utils from libfreyja moved to mstl
 *           aids system abstraction and debugging.
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.07.23:
 * Mongoose - Barrowed from my libfreyja, and then made more generic
 ================================================================*/

#ifndef GUARD__MSTL_TIMER_H_
#define GUARD__MSTL_TIMER_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#ifdef WIN32
#   include <windows.h>
#else // UNIX is default system
#   include <dlfcn.h>
#   include <sys/times.h>
#endif

namespace mstl {

#ifdef WIN32

// FIXME: StopWatch on windows needs to sync with unix
class StopWatch
{
public:

	StopWatch() : mStart(0), mStop(0) { }

	~StopWatch() { }

	void Start() {	mStart = GetTickCount(); } //QueryPerformanceCounter(); }

	void Stop() { mStop = GetTickCount(); } //QueryPerformanceCounter(); }

	double GetElapse() 
	{ 
		return (mStop - mStart) / 1000.0; //milliseconds
	}

	void GetElapse(unsigned long &sec, unsigned long &usec)
	{
		double e = (mStop - mStart) / 1000.0; //milliseconds
		sec = (unsigned long)e;
		usec = (unsigned long)((e - (double)sec) * 1000.0);
	}

	DWORD mStart;
	DWORD mStop;
};

#else ////////////////////////////////////////////////////////////

class AccumulationTimer
{
 public:	

	AccumulationTimer() : mStart(), mStop() { }

	~AccumulationTimer() { }

	void Start() { times(&mStart); }
	
	void Stop() { times(&mStop); }

	unsigned long GetElapse() { return mStop.tms_utime - mStart.tms_utime; }

	struct tms mStart; /* Stores the starting time */

	struct tms mStop;  /* Stores the ending time */
};


class StopWatch
{
public:

	StopWatch() : mStart(), mStop() { }

	~StopWatch() { }

	void Start() {	gettimeofday(&mStart, NULL); }

	void Stop() { gettimeofday(&mStop, NULL); }

	double GetElapse() 
	{ 
		double start = (double)mStart.tv_sec + (1.e-6) * mStart.tv_usec;
		double stop = (double)mStop.tv_sec + (1.e-6) * mStop.tv_usec;
		return stop - start;
	}

	void GetElapse(unsigned long &sec, unsigned long &usec)
	{
		// Please recall 1s = 1,000,000 usec 
		if (mStop.tv_usec < mStart.tv_usec)
		{
			sec = mStop.tv_sec - mStart.tv_sec - 1;
			usec = mStop.tv_usec + 1000000 - mStart.tv_usec;
		}
		else
		{
			sec = mStop.tv_sec - mStart.tv_sec;
			usec = mStop.tv_usec - mStart.tv_usec;
		}
	}

	struct timeval mStart;
	struct timeval mStop;
};


class AccumulationStopWatch 
{
public:

	AccumulationStopWatch() : mStart(), mStop(), mElapse() { Reset(); }

	AccumulationStopWatch(const AccumulationStopWatch &w) : mStart(), mStop(), mElapse()	{ *this = w; }

	AccumulationStopWatch(AccumulationStopWatch &w) : mStart(), mStop(), mElapse()	{ *this = w; }

	~AccumulationStopWatch() { }

	AccumulationStopWatch &operator =(const AccumulationStopWatch &w)
	{
#if 1
		memcpy(&mStart, &w.mStart, sizeof(struct timeval));
		memcpy(&mStop, &w.mStop, sizeof(struct timeval));
		memcpy(&mElapse, &w.mElapse, sizeof(struct timeval));
#else
		mStart.tv_sec = w.mStart.tv_sec;
		mStart.tv_usec = w.mStart.tv_usec;
		mStop.tv_sec = w.mStop.tv_sec;
		mStop.tv_usec = w.mStop.tv_usec;
		mElapse.tv_sec = w.mElapse.tv_sec;
		mElapse.tv_usec = w.mElapse.tv_usec;
#endif
		return *this;
	}

	void Reset() { mElapse.tv_sec = mElapse.tv_usec = 0; }

	void Start() {	gettimeofday(&mStart, NULL); }

	void Stop() 
	{ 
		gettimeofday(&mStop, NULL); 
		
		// Please recall 1s = 1,000,000 usec 
		if (mStop.tv_usec < mStart.tv_usec)
		{
			mElapse.tv_sec += mStop.tv_sec - mStart.tv_sec - 1;
			mElapse.tv_usec += mStop.tv_usec + 1000000 - mStart.tv_usec;
		}
		else
		{
			mElapse.tv_sec += mStop.tv_sec - mStart.tv_sec;
			mElapse.tv_usec += mStop.tv_usec - mStart.tv_usec;
		}
	}

	double GetElapse() 
	{ 
		return (double)mElapse.tv_sec + (1.e-6) * mElapse.tv_usec;
	}

	void GetElapse(unsigned long &sec, unsigned long &usec)
	{
		sec = mElapse.tv_sec;
		usec = mElapse.tv_usec;
	}

	struct timeval mStart;
	struct timeval mStop;
	struct timeval mElapse;
};
#endif

class Timer
{
 public:	

	Timer() : mTZ(), mStart(), mStop() { Reset(); }

	~Timer() { }

	void Reset() 
	{ 
#ifdef WIN32
		mStart = GetTickCount();
#else
		gettimeofday(&mStart, NULL);//&mTZ);
		//mTotal.tv_sec = mTotal.tv_usec = 0; 
#endif
	}
 
	unsigned long GetTicks()
	{
#ifdef WIN32
		mStop = GetTickCount();
		return mStop - mStart;
#else
		gettimeofday(&mStop, NULL);//&mTZ);

#if 0
		if (mStart.tv_usec > mStop.tv_usec) 
		{ 
			mStop.tv_usec = (100000 + mStop.tv_usec); 
			--mStop.tv_sec; 
		}

		return ( ((mStop.tv_sec - mStart.tv_sec)*1000) + 
					(mStop.tv_usec - mStart.tv_usec)/1000); 
#else
		// Please recall 1s = 1,000,000 usec 
		if (mStop.tv_usec < mStart.tv_usec)
		{
			return ( (mStop.tv_sec - mStart.tv_sec - 1) * 1000 +
						(mStop.tv_usec - mStart.tv_usec + 1000000) / 1000 );
		}

		return ( (mStop.tv_sec - mStart.tv_sec) * 1000 +
					(mStop.tv_usec - mStart.tv_usec)/ 1000 );
#endif

#endif
	}

	float GetElapsed()
	{
#ifdef WIN32
		mStop = GetTickCount();
		return mStop - mStart;
#else
		const float uinv = 1 / 1000.0f;

		gettimeofday(&mStop, &mTZ);
			
		if (mStart.tv_usec > mStop.tv_usec) 
		{ 
			mStop.tv_usec = (1000 + mStop.tv_usec); 
			--mStop.tv_sec; 
		} 
			
		//mStop.tv_usec -= mStart.tv_usec; 
		//mStop.tv_sec -= mStart.tv_sec;

		return ( ((float)(mStop.tv_sec - mStart.tv_sec)) +
					((float)((mStop.tv_usec - mStart.tv_usec) * 1000)) * uinv);
#endif
	}

#ifdef WIN32
	DWORD mTZ;
	DWORD mStart;
	DWORD mStop;
#else
	struct timezone mTZ;
	struct timeval mStart;
	struct timeval mStop;
#endif
};

} // namespace mstl

#endif // GUARD__MSTL_TIMER_H_
