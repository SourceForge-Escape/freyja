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
 *           template generator script.  <mongooseichban@gmail.com>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_PLUGINCONTROL - Builds PluginControl class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.11.05:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_PLUGIN_H_
#define GUARD__FREYJA3D_PLUGIN_H_

void freyja3d_plugin_application_init(const char* dir);
/*------------------------------------------------------
 * Pre  : 
 * Post : Does setup for application module plugins.
 *
 ------------------------------------------------------*/

void freyja3d_plugin_application_widget_init();
/*------------------------------------------------------
 * Pre  : 
 * Post : Generates the interface widgets for 
 *        application plugins.
 *
 ------------------------------------------------------*/

void freyja3d_plugin_binding(const char* symbol, void* data);
/*------------------------------------------------------
 * Pre  : 
 * Post : Legacy callback for init of application modules.
 *
 ------------------------------------------------------*/

void freyja3d_plugin_draw_loop();
/*------------------------------------------------------
 * Pre  : 
 * Post : Allows plugins to hook into scene rendering by
 *        doing a callback loop into plugins.
 *
 ------------------------------------------------------*/

void freyja3d_plugin_init();
/*------------------------------------------------------
 * Pre  : 
 * Post : Does setup for import/export plugins.
 *
 ------------------------------------------------------*/

void freyja3d_plugin_generate_dialog(const char* filename);
/*------------------------------------------------------
 * Pre  : 
 * Post : Generates a query dialog for import/export
 *        plugin options.
 *
 ------------------------------------------------------*/

void freyja3d_plugin_get_basename(const char* filename, char* basename, uint32 size);
/*------------------------------------------------------
 * Pre  : 
 * Post : Legacy utility function to get the basename of 
 *        the full path filename of a module/plugin and 
 *        strip the file extension. ( '.so', '.dll', etc )
 *
 ------------------------------------------------------*/


#endif // GUARD__FREYJA3D_PLUGIN_H_


