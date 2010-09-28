#ifndef WIN_PTHREAD_H
#define WIN_PTHREAD_H

#include <setjmp.h>

/* private non-public types.  */
typedef struct _pthread_cleanup _pthread_cleanup;
struct _pthread_cleanup
{
    void (*func)(pthread_once_t *);
    void *arg;
    _pthread_cleanup *next;
};

struct _pthread_v
{
    void *ret_arg;
    void *(* func)(void *);
    _pthread_cleanup *clean;
    HANDLE h;
    int cancelled;
    unsigned p_state;
    unsigned int keymax;
    void **keyval;
    int tid;

    jmp_buf jb;
};

#define pthread_cleanup_push(F, A)\
{\
    const _pthread_cleanup _pthread_cup = {(F), (A), pthread_self()->clean};\
    _ReadWriteBarrier();\
    pthread_self()->clean = (_pthread_cleanup *) &_pthread_cup;\
    _ReadWriteBarrier()

/* Note that if async cancelling is used, then there is a race here */
#define pthread_cleanup_pop(E)\
    (pthread_self()->clean = _pthread_cup.next, (E?_pthread_cup.func((pthread_once_t *)_pthread_cup.arg):0));}

#endif
