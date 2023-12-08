#ifndef CHECK_ERRNO_H
#define CHECK_ERRNO_H

/**
 * @brief Revisa la variable errno después de reservar memoria dinámica
 * 
 * Importante: setear errno con 0 previo a cada asignación
 * 
 * Ejemplo de uso:
 * 
 * ptr = alloc(asignación de memoria en el heap);
 * if(checkErrno(ptr)){
 *      // Que hacer si falla la asignación de memoria con la función de la familia alloc
 * }
 * 
 * @return 1 si falló la asignación, 0 si no hubo errores
*/
int checkErrno(void * ptr);

#endif
