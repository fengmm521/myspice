#ifndef _LOCK_
#define _LOCK_
#include "cocos2d.h"
#include <thread>

//enum
//{
//    PTHREAD_MUTEX_TIMED_NP,
//    PTHREAD_MUTEX_RECURSIVE_NP,
//    PTHREAD_MUTEX_ERRORCHECK_NP,
//    PTHREAD_MUTEX_ADAPTIVE_NP
//};


class CCriticalSection
{
private:
    std::mutex m_mutex1;

public:
	CCriticalSection()
	{
		Init();
	}
	
	void Init()
	{
        
	}

	~CCriticalSection()
	{
		
	}

	void Lock()
	{
		m_mutex1.lock();
	}

	void Unlock()
	{
		m_mutex1.unlock();
	}
};

class CEvent
{
private:
    std::mutex m_mutex;
    std::condition_variable m_SleepCondition;
public:
    CEvent()
    {
        Init();
    }
    
    void Init()
    {
        
        
    }
    ~CEvent()
    {

    }
    
    void Lock()
    {
        m_mutex.lock();
    }
    
    void Unlock()
    {
        m_mutex.unlock();
    }
    
    void Wait()
    {
        std::unique_lock<std::mutex> lk(m_mutex,std::adopt_lock);
        m_SleepCondition.wait(lk);
    }
    
    void Post()
    {
        m_SleepCondition.notify_one();
    }
};

#endif

