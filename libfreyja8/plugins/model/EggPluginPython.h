/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : GooseEgg, Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Plugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_PLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001-02-24:
 * Mongoose - Created, based on python test code
 *
 ==============================================================*/

#ifndef GUARD__MONGOOSE_GOOSEEGG_PLUGIN_H
#define GUARD__MONGOOSE_GOOSEEGG_PLUGIN_H

#include <Python.h>
#include "EggPlugin.h"


#   ifdef USE_CPP_PYTHON_WRAPPER
class EggPluginPython
{
 public:

  EggPluginPython();
  /*-----------------------------------------
   * Created  : 1999-07-31, Mongoose
   * Modified : 
   * 
   * Pre  : 
   * Post : Constructs an object of this class
   -----------------------------------------*/

  ~EggPluginPython();
  /*-----------------------------------------
   * Created  : 1999-07-31, Mongoose
   * Modified : 
   * 
   * Pre  : This object is allocated
   * Post : Deconstructs the object
   -----------------------------------------*/

 private:
};
#   endif

#endif


