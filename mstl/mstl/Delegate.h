/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongooseichiban@gmail.com
 * Object  : MethodDelegate
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: 
 * 
 *-- NOTES --------------------------------------------------
 *
 * Q: Hey, these are just like your thread delegates?!
 *
 * A: These do NOT store arguments, which the other 'delegates' do.  They
 *    are like function calls with stored parameters to go with them.
 *
 *    The only thing stored along with the function pointer is the object.
 *
 * Q: How do I use this wonderus abstraction you provide?
 * A: Example code always helps...
 *
 * MethodDelegate<YourClass, void> d = MethodDelegate(this, &YourClass::Method);
 * d.Run();
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.04.01:
 * Mongoose - Created, abstracted from my ThreadDelegate test code
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_METHODDELEGATE_H_
#define GUARD__FREYJA_MONGOOSE_METHODDELEGATE_H_

namespace mstl {

class ArgList
{
public:
	ArgList() {}
	virtual ~ArgList() {}
};

class MethodDelegate
{
 public:

	MethodDelegate() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Action
	 *
	 ------------------------------------------------------*/

	virtual ~MethodDelegate() {}
	/*------------------------------------------------------
	 * Pre  : Action object is allocated
	 * Post : Deconstructs an object of Action
	 *
	 ------------------------------------------------------*/

	virtual void Execute(ArgList &varg) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Allow access to a typesafe variable argument
	 *        parameter list.
	 *
	 ------------------------------------------------------*/

	virtual void Execute() = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


 private:

	/* */
};


// If variable template arguments come to C++ later this code will change.  ;)
template <class Type, typename ReturnType = void> 
class MethodDelegateArg0 : public MethodDelegate
{
 public:

	typedef ReturnType (Type::*MethodPtr)( );

	MethodDelegateArg0(Type *obj, MethodPtr ptr) : 
		mObject(obj), mMethod(ptr) { }

	virtual ~MethodDelegateArg0() {}

	ReturnType Run() { return (mObject->*mMethod)(); }

	virtual void PushArg(void *ptr) { }

	virtual void Execute() { Run(); }

	virtual void Execute(ArgList &varg) { Run(); }

	Type *mObject;      /* The Object calling the method. */
	MethodPtr mMethod;  /* The method be called. */
};


template <typename Arg> 
class ArgList1 : public ArgList 
{
 public:
	ArgList1(Arg a) : mArg(a) {}
	virtual ~ArgList1() {}
	Arg mArg;
};

template <class Type, typename Arg, typename ReturnType = void> 
class MethodDelegateArg1 : public MethodDelegate
{
 public:

	typedef ReturnType (Type::*MethodPtr)( Arg );

	MethodDelegateArg1(Type *obj, MethodPtr ptr) :
		mObject(obj), mMethod(ptr) { }

	virtual ~MethodDelegateArg1() { }

	ReturnType Run(Arg arg)	{ return (mObject->*mMethod)(arg); }
	
	virtual void PushArg(void *ptr) { }

	virtual void Execute() { }

	virtual void Execute(ArgList &varg) 
	{
		ArgList1<Arg> *v = (ArgList1<Arg>*)&varg;
		Run(v->mArg); 
	}

	Type *mObject;      /* The Object calling the method. */
	MethodPtr mMethod;  /* The method be called. */
};


template <class Type, typename Arg, typename ReturnType = void> 
class MethodClosureArg1 : public MethodDelegate
{
public:

	typedef ReturnType (Type::*MethodPtr)( Arg );

	MethodClosureArg1(Type *obj, MethodPtr ptr, Arg a) :
		mObject(obj), mMethod(ptr), mStoredArgs(a) { }

	virtual ~MethodClosureArg1() { }

	ReturnType Run(Arg arg)	{ return (mObject->*mMethod)(arg); }
	
	virtual void PushArg(void *ptr) { }

	virtual void Execute() { return (mObject->*mMethod)(mStoredArgs.mArg); }

	virtual void Execute(ArgList &varg) 
	{
		ArgList1<Arg> *v = (ArgList1<Arg>*)&varg;
		Run(v->mArg); 
	}

	Type *mObject;             /* The Object calling the method. */
	MethodPtr mMethod;         /* The method be called. */
	ArgList1<Arg> mStoredArgs; /* Stored arguments for closure. */
};



template <typename Arg, typename Arg2> 
class ArgList2 : public ArgList 
{
 public:
	ArgList2(Arg a, Arg2 b) : mArg(a), mArg2(b) {}
	virtual ~ArgList2() {}
	Arg mArg;
	Arg2 mArg2;
};

template <class Type, typename Arg, typename Arg2, typename ReturnType = void> 
class MethodDelegateArg2 : public MethodDelegate
{
 public:

	typedef ReturnType (Type::*MethodPtr)( Arg, Arg2 );

	MethodDelegateArg2(Type *obj, MethodPtr ptr) :
		mObject(obj), mMethod(ptr) { }

	virtual ~MethodDelegateArg2() { }

	ReturnType Run(Arg arg, Arg2 arg2) 
	{ return (mObject->*mMethod)(arg, arg2); }
	
	virtual void PushArg(void *ptr) { }

	virtual void Execute() { }

	virtual void Execute(ArgList &varg) 
	{
		ArgList2<Arg, Arg2> *v = (ArgList2<Arg, Arg2>*)&varg;
		Run(v->mArg, v->mArg2); 
	}


	Type *mObject;      /* The Object calling the method. */
	MethodPtr mMethod;  /* The method be called. */
};


template <class Type, typename Arg, typename Arg2, typename ReturnType = void> 
class MethodClosureArg2 : public MethodDelegate
{
 public:

	typedef ReturnType (Type::*MethodPtr)( Arg, Arg2 );

	MethodClosureArg2(Type *obj, MethodPtr ptr, Arg a, Arg2 a2) :
		mObject(obj), mMethod(ptr), mStoredArgs(a, a2) { }

	virtual ~MethodClosureArg2() { }

	ReturnType Run(Arg arg, Arg2 arg2) 
	{ return (mObject->*mMethod)(arg, arg2); }
	
	virtual void PushArg(void *ptr) { }

	virtual void Execute() { return (mObject->*mMethod)(mStoredArgs.mArg); }

	virtual void Execute(ArgList &varg) 
	{
		ArgList2<Arg, Arg2> *v = (ArgList2<Arg, Arg2>*)&varg;
		Run(v->mArg, v->mArg2); 
	}

	Type *mObject;      /* The Object calling the method. */
	MethodPtr mMethod;  /* The method be called. */
	ArgList2<Arg, Arg2> mStoredArgs; /* Stored arguments for closure. */
};


// Opposed to the listening system for ResourceEvent, Delegate events
// are segreated due to their nature.  For example only certian managers
// have listeners for certain templates, where as Events must implement
// all possible listeners.  This is done for sanity and speed of execution.
// Also unlike ResourceEvent there is no two-way signaling.

// REMOVED


} // namespace mstl

#endif // GUARD__FREYJA_MONGOOSE_METHODDELEGATE_H_
