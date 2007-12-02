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

#include "ControlPrinter.h"


using namespace freyja3d;

bool ControlPrinter::mLogging = false;
bool ControlPrinter::mInitLogging = false;
mstl::SystemIO::TextFileWriter ControlPrinter::mLog;


void ControlPrinter::PrintArgs(const char *format, va_list *args)
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


void ControlPrinter::Log(const char *format, ...)
{
	if (!format || !format[0])
		return;

	const uint32 sz = 1023;
	char buffer[sz+1];
	va_list args;

	va_start(args, format);
	vsnprintf(buffer, sz, format, args);
	buffer[sz] = 0;
	va_end(args);

	unsigned long l = strlen(buffer);
  
	// Don't process empty strings.
	if (!l || !buffer[0])
		return;

	if (mLogging)
	{
		mLog.Print("> %s\n", buffer);
		mLog.Flush();
	}
}


void ControlPrinter::Print( const char* s )
{
	// Don't process empty strings.
	if ( !s || !s[0] )
		return;

	// Text starting with '!' is sent to stderr was well.
	if ( s[0] == '!' )
	{
		//s[0] = ' ';
		fprintf( stderr, "%s\n", s );
	}

	// Print message to stdout and the status bar.
	mgtk_event_notify_statusbar( s );

	if (mLogging)
	{
		mLog.Print( "> %s\n", s );
		mLog.Flush();
	}
}


void ControlPrinter::Log( mstl::String& s )
{
	// Don't process empty strings.
	if ( s.empty() )
		return;

	if (mLogging)
	{
		mLog.Print("> %s\n", s.c_str() );
		mLog.Flush();
	}
}
