/*================================================================
 * 
 * Project : GooseEgg
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

#ifndef __MONGOOSE_GOOSEEGG_PLUGIN_H
#define __MONGOOSE_GOOSEEGG_PLUGIN_H

#include <Python.h>
#include "Egg.h"
#include "List.h"

#define GEGG_PLUGIN_VERSION  "0.0.1"

class PythonPlugin
{
 public:

  PythonPlugin();
  /*-----------------------------------------
   * Created  : 1999-07-31, Mongoose
   * Modified : 
   * 
   * Pre  : 
   * Post : Constructs an object of this class
   -----------------------------------------*/

  ~PythonPlugin();
  /*-----------------------------------------
   * Created  : 1999-07-31, Mongoose
   * Modified : 
   * 
   * Pre  : This object is allocated
   * Post : Deconstructs the object
   -----------------------------------------*/

 private:

  Egg *_egg;                     /* Pointer to the modeler backend  */
};

#endif


