#ifndef THRD_RETVAL_TO_ERRNO_H
#define THRD_RETVAL_TO_ERRNO_H

#include <errno.h>

inline int thrdRetvalToErrno(const int thrd_ret)
{
    switch (thrd_ret)
    {
        case thrd_busy:
            return EBUSY;
        case thrd_error:
            return EINVAL;
        case thrd_nomem:
            return ENOMEM;
        default:
            return 0;
    }
}

#endif //THRD_RETVAL_TO_ERRNO_H
