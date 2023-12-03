#include <errno.h>

#ifndef CHECK_ERRNO_H
#define CHECK_ERRNO_H

/**
 * Revisa la variable errno después de reservar memoria dinámica
 * @return 1 si falló la asignación, 0 si no hubo errores
*/
static int checkErrno(void * ptr);

#endif