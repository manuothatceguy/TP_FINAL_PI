#include <errno.h>

/**
 * Revisa la variable errno después de reservar memoria dinámica
*/
static int checkErrno(void * ptr);