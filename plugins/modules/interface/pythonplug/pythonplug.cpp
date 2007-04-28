/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : perlinnoise plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.23:
 * Mongoose - Created, Based on freyja prototype
 ==========================================================================*/

#include <string.h>
#include <freyja/PythonABI.h>
#include <freyja/PluginABI.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>
#include <mstl/String.h>


extern "C" {

	void freyja_pythonplug_init(void (*func)(const char*, void*));
}

void PythonEventsAttach();
void PythonGUIAttach();

/* This hooks up the plugin to the appliciation */
void freyja_pythonplug_init(void (*func)(const char*, void*))
{
	mgtk::ResourcePlugin *plugin;
	plugin = new mgtk::ResourcePlugin(PythonEventsAttach, PythonGUIAttach);	
}


mstl::String gPythonPluginSymbol = "DefaultAction";
void ePythonPluginSymbol(char *s)
{
	if (s && s[0])
		gPythonPluginSymbol = s;
}


mstl::String gPythonPluginArgs = "  ";
void ePythonPluginArgs(char *s)
{
	if (s && s[0])
		gPythonPluginArgs = s;
}


void eLoadPythonPlugin(char *filename)
{
	if (filename && filename[0])
		freyjaPython1s(filename, 
					   gPythonPluginSymbol.c_str(), gPythonPluginArgs.c_str());
}


void PythonEventsAttach()
{
	ResourceEventCallbackString::add("ePythonPluginSymbol", &ePythonPluginSymbol);
	ResourceEventCallbackString::add("ePythonPluginArgs", &ePythonPluginArgs);
	ResourceEventCallbackString::add("eLoadPythonPlugin", &eLoadPythonPlugin);
}


void PythonGUIAttach()
{
	mstl::String s;

	if (mgtk_get_resource_path() == NULL)
	{
		s = "plugins/pythonplug.mlisp";
	}
	else
	{
		s = mgtk_get_resource_path();
		s += "plugins/pythonplug.mlisp";
	}

#ifdef WIN32
	s = "plugins/pythonplug.mlisp";
#endif

	Resource::mInstance->Load((char*)s.c_str());

	mgtk_textentry_value_set(ResourceEvent::GetResourceIdBySymbol("ePythonPluginSymbol"),
							 gPythonPluginSymbol.c_str());
	mgtk_textentry_value_set(ResourceEvent::GetResourceIdBySymbol("ePythonPluginArgs"), 
							 gPythonPluginArgs.c_str());
}






