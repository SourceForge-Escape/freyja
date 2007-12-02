/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : ControlPrinter
 * License : No use w/o permission, (C) 2000-2007 Mongoose
 * Comments: Hook in library printers and control messaging, errors, and logs.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.10:
 * Mongoose - Created, reformed from various events and such.
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_CONTROLPRINTER_H_
#define GUARD__FREYJA3D_CONTROLPRINTER_H_

#include <freyja/Printer.h>
#include <hel/math.h>
#include <mstl/SystemIO.h>

#include "freyja_events.h"


namespace freyja3d {

//String s = freyja_rc_map_string(FREYJA_LOG_FILE);
class ControlPrinter : public freyja::Printer
{
public:

	ControlPrinter() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~ControlPrinter() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void PauseLogging() { mLogging = false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void ResumeLogging() { mLogging = mInitLogging; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static bool StartLogging(const char *filename)
	{ mLogging = mInitLogging = mLog.Open(filename); return mInitLogging; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void StopLogging()
	{ mLogging = mInitLogging = false;	mLog.Close(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void Print( const char* s );
	static void PrintArgs(const char *format, va_list *args);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : WARNING: Non-local va_list only works with glibc
	 *
	 ------------------------------------------------------*/

	static void Log( mstl::String& s);
	static void Log(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ErrorArgs(char *format, va_list *args)	
    { PrintArgs(format, args); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void MessageArgs(char *format, va_list *args)
	{ PrintArgs(format, args); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static bool mLogging, mInitLogging;
	static mstl::SystemIO::TextFileWriter mLog;
};


} // namespace freyja3d

#endif // GUARD__FREYJA3D_CONTROLPRINTER_H_
