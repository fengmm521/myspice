#ifndef ngspice_MEMORY_H
#define ngspice_MEMORY_H

#include <stddef.h>

#define TMALLOC(t, n)       (t*) tmalloc(sizeof(t) * (size_t)(n))
#define TREALLOC(t, p, n)   (t*) trealloc(p, sizeof(t) * (size_t)(n))


#ifndef HAVE_LIBGC

extern void *tmalloc(size_t num);
extern void *trealloc(void *str, size_t num);
extern void txfree(void *ptr);

#define tfree(x) (txfree(x), (x) = 0)

#else

#include <gc/gc.h>

#define tmalloc(m)      GC_malloc(m)
#define trealloc(m, n)  GC_realloc((m), (n))
#define tfree(m)
#define txfree(m)

#endif /* HAVE_LIBGC */


#include "ngspice/stringutil.h" /* va: spice3 internally bzero */

#define alloc(TYPE)      TMALLOC(TYPE, 1)
#define MALLOC(x)        tmalloc((size_t) (x))
#define FREE(x)          do { if(x) { txfree(x); (x) = NULL; } } while(0)
#define REALLOC(x, y)    trealloc(x, (size_t) (y))
#define ZERO(PTR, TYPE)  bzero(PTR, sizeof(TYPE))


#if defined(_MSC_VER) || defined(__MINGW32__)
void *hrealloc(void *ptr, size_t num);
#endif


#ifdef CIDER

#define RALLOC(ptr, type, number)                                       \
    do {                                                                \
        if ((number) && (ptr = (type *)calloc((size_t)(number), sizeof(type))) == NULL) \
            return E_NOMEM;                                             \
    } while(0)

#define XALLOC(ptr, type, number)                                       \
    do {                                                                \
        if ((number) && (ptr = (type *)calloc((size_t)(number), sizeof(type))) == NULL) { \
            SPfrontEnd->IFerrorf(E_PANIC, "Out of Memory");             \
            exit(1);                                                    \
        }                                                               \
    } while(0)

#define XCALLOC(ptr, type, number)                                      \
    do {                                                                \
        if ((number) && (ptr = (type *)calloc((size_t)(number), sizeof(type))) == NULL) { \
            fprintf(stderr, "Out of Memory\n");                         \
            exit(1);                                                    \
        }                                                               \
    } while(0)

#endif /* CIDER */

#endif
