/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : MLisp
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : MLisp
 * License : No use w/o permission (C) 2001-2005 Mongoose
 * Comments: MLisp domain language class
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "MLisp.h"


#define MLISP_DEBUG_ENABLED

/* TODO:
 *
 * 	o Eval shouldn't buffer the way it does - setq's
 *    bind in reverse order eg not line 1, 2, 3 but 3, 2, 1
 *	  ( FIX by executing in true scopes )
 *
 * Add quote support and execution stack use
 * Handle scoping?
 * True function parm matching
 * Return Objects instead of ObjectLists
 */


////////////////////////////////////////////////////////////
/// Lisp Object functions //////////////////////////////////
////////////////////////////////////////////////////////////

MlispObject *mlisp_new_float_obj(float n)
{
	MlispObject *obj;
	float *f;


	f = new float;
	*f = n;

	obj = new MlispObject;
	obj->symbol = NULL;
	obj->type = FLOAT;
	obj->data = (void *)f;
	obj->flags = 0;	

	return obj;
}


MlispObject *mlisp_new_int_obj(int n)
{
	MlispObject *obj;
	int *i;

	i = new int;
	*i = n;

	obj = new MlispObject;
	obj->symbol = NULL;
	obj->type = INT;
	obj->data = (void *)i;
	obj->flags = 0;

	return obj;
}


MlispObject *mlisp_new_str_obj(const char *s)
{
	MlispObject *obj = NULL;
	unsigned int lenght;


	obj = new MlispObject;
	obj->symbol = NULL;
	obj->type = CSTRING;
	obj->data = NULL;
	obj->flags = 0;

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
MlispObject *mlisp_new_func_obj(MlispObject *(*f)(MlispObjectList *), const char *name)
{
	MlispObject *obj = NULL;
	unsigned int lenght;


	if (!name || !name[0])
		return obj;

	lenght = strlen(name);

	obj = new MlispObject;
	obj->type = FUNC;
	obj->data = (void *)f;
	obj->symbol = new char[lenght+1];
	strncpy(obj->symbol, name, lenght);
	obj->symbol[lenght] = 0;
	obj->flags = 0;

	return obj;
}


MlispObject *mlisp_new_list_obj(MlispObjectList *list)
{
	MlispObject *obj;

	obj = new MlispObject;
	obj->symbol = NULL;
	obj->type = LIST;
	obj->data = (void *)list;
	obj->flags = 0;	

	return obj;
}


MlispObject *mlisp_new_obj(unsigned int type, void *data)
{
	MlispObject *obj = new MlispObject;

	obj->symbol = NULL;
	obj->type = type;
	obj->data = data;
	obj->flags = 0;

	return obj;
}


void mlisp_delete_obj(MlispObject **object)
{
	if (!(*object) || ((*object)->flags & mObjDisableGarbageCollection))
		return;

	if ((*object)->symbol)
		delete [] (char *)(*object)->symbol;

	switch ((*object)->type)
	{
	case FUNC:
		break;

	case BUILTINFUNC:
		if ((*object)->data)
			delete [] (char *)(*object)->data;
		break;

	case CSTRING:
		if ((*object)->data)
			delete [] (char *)(*object)->data;		
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
		while ((MlispObjectList *)(*object)->data)
		{
			mlisp_obj_pop((MlispObjectList **)&(*object)->data);
		}
		break;

	default:
		;
	}
		
	delete *object;
	*object = NULL;
}


void mlisp_obj_push(MlispObjectList **list, MlispObject *object)
{
	MlispObjectList *newList;


	if (!(*list))
	{
		*list = new MlispObjectList;
		(*list)->next = NULL;
	}
	else
	{
		newList = new MlispObjectList;
		newList->next = *list;
		*list = newList;
	}

	(*list)->data = object;
}


MlispObject *mlisp_obj_pop(MlispObjectList **list)
{
	MlispObjectList *oldHead;
	MlispObject *object;


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


MlispObject *mlisp_obj_peek(MlispObjectList *list)
{
	if (!list)
	{
		return NULL;
	}

	return list->data;
}


/* Heh, this should be two lists in, one back out */
MlispObjectList *mlisp_obj_append(MlispObject *object, MlispObjectList *list)
{
	MlispObjectList *current = 0x0;


	if (!list)
	{
		list = new MlispObjectList;
		list->next = NULL;
		list->data = object;
	}
	else
	{
		current = list;

		while (current->next)
			current = current->next;

		current->next = new MlispObjectList;
		current->next->next = NULL;
		current->next->data = object;
	}

	return list;
}


void mlisp_delete_list_obj(MlispObjectList **list)
{
	while (*list)
	{
		mlisp_obj_pop(list);
	}
}


void mlisp_print_obj(MlispObject *obj)
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
	case BUILTINFUNC:
		printf("builtin_func <%s>(...)\n", obj->symbol);
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


void mlisp_print_list_obj(MlispObjectList *list)
{
	MlispObjectList *current = list;
	MlispObject *obj;


	if (!list)
	{
		printf("ObjectList is NULL.\n");
		return;
	}

	while (current)
	{
		obj = mlisp_obj_peek(current);
		current = current->next;

		mlisp_print_obj(obj);
	}
}


void mlisp_print_list_obj2(MlispObjectList *list, char *s)
{
	MlispObjectList *current = list;
	MlispObject *obj;


	if (!list)
	{
		printf("ObjectList is NULL.\n");
		return;
	}

	while (current)
	{
		obj = mlisp_obj_peek(current);
		current = current->next;

		printf("%s", s);
		mlisp_print_obj(obj);
	}
}


////////////////////////////////////////////////////////////
/// Internal built-in functions ////////////////////////////
////////////////////////////////////////////////////////////

/* (append ELEMENT LIST )
 * Returns list which is LIST with 
 * ELEMENT appended to the end. */


MlispObject *numberp(MlispObjectList *parms)
{
	//if (object->type == INT || object->type == FLOAT)
	//	return (MlispObject *)1;  /* FIXME: ew bad form old bean */

	return NULL;
}


MlispObject *nil(MlispObjectList *parms)
{
	return NULL; // (NULL == parms->data || something)
}


MlispObject *setq(MlispObjectList *parms)
{
	return NULL;
}


/* (rest LIST)
 * Returns the list of remaining elements 
 * after the first element of LIST. */
MlispObject *rest(MlispObjectList *list)
{
	if (list)
		return mlisp_new_list_obj(list->next);

	return NULL;
}

 
/* (endp x)
 * Determines whether x is the empty list, 
 * returning the symbolic atom T if x=(), 
 * or the symbolic atom F if x is nonempty. */

/* (first LIST) 
 * Returns the first element of LIST. */
MlispObject *first(MlispObjectList *list)
{
	//if (list)
	//	return list->data;

	return NULL;
}

/* (cons ELEMENT LIST)
 * Returns a new list consisting of the ELEMENT, 
 * followed by the elements of LIST. */
MlispObject *cons(MlispObjectList *list)
{
	/* FIXME */

	return NULL;
}


////////////////////////////////////////////////////////////
/// Internal built-in math functions ///////////////////////
////////////////////////////////////////////////////////////

/* Assign value */
MlispObject *assign(MlispObjectList *list)
{
	MlispObjectList *current = list;
	MlispObject *objA = NULL, *objB = NULL;
	unsigned int lenght;


	objA = mlisp_obj_pop(&current);
	objB = mlisp_obj_pop(&current);

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


MlispObject *add(MlispObjectList *list)
{
	MlispObjectList *current = list;
	MlispObject *obj;
	float n = 0.0;

	
	while (current)
	{
		obj = mlisp_obj_peek(current);
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

	return mlisp_new_float_obj(n); 
}


MlispObject *subtract(MlispObjectList *list)
{
	MlispObjectList *current = list;
	MlispObject *obj;
	float n = 0.0, s;
	char first = 1;

	
	while (current)
	{
		obj = mlisp_obj_peek(current);
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

	return mlisp_new_float_obj(n); 
}


MlispObject *multiply(MlispObjectList *list)
{
	MlispObjectList *current = list;
	MlispObject *obj;
	float n = 0.0, m;
	char first = 1;

	
	while (current)
	{
		obj = mlisp_obj_peek(current);
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

	return mlisp_new_float_obj(n); 
}


MlispObject *divide(MlispObjectList *list)
{
	MlispObjectList *current = list;
	MlispObject *obj;
	float n = 0.0, d;
	char first = 1;

	
	while (current)
	{
		obj = mlisp_obj_peek(current);
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

	return mlisp_new_float_obj(n); 
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

MLisp::MLisp()
{
	mSymbolTable = NULL;
	mExecStack = NULL;
	mDataStack = NULL;
	mScopeBegin = mlisp_new_obj(BEGIN, NULL);
	mScopeEnd = mlisp_new_obj(END, NULL);
	mBuffer = NULL;
	mSymbol = NULL;
	mBufferSize = 0;
	mSymbolSize = 256;
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
	 * MlispObject     - Element
	 * MlispObjectList - List
	 */
	
	/* Append lisp built-in functions to symbol table */
	RegisterLispFunctionBuiltIn("setq", SetQ); 
	RegisterLispFunction("nil", nil);
	RegisterLispFunction("first", first);
	RegisterLispFunction("rest", rest);

	RegisterLispFunction("+", add);
	RegisterLispFunction("add", add);
	RegisterLispFunction("sub", subtract);
	RegisterLispFunction("-", subtract);
	RegisterLispFunction("*", multiply);
	RegisterLispFunction("/", divide);

	/* Append lisp built-in variables */
	RegisterSymbolValue("T", 0);
	RegisterSymbol("cDebugLevel", INT, (void *)(&mDebug));
}


MLisp::~MLisp()
{
	Clear();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void MLisp::DumpSymbols()
{
	printf("\n\nDumping symbols:\n");
	printf("------------------------------------------------------------\n");
	mlisp_print_list_obj(mSymbolTable);
	printf("------------------------------------------------------------\n");
}


MlispObject *MLisp::GetSymbol(const char *symbol)
{
	MlispObjectList *current;
	MlispObject *obj;
	

	if (!symbol || !symbol[0])
		return 0x0;

	current = mSymbolTable;

	while (current)
	{
		obj = mlisp_obj_peek(current);
		current = current->next;

		// Mongoose 2002.01.12, FIXME, string
		if (strcmp(symbol, obj->symbol) == 0)
		{
			return obj;
		}
	}

	return 0x0;
}


int MLisp::GetSymbolData(char *symbol, unsigned int type, void **data)
{
	MlispObjectList *current;
	MlispObject *obj;


	if (!symbol || !symbol[0])
		return -1;

	current = mSymbolTable;

	while (current)
	{
		obj = mlisp_obj_peek(current);
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

void MLisp::SetDebugLevel(int level)
{
	mDebug = level;
}


int MLisp::EvalBuffer(const char *buffer)
{
	if (mSymbolSize < 65)
	{
		mSymbolSize = 65;
	}

	if (!mSymbol)
		mSymbol = new char[mSymbolSize];

	//printf("%s\n", buffer);

#ifdef NO_FILE_REQUIRED
	if (!mBuffer)  // Good idea?
	{
		mBufferSize = strlen(buffer);

		char *tmp = new char[mBufferSize+1];
		strncpy(tmp, buffer, mBufferSize);
		tmp[mBufferSize] = '\0';

		--mBufferSize;
		--mBufferSize;

		if (mBuffer)
		{
			delete [] mBuffer;

		}

		mBuffer = tmp;
	}
#endif

	if (!ParseEvalBuffer(buffer))
		Eval();

	return 0;
}


int MLisp::EvalFile(const char *filename)
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

	if (!BufferFile(filename, &mBuffer, &mBufferSize))
	{
		if (!EvalBuffer(mBuffer))
		{
			return 0;
		}
	}

	return -1;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool MLisp::IsAtoZ(char c)
{
	return ((unsigned char)c > 64 && (unsigned char)c < 91);
}


bool MLisp::Isatoz(char c)
{
	return ((unsigned char)c > 96 && (unsigned char)c < 123);
}


bool MLisp::IsDigit(char c)
{
	return ((unsigned char)c > 47 && (unsigned char)c < 58);
}


bool MLisp::IsMisc(char c)
{
	if ((unsigned char)c == 95)
		return true;

	return false;
}


bool MLisp::IsNumeric(char *symbol, float *n)
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

		if (!IsDigit(symbol[i]))
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


void MLisp::PrintError(char *format, ...)
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

int MLisp::AppendSymbolTable(MlispObject *object)
{
	mSymbolTable = mlisp_obj_append(object, mSymbolTable);

	return !(mSymbolTable == NULL);
}


void MLisp::Bind(const char *symbol, MlispObject *data)
{
	int len;


	if (!symbol || !symbol[0] || !data)
	{
		++mErrors;
		PrintError("BIND receieved invalid symbol for binding");
		mlisp_delete_obj(&data);
		return;
	}

	/* Make a copy of the symbol to store in Object */
	len = strlen(symbol);
	data->symbol = new char[len + 1];
	strncpy(data->symbol, symbol, len);
	data->symbol[len] = 0;

	/* Append to the lisp object/symbol table */
	AppendSymbolTable(data);
}


int MLisp::BufferFile(const char *filename, 
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


void MLisp::Clear()
{
	while (mDataStack)
	{
		mlisp_obj_pop(&mDataStack);
	}

	while (mExecStack)
	{
		mlisp_obj_pop(&mExecStack);
	}

	while (mSymbolTable)
	{
		MlispObject *obj = mlisp_obj_peek(mSymbolTable);
		mlisp_delete_obj(&obj);
		mlisp_obj_pop(&mSymbolTable);
	}

	if (mScopeBegin)
	{
		mlisp_delete_obj(&mScopeBegin);
	}

	if (mScopeEnd)
	{
		mlisp_delete_obj(&mScopeEnd);
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


MlispObjectList *getNextScopeStack(MlispObjectList **stack)
{
	MlispObjectList *tmpStack = NULL;
	MlispObject *obj = NULL;
		
		
	while (*stack)
	{
		obj = mlisp_obj_pop(stack);
		
		if (!obj || obj->type == END)
		{
			/* End with NIL Object for var parm func (func ...) */
			//mlisp_obj_push(&tmpStack, NULL); // No longer valid?
			break; 
		}
		
		//Console.Write(">> ");
		//obj.print();
		
		mlisp_obj_push(&tmpStack, obj);
	}
	
	/* Reverse stack, so that order is maintained in execution from parsing */
	MlispObjectList *reverse = NULL;
	while (tmpStack)
	{
		obj = mlisp_obj_pop(&tmpStack);
		mlisp_obj_push(&reverse, obj);
	}
	
	return reverse;
	
	// tmpStack = reverse;
	//return tmpStack;
}


// FIXME: Not trimming given stack, so (setq (add ...)) calls (add ...) agian
MlispObject *MLisp::EvalFunction(MlispObjectList **stack, MlispObject *func)
{
	MlispObject *(*callFunction)(MlispObjectList *);
	MlispObjectList *parms = NULL, *reverse = NULL, *fstack = NULL;
	MlispObject *obj, *result = NULL;
	int scopeMatch = 0;


	if (mDebug > 2)
		printf("-begin----------------------\n");

	if (!(func->type == BUILTINFUNC || func->type == FUNC))
	{
		printf("FATAL ERROR: Lisp eval passed unbound function\n");
		return result;
	}
	else
	{
		mlisp_print_obj(func);
	}


	/* 0. Push func obj to grouped function stack */
	mlisp_obj_push(&fstack, func);


	/* 1. Pop a Scope BEGIN obj */
	obj = mlisp_obj_peek(*stack);
	
	if (!mlisp_obj_typep(obj, BEGIN))
	{
		printf("FATAL ERROR: Lisp stack corrupt... BEGIN !=");
		mlisp_print_obj(obj);

		// FIXME: Add stack dump here for debugging

		return result;
	}


	/* 2. Copy stack items to parms stack in same order until end of this
	 *    function's scope is reached, eval passed functions as well */
	while (stack)
	{
		obj = mlisp_obj_peek(*stack);

		if (mDebug > 2)
		{
			printf(" ");
			mlisp_print_obj(obj);
		}

		if (obj->type == BEGIN)
		{
			++scopeMatch;
			mlisp_obj_push(&parms, obj); // Push BEGIN
		}
		else if (obj->type == END)
		{
			--scopeMatch;
			mlisp_obj_push(&parms, obj); // Push END
			
			if (scopeMatch == 0)
			{
				if (mDebug > 1)
				{
					printf("xxxxxxxxxxxxxxxxxxxxxxxxxx\n");
					mlisp_print_list_obj(fstack);
					printf("xxxxxxxxxxxxxxxxxxxxxxxxxx\n");
				}

				/* 1. If func is non-nil continue to execute and push result */
				if (!func)
					break;
				
				/* 2. If the function is passed no args, make a null list */
				//if (!parms)
				//	mlisp_obj_push(&parms, NULL); // Is this no longer valid?

				/* 3. Reverse order of stack for use as args list */
				while (parms)
				{
					obj = mlisp_obj_pop(&parms);

					if (mDebug > 1)
					{
						printf("Buffering AR... ");
						mlisp_print_obj(obj);
					}


					/* 4. Call function implementation if FUNC, buffer others */
					switch (obj->type)
					{
					case BEGIN:
						continue;

					case FUNC:
					case BUILTINFUNC:
						{
							if (mDebug > 1)
							{
								printf("--- FUNC ---------------------\n");
								printf(" Calling ");
								mlisp_print_obj(obj);
								mlisp_print_list_obj2(reverse, "\te> ");
							}

							MlispObjectList *tmp = getNextScopeStack(&reverse);
							
							if (obj->type == FUNC)
							{
								callFunction = (MlispObject * (*)(MlispObjectList *))obj->data;
								obj = (*callFunction)(tmp);
							}
							// Use external BUILTINFUNC method w/ indices later
							else if (!strncmp("setq", obj->symbol, 4))
							{
								obj = Builtin_setq(tmp); 
							}
							else
							{
								printf("ERROR '%s' not implemented\n", obj->symbol);
							}

							//mlisp_obj_push(&reverse, obj); // could fall through
							mlisp_obj_pop(&fstack);
						}
						break;

					}

					mlisp_obj_push(&reverse, obj);
				}

				parms = reverse; // FIXME: Memory leak

				func = mlisp_obj_pop(&fstack);
				
				if (func->type == FUNC)
				{
					callFunction = (MlispObject * (*)(MlispObjectList *))func->data;
					result = (*callFunction)(reverse);
				}
				else if (!strncmp("setq", func->symbol, 4)) // use external method and indices later
				{
					result = Builtin_setq(reverse); 
				}
				
				mlisp_obj_push(&parms, result);

				if (mDebug > 1)
				{
					printf("\t <-- : ");
					mlisp_print_obj(result);
				}
				

				/* 5. No more functions to inline or callback */
				if (!func || !fstack)
					break;
			}
		}

		/* Cull scope markers, setup call stack */
		switch (obj->type)
		{
		case BEGIN:
		case END:
			break;

		case BUILTINFUNC:
		case FUNC:
			mlisp_obj_push(&fstack, obj);

		default:

			mlisp_obj_push(&parms, obj);
		}

		*stack = (*stack)->next;
	}


	if (mDebug > 2)
		printf("-end------------------------\n");

	/* 6. Return the result to eval system */
	return result;
}


void MLisp::Eval()
{
	MlispObject *(*callFunction)(MlispObjectList *);
	MlispObjectList *parms = NULL, *currentStack = NULL, *curStack, *cur;
	MlispObject *obj, *result;
	unsigned int frame = 0;


	if (mDebug > 0)
	{
		printf("\n--------------------------------\n");
		printf("Evaluating lisp\n");
	}


	/* Reverse stack, so that order is maintained in execution from parsing */
	MlispObjectList *reverse = NULL;
	while (mExecStack)
	{
		obj = mlisp_obj_pop(&mExecStack);
		mlisp_obj_push(&reverse, obj);
		//mlisp_print_obj(obj);
	}

	mExecStack = reverse;


	while (mExecStack)
	{
		obj = mlisp_obj_pop(&mExecStack);

		if (obj && obj->type == FUNC || obj->type ==BUILTINFUNC)
		{
			printf("evalFunction <-- "); mlisp_print_obj(obj);
			EvalFunction(&mExecStack, obj);
		}
	}

	while (mExecStack)
	{
		obj = mlisp_obj_pop(&mExecStack);

		if (mDebug > 4)
		{
			/* Not an AR or stack frame, just a stack use counter */
			printf("-frame %u----------------\n", ++frame);
		}

		if (!obj)
		{
			PrintError("FATAL ERROR: eval() feed NULL data");
			return;
		}
		else
		{
			switch (obj->type)
			{
			case BUILTINFUNC:
			case FUNC:
				

				/* 1. Pop a Scope BEGIN obj */
				obj = mlisp_obj_pop(&currentStack);

				if (!mlisp_obj_typep(obj, BEGIN))
				{
					printf("FATAL ERROR: Lisp data stack corrupt... BEGIN !=");
					mlisp_print_obj(obj);
				}

				/* 2. Copy currentStack items to parms stack in same order */
				parms = NULL;
				curStack = currentStack;

				//printf("=\n");
				while (curStack)
				{
					obj = mlisp_obj_peek(curStack);

					if (obj->type == END)
						break;

					if (!parms)
					{
						parms = new MlispObjectList;
						cur = parms;
					}
					else
					{
						cur->next = new MlispObjectList;
						cur = cur->next;
					}

					//mlisp_print_obj(obj);						
					cur->data = obj;
					cur->next = NULL;

					curStack = curStack->next;
				}
				//printf("=\n");

				/* 3. Call C function implementation, or method */
				switch (obj->type)
				{
				case FUNC:
					printf("FUNC: calling %s\n", obj->symbol);
					callFunction = (MlispObject * (*)(MlispObjectList *))obj->data;
					result = (*callFunction)(parms);
					break;
				default:
				case BUILTINFUNC:
					//MlispObject *(MLisp::*func)(MlispObjectList *);
					//func =(MlispObject *(MLisp::*)(MlispObjectList *))(*(obj->data));
					//result = (this->*func)(parms);
					printf("BUILTINFUNC: calling %s\n", obj->symbol);
					result = Builtin_setq(parms);
					break;
					//default:
					//printf("%s\n", obj->symbol);
					//assert(obj->type == FUNC || obj->type == BUILTINFUNC);
				}

				/* 4. Pop off this entire scope now */
				while ((obj = mlisp_obj_pop(&currentStack)))
				{
					if (mDebug > 4)
					{
						printf("POP ");
						mlisp_print_obj(obj);
					}

					if (obj->type == END)
						break;

					if (mGarbageCollection)
					{
						/* Weak garbage collect for now */
						mlisp_delete_obj(&obj);
					}
				}

				/* 5. Push result on stack */
				if (mDebug > 4)
				{
					printf("RESULT_PUSH ");
					mlisp_print_obj(result);
				}

				mlisp_obj_push(&currentStack, result);
				break;
			case BEGIN:
				/* END objects mark the end of a function parm list */

				if (mDebug > 4)
				{
					printf("BEGIN_PUSH\n");
				}

				mlisp_obj_push(&currentStack, obj);

				//mlisp_obj_push(&mDataStack, mlisp_new_list_obj(currentStack));
				//obj = mlisp_obj_peek(mDataStack);
				//currentStack = (MlispObjectList *)obj->data;
				break;
			case END:
				/* BEGIN objects mark the start of a function parm list */

				if (mDebug > 4)
				{
					printf("END_PUSH\n");
				}

				mlisp_obj_push(&currentStack, obj);

				//obj = mlisp_obj_pop(&mDataStack);
				//currentStack = (MlispObjectList *)obj->data;
				//printObjectList(currentStack);
				break;
			default:
				/* Everything else push into the scope list */

				if (mDebug > 4)
				{
					printf("DATA_PUSH ");
					mlisp_print_obj(obj);
				}

				mlisp_obj_push(&currentStack, obj);
			}
		}
	}

#ifdef MLISP_DEBUG_ENABLED
	  if (mDebug > 0)
	  {
		  printf("--------------------------------\n");
	  }
#endif

	  //dumpListSymbols(currentStack);
	  //mlisp_obj_push(&mStack, mlisp_new_list_obj(currentStack));
}


void MLisp::Expected(char c)
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
	PrintError("Expected '%c', not '%s' (%u)\nLast symbol before error: '%s'", 
			   c, s, mLook, mSymbol);
}


MlispObject *MLisp::GetNextSymbol()
{
	MlispObject *obj;

	ParseSeperator();

	if (mLook == ';' && !mString)
	{
		ParseComment();
		ParseSeperator();
	}

	obj = ParseNextSymbol();
	
	return obj;
}


bool MLisp::Is(char c)
{
  return (c == mLook);
}


void MLisp::Lex()
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
		  ParseComment();
	  }
  }
  else
  {
	  mLook = EOF;
  }
}


bool MLisp::Match(char c)
{
	bool r = Is(c);

	if (!r)
	{
		Expected(c);
	}

	Lex();

	return r;
}


void MLisp::ParseComment()
{
	Match(';');

	while (!Is('\n') && !Is(EOF))
	{
		Lex();
	}

	if (Is(';'))
	{
		ParseComment();
	}
}


int MLisp::ParseEvalBuffer(const char *buffer)
{
	MlispObject *object;


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

	Lex();
	ParseSeperator();

	while (Is('('))
	{
		object = ParseFunction();

#ifdef MLISP_DEBUG_ENABLED
		if (mDebug > 4)
		{
			printf("Pushing, ");
			mlisp_print_obj(object);
		}
#endif

		//mlisp_obj_push(&mExecStack, object);
		ParseSeperator();
	}

	if (mErrors)
	{
		printf("\n\nlisp> Encountered %i parse errors\n\n", mErrors);
	}

	return 0;
}


MlispObject *MLisp::ParseFunction()
{
	MlispObject *func = NULL, *object = NULL;
	void *data;


	Match('(');

	func = GetNextSymbol();

	if (!GetSymbolData(mSymbol, BUILTINFUNC, &data) && data)
	{
		mlisp_obj_push(&mExecStack, func);
	}
	else if (!GetSymbolData(mSymbol, FUNC, &data) && data)
	{
		mlisp_obj_push(&mExecStack, func);
	}
	else
	{
		++mErrors;
		PrintError("Invalid function '%s'\n", mSymbol);
		return NULL;
	}

#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 4)
	{
		printf("> BEGIN %s\n", func->symbol);
	}
#endif


	mlisp_obj_push(&mExecStack, mScopeBegin);

	/* Handle function parms, must be either FUNC objects or 
	 * the elements must be composed into a object list 
	 * pushed on the stack */
	while (!Is(')'))
	{
		object = GetNextSymbol();

#ifdef MLISP_DEBUG_ENABLED
		if (mDebug > 4)
		{
			printf("   Pushing parm, ");
			mlisp_print_obj(object);
		}
#endif
		
		if (object && object->type != FUNC)
		{
			mlisp_obj_push(&mExecStack, object);
		}
	}

#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 4)
	{
		printf("> END %s\n", func->symbol);
	}
#endif

	mlisp_obj_push(&mExecStack, mScopeEnd);

	Match(')');
	ParseSeperator();

	return func;
}


MlispObject *MLisp::ParseNextSymbol()
{
	int i = 0, j;
	MlispObject *object = NULL;
	bool string = false;
	bool fp = false;
	float f;


	mSymbol[0] = 0;
	
	if (Is('"'))
	{
		Lex();
		string = true;
	}
	
	/* Mongoose 2001.11.09: 
	 * Best handle for? grouped (func (func */
	if (Is('('))
	{
		return ParseFunction();
	}
	
	while (string || (i == 0 && mLook == '-') ||
		   Isatoz(mLook) || IsAtoZ(mLook) || IsDigit(mLook) || IsMisc(mLook))
	{
		if (i < (int)mSymbolSize)
		{
			mSymbol[i++] = mLook;
			mSymbol[i] = 0;
		}
		
		if (string && Is('"'))
		{
			--i;
			mSymbol[i] = 0;
			Lex();
			break;
		}
		
		Lex();
		
		if (string)
		{
			continue;
		}
		
		if (Is('.'))
		{
			fp = true;
			
			for (j = 0; j < i; ++j)
			{
				if (!j && mSymbol[j] == '-')
					continue;
				
				if (!IsDigit(mSymbol[j]))
					break;
			}

			if (i == j)
			{
			  mSymbol[i++] = mLook;
			  mSymbol[i] = 0;
			  Lex();
			}
		}
		
		
		// Mongoose 2002.01.23, Hhhmm... fixes -100.00 and -100, 
		//   but is it 'good idea'?
		if (Is('-') && i == 0)
		{
			mSymbol[i++] = mLook;
			mSymbol[i] = 0;
			Lex();
		}
	}
	
#ifdef MLISP_DEBUG_ENABLED
	if (mDebug > 7)
	{
		printf("\n> New symbol <%s>\n", mSymbol);
	}
#endif
	
	if (IsNumeric(mSymbol, &f))
	{
		if (fp)
		{
			object = mlisp_new_float_obj(f);
		}
		else
		{
			object = mlisp_new_int_obj((int)f);
		}
	}
	else
	{
		/* Handle symbol references here */
		if ((GetSymbolData(mSymbol, 0, (void **)&object)) == 0)
		{
		}
		else
		{
			if (!string)
			{
				++mErrors;
				PrintError("Making string out of non-string <%s>?\n", mSymbol);
			}

			object = mlisp_new_str_obj(mSymbol);
		}
	}
	
	return object;
}


void MLisp::ParseSeperator()
{
	// Real whitespace stripping
	while (Is(' ') || Is('\t') || Is('\n'))
	{
		if (Is('\n'))
		{
			mLine++;
		}
		
		Lex();
	}
}


void MLisp::ParseString(char *string, int len)
{
	int i = 0;
	
	Match('"');
	
	mString = 1;
	
	len -= 2;
	
	while (!Is('"') && !Is(EOF))
	{
		if (i < len)
		{
			string[i++] = mLook;
			string[i] = 0;
		}
		
		Lex();
	}

	Match('"');

	mString = 0;
}


int MLisp::RegisterLispFunctionBuiltIn(const char *symbol, builtin_t func)
//MlispObject *(MLisp::*func)(MlispObjectList *))
{
    MlispObject *obj = NULL;
    unsigned int lenght;
                                                                               
	if (!symbol || !symbol[0]) // || !func)
	{
		PrintError("ERROR Failed builtin function bind %s\n", symbol);
		return -1;
	}

	obj = new MlispObject;
    obj->type = BUILTINFUNC;
    //obj->data = //(void *)&func;
 
	lenght = strlen(symbol) + 1;
	obj->symbol = (char *)new char[lenght+1];
	strncpy(obj->symbol, symbol, lenght);
	obj->symbol[lenght] = 0;	

	obj->data = (void *)new char[lenght+1];
	strncpy((char *)obj->data, symbol, lenght);
	((char *)obj->data)[lenght] = 0;

	return AppendSymbolTable(obj);
}


int MLisp::RegisterLispFunction(const char *symbol, 
								MlispObject *(*func)(MlispObjectList *))
{
	if (!func)
	{
		return -1;
	}

	return AppendSymbolTable(mlisp_new_func_obj(func, symbol));
}


int MLisp::RegisterSymbol(const char *symbol, unsigned int type, void *data)
{
	if (!symbol || !symbol[0])
	{
		return -1;
	}

	MlispObject *object = new MlispObject;
	object->type = type;
	object->data = data;
	object->flags = mObjDisableGarbageCollection;

	Bind(symbol, object);

	return 0;
}


int MLisp::RegisterSymbolValue(const char *symbol, float d)
{
	MlispObject* object = mlisp_new_float_obj(d);


	if (RegisterSymbolObject(symbol, object) < 0)
	{
		mlisp_delete_obj(&object);
		return -1;
	}

	object->flags = mObjDisableGarbageCollection;

	return 0;
}


int MLisp::RegisterSymbolValue(const char *symbol, int i)
{
	MlispObject* object = mlisp_new_int_obj(i);


	if (RegisterSymbolObject(symbol, object) < 0)
	{
		mlisp_delete_obj(&object);
		return -1;
	}

	return 0;
}


// Should dupe data pointer for fear of GC
MlispObject *MLisp::Builtin_setq(MlispObjectList *parms)
{
	MlispObject *symbol, *data, *obj;


	symbol = mlisp_obj_pop(&parms);
	data = mlisp_obj_pop(&parms);

	obj = 0x0;
	
	if (data->type == CSTRING)
	{
		obj = GetSymbol((char *)data->data);
	}

	if (obj)
	{
		RegisterSymbol((char *)symbol->data, obj->type, obj->data);
	}
	else
	{
		//registerSymbolObject((char *)symbol->data, data);
		RegisterSymbol((char *)symbol->data, data->type, data->data);
		obj = data;
	}

	return obj;
}


int MLisp::RegisterSymbolValue(const char *symbol, char *string)
{
	MlispObject* object = mlisp_new_str_obj(string);


	if (RegisterSymbolObject(symbol, object) < 0)
	{
		mlisp_delete_obj(&object);
		return -1;
	}

	return 0;
}


int MLisp::RegisterSymbolObject(const char *symbol, MlispObject *object)
{
	if (!symbol || !symbol[0] || !object)
	{
		printf("Error: Couldn't bind invalid NULL object symbol\n");
		return -1;
	}

	Bind(symbol, object);

	return 0;
}


////////////////////////////////////////////////////////////
// Unit Test
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MLISP
MlispObject *menu(MlispObjectList *parms)
{
	MlispObject *xPos, *yPos, *ret, *obj;


	xPos = mlisp_obj_pop(&parms);
	yPos = mlisp_obj_pop(&parms);
	ret = NULL;
	
	if (mlisp_obj_numberp(xPos) && mlisp_obj_numberp(yPos))
	{
		/* Call event func here - simulated with printf for test */
		printf("extern \"C\" { menu(%f, %f); }\n",
			   mlisp_get_number(xPos), mlisp_get_number(yPos));

		while ((obj = mlisp_obj_peek(parms)))
		{
			printf("             submenu?, ");
			mlisp_print_obj(obj);
			
			mlisp_obj_pop(&parms);
		}

		ret = mlisp_new_str_obj("Menu");
	}
	else
	{
		printf("menu> Failed to extract strict typed data from script\n");
	}
	
	/* Would return menu struct here if it wasn't a simulation */
	return ret; 
}


MlispObject *submenu(MlispObjectList *parms)
{
	MlispObject *label, *obj, *ret = NULL;


	label = mlisp_obj_pop(&parms);

	if (mlisp_obj_typep(label, CSTRING))
	{
		/* Call event func here - simulated with printf in tests */
		printf("extern \"C\" { submenu(\"%s\"); }\n",
				 mlisp_get_string(label));

		while ((obj = mlisp_obj_peek(parms)))
		{
			printf("             menuitem?, ");
			mlisp_print_obj(obj);
			
			mlisp_obj_pop(&parms);
		}

		ret = mlisp_new_str_obj(mlisp_get_string(label));
	}
	else
	{
		printf("submenu> Failed to extract strict typed data from script\n");
	}

	/* Would return submenu struct here if it wasn't a simulation */
	return ret;
}


MlispObject *menu_item(MlispObjectList *parms)
{
	MlispObject *label, *event, *ret;


	label = mlisp_obj_pop(&parms);
	event = mlisp_obj_pop(&parms);
	ret = NULL;

	if (mlisp_obj_typep(label, CSTRING) && mlisp_obj_typep(label, INT))
	{
		/* Call event func here - simulated with printf in tests */
		printf("extern \"C\" { menu_item(\"%s\", %i); }\n",
				 mlisp_get_string(label), mlisp_get_int(event));

		ret = mlisp_new_str_obj(mlisp_get_string(label));
	}
	else
	{
		mlisp_print_obj(label);
		mlisp_print_obj(event);
		printf("menu_item> Failed to extract strict typed data from script\n");
	}

	/* Would return menu_item struct here if it wasn't a simulation */
	return ret;
}


MlispObject *mlisp_stub(MlispObjectList *parms)
{
	return NULL;
}


MlispObject *vbox(MlispObjectList *parms)
{
	MlispObject *arg;


	arg = mlisp_obj_pop(&parms);
	arg = mlisp_obj_pop(&parms);
	arg = mlisp_obj_pop(&parms);
	arg = mlisp_obj_pop(&parms);
	return NULL;
}


int runUnitTest(int argc, char *argv[])
{
	MLisp rc;
	int i = 0;


	rc.SetDebugLevel(5);

	if (argc <= 1)
	{
		printf("%s FILENAME.LISP\n", argv[0]);

		rc.SetDebugLevel(20);
		rc.EvalBuffer("(setq SHOULD_BE_14 (+ 4 5 (- 9 10) (+ 3 (+ 2 1))))");
		rc.DumpSymbols();
	}
	else if (argc > 1)
	{
		rc.RegisterLispFunction("mlispdebug", mlisp_stub);
		rc.RegisterLispFunction("window", menu);
		rc.RegisterLispFunction("menubar", menu);
		rc.RegisterLispFunction("vbox", vbox);

		rc.RegisterLispFunction("menu", menu);
		rc.RegisterLispFunction("submenu", submenu);
		rc.RegisterLispFunction("menu_item", menu_item);
		rc.RegisterSymbol("cMyCounter", INT, (void *)(&i));

		rc.RegisterSymbolValue("EVENT_SCREENSHOT",   10);
		rc.RegisterSymbolValue("EVENT_QUIT",         11);
		rc.RegisterSymbolValue("EVENT_SHOWBONES",    12);
		rc.RegisterSymbolValue("EVENT_WIREFRAME",    13);
		rc.RegisterSymbolValue("EVENT_FULLSCREEN",   14);
		rc.RegisterSymbolValue("EVENT_RES640x480",   15);
		rc.RegisterSymbolValue("EVENT_RES800x600",   16);
		rc.RegisterSymbolValue("EVENT_RES1024x768",  17);
		rc.RegisterSymbolValue("EVENT_RES1280x1024", 18);

		if (argc > 1 && rc.EvalFile(argv[1]) < 0)
		{
			printf("Couldn't load file!\n");
		}
		else
		{
			// rc.EvalBuffer("(+ 23 42 54 23)");
		}

		rc.DumpSymbols();
	}

	return -1;
}


int main(int argc, char *argv[])
{
	printf("[MLisp class test]\n");

	return runUnitTest(argc, argv);
}
#endif
