/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Mlisp
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: Mlisp (metadata) handler class
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2004.01.26:
 * Mongoose - API change for function signature
 *
 * 2004.01.04:
 * Mongoose - Major API changes, fun updates  =)
 *
 * 2001.11.02:
 * Mongoose - Created
 ===========================================================================*/

#ifndef GUARD__MIDGARD_MONGOOSE_MLISP_H_
#define GUARD__MIDGARD_MONGOOSE_MLISP_H_

#include <stdio.h>

typedef enum
{
	BOOLEAN   = 1,
	LIST      = 2,
	INT       = 3,
	FLOAT     = 4,
	CSTRING   = 5,
	FUNC      = 6,
	BEGIN     = 7,
	END       = 8,
	BUILTINFUNC = 9,
	RESERVED4 = 10

} mObjectType;


typedef enum
{
	mObjDisableGarbageCollection = 1

} mObjectFlags;


typedef struct mObject_s
{
	unsigned int type;
	unsigned char flags;
	char *symbol;
	void *data;

} mObject;


typedef struct mObjectList_s
{
	struct mObjectList_s *next;
	mObject *data;

} mObjectList;


#define getNumber(obj) (float)((obj->type == INT) ? (*((int*)(obj->data))) : \
							 (obj->type == FLOAT) ? (*((float*)(obj->data))):0)
#define getString(obj) (obj->type == CSTRING) ? (char *)(obj->data) : 0
#define getInt(obj) (obj->type == INT) ? (*((int *)(obj->data))) : 0
#define objTypeP(obj, objtype) obj ? (obj->type == objtype) : 0 
#define objNumberP(obj) obj ? (obj->type == INT || obj->type == FLOAT) : 0 


void deleteObj(mObject **object);
mObject *newADTObj(unsigned int type, void *data);

mObjectList *objAppend(mObject *object, mObjectList *list);
void objPush(mObjectList **list, mObject *object);
mObject *objPop(mObjectList **list);
mObject *objPeek(mObjectList *list);
mObject *newListObj(mObjectList *list);


class Mlisp
{
 public:

	typedef enum {
		SetQ = 1
	} builtin_t;

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mlisp();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Mlisp
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Mlisp();
	/*------------------------------------------------------
	 * Pre  : Mlisp object is allocated
	 * Post : Deconstructs an object of Mlisp
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void dumpSymbols();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Prints out all symbols in mlisp system
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *getSymbol(const char *symbol);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Object bound to symbol or NULL ( 0x0 )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int getSymbolData(char *symbol, unsigned int type, void **data);
	/*------------------------------------------------------
	 * Pre  : CAUTION
	 *        This is low level, know what you're doing
	 * 
	 *        Set TYPE to 0 for the whole mObject
	 *
	 * Post : Gets the address of the symbol's data, 
	 *        func ptr, etc -- so you can do some real damage
	 *
	 *        Returns 0 if there are no errors
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.04: 
	 * Mongoose - Created, replaces Lookup API
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setDebugLevel(int level);
	/*------------------------------------------------------
	 * Pre  : Larger the number, more dumping
	 * Post : Sets debug level
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int evalBuffer(const char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Parses and evaluates buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int evalFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Loads a mlisp file
	 *        Inits mlisp parsing system
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *parseNextSymbol();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Pull the next symbol out of text buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 * Mongoose - Handle bound symbols int/float/string/adt
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int registerLispFunctionBuiltIn(const char *symbol, builtin_t func);
	//int registerLispFunctionBuiltIn(char *symbol, 
	//								mObject *(Mlisp::*func)(mObjectList *));
	int registerLispFunction(const char *symbol, 
							 mObject *(*func)(mObjectList *));
	/*------------------------------------------------------
	 * Pre  : Symbol is valid string
	 *        Func is a valid and working function
	 *
	 * Post : Func is bound to symbol, can now be used by
	 *        mlisp
	 *        Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int registerSymbol(const char *symbol, unsigned int type, void *data);
	/*------------------------------------------------------
	 * Pre  : <Symbol> is valid string symbol
	 *        <Type> is valid for data
	 *        <Data> is valid for type and allocated
	 *
	 * Post : Symbol is created and bound to symbol table.
	 *
	 *        The data is set directly in the object, and
	 *        it is expected that deallocation for this data
	 *        occurs after Mlisp is deallocated.
	 *
	 *        Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.04: 
	 * Mongoose - Shares data by reference with C varible
	 *
	 * 2002.01.21:
	 * Mongoose - Full type implementation?
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int registerSymbolValue(const char *symbol, float d);
	int registerSymbolValue(const char *symbol, int i);
	int registerSymbolValue(const char *symbol, char *string);
	/*------------------------------------------------------
	 * Pre  : Symbol is valid string
	 *
	 * Post : Symbol is created and bound to symbol table
	 *        Returns 0 on sucess, the data value is copied
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 * Mongoose - Full type implementation?
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	static bool isAtoZ(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isatoz(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isDigit(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isMisc(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isNumeric(char *symbol, float *n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void printError(char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Works like printf, but cooler than that
	 *        also limits text to 256 bytes
	 *
	 * Post : Prints error and the line number in 
	 *        which in occurs in mlisp
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	int appendSymbolTable(mObject *object);
	/*------------------------------------------------------
	 * Pre  : <Object> is valid in every field
	 *
	 * Post : Puts the <Object> in the 'symbol table'
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void bind(const char *symbol, mObject *data);
	/*------------------------------------------------------
	 * Pre  : Symbol is a CSTRING type and has string data
	 *
	 * Post : Symbol's string data becomes bound to Data's
	 *        data mlisp
	 *
	 *        All passed args are freed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int bufferFile(const char *filename, char **buffer, unsigned int *bytes);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Buffers loaded script file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Flushs mlisp symbol table and other
	 *        allocated memory
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	mObject *evalFunction(mObjectList *stack, mObject *func);
	void eval();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Evaluates whatever is in the mExecStack
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void expected(char c);
	/*------------------------------------------------------
	 * Pre  : C is char that was expected from the text
	 *        buffer
	 *
	 * Post : Records error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool is(char c);
	/*------------------------------------------------------
	 * Pre  : C is char you're looking for next from text
	 *        buffer
	 *
	 * Post : Compares C to actual next/top char in text
	 *        buffer
	 *
	 *        Returns true if they do match
	 *        Returns false if they don't match
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *getNextSymbol();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets the next symbol from eval buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void lex();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Move text buffer cursor forward
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool match(char c);
	/*------------------------------------------------------
	 * Pre  : C is char you're looking for next from text
	 *        buffer
	 *
	 * Post : Compares C to actual next/top char in text
	 *        buffer, then moves the text buffer cusor to
	 *        next char
	 *
	 *        Returns true if they do match
	 *        Returns false if they don't match
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int parseEvalBuffer(const char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Parses eval buffer and loads the execution 
	 *        stack for eval()
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void parseComment();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Strips comments from text buffer line
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *parseFunction();
	/*------------------------------------------------------
	 * Pre  : Arg is valid and allocated mObject
	 *
	 * Post : Arg is bound to symbol
	 *        Returns arg list generated by function
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void parseSeperator();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Whitespace stripping from buffer line and
	 *        between symbols and seperators
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void parseString(char *string, int len);
	/*------------------------------------------------------
	 * Pre  : String is allocated to buffer the string
	 *        Len is the number of allocated bytes of
	 *        String 
	 *
	 * Post : Handles quote offset string data in text
	 *        buffer  eg "a string"
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *builtin_setq(mObjectList *parms);

	int registerSymbolObject(const char *symbol, mObject *object);
	/*------------------------------------------------------
	 * Pre  : Be really careful making valid objects
	 * Post : Registers a premade object to symbol table
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.01.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	mObjectList *mSymbolTable;    /* Holds all the symbols, data, func */

	mObjectList *mDataStack;      /* Stack of records for internal use */

	mObjectList *mExecStack;      /* Execution stack for internal use */

	mObject *mScopeBegin;         /* Used to define scope boundries */

	mObject *mScopeEnd;           /* Used to define scope boundries */


	/* RDP goodies */

	char *mSymbol;  	          /* Symbol buffer */

	char *mBuffer;                /* Eval buffer */

	unsigned int mBufferSize;     /* Symbol buffer byte size */

	unsigned int mSymbolSize;     /* Eval buffer byte size */

	unsigned int mTop;

	char mLook;

	FILE *mTape;

	int mLine;

	int mString;

	int mErrors;                  /* Parse error counter */

	int mDebug;                   /* Level of debugging for parser */

	bool mGarbageCollection;
};

#endif
