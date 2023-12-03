#include "checkErrno.h"
#include <stdlib.h>
#include <errno.h>

int checkErrno(void * ptr){
    if(ptr == NULL || errno == ENOMEM){
        errno = ENOMEM;
        return 1;
    }
    return 0;
}