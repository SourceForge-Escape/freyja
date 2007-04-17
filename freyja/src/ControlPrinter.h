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

	~ControlPrinter() {}

	static void PauseLogging() { mLogging = false; }

	static void ResumeLogging() { mLogging = mInitLogging; }

	static bool StartLogging(const char *filename)
	{
		mLogging = mInitLogging = mLog.Open(filename);
		return mInitLogging;
	}

	static void StopLogging()
	{
		mLogging = mInitLogging = false;
		mLog.Close();
	}

	// WARNING: Non-local va_list only works with glibc
	static void PrintArgs(const char *format, va_list *args)
	{
		const uint32 sz = 1023;
		char buffer[sz+1];
		vsnprintf(buffer, sz, format, *args);
		buffer[sz] = 0;

		unsigned long l = strlen(buffer);
  
		// Don't process empty strings.
		if (!l || !buffer[0])
			return;

		// Strip message off trailing carrage return.
		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		// Text starting with '!' is sent to stderr was well.
		if (buffer[0] == '!')
		{
			buffer[0] = ' ';
			fprintf(stderr, "%s\n", buffer);
		}

		// Print message to stdout and the status bar.
		mgtk_event_notify_statusbar(buffer);

		if (mLogging)
		{
			mLog.Print("> %s\n", buffer);
			mLog.Flush();
		}
	}

	virtual void ErrorArgs(char *format, va_list *args)
	{
		PrintArgs(format, args);
	}


	virtual void MessageArgs(char *format, va_list *args)
	{
		PrintArgs(format, args);
	}

	static bool mLogging, mInitLogging;
	static mstl::SystemIO::TextFileWriter mLog;
};


} // namespace freyja3d

#endif // GUARD__FREYJA3D_CONTROLPRINTER_H_
