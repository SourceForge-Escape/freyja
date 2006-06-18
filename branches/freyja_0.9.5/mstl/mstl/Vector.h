/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Vector
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: mtk template vector
 *
 *           UINT_MAX is an error condition, used in place of -1
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
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

template <class Object> class Vector
{
public:

	Vector()
	{
		mData = 0x0;

		mReserve = 0;
		mStart = 0;
		mEnd = 0;
		mExpand = VECTOR_BASE_EXPAND;
	}


	Vector(Vector &vector)
	{
		mData = 0x0;

		mReserve = 0;
		mStart = 0;
		mEnd = 0;
		mExpand = VECTOR_BASE_EXPAND;

		copy(vector);
	}


	Vector(unsigned int size)
	{
		mData = 0x0;

		mReserve = 0;
		mStart = 0;
		mEnd = 0;
		mExpand = VECTOR_BASE_EXPAND;

		resize(size);
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
		resize(count, 0x0);
	}


	void resize(unsigned int count, Object object)
	{
		unsigned int i;


		if (!reserve(count))
		{
			for (i = 0; i < count; ++i)
			{
				if (i < begin() || i >= end())
				{
					mData[i] = object;
				}
			}
		}

		mEnd = count;
	}


	void pushBack()
	{
		pushBack(0x0);
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
	

	void copy(Vector<Object> &vector)
	{
		unsigned int i;


		if (vector.capacity() > capacity())
		{
			resize(vector.capacity());
		}

		mStart = vector.begin();
		mEnd = vector.end();

		for (i = vector.begin(); i < vector.end(); ++i)
		{
			mData[i] = vector[i];
			// Add(list->Current());
		}
	}

	
	void assign(unsigned int index, Object object)
	{
		mData[index] = object;
	}


	/* Mongoose 2002.08.31, The burden of bounds checking is on you
	 *   this way you can handle your own problems w/o exceptions */
	Object operator [] (unsigned int index)
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

	Object *getVectorArray() /* Danger, Danger ;) */
	{
		return mData;
	}
	

private:

	Object *mData;

	unsigned int mReserve;
	unsigned int mStart;
	unsigned int mEnd;
	unsigned int mExpand;
};




#define VectorIterator_BEGIN(v) for (v.start(); v.forward(); v.end()) {
#define VectorIterator_END }

template <class Object> class VectorIterator
{
public:

	VectorIterator(Vector<Object> *vector)
	{
		mVector = vector;
		mIndex = 0;
	}

	void start()
	{
		mIndex = mVector->begin();
	}


	void finish()
	{
		mIndex = mVector->end() - 1;
	}


	bool forward()
	{
		return (mIndex < mVector->end());
	}


	bool backward()
	{
		return (mIndex + 1 > mVector->begin());
	}


	void next()
	{
		++mIndex;
	}

	void prev()
	{
		--mIndex;
	}

	unsigned int index()
	{
		return mIndex;
	}

	Object current()
	{
		return (*mVector)[mIndex];
	}


	Vector<Object> *mVector;
	unsigned int mIndex;
};


#endif
