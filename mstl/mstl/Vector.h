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


#ifndef GUARD__FREYJA_MONGOOSE_VECTOR_H_
#define GUARD__FREYJA_MONGOOSE_VECTOR_H_

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#define VECTOR_BASE_EXPAND 16

namespace mstl {

template <class Object> class Vector
{
public:

	Vector() :
		mData(0x0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
	}


	Vector(const Vector &v) :
		mData(0x0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
		copy(v);
	}


	Vector(unsigned int size) :
		mData(0x0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
		resize(size);
	}


	Vector &operator=(const Vector<Object> &v)
	{
		mData = NULL;
		mReserve = v.mReserve;
		mStart = v.mStart;
		mEnd = v.mEnd;

		if ( v.mData != NULL && mReserve )
		{
#if 0
			mData = new Object[mReserve];
			memcpy(mData, v.mData, sizeof(Object) * mReserve);
#else // FIX for special class operator = usage
			mData = new Object[mReserve];

			for (unsigned int i = 0; i < mReserve; ++i)
			{
				mData[i] = v.mData[i];
			}
#endif
		}

		return *this;
	}


	~Vector()
	{
		clear();

		if (mData)
		{
			delete [] mData;
		}
		
		mReserve = 0;
	}

	////////////////////////////////////////////

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
		unsigned int i;


		if (!reserve(count))
		{
			for (i = 0; i < count; ++i)
			{
				if (i < begin() || i >= end())
				{
					mData[i] = obj;
				}
			}
		}

		mEnd = count;
	}


	void push_back()
	{
		pushBack();
	}
	
	
	void pushBack()
	{
		pushBack(0x0);
	}
	

	void push_back(Object object)
	{
		pushBack(object);
	}

	void pushBack(Object object)
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
		unsigned int i, count;

		//if (&v == 0x0)
		//	return;

		if (v.mReserve/*v.capacity()*/ > capacity())
		{
			resize(v.mReserve/*v.capacity()*/);
		}

		mStart = v.mStart;//v.begin();
		mEnd = v.mEnd;//v.end();

		for (i = mStart, count = mEnd; i < count; ++i)
		{
			mData[i] = v.mData[i];//v[i];
		}
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


#ifdef VECTOR_H_OLD_INTERNAL_ITERATOR
	// Built-in iterator methods ////////////////////////////////

 	unsigned int mIndex;

 	void start()
 	{
 		mIndex = begin();
 	}


 	void finish()
 	{
 		mIndex = end() - 1;
 	}


 	bool forward()
 	{
 		return (mIndex < end());
 	}


 	bool backward()
 	{
 		return (mIndex + 1 > begin());
 	}


 	void next()
 	{
 		++mIndex;
 	}


 	void prev()
 	{
 		--mIndex;
 	}


 	unsigned int currentIndex()
 	{
 			return mIndex;
 	}


 	Object current()
 	{
 			return mData[mIndex];
 	}

#endif



	// Instead of appending objects this apptempts replacing 'removed' objects
	unsigned int add(Object object)
	{
		if (begin() > 0)
		{
			mData[begin() - 1] = object;
			--mStart;

			return begin();
		}

		pushBack(object);
		return size();
	}


	void remove(unsigned int index)
	{
		if (index < end())
			mData[index] = 0x0;  // No, just invalidate it

		// Hhhmm... dangerous and fun - this gets your data out of order	
		//mData[index] = mData[begin()];
		//++mStart;
	}


	Object *get_array() /* Danger, Danger ;) */
	{
		return mData;
	}


	Object *getVectorArray() /* Danger, Danger ;) */
	{
		return mData;
	}

	bool start(unsigned int &idx) { idx = mStart; return (idx != mEnd); }
	bool next(unsigned int &idx) { ++idx; return (idx != mEnd); }
	Object &current(unsigned int idx) { return mData[idx]; }

private:

	Object *mData;

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
