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
 * 2004.01.04:
 * Mongoose - Major API changes, fun updates  =)
 *
 * 2001.11.02:
 * Mongoose - Created
 ===========================================================================*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "Mlisp.h"

#define MLISP_DEBUG_ENABLED

/* TODO:
 *
 * Add quote support and execution stack use
 * Handle scoping?
 * True function parm matching
 * Return Objects instead of ObjectLists
 */


////////////////////////////////////////////////////////////
/// Lisp Object functions //////////////////////////////////
////////////////////////////////////////////////////////////

mObject *newFloatObj(float n)
{
	mObject *obj;
	float *f;


	f = new float;
	*f = n;

	obj = new mObject;
	obj->symbol = NULL;
	obj->type = FLOAT;
	obj->data = (void *)f;	

	return obj;
}


mObject *newIntObj(int n)
{
	mObject *obj;
	int *i;

	i = new int;
	*i = n;

	obj = new mObject;
	obj->symbol = NULL;
	obj->type = INT;
	obj->data = (void *)i;

	return obj;
}


mObject *newStringObj(const char *s)
{
	mObject *obj = NULL;
	unsigned int lenght;


	obj = new mObject;
	obj->symbol = NULL;
	obj->type = CSTRING;
	obj->data = NULL;

	if (!s || !s[0])
	{
		printf("Empty constant string allocated\n");
		return obj;
	}

	lenght = strlen(s);

	obj->data = (void *)new char[lenght+1];
	strncpy((char *)obj->data, s, lenght);
	((char *)obj->data)[lenght] = 0;

	return obj;
}


/* (define FUNC_PTR, PARM_TYPE_LIST, NAME) */
mObject *newFuncObj(mObject *(*f)(mObjectList *), char *name)
{
	mObject *obj = NULL;
	unsigned int lenght;


	if (!name || !name[0])
		return obj;

	lenght = strlen(name);

	obj = new mObject;
	obj->type = FUNC;
	obj->data = (void *)f;
	obj->symbol = new char[lenght+1];
	strncpy(obj->symbol, name, lenght);
	obj->symbol[lenght] = 0;

	return obj;
}


mObject *newListObj(mObjectList *list)
{
	mObject *obj;

	obj = new mObject;
	obj->symbol = NULL;
	obj->type = LIST;
	obj->data = (void *)list;	

	return obj;
}


mObject *newADTObj(unsigned int type, void *data)
{
	mObject *obj = new mObject;

	obj->symbol = NULL;
	obj->type = type;
	obj->data = data;

	if (type < 33)
	{
		printf("ERROR: Likely type index conflict\n");
	}

	return obj;
}


void deleteObj(mObject **object)
{
	if (!(*object) || ((*object)->flags & mObjDisableGarbageCollection))
		return;

	if ((*object)->symbol)
		delete (char *)(*object)->symbol;

	switch ((*object)->type)
	{
	case FUNC:
		break;
	case CSTRING:
		if ((*object)->data)
			delete (char *)(*object)->data;		
		break;
	case INT:
		if ((*object)->data)
			delete (int*)(*object)->data;
		break;
	case FLOAT:
		if ((*object)->data)
			delete (float*)(*object)->data;
		break;
	case LIST:
		while ((mObjectList *)(*object)->data)
		{
			objPop((mObjectList **)&(*object)->data);
		}
		break;
	default:
		;
	}
		
	delete *object;
	*object = NULL;
}


void objPush(mObjectList **list, mObject *object)
{
	mObjectList *newList;


	if (!(*list))
	{
		*list = new mObjectList;
		(*list)->next = NULL;
	}
	else
	{
		newList = new mObjectList;
		newList->next = *list;
		*list = newList;
	}

	(*list)->data = object;
}


mObject *objPop(mObjectList **list)
{
	mObjectList *oldHead;
	mObject *object;


	if (!(*list))
	{
		return NULL;
	}

	object = (*list)->data;

	oldHead = *list;
	*list = (*list)->next;
	delete oldHead;

	return object;
}


mObject *objPeek(mObjectList *list)
{
	if (!list)
	{
		return NULL;
	}

	return list->data;
}


/* Heh, this should be two lists in, one back out */
mObjectList *objAppend(mObject *object, mObjectList *list)
{
	mObjectList *current;


	if (!list)
	{
		list = new mObjectList;
		list->next = NULL;
		list->data = object;
	}
	else
	{
		current = list;

		while (current->next)
			current = current->next;

		current->next = new mObjectList;
		current->next->data = object;
	}

	return list;
}


void deleteObjectList(mObjectList **list)
{
	while (*list)
	{
		objPop(list);
	}
}


void printObj(mObject *obj)
{
	if (!obj)
	{
		printf("NULL Object\n");
		return;
	}

	switch (obj->type)
	{
	case INT:
		printf("int <%s> : %i\n", obj->symbol, *(int *)obj->data);
		break;
	case FLOAT:
		printf("float <%s> : %f\n", obj->symbol, *(float *)obj->data);
		break;
	case CSTRING:
		printf("string <%s> : \"%s\"\n", obj->symbol, (char *)obj->data);
		break;
	case FUNC:
		printf("func <%s>(...)\n", obj->symbol);
		break;
	case LIST:
		printf("list <%s>\n", obj->symbol);
		break;
	case BEGIN:
		printf("SCOPE BEGIN\n");
		break;
	case END:
		printf("SCOPE END\n");
		break;
	default:
		printf("[Unknown type %i] <%s>\n",
			   obj->type, obj->symbol);
	}
}


void printObjList(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;

	if (!list)
	{
		printf("ObjectList is NULL.\n");
		return;
	}

	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		printObj(obj);
	}
}


////////////////////////////////////////////////////////////
/// Internal built-in functions ////////////////////////////
////////////////////////////////////////////////////////////

/* (append ELEMENT LIST )
 * Returns list which is LIST with 
 * ELEMENT appended to the end. */


mObject *numberp(mObjectList *parms)
{
	//if (object->type == INT || object->type == FLOAT)
	//	return (mObject *)1;  /* FIXME: ew bad form old bean */

	return NULL;
}


mObject *nil(mObjectList *parms)
{
	return NULL; // (NULL == parms->data || something)
}


mObject *setq(mObjectList *parms)
{
	return NULL;
}


/* (rest LIST)
 * Returns the list of remaining elements 
 * after the first element of LIST. */
mObject *rest(mObjectList *list)
{
	if (list)
		return newListObj(list->next);

	return NULL;
}

 
/* (endp x)
 * Determines whether x is the empty list, 
 * returning the symbolic atom T if x=(), 
 * or the symbolic atom F if x is nonempty. */

/* (first LIST) 
 * Returns the first element of LIST. */
mObject *first(mObjectList *list)
{
	//if (list)
	//	return list->data;

	return NULL;
}

/* (cons ELEMENT LIST)
 * Returns a new list consisting of the ELEMENT, 
 * followed by the elements of LIST. */
mObject *cons(mObjectList *list)
{
	/* FIXME */

	return NULL;
}


////////////////////////////////////////////////////////////
/// Internal built-in math functions ///////////////////////
////////////////////////////////////////////////////////////

/* Assign value */
mObject *assign(mObjectList *list)
{
	mObjectList *current = list;
	mObject *objA = NULL, *objB = NULL;
	unsigned int lenght;


	objA = objPop(&current);
	objB = objPop(&current);

	if (objA && objB && objA->type == objB->type)
	{
		switch (objA->type)
		{
		case INT:
			objA->data = objB->data;
			break;
		case FLOAT:
			objA->data = objB->data;
			break;
		case CSTRING:
			if ((char *)objA->data && ((char *)objA->data)[0])
			{
				/* Mongoose 2004.01.13, 
				 * Shallow copies only -- could fuck up CVARS otherwise */
				lenght = strlen((char *)objA->data);
				strncpy((char *)objA->data, (char *)objB->data, lenght);
				((char *)objA->data)[lenght] = 0;
			}
			break;
		default:
			objA->data = objB->data;
		}
	}

	return objA;
}


mObject *add(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			n += (*(float *)obj->data);
			break;
		case INT:
			n += (*(int *)obj->data);
			break;
		default:
			;
		}
	}

	return newFloatObj(n); 
}


mObject *subtract(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0, s;
	char first = 1;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			s = (*(float *)obj->data);
			break;
		case INT:
			s = (*(int *)obj->data);
			break;
		default:
			;
		}

		if (first)
		{
			n = s;
			first = 0;
		}
		else
		{
			n -= s;
		}
	}

	return newFloatObj(n); 
}


mObject *multiply(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0, m;
	char first = 1;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			m = (*(float *)obj->data);
			break;
		case INT:
			m = (*(int *)obj->data);
			break;
		default:
			;
		}

		if (first)
		{
			n = m;
			first = 0;
		}
		else
		{
			n *= m;
		}
	}

	return newFloatObj(n); 
}


mObject *divide(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0, d;
	char first = 1;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			d = (*(float *)obj->data);
			break;
		case INT:
			d = (*(int *)obj->data);
			break;
		default:
			;
		}

		if (first)
		{
			first = 0;
			n = d;
		}
		else
		{
			if (d == 0.0)
			{
				printf("Error: Divide by zero avoided in lisp math\n");
			}
			else
			{
				n /= d;
			}
		}
	}

	return newFloatObj(n); 
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Mlisp::Mlisp()
{
	mSymbolTable = NULL;
	mExecStack = NULL;
	mDataStack = NULL;
	mScopeBegin = newADTObj(BEGIN, NULL);
	mScopeEnd = newADTObj(END, NULL);
	mBuffer = NULL;
	mSymbol = NULL;
	mBufferSize = 0;
	mSymbolSize = 65;
	mTop = 0;
	mLook = 0;
	mString = 0;
	mErrors = 0;
	mLine = 1;

	mDebug = 0;
	mGarbageCollection = false;

	/* The most basic procedures:
	 *
	 * (car LIST)     - Given a LIST, return the first element
	 * (cdr LIST)     - Given a LIST, return the second element
	 * (cons OBJ OBJ) - Construct a new LIST given first and second elements
	 * (quote EXPR)   - Denote an expression as literal, do not to interpret
	 * (eq OBJ OBJ)   - Compare two objects for equality, return true or false
	 *
	 * (if <test> <then form> <else form>) - Branch operation
	 *
	 * (define <name> (<parms>) <form>)    - Define functions
	 *
	 * mObject     - Element
	 * mObjectList - List
	 */
	
	/* Append lisp built-in functions to symbol table */
	registerLispFunction("nil", nil);
	registerLispFunctionBuiltIn("setq", &Mlisp::builtin_setq);
	registerLispFunction("first", first);
	registerLispFunction("rest", rest);

	registerLispFunction("+", add);
	registerLispFunction("-", subtract);
	registerLispFunction("*", multiply);
	registerLispFunction("/", divide);

	/* Append lisp built-in variables */
	registerSymbolValue("T", 0);
	registerSymbol("cDebugLevel", INT, (void *)(&mDebug));
}


Mlisp::~Mlisp()
{
	clear();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Mlisp::dumpSymbols()
{
	printf("\n\nDumping symbols:\n");
	printf("------------------------------------------------------------\n");
	printObjList(mSymbolTable);
	printf("------------------------------------------------------------\n");
}


int Mlisp::getSymbolData(char *symbol, unsigned int type, void **data)
{
	mObjectList *current;
	mObject *obj;


	if (!symbol || !symbol[0])
		return -1;

	current = mSymbolTable;

	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		/* If type == 0 then return the whole object */
		if (type == 0) 
		{
			// Mongoose 2002.01.12, FIXME, string
			if (strcmp(symbol, obj->symbol) == 0)
			{
				*data = obj;
				return 0;
			}			
		}
		else if (obj->type == type)
		{
			// Mongoose 2002.01.12, FIXME, string
			if (strcmp(symbol, obj->symbol) == 0)
			{
				*data = obj->data;
				return 0;
			}
		}
	}

	return -2;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Mlisp::setDebugLevel(int level)
{
	mDebug = level;
}


int Mlisp::evalBuffer(const char *buffer)
{
	if (!parseEvalBuffer(buffer))
		eval();

	return 0;
}


int Mlisp::evalFile(const char *filename)
{
	if (mBuffer)
	{
		delete [] mBuffer;
	}

	if (mSymbol)
	{
		delete [] mSymbol;
	}

	if (mSymbolSize < 65)
	{
		mSymbolSize = 65;
	}

	mSymbol = new char[mSymbolSize];

	if (!bufferFile(filename, &mBuffer, &mBufferSize))
	{
		if (!evalBuffer(mBuffer))
		{
			return 0;
		}
	}

	return -1;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool Mlisp::isAtoZ(char c)
{
	return ((unsigned char)c > 64 && (unsigned char)c < 91);
}


bool Mlisp::isatoz(char c)
{
	return ((unsigned char)c > 96 && (unsigned char)c < 123);
}


bool Mlisp::isDigit(char c)
{
	return ((unsigned char)c > 47 && (unsigned char)c < 58);
}


bool Mlisp::isMisc(char c)
{
	if ((unsigned char)c == 95)
		return true;

	return false;
}


bool Mlisp::isNumeric(char *symbol, float *n)
{
	int len, i;
	bool real = false;
	
	
	if (!symbol || !symbol[0])
		return false;
	
	len = strlen(symbol);
	
	for (i = 0; i < len; i++)
	{
		// Mongoose: 2001-10-31 Handle '[0-9]+[.][0-9]+'
		if (i && symbol[i] == '.')
		{
			real = true;
			i++;
		}

		// Mongoose 2002.01.23, Handle '-[0-9]+'
		if (i == 0 && symbol[i] == '-')
		{
			i++;
		}

		if (!isDigit(symbol[i]))
		{
			return false;
		}
	}

	if (real)
	{
		*n = atof(symbol);
	}
	else
	{
		*n = atoi(symbol);
	}

	return true;
}


void Mlisp::printError(char *format, ...)
{
	char buffer[256];
	va_list args;
	unsigned int l;

	
	va_start(args, format);
	vsnprintf(buffer, 128, format, args);

	l = strlen(buffer);
    
	if (!l || !buffer[0])
	{
		return;
	}

	/* Strip message of an trailing carrage return */
	if (buffer[l-1] == '\n')
	{
		buffer[l-1] = 0;
	}
    
	fprintf(stderr, "lisp> ERROR #%i: Line %i, %s\n",
			mErrors, mLine, buffer);
    
	va_end(args);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

int Mlisp::appendSymbolTable(mObject *object)
{
	mSymbolTable = objAppend(object, mSymbolTable);

	return !(mSymbolTable == NULL);
}


void Mlisp::bind(const char *symbol, mObject *data)
{
	int len;


	if (!symbol || !symbol[0] || !data)
	{
		++mErrors;
		printError("BIND receieved invalid symbol for binding");
		deleteObj(&data);
		return;
	}

	/* Make a copy of the symbol to store in Object */
	len = strlen(symbol);
	data->symbol = new char[len + 1];
	strncpy(data->symbol, symbol, len);
	data->symbol[len] = 0;

	/* Append to the lisp object/symbol table */
	appendSymbolTable(data);
}


int Mlisp::bufferFile(const char *filename, 
						 char **buffer, unsigned int *bytes)
{
	FILE *f;


	*buffer = NULL;

	if (!filename || !filename[0])
	{
		return -1;
	}

	f = fopen(filename, "r");

	if (!f)
	{
		perror("fopen> ERROR: ");
		return -2;
	}

	fseek(f, 0, SEEK_END);
	*bytes = ftell(f);
	fseek(f, 0, SEEK_SET);

	/* FIXME: Might want to go ahead and strip comments here,
	 *        and go ahead and do preprocessing instead of
	 *        a straight file buffer.
	 */

	*buffer = new char[*bytes];
	fread(*buffer, *bytes, 1, f);
	fclose(f);

	return 0;
}


void Mlisp::clear()
{
	while (mDataStack)
	{
		objPop(&mDataStack);
	}

	while (mExecStack)
	{
		objPop(&mExecStack);
	}

	while (mSymbolTable)
	{
		mObject *obj = objPeek(mSymbolTable);
		deleteObj(&obj);
		objPop(&mSymbolTable);
	}

	if (mScopeBegin)
	{
		deleteObj(&mScopeBegin);
	}

	if (mScopeEnd)
	{
		deleteObj(&mScopeEnd);
	}

	if (mBuffer)
	{
		delete [] mBuffer;
	}

	if (mSymbol)
	{
		delete [] mSymbol;
	}
}


void Mlisp::eval()
{
	mObject *(*callFunction)(mObjectList *);
	mObjectList *parms = NULL, *currentStack = NULL, *curStack, *cur;
	mObject *obj, *result;
	unsigned int frame = 0;


#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 0)
	{
		printf("\n--------------------------------\n");
		printf("Evaluating lisp\n");
	}
#endif

	while (mExecStack)
	{
		obj = objPop(&mExecStack);

#ifdef MLISP_DEBUG_ENABLED
		if (mDebug > 4)
		{
			/* Not an AR or stack frame, just a stack use counter */
			printf("-frame %u----------------\n", ++frame);
		}
#endif

		if (obj)
		{
			switch (obj->type)
			{
			case BUILTINFUNC:
			case FUNC:
				/* 1. Pop a Scope BEGIN obj */
				obj = objPop(&currentStack);

				if (!objTypeP(obj, BEGIN))
				{
					printf("FATAL ERROR: Lisp data stack corupt... BEGIN !=");
					printObj(obj);
				}

				/* 2. Copy currentStack items to parms stack in same order */
				parms = NULL;
				curStack = currentStack;

				//printf("=\n");
				while (curStack)
				{
					obj = objPeek(curStack);

					if (obj->type == END)
						break;

					if (!parms)
					{
						parms = new mObjectList;
						cur = parms;
					}
					else
					{
						cur->next = new mObjectList;
						cur = cur->next;
					}

					//printObj(obj);						
					cur->data = obj;
					cur->next = NULL;

					curStack = curStack->next;
				}
				//printf("=\n");

				/* 3. Call C / C++ function implementation */
				if (obj->type == FUNC)
				{
					callFunction = (mObject * (*)(mObjectList *))obj->data;
					result = (*callFunction)(parms);
				}
				else
				{
					mObject *(Mlisp::*func)(mObjectList *);
					func = (mObject *(Mlisp::*)(mObjectList *))(*(obj->data));
					result = (this->*func)(parms);
				}

				/* 4. Pop off this entire scope now */
				while ((obj = objPop(&currentStack)))
				{
#ifdef MLISP_DEBUG_ENABLED
					if (mDebug > 4)
					{
						printf("POP ");
						printObj(obj);
					}
#endif
					if (obj->type == END)
						break;

					if (mGarbageCollection)
					{
						/* Weak garbage collect for now */
						deleteObj(&obj);
					}
				}

				/* 5. Push result on stack */
#ifdef MLISP_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("RESULT_PUSH ");
					printObj(result);
				}
#endif
				objPush(&currentStack, result);
				break;
			case END:
				/* END objects mark the end of a function parm list */
#ifdef MLISP_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("END_PUSH\n");
				}
#endif
				objPush(&currentStack, obj);

				//objPush(&mDataStack, newListObj(currentStack));
				//obj = objPeek(mDataStack);
				//currentStack = (mObjectList *)obj->data;
				break;
			case BEGIN:
				/* BEGIN objects mark the start of a function parm list */
#ifdef MLISP_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("BEGIN_PUSH\n");
				}
#endif
				objPush(&currentStack, obj);

				//obj = objPop(&mDataStack);
				//currentStack = (mObjectList *)obj->data;
				//printObjectList(currentStack);
				break;
			default:
				/* Everything else push into the scope list */
#ifdef MLISP_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("DATA_PUSH ");
					printObj(obj);
				}
#endif
				objPush(&currentStack, obj);
			}
		}
		else
		{
			printError("FATAL ERROR: eval() feed NULL data");
			return;
		}
	}

#ifdef MLISP_DEBUG_ENABLED
	  if (mDebug > 0)
	  {
		  printf("--------------------------------\n");
	  }
#endif

	  //dumpListSymbols(currentStack);
	  //objPush(&mStack, newListObj(currentStack));
}


void Mlisp::expected(char c)
{
	char s[4];


	switch (mLook)
	{
	case '\n':
		strncpy(s, "EOL", 4);
		break;
	case '\v':
		strncpy(s, "\\v", 4);
		break;
	case '\t':
		strncpy(s, "TAB", 4);
		break;
	default:
		s[0] = mLook;
		s[1] = 0;
	}

	++mErrors;
	printError("Expected '%c', not '%s' (%u)\nLast symbol before error: '%s'", 
			   c, s, mLook, mSymbol);
}


mObject *Mlisp::getNextSymbol()
{
	mObject *obj;

	parseSeperator();

	if (mLook == ';' && !mString)
	{
		parseComment();
		parseSeperator();
	}

	obj = parseNextSymbol();
	
	return obj;
}


bool Mlisp::is(char c)
{
  return (c == mLook);
}


void Mlisp::lex()
{
  if (mTop < mBufferSize)
  {
	  ++mTop;
	  mLook = mBuffer[mTop];

#ifdef MLISP_DEBUG_ENABLED
	  if (mDebug > 9)
	  {
		  printf("%c", mLook);
		  fflush(stdout);
	  }
#endif

	  if (mLook == ';' && !mString)
	  {
		  parseComment();
	  }
  }
  else
  {
	  mLook = EOF;
  }
}


bool Mlisp::match(char c)
{
  bool r = is(c);


  if (!r)
  {
    expected(c);
  }

  lex();

  return r;
}


void Mlisp::parseComment()
{
	match(';');

	while (!is('\n') && !is(EOF))
	{
		lex();
	}

	if (is(';'))
	{
		parseComment();
	}
}


int Mlisp::parseEvalBuffer(const char *buffer)
{
	mObject *object;


	if (!buffer || !buffer[0])
	{
		return -1;
	}
	
#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 0)
	{
		printf("> Parsing eval buffer...\n");
	}

	if (mDebug > 14)
	{
		printf("> %s\n", buffer);
	}
#endif
	
	/* Start counting lines at 1, errors at 0 */
	mLine = 1; 
	mErrors = 0;

	/* Reset parsing counters */
	mTop = 0;
	mLook = 0;
	mString = 0;

	lex();
	parseSeperator();

	while (is('('))
	{
		object = parseFunction();

#ifdef MLISP_DEBUG_ENABLED
		if (mDebug > 4)
		{
			printf("Pushing, ");
			printObj(object);
		}
#endif

		//objPush(&mExecStack, object);
		parseSeperator();
	}

	if (mErrors)
	{
		printf("\n\nlisp> Encountered %i parse errors\n\n", mErrors);
	}

	return 0;
}


mObject *Mlisp::parseFunction()
{
	mObject *func = NULL, *object = NULL;
	void *data;


	match('(');

	func = getNextSymbol();

	if (!getSymbolData(mSymbol, BUILTINFUNC, &data) && data)
	{
		objPush(&mExecStack, func);
	}
	else if (!getSymbolData(mSymbol, FUNC, &data) && data)
	{
		objPush(&mExecStack, func);
	}
	else
	{
		++mErrors;
		printError("Invalid function '%s'\n", mSymbol);
		return NULL;
	}

#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 4)
	{
		printf("> BEGIN %s\n", func->symbol);
	}
#endif

	objPush(&mExecStack, mScopeBegin);

	/* Handle function parms, must be either FUNC objects or 
	 * the elements must be composed into a object list 
	 * pushed on the stack */
	while (!is(')'))
	{
		object = getNextSymbol();

#ifdef MLISP_DEBUG_ENABLED
		if (mDebug > 4)
		{
			printf("   Pushing parm, ");
			printObj(object);
		}
#endif
		
		if (object && object->type != FUNC)
		{
			objPush(&mExecStack, object);
		}
	}

#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 4)
	{
		printf("> END %s\n", func->symbol);
	}
#endif

	objPush(&mExecStack, mScopeEnd);

	match(')');
	parseSeperator();

	return func;
}


mObject *Mlisp::parseNextSymbol()
{
	int i = 0, j;
	mObject *object = NULL;
	bool string = false;
	bool fp = false;
	float f;


	mSymbol[0] = 0;
	
	if (is('"'))
	{
		lex();
		string = true;
	}
	
	/* Mongoose 2001.11.09: 
	 * Best handle for? grouped (func (func */
	if (is('('))
	{
		return parseFunction();
	}
	
	while (string || (i == 0 && mLook == '-') ||
		   isatoz(mLook) || isAtoZ(mLook) || isDigit(mLook) || isMisc(mLook))
	{
		if (i < (int)mSymbolSize)
		{
			mSymbol[i++] = mLook;
			mSymbol[i] = 0;
		}
		
		if (string && is('"'))
		{
			--i;
			mSymbol[i] = 0;
			lex();
			break;
		}
		
		lex();
		
		if (string)
		{
			continue;
		}
		
		if (is('.'))
		{
			fp = true;
			
			for (j = 0; j < i; ++j)
			{
				if (!j && mSymbol[j] == '-')
					continue;
				
				if (!isDigit(mSymbol[j]))
					break;
			}

			if (i == j)
			{
			  mSymbol[i++] = mLook;
			  mSymbol[i] = 0;
			  lex();
			}
		}
		
		
		// Mongoose 2002.01.23, Hhhmm... fixes -100.00 and -100, 
		//   but is it 'good idea'?
		if (is('-') && i == 0)
		{
			mSymbol[i++] = mLook;
			mSymbol[i] = 0;
			lex();
		}
	}
	
#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 7)
	{
		printf("\n> New symbol <%s>\n", mSymbol);
	}
#endif
	
	if (isNumeric(mSymbol, &f))
	{
		if (fp)
		{
			object = newFloatObj(f);
		}
		else
		{
			object = newIntObj((int)f);
		}
	}
	else
	{
		/* Handle symbol references here */
		if ((getSymbolData(mSymbol, 0, (void **)&object)) == 0)
		{
		}
		else
		{
			if (!string)
			{
				++mErrors;
				printError("Making string out of non-string <%s>?\n", mSymbol);
			}

			object = newStringObj(mSymbol);
		}
	}
	
	return object;
}


void Mlisp::parseSeperator()
{
	// Real whitespace stripping
	while (is(' ') || is('\t') || is('\n'))
	{
		if (is('\n'))
		{
			mLine++;
		}
		
		lex();
	}
}


void Mlisp::parseString(char *string, int len)
{
	int i = 0;
	
	
	match('"');
	
	mString = 1;
	
	len -= 2;
	
	while (!is('"') && !is(EOF))
	{
		if (i < len)
		{
			string[i++] = mLook;
			string[i] = 0;
		}
		
		lex();
	}

	match('"');

	mString = 0;
}


int Mlisp::registerLispFunctionBuiltIn(char *symbol, 
									   mObject *(Mlisp::*func)(mObjectList *))
{
    mObject *obj = NULL;
    unsigned int lenght;
                                                                               
	if (!func || !symbol || !symbol[0])
	{
		printError("ERROR Failed builtin function bind %s\n", symbol);
		return -1;
	}

	lenght = strlen(symbol);

    obj = new mObject;
    obj->type = BUILTINFUNC;
    obj->data = (void *)&func;
    obj->symbol = new char[lenght+1];
    strncpy(obj->symbol, symbol, lenght);
    obj->symbol[lenght] = 0;

	return appendSymbolTable(obj);
}


int Mlisp::registerLispFunction(char *symbol, 
								   mObject *(*func)(mObjectList *))
{
	if (!func)
	{
		return -1;
	}

	return appendSymbolTable(newFuncObj(func, symbol));
}


int Mlisp::registerSymbol(const char *symbol, unsigned int type, void *data)
{
	mObject *object;


	if (!symbol || !symbol[0])
	{
		return -1;
	}

	object = new mObject;
	object->type = type;
	object->data = data;
	object->flags = mObjDisableGarbageCollection;

	bind(symbol, object);

	return 0;
}


int Mlisp::registerSymbolValue(const char *symbol, float d)
{
	mObject* object = newFloatObj(d);


	if (registerSymbolObject(symbol, object) < 0)
	{
		deleteObj(&object);
		return -1;
	}

	object->flags = mObjDisableGarbageCollection;

	return 0;
}


int Mlisp::registerSymbolValue(const char *symbol, int i)
{
	mObject* object = newIntObj(i);


	if (registerSymbolObject(symbol, object) < 0)
	{
		deleteObj(&object);
		return -1;
	}

	return 0;
}

mObject *Mlisp::builtin_setq(mObjectList *parms)
{
	return NULL;
}

int Mlisp::registerSymbolValue(const char *symbol, char *string)
{
	mObject* object = newStringObj(string);


	if (registerSymbolObject(symbol, object) < 0)
	{
		deleteObj(&object);
		return -1;
	}

	return 0;
}


int Mlisp::registerSymbolObject(const char *symbol, mObject *object)
{
	if (!symbol || !symbol[0] || !object)
	{
		printf("Error: Couldn't bind invalid NULL object symbol\n");
		return -1;
	}

	bind(symbol, object);

	return 0;
}


////////////////////////////////////////////////////////////
// Unit Test
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MLISP
mObject *menu(mObjectList *parms)
{
	mObject *xPos, *yPos, *ret, *obj;


	xPos = objPop(&parms);
	yPos = objPop(&parms);
	ret = NULL;
	
	if (objNumberP(xPos) && objNumberP(yPos))
	{
		/* Call event func here - simulated with printf for test */
		printf("extern \"C\" { menu(%f, %f); }\n",
			   getNumber(xPos), getNumber(yPos));

		while ((obj = objPeek(parms)))
		{
			printf("             submenu?, ");
			printObj(obj);
			
			objPop(&parms);
		}

		ret = newStringObj("Menu");
	}
	else
	{
		printf("menu> Failed to extract strict typed data from script\n");
	}
	
	/* Would return menu struct here if it wasn't a simulation */
	return ret; 
}


mObject *submenu(mObjectList *parms)
{
	mObject *label, *obj, *ret = NULL;


	label = objPop(&parms);

	if (objTypeP(label, CSTRING))
	{
		/* Call event func here - simulated with printf in tests */
		printf("extern \"C\" { submenu(\"%s\"); }\n",
				 getString(label));

		while ((obj = objPeek(parms)))
		{
			printf("             menuitem?, ");
			printObj(obj);
			
			objPop(&parms);
		}

		ret = newStringObj(getString(label));
	}
	else
	{
		printf("submenu> Failed to extract strict typed data from script\n");
	}

	/* Would return submenu struct here if it wasn't a simulation */
	return ret;
}


mObject *menu_item(mObjectList *parms)
{
	mObject *label, *event, *ret;


	label = objPop(&parms);
	event = objPop(&parms);
	ret = NULL;

	if (objTypeP(label, CSTRING) && objTypeP(label, INT))
	{
		/* Call event func here - simulated with printf in tests */
		printf("extern \"C\" { menu_item(\"%s\", %i); }\n",
				 getString(label), getInt(event));

		ret = newStringObj(getString(label));
	}
	else
	{
		printObj(label);
		printObj(event);
		printf("menu_item> Failed to extract strict typed data from script\n");
	}

	/* Would return menu_item struct here if it wasn't a simulation */
	return ret;
}


int runUnitTest(int argc, char *argv[])
{
	Mlisp rc;
	int i = 0;


	rc.setDebugLevel(5);

	if (argc <= 1)
	{
		printf("%s FILENAME.LISP\n", argv[0]);
	}
	else if (argc > 1)
	{
		rc.registerLispFunction("menu", menu);
		rc.registerLispFunction("submenu", submenu);
		rc.registerLispFunction("menu_item", menu_item);
		rc.registerSymbol("cMyCounter", INT, (void *)(&i));

		rc.registerSymbolValue("EVENT_SCREENSHOT",   10);
		rc.registerSymbolValue("EVENT_QUIT",         11);
		rc.registerSymbolValue("EVENT_SHOWBONES",    12);
		rc.registerSymbolValue("EVENT_WIREFRAME",    13);
		rc.registerSymbolValue("EVENT_FULLSCREEN",   14);
		rc.registerSymbolValue("EVENT_RES640x480",   15);
		rc.registerSymbolValue("EVENT_RES800x600",   16);
		rc.registerSymbolValue("EVENT_RES1024x768",  17);
		rc.registerSymbolValue("EVENT_RES1280x1024", 18);

		if (argc > 1 && rc.evalFile(argv[1]) < 0)
		{
			printf("Couldn't load file!\n");
		}
		else
		{
			// rc.evalBuffer("(+ 23 42 54 23)");
		}

		rc.dumpSymbols();
	}

	return -1;
}


int main(int argc, char *argv[])
{
	printf("[Mlisp class test]\n");

	return runUnitTest(argc, argv);
}
#endif
