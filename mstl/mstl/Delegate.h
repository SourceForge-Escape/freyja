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

class MethodDelegate
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

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


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	//virtual bool Init() = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	//virtual void Execute() = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


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

	Type *mObject;      /* The Object calling the method. */
	MethodPtr mMethod;  /* The method be called. */
};


template <class Type, typename Arg, typename ReturnType = void> 
class MethodDelegateArg1 : public Delegate
{
 public:

	typedef ReturnType (Type::*MethodPtr)( Arg );

	MethodDelegateArg1(Type *obj, MethodPtr ptr) :
		mObject(obj), mMethod(ptr) { }

	virtual ~MethodDelegateArg1() { }

	ReturnType Run(Arg arg)	{ return (mObject->*mMethod)(arg); }
	
	Type *mObject;      /* The Object calling the method. */
	MethodPtr mMethod;  /* The method be called. */
};


template <class Type, typename Arg, typename Arg2, typename ReturnType = void> 
class MethodDelegateArg2 : public Delegate
{
 public:

	typedef ReturnType (Type::*MethodPtr)( Arg, Arg2 );

	MethodDelegateArg2(Type *obj, MethodPtr ptr) :
		mObject(obj), mMethod(ptr) { }

	virtual ~MethodDelegateArg2() { }

	ReturnType Run(Arg arg, Arg2 arg2) 
	{ return (mObject->*mMethod)(arg, arg2); }
	
	Type *mObject;      /* The Object calling the method. */
	MethodPtr mMethod;  /* The method be called. */
};

} // namespace mstl

#endif // GUARD__FREYJA_MONGOOSE_METHODDELEGATE_H_
