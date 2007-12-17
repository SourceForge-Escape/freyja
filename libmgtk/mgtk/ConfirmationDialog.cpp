/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : ConfirmationDialog
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: Confirmation Dialog class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.04:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>

#include "Resource.h"
#include "ResourceEvent.h"
#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"

#include "ConfirmationDialog.h"

Vector<ConfirmationDialog> gConfirmationDialogs;

namespace mgtk {

bool ExecuteConfirmationDialog(const char *name)
{
	String n(name);
	unsigned int i;

	foreach (gConfirmationDialogs, i)
	{
		if (n == gConfirmationDialogs[i].mName)
		{
			printf("ExecuteConfirmationDialog('%s'): FOUND\n", name);
			return gConfirmationDialogs[i].Execute();
		}
	}

	return false;
}

} // namespace mgtk

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool ConfirmationDialog::AddToPool()
{
	gConfirmationDialogs.push_back( *this );
	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_CONFIRMATIONDIALOG
int runConfirmationDialogUnitTest(int argc, char *argv[])
{
	ConfirmationDialog test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[ConfirmationDialog class test]\n");

	return runConfirmationDialogUnitTest(argc, argv);
}
#endif


////////////////////////////////////////////////////////////
// MLISP RC implementations
////////////////////////////////////////////////////////////

arg_list_t *mgtk_rc_confirmation_dialog(arg_list_t *box)
{
  ConfirmationDialog d;
  arg_list_t *a;

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mName == CSTRING");
  d.mName = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mDialogIcon == CSTRING");
  d.mDialogIcon = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mInformationMessage == CSTRING");
  d.mInformationMessage = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mQuestionMessage == CSTRING");
  d.mQuestionMessage = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mCancelIcon == CSTRING");
  d.mCancelIcon = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mCancelText == CSTRING");
  d.mCancelText = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mAcceptIcon == CSTRING");
  d.mAcceptIcon = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  //MGTK_ASSERTMSG(a, "a.mAacceptText == CSTRING");
  d.mAcceptText = mlisp_get_string(a); 
  delete_arg(&a);

  gConfirmationDialogs.push_back(d);

  return NULL;
}
