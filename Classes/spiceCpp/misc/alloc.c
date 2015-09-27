/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
**********/

/* for thread handling */
#if defined __MINGW32__ || defined _MSC_VER
#include <windows.h>
#endif

/*
 * Memory alloction functions
 */
#include "ngspice/ngspice.h"


#ifdef SHARED_MODULE
#ifndef HAVE_LIBPTHREAD
#ifdef SRW
#define mutex_lock(a) AcquireSRWLockExclusive(a)
#define mutex_unlock(a) ReleaseSRWLockExclusive(a)
typedef SRWLOCK mutexType;
#else
#define mutex_lock(a) EnterCriticalSection(a)
#define mutex_unlock(a) LeaveCriticalSection(a)
typedef CRITICAL_SECTION mutexType;
#endif
extern mutexType allocMutex;
#else
#include <pthread.h>
#define mutex_lock(a) pthread_mutex_lock(a)
#define mutex_unlock(a) pthread_mutex_unlock(a)
typedef pthread_mutex_t mutexType;
extern mutexType allocMutex;
#endif
#endif


#ifndef HAVE_LIBGC

/*saj For Tcl module locking*/
#ifdef TCL_MODULE
#include <tcl.h>
#endif

/* Malloc num bytes and initialize to zero. Fatal error if the space can't
 * be tmalloc'd.   Return NULL for a request for 0 bytes.
 */

/* New implementation of tmalloc, it uses calloc and does not call bzero()  */

void *
tmalloc(size_t num)
{
  void *s;
/*saj*/
#ifdef TCL_MODULE
  Tcl_Mutex *alloc;
  alloc = Tcl_GetAllocMutex();
#endif
    if (!num)
      return NULL;
/*saj*/
#ifdef TCL_MODULE
  Tcl_MutexLock(alloc);
#elif defined SHARED_MODULE
  mutex_lock(&allocMutex);
#endif
    s = calloc(num,1);
/*saj*/
#ifdef TCL_MODULE
  Tcl_MutexUnlock(alloc);
#elif defined SHARED_MODULE
  mutex_unlock(&allocMutex);
#endif
    if (!s){
      fprintf(stderr,"malloc: Internal Error: can't allocate %ld bytes. \n",(long)num);
#if defined HAS_WINGUI || defined SHARED_MODULE
      controlled_exit(EXIT_FAILURE);
#else
      exit(EXIT_FAILURE);
#endif
    }
    return(s);
}


void *
trealloc(void *ptr, size_t num)
{
  void *s;
/*saj*/
#ifdef TCL_MODULE
  Tcl_Mutex *alloc;
  alloc = Tcl_GetAllocMutex();
#endif
  if (!num) {
    if (ptr)
      free(ptr);
    return NULL;
  }

  if (!ptr)
    s = tmalloc(num);
  else {
/*saj*/
#ifdef TCL_MODULE
    Tcl_MutexLock(alloc);
#elif defined SHARED_MODULE
  mutex_lock(&allocMutex);
#endif
    s = realloc(ptr, num);
/*saj*/
#ifdef TCL_MODULE
  Tcl_MutexUnlock(alloc);
#elif defined SHARED_MODULE
  mutex_unlock(&allocMutex);
#endif
  }
  if (!s) {
    fprintf(stderr,"realloc: Internal Error: can't allocate %ld bytes.\n", (long)num);
#if defined HAS_WINGUI || defined SHARED_MODULE
      controlled_exit(EXIT_FAILURE);
#else
      exit(EXIT_FAILURE);
#endif
  }
  return(s);
}


void
txfree(void *ptr)
{
/*saj*/
#ifdef TCL_MODULE
  Tcl_Mutex *alloc;
  alloc = Tcl_GetAllocMutex();
  Tcl_MutexLock(alloc);
#endif
#ifdef SHARED_MODULE
  mutex_lock(&allocMutex);
#endif
	if (ptr)
		free(ptr);
/*saj*/
#ifdef TCL_MODULE
  Tcl_MutexUnlock(alloc);
#elif defined SHARED_MODULE
  mutex_unlock(&allocMutex);
#endif
}

#endif
