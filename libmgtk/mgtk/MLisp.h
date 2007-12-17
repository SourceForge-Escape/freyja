/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : MLisp
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongooseichiban@gmail.com
 * Object  : MLisp
 * License : No use w/o permission (C) 2001-2006 Mongoose
 * Comments: MLisp domain language class
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.12.21:
 * Mongoose - Update to meet freyja code guidelines, which makes it easier
 *            to read and automate tasks on.
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

#ifndef GUARD__MLISP_MLISP_H_
#define GUARD__MLISP_MLISP_H_

#include <stdio.h>

extern "C" { // This should be moved into a 'C ABI' header for automation

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
	ADT       = 10

} MlispObjectType;


typedef enum
{
	mObjDisableGarbageCollection = 1

} MlispObjectFlags;


typedef struct MlispObject_s
{
	unsigned int type;
	unsigned char flags;
	char *symbol;
	void *data;

} MlispObject;


typedef struct MlispObjectList_s
{
	struct MlispObjectList_s *next;
	MlispObject *data;

} MlispObjectList;


#define mlisp_get_number(obj) (float)((obj->type == INT) ? (*((int*)(obj->data))) : (obj->type == FLOAT) ? (*((float*)(obj->data))):0)

#define mlisp_get_string(obj) (obj->type == CSTRING) ? (char *)(obj->data) : 0

#define mlisp_get_int(obj) (obj->type == INT) ? (*((int *)(obj->data))) : 0

#define mlisp_obj_typep(obj, objtype) obj ? (obj->type == objtype) : 0 

#define mlisp_obj_numberp(obj) obj ? (obj->type == INT || obj->type == FLOAT) : 0

MlispObject *mlisp_new_list_obj(MlispObjectList *list);

MlispObjectList *mlisp_obj_append(MlispObject *object, MlispObjectList *list);

MlispObject *mlisp_new_obj(unsigned int type, void *data);

MlispObject *mlisp_new_float_obj(float n);

MlispObject *mlisp_new_int_obj(int n);

MlispObject *mlisp_new_str_obj(const char *s);

MlispObject *mlisp_new_func_obj(MlispObject *(*f)(MlispObjectList *), const char *name);

void mlisp_delete_obj(MlispObject **object);

void mlisp_obj_push(MlispObjectList **list, MlispObject *object);

MlispObject *mlisp_obj_peek(MlispObjectList *list);

MlispObject *mlisp_obj_pop(MlispObjectList **list);

}


class MLisp
{
 public:

	typedef enum {
		SetQ = 1
	} builtin_t;

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MLisp();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Mlisp
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~MLisp();
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

	void DumpSymbols();
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

	MlispObject *GetSymbol(const char *symbol);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Object bound to symbol or NULL ( 0x0 )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int GetSymbolData(char *symbol, unsigned int type, void **data);
	/*------------------------------------------------------
	 * Pre  : CAUTION
	 *        This is low level, know what you're doing
	 * 
	 *        Set TYPE to 0 for the whole MlispObject
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

	void SetDebugLevel(int level);
	/*------------------------------------------------------
	 * Pre  : Larger the number, more dumping
	 * Post : Sets debug level
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int EvalBuffer(const char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Parses and evaluates buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int EvalFile(const char *filename);
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

	MlispObject *ParseNextSymbol();
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

	int RegisterLispFunctionBuiltIn(const char *symbol, builtin_t func);
	//int registerLispFunctionBuiltIn(char *symbol, 
	//								MlispObject *(Mlisp::*func)(MlispObjectList *));
	int RegisterLispFunction(const char *symbol, 
							 MlispObject *(*func)(MlispObjectList *));
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

	int RegisterSymbol(const char *symbol, unsigned int type, void *data);
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

	int RegisterSymbolValue(const char *symbol, float d);
	int RegisterSymbolValue(const char *symbol, int i);
	int RegisterSymbolValue(const char *symbol, char *string);
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

	static bool IsAtoZ(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool Isatoz(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool IsDigit(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool IsMisc(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool IsNumeric(char *symbol, float *n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void PrintError(char *format, ...);
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

	int AppendSymbolTable(MlispObject *object);
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

	void Bind(const char *symbol, MlispObject *data);
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

	int BufferFile(const char *filename, char **buffer, unsigned int *bytes);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Buffers loaded script file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Clear();
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
	
	MlispObject *EvalFunction(MlispObjectList **stack, MlispObject *func);
	void Eval();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Evaluates whatever is in the mExecStack
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Expected(char c);
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

	bool Is(char c);
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

	MlispObject *GetNextSymbol();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets the next symbol from eval buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Lex();
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

	bool Match(char c);
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

	int ParseEvalBuffer(const char *buffer);
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

	void ParseComment();
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

	MlispObject *ParseFunction();
	/*------------------------------------------------------
	 * Pre  : Arg is valid and allocated MlispObject
	 *
	 * Post : Arg is bound to symbol
	 *        Returns arg list generated by function
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void ParseSeperator();
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

	void ParseString(char *string, int len);
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

	MlispObject *Builtin_setq(MlispObjectList *parms);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	int RegisterSymbolObject(const char *symbol, MlispObject *object);
	/*------------------------------------------------------
	 * Pre  : Be really careful making valid objects
	 * Post : Registers a premade object to symbol table
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.01.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	MlispObjectList *mSymbolTable;    /* Holds all the symbols, data, func */

	MlispObjectList *mDataStack;      /* Stack of records for internal use */

	MlispObjectList *mExecStack;      /* Execution stack for internal use */

	MlispObject *mScopeBegin;         /* Used to define scope boundries */

	MlispObject *mScopeEnd;           /* Used to define scope boundries */


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
