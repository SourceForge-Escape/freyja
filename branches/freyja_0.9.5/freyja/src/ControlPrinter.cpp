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

