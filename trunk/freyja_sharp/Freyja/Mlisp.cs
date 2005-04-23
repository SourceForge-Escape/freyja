/* -*- Mode: C#; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
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
 * 2005.04.07:
 * Mongoose - Ported to C#
 *
 * 2004.01.04:
 * Mongoose - Major API changes, fun updates  =)
 *
 * 2001.11.02:
 * Mongoose - Created
 ===========================================================================*/

#define MLISP_DEBUG_ENABLED
#define INLINE_EVAL_FUNCTION

using System;
using System.IO;


enum MLispObjectType
{
	BOOLEAN     = 1,
	LIST        = 2,
	INT         = 3,
	FLOAT       = 4,
	CSTRING     = 5,
	FUNC        = 6,
	BEGIN       = 7,
	END         = 8,
	EVENT       = 9,
	NIL         = 10,
	VAREND      = 11
}


enum MLispObjectFlags
{
	mObjDisableGarbageCollection = 1
}

/* Declare delegate for Mlisp# func pointers */
public delegate MLispObject MLispFunction(ref MLispObjectList args);

public class MLispObject {

	public uint type;
	public uint flags = 0;
	public string symbol;
	public object data;
	public string mTypeName;
	public MLispFunction callback;

	
	////////////////////////////////////////////////////////////
	/// Lisp Object functions //////////////////////////////////
	////////////////////////////////////////////////////////////

	public MLispObject()
	{
		type = (uint)MLispObjectType.NIL;
		symbol = null;
		data = null;
		flags = 0;
		callback = null;
		mTypeName = null;
	}


	public void setTypeName(string name)
	{
		// I hate this for memory use, but it's fucking #C
		mTypeName = String.Copy(name);
	}


	public MLispObject execute(ref MLispObjectList objList)
	{
		MLispObject obj = new MLispObject();
		//MLispFunction func;
		
		if (type != (uint)MLispObjectType.FUNC || callback == null)
			return obj; 

		return callback(ref objList);
	}


	public bool isNumeric()
	{
		if (type == (uint)MLispObjectType.INT ||
			type == (uint)MLispObjectType.FLOAT)
		{
			return true;
		}
		
		return false;
	}


	public bool isNil()
	{
		if (type == (uint)MLispObjectType.NIL)
		{
			return true;
		}
		
		return false;
	}


	public void print()
	{
		if (type == (uint)MLispObjectType.NIL)
		{
			Console.WriteLine("NIL <> : {NIL Object}");
			return;
		}

		switch (type)
		{
		case (uint)MLispObjectType.INT:
			Console.WriteLine("int <{0}> : {1}", symbol, data);
			break;
			
		case (uint)MLispObjectType.FLOAT:
			Console.WriteLine("float <{0}> : {1}", symbol, data);
			break;
			
		case (uint)MLispObjectType.CSTRING:
			Console.WriteLine("string <{0}> : \"{1}\"", symbol, data);
			break;
			
		case (uint)MLispObjectType.FUNC:
			Console.WriteLine("func <{0}>(...)", symbol);
			break;
			
		case (uint)MLispObjectType.LIST:
			Console.WriteLine("list <{0}>", symbol);
			break;
			
		case (uint)MLispObjectType.BEGIN:
			Console.WriteLine("SCOPE BEGIN");
			break;
			
		case (uint)MLispObjectType.END:
			Console.WriteLine("SCOPE END");
			break;
			
		default:
		
			if (mTypeName == null)
			{
				Console.WriteLine("[Unknown type {0}] <{1}>",
					   type, symbol);
			}
			else
			{
				Console.WriteLine("{0} <{1}>", mTypeName, symbol);
			}
			break;
		}
	}
}


public class MLispObjectBegin : MLispObject {

	public MLispObjectBegin() : base()
	{
		type = (uint)MLispObjectType.BEGIN;
		data = null;
	}
}


public class MLispObjectEnd : MLispObject {

	public MLispObjectEnd() : base()
	{
		type = (uint)MLispObjectType.END;
		data = null;
	}
}


public class MLispObjectInt : MLispObject {

	public MLispObjectInt(int n) : base()
	{
		int i = new int();
		i = n;
		type = (uint)MLispObjectType.INT;
		data = i;
	}
}


public class MLispObjectFloat : MLispObject {

	public MLispObjectFloat(float n) : base()
	{
		float f = new float();
		f = n;
		type = (uint)MLispObjectType.FLOAT;
		data = f;
	}
}


public class MLispObjectString : MLispObject {

	public MLispObjectString(string s)
	{
		if (s == null || s == "" || s == "\0")
		{
			Console.WriteLine("Empty constant string allocated");
			return;
		}

		type = (uint)MLispObjectType.CSTRING;
		data = s;//FIXME? new string(s);
	}
}


public class MLispObjectFunc : MLispObject {

	public MLispObjectFunc(MLispFunction func, string name)
	{
		if (name == null || name == "" || name == "\0")
			return;

		type = (uint)MLispObjectType.FUNC;
		data = func;
		callback = func;
		symbol = name; //FIXME? new string(name);
	}
}


// FIXME: Rewrite this as strictly HASA, then write a ISA container for it
public class MLispObjectList : MLispObject {

	public MLispObjectList head, next;  // next is really prev b/c this is really a stack
	public MLispObject ldata;

	public MLispObjectList()
	{
		head = null;
		type = (uint)MLispObjectType.LIST;
		ldata = null;
		next = null;
	}


	public MLispObjectList(MLispObjectList src)
	{
		MLispObjectList cur = src;

		head = null;
		type = (uint)MLispObjectType.LIST;
		ldata = null;
		next = null;

		while (cur.head != null)
		{
			push(cur);
			cur.head = cur.next;
		}
		
		reverse();
	}


	public void reverse()
	{
		MLispObjectList r = new MLispObjectList();
		MLispObject obj;

		while (head != null)
		{
			obj = pop();
			r.push(obj);
		}
		
		head = r.head;
		ldata = r.ldata;
		next = r.next;
	}


	public void push(MLispObject obj)
	{
		MLispObjectList oldhead;

		if (head == null)
		{
			head = this;
			ldata = obj;
		}
		else
		{
			oldhead = head;
			head = new MLispObjectList();
			head.next = oldhead;
			head.ldata = obj;
		}
	}


	public MLispObject pop()
	{
		//MLispObjectList oldHead;
		MLispObject obj;

		if (head == null)
		{
			return null;
		}

		obj = head.ldata;
		//oldHead = head;
		head = head.next;

		return obj;
	}


	public MLispObject peek()
	{
		return ldata;
	}


	/* Heh, this should be two lists in, one back out */
	public MLispObjectList append(MLispObject obj)
	{
		MLispObjectList current;


		if (head == null)
		{
			head = this;
			head.ldata = obj;
		}
		else
		{
			current = head;

			while (current.next != null)
			{
				current = current.next;
			}

			current.next = new MLispObjectList();
			current.next.ldata = obj;
		}

		return head;
	}


	public void dispose()
	{
		while (head != null)
		{
			head.pop();
		}
	}


	new public void print(string s)
	{
		MLispObjectList current = head;
		MLispObject obj;


		if (head == null)
		{
			Console.WriteLine("ObjectList is NIL.");
			return;
		}

		while (current != null)
		{
			obj = current.peek();
			current = current.next;
			Console.Write(s);
			obj.print();
		}
	}
	
	
	new public void print()
	{
		MLispObjectList current = head;
		MLispObject obj;


		if (head == null)
		{
			Console.WriteLine("ObjectList is NIL.");
			return;
		}

		while (current != null)
		{
			obj = current.peek();
			current = current.next;
			obj.print();
		}
	}
}


public class MLispObjectEvent : MLispObject {

	public MLispObjectEvent(string name, EventHandler e) : base()
	{
		symbol = name;
		data = e;
		type = (uint)MLispObjectType.EVENT;
	}
}


public class Mlisp {

	enum BuiltinFunc  /* This is used to bind interal methods to index */
	{
		SetQ = 1,
		Nil,
		First,
		Rest
	}

	MLispObjectList mSymbolTable;    /* Holds all the symbols, data, func */
	MLispObjectList mDataStack;      /* Stack of records for internal use */
	MLispObjectList mExecStack;      /* Execution stack for internal use */

	/* RDP goodies */
	char [] mSymbol;  	         /* Symbol buffer */
	char [] mBuffer;              /* Eval buffer */
	uint mBufferSize;    		 /* Symbol buffer byte size */
	uint mSymbolSize;    		 /* Eval buffer byte size */
	uint mTop;
	char mLook;

	BinaryReader mTape; 

	int mLine;
	int mString;
	int mErrors;                  /* Parse error counter */
	int mDebug;                   /* Level of debugging for parser */
	bool mGarbageCollection;

	char EOF = (char)26;

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////	
	
	public Mlisp()
	{
		mSymbolTable = new MLispObjectList();
		mExecStack = new MLispObjectList();
		mDataStack = new MLispObjectList();
		mBuffer = null;
		mSymbol = null;
		mBufferSize = 0;
		mSymbolSize = 256;
		mTop = 0;
		mLook = '\0';
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
		 * MLispObject     - Element
		 * MLispObjectList - List
		 */
	
		/* Append lisp built-in functions to symbol table */
		registerLispFunction("setq", new MLispFunction(setq));
		registerLispFunction("nil", new MLispFunction(nil));
		registerLispFunction("first", new MLispFunction(first));
		registerLispFunction("car", new MLispFunction(first));
		registerLispFunction("cdr", new MLispFunction(rest));
		registerLispFunction("rest", new MLispFunction(rest));
		registerLispFunction("add", new MLispFunction(madd));
		registerLispFunction("+", new MLispFunction(madd));
		registerLispFunction("sub", new MLispFunction(subtract));
		registerLispFunction("-", new MLispFunction(subtract));
		registerLispFunction("mul", new MLispFunction(multiply));
		registerLispFunction("*", new MLispFunction(multiply));
		registerLispFunction("div", new MLispFunction(divide));
		registerLispFunction("/", new MLispFunction(divide));
		registerLispFunction("mlispdebug", new MLispFunction(setDebug));

		/* Append lisp built-in variables */
		registerSymbolValue("T", 0);
		
		// FIXME, Won't do generic symbol binding
		//registerSymbol("cDebugLevel", (uint)MLispObjectType.INT, ref mDebug);
	}

	
	~Mlisp()
	{
		clear();
	}


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	public void dumpSymbols()
	{
		Console.WriteLine("\nDumping symbols:");
		Console.WriteLine("------------------------------------------------------------");
		mSymbolTable.print();
		Console.WriteLine("------------------------------------------------------------");
	}


	public MLispObject getSymbol(string symbol)
	{
		MLispObjectList current;
		MLispObject obj;
		
		if (symbol == null || symbol == "" || symbol == "\0")
			return null;

		current = mSymbolTable;

		while (current != null)
		{
			obj = current.peek();
			current = current.next;

			if (symbol.CompareTo(obj.symbol) == 0)
			{
				return obj;
			}
		}

		return null;
	}


	public object getSymbolData(char []symbol, uint type)
	{
		MLispObjectList current;
		MLispObject obj;
		// Hack to trim C# strings
		string tmp = new string(symbol);
		string sym = tmp.Substring(0, tmp.IndexOf('\0'));

				
		if (symbol == null || symbol[0] == '\0')
			return null;

		current = mSymbolTable;

		while (current != null)
		{
			obj = current.peek();
			current = current.next;

			//Console.WriteLine("getSymbolData$ '{0}' '{1}'", sym, obj.symbol);
			//Console.WriteLine("getSymbolData$ '{0}' '{1}'", sym.Length, obj.symbol.Length);

			if (sym.CompareTo(obj.symbol) == 0)
			{
				/* If type == 0 then return the whole object */
				if (type == 0) 
				{
					return obj;
				}
				else if (obj.type == type)
				{
					return obj.data;
				}
			}
		}

		return null;
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	public void setDebugLevel(int level)
	{
		mDebug = level;
	}


	public int evalBuffer(char []buffer)
	{
		if (parseEvalBuffer(buffer) == 0)
		eval();

		return 0;
	}


	public int evalFile(string filename)
	{
		if (mBuffer != null)
		{
			//FIXME delete [] mBuffer;
			mBuffer = null;
		}

		if (mSymbol != null)
		{
			//FIXME delete [] mSymbol;
			mSymbol = null;
		}

		if (mSymbolSize < 65)
		{
			mSymbolSize = 65;
		}

		mSymbol = new char[mSymbolSize];

		if (bufferFile(filename, ref mBuffer, ref mBufferSize) == 0)
		{
			if (evalBuffer(mBuffer) == 0)
			{
				return 0;
			}
		}

		return -1;
	}


	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	bool isAtoZ(char c)
	{
		return ((byte)c > 64 && (byte)c < 91);
	}


	bool isatoz(char c)
	{
		return ((byte)c > 96 && (byte)c < 123);
	}


	bool isDigit(char c)
	{
		return ((byte)c > 47 && (byte)c < 58);
	}


	bool isMisc(char c)
	{
		if ((byte)c == 95)
			return true;

		return false;
	}


	bool isNumeric(char []symbol, ref float n)
	{
		int len, i;
		bool real = false;
		string s;

	
		if (symbol == null || symbol[0] == '\0')
			return false;
	
		len = symbol.Length;
	
		for (i = 0; i < len; ++i)
		{
			// Mongoose: Handle null terminated string in C#
			if (symbol[i] == '\0')
			{
				break;
			}
		
			// Mongoose: 2001-10-31 Handle '[0-9]+[.][0-9]+'
			if (i != 0 && symbol[i] == '.')
			{
				real = true;
				continue;
			}

			// Mongoose 2002.01.23, Handle '-[0-9]+'
			if (i == 0 && symbol[i] == '-')
			{
				continue;
			}

			if (!isDigit(symbol[i]))
			{
				return false;
			}
		}

		s = new string(symbol);

		if (real)
		{
			n = (float)Convert.ToDouble(s);
		}
		else
		{
			n = Convert.ToInt32(s);
		}

		return true;
	}


	void printError(string s)
	{
		//Console.OpenStandardError();
		Console.WriteLine("lisp> ERROR #{0}: Line {1}, {2}", mErrors, mLine, s);
	}


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	bool appendSymbolTable(MLispObject obj)
	{
		mSymbolTable = mSymbolTable.append(obj);
		return (mSymbolTable != null);
	}


	void bind(string symbol, MLispObject data)
	{
		if (symbol == null || symbol == "" || symbol == "\0" || data == null)
		{
			++mErrors;
			printError("BIND receieved invalid symbol for binding");
			//deleteObj(&data);
			data = null;
			return;
		}

		/* Make a copy of the symbol to store in Object */
		data.symbol = symbol;

		/* Append to the lisp object/symbol table */
		appendSymbolTable(data);
	}


	int bufferFile(string filename,	ref char []buffer, ref uint bytes)
	{
		uint i;
		buffer = null;


		if (filename == null || filename == "" || filename == "\0")
		{
			return -1;
		}

		FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
		StreamReader r = new StreamReader(fs);

		try {

			r.BaseStream.Seek(0, SeekOrigin.End); 
			bytes = (uint)r.BaseStream.Position;
			r.BaseStream.Seek(0, SeekOrigin.Begin); 

			/* FIXME: Might want to go ahead and strip comments here,
			 *        and go ahead and do preprocessing instead of
			 *        a straight file buffer.
			 */

			buffer = new char[bytes+1];
			byte []tmp = new byte[bytes]; // lame I know
			r.BaseStream.Read(tmp, 0, (int)bytes);
			
			for (i = 0; i < bytes; ++i)  // argh
				buffer[i] = (char)tmp[i];
				
			buffer[i] = EOF; // C# fix/hack/etc
		}

		catch (EndOfStreamException e) {
			Console.WriteLine("{0} exceptions caught and ignored.", e.GetType().Name);
			return -2;
		}
		finally
		{
			r.BaseStream.Close();
		}

		return 0;
	}


	void clear()
	{
		while (mDataStack.head != null)
		{
			mDataStack.pop();
		}

		while (mExecStack.head != null)
		{
			mExecStack.pop();
		}

		while (mSymbolTable.head != null)
		{
			//MLispObject obj = mSymbolTable.peek();
			//deleteObj(&obj);
			//obj = null;
			mSymbolTable.pop();
		}

		if (mBuffer != null)
		{
			// delete [] mBuffer;
			mBuffer = null;
		}

		if (mSymbol != null)
		{
			// delete [] mSymbol;
			mSymbol = null;
		}
	}


	MLispObjectList getNextScopeStack(ref MLispObjectList stack)
	{
		MLispObjectList tmpStack = new MLispObjectList();
		MLispObject obj = new MLispObject();
		
		
		while (stack.head != null)
		{
			obj = stack.pop();
						
			if (obj == null ||
				obj.isNil() ||
				obj.type == (uint)MLispObjectType.END)
			{
				/* End with NIL Object for var parm func (func ...) */
				tmpStack.push(new MLispObject());
				 
				break; 
			}

			//Console.Write(">> ");
			//obj.print();

			tmpStack.push(obj);
		}
		
		tmpStack.reverse();
		
		return tmpStack;
	}


	MLispObject evalFunction(ref MLispObjectList stack, MLispObject func)
	{
		MLispObjectList parms = null;
		MLispObjectList fstack = new MLispObjectList();
		MLispObject obj = new MLispObject();
		MLispObject result = new MLispObject();
		int scope = 0;


		if (mDebug > 2)
			Console.WriteLine("-begin----------------------");

		if (func.type != (uint)MLispObjectType.FUNC)
		{
			Console.WriteLine("FATAL ERROR: Lisp eval passed unbound function");
			return result;
		}
		else
		{
			if (mDebug > 1)
				func.print();
		}

		/* 0. Push func obj to grouped function stack */
		fstack.push(func);

		/* 1. Pop a Scope BEGIN obj */
		obj = stack.head.peek();

		if (obj.type != (uint)MLispObjectType.BEGIN)
		{
			if (mDebug > 6)
			{
				MLispObjectList current;
				Console.Write("FATAL ERROR: Lisp stack corrupt... BEGIN != ");
				obj.print();
				Console.WriteLine("Stack dump follows:");
				Console.WriteLine("**************************************");
			
				current = stack.head;
				while (current != null)
				{
					obj = current.peek();
					obj.print();
				
					current = current.next;
				}
			
				Console.WriteLine("**************************************");
			}
			
			return result;
		}


		/* 2. Copy stack items to parms stack in same order until end of this
		 *    function's scope is reached, eval passed functions as well */
		while (stack.head != null)
		{
			obj = stack.head.peek();

			if (mDebug > 2)
				obj.print();

			if (obj.type == (uint)MLispObjectType.BEGIN)
			{
				++scope;
				
				if (parms == null)
				{
					parms = new MLispObjectList();
				}

				parms.push(obj); // 20050520, Added to provide scope seperator
			}
			else if (obj.type == (uint)MLispObjectType.END)
			{
				--scope;
				parms.push(obj); // 20050520, Added to provide scope seperator
				
				if (scope == 0)
				{
					if (mDebug > 1)
					{
						Console.WriteLine("xxxxxxxxxxxxxxxxxxxxxxxxxxxx");
						fstack.print();
						Console.WriteLine("xxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					}
					
					/* 2.1. Pop func off fstack -- if nil break */
					//func = fstack.pop();
	
					/* 2.2. if non-nil continue to execute and later push result */		
					if (func.isNil())
						break;
		
					/* In case function was passed no args, then make a null list */
					if (parms == null)
						parms = new MLispObjectList();

					/* Reverse stack, so that order is maintained in callee from mlisp C */
					MLispObjectList reverse = new MLispObjectList();
					while (parms.head != null)
					{
						obj = parms.pop();
					
						if (mDebug > 1)
						{
							Console.Write("Buffering AR... ");
							obj.print();
						}

						if (obj.type == (uint)MLispObjectType.FUNC)
						{
							if (mDebug > 1)
							{
								Console.WriteLine("--- FUNC ----------------------");				
								Console.WriteLine("Calling {0}", obj.symbol);
								reverse.print("\te ");
							}
							
							MLispObjectList tmp = getNextScopeStack(ref reverse);
							//tmp.print("  e> ");
							obj = obj.execute(ref tmp);
							reverse.push(obj);
						
							if (mDebug > 1)
							{
								Console.Write("\t<-- ");
								obj.print();
							}
													
							fstack.pop(); // cull 'obj', and yes this should work
							continue;
						}
						else if (obj.type == (uint)MLispObjectType.BEGIN)
						{
							//Console.Write("Stripping AR... ");
							//obj.print();
							continue;
						}
						else if (obj.type == (uint)MLispObjectType.END)
						{
							//Console.Write("Stripping AR... ");
							//obj.print();
							//continue;
							
						}

						reverse.push(obj);
					}

					parms = reverse;		

					func = fstack.pop();

					/* 2.3. Call function implementation */
					switch (func.type)
					{
					case (uint)MLispObjectType.FUNC:
						//Console.WriteLine("FUNC: Calling {0}", func.symbol);
						//reverse.print("\t+ ");
						result = func.execute(ref reverse);
						break;

					default:
						//Console.WriteLine("{0}", func.symbol);
						break;
					}

					parms.push(result);

					// Show return value
					if (mDebug > 1)
					{
						Console.Write("\t <-- : ");
						result.print();
					}

					if (func == null || func.isNil() || fstack.head == null)
						break;
				}
			}

			/* This was a switch in C, but C# can't do fall through,
			 * Cull scope markers, setup call stack */
			if (obj.type == (uint)MLispObjectType.BEGIN ||
				obj.type == (uint)MLispObjectType.END)
			{
			}
			else
			{
				if (obj.type == (uint)MLispObjectType.FUNC)
				{
					//Console.WriteLine("fstack.push({0})", obj.symbol);
					fstack.push(obj);
				}
				
				if (parms == null)
				{
					parms = new MLispObjectList();
				}

				parms.push(obj);
			}
	
			stack.head = stack.head.next;
		}

		if (mDebug > 2)
		{
			Console.WriteLine("-end------------------------\n\n");
		}

		/* 4. Return the result to eval system */
		return result;
	}


	void eval()
	{
		MLispObjectList parms = new MLispObjectList();
		MLispObjectList currentStack = new MLispObjectList();
		MLispObjectList curStack = new MLispObjectList();
		MLispObjectList cur = new MLispObjectList();
		MLispObject obj = null;
		MLispObject result = null;
		MLispObjectList reverse = new MLispObjectList();	
		uint frame = 0;
	
	
		if (mDebug > 0)
		{
			Console.WriteLine("--------------------------------");
			Console.WriteLine("Evaluating lisp");
		}

		/* Reverse stack, so that order is maintained in execution from parsing */
		while (mExecStack.head != null)
		{
			obj = mExecStack.pop();
			reverse.push(obj);
		}
	
		mExecStack = reverse;

		/* Generate ExecStack frames */
		while (mExecStack.head != null)
		{
			obj = mExecStack.pop();

			if (obj != null && obj.type == (uint)MLispObjectType.FUNC)
			{
				if (mDebug > 1)
				{
					Console.Write("evalFunction <-- ");
					obj.print();
				}
				evalFunction(ref mExecStack, obj);
			}
		}

		while (mExecStack.head != null)
		{
			obj = mExecStack.pop();

			if (mDebug > 4)
			{
				/* Not an AR or stack frame, just a stack use counter */
				Console.WriteLine("-frame {0}----------------", ++frame);
			}

			if (obj == null)
			{
				printError("FATAL ERROR: eval() fed null data");
				return;
			}
			else
			{
				if (obj.type == (uint)MLispObjectType.FUNC)
				{
					/* 1. Pop a Scope BEGIN obj */
					obj = currentStack.pop();

					if (obj == null || obj.type != (uint)MLispObjectType.BEGIN)
					{
						Console.Write("FATAL ERROR: Lisp data stack corrupt... BEGIN != ");
						obj.print();
					}
	
					/* 2. Copy currentStack items to parms stack in same order */
					parms = null;
					curStack = currentStack;

					//Console.WriteLine("=");
					while (curStack.head != null)
					{
						obj = curStack.peek();
	
						if (obj.type == (uint)MLispObjectType.END)
							break;
	
						if (parms == null)
						{
							parms = new MLispObjectList();
							cur = parms;
						}
						else
						{
							cur.next = new MLispObjectList();
							cur = cur.next;
						}
	
						//obj.print();						
						cur.data = obj;
						cur.next = null;

						curStack.head = curStack.head.next;
					}
					//Console.WriteLine("=");

					/* 3. Call C function implementation, or method */
					switch (obj.type)
					{
					case (uint)MLispObjectType.FUNC:
						result = obj.execute(ref parms);
						break;

					default: // Was commented out and set to fall to BUILTINFUNC
						Console.WriteLine("{0}", obj.symbol);
						//assert(obj.type == FUNC || obj.type == BUILTINFUNC);
						break;
					}

					/* 4. Pop off this entire scope now */
					while ((obj = currentStack.pop()) != null)
					{
						if (mDebug > 4)
						{
							Console.WriteLine("POP ");
							obj.print();
						}
	
						if (obj.type == (uint)MLispObjectType.END)
							break;
	
						if (mGarbageCollection)
						{
							/* Weak garbage collect for now */
							//deleteObj(&obj);
							obj = null;
						}
					}
	
					/* 5. Push result on stack */
					if (mDebug > 4)
					{
						Console.WriteLine("RESULT_PUSH ");
						result.print();
					}
	
					currentStack.push(result);
				}
				else if (obj.type == (uint)MLispObjectType.BEGIN)
				{
					/* END objects mark the end of a function parm list */
	
					if (mDebug > 4)
					{
						Console.WriteLine("BEGIN_PUSH");
					}
	
					currentStack.push(obj);
	
					//objPush(&mDataStack, newListObj(currentStack));
					//obj = objPeek(mDataStack);
					//currentStack = (MLispObjectList *)obj.data;
				}
				else if (obj.type == (uint)MLispObjectType.END)
				{
					/* BEGIN objects mark the start of a function parm list */
	
					if (mDebug > 4)
					{
						Console.WriteLine("END_PUSH");
					}
	
					currentStack.push(obj);
	
					//obj = objPop(&mDataStack);
					//currentStack = (MLispObjectList *)obj.data;
					//printObjectList(currentStack);
				}
				else
				{
					/* Everything else push into the scope list */
	
					if (mDebug > 4)
					{
						Console.WriteLine("DATA_PUSH ");
						obj.print();
					}
	
					currentStack.push(obj);
				}
			}
		}
	
#if MLISP_DEBUG_ENABLED
		  if (mDebug > 0)
		  {
			  Console.WriteLine("--------------------------------");
		  }
#endif

		  //dumpListSymbols(currentStack);
		  //objPush(&mStack, newListObj(currentStack));
	}


	void expected(char c)
	{
		string s = "";

		switch (mLook)
		{
		case '\n':
			s += "EOL";
			break;
		case '\v':
			s += "\\v";
			break;
		case '\t':
			s += "TAB";
			break;
		default:
			s += mLook;
			break;
		}

		++mErrors;
		printError("Expected '" + c +"', not '"  + s + "' (" + mLook + ")\nLast symbol before error: '" + new string(mSymbol) + "'");
	}


	MLispObject getNextSymbol()
	{
		MLispObject obj;

		parseSeperator();

		if (mLook == ';' && mString == 0)
		{
			parseComment();
			parseSeperator();
		}

		obj = parseNextSymbol();
	
		return obj;
	}


	bool isLook(char c)
	{
  		return (c == mLook);
	}


	void lex()
	{
		if (mTop < mBufferSize)
		{
			++mTop;
			mLook = mBuffer[mTop];

#if MLISP_DEBUG_ENABLED
			if (mDebug > 9)
			{
				Console.Write("{0}", mLook);
			}
#endif

			if (mLook == ';' && mString == 0)
			{
				parseComment();
			}
		}
		else
		{
			mLook = EOF;
		}
	}


	bool match(char c)
	{
		bool r = isLook(c);


		if (!r)
		{
			expected(c);
		}

		lex();

		return r;
	}


	void parseComment()
	{
		match(';');

		while (!isLook('\n') && !isLook(EOF))
		{
			lex();
		}

		if (isLook(';'))
		{
			parseComment();
		}
	}


	int parseEvalBuffer(char []buffer)
	{
		MLispObject obj;


		if (buffer == null || buffer[0] == '\0')
		{
			return -1;
		}
	
#if MLISP_DEBUG_ENABLED
		if (mDebug > 0)
		{
			Console.WriteLine("> Parsing eval buffer...");
		}

		if (mDebug > 14)
		{
			Console.WriteLine("> {0}", buffer);
		}
#endif
	
		/* Start counting lines at 1, errors at 0 */
		mLine = 1; 
		mErrors = 0;

		/* Reset parsing counters */
		mTop = 0;
		mLook = (char)0;
		mString = 0;

		lex();
		parseSeperator();

		while (isLook('('))
		{
			obj = parseFunction();

#if MLISP_DEBUG_ENABLED
			if (mDebug > 4)
			{
				Console.Write("Pushing, ");
				obj.print();
			}
#endif

			//objPush(&mExecStack, obj);
			parseSeperator();
		}

		if (mErrors != 0)
		{
			Console.WriteLine("\nlisp> Encountered {0} parse errors\n", mErrors);
		}

		return 0;
	}


	MLispObject parseFunction()
	{
		MLispObject func;
		MLispObject obj;
		object data = null;


		match('(');

		func = getNextSymbol();

		data = getSymbolData(mSymbol, (uint)MLispObjectType.FUNC); // changed for 1.1.6

		if (data != null)
		{
			mExecStack.push(func);
		}
		else
		{
			++mErrors;
			printError("Invalid function '" + new string(mSymbol) + "'");
			return null;
		}

#if MLISP_DEBUG_ENABLED
		if (mDebug > 4)
		{
			Console.WriteLine("> BEGIN {0}", func.symbol);
		}
#endif

		mExecStack.push(new MLispObjectBegin());

		/* Handle function parms, must be either FUNC objects or 
		 * the elements must be composed into a object list 
		 * pushed on the stack */
		while (isLook(')') == false)
		{
			obj = getNextSymbol();

#if MLISP_DEBUG_ENABLED
			if (mDebug > 4)
			{
				Console.Write("   Pushing parm, ");
				obj.print();
			}
#endif
		
			if (obj != null && obj.type != (uint)MLispObjectType.FUNC)
			{
				mExecStack.push(obj);
			}
		}

#if MLISP_DEBUG_ENABLED
		if (mDebug > 4)
		{
			Console.WriteLine("> END {0}", func.symbol);
		}
#endif

		mExecStack.push(new MLispObjectEnd());

		match(')');
		parseSeperator();

		return func;
	}


	MLispObject parseNextSymbol()
	{
		int i = 0, j = 0;
		MLispObject obj = null;
		bool isString = false;
		bool fp = false;
		float f = 0.0f;


		/* C# seems to ignore null terminated strings when converted to string
		 * so instead of just marking the end with null chars we mark all */
		for (i = 0; i < mSymbolSize; ++i)
			mSymbol[i] = '\0';

		i = 0;

		if (isLook('"'))
		{
			lex();
			isString = true;
		}
	
		/* Mongoose 2001.11.09: 
		 * Best handle for? grouped (func (func */
		if (isLook('('))
		{
			return parseFunction();
		}
	
		while (isString || (i == 0 && mLook == '-') ||
			   isatoz(mLook) || isAtoZ(mLook) || isDigit(mLook) || isMisc(mLook))
		{
			if (i < (int)mSymbolSize)
			{
				mSymbol[i++] = mLook;
				mSymbol[i] = '\0';
			}

			if (isString && isLook('"'))
			{
				--i;
				mSymbol[i] = '\0';
				lex();
				break;
			}
			
			lex();
			
			if (isString)
			{
				continue;
			}
			
			if (isLook('.'))
			{
				fp = true;
				
				for (j = 0; j < i; ++j)
				{
					if (j == 0 && mSymbol[j] == '-')
						continue;
					
					if (!isDigit(mSymbol[j]))
						break;
				}
	
				if (i == j)
				{
					mSymbol[i++] = mLook;
					mSymbol[i] = '\0';
					lex();
				}
			}
		
		
			// Mongoose 2002.01.23, Hhhmm... fixes -100.00 and -100, 
			//   but is it 'good idea'?
			if (isLook('-') && i == 0)
			{
				mSymbol[i++] = mLook;
				mSymbol[i] = '\0';
				lex();
			}
		}

#if MLISP_DEBUG_ENABLED
		if (mDebug > 7)
		{
			Console.WriteLine("> New symbol <{0}>", new string(mSymbol));
		}
#endif
	
		if (isNumeric(mSymbol, ref f))
		{
			if (fp)
			{
				obj = new MLispObjectFloat(f);
			}
			else
			{
				obj = new MLispObjectInt((int)f);
			}
		}
		else
		{
			/* Handle symbol references here */
			if ((obj = (MLispObject)getSymbolData(mSymbol, (uint)0)) != null)
			{
				// FIXME
			}
			else
			{
				// Hack to trim C# strings
				string tmp = new string(mSymbol);
				string tmp2 = tmp.Substring(0, tmp.IndexOf('\0'));
				//string tmp2 = new String(mSymbol, 0, tmp.IndexOf('\0'));
				
				if (!isString)
				{
					++mErrors;
					printError("Making string out of non-string <" + tmp2 + ">?");
				}

				obj = new MLispObjectString(tmp2);
			}
		}
	
		return obj;
	}


	void parseSeperator()
	{
		// Real whitespace stripping
		while (isLook(' ') || isLook('\t') || isLook('\n'))
		{
			if (isLook('\n'))
			{
				mLine++;
			}
			
			lex();
		}
	}


	void parseString(char []s, int len)
	{
		int i = 0;
	
	
		match('"');
		mString = 1;
		len -= 2;
	
		while (!isLook('"') && !isLook(EOF))
		{
			if (i < len)
			{
				s[i++] = mLook;
				s[i] = '\0';
			}
		
			lex();
		}

		match('"');
		mString = 0;
	}


	public int registerLispFunction(string symbol, MLispFunction func)
	{
		if (func == null)
		{
			return -1;
		}

		if (appendSymbolTable(new MLispObjectFunc(func, symbol)))
			return 1;
			
		return 0;
	}


	bool registerSymbol(string symbol, uint type, ref object data)
	{
		MLispObject obj;


		if (symbol == null || symbol == "" || symbol == "\0")
		{
			return false;
		}

		obj = new MLispObject();
		obj.type = type;
		obj.data = data;
		obj.flags = (uint)MLispObjectFlags.mObjDisableGarbageCollection;
	
		bind(symbol, obj);

		return true;
	}


	public int registerSymbolValue(string symbol, float d)
	{
		MLispObject obj = new MLispObjectFloat(d);


		if (registerSymbolObject(symbol, obj) < 0)
		{
			//delete obj;
			obj = null;
			return -1;
		}

		obj.flags = (uint)MLispObjectFlags.mObjDisableGarbageCollection;

		return 0;
	}


	int registerSymbolValue(string symbol, int i)
	{
		MLispObject obj = new MLispObjectInt(i);

		if (registerSymbolObject(symbol, obj) < 0)
		{
			//delete obj;
			obj = null;
			return -1;
		}

		return 0;
	}


	// Should deep copy?
	MLispObject setq(ref MLispObjectList parms)
	{
		MLispObject symbol, data, obj;
		obj = new MLispObject();

		
		//if (parms == null || parms.peek() == null)
		//{
		//	Console.WriteLine("setq> ERROR Invalid parms");
		//	return obj; // NIL Object
		//}
		
		symbol = parms.pop();
		data = parms.pop();

		if (data.type == (uint)MLispObjectType.CSTRING)
		{
			obj = getSymbol((string)data.data);
		}

		if (obj != null && !obj.isNil())
		{
			registerSymbol((string)symbol.data, obj.type, ref obj.data);
		}
		else
		{
			registerSymbol((string)symbol.data, data.type, ref data.data);
			obj = data;
		}

		return obj;
	}


	int registerSymbolValue(string symbol, string s)
	{
		MLispObject obj = new MLispObjectString(s);

		if (registerSymbolObject(symbol, obj) < 0)
		{
			//FIXME deleteObj(&obj);
			return -1;
		}

		return 0;
	}


	int registerSymbolObject(string symbol, MLispObject obj)
	{
		if (symbol == null || symbol == "" || symbol == "\0" || obj == null)
		{
			Console.WriteLine("Error: Couldn't bind invalid null object symbol");
			return -1;
		}

		bind(symbol, obj);

		return 0;
	}


	////////////////////////////////////////////////////////////
	/// Internal built-in functions ////////////////////////////
	////////////////////////////////////////////////////////////

	/* (append ELEMENT LIST )
	 * Returns list which is LIST with 
	 * ELEMENT appended to the end. */

	MLispObject numberp(ref MLispObjectList parms)
	{
		//if (object.type == INT || object.type == FLOAT)
		//	return (MLispObject )1;  /* FIXME: ew bad form old bean */

		return null;
	}


	MLispObject nil(ref MLispObjectList parms)
	{
		if (parms == null || parms.head == null)
			return null;
			
		return new MLispObject();
	}

	
	/* LIST (rest LIST)
	 * Returns the list of remaining elements 
	 * after the first element of LIST. */
	MLispObject rest(ref MLispObjectList args)
	{
		MLispObjectList list = new MLispObjectList();
		
		/* Deep copy duplication for remaining list, if non-null */
		if (args == null)
			return null;
	
		return new MLispObjectList(list.next);
	}

 
	/* (endp x)
	 * Determines whether x is the empty list, 
	 * returning the symbolic atom T if x=(), 
	 * or the symbolic atom F if x is nonempty. */

	/* (first LIST) 
	 * Returns the first element of LIST. */
	MLispObject first(ref MLispObjectList list)
	{
		if (list != null)
			return list.peek();

		return null;
	}

	/* (cons ELEMENT LIST)
	 * Returns a new list consisting of the ELEMENT, 
	 * followed by the elements of LIST. */
	MLispObject cons(ref MLispObjectList list)
	{
		/* FIXME */
		return null;
	}


	////////////////////////////////////////////////////////////
	/// Internal built-in math functions ///////////////////////
	////////////////////////////////////////////////////////////

	/* Assign value */
	MLispObject assign(ref MLispObjectList list)
	{
		MLispObjectList current = list;
		MLispObject objA;
		MLispObject objB;
		//uint length;


		objA = current.pop();
		objB = current.pop();

		if (objA != null && objB != null && objA.type == objB.type)
		{
			switch (objA.type)
			{
			case (uint)MLispObjectType.INT:
				objA.data = objB.data;
				break;
			case (uint)MLispObjectType.FLOAT:
				objA.data = objB.data;
				break;
			case (uint)MLispObjectType.CSTRING:
				if (objA.data != null && objA.data != "" && objA.data != "\0")
				{
					/* Mongoose 2004.01.13, 
					 * Shallow copies only -- could fuck up CVARS otherwise */
					string s = (string)objB.data;
					objA.data = s;
				}
				break;
			default:
				objA.data = objB.data;
				break;
			}
		}
	
		return objA;
	}


	MLispObject setDebug(ref MLispObjectList args)
	{
		MLispObject obj = args.pop();

		if (obj.isNumeric())
		{
			mDebug = (int)obj.data;
		}

		return new MLispObject();
	}
	
	
	MLispObject madd(ref MLispObjectList args)
	{
		MLispObject obj;
		float n = 0.0f;


		while ((obj = args.pop()) != null && !obj.isNil())
		{
			//Console.Write("\t+ arg ");
			//obj.print();

			switch (obj.type)
			{
			case (uint)MLispObjectType.FLOAT:
				n += (float)obj.data;
				break;
			case (uint)MLispObjectType.INT:
				n += (int)obj.data;
				break;
			default:
				continue;
			}
		}

		return new MLispObjectFloat(n); 
	}


	MLispObject subtract(ref MLispObjectList args)
	{
		MLispObject obj;
		float n = 0.0f, s;
		char first = (char)1;


		while ((obj = args.pop()) != null && !obj.isNil())
		{
			switch (obj.type)
			{
			case (uint)MLispObjectType.FLOAT:
				s = (float)obj.data;
				break;
			case (uint)MLispObjectType.INT:
				s = (int)obj.data;
				break;
			default:
				continue;
			}

			if (first != 0)
			{
				n = s;
				first = (char)0;
			}
			else
			{
				n -= s;
			}
		}

		return new MLispObjectFloat(n); 
	}


	MLispObject multiply(ref MLispObjectList list)
	{
		MLispObjectList current = list;
		MLispObject obj;
		float n = 0.0f, m;
		char first = (char)1;

	
		while (current != null)
		{
			obj = current.peek();
			current = current.next;

			switch (obj.type)
			{
			case (uint)MLispObjectType.FLOAT:
				m = (float)obj.data;
				break;
			case (uint)MLispObjectType.INT:
				m = (int)obj.data;
				break;
			default:
				continue;
			}

			if (first != 0)
			{
				n = m;
				first = (char)0;
			}
			else
			{
				n *= m;
			}
		}

		return new MLispObjectFloat(n); 
	}


	MLispObject divide(ref MLispObjectList list)
	{
		MLispObjectList current = list;
		MLispObject obj;
		float n = 0.0f, d = 1.0f;
		char first = (char)1;

		while (current != null)
		{
			obj = current.peek();
			current = current.next;
	
			switch (obj.type)
			{
			case (uint)MLispObjectType.FLOAT:
				d = (float)obj.data;
				break;
			case (uint)MLispObjectType.INT:
				d = (int)obj.data;
				break;
			default:
				continue;
			}
	
			if (first != 0)
			{
				first = (char)0;
				n = d;
			}
			else
			{
				if (d == 0.0)
				{
					Console.WriteLine("Error: Divide by zero avoided in lisp math");
				}
				else
				{
					n /= d;
				}
			}
		}

		return new MLispObjectFloat(n); 
	}
	

	////////////////////////////////////////////////////////////
	// Unit Test
	////////////////////////////////////////////////////////////

#if UNIT_TEST_MLISP
	MLispObject menu(MLispObjectList *parms)
	{
		MLispObject xPos, *yPos, *ret, *obj;


		xPos = objPop(&parms);
		yPos = objPop(&parms);
		ret = null;
	
		if (objNumberP(xPos) && objNumberP(yPos))
		{
			/* Call event func here - simulated with printf for test */
			Console.WriteLine("extern \"C\" { menu({0}, {1}); }",
				   getNumber(xPos), getNumber(yPos));

			while ((obj = objPeek(parms)))
			{
				Console.WriteLine("             submenu?, ");
				obj.print();
			
				objPop(&parms);
			}

			ret = newStringObj("Menu");
		}
		else
		{
			Console.WriteLine("menu> Failed to extract strict typed data from script");
		}
	
		/* Would return menu struct here if it wasn't a simulation */
		return ret; 
	}


	MLispObject submenu(MLispObjectList *parms)
	{
		MLispObject label, *obj, *ret = null;


		label = objPop(&parms);

		if (objTypeP(label, CSTRING))
		{
			/* Call event func here - simulated with printf in tests */
			Console.WriteLine("extern \"C\" { submenu(\"{0}\"); }",
					 getString(label));

			while ((obj = objPeek(parms)))
			{
				Console.WriteLine("             menuitem?, ");
				obj.print();
			
				objPop(&parms);
			}
	
			ret = newStringObj(getString(label));
		}
		else
		{
			Console.WriteLine("submenu> Failed to extract strict typed data from script");
		}

		/* Would return submenu struct here if it wasn't a simulation */
		return ret;
	}


	MLispObject menu_item(MLispObjectList *parms)
	{
		MLispObject label, *event, *ret;


		label = objPop(&parms);
		event = objPop(&parms);
		ret = null;
	
		if (objTypeP(label, CSTRING) && objTypeP(label, INT))
		{
			/* Call event func here - simulated with printf in tests */
			Console.WriteLine("extern \"C\" { menu_item(\"{0}\", {1}); }",
					 getString(label), getInt(event));
	
			ret = newStringObj(getString(label));
		}
		else
		{
			printObj(label);
			printObj(event);
			Console.WriteLine("menu_item> Failed to extract strict typed data from script");
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
			Console.WriteLine("{0} FILENAME.LISP", argv[0]);
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
				Console.WriteLine("Couldn't load file!");
			}
			else
			{
				// rc.evalBuffer("(+ 23 42 54 23)");
			}

			rc.dumpSymbols();
		}

		return -1;
	}
#endif
}

