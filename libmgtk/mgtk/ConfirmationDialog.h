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
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_CONFIRMATIONDIALOG - Builds ConfirmationDialog class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.04.04:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__MGTK_MONGOOSE_CONFIRMATIONDIALOG_H_
#define GUARD__MGTK_MONGOOSE_CONFIRMATIONDIALOG_H_

#include <mstl/String.h>

#include "mgtk_events.h"

class ConfirmationDialog
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	ConfirmationDialog() :
		mName(""),
		mDialogIcon(),
		mInformationMessage(),
		mQuestionMessage(),
		mCancelIcon(),
		mCancelText(),
		mAcceptIcon(),
		mAcceptText()
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of ConfirmationDialog
	 *
	 ------------------------------------------------------*/

	ConfirmationDialog(const char *sName, 
					   const char *sDialogIcon, 
					   const char *sInformationMessage, 
					   const char *sQuestionMessage,
					   const char *sCancelIcon,
					   const char *sCancelText,
					   const char *sAcceptIcon,
					   const char *sAcceptText) :
		mName(sName),
		mDialogIcon(sDialogIcon),
		mInformationMessage(sInformationMessage),
		mQuestionMessage(sQuestionMessage),
		mCancelIcon(sCancelIcon),
		mCancelText(sCancelText),
		mAcceptIcon(sAcceptIcon),
		mAcceptText(sAcceptText)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of ConfirmationDialog
	 *
	 ------------------------------------------------------*/

	ConfirmationDialog(const ConfirmationDialog &d) :
		mName(d.mName),
		mDialogIcon(d.mDialogIcon),
		mInformationMessage(d.mInformationMessage),
		mQuestionMessage(d.mQuestionMessage),
		mCancelIcon(d.mCancelIcon),
		mCancelText(d.mCancelText),
		mAcceptIcon(d.mAcceptIcon),
		mAcceptText(d.mAcceptText)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of ConfirmationDialog
	 *
	 ------------------------------------------------------*/

	ConfirmationDialog &operator =(const ConfirmationDialog &d)
	{
		mName = d.mName;
		mDialogIcon = d.mDialogIcon;
		mInformationMessage = d.mInformationMessage;
		mQuestionMessage = d.mQuestionMessage;
		mCancelIcon = d.mCancelIcon;
		mCancelText = d.mCancelText;
		mAcceptIcon = d.mAcceptIcon;
		mAcceptText = d.mAcceptText;

		return *this;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assignment operator for ConfirmationDialog
	 *
	 ------------------------------------------------------*/

	~ConfirmationDialog() {}
	/*------------------------------------------------------
	 * Pre  : ConfirmationDialog object is allocated
	 * Post : Deconstructs an object of ConfirmationDialog
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properties
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool Execute()
	{
		return mgtk_create_confirm_dialog(mDialogIcon.c_str(), 
										  mInformationMessage.c_str(), 
										  mQuestionMessage.c_str(),
										  mCancelIcon.c_str(),
										  mCancelText.c_str(),
										  mAcceptIcon.c_str(),
										  mAcceptText.c_str());
	}
	/*------------------------------------------------------
	 * Pre  : ConfirmationDialog object is allocated
	 * Post : Generates a blocking confirmation dialog
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	mstl::String mName;

	mstl::String mDialogIcon;
	
	mstl::String mInformationMessage;
	
	mstl::String mQuestionMessage;
	
	mstl::String mCancelIcon;
	
	mstl::String mCancelText;
	
	mstl::String mAcceptIcon;
	
	mstl::String mAcceptText;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

namespace mgtk {

	bool ExecuteConfirmationDialog(const char *name);
}

#endif
