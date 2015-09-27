#pragma once

#include "Lock.h"
/////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class CQueue
{
private:
	CCriticalSection cs;
	
	T * first;
	T * last;
	
public:
	CQueue()
	{
		Init();
	}
	
	~CQueue()
	{
	}
	
	void Init()
	{
		first = NULL;
		last = NULL;
	}
	
	bool IsEmpty()
	{
		bool ret;
		cs.Lock();
		ret = (first == NULL);
		cs.Unlock();
		return ret;
	}
	
	void Push(T * t)
	{
		t->next = NULL;
		
		cs.Lock();
		if(first)
		{
			last->next = t;
		}
		else
		{
			first = t;
		}
		last = t;
		cs.Unlock();
	}
	
	T* Pop()
	{
		T * ret;
		
		cs.Lock();
		if(first)
		{
			ret = first;
			
			first = first->next;
			
			if(!first)
			{
				last = NULL;
			}
		}
		else
		{
			ret = NULL;
		}
		cs.Unlock();
		
		return ret;
	}
};