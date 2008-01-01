/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : PluginControl
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.11.05:
 * Mongoose - Created
 ==========================================================================*/

#include "config.h"

#include <mstl/String.h>
#include <mstl/SystemIO.h>

#include <freyja/Plugin.h>
#include <freyja/PluginABI.h>
#include <freyja/SkeletonABI.h>

#include <mgtk/QueryDialog.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ResourceEventDelegate.h>
#include <mgtk/ConfirmationDialog.h>

#include "freyja3d_scenegraph.h"
#include "Control.h"
#include "Plugins.h"

using namespace freyja3d;

void ePluginImport(ResourceEvent *e);

void ePluginExport(ResourceEvent *e);


void freyja3d_plugin_application_init(const char* dir)
{
#if FREYJA_APP_PLUGINS
	SystemIO::FileReader reader;
	char module_name[128];
	char module_symbol[128];
	void (*init)(void (*func)(const char*, void*));
	bool done = false;
	const char *module_filename;
	void *handle;

	FREYJA3D_LOG("[Freyja application plugin system invoked]");

	if (!reader.OpenDir(dir))
	{
		freyja3d_print("!Couldn't access application plugin directory.");
		return;
	}

	while (!done && (module_filename = reader.GetNextDirectoryListing()))
	{
		if (reader.IsDirectory(module_filename))
			continue;

		if (!SystemIO::CheckModuleExt(module_filename))
			continue;

		if (!(handle = freyjaModuleLoad(module_filename)))
		{
			continue; /* Try the next plugin, even after a bad module load */
		}
		else
		{
			freyja3d_plugin_get_basename(module_filename, module_name, 128);
			snprintf(module_symbol, 128, "freyja_%s_init", module_name);
			FREYJA3D_LOG("Module '%s' opened.", module_filename);

			init = (void (*)(void (*)(const char*, void*)))freyjaModuleImportFunction(handle, module_symbol);
			FREYJA3D_LOG("Module '%s' symbol '%s' %simported.", module_filename, module_symbol, init ? "" : "not ");

			if (init == NULL)  
			{
				freyjaModuleUnload(handle);
				continue;
			}

			/* Call plugin's init function */
	      	(*init)(freyja3d_plugin_binding);

			FREYJA3D_LOG("Module '%s' linked.", module_filename);

			// Keep these plugins in memory...
			//freyjaModuleUnload(handle);
		}
	}

	reader.CloseDir();

	FREYJA3D_LOG("[Freyja application plugin loader sleeps now]\n");

#else
	FREYJA3D_LOG("%s:%i -- This build was compiled w/o plugin support.", __FILE__, __LINE__);
#endif

	/* Hook plugins into the resource event subsystem. */
	{
		uint32 i, n = mgtk::ResourcePlugin::mPlugins.size();
		for (i = 0; i < n; ++i)
		{
			if ( mgtk::ResourcePlugin::mPlugins[i] != NULL )
			{
				mgtk::ResourcePlugin::mPlugins[i]->mEventsAttach();
			}
		}
	}
}


void freyja3d_plugin_application_widget_init()
{
	/* FreyjaAppPlugin prototype testing... */
	uint32 i, n = mgtk::ResourcePlugin::mPlugins.size();
	
	for (i = 0; i < n; ++i)
	{
		if (mgtk::ResourcePlugin::mPlugins[i] != 0x0)
		{
			mgtk::ResourcePlugin::mPlugins[i]->mGUIAttach();
		}
	}
}


void freyja3d_plugin_binding(const char* symbol, void* data)
{
	/* Disabled. */
	data = 0x0;
}


void freyja3d_plugin_draw_loop()
{
	/* FreyjaAppPlugin prototype testing... */
	for (uint32 i = 0, n = mgtk::ResourcePlugin::mPlugins.size(); i < n; ++i)
	{
		if (mgtk::ResourcePlugin::mPlugins[i] != 0x0)
		{
			mgtk::ResourcePlugin::mPlugins[i]->Draw();
		}
	}
}


void freyja3d_plugin_init()
{
	/* Add extra paths for freyja plugins. */
	String sPluginDir = freyja_rc_map_string("plugins/");
	//freyjaPluginAddDirectory( sPluginDir.c_str() );

	/* Query for external plugins and rebuild the 'database'. */
	freyjaPluginsInit();
		
	/* Model import/export menus and file dialog pattern setup. */

	int loadEventId = Control::GetEventIdByName("eOpenModel");
	int saveEventId = Control::GetEventIdByName("eSaveModel");

	mgtk_event_fileselection_append_pattern(loadEventId, 
											"All Files (*.*)", "*.*");

	mgtk_event_fileselection_append_pattern(saveEventId, 
											"All Files (*.*)", "*.*");

	/* Create sorted mapping for import/export menus and filters. */
	{
		unsigned int count = freyjaGetPluginCount();

		unsigned int import_map[count];
		unsigned int import_count = 0;

		unsigned int export_map[count];
		unsigned int export_count = 0;

		for (unsigned int i = 0; i < count; ++i)
		{
			freyja::PluginDesc* plugin = freyjaGetPluginClassByIndex( i );
			
			if (plugin == NULL)
				continue;

			FREYJA3D_LOG("%i/%i %i. [%s] %s, %s%s", i, count, 
						 plugin->GetId(),
						 plugin->mName.c_str(),
						 plugin->mFilename.c_str(),
						 plugin->mImportFlags ? "Import " : "",
						 plugin->mExportFlags ? "Export " : "");
				
			/* Must be a valid, external module plugin to be mapped. */
			if ( plugin->mFilename.c_str() && plugin->mName.c_str() )
			{
				if ( plugin->mImportFlags )
				{
					import_map[import_count++] = i;						
				}

				if ( plugin->mExportFlags )
				{
					export_map[export_count++] = i;	
				}
			}
		}

		// FIXME: Break these sort/create loops out into functions to reduce duplicate code.

		/* Sort export menu labels. */
		for (unsigned int i = 0; i < export_count; ++i)
		{
			freyja::PluginDesc* pluginI = freyjaGetPluginClassByIndex( export_map[i] );

			for (unsigned int j = i; j < export_count; ++j)
			{
				freyja::PluginDesc* pluginJ = freyjaGetPluginClassByIndex( export_map[j] );

				if ( pluginJ->mDescription <= pluginI->mDescription )
				{
					unsigned int tmp = export_map[i];
					export_map[i] = export_map[j];
					export_map[j] = tmp;

					pluginI = pluginJ;
				} 
			}
		}

		/* Generate export menu and save dialog drop-down. */
		int exportEventId = Control::GetEventIdByName("eExportFile");
		for (unsigned int i = 0; i < export_count; ++i)
		{
			int id = export_map[i];
			freyja::PluginDesc* plugin = freyjaGetPluginClassByIndex( id );
			//Print("!Export %i. { %i, %s }\n", i, id, (plugin) ? plugin->mDescription.c_str() : "[?]" );

			/* Append to menu in sorted order. */
			if ( plugin )
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();

				// Append filter to generic filedialog.
				mgtk_event_fileselection_append_pattern(saveEventId, desc, ext);

				// Generate menu item.
				mstl::String s;
				s.Set("%i|%s-ePluginExport", 
					  plugin->GetId(), plugin->mName.c_str());
				ResourceEventCallback2::add(s.c_str(), &ePluginExport);
				int EvExportModelId = Control::LookupEventSymbol(s.c_str());
				mgtk_append_item_to_menu(exportEventId, desc, EvExportModelId);
			}

		}
			
		/* Sort import menu labels. */
		for (unsigned int i = 0; i < import_count; ++i)
		{
			freyja::PluginDesc* pluginI = freyjaGetPluginClassByIndex( import_map[i] );

			for (unsigned int j = i; j < import_count; ++j)
			{
				freyja::PluginDesc* pluginJ = freyjaGetPluginClassByIndex( import_map[j] );

				if ( pluginJ->mDescription <= pluginI->mDescription )
				{
					unsigned int tmp = import_map[i];
					import_map[i] = import_map[j];
					import_map[j] = tmp;

					pluginI = pluginJ;
				} 
			}
		}

		/* Generate import menu and open dialog drop-down. */
		int importEventId = Control::GetEventIdByName("eImportFile");
		for (unsigned int i = 0; i < import_count; ++i)
		{
			int id = import_map[i];
			freyja::PluginDesc* plugin = freyjaGetPluginClassByIndex( id );
			//Print("!Import %i. { %i, %s }\n", i, id, (plugin) ? plugin->mDescription.c_str() : "[?]" );

			/* Append to menu in sorted order. */
			if ( plugin )
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();

				// Append filter to generic filedialog.
				mgtk_event_fileselection_append_pattern(loadEventId, desc, ext);

				// Generate menu item.
				mstl::String s;
				s.Set("%i|%s-ePluginImport", 
					  plugin->GetId(), plugin->mName.c_str());
				ResourceEventCallback2::add(s.c_str(), &ePluginImport);
				int EvImportModelId = Control::LookupEventSymbol(s.c_str());
				mgtk_append_item_to_menu(importEventId, desc, EvImportModelId);
			}
		}

	}


	/* FIXME: Rework image and shader plugin systems to work 
	   like external model plugins were possible. */

	/* Image file dialog patterns - texture */
	uint32 eOpenTextureId = Control::GetEventIdByName("eOpenTexture");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"All Files (*.*)", "*.*");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"BMP Image (*.bmp)", "*.bmp");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"DDS Image (*.dds)", "*.dds");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"JPEG Image (*.jpg)", "*.jpg");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"PCX Image (*.pcx)", "*.pcx");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"PNG Image (*.png)", "*.png");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"PPM Image (*.ppm)", "*.ppm");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"TARGA Image (*.tga)", "*.tga");

	/* Shader file dialog patterns */
	uint32 eOpenShaderId = Control::GetEventIdByName("eOpenShader");
	mgtk_event_fileselection_append_pattern(eOpenShaderId, 
											"All Files (*.*)", "*.*");
	mgtk_event_fileselection_append_pattern(eOpenShaderId, 
											"GLSL Fragment (*.frag)", "*.frag");
	mgtk_event_fileselection_append_pattern(eOpenShaderId, 
											"ARB Fragment (*.frag)", "*.frag");
}


void freyja3d_plugin_generate_dialog(const char* filename)
{
	/* Generate query dialogs for module options for import/export settings. */
	freyja::PluginDesc* plugin = freyjaGetPluginClassByFilename(filename);

	if (plugin)
	{
		QueryDialog d;
		d.mName = "Plugin options"; 
		d.mDialogIcon = "gtk-question"; 
		d.mInformationMessage.Set( "Plugin: %s", plugin->mName.c_str() ); 
		d.mCancelIcon = "gtk-cancel"; 
		d.mCancelText = "Cancel"; 
		d.mAcceptIcon = "gtk-ok"; 
		d.mAcceptText = "Ok"; 

		uint32 i;
		foreach (plugin->mArgs, i)
		{
			if (plugin->mArgs[i].GetStringType() == "float")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				float value = plugin->GetFloatArg(plugin->mArgs[i].GetName());

				QueryDialogValue<float> v(symbol.c_str(), question.c_str(), value);
				d.mFloats.push_back(v);	
			}
			else if (plugin->mArgs[i].GetStringType() == "int")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				int value = plugin->GetIntArg(plugin->mArgs[i].GetName());

				QueryDialogValue<int> v(symbol.c_str(), question.c_str(), value);
				d.mInts.push_back(v);	
			}
		}

		/* Run generated dialog. */
		d.Execute();

		/* Update plugin settings from dialog input. */
		foreach (plugin->mArgs, i)
		{
			if (plugin->mArgs[i].GetStringType() == "float")
			{
				float value = d.GetFloat(plugin->mArgs[i].GetName());
				mstl::String s;
				s.Set("%f", value);
				plugin->mArgs[i].SetValue(s.c_str());
			}
			else if (plugin->mArgs[i].GetStringType() == "int")
			{
				int value = d.GetInt(plugin->mArgs[i].GetName());
				mstl::String s;
				s.Set("%i", value);
				plugin->mArgs[i].SetValue(s.c_str());
			}
		}
	}
}


// Get the basename of the full path name and strip ext '.so', '.dll', etc
void freyja3d_plugin_get_basename(const char* filename, char* basename, uint32 size)
{
	uint32 i, j, l = strlen(filename);
	char c;


	for (i = 0, j = 0; i < l; ++i)
	{
		c = filename[i];

		switch (c)
		{
		case '\\':
		case '/':
			j = 0;
			basename[0] = 0;
			break;
		
		default:
			if (j < size-1)
			{
				basename[j++] = c;
				basename[j] = 0;
			}
		}
	}

	for (; i > 0; --i)
	{
		c = basename[i];

		if (c == '.')
			break;
	}

	if (i > 0)
		basename[i] = 0;

	//freyjaPrintMessage("! %s", basename);
}


void ePluginImport(ResourceEvent *e)
{
	if (!e || !e->getName())
		return;

	mstl::String s = e->getName();

	s.Replace('|', 0);

	int idx = atoi(s.c_str());

	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(idx);
			
	if (plugin)
	{
		QueryDialog d;
		d.mName = e->getName(); 
		d.mDialogIcon = "gtk-question"; 
		d.mInformationMessage.Set("Plugin Import: %s", 
								  plugin->mName.c_str()); 
		d.mCancelIcon = "gtk-cancel"; 
		d.mCancelText = "Cancel"; 
		d.mAcceptIcon = "gtk-ok";
		d.mAcceptText = "Import"; 

		uint32 i;
		foreach (plugin->mArgs, i)
		{
			if (plugin->mArgs[i].GetStringType() == "float")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				float value = 0.0f;
				value = plugin->GetFloatArg(plugin->mArgs[i].GetName());

				QueryDialogValue<float> v(symbol.c_str(), question.c_str(), value);
				d.mFloats.push_back(v);	
			}
			else if (plugin->mArgs[i].GetStringType() == "int")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				int value = 0;
				value = plugin->GetIntArg(plugin->mArgs[i].GetName());

				QueryDialogValue<int> v(symbol.c_str(), question.c_str(), value);
				d.mInts.push_back(v);	
			}
		}



		/* Generate File dialog here with filters for export type. */
		mstl::String title;
		title.Set("Importing %s - Freyja", plugin->mName.c_str());
		char *filename =
		mgtk_filechooser_blocking(title.c_str(),  
								  freyja_rc_map_string("/").c_str(), 
								  0,
								  plugin->mDescription.c_str(),
								  plugin->mExtention.c_str());
		if ( filename )
		{
			/* Generate blocking dialog for plugin options. */
			if ( !plugin->mArgs.size() || d.Execute() )
			{
				/* Update plugin settings from dialog input. */
				foreach (plugin->mArgs, i)
				{
					if (plugin->mArgs[i].GetStringType() == "float")
					{
						float value = d.GetFloat(plugin->mArgs[i].GetName());
						mstl::String s;
						s.Set("%f", value);
						plugin->mArgs[i].SetValue(s.c_str());
					}
					else if (plugin->mArgs[i].GetStringType() == "int")
					{
						int value = d.GetInt(plugin->mArgs[i].GetName());
						mstl::String s;
						s.Set("%i", value);
						plugin->mArgs[i].SetValue(s.c_str());
					}
				}
			}

			freyja3d_print("! Importing: '%s'\n", filename);

			if (!freyjaImportModelByModule(filename, 
										   plugin->mFilename.c_str()))
			{
				if (plugin->mImportFlags & FREYJA_PLUGIN_PAK_VFS)
				{
					// FIXME: Old school plugin callback
					uint32 id = ResourceEvent::GetResourceIdBySymbol("eDialogPakReader");
					ResourceEvent::listen(id);
					id = ResourceEvent::GetResourceIdBySymbol("ePakReaderMenuUpdate");
					ResourceEvent::listen(id);

					FREYJA_INFOMSG(0, "Click on the Plugins Tab and expand PakBrowser to browse the pak file.\nClick the Reset VFS button if the vfs doesn't automaticaly load for you.\nIf the file doesn't open automatically from the pak\nit will likely still be written to /tmp/utpak \nor ./utpak to load from another plugin.");
				}
				else
				{
					freyja3d_record_saved_model(filename);
					freyja3d_print("! Imported: '%s'\n", filename);

#if FIXME
					// Update skeletal UI
					if (freyjaGetCurrentSkeleton() == INDEX_INVALID &&
						freyjaGetSkeletonCount() > 0)
					{
						freyjaCurrentSkeleton(0);
					}
#endif
					freyja3d_scenegraph_update();
				}
			}
			else
			{
				freyja3d_print("! Failed to import: '%s'\n", filename);
			}

			mgtk_filechooser_blocking_free(filename);
		}
	}
}


void ePluginExport(ResourceEvent *e)
{
	if (!e || !e->getName())
		return;

	mstl::String s = e->getName();

	s.Replace('|', 0);

	int idx = atoi(s.c_str());

	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(idx);
			
	if (plugin)
	{
		QueryDialog d;
		d.mName = e->getName(); 
		d.mDialogIcon = "gtk-question"; 
		d.mInformationMessage.Set("Plugin Export: %s", 
								  plugin->mName.c_str()); 
		d.mCancelIcon = "gtk-cancel"; 
		d.mCancelText = "Cancel"; 
		d.mAcceptIcon = "gtk-ok"; 
		d.mAcceptText = "Export"; 

		uint32 i;
		foreach (plugin->mArgs, i)
		{
			if (plugin->mArgs[i].GetStringType() == "float")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				float value = plugin->GetFloatArg(plugin->mArgs[i].GetName());

				QueryDialogValue<float> v(symbol.c_str(), question.c_str(), value);
				d.mFloats.push_back(v);	
			}
			else if (plugin->mArgs[i].GetStringType() == "int")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				int value = plugin->GetIntArg(plugin->mArgs[i].GetName());

				QueryDialogValue<int> v(symbol.c_str(), question.c_str(), value);
				d.mInts.push_back(v);	
			}
		}


		/* Generate file dialog with filter for export type. */
		mstl::String title;
		title.Set("Exporting %s - Freyja", plugin->mName.c_str());
		char *filename =
		mgtk_filechooser_blocking(title.c_str(),  
								  freyja_rc_map_string("/").c_str(), 
								  1,
								  plugin->mDescription.c_str(),
								  plugin->mExtention.c_str());
		
		if ( filename )
		{
			if (!plugin->mArgs.size() || d.Execute())
			{
				// Update plugin settings from dialog input.
				foreach (plugin->mArgs, i)
				{
					if (plugin->mArgs[i].GetStringType() == "float")
					{
						float value = d.GetFloat(plugin->mArgs[i].GetName());
						mstl::String s;
						s.Set("%f", value);
						plugin->mArgs[i].SetValue(s.c_str());
					}
					else if (plugin->mArgs[i].GetStringType() == "int")
					{
						int value = d.GetInt(plugin->mArgs[i].GetName());
						mstl::String s;
						s.Set("%i", value);
						plugin->mArgs[i].SetValue(s.c_str());
					}
				}
			}


#if 0 // This was part of some older forced extention code.
			{
				String ext = filename;

				int pos = ext.find_last_of('.');

				if (pos == String::npos)
				{
					
				}
			}
#endif
			freyja3d_print("! Exporting: '%s'\n", filename);
				
			if (!freyjaExportModelByModule(filename, 
										   plugin->mFilename.c_str()))
			{				   
				freyja3d_record_saved_model(filename);
				freyja3d_print("! Exported: '%s'\n", filename);
			}
			else
			{
				freyja3d_print("! Failed to export: '%s'\n", filename);
			}

			mgtk_filechooser_blocking_free(filename);
		}
	}
}


