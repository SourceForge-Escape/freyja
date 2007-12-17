/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Vector
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: mtk template vector
 *
 *           UINT_MAX is an error condition, used in place of -1
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.07.01:
 * Mongoose - ISO C++98 update
 *
 * 2002.08.31:
 * Mongoose - Created
 ================================================================*/

#ifndef GUARD__MSTL_VECTOR_H_
#define GUARD__MSTL_VECTOR_H_

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#define VECTOR_BASE_EXPAND 16

namespace mstl {

template <class Object> class Vector
{
public:

	typedef enum {

		fNonClass = 1,   // Set this to used optimize non-class arrays.
		fSorted   = 2

	} OptionFlags_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Vector();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Contstructor.
	 *
	 ------------------------------------------------------*/

	Vector(const Vector& v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy contstructor.
	 *
	 ------------------------------------------------------*/

	Vector(const unsigned int size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Contstructor with initial array size.
	 *
	 ------------------------------------------------------*/

	~Vector();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////

	Vector &operator=(const Vector<Object>& v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assignment operator.
	 *
	 ------------------------------------------------------*/

	Object& operator [] (const unsigned int idx) const;
	/*------------------------------------------------------
	 * Pre  : Caller does any required bounds checking.
	 * Post : Returns a reference to <idx>th array element. 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Acessors
	////////////////////////////////////////////////////////////

	unsigned int begin() const
	{ return mStart; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	unsigned int capacity() const
	{ return mReserve; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool empty() const
	{ return (begin() == end()); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void for_each(void (*func)(Object)) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : func is called with each element as a parameter. 
	 *
	 ------------------------------------------------------*/

	unsigned int end() const
	{ return mEnd; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	unsigned int size() const
	{ return mEnd;	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static const int npos = -1;


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void ClearFlag(OptionFlags_t f) { mFlags &= ~f;	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears an option flag.
	 *
	 ------------------------------------------------------*/

	void SetFlag(OptionFlags_t f)	{ mFlags |= f;	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets an option flag.
	 *
	 ------------------------------------------------------*/

	void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void copy(const Vector<Object>& v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make this Vector a copy of v.
	 *
	 ------------------------------------------------------*/

	void erase();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void push_back() 
	{ push_back(0x0); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	void push_back(Object object);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool reserve(const unsigned int count);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns false if array changes.
	 *
	 ------------------------------------------------------*/

	void resize(unsigned int count);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void resize(unsigned int count, Object obj);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void qSort(int (*compare_func)(const void*, const void*));
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/	

	void swap(unsigned int a, unsigned int b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	void assign(unsigned int index, Object object);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void assign_all(Object object);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void remove(unsigned int idx) { if (idx < end()) mData[idx] = 0x0; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Marks <idx> NULL for vectors of pointers.
	 *
	 ------------------------------------------------------*/

	Object *get_array() { return mData; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Direct array access, returns array for this vector.
	 *
	 ------------------------------------------------------*/

	bool start(unsigned int &idx) { idx = mStart; return (idx != mEnd); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : foreach() iterator use -- reset iter.
	 *
	 ------------------------------------------------------*/

	bool next(unsigned int &idx) { ++idx; return (idx != mEnd); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : foreach() iterator use -- next iter.
	 *
	 ------------------------------------------------------*/

	Object &current(unsigned int idx) { return mData[idx]; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : foreach() iterator use -- array index.
	 *
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Deprecated API
	////////////////////////////////////////////////////////////

	long findFirstMatch(Object obj) const;
	/*------------------------------------------------------
	 * Pre  : DEPRECATED API
	 * Post : Returns index to first obj or -1 on error.
	 *
	 ------------------------------------------------------*/

	unsigned int GetErrorIndex() const;
	/*------------------------------------------------------
	 * Pre  : DEPRECATED API
	 * Post : 
	 *
	 ------------------------------------------------------*/

	unsigned int SearchIndex(Object obj) const;
	/*------------------------------------------------------
	 * Pre  : DEPRECATED API
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Replace(Object oldObj, Object newObj);
	/*------------------------------------------------------
	 * Pre  : DEPRECATED API
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void print(void (*print_func)(Object));
	/*------------------------------------------------------
	 * Pre  : DEPRECATED API
	 * Post : Uses passed print function on each element. 
	 *
	 ------------------------------------------------------*/

	unsigned int add(Object object);
	/*------------------------------------------------------
	 * Pre  : DEPRECATED API
	 * Post : This replaces 'removed' objects with new object.
	 *
	 ------------------------------------------------------*/

private:

	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Object *mData;

	unsigned char mFlags;
	unsigned int mReserve;
	unsigned int mStart;
	unsigned int mEnd;
	unsigned int mExpand;
};



////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////


template <typename T>
Vector<T>::Vector() :
	mData(0x0),
	mFlags(0),
	mReserve(0),
	mStart(0),
	mEnd(0),
	mExpand(VECTOR_BASE_EXPAND)
{ }


template <typename T>
Vector<T>::Vector(const Vector<T>& v) :
	mData(0x0),
	mFlags(0),
	mReserve(0),
	mStart(0),
	mEnd(0),
	mExpand(VECTOR_BASE_EXPAND)
{
	copy(v);
}


template <typename T>
Vector<T>::Vector(const unsigned int size) :
	mData(0x0),
	mFlags(0),
	mReserve(0),
	mStart(0),
	mEnd(0),
	mExpand(VECTOR_BASE_EXPAND)
{
	resize(size);
}


template <typename T>
Vector<T>::~Vector()
{
	clear();
	mReserve = 0;
	
	if (mData)
	{
		delete [] mData;
	}
}


////////////////////////////////////////////////////////////
// Operators
////////////////////////////////////////////////////////////

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &v)
{
	// Mongoose, 20070426 - Don't trust Vector user to free data!
	if (mData) 
		delete [] mData;

	mData = NULL;
	mFlags = v.mFlags;
	mReserve = v.mReserve;
	mStart = v.mStart;
	mEnd = v.mEnd;

	if ( v.mData != NULL && mReserve )
	{
		mData = new T[mReserve];
		
		if ( mFlags & fNonClass )
		{
			memcpy(mData, v.mData, sizeof(T) * mReserve);
		}
		else // FIX for special class operator = usage ( default )
		{
			for (unsigned int i = 0; i < mReserve; ++i)
			{
				mData[i] = v.mData[i];
			}
		}
	}
	
	return *this;
}


template <typename T>
T& Vector<T>::operator [] (const unsigned int idx) const
{
	return mData[idx];
}


////////////////////////////////////////////////////////////
// Public Acessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

template <typename T>
void Vector<T>::Replace(T oldObj, T newObj)
{
	for (unsigned int i = begin(); i < end(); ++i)
	{
		if (mData[i] == oldObj)
		{
			assign(i, newObj);
		}
	}
}


template <typename T>
unsigned int Vector<T>::SearchIndex(T obj) const
{
	for (unsigned int i = begin(); i < end(); ++i)
	{
		if (mData[i] == obj)
		{
			return i;
		}
	}

	return UINT_MAX;
}


template <typename T>
unsigned int Vector<T>::GetErrorIndex() const
{ 
	return UINT_MAX; 
}


template <typename T>
long Vector<T>::findFirstMatch(T obj) const
{
	for (unsigned int i = begin(); i < end(); ++i)
	{
		if (mData[i] == obj)
		{
			return i;
		}
	}

	return -1;
}


template <typename T>
void Vector<T>::clear()
{
	mStart = 0;
	mEnd = 0;
}


template <typename T>
void Vector<T>::erase()
{
	for (unsigned int i = begin(); i < end(); ++i)
	{
		if (mData[i])
		{
			delete mData[i];
		}
	}
		
	clear();
}


template <typename T>
bool Vector<T>::reserve(const unsigned int count)
{
	T* swap = 0x0;
	
	if (count > mReserve)
	{
		// Yeah it's stupid, but I don't trust pure scaling
		if (count + mReserve > 100)
			mExpand += 10;
		
		if (count + mReserve > 500)
			mExpand += 100;
		
		if (count + mReserve > 7000)
			mExpand += 1000;
		
		swap = mData;
		mReserve = count + mExpand;
		mData = new T[count + mExpand];
	}
	
	if (swap)
	{
		for (unsigned int i = begin(); i < end(); ++i)
		{
			mData[i] = swap[i];
		}
		
		delete [] swap;
	}
	
	return (swap == 0x0);
}


template <typename T>
void Vector<T>::resize(unsigned int count)
{
	if (!count)
	{
		/* I don't do deallocation here, call erase */
		mStart = 0;
		mEnd = 0;
		return;
	}

#if OLD_VECTOR_PTR_HELPER
	resize(count, 0x0);
#else

	if (!reserve(count))
	{
		for (unsigned int i = 0; i < count; ++i)
		{
			if (i < begin() || i >= end())
			{
				mData[i] = T();
			}
		}
	}

	mEnd = count;
#endif
}


template <typename T>
void Vector<T>::resize(unsigned int count, T obj)
{
	if (!reserve(count))
	{
		for (unsigned int i = 0; i < count; ++i)
		{
			if (i < begin() || i >= end())
			{
				mData[i] = obj;
			}
		}
	}

	mEnd = count;
}


template <typename T>
void Vector<T>::push_back(T object)
{
	resize(size() + 1);
	mData[size()-1] = object;
}


template <typename T>
void Vector<T>::copy(const Vector<T>& v)
{
	*this = v;
}


template <typename T>
void Vector<T>::qSort(int (*compare_func)(const void*, const void*))
{
	qsort(mData, end(), sizeof(T), compare_func);
}
	

template <typename T>
void Vector<T>::swap(unsigned int a, unsigned int b)
{
#if 0
	// Bounds checking.
	if ( a < begin() || a > end() )
		return;
	
	if ( b < begin() || b > end() )
		return;
#endif

	T swap = mData[a];
	mData[a] = mData[b];
	mData[b] = swap;
}

	
template <typename T>
void Vector<T>::assign(unsigned int idx, T object)
{
	mData[idx] = object;
}


template <typename T>
void Vector<T>::assign_all(T object)
{
	for (unsigned int i = begin(); i < mEnd; ++i)
	{
		mData[i] = object;
	}
}


template <typename T>
void Vector<T>::print(void (*print_func)(T))
{
	if ( print_func )
	{
		for (unsigned int i = begin(); i < end(); ++i)
		{
			(*print_func)(mData[i]);
			fflush(stdout);
		}
	
		printf("\n");    
	}
}


template <typename T>
void Vector<T>::for_each(void (*func)(T)) const
{
	if ( func )
	{
		for (unsigned int i = begin(); i < end(); ++i)
		{
			(*func)(mData[i]);
		}
	}
}


template <typename T>
unsigned int Vector<T>::add(T object)
{
	if (begin() > 0)
	{
		mData[begin() - 1] = object;
		--mStart;

		return begin();
	}

	push_back(object);
	return size();
}


////////////////////////////////////////////////////////////
// foreach macro
//
// Thanks to Rajiv Bhagwat for foreach macro idea, but 
// I use a different implementation for my needs.
////////////////////////////////////////////////////////////

#define F_E_JOIN2(a,b) a##b
#define F_E_JOIN(a,b) F_E_JOIN2(a,b)
#define foreach(v, idx) for(bool F_E_JOIN(flag,__LINE__)=v.start(idx); F_E_JOIN(flag,__LINE__);F_E_JOIN(flag,__LINE__)=v.next(idx)) 


} // namespace mstl

#endif // GUARD__MSTL_VECTOR_H_
