/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : QueryDialog
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: Query Dialog class 
 * Todo    : Support XML structured dialogs.
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_CONFIRMATIONDIALOG - Builds QueryDialog class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.04.04:
 * Mongoose - Created, for improved locale support and UI design
 ==========================================================================*/

#ifndef GUARD__MGTK_MONGOOSE_QUERYDIALOG_H_
#define GUARD__MGTK_MONGOOSE_QUERYDIALOG_H_

#include <mstl/String.h>
#include <mstl/Vector.h>

#include "mgtk_events.h"

template <typename T>
class QueryDialogValue
{
public:

	QueryDialogValue() : mSymbol(""), mQuestion("") {}

	QueryDialogValue(const char *symbol, const char *question, T v) : 
		mSymbol(symbol), mQuestion(question), mValue(v), mDefault(v) {}

	QueryDialogValue(const QueryDialogValue &d) : 
		mSymbol(d.mSymbol), mQuestion(d.mQuestion), 
		mValue(d.mValue), mDefault(d.mDefault) {}

	QueryDialogValue(T v) { mValue = v; }

	virtual ~QueryDialogValue() {}
	
	QueryDialogValue &operator =(const QueryDialogValue &d) 
	{ 
		mSymbol = d.mSymbol;
		mQuestion = d.mQuestion;
		mValue = d.mValue; 
		mDefault = d.mDefault;
		return *this;
	} 

	mstl::String& GetSymbolString() { return mSymbol; }

	const char *GetSymbol() { return mSymbol.c_str(); }

	const char *GetQuestion() { return mQuestion.c_str(); }

	T Get() { return mValue; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns by *value* the stored data.
	 *
	 ------------------------------------------------------*/

	void Set(T v) { mValue = v; }

	T GetDefault() { return mDefault; }

	void SetDefault(T v) { mDefault = v; }

	mstl::String mSymbol;

	mstl::String mQuestion;

	T mValue;

	T mDefault;
};


class QueryDialog
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	QueryDialog() :
		mName(""),
		mDialogIcon(),
		mInformationMessage(),
		mCancelIcon(),
		mCancelText(),
		mAcceptIcon(),
		mAcceptText()
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of QueryDialog
	 *
	 ------------------------------------------------------*/

	QueryDialog(const char *sName, 
				const char *sDialogIcon, 
				const char *sInformationMessage, 
				const char *sCancelIcon,
				const char *sCancelText,
				const char *sAcceptIcon,
				const char *sAcceptText) :
		mName(sName),
		mDialogIcon(sDialogIcon),
		mInformationMessage(sInformationMessage),
		mCancelIcon(sCancelIcon),
		mCancelText(sCancelText),
		mAcceptIcon(sAcceptIcon),
		mAcceptText(sAcceptText)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of QueryDialog
	 *
	 ------------------------------------------------------*/

	QueryDialog(const QueryDialog &d) :
		mName(d.mName),
		mDialogIcon(d.mDialogIcon),
		mInformationMessage(d.mInformationMessage),
		mCancelIcon(d.mCancelIcon),
		mCancelText(d.mCancelText),
		mAcceptIcon(d.mAcceptIcon),
		mAcceptText(d.mAcceptText),
		mFloats(d.mFloats),
		mInts(d.mInts),
		mTextEntryStrings(d.mTextEntryStrings),
		mTextAreaStrings(d.mTextAreaStrings)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of QueryDialog
	 *
	 ------------------------------------------------------*/

	QueryDialog &operator =(const QueryDialog &d)
	{
		mName = d.mName;
		mDialogIcon = d.mDialogIcon;
		mInformationMessage = d.mInformationMessage;
		mCancelIcon = d.mCancelIcon;
		mCancelText = d.mCancelText;
		mAcceptIcon = d.mAcceptIcon;
		mAcceptText = d.mAcceptText;
		mFloats = d.mFloats;
		mInts = d.mInts;
		mTextEntryStrings = d.mTextEntryStrings;
		mTextAreaStrings = d.mTextAreaStrings;

		return *this;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assignment operator for QueryDialog
	 *
	 ------------------------------------------------------*/

	~QueryDialog() {}
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Deconstructs an object of QueryDialog
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properties
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool Execute();
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Generates a blocking confirmation dialog
	 *
	 ------------------------------------------------------*/

	float GetFloat(const char *symbol);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found or 0.0f
	 *
	 ------------------------------------------------------*/

	int GetInt(const char *symbol);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found or 0
	 *
	 ------------------------------------------------------*/

	const char* GetString(const char* symbol);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found or 0
	 *
	 ------------------------------------------------------*/

	const char* GetTextEntryString(const char* symbol);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found or 0
	 *
	 ------------------------------------------------------*/

	const char* GetTextAreaString(const char* symbol);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found or 0
	 *
	 ------------------------------------------------------*/

	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool SetDefaultString(const char* symbol, const char* value);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found and value set.
	 *
	 ------------------------------------------------------*/

	bool SetDefaultTextEntryString(const char* symbol, const char* value);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found and value set.
	 *
	 ------------------------------------------------------*/

	bool SetDefaultTextAreaString(const char* symbol, const char* value);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found and value set.
	 *
	 ------------------------------------------------------*/

	bool SetString(const char* symbol, const char* value);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found and value set.
	 *
	 ------------------------------------------------------*/

	bool SetTextEntryString(const char* symbol, const char* value);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found and value set.
	 *
	 ------------------------------------------------------*/

	bool SetTextAreaString(const char* symbol, const char* value);
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Returns value if <symbol> found and value set.
	 *
	 ------------------------------------------------------*/

	bool AddToPool();
	/*------------------------------------------------------
	 * Pre  : QueryDialog object is allocated
	 * Post : Add this QueryDialog to the gobal pool.
	 *
	 ------------------------------------------------------*/


	mstl::String mName;

	mstl::String mDialogIcon;
	
	mstl::String mInformationMessage;
	
	mstl::String mCancelIcon;
	
	mstl::String mCancelText;
	
	mstl::String mAcceptIcon;
	
	mstl::String mAcceptText;


	// Explict type support due to strict UI mapping
	mstl::Vector<QueryDialogValue<float> > mFloats;
	mstl::Vector<QueryDialogValue<int> > mInts;
	mstl::Vector<QueryDialogValue<mstl::String> > mTextEntryStrings;
	mstl::Vector<QueryDialogValue<mstl::String> > mTextAreaStrings;

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

	bool ExecuteQueryDialog(const char *name);
}

#endif // GUARD__MGTK_MONGOOSE_QUERYDIALOG_H_
