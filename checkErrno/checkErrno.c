#include "checkErrno.h"
#include <stdlib.h>

static int checkErrno(void * ptr){
    if(ptr == NULL || errno == ENOMEM){
        errno = ENOMEM;
        return 1;
    }
    return 0;
}