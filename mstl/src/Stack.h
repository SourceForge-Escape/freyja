/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * Project: Freyja
 * Author : Terry 'Mongoose' Hendrix II
 * Website: http://www.westga.edu/~stu7440/
 * Email  : stu7440@westga.edu
 * Object : Stack
 * Comment: 
 *
 *-- History -----------------------------------------------
 *
 * 2004.04.15:
 * Mongoose - No more leading underscores, style changes
 *
 * 2001.05.12:
 * Mongoose - Created, from my old Stack template class
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_FREYJA_STACK_H
#define GUARD__MONGOOSE_FREYJA_STACK_H

#include <stdio.h>
#include <stdlib.h>

 
template <class T> class StackNode
{
 public:

  StackNode(T data)
  {
    mData  = data;
    mPrev = NULL;
  }


  ~StackNode()
  {
  }

  void Data(T data)
  {
    mData = data;
  }

  T Data()
  {
    return mData;
  }

  StackNode<T> *Prev()
  {
    return mPrev;
  }

  void Prev(StackNode<T> *prev)
  {
    mPrev = prev;
  }

 private:

  StackNode<T> *mPrev;

  T mData;
};



template <class T> class Stack
{
public:

   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : Nothing
    * Pre      : None
    * Post     : Constructs an Stack
    -----------------------------------------*/
   Stack()
   {
     mCount = 0;
     mTop = NULL;
   }


   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : Nothing
    * Pre      : None
    * Post     : Deconstructs an Stack
    -----------------------------------------*/
   ~Stack()
   {
     while (mTop)
       pop();
   }


   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : value of top stack element
    * Pre      : stack isn't empty
    * Post     : Returns top of stack's value
    -----------------------------------------*/
   T peek()
   {
     if (mTop)
     {
       return mTop->Data();
     }
     else
     {
       fprintf(stderr, "Stack<T>::Peek> NULL mTop!\n");     
       return (T)0;
     }
   }


   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : value of top stack element
    * Pre      : stack isn't empty
    * Post     : Removes top of stack and 
    *            returns it's value
    -----------------------------------------*/
   T pop()
   {
     StackNode<T> *rm;
     T data;


     if (mTop)
     {
       rm = mTop;

       data = mTop->Data();
       mTop = mTop->Prev();

       delete rm;
       mCount--;

       return data;
     }
     else
     {
       fprintf(stderr, "Stack<T>::Pop> NULL mTop!\n");
       return (T)0;
     }
   }


   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : Value to push on top of stack
    * Returns  : Nothing
    * Pre      : Stack isn't full
    * Post     : Stack has new top, with value i
    -----------------------------------------*/
   void push(T data)
   {
     StackNode<T> *node;


     node = new StackNode<T>(data);
     node->Prev(mTop);

     mTop = node;
     mCount++;
   }


   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : true if stack is empty, else
    *            returns false
    * Pre      : None
    * Post     : No side effect
    -----------------------------------------*/
	bool empty()
	{
		return (mCount && mTop);
	}
 

 private:

	unsigned int mCount;

	StackNode<T> *mTop;
};

#endif




