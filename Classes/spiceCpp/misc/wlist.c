/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/* Wordlist manipulation stuff.  */

#include "ngspice/ngspice.h"
#include "ngspice/bool.h"
#include "ngspice/wordlist.h"


/* Determine the length of a word list. */
int
wl_length(const wordlist *wl)
{
    int i = 0;

    for (; wl; wl = wl->wl_next)
        i++;

    return (i);
}


/* Free the storage used by a word list. */
void
wl_free(wordlist *wl)
{
    while (wl) {
        wordlist *next = wl->wl_next;
        tfree(wl->wl_word);
        tfree(wl);
        wl = next;
    }
}


/* Copy a wordlist and the words. */
wordlist *
wl_copy(wordlist *wl)
{
    wordlist *first = NULL, *last = NULL;

    for (; wl; wl = wl->wl_next)
        wl_append_word(&first, &last, copy(wl->wl_word));

    return (first);
}


/* Substitute a wordlist for one element of a wordlist, and return a
 * pointer to the last element of the inserted list.  */
wordlist *
wl_splice(wordlist *elt, wordlist *list)
{

    if (list)
        list->wl_prev = elt->wl_prev;
    if (elt->wl_prev)
        elt->wl_prev->wl_next = list;
    if (list) {
        while (list->wl_next)
            list = list->wl_next;
        list->wl_next = elt->wl_next;
    }
    if (elt->wl_next)
        elt->wl_next->wl_prev = list;
    tfree(elt->wl_word);
    tfree(elt);
    return (list);
}



static void
printword(char *string, FILE *fp)
{
    if (string)
        while (*string)
            putc(strip(*string++), fp);
}


/* Print a word list. (No \n at the end...) */
void
wl_print(const wordlist *wl, FILE *fp)
{
    for (; wl; wl = wl->wl_next) {
        printword(wl->wl_word, fp);
        if (wl->wl_next)
            putc(' ', fp);
    }
}


/* Turn an array of char *'s into a wordlist. */
wordlist *
wl_build(char **v)
{
    wordlist *first = NULL;
    wordlist *last = NULL;

    while (*v)
        wl_append_word(&first, &last, copy(*v++));

    return (first);
}


char **
wl_mkvec(wordlist *wl)
{
    int  len = wl_length(wl);
    char **vec = TMALLOC(char *, len + 1);

    int i;

    for (i = 0; i < len; i++) {
        vec[i] = copy(wl->wl_word);
        wl = wl->wl_next;
    }
    vec[i] = NULL;

    return (vec);
}


/* Nconc two wordlists together. */
wordlist *
wl_append(wordlist *wlist, wordlist *nwl)
{
    wordlist *wl;
    if (wlist == NULL)
        return (nwl);
    if (nwl == NULL)
        return (wlist);
    for (wl = wlist; wl->wl_next; wl = wl->wl_next)
        ;
    wl->wl_next = nwl;
    nwl->wl_prev = wl;
    return (wlist);
}


/* Reverse a word list. */
wordlist *
wl_reverse(wordlist *wl)
{
    if (!wl)
        return (wl);

    for (;;) {
        wordlist *t = wl->wl_next;
        wl->wl_next = wl->wl_prev;
        wl->wl_prev = t;
        if (!t)
            return (wl);
        wl = t;
    }
}


/* Convert a wordlist into a string. */
char *
wl_flatten(wordlist *wlist)
{
    char *buf;
    wordlist *wl;
    size_t len = 0;

    for (wl = wlist; wl; wl = wl->wl_next)
        len += strlen(wl->wl_word) + 1;

    buf = TMALLOC(char, len + 1);
    *buf = '\0';

    for (wl = wlist; wl; wl = wl->wl_next) {
        (void) strcat(buf, wl->wl_word);
        if (wl->wl_next)
            (void) strcat(buf, " ");
    }
    return (buf);

}


/* Return the nth element of a wordlist, or the last one if n is too
 * big.  Numbering starts at 0...  */
wordlist *
wl_nthelem(int i, wordlist *wl)
{
    while ((i-- > 0) && wl->wl_next)
        wl = wl->wl_next;

    return (wl);
}


static int
wlcomp(const void *a, const void *b)
{
    const char **s = (const char **) a;
    const char **t = (const char **) b;
    return (strcmp(*s, *t));
}


void
wl_sort(wordlist *wl)
{
    size_t i = 0;
    wordlist *ww = wl;
    char **stuff;

    for (i = 0; ww; i++)
        ww = ww->wl_next;
    if (i < 2)
        return;
    stuff = TMALLOC(char *, i);
    for (i = 0, ww = wl; ww; i++, ww = ww->wl_next)
        stuff[i] = ww->wl_word;
    qsort(stuff, i, sizeof (char *), wlcomp);
    for (i = 0, ww = wl; ww; i++, ww = ww->wl_next)
        ww->wl_word = stuff[i];
    tfree(stuff);
}


/* Return a range of wordlist elements... */
wordlist *
wl_range(wordlist *wl, int low, int up)
{
    int i;
    wordlist *tt;
    //bool rev = FALSE;
    int rev = FALSE;

    if (low > up) {
        i = up;
        up = low;
        low = i;
        rev = TRUE;
    }
    up -= low;
    while (wl && (low > 0)) {
        tt = wl->wl_next;
        tfree(wl->wl_word);
        tfree(wl);
        wl = tt;
        if (wl)
            wl->wl_prev = NULL;
        low--;
    }
    tt = wl;
    while (tt && (up > 0)) {
        tt = tt->wl_next;
        up--;
    }
    if (tt && tt->wl_next) {
        wl_free(tt->wl_next);
        tt->wl_next = NULL;
    }
    if (rev)
        wl = wl_reverse(wl);
    return (wl);
}


/*
 *  prepend a new `word'
 *     to the front of the given `wlist' wordlist
 *  and return this new list
 */

wordlist *
wl_cons(char *word, wordlist *wlist)
{
    wordlist *w = alloc(wordlist);
    w->wl_next = wlist;
    w->wl_prev = NULL;
    w->wl_word = word;

    if (wlist)
        wlist->wl_prev = w;

    return (w);
}


/*
 * given a wordlist
 *   described by a `first' and `last' wordlist element
 * append a new `word'
 *   and update the given `first' and `last' pointers accordingly
 */

void
wl_append_word(wordlist **first, wordlist **last, char *word)
{
    wordlist *w = alloc(wordlist);
    w->wl_next = NULL;
    w->wl_prev = (*last);
    w->wl_word = word;

    if (*last)
        (*last)->wl_next = w;
    else
        (*first) = w;

    (*last) = w;
}


/*
 * given a pointer `wl' into a wordlist
 *   cut off this list from its preceding elements
 *   and return itself
 */

wordlist *
wl_chop(wordlist *wl)
{
    if (wl && wl->wl_prev) {
        wl->wl_prev->wl_next = NULL;
        wl->wl_prev = NULL;
    }
    return (wl);
}


/*
 * given a pointer `wl' into a wordlist
 *   cut off the rest of the list
 *   and return this rest
 */

wordlist *
wl_chop_rest(wordlist *wl)
{
    wordlist *rest = wl->wl_next;
    wl->wl_next = NULL;
    if(rest)
        rest->wl_prev = NULL;
    return (rest);
}


/*
 * search for a string in a wordlist
 */

wordlist *
wl_find(const char *string, const wordlist *wl)
{
    for (; wl; wl = wl->wl_next)
        if (eq(string, wl->wl_word))
            break;
    return ((wordlist *) wl);
}


/*
 * delete elements from a wordlist
 *   from inclusive `from'
 *   up to exclusive `to'
 */

void
wl_delete_slice(wordlist *from, wordlist *to)
{
    wordlist *prev;

    if (from == to)
        return;

    prev = from->wl_prev;

    if(prev)
        prev->wl_next = to;

    if (to) {
        to->wl_prev->wl_next = NULL;
        to->wl_prev = prev;
    }

    wl_free(from);
}
