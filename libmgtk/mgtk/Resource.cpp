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
 *-- History ------------------------------------------------- 
 *
 * 2001.11.02:
 * Mongoose - Created
 =================================================================*/

//#define DEBUG_RESOURCE_SYMBOL

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mgtk_events.h"
#include "Resource.h"


#define MULTI_EVAL
//#define DEBUG_RESOURCE_EXE
//#define DEBUG_RESOURCE_SYMBOL

Resource *Resource::mInstance = 0x0;
Resource *__RESOURCE_AGENT_ = NULL;


int resourceGetEventId1s(const char *name)
{
	return Resource::mInstance->getIntByName(name);
}


/// Arg list /////////////////////////////////////////////////////////


void new_func(arg_list_t **a, arg_list_t *(*f)(arg_list_t *), char *s)
{
	*a = NULL;

	if (!s || !s[0])
		return;

	*a = new arg_list_t;

	(*a)->next = NULL;
	(*a)->type = FUNC;
	(*a)->data = (void *)f;

	(*a)->symbol = new char[strlen(s)+1];
	strcpy((char *)(*a)->symbol, s);
}


void new_float(arg_list_t **a, float n)
{
	float *f;


	*a = new arg_list_t;

	(*a)->symbol = NULL;
	(*a)->next = NULL;
	(*a)->type = FLOAT;

	f = new float;
	*f = n;
	(*a)->data = (void *)f;	
}


void new_adt(arg_list_t **a, int type, void *data)
{
	*a = new arg_list_t;

	(*a)->symbol = NULL;
	(*a)->next = NULL;
	(*a)->type = (arg_type_t)type;
	(*a)->data = data;	
}


void new_int(arg_list_t **a, int n)
{
	int *i;


	*a = new arg_list_t;

	(*a)->symbol = NULL;
	(*a)->next = NULL;
	(*a)->type = INT;

	i = new int;
	*i = n;
	(*a)->data = (void *)i;
}


void new_string(arg_list_t **a, char *s)
{
	*a = NULL;

	if (!s || !s[0])
		return;

	*a = new arg_list_t;

	(*a)->symbol = NULL;
	(*a)->next = NULL;
	(*a)->type = CSTRING;

	(*a)->data = (void *)new char[strlen(s)+1];
	strcpy((char *)(*a)->data, s);
}


void delete_arg(arg_list_t **a)
{
	if (!(*a))
		return;

	if ((*a)->symbol)
		delete [] (char *)(*a)->symbol;

	switch ((*a)->type)
	{
	case FUNC:
		// We don't free function pointers, but we need to free container 'a'  ;)
		break;
	case CSTRING:
		if ((*a)->data)
			delete [] (char *)(*a)->data;		
		break;
	case INT:
		if ((*a)->data)
			delete (int*)(*a)->data;
		break;
	case FLOAT:
		if ((*a)->data)
			delete (float*)(*a)->data;
		break;
	default:
		;
	}
	
	delete *a;
	*a = NULL;
}


char *get_string(arg_list_t *a)
{
	return (char *)a->data;
}


float get_float(arg_list_t *a)
{
	return (*((float *)(a->data)));
}


int get_int(arg_list_t *a)
{
	return (*((int *)(a->data)));
}


void symbol_enforce_type(arg_list_t **a, int type)
{
	*a = symbol();
	arg_enforce_type(a, type);
}


void symbol_enforce_type_assert(arg_list_t **a, int type)
{
	*a = symbol();
	arg_enforce_type_assert(a, type);
}


void arg_enforce_type_assert(arg_list_t **a, int type)
{
	if (!(*a))
		return;

	if (type & (*a)->type)
		return;

	MGTK_ASSERTMSG(false, 

						"MLISP %s:%i\n\tType mismatch expected '%s', not '%s'\n\t(%s)\n", 

						mlisp_get_filename(), mlisp_get_line_num(), 

						((type & INT) ? "int" :
						 (type & FLOAT) ? "float" :
						 (type & FUNC) ? "func" :
						 (type & CSTRING) ? "string" : "adt"),

						(((*a)->type & INT) ? "int" :
						 ((*a)->type & FLOAT) ? "float" :
						 ((*a)->type & FUNC) ? "func" :
						 ((*a)->type & CSTRING) ? "string" : "adt"),

						((*a)->type == CSTRING) ? (char*)((*a)->data) : "?"
						);

	delete_arg(a);
}


void arg_enforce_type(arg_list_t **a, int type)
{
	if (!(*a))
		return;

	if (type & (*a)->type)
		return;

	printf("ERROR: Type mismatch expected '%s', not '%s'\n",
			 (type & INT) ? "int" :
			 (type & FLOAT) ? "float" :
			 (type & FUNC) ? "func" :
			 (type & CSTRING) ? "string" : "adt",

			 ((*a)->type & INT) ? "int" :
			 ((*a)->type & FLOAT) ? "float" :
			 ((*a)->type & FUNC) ? "func" :
			 ((*a)->type & CSTRING) ? "string" : "adt");

	if ((*a)->type == CSTRING)
	{
		printf("ERROR: string was '%s'\n", (char*)((*a)->data));
	}

	delete_arg(a);
}


void arg_push(arg_stack_t **s, arg_list_t *a)
{
	arg_stack_t *n;

	if (!(*s))
	{
		*s = new arg_stack_t;
		(*s)->next = NULL;
	}
	else
	{
		n = new arg_stack_t;
		n->next = *s;
		*s = n;
	}

	(*s)->data = a;
}


arg_list_t *arg_pop(arg_stack_t **s)
{
	arg_stack_t *n;
	arg_list_t *a;


	if (!(*s))
	{
		return NULL;
	}

	a = (*s)->data;

	n = *s;
	*s = (*s)->next;
	delete n;

	return a;
}


arg_list_t *arg_peek(arg_stack_t **s)
{
	if (!(*s))
	{
		return NULL;
	}

	return (*s)->data;
}


/// Helpers /////////////////////////////////////////////////////////


bool isdigit(char c)
{
	return ((unsigned char)c > 47 && (unsigned char)c < 58);
}


bool isAtoZ(char c)
{
	return ((unsigned char)c > 64 && (unsigned char)c < 91);
}


bool isatoz(char c)
{
	return ((unsigned char)c > 96 && (unsigned char)c < 123);
}


bool ismisc(char c)
{
	if ((unsigned char)c == 95)
		return true;

	return false;
}


bool isnumeric(char *symbol, float *n)
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

		if (!isdigit(symbol[i]))
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


// Decided to combine seperator() and symbol()
arg_list_t *symbol()
{
	if (!__RESOURCE_AGENT_)
		return NULL;

	__RESOURCE_AGENT_->Seperator();

	return __RESOURCE_AGENT_->Symbol();
}


void seperator()
{
	if (!__RESOURCE_AGENT_)
		return;

	__RESOURCE_AGENT_->Seperator();
}


void mlisp_bind(arg_list_t *symbol, arg_list_t *data)
{
	if (!__RESOURCE_AGENT_)
		return;

	__RESOURCE_AGENT_->Bind(symbol, data);
}


void *mlisp_recall(const char *symbol)
{
	if (!__RESOURCE_AGENT_)
		return NULL;

	arg_list_t *becareful;
	__RESOURCE_AGENT_->Lookup(symbol, &becareful);
	return becareful ? becareful->data : NULL;
}


char mlisp_peek_for_vargs()
{
	if (!__RESOURCE_AGENT_)
		return 0;

	__RESOURCE_AGENT_->Seperator();

	return ( !__RESOURCE_AGENT_->Is(')') && !__RESOURCE_AGENT_->Is('('));
}


const char *mlisp_get_filename()
{
	if (!__RESOURCE_AGENT_)
		return 0;

	return __RESOURCE_AGENT_->GetFilename();
}


int mlisp_get_line_num()
{
	if (!__RESOURCE_AGENT_)
		return 0;

	return __RESOURCE_AGENT_->GetLine();
}


/// External built-in funcs //////////////////////////////////////

arg_list_t *nil(arg_list_t *arg)
{
	return NULL;
}

arg_list_t *setq(arg_list_t *arg)
{
	arg_list_t *a, *b;


	a = symbol();
	b = symbol();
	mlisp_bind(a, b);

	return NULL;
}


/// Hooks //////////////////////////////////////////////////////


/* Mongoose 2002.01.12, Interfaces for Resource
   must be implmented for each interface ( gtk, win32, etc ) */
arg_list_t *mgtk_rc_window(arg_list_t *args);
arg_list_t *mgtk_rc_gl_widget(arg_list_t *args);
arg_list_t *mgtk_rc_notebook(arg_list_t *args);
arg_list_t *mgtk_rc_tab(arg_list_t *args);
arg_list_t *mgtk_rc_textbox(arg_list_t *args);
arg_list_t *mgtk_rc_hsep(arg_list_t *args);
arg_list_t *mgtk_rc_vsep(arg_list_t *args);
arg_list_t *mgtk_rc_hbox(arg_list_t *args);
arg_list_t *mgtk_rc_vbox(arg_list_t *args);
arg_list_t *mgtk_rc_handlebox(arg_list_t *args);
arg_list_t *mgtk_rc_label(arg_list_t *args);
arg_list_t *mgtk_rc_button(arg_list_t *args);
arg_list_t *mgtk_rc_colorbutton(arg_list_t *args);
arg_list_t *mgtk_rc_togglebutton(arg_list_t *args);
arg_list_t *mgtk_rc_spinbutton(arg_list_t *args);
arg_list_t *mgtk_rc_spinbutton2(arg_list_t *args);
arg_list_t *mgtk_rc_submenu(arg_list_t *args);
arg_list_t *mgtk_rc_menu_seperator(arg_list_t *args);
arg_list_t *mgtk_rc_menu_item(arg_list_t *args);
arg_list_t *mgtk_rc_check_menu_item(arg_list_t *args);
arg_list_t *mgtk_rc_optionmenu(arg_list_t *args);
arg_list_t *mgtk_rc_popup_menu(arg_list_t *args);
arg_list_t *mgtk_rc_menubar(arg_list_t *args);
arg_list_t *mgtk_rc_animation_tab_hack(arg_list_t *args);
arg_list_t *mgtk_rc_toolbar(arg_list_t *args);
arg_list_t *mgtk_rc_toolbar_togglebutton(arg_list_t *args);
arg_list_t *mgtk_rc_toolbar_button(arg_list_t *args);
arg_list_t *mgtk_rc_toolbar_box(arg_list_t *args);
arg_list_t *mgtk_rc_color(arg_list_t *args);
arg_list_t *mgtk_rc_hslider(arg_list_t *args);
arg_list_t *mgtk_rc_icon(arg_list_t *args);
arg_list_t *mgtk_rc_dialog(arg_list_t *args);
arg_list_t *mgtk_rc_query_dialog(arg_list_t *args);
arg_list_t *mgtk_rc_confirmation_dialog(arg_list_t *args);
arg_list_t *mgtk_rc_label2(arg_list_t *args);
arg_list_t *mgtk_rc_fileselection_drop_down_menu(arg_list_t *box);
arg_list_t *mgtk_rc_filechooserbutton(arg_list_t *box);
arg_list_t *mgtk_rc_filechoosermenu_item(arg_list_t *box);
arg_list_t *mgtk_rc_filechoosertoolbar_button(arg_list_t *box);
arg_list_t *mgtk_rc_toolbar_separator(arg_list_t *box);
arg_list_t *mgtk_rc_expander(arg_list_t *box);
arg_list_t *mgtk_rc_summonbox(arg_list_t *box);
arg_list_t *mgtk_rc_statusbar(arg_list_t *box);
arg_list_t *mgtk_rc_textview(arg_list_t *args);

arg_list_t *mgtk_func_toggle_set(arg_list_t *args);

////////////////////////////////////////////////////////////////


Resource::Resource() :
	_symbol(NULL),
	_buffer(NULL),
	_buffer_len(0),
	_symbol_len(256),
	_top(0),
	_look(0),
	_tape(NULL),
	_line(1),	
	_string(0),
	_error(0),
	_stack(NULL),
	_sym_tab(NULL)
{
	mFilename[0] = 0;

	RegisterFunction("setq", setq);
	RegisterFunction("nil", nil);

	Resource::mInstance = __RESOURCE_AGENT_ = this;

	/* Mongoose 2002.01.12, 
	 * Bind script functions to C/C++ functions */
	RegisterFunction("window", mgtk_rc_window);
	RegisterFunction("icon", mgtk_rc_icon);
	RegisterFunction("gl_widget", mgtk_rc_gl_widget);
	RegisterFunction("statusbar", mgtk_rc_statusbar);
	RegisterFunction("textview", mgtk_rc_textview);
	RegisterFunction("notebook", mgtk_rc_notebook);
	RegisterFunction("dialog", mgtk_rc_dialog);
	RegisterFunction("tab", mgtk_rc_tab);
	RegisterFunction("hsep", mgtk_rc_hsep);
	RegisterFunction("vsep", mgtk_rc_vsep);
	RegisterFunction("vbox", mgtk_rc_vbox);
	RegisterFunction("hbox", mgtk_rc_hbox);
	RegisterFunction("handlebox", mgtk_rc_handlebox);
	RegisterFunction("textbox", mgtk_rc_textbox);
	RegisterFunction("label", mgtk_rc_label);
	RegisterFunction("label2", mgtk_rc_label2);
	RegisterFunction("colorbutton", mgtk_rc_colorbutton);
	RegisterFunction("button", mgtk_rc_button);
	RegisterFunction("togglebutton", mgtk_rc_togglebutton);
	RegisterFunction("spinbutton", mgtk_rc_spinbutton);
	RegisterFunction("spinbutton2", mgtk_rc_spinbutton2);
	RegisterFunction("menu_item", mgtk_rc_menu_item);
	RegisterFunction("toggle_menu_item", mgtk_rc_check_menu_item);
	RegisterFunction("submenu", mgtk_rc_submenu);
	RegisterFunction("menu_seperator", mgtk_rc_menu_seperator);
	RegisterFunction("optionmenu", mgtk_rc_optionmenu);
	RegisterFunction("popup_menu", mgtk_rc_popup_menu);
	RegisterFunction("menubar", mgtk_rc_menubar);
	RegisterFunction("tree", mgtk_rc_animation_tab_hack);
	RegisterFunction("toolbar", mgtk_rc_toolbar);
	RegisterFunction("toolbar_togglebutton", mgtk_rc_toolbar_togglebutton);
	RegisterFunction("toolbar_button", mgtk_rc_toolbar_button);
	RegisterFunction("hslider", mgtk_rc_hslider);
	RegisterFunction("dialog", mgtk_rc_dialog);
	RegisterFunction("query_dialog", mgtk_rc_query_dialog);
	RegisterFunction("confirmation_dialog", mgtk_rc_confirmation_dialog);
	RegisterFunction("fileselection_hack", mgtk_rc_fileselection_drop_down_menu);
	RegisterFunction("filechooserbutton", mgtk_rc_filechooserbutton);
	RegisterFunction("filechoosermenu_item", mgtk_rc_filechoosermenu_item);
	RegisterFunction("filechoosertoolbar_button", mgtk_rc_filechoosertoolbar_button);
	RegisterFunction("toolbar_separator", mgtk_rc_toolbar_separator);
	RegisterFunction("func_set_toggle", mgtk_func_toggle_set);
	RegisterFunction("expander", mgtk_rc_expander);
	RegisterFunction("summonbox", mgtk_rc_summonbox);

	RegisterInt("IconSize_Menu", 1);
	RegisterInt("IconSize_ToolbarSmall", 2);
	RegisterInt("IconSize_Toolbar", 3);
	RegisterInt("IconSize_Button", 4);
	RegisterInt("IconSize_Dialog", 5);
}

Resource::~Resource()
{
	__RESOURCE_AGENT_ = NULL;

	Flush();
}



void Resource::Flush()
{
	arg_list_t *a;


	while (_stack)
	{
		arg_pop(&_stack);
	}

	while (_sym_tab)
	{
		a = _sym_tab;
		_sym_tab = _sym_tab->next;
		delete_arg(&a);
	}

	if (_buffer)
	{
		delete [] _buffer;
	}

	if (_symbol)
	{
		delete [] _symbol;
	}
}


int Resource::RegisterFunction(char *symbol, arg_list_t *(*func)(arg_list_t *))
{
	arg_list_t *f, *sym_tab;


	new_func(&f, func, symbol);

	if (!func)
	{
      return -1;
	}

	if (!_sym_tab)
	{
		_sym_tab = f;
	}
	else
	{
		sym_tab = _sym_tab;

		while (sym_tab->next)
		{
			sym_tab = sym_tab->next;
		}
		
		sym_tab->next = f;
	}

	return 0;
}


int Resource::RegisterSymbol(char *symbol, arg_type_t type, void *data)
{
	arg_list_t *sym;
	arg_list_t *sym_name;


	if (!symbol || !symbol[0])
	{
		return -1;
	}

	switch (type)
	{
	case FUNC:
		printf("You can't bind functions using generic binding\n");
		return -2;
		break;
	case INT:
		new_int(&sym, *((int *)data));
		break;
	case FLOAT:
		new_float(&sym, *((float *)data));
		break;
	case CSTRING:
		new_string(&sym, (char *)data);
		break;
	default:
		new_adt(&sym, type, data);
	}

	new_string(&sym_name, symbol);
	Bind(sym_name, sym);

	return 0;
}


arg_list_t *Resource::Function(arg_list_t *arg)
{
	arg_list_t *sym_tab;
	arg_list_t *ret;
	arg_list_t *func;
	arg_list_t *(*call)(arg_list_t *);


	Match('(');
	func = Symbol();

	sym_tab = _sym_tab;
	ret = NULL;

	// FIXME: This causes bad things with MULT_EVAL
	while (sym_tab)
	{
		if (sym_tab->type == FUNC && strcmp(_symbol, sym_tab->symbol) == 0)
		{
			if (sym_tab->data)
			{
				call = (arg_list_t * (*)(arg_list_t *))sym_tab->data;
				ret = (*call)(arg);
				break;
			}
		}

		sym_tab = sym_tab->next;
	}

	//arg_enforce_type(&sym_tab, FUNC);

	if (!sym_tab || (sym_tab && sym_tab->type != FUNC))
	{
		Error("Invalid function call");
		
		if (sym_tab && sym_tab->symbol)
		{
			printf("Probable unbound function '%s'\n", sym_tab->symbol);
		}
		else if (func && func->type == CSTRING && func->data && 
					((char *)func->data)[0])
		{
			printf("Probable unbound function '%s'\n", (char *)func->data);
		}
	}

	//Mongoose 2001.11.09: For 'empty space' or '(func (func' use
	Seperator(); 

	// Mongoose 2001.11.09: Best handle for? grouped (func (func
	while (Is('('))
	{
		//arg_push(&_stack, arg);
		//arg_peek(&_stack));
		Function(ret);
		//ret = arg_pop(&_stack);

#ifdef DEBUG_RESOURCE_EXE
		printf("Function executed on Line %i\n", _line);
#endif
		Seperator();
	}

	delete_arg(&func);

	Match(')');

	return ret;
}


void Resource::RegisterInt(char *symbol, int value)
{
	if (RegisterSymbol(symbol, INT, &value))
	{
		printf("ERROR couldn't register new resource symbol %s\n",
				 symbol);
	}
}


void Resource::Bind(arg_list_t *symbol, arg_list_t *data)
{
	arg_list_t *sym_tab;
	unsigned int len;


	if (!symbol || !data || (symbol->type != CSTRING) ||
		 !symbol->data || !((char *)symbol->data)[0])
	{
		Error("BIND receieved invalid symbol");
		delete_arg(&symbol);
		delete_arg(&data);
		return;
	}

	// Mongoose 2002.01.12, String fix for overflow
	len = strlen((char *)symbol->data);
	data->symbol = new char[len + 4];
	strncpy(data->symbol, (char *)symbol->data, len);
	data->symbol[len] = 0;

	delete_arg(&symbol);

	if (!_sym_tab)
	{
		_sym_tab = data;
	}
	else
	{
		sym_tab = _sym_tab;

		while (sym_tab->next)
		{
			sym_tab = sym_tab->next;
		}
		
		sym_tab->next = data;
	}
}


arg_list_t *Resource::Symbol()
{
  int i = 0, j;
  arg_list_t *a;
  bool string = false;
  bool fp = false;
  float f;
  arg_list_t *adt;


  _symbol[0] = 0;

  if (Is('"'))
  {
	  Lex();
	  string = true;
  }

  // Mongoose 2001.11.09: Best handle for? grouped (func (func
  if (Is('('))
  {
	  return Function(arg_peek(&_stack));
  }

  while (string || (i == 0 && _look == '-') ||
			isatoz(_look) || isAtoZ(_look) || isdigit(_look) || ismisc(_look))
  {
	  if (i < (int)_symbol_len)
	  {
		  _symbol[i++] = _look;
		  _symbol[i] = 0;
	  }

	  if (string && Is('"'))
	  {
		  i--;
		  _symbol[i] = 0;
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

		  for (j = 0; j < i; j++)
		  {
			  if (!j && _symbol[j] == '-')
				  continue;

			  if (!isdigit(_symbol[j]))
				  break;
		  }

		  if (i == j)
		  {
			  _symbol[i++] = _look;
			  _symbol[i] = 0;
			  Lex();
		  }
	  }


	  // Mongoose 2002.01.23, Hhhmm... fixes -100.00 and -100, 
	  //   but is it 'good idea'?
	  if (Is('-') && i == 0)
	  {
		  _symbol[i++] = _look;
		  _symbol[i] = 0;
		  Lex();
	  }
  }

#ifdef DEBUG_RESOURCE_SYMBOL
  printf("%s\n", _symbol);
#endif

  if (isnumeric(_symbol, &f))
  {
	  if (fp)
	  {
		  new_float(&a, f);
	  }
	  else
	  {
		  new_int(&a, (int)f);
	  }
  }
  else
  {
	  // Mongoose 2002.01.21, FIXME support ADTs and etc too
	  if (Lookup(_symbol, &i))
	  {
		  new_int(&a, i);
	  }
	  else if (Lookup(_symbol, &f))
	  {
		  new_float(&a, f);
	  }
	  else if (string)
	  {
		  new_string(&a, _symbol);
	  }
	  else if (Lookup(_symbol, &adt))
	  {
		  new_adt(&a, adt->type, adt->data);
	  }
	  else
	  {
		  // Mongoose 2002.01.21, FIXME: hack to handle old string
		  //   use for def symbols
		  new_string(&a, _symbol);
	  }
  }

  return a;
}


void Resource::Lex()
{
  if (_top < _buffer_len)
  {
	  _top++;
	  _look = _buffer[_top];

#ifdef DEBUG_RESOURCE_LEX
	  printf("%c", _look);
	  fflush(stdout);
#endif

	  if (_look == ';' && !_string)
	  {
		  Comment();
	  }
  }
  else
  {
	  _look = EOF;
  }
}


void Resource::Seperator()
{
	// Real whitespace stripping
	while (Is(' ') || Is('\t') || Is('\n'))
	{
		if (Is('\n'))
		{
			_line++;
		}
		
		Lex();
	}
}


void Resource::String(char *string, int len)
{
  int i = 0;

  
  Match('"');

  _string = 1;

  len -= 2;
  
  while (!Is('"') && !Is(EOF))
  {
    if (i < len)
    {
      string[i++] = _look;
      string[i] = 0;
    }
    
    Lex();
  }

  Match('"');

  _string = 0;
}


void Resource::Error(char *s)
{
  printf("\nERROR: Line %i; %s\n", _line, s);
  _error++;
}


bool Resource::Match(char c)
{
  bool r = Is(c);


  if (!r)
  {
    Expected(c);
  }

  Lex();

  return r;
}


bool Resource::Is(char c)
{
	return (c == _look);
}


void Resource::Expected(char c)
{
	char s[4];


	switch (_look)
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
		s[0] = _look;
		s[1] = 0;
	}

	printf("\nERROR: Line %i; Expected '%c', not '%s' (%u)\n", 
			 _line, c, s, _look);
	printf("\nERROR: Last symbol before error: '%s'\n", _symbol);
	_error++;
}


void Resource::Comment()
{
	Match(';');

	while (!Is('\n') && !Is(EOF))
	{
		Lex();
	}

	if (Is(';'))
	{
		Comment();
	}
}


int Resource::Load(char *filename)
{
#ifdef MULTI_EVAL
	printf("Resource::Load> '%s'\n", filename);
#endif

	strncpy(mFilename, filename, 95);
	mFilename[95] = 0;

	if (_buffer)
	{
		delete [] _buffer;
	}

	if (_symbol)
	{
		delete [] _symbol;
	}

	if (_symbol_len < 65)
	{
		_symbol_len = 65;
	}

	_symbol = new char[_symbol_len];

	if (!BufferFromFile(filename, &_buffer, &_buffer_len))
	{
		if (!Eval(_buffer))
		{
			return 0;
		}
	}

	return -1;
}


int Resource::BufferFromFile(char *filename, char **buffer,unsigned int *bytes)
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
		printf("ERROR: Reading lisp file.\n");
		perror(filename);
		return -2;
	}

	fseek(f, 0, SEEK_END);
	*bytes = ftell(f);
	fseek(f, 0, SEEK_SET);

	// FIXME: Might want to go ahead and strip comments here,
	//        and go ahead and do preprocessing instead of
	//        a straight file buffer.

	*buffer = new char[*bytes];
	fread(*buffer, *bytes, 1, f);
	fclose(f);

	return 0;
}


int Resource::Eval(char *buffer)
{
	arg_list_t *a, *b, *c;


	if (!buffer || !buffer[0])
	{
		return -1;
	}

	// Mongoose: Start counting lines at 1, errors at 0
	_line = 1; 
	_error = 0;


#ifdef MULTI_EVAL
	_top = 0;
	_string = 0;

	while (_stack)
	{
		arg_pop(&_stack);
	}

	_stack = NULL;
#endif

	Lex();
	Seperator();

	while (Is('#'))
	{
		printf("Resource::Eval> Preprocessor not implemented yet.\n");
		
		Lex();
		a = Symbol();
		Seperator();
		b = Symbol();
		Seperator();
		c = Symbol();
		Seperator();

		// #func arg1 arg2  ( eg #define FOOBAR 1 )

		// Preprocessor function
		delete_arg(&a);

		// Arg 1
		delete_arg(&b);

		// Arg 2
		delete_arg(&c);
	}

	a = NULL;

	while (Is('('))
	{
		// FIXME: Handle scope with paren counter and appending to list a
		a = Function(a);
		Seperator();
	}
			
	if (_error)
		printf("\n\nEval> Encountered %i Errors\n\n", _error);

	return 0;
}


int Resource::getIntByName(const char *symbol)
{
	int id = -1;

	Lookup(symbol, &id);

	return id;	
}


bool Resource::Lookup(const char *symbol, float *f)
{
	arg_list_t *sym_tab;


	if (!symbol || !symbol[0] || !f)
		return false;

	sym_tab = _sym_tab;

	while (sym_tab)
	{
		if (sym_tab->type == FLOAT)
		{
			// Mongoose 2002.01.12, FIXME, string
			if (strcmp(symbol, sym_tab->symbol) == 0)
			{
				*f = get_float(sym_tab);
				return true;
			}
		}

		sym_tab = sym_tab->next;
	}

	return false;
}


bool Resource::Lookup(const char *symbol, int *i)
{
	arg_list_t *sym_tab;


	if (!symbol || !symbol[0] || !i)
		return false;

	sym_tab = _sym_tab;

	while (sym_tab)
	{
		if (sym_tab->type == INT)
		{
			if (strcmp(symbol, sym_tab->symbol) == 0)
			{
				*i = get_int(sym_tab);
				return true;
			}
		}

		sym_tab = sym_tab->next;
	}

	return false;
}


bool Resource::Lookup(const char *symbol, arg_list_t **adt)
{
	arg_list_t *sym_tab;


	if (!symbol || !symbol[0])
		return false;

	sym_tab = _sym_tab;

	while (sym_tab)
	{
		if (sym_tab->type != INT && sym_tab->type != FLOAT &&
			 sym_tab->type != FUNC)
		{
			if (strcmp(symbol, sym_tab->symbol) == 0)
			{
				*adt = sym_tab;
				return true;
			}
		}

		sym_tab = sym_tab->next;
	}

	return false;
}


bool Resource::Lookup(const char *symbol, char **s)
{
	arg_list_t *sym_tab;


	if (!symbol || !symbol[0])
		return false;

	sym_tab = _sym_tab;

	while (sym_tab)
	{
		if (sym_tab->type == CSTRING)
		{
			if (strcmp(symbol, sym_tab->symbol) == 0)
			{
				*s = get_string(sym_tab);
				return true;
			}
		}

		sym_tab = sym_tab->next;
	}

	return false;
}


void Resource::Print()
{
	arg_list_t *sym_tab;


	sym_tab = _sym_tab;

	while (sym_tab)
	{
		if (sym_tab->symbol)
			printf("%s <- ", sym_tab->symbol);
		else
			printf("? <- ");

		switch (sym_tab->type)
		{
		case INT:
			printf("int (%i)\n", *(int *)sym_tab->data);
			break;
		case FLOAT:
			printf("float (%f)\n", *(float *)sym_tab->data);
			break;
		case CSTRING:
			printf("string (\"%s\")\n", (char *)sym_tab->data);
			break;
		case FUNC:
			printf("func (...)\n");
			break;
		default:
			printf("[Unknown type %i] Error: Email mongoose@users.sourceforge.net\n",
					 sym_tab->type);
		}

		sym_tab = sym_tab->next;
	}
}


////////////////////////////////////////////////////////////
// Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_RESOURCE

#if 0
arg_list_t *color(arg_list_t *args)
{
	arg_list_t *sym, *r, *g, *b;

	sym = symbol();
	r = symbol();
	g = symbol();
	b = symbol();
	
	arg_enforce_type(&r, FLOAT);
	arg_enforce_type(&g, FLOAT);
	arg_enforce_type(&b, FLOAT);

	// Call event func here - simulated with printf in tests
	if (sym && r && g && b)
	{
		printf("extern \"C\" { color(%s, %f, %f, %f); }\n",
				 get_string(sym),
				 get_float(r), get_float(g), get_float(b));
	}
	else
	{
		printf("color> Failed to extract strict typed data from script\n");
	}

	delete_arg(&sym);
	delete_arg(&r);
	delete_arg(&g);
	delete_arg(&b);

	return NULL;
}


arg_list_t *key(arg_list_t *args)
{
	arg_list_t *code, *mod, *event, *cmd;


	code = symbol();
	mod = symbol();
	event = symbol();
	cmd = symbol();
	
	arg_enforce_type(&code, INT);
	//arg_enforce_type(&mod, INT);

	// Call event func here - simulated with printf in tests
	if (code && mod && event && cmd)
	{
		printf("extern \"C\" { key(%i, %s, %s, %s); }\n",
				 get_int(code), get_string(mod),
				 get_string(event), get_string(cmd));
	}
	else
	{
		printf("key> Failed to extract strict typed data from script\n");
	}

	delete_arg(&code);
	delete_arg(&mod);
	delete_arg(&event);
	delete_arg(&cmd);

	return NULL;
}


int Resource::_RegressionTest(int argc, char *argv[])
{
	int r;


	if (argc > 1)
	{
		RegisterFunction("color", color);
		RegisterFunction("key", key);

		r = Load(argv[1]);
		Print();
		return r;
	}
	else
	{
		printf("_RegressionTest> ERROR: Expected filename argument\n");
	}

	return -1;
}
#endif


#include "mgtk_events.h"

int main(int argc, char *argv[])
{
	Resource test;

	printf("[Resource class test]\n");
	
	if (argc > 1)
	{
		mgtk_init(argc, argv);

		test.Load(argv[1]);
		test.Print();

		mgtk_start();
	}
}
#endif
