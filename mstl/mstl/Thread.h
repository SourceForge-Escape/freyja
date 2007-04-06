/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry Hendrix ( Mongoose )
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Thread abstraction
 * License : No use w/o permission (C) 2001-2006 Mongoose
 * Comments: Lots of nice utils from libfreyja moved to mstl
 *           aids system abstraction and debugging.
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.07.23:
 * Mongoose - Barrowed from my libfreyja, and then made more generic
 ================================================================*/

#ifndef GUARD__MSTL_THREAD_H_
#define GUARD__MSTL_THREAD_H_

#include <pthread.h>
#include <mstl/String.h>

namespace mstl {

// Renamed from CallbackBase after my favorite C# feature  ;)
class Delegate
{
 public:

	Delegate() {}

	virtual ~Delegate() {}

	virtual void Execute() = 0;
};


// If there is a nice way to var template arguments later rewrite this trash
template <class Type, typename ReturnType = void> 
class Callback : public Delegate
{
 public:

	// Example usage:
	// Callback<YourClass, void> cb = Callback(this, &YourClass::Method);
	// cb.Run();

	typedef ReturnType (Type::*ClassFuncPtr)( );


	Callback(Type *obj, ClassFuncPtr cb) : mObject(obj), mMethod(cb) { }

	virtual ~Callback() { }

	void Execute() { (mObject->*mMethod)(); }

	ReturnType Run() { return (mObject->*mMethod)(); }

	
	Type *mObject;
	ClassFuncPtr mMethod;
};


template <class Type, typename Arg, typename ReturnType = void> 
class CallbackArg : public Delegate
{
 public:

	// Example usage:
	// Callback<YourClass, int, void> cb = Callback(this, &YourClass::Method);
	// cb.Run(0);

	typedef ReturnType (Type::*ClassFuncPtr)( Arg );

	CallbackArg(Type *obj, ClassFuncPtr cb) : mObject(obj), mMethod(cb) { }

	CallbackArg(Type *obj, ClassFuncPtr cb, Arg a) : 
		mObject(obj), mMethod(cb), mArg(a) { }

	virtual ~CallbackArg() { }

	ReturnType Run(Arg arg)	{ return (mObject->*mMethod)(arg); }

	void Execute() { (mObject->*mMethod)(mArg); }
	
	Type *mObject;
	ClassFuncPtr mMethod;
	Arg mArg;
};


template <class Type, typename Arg, typename Arg2, typename ReturnType = void> 
class CallbackArg2 : public Delegate
{
 public:

	// Example usage:
	// Callback<YourClass, int, void> cb = Callback(this, &YourClass::Method);
	// cb.Run(0);

	typedef ReturnType (Type::*ClassFuncPtr)( Arg, Arg2 );

	CallbackArg2(Type *obj, ClassFuncPtr cb) : mObject(obj), mMethod(cb) { }

	CallbackArg2(Type *obj, ClassFuncPtr cb, Arg a, Arg2 a2) : 
		mObject(obj), mMethod(cb), mArg(a), mArg2(a2) { }

	virtual ~CallbackArg2() { }

	ReturnType Run(Arg arg, Arg2 arg2) { return (mObject->*mMethod)(arg, arg2); }

	void Execute() { (mObject->*mMethod)(mArg, mArg2); }
	
	Type *mObject;
	ClassFuncPtr mMethod;
	Arg mArg;
	Arg2 mArg2;
};

template <class Type, typename Arg, typename Arg2, typename Arg3, typename ReturnType = void> 
class CallbackArg3 : public Delegate
{
 public:

	typedef ReturnType (Type::*ClassFuncPtr)( Arg, Arg2, Arg3 );

	CallbackArg3(Type *obj, ClassFuncPtr cb) : mObject(obj), mMethod(cb) { }

	CallbackArg3(Type *obj, ClassFuncPtr cb, Arg a, Arg2 a2, Arg3 a3) : 
		mObject(obj), mMethod(cb), mArg(a), mArg2(a2), mArg3(a3) { }

	virtual ~CallbackArg3() { }

	ReturnType Run(Arg arg, Arg2 arg2, Arg3 arg3) { return (mObject->*mMethod)(arg, arg2, arg3); }

	void Execute() { (mObject->*mMethod)(mArg, mArg2, mArg3); }
	
	Type *mObject;
	ClassFuncPtr mMethod;
	Arg mArg;
	Arg2 mArg2;
	Arg3 mArg3;
};


// Only supporting joins for now, until the delegate API is finished you SHOULD NOT USE THIS CLASS besides in simple tests like for the 10M vertex test 
// mutex, cond, etc will all be added in time -- chill
// Yes, supporting named threads
class Thread
{
public:

	Thread() : mThread(), mAttr(NULL), mName("") {}

	// Please don't actually use copy constructors and = operator, be smart
	Thread(const Thread &t) : mThread(t.mThread), mAttr(t.mAttr), mName(t.mName) {}

	Thread &operator =(const Thread &t) 
	{ 
		mThread = t.mThread;
		mAttr = t.mAttr;
		mName = t.mName;
		return *this;
	}


	virtual ~Thread() {}

	int Create(void *(*func)(void *), void *arg)
	{
		int ret = pthread_create( &mThread, mAttr, func, arg );
		// Might want to encode status into generic enum when made portable
		return ret;		
	}


	int Create(Delegate *d)
	{
		void *ptr = (void *)d;
		int ret = pthread_create( &mThread, mAttr, Thread::MethodCallback, ptr);
		// Might want to encode status into generic enum when made portable
		return ret;
	}

	
	static void *MethodCallback(void *obj)
	{
		Delegate *d = (Delegate*)obj;
		d->Execute();
		return NULL;
	}


	void Suspend() {}

	void Resume() {}

	void Kill() {}

	bool IsAlive() { return false; }

	bool IsBackground() { return false; }

	void Sleep(long t) {}

	void SpinWait(long t) {}

	void SetPriority(long t) {}

	long GetPriority() { return 0; }

	String &GetName() { return mName; }

	void Join() 
	{
		pthread_join( mThread, NULL );
	}

	pthread_t mThread;

	pthread_attr_t *mAttr;

	mstl::String mName;
};


class DelegateThread : public Thread
{
public:

	DelegateThread(Delegate *d) : mDelegate(d) {}

	DelegateThread(const DelegateThread &t) : mDelegate(t.mDelegate) {}

	DelegateThread &operator =(const DelegateThread &t) 
	{ 
		mDelegate = t.mDelegate;
		return *this;
	}

	DelegateThread() : mDelegate(NULL) {}

	virtual ~DelegateThread() { if (mDelegate) delete mDelegate; }

	int Create() { return Thread::Create(mDelegate); }

	void SetDelegate(Delegate *d) { mDelegate = d; }

	Delegate *mDelegate;
};


} // namespace mstl

#endif // GUARD__MSTL_THREAD_H_
