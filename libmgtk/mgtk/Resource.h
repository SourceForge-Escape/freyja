/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Resource
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: Resource (metadata) handler class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001.11.02:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_RESOURCE_H_
#define GUARD__FREYJA_MONGOOSE_RESOURCE_H_

#include <stdio.h>

// Avoiding namespacing for C export
#define ATOM MGTK_C_EXPORTABLE_ATOM
#define INT MGTK_C_EXPORTABLE_INT
#define FLOAT MGTK_C_EXPORTABLE_FLOAT
#define CSTRING MGTK_C_EXPORTABLE_CSTRING
#define FUNC MGTK_C_EXPORTABLE_FUNC

typedef enum
{
	ATOM    = 1,
	LIST    = 2,
	INT     = 4,
	FLOAT   = 8,
	CSTRING = 16,
	FUNC    = 32

} arg_type_t;


typedef struct arg_list_s
{
	struct arg_list_s *next;
	
	arg_type_t type;
	char *symbol;
	void *data;

} arg_list_t;


typedef struct arg_stack_s
{
	struct arg_stack_s *next;
	arg_list_t *data;

} arg_stack_t;


#define get_number(a) (a->type == INT) ? get_int(a) : \
(a->type == FLOAT) ? get_float(a) : 0


#define mlisp_is_cstring(a) (a && a->type == CSTRING)


#define mlisp_get_string(a) \
(a && a->data && a->type == CSTRING) ? (char *)a->data : ""


void symbol_enforce_type(arg_list_t **a, int type);
void delete_arg(arg_list_t **a);
char *get_string(arg_list_t *a);
float get_float(arg_list_t *a);
int get_int(arg_list_t *a);
void new_adt(arg_list_t **a, int type, void *data);
void arg_enforce_type(arg_list_t **a, int type);
arg_list_t *symbol();
char mlisp_peek_for_vargs();
int mlisp_get_line_num();
const char *mlisp_get_filename();

class Resource
{
 public:

	Resource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Resource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Resource();
	/*------------------------------------------------------
	 * Pre  : Resource object is allocated
	 * Post : Deconstructs an object of Resource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	void Flush();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Flushs resource symbol table and other
	 *        allocated memory
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/


  int RegisterFunction(char *symbol, arg_list_t *(*func)(arg_list_t *));
  /*------------------------------------------------------
   * Pre  : Symbol is valid string
	*        Func is a valid and working function
	*
   * Post : Func is bound to symbol
	*        Returns 0 on sucess
   *
   *-- History ------------------------------------------
   *
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/

  int RegisterSymbol(char *symbol, arg_type_t type, void *data);
  /*------------------------------------------------------
   * Pre  : Symbol is valid string
	*        Type is valid for data
	*        Data is valid for type and allocated
	*
   * Post : Symbol is created and bound to symbol table
	*        Returns 0 on sucess
   *
   *-- History ------------------------------------------
   *
	* 2002.01.21:
	* Mongoose - Full type implementation?
	*
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


  arg_list_t *Function(arg_list_t *arg);
  /*------------------------------------------------------
   * Pre  : Arg is valid and allocated arg_list_t
	*
   * Post : Arg is bound to symbol
	*        Returns arg list generated by function
   *
   *-- History ------------------------------------------
   *
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


	void Bind(arg_list_t *symbol, arg_list_t *data);
  /*------------------------------------------------------
   * Pre  : Symbol is a CSTRING type and has string data
	*
   * Post : Symbol's string data becomes bound to Data's
	*        data resource
	*
	*        All passed args are freed
	*
   *-- History ------------------------------------------
   *
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


	arg_list_t *Symbol();
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


	void Seperator();
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

	void RegisterInt(char *symbol, int value);
	/*------------------------------------------------------
	 * Pre  : Symbol is valid and matches Value
	 * Post : Binds symbol to matching variable name in resource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	void String(char *string, int len);
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


	void Error(char *s);
  /*------------------------------------------------------
   * Pre  : S is string containing error being reported
	*
   * Post : Prints error and the line number on 
	*        which in occurs
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


	void Comment();
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


	int Load(char *filename);
  /*------------------------------------------------------
   * Pre  : 
	*
   * Post : Loads a resource file
	*        Inits resource parsing system
   *
   *-- History ------------------------------------------
   *
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


	int BufferFromFile(char *filename, char **buffer, unsigned int *bytes);
  /*------------------------------------------------------
   * Pre  : 
	*
   * Post : Buffers loaded script file
   *
   *-- History ------------------------------------------
   *
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


	int Eval(char *buffer);
  /*------------------------------------------------------
   * Pre  : 
	*
   * Post : Evaluates bufferd and loaded script file
   *
   *-- History ------------------------------------------
   *
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


	int GetEventIdByName(const char *symbol) { return getIntByName(symbol); }
	int getIntByName(const char *symbol);

	bool Lookup(char *symbol, arg_list_t **adt);
	bool Lookup(char *symbol, float *f);
	bool Lookup(char *symbol, int *i);
	bool Lookup(char *symbol, char **s);
  /*------------------------------------------------------
   * Pre  : 
	*
   * Post : Looks up symbol names and returns their values
	*        or a pointer to the adt actual symbol
	*
	*        DO NOT MODIFY THE RETURNED ADT SYMBOL!
	*
	*        The adt is only for supporting lowlevel
	*        binding inside and outside this class
	*
	*        Returns true if lookup succeded
	*        Returns false if lookup failed 
   *
   *-- History ------------------------------------------
   *
	*
	* 2002.01.21:
	* Mongoose - Extention for ADT
	*
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/

	const char *GetFilename() { return mFilename; }
  /*------------------------------------------------------
   * Pre  : 
   * Post : Current script file loaded
   *
   ------------------------------------------------------*/

	int GetLine() { return _line; }
  /*------------------------------------------------------
   * Pre  : 
   * Post : Current line in mlisp source
   *
   ------------------------------------------------------*/

	void Print();
  /*------------------------------------------------------
   * Pre  : 
	*
   * Post : Prints out all symbols in resource system
   *
   *-- History ------------------------------------------
   *
   * 2001.11.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


#ifdef __TEST__
	int _RegressionTest(int argc, char *argv[]);
	/*------------------------------------------------------
	 * Pre  : argc and argv are valid
	 * Post : Tests Resource
	 *
	 *        Returns 0 on sucess, or an error id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
#endif

	static Resource *mInstance;

 private:

	char mFilename[96];

	char *_symbol;  	             /* */

	char *_buffer;

	unsigned int _buffer_len;

	unsigned int _symbol_len;

	unsigned int _top;

	char _look;

	FILE *_tape;

	int _line;

	int _string;

	int _error;

	arg_stack_t *_stack;

	arg_list_t *_sym_tab;
};

int resourceGetEventId1s(const char *name);


#endif
