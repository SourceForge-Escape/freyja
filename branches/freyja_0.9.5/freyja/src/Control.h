/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Control
 * License : No use w/o permission, (C) 2000-2007 Mongoose
 * Comments: This class is an event control and user interface signals.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.10:
 * Mongoose - Created, reformed from various events and such.
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_CONTROL_H_
#define GUARD__FREYJA3D_CONTROL_H_

#include <mstl/SystemIO.h>

#include <hel/math.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ResourceEventDelegate.h>
#include <mgtk/ConfirmationDialog.h>
#include <mgtk/Resource.h> 

#include "ControlPrinter.h"
#include "freyja_events.h"


namespace freyja3d {

class Control
{
public:
	Control() {}

	~Control() {}

	int GetEventIdByName(const char *symbol)
	{ return GetResource().GetEventIdByName(symbol); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Resource &GetResource() { return mResource; } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	int GetResourceInt(const char *symbol)
	{ return (int)GetResourceFloat(symbol); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetResourceInt(const char *symbol, int i)
	{ return SetResourceFloat(symbol, i); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	int LookupEventSymbol(const char *symbol) 
	{ int event; mResource.Lookup(symbol, &event); return event; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	float GetResourceFloat(const char *symbol)
	{ return freyja_event_get_float(LookupEventSymbol(symbol)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetResourceFloat(const char *symbol, float r)
	{ return freyja_event_set_float(LookupEventSymbol(symbol), r); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void GetResourceColor(const char *symbol, 
						  float &r, float &g, float &b, float &a)
	{ mgtk_event_get_color(LookupEventSymbol(symbol), &r, &g, &b, &a); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetResourceColor(const char *symbol, 
						  float r, float g, float b, float a)
	{ mgtk_event_set_color(LookupEventSymbol(symbol), r, g, b, a); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void Print(const char *format, ...)
	{
		// NOTE: This only works for GCC.
		va_list args;
		va_start(args, format);
		mPrinter.PrintArgs(format, &args);
		va_end(args);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void RefreshContext() { freyja_event_gl_refresh(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


protected:

	static ControlPrinter mPrinter;     /* Reroute text msg from backend */

	static Resource mResource;          /* Resource system */

private:

};


} // namespace freyja3d

#endif // GUARD__FREYJA3D_CONTROL_H_
