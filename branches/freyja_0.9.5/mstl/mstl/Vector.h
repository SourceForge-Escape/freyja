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

		fNonClass = 1   // Mainly to avoid class data member operator = bugs

	} OptionFlags_t;


	Vector() :
		mData(0x0),
		mFlags(0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
	}


	Vector(const Vector &v) :
		mData(0x0),
		mFlags(0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
		copy(v);
	}


	Vector(unsigned int size) :
		mData(0x0),
		mFlags(0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
		resize(size);
	}


	Vector &operator=(const Vector<Object> &v)
	{
		// Mongoose, 20070426 - Don't trust Vector user to free data!
		if (mData) delete [] mData;

		mData = NULL;
		mFlags = v.mFlags;
		mReserve = v.mReserve;
		mStart = v.mStart;
		mEnd = v.mEnd;

		if ( v.mData != NULL && mReserve )
		{
			mData = new Object[mReserve];

			if (mFlags & fNonClass)
			{
				memcpy(mData, v.mData, sizeof(Object) * mReserve);
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


	~Vector()
	{
		clear();
		mReserve = 0;

		if (mData)
		{
			delete [] mData;
		}
	}

	////////////////////////////////////////////

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

	void Replace(Object oldObj, Object newObj)
	{
		unsigned int i;

		for (i = begin(); i< end(); ++i)
		{
			if (mData[i] == oldObj)
			{
				assign(i, newObj);
			}
		}
	}

	unsigned int SearchIndex(Object obj)
	{
		unsigned int i;

		for (i = begin(); i < end(); ++i)
		{
			if (mData[i] == obj)
			{
				return i;
			}
		}

		return UINT_MAX;
	}


	unsigned int GetErrorIndex() { return UINT_MAX; }


	long findFirstMatch(Object obj)
	{
		unsigned int i;

		for (i = begin(); i < end(); ++i)
		{
			if (mData[i] == obj)
			{
				return i;
			}
		}

		return -1;
	}

	////////////////////////////////////////////


	void clear()
	{
		mStart = 0;
		mEnd = 0;
	}

	void erase()
	{
		unsigned int i;


		for (i = begin(); i < end(); ++i)
		{
			if (mData[i])
			{
				delete mData[i];
			}
		}
		
		clear();
	}

	/* Returns false if reserve changes */
	bool reserve(unsigned int count)
	{
		Object *swap = 0x0;
		unsigned int i;

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
			mData = new Object[count + mExpand];
		}

		if (swap)
		{
			for (i = begin(); i < end(); ++i)
			{
				mData[i] = swap[i];
			}
			
			delete [] swap;
		}

		return (swap == 0x0);
	}


	void resize(unsigned int count)
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
		unsigned int i;

		if (!reserve(count))
		{
			for (i = 0; i < count; ++i)
			{
				if (i < begin() || i >= end())
				{
					mData[i] = Object();
				}
			}
		}

		mEnd = count;
#endif
	}


	void resize(unsigned int count, Object obj)//Object &obj = Object())
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


	void push_back() { push_back(0x0); }
	
	void push_back(Object object)
	{
		resize(size() + 1);
		mData[size()-1] = object;
	}


	bool empty()
	{
		return (begin() == end());
	}


	unsigned int capacity()
	{
		return mReserve;
	}
	

	unsigned int begin()
	{
		return mStart;
	}
	

	unsigned int end()
	{
		return mEnd;
	}


	unsigned int size()
	{
		return mEnd;
	}
	

	void copy(const Vector<Object> &v)
	{
#if 1 // Do full copies now...
		*this = v;
#else
		unsigned int i, count;

		if (v.mReserve > capacity())
		{
			resize(v.mReserve);
		}

		mFlags = v.mFlags;
		mStart = v.mStart;
		mEnd = v.mEnd;

		for (i = mStart, count = mEnd; i < count; ++i)
		{
			mData[i] = v.mData[i];
		}
#endif
	}


	void qSort(int (*compare_func)(const void *, const void *))
	{
		qsort(mData, end(), sizeof(Object), compare_func);
	}
	

	void swap(unsigned int a, unsigned int b)
	{
		if (a < begin() || a > end())
			return;
		
		if (b < begin() || b > end())
			return;

		Object swap = mData[a];
		mData[a] = mData[b];
		mData[b] = swap;
	}

	
	void assign(unsigned int index, Object object)
	{
		mData[index] = object;
	}


	void assign_all(Object object)
	{
		for (unsigned int i = mStart, count = mEnd; i < count; ++i)
		{
			mData[i] = object;
		}
	}



	/* Mongoose 2002.08.31, The burden of bounds checking is on you
	 *   this way you can handle your own problems w/o exceptions */
	Object &operator [] (unsigned int index)
	{
		return mData[index];
	}


	void print(void (*print_func)(Object))
	{
		unsigned int i;


		for (i = begin(); i < end(); ++i)
		{
			if (print_func)
			{
				(*print_func)(mData[i]);
			}

			fflush(stdout);
		}
		
		printf("\n");    
	}


	// Instead of appending objects this apptempts replacing 'removed' objects
	unsigned int add(Object object)
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


	// Deprecated API wrappers...
	void pushBack() { push_back(); }
	
	void pushBack(Object object) { push_back(object); }

	Object *getVectorArray() { return get_array(); }


private:

	Object *mData;

	unsigned int mFlags;
	unsigned int mReserve;
	unsigned int mStart;
	unsigned int mEnd;
	unsigned int mExpand;
};

// Thanks to Rajiv Bhagwat for foreach macro idea, didn't want to use internal iterators however
#define F_E_JOIN2(a,b) a##b
#define F_E_JOIN(a,b) F_E_JOIN2(a,b)
#define foreach(v, idx) for(bool F_E_JOIN(flag,__LINE__)=v.start(idx); F_E_JOIN(flag,__LINE__);F_E_JOIN(flag,__LINE__)=v.next(idx)) 


} // namespace mstl

#endif
